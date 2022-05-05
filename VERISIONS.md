# 2.2.0
- Refactored the SCPI parser
    - Bugs fixed:
        - Commands longer then 64 characters long fail
        - Unable to send response more then 64 characters long
        - Commands sent within 1mS of each other would fail
    -  Consequences:
        - Command parsing takes ~twice as long
        - Infinite command length
        - Infinite response length

# 2.1.0
- Added the `SYSTem:ERRor` command and many many error codes to help users understand what is going wrong
    - Replaced `IIC:ERROr` with `IIC:ACKnowledge` and `SYSTem:ERRor`
    - Replaced `UART:ERROr` with `UART:OVERflow` and `SYSTem:ERRor`

# 2.0.0
- First full release of Version 2!
- Feature parity with Version 1 with the addition of
    - 4 additional Servo Outputs
    - Accepts commands from the UART Port (when `UART:MODE` is `SCPI`)
    - A user programable Serial Number
    - I2C register commands making I2C much easier to use
    