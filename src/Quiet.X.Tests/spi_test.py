
from quiet_tester import QuietTester

class QuietSPI(QuietTester):
    def __init__(self, coms, **kargs) -> None:

        QuietTester.__init__(self, coms, **kargs)

    def set_spi_cs(self, value:bool):

        bString = '1' if value else '0'

        # Assert the Chip Select
        response = self.query(f'SPI:CS {bString};CS?')
        # Verify Assertion
        if not value and response != f';{bString}':
            raise Exception('Failed to set Chip Select')

    def spi_exchange(self, data):

        # Assert the Chip Select
        self.set_spi_cs(False)

        # Send data
        self.writeIEEE('SPI:EXCH', data)

        # Get the response
        self.com.timeout = None
        result = self.readIEEE()

        # De-assert Chip Select Assertion
        self.set_spi_cs(True)

        # Return all but the header
        return result

    def sd_card_exchange(self, data, response_length):

        response_raw = self.spi_exchange(data + [0xFF] * (response_length + 1))

        return response_raw[len(data) + 1:]



def spi_test(quiet:QuietSPI):

    quiet.set_and_verify('SPI:BAUD', 250000)

    # Init the SD Card
    # 80 Clocks while deselected
    quiet.set_spi_cs(True)
    quiet.writeIEEE('SPI:EXCH', [0xFF] * 16)
    quiet.timeout = 5000
    response = quiet.readIEEE()

    # More clocks while selected
    response = quiet.spi_exchange([0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF])

    # COM_0 -> Reset?
    response = quiet.sd_card_exchange([0x40, 0x00, 0x00, 0x00, 0x00, 0x95], 1)

    # COM_8 -> Wait for the card to be ready
    attempt = 0
    while True:
        attempt += 1
        response = quiet.sd_card_exchange([0x48, 0x00, 0x00, 0x01, 0xAA, 0x86], 1)

        if response[0] == 1:
            break
        elif (attempt > 100):
            raise Exception('SD Card does not want to be ready')

    # COM_58 -> Check the supported voltage levels of the card
    response = quiet.sd_card_exchange([0x7A, 0x00, 0x00, 0x00, 0x00, 0x95], 4)

    # TODO: Maybe actually look at the supported levels...

    
    while True:
        # COM_55 -> Prepare for the app command
        response = quiet.sd_card_exchange([0x77, 0x00, 0x00, 0x00, 0x00, 0x7A], 1)

        # COM_41 -> Leave Idle Mode
        response = quiet.sd_card_exchange([0x69, 0x40, 0x00, 0x00, 0x00, 0x95], 1)

        if response[0] == 1:
            break

    # Full Speed AHEAD!!!!!
    quiet.set_and_verify('SPI:BAUD', 4000000)

    # Read the MBR
    response = quiet.sd_card_exchange([0x51, 0x00, 0x00, 0x00, 0x00, 0x95], 515)

    print(response)


if __name__ == "__main__":

    spi_test(QuietSPI(None, log_path='usb_log.txt'))
