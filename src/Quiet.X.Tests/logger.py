


class LogWrapper:

    def __init__(self, child, logger) -> None:
        self.child = child
        self.logger = logger

    def write(self, input):
        self.logger.write(f'WRITE -> {input}\r\n')
        self.child.write(input)

    def read_until(self):
        result = self.child.read_until()
        self.logger.write(f'READ  -> {result}\r\n')

        return result

    def flushInput(self):
        self.child.flushInput()
        self.logger.write(f'FLUSH -> Input\r\n')

    def read(self, size):
        result = self.child.read(size)
        self.logger.write(f'READ  -> {result}\r\n')

        return result