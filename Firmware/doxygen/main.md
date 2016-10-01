# What is aPCmeter? # {#mainpage}

aPCmeter is Arduino Nano 3.0 based device which shows CPU and RAM usage of PC in a vintage looking way. Here there are some features of the board. Although I included some photos of the hardware, this document is dedicated to the Arduino firmware.

@note For the most up-to-date information, please check the project webpage http://www.alperyazar.com/r/aPCmeter

@image latex aPCmeter_Promo.png

Since my wood working ability is very limited, it has a crude looking wood case.

@image latex aPCmeter_white_annotated_1024_721.jpg My Ugly aPCmeter
@image latex  aPCmeter_1.jpg
@image latex  aPCmeter_2.jpg
@image latex  aPCmeter_6.jpg

# The Overall System Structure # {#the_overall_system_structure}

The overall structure is very simple. A software running on PC computes CPU and RAM usage percentages. Notice that both CPU and RAM gauges have lighting. Each gauges has its own RGB LED. For example if CPU usage is low, its gauge is illuminated by only green LED. As CPU usage increases, it is illuminated by both red and green LED resulting a yellow light. In addition to CPU and RAM usage information, PC software calculates lighting. Gauge position and lighting information are independent for Arduino software. PC software sends data over USB based virtual COM port and Arduino displays them. This is how Arduino code works basically.

@note For available PC software, please check the project webpage http://www.alperyazar.com/r/aPCmeter Wtihout a proper PC software, aPCmeter hardware can't work properly.