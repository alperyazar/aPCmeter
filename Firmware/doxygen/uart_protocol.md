# UART Protocol # {#uart}

UART protocol works on 9600/8-N-1.

aPCmeter is a command driven device. In other words, PC software sends commands to aPCmeter, it does its job and acknowledges the PC software. Next command should send after the previous one is acknowledged. Each command starts with a capital letter followed by data of command and ends with small case of the starting letter. If it is desired after the last small case letter 'E' character can be send. E stands for end. When aPCmeter receives the 'E' character, it starts to parse the command immediately. If no 'E' is sent after the command, aPCmeter may wait at most 2 seconds before start to process the sent command. It is advised to end all comands with 'E' to trigger the command processing.

Here is the list of all possible messages:

## Hello Mesage (Dir: aPCmeter -> PC)
A hello message is sent from aPCmeter when it is in Wait or Silent Wait state periodically. Here is the message: "aPCmeter\r\n", total of 10 bytes. If PC software ready, it should respond with Start Message.

## Start Message (Dir: PC -> aPCmeter)
This message is meaningful when aPCmeter is in Wait or Silent Wait state. It should be send from the PC when aPCmeter sends the Hello Message. It consists of 2 (+1 optional ending) bytes

Byte 1 | Byte 2 | Byte 3 
------ | ------ | ----------------
'S'    | 's'    | 'E' (Optional)

Response from the aPCmeter:

Byte 1 | Byte 2 | Byte 3 | Byte 4 
------ | ------ | ------ | ------
'O'    | 'K'    | '\\r'   | '\\n'

After the response, LEDs of both gauges stay at green. aPCmeter enters to the Active state and the all following commands may be send without any particular order. In Active state, the PC software should send the following commands with period no less than 2 seconds.

## PWM Message (Dir: PC -> aPCmeter)
This is the main message to use aPCmeter. It is used to change PWM values of gauges and LEDs. Here is the structure

Byte Number | Message |
------ | ------ 
1   | 'P'
2-4 | PWM value of red LED of CPU gauge
5-7 | PWM value of green LED of CPU gauge
8-10 | PWM value of CPU gauge
11-13 | PWM value of red LED of RAM gauge
14-16 | PWM value of green LED of RAM gauge
17-19 | PWM value of RAM gauge
20 | 'p'
21 | 'E' (Optional)

Each PWM value is 3 byte length string without null termination like "100", "001", "245". Values should between 0 and 255 (including the both limits) otherwise they are truncated. All PWM values are updated with this command. This is the way to change color and position of gauges.

For LEDs, "000" corresponds to full bright LED and "255" corresponds to completely off LED. For gauges, it is the opposite. "000" corresponds to no movement whereas "255" corresponds to full deflection.

Response from the aPCmeter:

Byte 1 | Byte 2 | Byte 3 | Byte 4 
------ | ------ | ------ | ------
'O'    | 'K'    | '\\r'   | '\\n'

## Test Message(Dir: PC -> aPCmeter)
It is used to start built-in self-test routine. In this routine both gauges are moved slowly and color of gauges are changed slowly. This may be used to check functional operation of aPCmeter.

Byte 1 | Byte 2 | Byte 3 
------ | ------ | ----------------
'T'    | 't'    | 'E' (Optional)

Response from the aPCmeter after self-test:

Byte 1 | Byte 2 | Byte 3 | Byte 4 
------ | ------ | ------ | ------
'O'    | 'K'    | '\\r'   | '\\n'

## Blue Message(Dir: PC -> aPCmeter)

Blue LEDs are not PWM driven and they are not controlled by the PWM command. There are seperate command for blue LEDs just to turn them on or off seperately. 

Byte 1 | Byte 2 | Byte 3 | Byte 4 | Byte 5 
------ | ------ | ------ | ------ | ------
'B'    | 'B'    | CPU Blue LED     | RAM Blue LED | 'E' (Optional)

x Blue LED: Controls the blue LED of the x gauge. If the corresponding byte is '1', the corresponding blue LED becomes on. Similarly, if the byte is '0' the corresponding LED becomes off.

Response from the aPCmeter:

Byte 1 | Byte 2 | Byte 3 | Byte 4 
------ | ------ | ------ | ------
'O'    | 'K'    | '\\r'   | '\\n'

## Reset Message(Dir: PC -> aPCmeter)
It is used to reset the aPCmeter. State is changed from Active to Reset after this command.

Byte 1 | Byte 2 | Byte 3 
------ | ------ | ----------------
'R'    | 'r'    | 'E' (Optional)

Response from the aPCmeter:

Byte 1 | Byte 2 | Byte 3 | Byte 4 
------ | ------ | ------ | ------
'O'    | 'K'    | '\\r'   | '\\n'

## Keep Alive Message(Dir: PC -> aPCmeter)
It is used to avoid _No CMD Timouet_. As stated previously, PC software should send messages to aPCmeter periodically to avoid timeout. If PC software has nothing to say, it can just send Keep Alive Message. It does nothing except avoiding timeout.

Byte 1 | Byte 2 | Byte 3 
------ | ------ | ----------------
'K'    | 'k'    | 'E' (Optional)

Response from the aPCmeter:

Byte 1 | Byte 2 | Byte 3 | Byte 4 
------ | ------ | ------ | ------
'O'    | 'K'    | '\\r'   | '\\n'