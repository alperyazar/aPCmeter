# Software

aPCmeter is a dummy display device. It needs a program running on computer to get CPU and RAM usages. The communication between the device and the computer implements basic commands. Therefore, you can easily create your own software. Here, I share an example program written in Python **2** written in 2016. If you are a Windows user, you can run the `.exe` directly.

The Python script is just an example but does the job. Remember that it is Python 2 script, not Python 3.

The script gets all available COM ports and connects them one by one until it finds the aPCmeter. After successful identification, CPU and RAM usages reported by OS is sent over COM port.

* `aPCmeterHpre.exe` Ready to run Windows application
* `aPCmeterHpre.py` Python 2 source code, should run on all major OSes.