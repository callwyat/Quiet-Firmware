

import serial
import serial.tools.list_ports
import re
import time

QUIET_TERMINATION = '\r\n'
BOOL_PATTERN = '\\b[01]\\b'
HEX8_PATTERN = "\\b0[xX]([0-9a-fA-F]{2})\\b"
HEX16_PATTERN = "\\b0[xX]([0-9a-fA-F]{4}|[0-9a-fA-F]{2})\\b"
HEX24_PATTERN = "\\b0[xX]([0-9a-fA-F]{6}|[0-9a-fA-F]{4}|[0-9a-fA-F]{2})\\b"
INT16_PATTERN = '\\b[\\d]{1,5}\\b'
OUTPUT_MODE_PATTERN = '\\b(DISC|PWM|SERV)\\b'

VERBOSE = True
EXIT_ON_FAIL = True

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

def generate_fail_message(command, expected, response):
    return (f"Test Failed\nSent:     {repr(command)}\n" +
          f"Expected: {repr(expected)}\n" +
          f"Received: {repr(response)}")

def print_fail_message(command, expected, response):
    print(generate_fail_message(command, expected, response))

def output_test(com, command, count, mode, value):
    for i in range(1, count + 1):
        full_command = f'{command}:CH{i}:MODE {mode};VALUE {value};MODE?\r\n'
        com.write(full_command.encode())
        
        expected = f';;{mode}\r\n'
        response = com.read_until()
        response = response.decode()
        if response != expected:
            fail_string = generate_fail_message(full_command, expected, response)
            print(fail_string)
            if EXIT_ON_FAIL:
                raise Exception(fail_string)

        if VERBOSE:
            print(f'{command}:CH{i}'.ljust(10) + f' => {response.strip()}')

        time.sleep(1)



def command_test(com, number_mode='DECI'):

    if number_mode == 'DECI' or number_mode == 'HEX':
        # Changed the number mode
        com.write(f'SYST:NUMB {number_mode}\r\n'.encode())
    else:
        raise Exception(f'Invalid Number mode {number_mode}. Must be \'DECI\' or \'HEX\'')

    number_pattern_16 = HEX16_PATTERN if number_mode == 'HEX' else INT16_PATTERN
    number_pattern_8 = HEX8_PATTERN if number_mode == 'HEX' else INT16_PATTERN

    queryTests = [
        QueryTest('*IDN?', 'Y@ Technologies,Qy@ Board,.*?,[2-9]\\.[0-9]'),

        QueryTest('DIGI?', number_pattern_8),
        QueryTest('DIGInputs?', number_pattern_8),

        QueryChannelTest('ANAI:CH#?', 1, 4, number_pattern_16),

        QueryTest('DIGOutputs?', number_pattern_8),
        QueryChannelTest('DIGO:CH#?', 1, 8, number_pattern_16),
        QueryChannelTest('DIGO:CH#:VALU?', 1, 8, number_pattern_16),
        QueryChannelTest('DIGO:CH#:MODE?', 1, 8, '\\bDISC\\b'),

        QueryChannelTest('ANAO:CH#?', 1, 2, number_pattern_16),
        QueryChannelTest('ANAO:CH#:VALU?', 1, 2, number_pattern_16),
        QueryChannelTest('ANAO:CH#:MODE?', 1, 2, '\\bPWM\\b'),

        QueryChannelTest('PWM:CH#?', 1, 6, number_pattern_16),
        QueryChannelTest('PWM:CH#:VALU?', 1, 6, number_pattern_16),
        QueryChannelTest('PWM:CH#:MODE?', 1, 6, OUTPUT_MODE_PATTERN), 

        QueryChannelTest('SERV:CH#?', 1, 10, number_pattern_16),
        QueryChannelTest('SERV:CH#:VALU?', 1, 10, number_pattern_16),
        QueryChannelTest('SERVo:CH#:MODE?', 1, 10, OUTPUT_MODE_PATTERN), 
    ]

    print('Starting Command Tests')

    for test in queryTests:

        for command in test.commands:

            com.write(command.encode())

            response = com.read_until().decode()
            test_result = test.check_response(response)

            if not test_result:
                print_fail_message(command, test.response, response)

                if EXIT_ON_FAIL:
                    raise Exception(generate_fail_message(command, test.response, response))

            else:
                # Get the time to execute the command
                com.write('DIAG?\r\n'.encode())
                value_string = com.read_until().decode().strip()
                if number_mode == 'HEX':
                    execution_time = int(value_string[2:], 16)
                else:
                    execution_time = int(value_string, 10)

                if VERBOSE:
                    print(f"{command.strip().ljust(16)} ( {str(execution_time).ljust(5)} ) =>   {response.strip()}")

    print('Command Tests Passed')

def analog_stability_test(com, number_mode='DECI'):

    if number_mode == 'DECI' or number_mode == 'HEX':
        # Changed the number mode
        com.write(f'SYST:NUMB {number_mode}\r\n'.encode())
    else:
        raise Exception(f'Invalid Number mode {number_mode}. Must be \'DECI\' or \'HEX\'')

    print('Analog Input Stability Test')

    aMax = [0, 0, 0, 0]
    aMin = [ 10000, 10000, 10000, 10000]
    for i in range(1, 10000):

        com.write('ANAI:CH1?;CH2?;CH3?;CH4?\r\n'.encode())
        response_raw = com.read_until().decode().strip().split(';')

        i = 0
        for response in response_raw:
            r = int(response[2:], 16) if number_mode == 'HEX' else int(response, 10)

            if r < aMin[i]:
                aMin[i] = r
            if r > aMax[i]:
                aMax[i] = r

            i += 1

    for i in range(0, 4):
        print(f'{i + 1}  =>  Max: ' + str(aMax[i]).ljust(8) +  
        f'Min: ' + str(aMin[i]).ljust(8) + f'Range: {aMax[i] - aMin[i]}')

    print('Analog Input Stability Test Complete')

def parse_test(com, number_mode='DECI'):
    
    if number_mode == 'DECI' or number_mode == 'HEX':
        # Changed the number mode
        com.write(f'SYST:NUMB {number_mode}\r\n'.encode())
    else:
        raise Exception(f'Invalid Number mode {number_mode}. Must be \'DECI\' or \'HEX\'')

    print('Starting Parse Test')

    if number_mode == 'DECI':
        # Test the parsing method
        for val in range(0, 1024, 3):

            if VERBOSE:
                print(f'Testing             => {val}')

            command = f'ANAO:CH1 {val};CH1?\r\n'
            com.write(command.encode())
            response = com.read_until().decode()

            expected = f';{val}\r\n'
            if response != expected:
                fail_string = generate_fail_message(command, expected, response)
                print(fail_string)
                if EXIT_ON_FAIL:
                    raise Exception(fail_string)

    else:
        # Test the parsing method
        for val in range(0, 1024, 3):

            if VERBOSE:
                print(f'Testing             => {hex(val)}')

            command = f'ANAO:CH1 {hex(val)};CH1?\r\n'
            com.write(command.encode())
            response = com.read_until().decode()

            expected = f';0x{format(val, "02X" if val <= 0xFF else "04X")}\r\n'
            if response != expected:
                fail_string = generate_fail_message(command, expected, response)
                print(fail_string)
                if EXIT_ON_FAIL:
                    raise Exception(fail_string)

    print('Parse Test Passed')

def output_mode_test(com):
    print('Outputs Test')

    output_test(com, 'SERV', 10, 'SERV', '0x3FF')

    output_test(com, 'PWM', 6, 'PWM', '0x3FF')

    output_test(com, 'DIGO', 8, 'DISC', 0)

    print('Outputs Test Complete')

def uart_test(com):
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
        response = com.read_until().decode()

        if response != packet + '\r\n':
            fail_string = f'UART Test Failed at: {i}'
            print(fail_string)
            if EXIT_ON_FAIL:
                raise Exception(fail_string)

    print('UART Test Complete')
def run_quiet_test(com):

    com.write('*RST\r\n'.encode())

    command_test(com, 'DECI')

    parse_test(com, 'DECI')

    parse_test(com, 'HEX')
    
    analog_stability_test(com, 'DECI')

    output_mode_test(com)

    uart_test(com)

    # TODO: Test the manipulation of settings


if __name__ == "__main__":
    ports = list(serial.tools.list_ports.comports())
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            qPort = p.device
            break

    com = serial.Serial(port=qPort, timeout=1)

    if  run_quiet_test(com):
        print("All Tests Passed")

    

