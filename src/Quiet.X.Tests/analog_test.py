import quiet_coms

def analog_stability_test(quite: quiet_coms.QuietComs, number_mode='DECI'):

    if number_mode == 'DECI' or number_mode == 'HEX':
        # Changed the number mode
        quite.write(f'SYST:NUMB {number_mode}')
    else:
        raise Exception(f'Invalid Number mode {number_mode}. Must be \'DECI\' or \'HEX\'')

    print('Analog Input Stability Test')

    aMax = [0, 0, 0, 0]
    aMin = [ 10000, 10000, 10000, 10000]
    for i in range(1, 10000):

        response_raw = quite.query('ANAI:CH1?;CH2?;CH3?;CH4?').split(';')

        i = 0
        for response in response_raw:
            r = int(response[2:], 16) if number_mode == 'HEX' else int(response, 10)

            if r < aMin[i]:
                aMin[i] = r
            if r > aMax[i]:
                aMax[i] = r

            i += 1

    for i in range(0, 4):
        print(f'{i + 1}  =>  Max: ' + str(aMax[i]).ljust(8) +  
        f'Min: ' + str(aMin[i]).ljust(8) + f'Range: {aMax[i] - aMin[i]}')

    print('Analog Input Stability Test Complete')

if __name__ == "__main__":

    qPorts = quiet_coms.find_quiet_ports()

    quite = quiet_coms.QuietComs(port=qPorts[0])

    if analog_stability_test(quite):
        print("All Tests Passed")