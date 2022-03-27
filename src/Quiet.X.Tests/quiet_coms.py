import serial
import serial.tools.list_ports


class quiet_coms():

    def __init__(self, port=None):
        if port == None:
            ports = list(serial.tools.list_ports.comports())
            for p in ports:        
                if p.product and 'Qy@ Board' in p.product:
                    qPort = p.device
                    break
        
            self.com = serial.Serial(port=qPort, timeout=1)

        elif type(port) == serial.Serial:
            self.com = port
        elif type(port) == str:
            self.com = serial.Serial(port=port, timeout=1)
        else:
            raise Exception('Unable to determine what to do with \'port\'')

    def write(self, input):

        self.com.write(f'{input}\r\n'.encode())

    def read(self):
        return self.read().strip()

    def read_raw(self):
        return self.com.read_until().decode()

    def writeIEEE(self, command, data):

        # Generate the header
        data_size = str(len(data))
        header_size = str(len(data_size))

        ieee_header = f'#{header_size}{data_size}'

        # Send header and data
        self.com.write(f'{command} {ieee_header}'.encode() + bytearray(data))

    def queryIEEE(self, command):

        self.write(command)

        headerSize = int(self.com.read(2)[1])

        dataSize = int(self.com.read(headerSize))

        return self.com.read(dataSize)
        
