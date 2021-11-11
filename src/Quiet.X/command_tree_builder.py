

class Command:

    def __init__(self, name, takes_index, children = None) -> None:
        
        self.name = name
        
        self.children = children

        self.handle = name.strip('*') + 'Command'

        self.takes_index = takes_index

        self.lower = None
        self.upper = None

        self.command_hash = 0

        for c in name.encode():
            self.command_hash += c

            if self.command_hash >= 256:
                self.command_hash -= 256

    def __str__(self):
        return f'DEFINE_COMMAND({self.name}, {self.command_hash}, {self.handle})'

commands = [
    Command('*IDN', False),
    Command('DIGI', False),
    Command('ANAI', True, [ 
        Command('CH', True) 
    ]),
    Command('DIGO', False, [ 
        Command('CH', True, [
            Command('VALU', False),
            Command('MODE', False),
        ]
    )]),
    Command('ANAO', False, [ 
        Command('CH', True, [
            Command('VALU', False),
            Command('MODE', False),
        ]
    )]),
    Command('PWM', False, [ 
        Command('CH', True, [
            Command('VALU', False),
            Command('MODE', False),
        ]
    )]),
    Command('SERV', False, [ 
        Command('CH', True, [
            Command('VALU', False),
            Command('MODE', False),
        ]
    )]),
    Command('SYST', False, [
        Command('SERI', False),
        Command('SAVE', False),
        Command('REST', False),
    ]),
    Command('UART', False, [
        Command('WRIT', False),
        Command('READ', False),
    ]),
    Command('SPI', False, [
        Command('EXCH', False),
        Command('CS', False),
        Command('BAUD', False)
    ]),
]

def get_hash(command):
    return command.command_hash

def tree(commands):

    half_size = len(commands) >> 1
    quarter_size = half_size >> 1

    command = commands[half_size]

    command.lower = commands[half_size - quarter_size]
    command.upper = commands[half_size + quarter_size]
    
if __name__ == '__main__':

    commands.sort(key=get_hash)

    for x in commands:
        print(x)

    tree(commands)
