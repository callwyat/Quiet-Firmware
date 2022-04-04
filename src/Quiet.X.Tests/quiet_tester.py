
import re
from quiet import Quiet


if 'EXIT_ON_FAIL' not in locals():
    EXIT_ON_FAIL = True

if 'VERBOSE' not in locals():
    VERBOSE = True

QUIET_TERMINATION = '\r\n'

BOOL_PATTERN = '\\b[01]\\b'
HEX8_PATTERN = "\\b0[xX]([0-9a-fA-F]{2})\\b"
HEX16_PATTERN = "\\b0[xX]([0-9a-fA-F]{4}|[0-9a-fA-F]{2})\\b"
HEX24_PATTERN = "\\b0[xX]([0-9a-fA-F]{6}|[0-9a-fA-F]{4}|[0-9a-fA-F]{2})\\b"
INT16_PATTERN = '\\b[\\d]{1,5}\\b'
INT24_PATTERN = '\\b[\\d]{1,8}\\b'
OUTPUT_MODE_PATTERN = '\\b(DISC|PWM|SERV)\\b'

def generate_fail_message(command, expected, response):
    return (f"Test Failed\nSent:     {repr(command)}\n" +
          f"Expected: {repr(expected)}\n" +
          f"Received: {repr(response)}")

class QuietTester(Quiet):

    def __init__(self, coms) -> None:

        Quiet.__init__(self, coms)

    def query_test(self, command:str, expectation:str):

        response = self.query_raw(command + QUIET_TERMINATION)

        if re.search(expectation, response) is None:
            message = generate_fail_message(command, expectation, response)
            if EXIT_ON_FAIL:
                raise Exception(message)
            else:
                print(message)

        if VERBOSE:
            # Get the time to execute the command
            execution_time = f'( {str(self.query_int("DIAG?")).strip().ljust(5)} )'

            print(f"{command.ljust(24)} {execution_time} =>   {response.strip()}")
            
    def channel_query_test(self, command:str, start:int, stop:int, expectation:str):

        for i in range(start, stop + 1):
            self.query_test(command.replace('#', str(i)), expectation)


        