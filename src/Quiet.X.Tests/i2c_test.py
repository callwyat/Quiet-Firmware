from quiet_coms import find_quiet_ports
from quiet import Quiet
import time

if 'EXIT_ON_FAIL' not in locals():
    VERBOSE = True
    EXIT_ON_FAIL = True
class QuietI2C(Quiet):

    def __init__(self, coms, **kargs) -> None:
        Quiet.__init__(self, coms, **kargs)

    def raw_write(self, addr: int, data: bytearray):

        command = f'IIC:ADDR {addr};WRIT'
        self.writeIEEE(command, data)

    def raw_read(self, addr: int, readCount: int) -> bytearray:
        
        message = f'IIC:ADDR {addr};READ? {readCount}'

        return self.queryIEEE(message)

    def register_write(self, address: int, register:int, data: int, dataSize=2):
        self.write(f'IIC:ADDR {address};REGI:ADDR {register};RSIZ {dataSize};WRIT {data}')

    def register_read(self, address: int, register:int, dataSize=1) -> int:
        return self.query_int(f'IIC:ADDR {address};REGI:ADDR {register};RSIZ {dataSize};READ?')

    def enable_master_mode(self) -> None:
        self.set_and_verify('IIC:MODE', 'MAST')

    def disable(self) -> None:
        self.set_and_verify('IIC:MODE', 'OFF')

    def acknowledged(self) -> bool:
        ack = self.query('IIC:ACK?')
        return '1' in ack

def _i2c_register_read_test(i: QuietI2C, address:int, register:int, expectation:int):
    value = i.register_read(address, register, 2)

    if value != expectation:
        message = f'Failure at {hex(address)}:{hex(register)}. Expected {hex(expectation)} but read {hex(value)}'
        
        if EXIT_ON_FAIL:
            raise Exception()
        else:
            print(message)
    elif VERBOSE:
        print(f'REG\t{hex(address)}:{hex(register)} -> {hex(value)}')

def _i2c_register_write_test(i: QuietI2C, address:int, register:int, value:int):

    i.register_write(address, register, value, 2)
    time.sleep(0.1)
    _i2c_register_read_test(i, address, register, value)

def _i2c_raw_write(i: QuietI2C, address:int, data:bytearray):

    i.raw_write(address, data)

    if VERBOSE:
        print(f'RAW\t{hex(address)} -> {str(data)[10:].strip(")")}')

def _i2c_raw_read_test(i: QuietI2C, address:int, expected:bytearray):

    response = i.raw_read(address, len(expected))

    if response != expected:
        message = f'Failure at {hex(address)}. Expected {expected} but read {response}'
        if EXIT_ON_FAIL:
            raise Exception(message)
        else:
            print(message)
    elif VERBOSE:
        print(f'RAW\t{hex(address)} <- {response}')

def _i2c_check_error(i: QuietI2C, error_name: str, expectation: int):

    error = i.error()

    if error != expectation:
        message = f'Failure {error_name}. Expected {hex(expectation)} received {hex(error)}'
        if EXIT_ON_FAIL:
            raise Exception(message)
        else:
            print(message)
    elif VERBOSE:
        print(f'{error_name.ljust(32)} Pass')


def _i2c_check_lower_limit(i: QuietI2C, command:str, low:int, error_name:str, error_code, delay:int=0):
    
    under = low - 1
    i.write(f'{command} {under}')

    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'UNDER {error_name}', error_code if under >= 0 else 0x0110)

    i.write(f'{command} {low}')
    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'LOWER {error_name}', 0x00)

def _i2c_check_upper_limit(i: QuietI2C, command:str, high:int, error_name:str, error_code, delay:int=0):

    i.write(f'{command} {high}')
    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'UPPER {error_name}', 0x00)

    i.write(f'{command} {high + 1}')
    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'OVER  {error_name}', error_code)

def _i2c_check_limit(i: QuietI2C, command:str, low:int, high:int, error_name:str, error_code):

    _i2c_check_lower_limit(i, command, low, error_name, error_code)

    _i2c_check_upper_limit(i, command, high, error_name, error_code)

def _i2c_check_acknowledge(i, expectation:bool):

        ack = i.acknowledged()
        if ack != expectation:
            if ack:
                message = f'Failure ACKNOWLEDGED. Expected NO_ACKNOWLEDGED received ACKNOWLEDGED'
            else:
                message = f'Failure ACKNOWLEDGED. Expected ACKNOWLEDGED received NO_ACKNOWLEDGED'

            if EXIT_ON_FAIL:
                raise Exception(message)
            else:
                print(message)
        elif VERBOSE:
            print(f'{("" if ack else "NO_")}ACKNOWLEDGED'.ljust(32) + ' Pass')

def i2c_test_errors(i: QuietI2C) -> bool:

    # Clear Errors
    i.error()

    # Verify the second hook works
    if i.query_int('SYST:ERR?') != 0:
        messsage = 'Failure "SYS:ERR?" Command'
        if EXIT_ON_FAIL:
            raise Exception(messsage)
        else:
            print(messsage)
    elif VERBOSE:
        print('IIC:REGI:ERRO?                   Pass')
    
    i.disable()
    _i2c_check_error(i, 'ERROR_NONE', 0x00)

    _i2c_check_limit(i, 'IIC:BAUD', 16000, 1000000, 'INVALID_BAUD', 0x0B01)

    _i2c_check_limit(i, 'IIC:TIME', 1, 255, 'INVALID_TIMEOUT', 0x0B02)

    _i2c_check_limit(i, 'IIC:ADDR', 0, 127, 'INVALID_ADDRESS', 0x0B03)
 
    i.write('IIC:MODE MAS')
    _i2c_check_error(i, 'ERROR_INVALID_MODE', 0x0B04)
    
    _i2c_check_limit(i, 'IIC:REGI:RSIZ', 1, 2, 'INVALID_RSIZE', 0x0B20)

    _i2c_check_limit(i, 'IIC:REGI:ADDR', 0, 255, 'INVALID_REGISTER_ADDRESS', 0x0B21)

    i.write('IIC:REGI:WRIT 1')
    _i2c_check_error(i, 'ERROR_DISABLED_WRITE', 0x0B10)

    i.query('IIC:REGI:READ?')
    i.com.flushInput()
    _i2c_check_error(i, 'ERROR_DISABLED_READ', 0x0B11)
    
    i.write('IIC:WRIT #11A')
    _i2c_check_error(i, 'ERROR_DISABLED_WRITE', 0x0B10)

    i.query('IIC:READ? 2')
    _i2c_check_error(i, 'ERROR_DISABLED_READ', 0x0B11)

    i.reset()
    i.enable_master_mode()

    try:
        i.write('IIC:ADDR 0x50;REGI:ADDR 0xFF;RSIZ 1')
        i.com.flushInput()
        _i2c_check_upper_limit(i, 'IIC:REGI:WRIT', 255, 'INVALID_REGISTER_VALUE', 0x0B22, 0.1)

        i.write('IIC:WRIT #10')
        i.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_INVALID_WRITE_SIZE', 0x0B31)

        i.write('IIC:READ? 0')
        i.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_INVALID_READ_SIZE', 0x0B32)

        i.write('IIC:WRIT #2520AAAAAAAAA1BBBBBBBBB2CCCCCCCCC3DDDDDDDDD4EEEEEEEEE5F')
        i.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_INVALID_WRITE_SIZE', 0x0B31)

        i.query('IIC:READ? 64')
        i.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_INVALID_READ_SIZE', 0x0B32)

        i.write('IIC:READ?')
        i.com.flushInput()
        _i2c_check_error(i, 'I2C_ERROR_INVALID_READ_SYNTAX', 0x0B33)

        i.write('IIC:ADDR 0x10;WRIT #13ABC')
        time.sleep(0.1)
        _i2c_check_acknowledge(i, False)


    finally:
        i.disable()

def i2c_test(i: QuietI2C) -> bool:

    i.reset()

    i.enable_master_mode()
    try:
        _i2c_register_read_test(i, 0x50, 0xFE, 0x5449)
        _i2c_register_read_test(i, 0x50, 0xFF, 0x1004)

        _i2c_register_write_test(i, 0x50, 0x0C, 0x05AA)
        _i2c_register_write_test(i, 0x50, 0x08, 0x1E00)
        _i2c_register_write_test(i, 0x50, 0x0A, 0x5F80)

        _i2c_raw_write(i, 0x50, bytearray([0xFF]))
        _i2c_raw_read_test(i, 0x50, bytearray([0x10, 0x04]))

        _i2c_raw_write(i, 0x50, bytearray([0x0C, 0x05, 0xA0]))
        _i2c_raw_write(i, 0x50, bytearray([0x0C]))
        _i2c_raw_read_test(i, 0x50, bytearray([0x05, 0xA0]))

    finally:
        i.disable()

if __name__ == "__main__":

    q2c = QuietI2C(None, log_path='usb_log.txt')

    i2c_test(q2c)

    i2c_test_errors(q2c)

    i2c_test(q2c)
    
    print('All I2C Tests Passed')

