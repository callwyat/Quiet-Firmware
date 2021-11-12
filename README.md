# Qy@ Board
## Hardware Overview
The Qy@ (Quiet) Board was designed to add easy IO to a computer. It connects to a computer via USB as a virtual COM Port, or can be connected though a serial adapter as well (pending). The Qy@ Board has 8 Digital Inputs, 4 12-bit analog inputs sampled at 1k Hz, 8 Digital Outputs 4 and 2 Analog Outputs. Four of the Digital Outputs can be used in PWM Mode, and all outputs (the 2 Analog and 8 Digital) can be used in Servo Mode enabling up to 10 servo motors at once. Additionally, there is a SPI Port that can be used for IO extension.

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
│   └———:WRITe
│   └———:READ
│   └———:BAUD
├———:SPI
│   └———:EXCHange
│   └———:CS
│   └———:BAUD
├———:SYSTem
│   └———:SERIalNumber
│   └———:SAVEState
│   └———:RESToreState
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

### DIGO:CHannel\<N> \<output_value>
#### Parameters
- n -> The index of the channel to use
    - Range -> 1 to 8

## Output Commands
### \<TYPE>:CHannel\<N> \<output_value>
#### Parameters
- n -> The index of the channel to use

### Description
Gets or sets the value of an individual output. The input and output of this command is depended on the outputs `MODE` setting. 

### \<TYPE>:CHannel\<N>:MODE \<output_mode>
#### Parameters
- n -> The index of the channel to use

#### Description
Gets or sets the outputs mode.
- `DISCreet` the output is either turned on or off. `0` will turn the    output off, and any other number will turn the output on.
    - Range 0 to 1
- `PWM` the output will operate as a 1kHz Pulse Width Modulation output with 10 bits of resolution
    - Range 0 to 1023 or 0x000 to 0x3FF
- `SERVo` the output will operate as a 20 mS period Pulse Width Modulated output with 10 bits of resolution between 1 mS and 2 mS
    - Range 0 to 1023 or 0x000 to 0x3FF

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