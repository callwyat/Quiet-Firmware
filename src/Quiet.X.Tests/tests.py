

import serial
import serial.tools.list_ports
import re
import time

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

def print_fail_message(command, expected, response):
    print(f"Test Failed\nSent:     {repr(command)}\n" +
          f"Expected: {repr(expected)}\n" +
          f"Received: {repr(response)}")


def output_test(com, command, count, mode, value, verbose=False, exit_on_fail=True):
    for i in range(1, count + 1):
        full_command = f'{command}:CH{i}:MODE {mode};VALUE {value};MODE?'
        com.write(full_command.encode())
        
        expected = f',,{mode}\r\n'
        response = com.read_until().decode()
        if response != expected:
            print_fail_message(full_command, expected, response)
            if exit_on_fail:
                return False

        if verbose:
            print(f'{command}:CH{i}'.ljust(10) + f' => {response.strip()}')

        time.sleep(1)


def run_quiet_test(coms, verbose=False, exit_on_fail=True):

    # TODO: Apply default settings to the UUT

    queryTests = [
        QueryTest('*IDN?', 'Y@ Technologies,Qy@ Board,.*?,[2-9]\\.[0-9]'),

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

            if not test_result:
                print(f"Test Failed\nSent:     {repr(command)}\n" +
                f"Expected: {repr(test.response)}\n" +
                f"Received: {repr(response)}")

                if exit_on_fail:
                    return False

            else:
                # Get the time to execute the command
                coms.write('DIAG?'.encode())
                execution_time = coms.read_until().decode().strip()

                if verbose:
                    print(f"{command.strip().ljust(16)} ( {execution_time.ljust(5)} ) =>   {response.strip()}")

    print('Command Tests Passed')
    print('Starting Parse Test')

    # Test the parsing method
    for val in range(0, 1024, 3):

        if verbose:
            print(f'Testing             => {val}')

        command = f'ANAO:CH1 {val};CH1?\r\n'
        com.write(command.encode())
        response = coms.read_until().decode()

        expected = f',{val}\r\n'
        if response != expected:
            print_fail_message(command, expected, response)
            if exit_on_fail:
                    return False

        command = f'ANAO:CH1 {hex(val)};CH1?\r\n'
        com.write(command.encode())
        response = coms.read_until().decode()

        if response != expected:
            print_fail_message(command, expected, response)
            if exit_on_fail:
                    return False

    print('Parse Test Passed')

    print('Outputs Test')

    output_test(com, 'SERV', 10, 'SERV', '0x3FF', verbose)

    output_test(com, 'PWMO', 6, 'PWM', '0x3FF', verbose)

    output_test(com, 'DIGO', 8, 'DISC', 0, verbose)

    print('Outputs Test Complete')

    print('UART Tests')

    com.write('UART:BAUD 115200'.encode())
    
    data = ''
    for i in range(0, 64):

        data += 'U'
        data_size = str(len(data))
        header_size = len(data_size)

        packet = f'#{header_size}{data_size}{data}'
        com.write(f'UART:WRIT {packet}\r\n'.encode())

        time.sleep(0.001 * i)
        com.write('UART:READ?\r\n'.encode())
        response = coms.read_until().decode()

        if response != packet + '\r\n':
            print(f'UART Test Failed at: {i}')
            if exit_on_fail:
                    return False

    print('UART Test Complete')
    # TODO: Test the manipulation of settings

    return True



if __name__ == "__main__":
    ports = list(serial.tools.list_ports.comports())
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            qPort = p.device
            break

    com = serial.Serial(port=qPort, timeout=1)

    if  run_quiet_test(com, verbose=True, exit_on_fail=True):
        print("All Tests Passed")

    

