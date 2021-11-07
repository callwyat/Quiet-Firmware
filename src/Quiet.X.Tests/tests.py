

import serial
import re

QUIET_TERMINATION = '\r\n'
BOOL_PATTERN = '\\b[01]\\b'
HEX8_PATTERN = '\\b0x[0-9a-fA-F]{2}\\b'
INT16_PATTERN = '\\b[\\d]{1,5}\\b'
OUTPUT_MODE_PATTERN = '\\b[DISC|PWM|SERV]\\b'

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


ESCAPES = {
    '\r' : '\\r',
    '\n' : '\\n',
}

def show_escapes(a):

    for key in ESCAPES:
        a = a.replace(key, ESCAPES[key])

    return a

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

        QueryChannelTest('PWMO:CH#?', 1, 10, INT16_PATTERN),
        QueryChannelTest('PWMO:CH#:VALU?', 1, 10, INT16_PATTERN),
        QueryChannelTest('SERO:CH#:MODE?', 1, 10, OUTPUT_MODE_PATTERN), 

        QueryChannelTest('SERO:CH#?', 1, 10, INT16_PATTERN),
        QueryChannelTest('SERO:CH#:VALU?', 1, 10, INT16_PATTERN),
        QueryChannelTest('SERO:CH#:MODE?', 1, 10, OUTPUT_MODE_PATTERN), 
    ]

    for test in queryTests:

        for command in test.commands:

            coms.write(command.encode())

            response = coms.read_until().decode()
            test_result = test.check_response(response)
            
            if verbose:
                print(f"{command.strip().ljust(16)} =>   {response.strip()}")

            if not test_result:
                print(f"Test Failed\nSent:     \"{show_escapes(command)}\"\n" +
                f"Expected: \"{show_escapes(test.response)}\"\n" +
                f"Received: \"{show_escapes(response)}\"")

                if exit_on_fail:
                    return False

    # TODO: Test the manipulation of settings

    return True



if __name__ == "__main__":

    com = serial.Serial(port="/dev/tty.usbmodem142401", timeout=1)

    if (run_quiet_test(com, verbose=True, exit_on_fail=True)):
        print("All Tests Passed")

    

