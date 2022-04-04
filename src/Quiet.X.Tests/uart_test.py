import quiet_coms
import time

if 'EXIT_ON_FAIL' not in locals():
    EXIT_ON_FAIL = True

def uart_loopback_test(quite:quiet_coms.QuietComs):
    print('UART Tests')

    quite.write('UART:BAUD 115200')
    
    data = ''
    for i in range(0, 64):

        data += 'U'
        data_size = str(len(data))
        header_size = len(data_size)

        packet = f'#{header_size}{data_size}{data}'
        quite.write(f'UART:WRIT {packet}')

        time.sleep(0.001 * i)
        
        response = quite.query_raw('UART:READ?')

        if response != packet + '\r\n':
            fail_string = f'UART Test Failed at: {i}'
            print(fail_string)
            if EXIT_ON_FAIL:
                raise Exception(fail_string)

    print('UART Test Complete')


if __name__ == "__main__":

    qPorts = quiet_coms.find_quiet_ports()

    quite = quiet_coms.QuietComs(port=qPorts[0])

    uart_loopback_test(quite)

    print('All Test Complete')