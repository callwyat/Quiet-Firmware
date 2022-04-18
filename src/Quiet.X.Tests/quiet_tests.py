
import time
from quiet_coms import QuietComs, find_quiet_ports
from quiet_tester import *

VERBOSE = True
EXIT_ON_FAIL = True


def output_test(quiet: QuietComs, command, count, mode, value):
    for i in range(1, count + 1):
        full_command = f'{command}:CH{i}:MODE {mode};VALUE {value};MODE?\r\n'
        response = quiet.query_raw(full_command)
        
        expected = f';;{mode}\r\n'
        if response != expected:
            fail_string = generate_fail_message(full_command, expected, response)
            print(fail_string)
            if EXIT_ON_FAIL:
                raise Exception(fail_string)

        if VERBOSE:
            print(f'{command}:CH{i}'.ljust(10) + f' => {response.strip()}')

        time.sleep(0.2)

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

def defaults_test(quiet:QuietComs):

    # Restore factory defaults
    quiet.write(f'SYST:REST FACT')

    i = 1
    for expectation in default_test_results:
        result = quiet.query(f'SERV:CH{i}:MODE?')
    
        if result == expectation:
            if VERBOSE:
                print(f'Default CH{i} -> Pass')
        else:
            if EXIT_ON_FAIL:
                raise Exception(f'Channel {i} failed default test. Expected "{expectation}" Received "{result}"')
            else:
                print(f'Default CH{i} -> FAIL')

        i += 1

    

def command_test(tester: QuietTester, number_mode='DECI', all_commands=True):

    print('Starting Command Tests')

    number_pattern_24 = HEX24_PATTERN if number_mode == 'HEX' else INT24_PATTERN
    number_pattern_16 = HEX16_PATTERN if number_mode == 'HEX' else INT16_PATTERN
    number_pattern_8 = HEX8_PATTERN if number_mode == 'HEX' else INT16_PATTERN

    tester.query_test('*IDN?', 'Y@ Technologies,Qy@ Board,.*?,[2-9]\\.[0-9]'),

    tester.query_test('DIGI?', number_pattern_8),
    tester.query_test('DIGInputs?', number_pattern_8),
    tester.channel_query_test('DIGI:CH#?', 1, 8, number_pattern_8),
    tester.channel_query_test('DIGI:CH#:VALU?', 1, 8, number_pattern_8),

    tester.channel_query_test('ANAI:CH#?', 1, 4, number_pattern_16),

    tester.query_test('DIGOutputs?', number_pattern_8),
    tester.channel_query_test('DIGO:CH#?', 1, 8, number_pattern_16),
    tester.channel_query_test('DIGO:CH#:VALU?', 1, 8, number_pattern_16),
    tester.channel_query_test('DIGO:CH#:MODE?', 1, 8, '\\bDISC\\b'),

    tester.channel_query_test('ANAO:CH#?', 1, 2, number_pattern_16),
    tester.channel_query_test('ANAO:CH#:VALU?', 1, 2, number_pattern_16),
    tester.channel_query_test('ANAO:CH#:MODE?', 1, 2, '\\bPWM\\b'),

    tester.channel_query_test('PWM:CH#?', 1, 6, number_pattern_16),
    tester.channel_query_test('PWM:CH#:VALU?', 1, 6, number_pattern_16),
    tester.channel_query_test('PWM:CH#:MODE?', 1, 6, OUTPUT_MODE_PATTERN), 

    tester.channel_query_test('SERV:CH#?', 1, 10, number_pattern_16),
    tester.channel_query_test('SERV:CH#:VALU?', 1, 10, number_pattern_16),
    tester.channel_query_test('SERVo:CH#:MODE?', 1, 10, OUTPUT_MODE_PATTERN), 

    if all_commands:
        tester.query_test('UART:BAUD?', number_pattern_24),
        tester.query_test('UART:MODE?', '\\b(USBU|SCPI)\\b'),
        tester.query_test('UART:OVER?', number_pattern_8),

        tester.query_test('SPI:BAUD?', number_pattern_24),

        tester.query_test('IIC:MODE?', '\\b(OFF|MAST)\\b'),
        tester.query_test('IIC:BAUD?', number_pattern_24),
        tester.query_test('IIC:TIMEout?', number_pattern_16),
        tester.query_test('IIC:ADDRess?', number_pattern_8),
        tester.query_test('IIC:ACK?', number_pattern_8),

        tester.query_test('IIC:REGIster:ADDRess?', number_pattern_8),
        tester.query_test('IIC:REGIster:RSIZe?', number_pattern_8),
        tester.query_test('IIC:REGIster:ACK?', number_pattern_8),

    tester.query_test('SYST:ERR?', number_pattern_8),

    tester.query_test('SYST:INFO:COMM:HASH?', '"(~?[0-9a-fA-F]{40}~?)"'),
    tester.query_test('SYST:INFO:COMM:AUTH?', '"((\w*) *)*"'),
    tester.query_test('SYST:INFO:COMM:DATE?', '"(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})"'),

    tester.query_test('SYST:INFO:BUIL:VERS?', '"(\\d{4})"'),
    tester.query_test('SYST:INFO:BUIL:USER?', '"((\w*) *)*"'),
    tester.query_test('SYST:INFO:BUIL:DATE?', '"(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})"'),

    print('Command Tests Passed')

class ParseTest():

    def __init__(self, command:str, start:int, end:int, increment:int=1) -> None:
        self.command = command
        self.start = start
        self.end = end
        self.increment = increment

def parse_test(quiet: QuietTester, number_mode='DECI'):
    
    tests = [
        ParseTest('TEST:PARS #;PARS?\r\n', 0, 1024, 1),
        ParseTest('TEST:PARS #;PARS?\r\n', 1030, 2**15, 11),
        # TODO: Test ParseINT24 Method
    ]

    quiet.set_number_mode(number_mode)

    print('Starting Parse Test')
    print_count = 0

    for test in tests:
        if number_mode == 'DECI':

            # Test the parsing method
            for val in range(test.start, test.end, test.increment):

                if VERBOSE and val >= print_count:
                    print(f'Tested to            => {val}')
                    print_count = print_count + test.increment * 10

                response = quiet.query_raw(test.command.replace('#', str(val)))

                expected = f';{val}\r\n'
                if response != expected:
                    fail_string = generate_fail_message(test.command, expected, response)
                    print(fail_string)
                    if EXIT_ON_FAIL:
                        raise Exception(fail_string)

        else:
            # Test the parsing method
            for val in range(test.start, test.end, test.increment):

                if VERBOSE and val >= print_count:
                    print(f'Testing             => {hex(val)}')
                    print_count = print_count + test.increment * 10

                response = quiet.query_raw(test.command.replace('#', hex(val)))

                expected = f';0x{format(val, "02X" if val <= 0xFF else "04X")}\r\n'
                if response != expected:
                    fail_string = generate_fail_message(test.command, expected, response)
                    print(fail_string)
                    if EXIT_ON_FAIL:
                        raise Exception(fail_string)

    print('Parse Test Passed')

def output_mode_test(quiet: QuietComs):
    print('Outputs Test')

    output_test(quiet, 'SERV', 10, 'SERV', '0x3FF')

    output_test(quiet, 'PWM', 6, 'PWM', '0x3FF')

    output_test(quiet, 'DIGO', 8, 'DISC', 0)

    print('Outputs Test Complete')


def run_quiet_test(tester: QuietTester):

    tester.write('*RST')

    defaults_test(tester)

    command_test(tester)

    parse_test(tester, 'DECI')

    parse_test(tester, 'HEX')

    output_mode_test(tester)

    # TODO: Test the manipulation of settings


if __name__ == "__main__":

    qPort = find_quiet_ports()[0]

    run_quiet_test(QuietTester(qPort))
    
    print("All Tests Passed")

