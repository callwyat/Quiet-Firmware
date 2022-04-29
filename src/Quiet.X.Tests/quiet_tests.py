
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

    
def _output_command_test(tester: QuietTester, output_name:str, count:int, error_base:int, number_pattern:str):

    error_base = error_base << 8

    tester.channel_query_test(f'{output_name}:CH#?', 1, count, number_pattern, f'INVALID {output_name} CHANNEL', error_base + 0x01)
    tester.check_channel_limit(f'{output_name}:CH#', 1, count, 0, 1023, f'INVALID {output_name} VALUE', error_base + 0x03)

    tester.channel_query_test(f'{output_name}:CH#:VALU?', 1, count, number_pattern, f'INVALID {output_name} CHANNEL', error_base + 0x01)
    tester.check_channel_limit(f'{output_name}:CH#:VALU', 1, count, 0, 1023, f'INVALID {output_name} VALUE', error_base + 0x03)
    
    tester.channel_query_test(f'{output_name}:CH#:MODE?', 1, count, OUTPUT_MODE_PATTERN, f'INVALID {output_name} CHANNEL', error_base + 0x01)

def command_test(tester: QuietTester, number_mode='DECI', all_commands=True):

    print('Starting Command Tests')

    number_pattern_24 = HEX24_PATTERN if number_mode == 'HEX' else INT24_PATTERN
    number_pattern_16 = HEX16_PATTERN if number_mode == 'HEX' else INT16_PATTERN
    number_pattern_8 = HEX8_PATTERN if number_mode == 'HEX' else INT16_PATTERN

    tester.reset()
    tester.set_number_mode(number_mode)

    tester.query_test('*IDN?', 'Y@ Technologies,Qy@ Board,.*?,[2-9]\\.[0-9]')

    tester.query_test('DIGI?', number_pattern_8)
    tester.query_test('DIGInputs?', number_pattern_8)
    tester.channel_query_test('DIGI:CH#?', 1, 8, number_pattern_8, 'INVALID DIGI CHANNEL', 0x0201)
    tester.channel_query_test('DIGI:CH#:VALU?', 1, 8, number_pattern_8, 'INVALID DIGI CHANNEL', 0x0201)

    tester.channel_query_test('ANAI:CH#?', 1, 4, number_pattern_16, 'INVALID ANAI CHANNEL', 0x0301)

    tester.query_test('DIGOutputs?', number_pattern_8)
    tester.check_limit('DIGO', 0, 255, 'DIGO VALUE ERROR', 0x0403)
    for i in range(1, 9):
        tester.set_and_verify(f'DIGO:CH{i}:MODE', 'DISC')
        tester.check_boolean_value(f'DIGO:CH{i}')
        tester.check_boolean_value(f'DIGO:CH{i}:VALU')
        tester.set_and_verify(f'DIGO:CH{i}:MODE', 'SERV')

    _output_command_test(tester, 'DIGO', 8, 0x04, number_pattern_16)
    tester.check_modes('DIGO:CH1:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0402)
    tester.check_modes('DIGO:CH2:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0402)
    tester.check_modes('DIGO:CH3:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0402)
    tester.check_modes('DIGO:CH4:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0402)
    tester.check_modes('DIGO:CH5:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0402)
    tester.check_modes('DIGO:CH6:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0402)
    tester.check_modes('DIGO:CH7:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0402)
    tester.check_modes('DIGO:CH8:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0402)

    _output_command_test(tester, 'ANAO', 2, 0x05, number_pattern_16)
    tester.check_modes('ANAO:CH1:MODE', ['SERV', 'PWM'], ['DISC'], 'INVALID OUTPUT MODE', 0x0502)
    tester.check_modes('ANAO:CH2:MODE', ['SERV', 'PWM'], ['DISC'], 'INVALID OUTPUT MODE', 0x0502)
  
    _output_command_test(tester, 'PWM', 6, 0x06, number_pattern_16)
    tester.check_modes('PWM:CH1:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0602)
    tester.check_modes('PWM:CH2:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0602)
    tester.check_modes('PWM:CH3:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0602)
    tester.check_modes('PWM:CH4:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0602)
    tester.check_modes('PWM:CH5:MODE', ['SERV', 'PWM'], ['DISC'], 'INVALID OUTPUT MODE', 0x0602)
    tester.check_modes('PWM:CH6:MODE', ['SERV', 'PWM'], ['DISC'], 'INVALID OUTPUT MODE', 0x0602)

    _output_command_test(tester, 'SERV', 10, 0x07, number_pattern_16)
    tester.check_modes('SERV:CH1:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH2:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH3:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH4:MODE', ['DISC', 'SERV'], ['PWM'], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH5:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH6:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH7:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH8:MODE', ['DISC', 'SERV', 'PWM'], [], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH9:MODE', ['SERV', 'PWM'], ['DISC'], 'INVALID OUTPUT MODE', 0x0702)
    tester.check_modes('SERV:CH10:MODE', ['SERV', 'PWM'], ['DISC'], 'INVALID OUTPUT MODE', 0x0702)

    if all_commands:
        tester.query_test('UART:BAUD?', number_pattern_24)
        tester.check_limit('UART:BAUD', 61, 1000000, 'INVALID UART BAUD', 0x0910)
        tester.query_test('UART:MODE?', '\\b(USBU|SCPI)\\b')
        tester.check_modes('UART:MODE', ['USBU', 'SCPI'], [], 'INVALID UART MODE', 0x0911)
        tester.query_test('UART:OVER?', number_pattern_8)

        tester.query_test('SPI:BAUD?', number_pattern_24)
        tester.check_lower_limit('SPI:BAUD', 250000, 'INVALID SPI BAUD', 0x0A01)
        tester.query_test('SPI:CS?', BOOL_PATTERN)
        tester.check_boolean_value('SPI:CS')

        tester.query_test('IIC:MODE?', '\\b(OFF|MAST)\\b')
        tester.check_modes('IIC:MODE', ['OFF', 'MAST'], [], 'INVALID IIC MODE', 0x0B04)
        tester.query_test('IIC:BAUD?', number_pattern_24)
        tester.check_limit('IIC:BAUD', 16000, 1000000, 'INVALID IIC BAUD', 0x0B01)
        tester.query_test('IIC:TIMEout?', number_pattern_16)
        tester.check_limit('IIC:TIME', 1, 255, 'INVALID IIC TIMEOUT', 0x0B02)
        tester.query_test('IIC:ADDRess?', number_pattern_8)
        tester.check_limit('IIC:ADDR', 0, 127, 'INVALID IIC ADDRESS', 0x0B03)

        tester.query_test('IIC:ACK?', number_pattern_8)

        tester.query_test('IIC:REGIster:ADDRess?', number_pattern_8)
        tester.check_limit('IIC:REGI:ADDR', 0, 255, 'INVALID IIC REGISTER ADDRESS', 0x0B21)
        tester.query_test('IIC:REGIster:RSIZe?', number_pattern_8)
        tester.check_limit('IIC:REGI:RSIZ', 1, 2, 'INVALID IIC RSIZE', 0x0B20)

        tester.query_test('IIC:REGIster:ACK?', number_pattern_8)

    tester.query_test('SYST:ERR?', number_pattern_8)

    tester.query_test('SYST:INFO:COMM:HASH?', '"(~?[0-9a-fA-F]{40}~?)"')
    tester.query_test('SYST:INFO:COMM:AUTH?', '"((\w*) *)*"')
    tester.query_test('SYST:INFO:COMM:DATE?', '"(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})"')

    tester.query_test('SYST:INFO:BUIL:VERS?', '"(\\d{4})"')
    tester.query_test('SYST:INFO:BUIL:USER?', '"((\w*) *)*"')
    tester.query_test('SYST:INFO:BUIL:DATE?', '"(\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2})"')

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

