
import serial
import serial.tools.list_ports
import quiet_coms

def i2c_write(com: serial.Serial, addr: int, data: bytearray):

    # Generate the header
    data_size = str(len(data))
    header_size = str(len(data_size))

    ieee_header = f'#{header_size}{data_size}'

    com.write(f'IIC:ADDR {addr};WRIT {ieee_header}\r\n'.encode() + data)

def i2c_read(com: serial.Serial, addr: int, register: int, readCount: int):
    
    message = (f'IIC:ADDR {addr};WRIT #11'.encode() + 
        register.to_bytes(1, 'big') + 
        f';READ? {readCount}\r\n'.encode())

    com.write(message)

    h = com.read(4)
    headerSize = int(h[1])

    dataSize = int(com.read(headerSize))

    return com.read(dataSize)

def i2c_test(qCom: quiet_coms.quiet_coms):

    qCom.write('IIC:ENAB 1')
    try:
        result = int(i2c_read(qCom.com, 0x50, 0xFF, 2))

        if result != 1004:
            raise Exception('Fail')
    finally:
        qCom.write('IIC:ENAB 0')
    

if __name__ == "__main__":
    ports = list(serial.tools.list_ports.comports())
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            qPort = p.device
            break

    com = serial.Serial(port=qPort, timeout=5)

    qCom = quiet_coms.quiet_coms(com)

    i2c_test(qCom)

