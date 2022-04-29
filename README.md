# Qy@ Board
## Hardware Overview
The Qy@ (Quiet) Board was designed to add easy IO to a computer. It connects to a computer via USB as a virtual COM Port. The Qy@ Board has 8 Digital Inputs, 4 12-bit analog inputs sampled at 1k Hz, 8 Digital Outputs and 2 Analog Outputs. Four of the Digital Outputs can be used in PWM Mode, and all outputs (the 2 Analog and 8 Digital) can be used in Servo Mode enabling control of 10 servo motors at once. The Qy@ Board also has a SPI, UART, and I2C Port that can be used for small communication applications.

## Firmware Overview
Commands are formatted using the SCPI (Standard Communication for Programable Instruments) format.

## Command Tree
```scpi
:
├———:*IDN?
├———:*RST
├———:DIGInput?
│   └———:CHannel<n>
│       └———:VALUe
├———:ANAInput
│   └———:CHannel<n>
├———:DIGOutput
│   └———:CHannel<n>
│       ├———:VALUe
│       └———:MODE
├———:ANAOutput
│   └———:CHannel<n>
│       ├———:VALUe
│       └———:MODE
├———:PWM
│   └———:CHannel<n>
│       ├———:VALUe
│       └———:MODE
├———:SERVo
│   └———:CHannel<n>
│       ├———:VALUe
│       └———:MODE
├———:UART
│   ├———:WRITe
│   ├———:READ
│   ├———:BAUD
│   ├———:MODE
│   └———:OVERflow
│       └———:CLEAr
├———:SPI
│   ├———:EXCHange
│   ├———:CS
│   └———:BAUD
├———:IIC (I2C)
│   ├———:MODE
│   ├———:BAUD
│   ├———:TIMEout
│   ├———:ADDRess
│   ├———:REGIster
│   │   ├———:ADDRess
│   │   ├———:RSIZe
│   │   ├———:WRITe
│   │   ├———:READ
│   │   └———:ACKnowledged
│   ├———:WRITe
│   ├———:READ
│   └———:ACKnowledged
├———:SYSTem
│   ├———:ERRor
│   │   ├———:NEXT
│   │   └———:CLEAr
│   ├———:SERIalNumber
│   ├———:SAVEState
│   ├———:RESToreState
│   ├———:NUMBerformat
│   └———:INFOrmation
│       └———:COMMit
│       │   ├———:HASH
│       │   ├———:AUTHor
│       │   └———:DATEtime
│       └———:BUILd
│           ├———:VERSion
│           ├———:USERname
│           └———:DATEtime
└———DIAGnostics
```

## Command Descriptions
### *IDN? (Query Only)
#### Description
A required SCPI command that returns the identity of the device. The response is formatted as follows `{Manufacture Name},{Product Name},{Serial Number},{Firmware Version}`. Unfortunately, the serial number can not be programed in but can be manually set by the user with the `:SYST:SERI` command.
#### Example
```
Write -> *IDN?
Read  -> Y@ Technologies,Qy@ Board,{Serial Number},2.2
```

### *RST (Write Only)
#### Description
A required SCPI command. Preforms the `SYSTem:RESTorestate` action followed by the `SYSTem:ERRor:CLEAr` action
#### Example
```
Write -> *RST
```

### DIGInputs? (Query Only)
#### Description
Quires all the digital inputs and returns binary weighted value

#### Example
```
Write -> DIGI?
Read  -> 0xFF
```

### DIGInputs:CHannel\<n>? (Query Only)
#### Description
Quires the value of a single digital input specified by `n`

#### Example
```
Write -> DIGI:CH1?
Read  -> 1
```

### DIGInputs:CHannel\<n>:VALUe? (Query Only)
#### Description
Quires the value of a single digital input specified by `n`

#### Example
```
Write -> DIGI:CH8:VALUe?
Read  -> 1
```

### ANAInputs:CHannel\<N>? (Query Only)
#### Parameters
- n -> The index of the channel to use
    - Range -> 1 to 4

#### Description
Queries the last value for the given analog input channel

#### Example
```
Write -> ANAI:CH1?
Read  -> 4095
```

### DIGO \<output_value>
#### Description
Gets or sets the discreet value of all eight digital outputs

#### Value Range
0 - 255 or 0x00 - 0xFF

#### Examples
```
Write -> DIGO 255
Write -> DIGO 0xAA
Write -> DIGO?
Read  -> 0xAA
```

## Output Commands
All the output groups,`DIGOutput`, `PWM`, and `SERVo`, have the same command tree. "Type" is used to mean any three of these options.
### \<TYPE>:CHannel\<N>[:VALUe] \<output_value>
#### Parameters
- N -> The index of the channel to use
- output_value -> A number between 0 and 1023 to output

### Description
Gets or sets the value of an individual output. The input and output of this command is depended on the outputs `MODE` setting. 

### \<TYPE>:CHannel\<N>:MODE DISCreet|PWM|SERVo
#### Parameters
- N -> The index of the channel to use
- output_mode -> The desired way to use the output
    - `DISCreet` the output is either turned on or off. `0` will turn the    output off, and any other number will turn the output on.
        - Range 0 to 1
    - `PWM` the output will operate as a 1kHz Pulse Width Modulation output with 10 bits of resolution
        - Range 0 to 1023 or 0x000 to 0x3FF
    - `SERVo` the output will operate as a 20 mS period Pulse Width Modulated output with 10 bits of resolution between 1 mS and 2 mS
        - Range 0 to 1023 or 0x000 to 0x3FF

#### Description
Gets or sets the output mode for the givin output.

#### Examples
```
Write -> DIGO:CH7:MODE DISC
Write -> DIGO:CH3:MODE PWM
Write -> ANAO:CH1:MODE SERVo
Write -> PWM:CH6:MODE PWM
Write -> SERV:CH10:MODE SERVo
Write -> DIGO:CH8:MODE?
Read  -> SERV
```

### UART:WRITe (IEEE Write Only)
#### Description
Writes number of bytes specified by the IEEE Header to the UART transmit buffer
#### Example
```
Write -> UART:WRIT #2160123456789ABCDEF
```

### UART:READ? (IEEE Query Only)
#### Description
Return an IEEE Header with how many bytes have been buffered and a ready to read followed by the buffer
#### Example
```
Write -> UART:READ
Read  -> #2160123456789ABCDEF
```

### UART:BAUD
#### Description
Gets or sets the baud rate for the UART port. Valid baud rates are from 60 to 4M. Higher baud rates will result in less accurate timings. Baud rates above 115200 can easily overflow the input buffer and are not recommended if reading.
#### Example
```
Write -> UART:BAUD 115200
Write -> UART:BAUD 9600
Write -> UART:BAUD?
Read  -> 9614
```

### UART:MODE USBUart|SCPI
#### Description
Gets or sets the active mode for the UART Port
- USBUart: The port will act as a USB to TTY UART adapter. Use the WRITe command to send data, and the READ command to receive data
- SCPI: Data received on this port will be interpreted as a SCPI command. The UART commands are not available to this input
#### Example
```
Write -> UART:MODE SCPI
Write -> UART:MODE?
Read  -> SCPI
```

### SPI:EXCHange (Combination IEEE Write and Read)
#### Description
Writes the number of bytes specified by the IEEE header to the Serial Peripheral Interface (SPI). SPIs uses a synchronized shift register to clock bits in and out simultaneously. As such, every byte written to the SPI will return one byte.
#### Example
```
Write -> SPI:EXCH #2160123456789ABCDEF
Read  -> #2160123456789ABCDEF
```

### SPI:CS \<value>
#### Description
This command functions similar to a digital output. Gets or sets the output state of the chip select for the SPI. A value of `0` of `False` will pull the chip select low, and a value of `1` or `True` will pull the chip select high.
#### Example
```
Write -> SPI:CS 0
Write -> SPI:EXCH #14FFFF
Read  -> #140000
Write -> SPI:CS 1
Write -> SPI:CS?
Read  -> 1
```

### SPI:BAUD \<value>
#### Description
Gets or sets the baud rate of the SPI module. Acceptable baud rates are: 4000000, 2000000, 1000000, and 250000.
#### Example
```
Write -> SPI:BAUD 250000
Write -> SPI:BAUD?
Read  -> 250000
```

### IIC:MODE (OFF|MASTer)
#### Description
Gets or sets active mode for the I2C Module.
#### Notes
The I2C modules uses the same pins as digital outputs 5 and 6. As such, these two pins must be cleared of any loads greater then 1mA to work. This includes the status LED for the output, and the Opto-Coupler Input.

To use the I2C port ensure the following items are checked on digital outs 5 and 6:
- The Opto-Couplers is not connected
- The status LEDs are disabled by opening JP8
- There are pull-up resister installed on the bus
    - R8 and R9 can be used
    - The recommended pull-up size is between 1k and 10k ohms
#### Example
```
Write -> IIC:MODE MAST
Write -> IIC:MODE?
Read  -> MAST
```

### IIC:BAUD \<value>
#### Description
Gets or sets the baud rate for the I2C module to use. The default value is 100k. The acceptable range is from 16k and 1M. 
#### Example
```
Write -> IIC:BAUD 400000
Write -> IIC:BAUD?
Read  -> 400000
```

### IIC:TIMEout \<value>
#### Description
Gets or sets the max amount of time, in milliseconds, that a transfer can take. This is useful if a slave device uses clock stretching. Acceptable values are between 10 and 255. The default value is 128.
#### Example
```
Write -> IIC:TIME 100
Write -> IIC:TIME?
Read  -> 100
```
### IIC:ACKnowledged? (Query Only)
#### Description
Returns `1` if the last IIC operation was properly acknowledged by an attached device, or `0` if not.
#### Example
```
Write -> IIC:REGI:WRIT 0x1001;ACK?
Read  -> ;1
```

### IIC:REGIster:ADDRess \<value>
#### Description
Gets or sets the register address for a IIC:REGI:WRITe or IIC:REGI:READ? command.
#### Example
```
Write -> SYST:NUMB HEX
Write -> IIC:REGI:ADDR 0xFF
Write -> IIC:REGI:ADDR?
Read  -> 0xFF
```

### IIC:REGIster:RSIZe \<value>
#### Description
Gets or sets the size, in bytes, of the register to read or write to. Valid values are 1 and 2. The default value is 1.
#### Example
```
Write -> IIC:REGI:RSIZ 2
Write -> IIC:REGI:RSIZ?
Read  -> 2
```

### IIC:REGIster:WRITe \<value>
#### Description
Attempts the following actions with the I2C System:
- Starts I2C transfer
- Writes the Slave address specified by `IIC:ADDRess`
- Writes the Register address specified by `IIC:REGIster:ADDRess`
- Writes the first byte of the given value
- Writes the second byte of the given value if `IIC:REGIster:RSIZe` is 2
- Ends I2C transfer

#### Example
```
Write -> IIC:ADDR 0x50
Write -> IIC:REGI:ADDR 0x01
Write -> IIC:REGI:RSIZ 2
Write -> IIC:REGI:WRIT 0x1001
Write -> IIC:ACK?
Read  -> 1

Write -> IIC:ADDR 0x50;REGI:ADDR 0x01;RSIZ 2;WRIT 0x1001;ACK?
Read  -> ;;;;1
```

### IIC:REGIster:READ?
#### Description
Attempts the following actions with the I2C System:
- Starts I2C transfer
- Writes the Slave address specified by `IIC:ADDRess`
- Writes the Register address specified by `IIC:REGIster:ADDRess`
- Restarts the I2C transfer
- Writes the Slave address specified by `IIC:ADDRess`
- Reads one byte from the slave
- Reads a second byte from the slave if `IIC:REGIster:RSIZe` is 2
- Ends I2C transfer
- Returns little ended value of the two bytes

#### Example
```
Write -> SYST:NUMB HEX
Write -> IIC:ADDR 0x50
Write -> IIC:REGI:ADDR 0xFF
Write -> IIC:REGI:RSIZ 2
Write -> IIC:REGI:READ?
Read  -> 0x1004
Write -> IIC:ACK?
Read  -> 0x01

Write -> IIC:ADDR 0x50;REGI:ADDR 0x01;RSIZ 2;READ?;ACK?
Read  -> ;;;0x1004;0x01
```

### IIC:WRITe (IEEE Write Only)
#### Description
Write the given data block to the device specified by `IIC:ADDR`

#### Example
```
Write -> IIC:ADDR 0x50;
Write -> IIC:WRIT #13ABC

Write -> IIC:ADDR 0x0C;WRIT #13ABC
```

### IIC:READ? \<count> (IEEE Read Only)
#### Description
Reads the number of bytes specified by "count" from the slave device specified by `IIC:ADDR`.

#### Example
```
Write -> IIC:ADDR 0x50;
Write -> IIC:WRIT #11A
Write -> IIC:READ? 2
Read  -> #12BC

Write -> IIC:ADDR 0x0C;WRIT #11A;READ? 2
Read  -> ;;#2BC
```

### SYSTem:ERRor\[:NEXT\]?
#### Description
Queries all the the error systems and returns a summary of all the errors plus amy system specific errors. The system specific errors are cleared after the message is read. The summary bits are cleared by reading the corresponding systems error code e.g. `IIC:ERR?`

#### System Error Codes
Code      | Meaning                                           |
----------|-----------------------------------------------    |
0x0000    | No Errors                                         |
0x0001    | Error Buffer Overflow                             |
0x0010    | Attempted to set invalid number mode              |
0x0011    | Invalid SYST:REST parameter                       |
0x0020    | Invalid Serial Number type (quotes are needed)    |
0x0021    | Provided Serial Number was too long               |
-----     |                                                   |
0x0101    | Invalid Command                                   |
0x0102    | Invalid Command Branch (may be query only)        |
0x0110    | Failed to parse value as a number (uints only)    |
0x0111    | Invalid IEEE Header                               |
0x0120    | Failed to parse the given value as a boolean      |
-----     |                                                   |
0x0201    | DIGI: Invalid channel (Valid range is 1-8)        |
-----     |                                                   |
0x0301    | ANAI: Invalid channel (Valid range is 1-4)        |
-----     |                                                   |
0x0401    | DIGO: Invalid Channel (Valid range is 1-8)        |
0x0402    | DIGO: Invalid mode for the given Channel          |
0x0403    | DIGO: Invalid value for the given Channel         |
-----     |                                                   |
0x0501    | ANAO: Invalid Channel (Valid range is 1-2)        |
0x0502    | ANAO: Invalid mode for the given Channel          |
0x0603    | ANAO: Invalid value for the given Channel         |
-----     |                                                   |
0x0601    | PWM: Invalid Channel (Valid range is 1-6)         |
0x0602    | PWM: Invalid mode for the given Channel           |
0x0603    | PWM: Invalid value for the given Channel          |
-----     |                                                   |
0x0701    | SERVo: Invalid Channel (Valid range is 1-10)      |
0x0702    | SERVo: Invalid mode for the given Channel         |
0x0703    | SERVo: Invalid value for the given Channel        |
-----     |                                                   |
0x0910    | UART: Invalid baud rate (Valid range 60 - 1M)     |
0x0911    | UART: Invalid mode given                          |
0x0912    | UART: Attempted less then 1 byte                  |
0x0920    | UART: Receive buffer overflow                     |
0x0930    | UART: Attempted write while not in USBUart mode   |
0x0931    | UART: Attempted read while not in USBUart mode    |
-----     |                                                   |
0x0A01    | SPI: Invalid baud rate (Valid range 250k - 4M)    |
-----     |                                                   |
0x0B01    | IIC: Invalid baud rate (Valid range 16k - 1M)     |
0x0B02    | IIC: Invalid timeout                              |
0x0B03    | IIC: Invalid slave address                        |
0x0B04    | IIC: Invalid operation mode                       |
0x0B10    | IIC: Attempted to write with I2C disabled         |
0x0B11    | IIC: Attempted to read with I2C disabled          |
0x0B20    | IIC: Invalid register size                        |
0x0B21    | IIC: Invalid register address                     |
0x0B22    | IIC: Invalid register value                       |
0x0B30    | IIC: Buffer overflow would occur                  |
0x0B31    | IIC: Invalid number of bytes to write specified   |
0x0B32    | IIC: Invalid number of bytes to read specified    |

#### Example
```
Write -> SYST:ERR?
Read  -> 0
Write -> SYST:ERR:NEXT?
Read  -> 0
```

### SYSTem:ERRor:CLEAr
#### Description
Clears the error buffer of all errors

#### Example
```
Write -> SYST:ERR:CLEA
```

### SYSTem:SERIalnumber "\<value>"
#### Description
Gets the stored serial number. As there is no unique identifiers built into the hardware of the Qy@ Board, this command can also be used to set a Serial Number. The maximum length of the serial number is 16 charters. The serial number is also reported by the `*IDN?` command
#### Example
```
Write -> SYST:SERI "2022011909300901"
Write -> SYST:SERI?
Read  -> "2022011909300901"
```

### SYSTem:SAVEstate (Write Only)
#### Description
Stores the current mode and value of each output and the following settings into non-volitive memory
- SYST:NUMB
- UART:BAUD
- SPI:BAUD
- IIC:BAUD
- IIC:TIME

These settings are read and applied on power-up
#### Example
```
Write -> SYST:SAVE
```

### SYSTem:RESTorestate [FACTory] (Write Only)
#### Description
Reads and applies all the settings stored in non-volatile memory effectively putting the board back into the power on state. Optionally the `FACTory` parameter can be given that will restore the board back to the settings used when first programed.
#### Example
```
Write -> SYST:REST
Write -> SYST:REST FACT
```

### SYSTem:NUMBerformat HEX|DECImal
#### Description
Sets the format for all numbers read
#### Example
```
Write -> SYST:NUMB DECI
Write -> ANAI:CH1?
Read  -> 1023
Write -> SYST:NUMB HEX
Write -> ANAI:CH1?
Read  -> 0x3FF
Write -> SYST:NUMB?
Read  -> HEX
```
### DIAGnostics? (Query Only)
#### Description
Returns the number of micro seconds it took to execute the last command. Useful if trying to optimize commands.

#### Example
```
Write -> ANAI:CH1?
Read  -> 2045
Write -> DIAG?
Read  -> 1096
```