

import serial
import serial.tools.list_ports
import re

QUIET_TERMINATION = '\r\n'
BOOL_PATTERN = '\\b[01]\\b'
HEX8_PATTERN = '\\b0x[0-9a-fA-F]{2}\\b'
INT16_PATTERN = '\\b[\\d]{1,5}\\b'
OUTPUT_MODE_PATTERN = '\\b(DISC|PWM|SERV)\\b'

class QueryTest():

    def __init__(self, command, response):
        super().__init__()

        self.commands = [command + QUIET_TERMINATION]
        self.response = f'{response}({QUIET_TERMINATION})'

    def check_response(self, response):
        return re.search(self.response, response)

class QueryChannelTest():

    def __init__(self, command, start, stop, response):
        super().__init__()

        self.commands = []
        for i in range(start, stop + 1):
            self.commands.append(command.replace('#', str(i)) + QUIET_TERMINATION)

        self.response = f'{response}({QUIET_TERMINATION})'

    def check_response(self, response):
        return re.search(self.response, response)

def run_quiet_test(coms, verbose=False, exit_on_fail=True):

    # TODO: Apply default settings to the UUT

    queryTests = [
        QueryTest('*IDN?', 'Y@ Technologies,Qy@ Board,\\{Serial Number\\},[2-9]\\.[0-9]'),

        QueryTest('DIGI?', HEX8_PATTERN),

        QueryChannelTest('ANAI:CH#?', 1, 4, INT16_PATTERN),

        QueryTest('DIGO?', HEX8_PATTERN),
        QueryChannelTest('DIGO:CH#?', 1, 8, INT16_PATTERN),
        QueryChannelTest('DIGO:CH#:VALU?', 1, 8, BOOL_PATTERN),
        QueryChannelTest('DIGO:CH#:MODE?', 1, 8, '\\bDISC\\b'),

        QueryChannelTest('ANAO:CH#?', 1, 2, INT16_PATTERN),
        QueryChannelTest('ANAO:CH#:VALU?', 1, 2, INT16_PATTERN),
        QueryChannelTest('ANAO:CH#:MODE?', 1, 2, '\\bPWM\\b'),

        QueryChannelTest('PWMO:CH#?', 1, 6, INT16_PATTERN),
        QueryChannelTest('PWMO:CH#:VALU?', 1, 6, INT16_PATTERN),
        QueryChannelTest('PWMO:CH#:MODE?', 1, 6, OUTPUT_MODE_PATTERN), 

        QueryChannelTest('SERV:CH#?', 1, 10, INT16_PATTERN),
        QueryChannelTest('SERV:CH#:VALU?', 1, 10, INT16_PATTERN),
        QueryChannelTest('SERV:CH#:MODE?', 1, 10, OUTPUT_MODE_PATTERN), 
    ]

    print('Starting Command Tests')

    for test in queryTests:

        for command in test.commands:

            coms.write(command.encode())

            response = coms.read_until().decode()
            test_result = test.check_response(response)
            
            if verbose:
                print(f"{command.strip().ljust(16)} =>   {response.strip()}")

            if not test_result:
                print(f"Test Failed\nSent:     {repr(command)}\n" +
                f"Expected: {repr(test.response)}\n" +
                f"Received: {repr(response)}")

                if exit_on_fail:
                    return False

    print('Command Tests Passed')
    print('Starting Parse Test')

    # Test the parsing method
    for val in range(0, 1024):

        if verbose:
            print(f'Testing             => {val}')

        command = f'ANAO:CH1 {val};CH1?\r\n'
        com.write(command.encode())
        response = coms.read_until().decode()

        expected = f',{val}\r\n'
        if response != expected:
            print(f"Test Failed\nSent:     {repr(command)}\n" +
            f"Expected: {repr(expected)}\n" +
            f"Received: {repr(response)}")

        command = f'ANAO:CH1 {hex(val)};CH1?\r\n'
        com.write(command.encode())
        response = coms.read_until().decode()

        if response != expected:
            print(f"Test Failed\nSent:     {repr(command)}\n" +
            f"Expected: {repr(expected)}\n" +
            f"Received: {repr(response)}")

    print('Parse Test Passed')

    # TODO: Test the manipulation of settings

    return True



if __name__ == "__main__":
    ports = list(serial.tools.list_ports.comports())
    
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            qPort = p.device
            break

    com = serial.Serial(port=qPort, timeout=1)

    if (run_quiet_test(com, verbose=False, exit_on_fail=True)):
        print("All Tests Passed")

    

