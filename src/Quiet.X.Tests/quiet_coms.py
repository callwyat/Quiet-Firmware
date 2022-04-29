import time
import serial
import serial.tools.list_ports
from logger import LogWrapper
class QuietComs():

    def __init__(self, port=None, **kargs):
        if type(port) == serial.Serial:
            self.com = port
        elif type(port) == str:
            self.com = serial.Serial(port=port, timeout=1)
        elif port == None:
            ports = list(serial.tools.list_ports.comports())
            for p in ports:        
                if p.product and 'Qy@ Board' in p.product:
                    qPort = p.device
                    break
        
            self.com = serial.Serial(port=qPort, timeout=1)
        else:
            raise Exception('Unable to determine what to do with \'port\'')

        if 'log_path' in kargs:
            f = open(kargs['log_path'], 'w')
            self.com = LogWrapper(self.com, f)

    def write_raw(self, input:str):
        """ Writes the given input with no modification

        Args:
            input (str): The text to write to the quiet board
        """
        self.com.write(input.encode())

    def write(self, input:str):
        """ Writes the given input adding termination

        Args:
            input (str): The input to write to the quiet board
        """
        self.write_raw(f'{input}\r\n')

    def read_raw(self) -> str:
        """ Reads from the quiet board return everyhing

        Returns:
            str: The data stored in the quiets buffer
        """
        return self.com.read_until().decode()

    def read(self) -> str:
        """ Reads from the quiet board stripping off whitespace

        Returns:
            str: The data from the quiet board without the whitespace
        """
        return self.read_raw().strip()

    def query_raw(self, input:str, delay:int=0) -> str:
        """ Writes the raw input to quiet board and reads back the raw input

        Args:
            input (str): The raw string to write to the quiet board
            delay (int, optional): A time, in seconds, to delay between the write
                and the read. Defaults to 0.

        Returns:
            str: The raw result from the quiet board
        """
        self.com.flushInput()
        self.write_raw(input)

        if delay > 0:
            time.sleep(delay)

        return self.read_raw()

    def query(self, input:str, delay:int=0) -> str:
        """ Write the input, adding termination, and reads back the result without
        whitespace

        Args:
            input (str): The text to write to the quiet board
            delay (int, optional): The time, in seconds, to delay between write
                and read. Defaults to 0.

        Returns:
            str: The result from the quiet board without whitespace
        """
        self.com.flushInput()
        self.write(input)
        return self.read()

    def query_int(self, input:str) -> int:
        """ Writes the given input to the quiet board and attempts to parse the result
        into an interger

        Args:
            input (str): The command to send to the quiet board

        Returns:
            int: The interger value from the quiet board
        """
        result = self.query(input).strip(';')

        if '0x' in result:
            return int(result[2:], 16)
        else:
            return int(result)

    def writeIEEE(self, command:str, data:bytearray) -> None:
        """ Writes the given command and byte array to the quiet board

        Args:
            command (str): The header before the data
            data (bytearray): The raw bytes to write to the quiet board
        """

        # Generate the header
        data_size = str(len(data))
        header_size = str(len(data_size))

        ieee_header = f'#{header_size}{data_size}'

        # Send header and data
        self.com.write(f'{command} {ieee_header}'.encode() + bytearray(data) + '\r\n'.encode())

    def queryIEEE(self, command:str) -> bytearray:
        """ Writes the given command to the quiet board and attempts to read the IEEE
        data block back

        Args:
            command (str): A command that invokes an IEEE Data Block header result

        Returns:
            bytearray: The Data Block from the quiet board
        """
        self.com.flushInput()
        self.write(command)

        val = ''
        while not '#' in val: 
            val = self.com.read(1).decode()

        headerSize = int(self.com.read(1).decode())

        dataSize = int(self.com.read(headerSize).decode())

        return self.com.read(dataSize)
        
def find_quiet_ports() -> list:
    """ Scans the computer ports for a port that is a quiet board

    Returns:
        list: All ports that are quiet boards
    """

    ports = list(serial.tools.list_ports.comports())
    
    result = []
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            result.append(p.device)

    return result