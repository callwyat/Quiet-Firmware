from quiet_coms import find_quiet_ports
from quiet_tester import QuietTester
from quiet_tests import command_test
from serial import Serial
import serial.tools.list_ports
import time

if 'EXIT_ON_FAIL' not in locals():
    EXIT_ON_FAIL = True

if 'VERBOSE' not in locals():
    VERBOSE = True

class QuietUART(QuietTester):

    def __init__(self, coms) -> None:
        QuietTester.__init__(self, coms)

    def set_uart_baud_rate(self, rate:int):

        self.set_and_verify('UART:BAUD', rate, 0.1)

    def set_uart_mode(self, mode:str):

        if mode == 'USBU' or mode == 'SCPI':
            self.set_and_verify('UART:MODE', mode)
        else:
            raise Exception(f'Invalid mode specified {mode}. Valid modes are "USBU" and "SCPI"')

    def uart_write(self, data: bytearray):

        self.writeIEEE('UART:WRIT', data)

    def uart_read(self, count:int, timeout:float=1) -> bytearray:
        
        result = []
        timeout_time = time.time() + timeout

        while len(result) < count and time.time() < timeout_time:
            for b in self.queryIEEE("UART:READ?"):
                result.append(b)

        return bytearray(result)

    def uart_flush_read(self):

        self.queryIEEE("UART:READ?")

    def uart_error(self) -> int:
        error_int = self.quiet.query_int('UART:ERRO?')

        return error_int

def _uart_test(test_name:str, quiet:QuietUART, partner:Serial, test_string:str):

    test_bytes = test_string.encode()

    # Send with quiet
    quiet.uart_write(test_bytes)

    result = partner.read(len(test_bytes))

    if result != test_bytes:
        message = f'{test_name.ljust(16)} Failed. Sent: {test_string} Received: {result.encode()}'
        if EXIT_ON_FAIL:
            raise Exception(message)
        else:
            print(message)
    elif VERBOSE:
        print(f'{test_name.ljust(16)} => Pass')

    # Send with partner
    partner.write(test_bytes)

    result = quiet.uart_read(len(test_string))

    if result != test_bytes:
        message = f'{test_name.ljust(16)} Failed. Sent: {test_string} Received: {result}'
        if EXIT_ON_FAIL:
            raise Exception(message)
        else:
            print(message)
    elif VERBOSE:
        print(f'{test_name.ljust(16)} <= Pass')

def _uart_baud_test(quiet:QuietUART, partner_port:str):

    quiet.reset()
    quiet.uart_flush_read()
    
    print('Starting Baud Rate Tests')
    # TODO: Figure out why 115200 isn't working
    test_rates = [2400, 4800, 9600, 19200, 38400, 57500]

    for rate in test_rates:
        quiet.set_uart_baud_rate(rate)
        with Serial(partner_port, rate) as partner:

            _uart_test(f'Baud {rate}', quiet, partner, 'Hello World')

    print('Completed Baud Rate Tests')

def _uart_scpi_test(quiet:QuietUART, partner_port:str):

    quiet.reset()

    test_rate = 57600
    quiet.set_uart_baud_rate(test_rate)
    quiet.set_uart_mode('SCPI')

    with Serial(partner_port, test_rate, timeout=2) as partner:
        command_test(QuietTester(partner), all_commands=False)


def uart_dual_test(quiet:QuietUART, partner_port:str):

    print('Starting UART Test with partner')

    _uart_baud_test(quiet, partner_port)

    _uart_scpi_test(quiet, partner_port)


def uart_loopback_test(quiet:QuietTester):
    print('UART Tests')

    quiet.write('UART:BAUD 115200')
    
    data = ''
    for i in range(0, 64):

        data += 'U'
        data_size = str(len(data))
        header_size = len(data_size)

        packet = f'#{header_size}{data_size}{data}'
        quiet.write(f'UART:WRIT {packet}')

        time.sleep(0.001 * i)
        
        response = quiet.query_raw('UART:READ?')

        if response != packet + '\r\n':
            fail_string = f'UART Test Failed at: {i}'
            print(fail_string)
            if EXIT_ON_FAIL:
                raise Exception(fail_string)

    print('UART Test Complete')


if __name__ == "__main__":

    port = find_quiet_ports()[0]

    partner_port = None
    for p in serial.tools.list_ports.comports():
        if p.manufacturer and 'FTDI' in p.manufacturer:
            partner_port = p.device

    if partner_port:
        uart_dual_test(QuietUART(port), partner_port)
    else:
        uart_loopback_test(port)

    print('All Test Complete')