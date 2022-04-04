
from quiet_coms import QuietComs

class Quite(QuietComs):
    
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