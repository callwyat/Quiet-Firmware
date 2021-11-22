
import serial
import serial.tools.list_ports
import re
import time


def parse_time_test(com):

    average = 0

    for x in range(0, 100):
        com.write('DIGI?\r\n'.encode())

        value = com.read_until()

        com.write('DIAG?\r\n'.encode())

        value = int(com.read_until().decode())

        average += value

    average = average / 100

    print(average)



if __name__ == "__main__":
    ports = list(serial.tools.list_ports.comports())
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            qPort = p.device
            break

    com = serial.Serial(port=qPort, timeout=1)

    parse_time_test(com)
