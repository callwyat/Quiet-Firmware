import serial
import serial.tools.list_ports
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
        error_int = self.quiet.query_int('IIC:EERO')

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

def i2c_test(i: quiet_i2c) -> bool:

    # Get the communications setup
    if type(i) == quiet_i2c:
        pass
    elif type(i) == quiet_coms.quiet_coms:
        i = quiet_i2c(qCom)
    elif type(i) == serial.Serial:
        i = quiet_i2c(quiet_i2c(i))

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
    ports = list(serial.tools.list_ports.comports())
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            qPort = p.device
            break

    com = serial.Serial(port=qPort, timeout=30)

    qCom = quiet_coms.quiet_coms(com)

    i2c_test(qCom)

    print('All I2C Tests Passed')

