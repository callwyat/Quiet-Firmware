

import serial
import serial.tools.list_ports

def spi_exchange(com, data):

    # Assert the Chip Select
    com.write('SPI:CS 0;CS?\r\n'.encode())
    # Verify Assertion
    response = com.read_until().decode()
    if response != ',0\r\n':
        raise Exception('Chip Selection failed')

    # Generate the header
    data_size = str(len(data))
    header_size = str(len(data_size))

    ieee_header = f'#{header_size}{data_size}'

    # Send header and data
    com.write(f'SPI:EXCH {ieee_header}'.encode() + bytearray(data))

    # Get the response
    com.timeout = None
    result = com.read(len(ieee_header) + len(data) + 2)

    com.write('SPI:CS 1;CS?\r\n'.encode())

    # Verify Assertion
    response = com.read_until().decode()
    # if response != ',1':
    #     raise Exception('Chip Selection failed')

    # Return all but the header
    return result[len(ieee_header):]

def spi_test(com):

    # Init the SD Card
    # 80 Clocks while deselected
    com.write('SPI:CS 1\r\n'.encode())
    com.write('SPI:EXCH #216\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF'.encode())
    com.timeout = 5000
    response = com.read(22)

    # COM_0 -> Reset?
    response = spi_exchange(com, [0xFF, 0xFF, 0x40, 0x00, 0x00, 0x00, 0x00, 0x95])

    # COM_8 -> Wait for the card to be ready
    while True:
        response = spi_exchange(com, [0x48, 0x00, 0x00, 0x01, 0xAA, 0x86])

        if response[0] != 0:
            break

    # COM_58 -> Check the supported voltage levels of the card
    response = spi_exchange(com, [0x7A, 0x00, 0x00, 0x00, 0x00, 0x95])

    # TODO: Maybe actually look at the supported levels...

    # COM_55 -> Leave Idle
    while True:
        response = spi_exchange(com, [0x77, 0x00, 0x00, 0x00, 0x00, 0x7A])

        response = spi_exchange(com, [0x69, 0x40, 0x00, 0x00, 0x00, 0x95])

        if response[0] == 0:
            break

    print(response)


if __name__ == "__main__":
    ports = list(serial.tools.list_ports.comports())
    for p in ports:        
        if p.product and 'Qy@ Board' in p.product:
            qPort = p.device
            break

    com = serial.Serial(port=qPort, timeout=5)

    spi_test(com)
