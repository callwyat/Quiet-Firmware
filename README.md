# Qy@ Board
## Hardware Overview
The Qy@ (Quiet) Board was designed to add easy IO to a computer. It connects to a computer via USB as a virtual COM Port, or can be connected though a serial adapter as well (pending). The Qy@ Board has 8 Digital Inputs, 4 12-bit analog inputs sampled at 1k Hz, 8 Digital Outputs and 2 Analog Outputs. Four of the Digital Outputs can be used in PWM Mode, and all outputs (the 2 Analog and 8 Digital) can be used in Servo Mode enabling control of 10 servo motors at once. The Qy@ Board also has a SPI, and UART Port that can be used for small communication applications.

## Firmware Overview
Commands are formatted using the SCPI (Standard Communication for Programable Instruments) format.

## Command Tree
```scpi
├———:*IDN?
├———:DIGInput?
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
│   └———:BAUD
├———:SPI
│   ├———:EXCHange
│   ├———:CS
│   └———:BAUD
├———:SYSTem
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

## Command Breakdown
### *IDN? (Query Only)
#### Description
A required SCPI command that returns the identity of the device. The response is formatted as follows `{Manufacture Name},{Product Name},{Serial Number},{Firmware Version}`. Unfortunately, the serial number can not be programed in and instead must be set by the user using `:SYST:SERI` command.

#### Example
```
Write -> *IDN?
Read  -> Y@ Technologies,Qy@ Board,{Serial Number},2.x
```
### DIGInputs? (Query Only)
#### Description
Quires all the Digital inputs and returns the hexadecimal value.

#### Example
```
Write -> DIGI?
Read  -> 0xFF
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
Read  -> 9600
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

These settings are read and applied on power-up
#### Example
```
Write -> SYST:SAVE
```

### SYSTem:RESTorstate [FACTory] (Write Only)
#### Description
Reads and applies all the settings stored in non-volatile memory effectively putting the board back into the power on state. Optionally the `FACTory` parameter can be given that will restore the board back to the settings used when first programed.
#### Example
```
SYST:REST
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
Returns the number of micro seconds it took to execute the last command. Useful if trying to optimise commands.

#### Example
```
Write -> ANAI:CH1?
Read  -> 2045
Write -> DIAG?
Read  -> 1096
```