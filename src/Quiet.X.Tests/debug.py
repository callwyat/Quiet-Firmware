from email.charset import QP
from serial import Serial
from serial.tools.list_ports import comports


qPort = None
for p in list(comports()):        
    if p.product and 'FT232R' in p.product:
        qPort = p.device
        break


if not qPort:
    raise Exception('No Quiet Board found')

quite = Serial(qPort, 57600, timeout=1)

loops = 0
while True:
    loops += 1
    quite.write('DIGI?\r\n'.encode())
    read = quite.read_until()

    if len(read) <= 0:
        raise Exception(f'Failed after {loops} rounds')
