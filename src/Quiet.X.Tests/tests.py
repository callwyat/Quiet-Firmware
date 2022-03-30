
import re
import time
import quiet_coms

QUIET_TERMINATION = '\r\n'
BOOL_PATTERN = '\\b[01]\\b'
HEX8_PATTERN = "\\b0[xX]([0-9a-fA-F]{2})\\b"
HEX16_PATTERN = "\\b0[xX]([0-9a-fA-F]{4}|[0-9a-fA-F]{2})\\b"
HEX24_PATTERN = "\\b0[xX]([0-9a-fA-F]{6}|[0-9a-fA-F]{4}|[0-9a-fA-F]{2})\\b"
INT16_PATTERN = '\\b[\\d]{1,5}\\b'
INT24_PATTERN = '\\b[\\d]{1,8}\\b'
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

def output_test(quite: quiet_coms.quiet_coms, command, count, mode, value):
    for i in range(1, count + 1):
        full_command = f'{command}:CH{i}:MODE {mode};VALUE {value};MODE?'
        response = quite.query_raw(full_command)
        
        expected = f';;{mode}\r\n'
        if response != expected:
            fail_string = generate_fail_message(full_command, expected, response)
            print(fail_string)
            if EXIT_ON_FAIL:
                raise Exception(fail_string)

        if VERBOSE:
            print(f'{command}:CH{i}'.ljust(10) + f' => {response.strip()}')

        time.sleep(1)

default_test_results = [ 
    'DISC', 
    'DISC', 
    'DISC', 
    'DISC', 
    'DISC', 
    'DISC', 
    'DISC', 
    'DISC', 
    'PWM',
    'PWM',
    ]

def defaults_test(com):

    # Restore factory defaults
    quite.write(f'SYST:REST FACT')

    i = 1
    for expectation in default_test_results:
        result = quite.query(f'SERV:CH{i}:MODE?')
    
        if result == expectation:
            if VERBOSE:
                print(f'Default CH{i} -> Pass')
        else:
            if EXIT_ON_FAIL:
                raise Exception(f'Channel {i} failed default test. Expected "{expectation}" Received "{result}"')
            else:
                print(f'Default CH{i} -> FAIL')

        i += 1

    

def command_test(quite: quiet_coms.quiet_coms, number_mode='DECI'):

    if number_mode == 'DECI' or number_mode == 'HEX':
        # Changed the number mode
        quite.write(f'SYST:NUMB {number_mode}')
    else:
        raise Exception(f'Invalid Number mode {number_mode}. Must be \'DECI\' or \'HEX\'')

    number_pattern_24 = HEX24_PATTERN if number_mode == 'HEX' else INT24_PATTERN
    number_pattern_16 = HEX16_PATTERN if number_mode == 'HEX' else INT16_PATTERN
    number_pattern_8 = HEX8_PATTERN if number_mode == 'HEX' else INT16_PATTERN

    queryTests = [
        QueryTest('*IDN?', 'Y@ Technologies,Qy@ Board,.*?,[2-9]\\.[0-9]'),

        QueryTest('DIGI?', number_pattern_8),
        QueryTest('DIGInputs?', number_pattern_8),
        QueryChannelTest('DIGI:CH#?', 1, 8, number_pattern_8),
        QueryChannelTest('DIGI:CH#:VALU?', 1, 8, number_pattern_8),

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

        QueryTest('UART:BAUD?', number_pattern_24),

        QueryTest('SPI:BAUD?', number_pattern_24),

        QueryTest('IIC:ENABle?', number_pattern_8),
        QueryTest('IIC:BAUD?', number_pattern_24),
        QueryTest('IIC:TIMEout?', number_pattern_16),
        QueryTest('IIC:ADDRess?', number_pattern_8),

        QueryTest('SYST:INFO:COMM:HASH?', '"(~?[0-9a-fA-F]{40}~?)"'),
        QueryTest('SYST:INFO:COMM:AUTH?', '"((\w*) *)*"'),
        QueryTest('SYST:INFO:COMM:DATE?', '"(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})"'),

        QueryTest('SYST:INFO:BUIL:VERS?', '"(\\d{4})"'),
        QueryTest('SYST:INFO:BUIL:USER?', '"((\w*) *)*"'),
        QueryTest('SYST:INFO:BUIL:DATE?', '"(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})"'),
    ]

    print('Starting Command Tests')

    for test in queryTests:

        for command in test.commands:

            response = quite.query_raw(command)
            test_result = test.check_response(response)

            if not test_result:
                print_fail_message(command, test.response, response)

                if EXIT_ON_FAIL:
                    raise Exception(generate_fail_message(command, test.response, response))

            else:
                # Get the time to execute the command
                value_string = quite.query('DIAG?')
                if number_mode == 'HEX':
                    execution_time = int(value_string[2:], 16)
                else:
                    execution_time = int(value_string, 10)

                if VERBOSE:
                    print(f"{command.strip().ljust(16)} ( {str(execution_time).strip().ljust(5)} ) =>   {response.strip()}")

    print('Command Tests Passed')

def parse_test(quite: quiet_coms.quiet_coms, number_mode='DECI'):
    
    if number_mode == 'DECI' or number_mode == 'HEX':
        # Changed the number mode
        quite.write(f'SYST:NUMB {number_mode}')
    else:
        raise Exception(f'Invalid Number mode {number_mode}. Must be \'DECI\' or \'HEX\'')

    print('Starting Parse Test')

    if number_mode == 'DECI':
        # Test the parsing method
        for val in range(0, 1024, 3):

            if VERBOSE:
                print(f'Testing             => {val}')

            command = f'ANAO:CH1 {val};CH1?'
            response = quite.query_raw(command)

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
            response = quite.query_raw(command)

            expected = f';0x{format(val, "02X" if val <= 0xFF else "04X")}\r\n'
            if response != expected:
                fail_string = generate_fail_message(command, expected, response)
                print(fail_string)
                if EXIT_ON_FAIL:
                    raise Exception(fail_string)

    print('Parse Test Passed')

def output_mode_test(quite: quiet_coms.quiet_coms):
    print('Outputs Test')

    output_test(quite, 'SERV', 10, 'SERV', '0x3FF')

    output_test(quite, 'PWM', 6, 'PWM', '0x3FF')

    output_test(quite, 'DIGO', 8, 'DISC', 0)

    print('Outputs Test Complete')


def run_quiet_test(quite: quiet_coms.quiet_coms):

    quite.write('*RST')

    defaults_test(quite)

    command_test(quite, 'DECI')

    parse_test(quite, 'DECI')

    parse_test(quite, 'HEX')

    output_mode_test(quite)

    # TODO: Test the manipulation of settings


if __name__ == "__main__":

    qPorts = quiet_coms.find_quiet_ports()

    quite = quiet_coms.quiet_coms(qPorts[0])

    run_quiet_test(quite)
    
    print("All Tests Passed")

