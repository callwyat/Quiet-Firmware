
from quiet_coms import QuietComs

class Quiet(QuietComs):
    
    def __init__(self, coms) -> None:
        QuietComs.__init__(self, coms)

    def reset(self):
        """Resets quiet board
        """
        self.write('*RST')

    def set_number_mode(self, mode:str):
        """Sets the number mode used by

        Args:
            mode (str): Options are 'DECI' or 'HEX'

        Raises:
            Exception: Invalid Number mode
        """

        if mode == 'DECI' or mode == 'HEX':
            # Changed the number mode
            self.write(f'SYST:NUMB {mode}')
        else:
            raise Exception(f'Invalid Number mode {mode}. Must be \'DECI\' or \'HEX\'')

    def set_and_verify(self, command:str, value, tolerance:float=0):

        query_str = command.split(':')[-1]
        full_command = f'{command} {value};{query_str}?'

        if type(value) == int:
            response = self.query_int(full_command)

            low_limit = value * (1 - tolerance)
            high_rate = value * (1 + tolerance)

            if response < low_limit or high_rate < response:
                raise QuietSetError(f'{command} failed. Sent {value} Received {response}')

        else:
            response = self.query(full_command).strip(';')

            if response != value:
                raise QuietSetError(f'{command} failed. Sent {value} Received {response}')

class QuietSetError(Exception):
    def __init__(self, *args: object) -> None:
        super().__init__(*args)

