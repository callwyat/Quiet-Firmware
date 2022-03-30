import quiet_coms
import time

if 'EXIT_ON_FAIL' not in locals():
    VERBOSE = True
    EXIT_ON_FAIL = True
class quiet_i2c():

    def __init__(self, quiet:quiet_coms.quiet_coms) -> None:

        self.quiet = quiet
        self.com = quiet.com

    def raw_write(self, addr: int, data: bytearray):

        command = f'IIC:ADDR {addr};WRIT'
        self.quiet.writeIEEE(command, data)

    def raw_read(self, addr: int, readCount: int) -> bytearray:
        
        message = f'IIC:ADDR {addr};READ? {readCount}'

        return self.quiet.queryIEEE(message)

    def register_write(self, address: int, register:int, data: int, dataSize=2):
        self.quiet.write(f'IIC:ADDR {address};REGI:ADDR {register};RSIZ {dataSize};WRIT {data}')

    def register_read(self, address: int, register:int, dataSize=1) -> int:
        return self.quiet.query_int(f'IIC:ADDR {address};REGI:ADDR {register};RSIZ {dataSize};READ?')

    def enable(self) -> None:
        self.quiet.write('IIC:ENAB 1')

    def disable(self) -> None:
        self.quiet.write('IIC:ENAB 0')

    def error(self) -> int:
        error_int = self.quiet.query_int('IIC:ERRO?')

        return error_int

def _i2c_register_read_test(i: quiet_i2c, address:int, register:int, expectation:int):
    value = i.register_read(address, register, 2)


    if value != expectation:
        message = f'Failure at {hex(address)}:{hex(register)}. Expected {hex(expectation)} but read {hex(value)}'
        
        if EXIT_ON_FAIL:
            raise Exception()
        else:
            print(message)
    elif VERBOSE:
        print(f'REG\t{hex(address)}:{hex(register)} -> {hex(value)}')

def _i2c_register_write_test(i: quiet_i2c, address:int, register:int, value:int):

    i.register_write(address, register, value, 2)
    time.sleep(0.1)
    _i2c_register_read_test(i, address, register, value)

def _i2c_raw_write(i: quiet_i2c, address:int, data:bytearray):

    i.raw_write(address, data)

    if VERBOSE:
        print(f'RAW\t{hex(address)} -> {str(data)[10:].strip(")")}')

def _i2c_raw_read_test(i: quiet_i2c, address:int, expected:bytearray):

    response = i.raw_read(address, len(expected))

    if response != expected:
        message = f'Failure at {hex(address)}. Expected {expected} but read {response}'
        if EXIT_ON_FAIL:
            raise Exception(message)
        else:
            print(message)
    elif VERBOSE:
        print(f'RAW\t{hex(address)} <- {response}')

def _i2c_check_error(i: quiet_i2c, error_name: str, expectation: int):

    error = i.error()

    if error != expectation:
        message = f'Failure {error_name}. Expected {hex(expectation)} received {hex(error)}'
        if EXIT_ON_FAIL:
            raise Exception(message)
        else:
            print(message)
    elif VERBOSE:
        print(f'{error_name.ljust(32)} Pass')


def _i2c_check_lower_limit(i: quiet_i2c, command:str, low:int, error_name:str, error_code, delay:int=0):
    
    i.quiet.write(f'{command} {low - 1}')
    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'UNDER {error_name}', error_code)

    i.quiet.write(f'{command} {low}')
    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'LOWER {error_name}', 0x00)

def _i2c_check_upper_limit(i: quiet_i2c, command:str, high:int, error_name:str, error_code, delay:int=0):

    i.quiet.write(f'{command} {high}')
    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'UPPER {error_name}', 0x00)

    i.quiet.write(f'{command} {high + 1}')
    if delay > 0:
        time.sleep(delay)

    _i2c_check_error(i, f'OVER  {error_name}', error_code)

def _i2c_check_limit(i: quiet_i2c, command:str, low:int, high:int, error_name:str, error_code):

    _i2c_check_lower_limit(i, command, low, error_name, error_code)

    _i2c_check_upper_limit(i, command, high, error_name, error_code)

def i2c_test_errors(i: quiet_i2c) -> bool:

    # Clear Errors
    i.error()

    # Verify the second hook works
    if i.quiet.query_int('IIC:REGI:ERRO?') != 0:
        messsage = 'Failure "IIC:REGI:ERRO?" Command'
        if EXIT_ON_FAIL:
            raise Exception(messsage)
        else:
            print(messsage)
    elif VERBOSE:
        print('IIC:REGI:ERRO?                   Pass')
    
    i.disable()
    _i2c_check_error(i, 'ERROR_NONE', 0x00)

    _i2c_check_limit(i, 'IIC:BAUD', 16000, 1000000, 'INVALID_BAUD', 0x01)

    _i2c_check_limit(i, 'IIC:TIME', 1, 255, 'INVALID_TIMEOUT', 0x02)

    _i2c_check_limit(i, 'IIC:ADDR', 0, 127, 'INVALID_ADDRESS', 0x03)

    _i2c_check_limit(i, 'IIC:REGI:RSIZ', 1, 2, 'INVALID_RSIZE', 0x20)

    _i2c_check_limit(i, 'IIC:REGI:ADDR', 0, 255, 'INVALID_REGISTER_ADDRESS', 0x21)

    i.quiet.write('IIC:REGI:WRIT 1')
    _i2c_check_error(i, 'ERROR_DISABLED_WRITE', 0x10)

    i.quiet.query('IIC:REGI:READ?')
    i.quiet.com.flushInput()
    _i2c_check_error(i, 'ERROR_DISABLED_READ', 0x11)
    
    i.quiet.write('IIC:WRIT #11A')
    _i2c_check_error(i, 'ERROR_DISABLED_WRITE', 0x10)

    i.quiet.query('IIC:READ? 2')
    _i2c_check_error(i, 'ERROR_DISABLED_READ', 0x11)

    i.quiet.write('*RST')
    i.enable()

    try:
        i.quiet.write('IIC:ADDR 0x50;REGI:ADDR 0xFF;RSIZ 1')
        i.quiet.com.flushInput()
        _i2c_check_upper_limit(i, 'IIC:REGI:WRIT', 255, 'INVALID_REGISTER_VALUE', 0x22, 0.1)

        i.quiet.write('IIC:WRIT #10')
        i.quiet.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_INVALID_WRITE_SIZE', 0x31)

        i.quiet.write('IIC:READ? 0')
        i.quiet.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_INVALID_READ_SIZE', 0x32)

        i.quiet.write('IIC:WRIT #2520AAAAAAAAA1BBBBBBBBB2CCCCCCCCC3DDDDDDDDD4EEEEEEEEE5F')
        i.quiet.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_BUFFER_OVERFLOW', 0x30)

        i.quiet.query('IIC:READ? 64')
        i.quiet.com.flushInput()
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_BUFFER_OVERFLOW', 0x30)

        i.quiet.write('IIC:ADDR 0x10;WRIT #13ABC')
        time.sleep(0.1)
        _i2c_check_error(i, 'I2C_ERROR_NO_ACKNOWLEDGE', 0x12)

    finally:
        i.disable()

def i2c_test(i: quiet_i2c) -> bool:

    # Get the communications setup
    if type(i) == quiet_i2c:
        pass
    elif type(i) == quiet_coms.quiet_coms:
        i = quiet_i2c(i)
    else:
        raise Exception('Unsure what to do with "i"')

    i.quiet.write('*RST')

    i.enable()
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

    qPorts = quiet_coms.find_quiet_ports()

    quite = quiet_coms.quiet_coms(port=qPorts[0])

    q2c = quiet_i2c(quite)

    i2c_test(q2c)

    i2c_test_errors(q2c)

    i2c_test(q2c)
    
    print('All I2C Tests Passed')

