# Gauge Folder
This folder contains necessary drawings to customize gauges. Please refer to **HowToBuild** folder for detailed step by step instructions.

Gauges are designed for a specific type of gauge mentioned in **HowToBuild**. You have to scale designs if you are using gauges with different dimensions. 

I used 0-5 V analog DC multimeter as gauge. 0.5V is mapped to 0% and 4.5V is mapped tı 100% utilization. There are two reasons behind this decision. 1) In case of problem with aPCmeter it is possible to understand it easily because needles fall below 0% even if CPU utilization is near 0%. 2) Gauges are driven using PWM. Maximum output voltage of Arduino Nano is 5V too. However, I am not sure whether it is possible to obtain 5V while driving it 100% duty cycle. Also gauges are cheap and scale may not be correct. Therefore I mapped 4.5V to 100% not to have any scale problem.

## Files
* *.svg*: These are vector files of gauge desings. Gauges are designed using Inkscape. I didn't used Inkscape before. Simply I tried to follow this tutorial: http://goinkscape.com/how-to-create-a-tachometer-in-inkscape/ It is interesting that when I open this files again, numbers on the gauges seem to be missing. You should add these numbers if you want to edit this file. Sorry for this. I don't know whether it is my fault to save correct design folder or bug of Inkscape. Anyway, there are ready-to-print files. You don't have to open Inkscape. Inkscape part of this project is the harderst one for me.
* *ayazar_gauges_A4.pdf*: This is ready-to-print file for gauges. Just print it on white A4 paper and cut them by hand.
* *gauge_cpu.png*, *gauge_ram.png*: Gauge designs without my signature. Background is transparent. Be cautious about dimension, don't print it directly.
* *gauge_cpu_ayazar.png*, *gauge_ram_ayazar.png*: Gauge designs with my signature. Background is white. Be cautious about dimension, don't print it directly.

*C:16396*