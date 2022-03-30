import serial
import serial.tools.list_ports

class quiet_coms():

    def __init__(self, port=None):
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

    def write(self, input:str):
        self.com.write(f'{input}\r\n'.encode())

    def read(self) -> str:
        return self.read_raw().strip()

    def query(self, input:str) -> str:
        self.com.flushInput()
        self.write(input)
        return self.read()

    def query_int(self, input:str) -> int:
        result = self.query(input).strip(';')

        if '0x' in result:
            return int(result[2:], 16)
        else:
            return int(result)

    def read_raw(self) -> str:
        return self.com.read_until().decode()

    def query_raw(self, input:str) -> str:
        self.com.flushInput()
        self.write(input)
        return self.read_raw()

    def writeIEEE(self, command:str, data:bytearray) -> None:

        # Generate the header
        data_size = str(len(data))
        header_size = str(len(data_size))

        ieee_header = f'#{header_size}{data_size}'

        # Send header and data
        self.com.write(f'{command} {ieee_header}'.encode() + bytearray(data))

    def queryIEEE(self, command:str) -> bytearray:

        self.com.flushInput()
        self.write(command)

        val = ''
        while not '#' in val: 
            val = self.com.read(1).decode()

        headerSize = int(self.com.read(1).decode())

        dataSize = int(self.com.read(headerSize).decode())

        return self.com.read(dataSize)
        
def find_quiet_ports() -> list:

    ports = list(serial.tools.list_ports.comports())
    
    result = []
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            result.append(p.device)

    return result