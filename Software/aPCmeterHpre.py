import sys
import glob
import serial
import time
import psutil

version ="pre1dev3"

available_ports=[];
connection_check_str = "aPCmeter"
ser = serial.Serial()

cpuPWM0 = 25
cpuPWM100 = 229
ramPWM0 = 25
ramPWM100 = 229

# http://stackoverflow.com/a/14224477/1766391
def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

def connect_to_aPCmeter(port_list):
    ser.baudrate = 9600
    ser.timeout = 3
    ser.dsrdtr = False
    ser.xonxoff = False
    ser.rtscts = False
    ser.parity = serial.PARITY_NONE
    ser.bytesize = serial.EIGHTBITS
    ser.stopbits = serial.STOPBITS_ONE
    
    for port in port_list:
        ser.port = port
        try:
            print "Trying " + port
            ser.open()
            ser.reset_input_buffer()
            ser.reset_output_buffer()
            read_value = ser.read(size=100)
            print read_value
            print type(read_value)
            print len(read_value)
            
            if (len(read_value)>9):
                if connection_check_str in read_value:
                    print "aPCmeter is found at " + port
                    initialize_aPCmeter()
            else:
                print "NO aPCmeter is found at" + port
                ser.close()
            
        except (OSError, serial.SerialException):
            pass

    return False

def initialize_aPCmeter():
    print "Starting the aPCmeter..."
    print ser.write('SsE'.encode('ascii'));
    ser.flush()
    read_value = ser.read(4)
    if (len(read_value)>3):
        if "OK" in read_value:
            print "aPCmeter is initialized."
            run_aPCmeter()
            
def run_aPCmeter():
    
    while True:
        ser.reset_input_buffer()
        ser.reset_output_buffer()
		
        cpu = psutil.cpu_percent(interval=1, percpu=False)
        ram = psutil.virtual_memory().percent
        
        cpu_pwm = int(cpuPWM0 + (cpu / 100.0) * (cpuPWM100 - cpuPWM0))
        ram_pwm = int(ramPWM0 + (ram / 100.0) * (ramPWM100 - ramPWM0))
        
        if cpu_pwm < 0:
            cpu_pwm = 0
        if cpu_pwm > 255:
            cpu_pwm = 255
        
        if ram_pwm < 0:
            ram_pwm = 0
        if ram_pwm > 255:
            ram_pwm = 255
            
        cpuPWMg = int(255 * cpu/100)
        cpuPWMr = 255 - cpuPWMg
        
        if cpuPWMr < 0:
            cpuPWMr = 0
        if cpuPWMr > 255:
            cpuPWMr = 255
        
        if cpuPWMg < 0:
            cpuPWMg = 0
        if cpuPWMg > 255:
            cpuPWMg = 255
        
        
        ramPWMg = int(255 * ram/100)
        ramPWMr = 255 - ramPWMg
        
        if ramPWMr < 0:
            ramPWMr = 0
        if ramPWMr > 255:
            ramPWMr = 255
        
        if ramPWMg < 0:
            ramPWMg = 0
        if ramPWMg > 255:
            ramPWMg = 255
        
          
        msg = 'P'
        msg += "%03d" % (cpuPWMr)
        msg += "%03d" % (cpuPWMg)
        msg += "%03d" % (cpu_pwm)
        msg += "%03d" % (ramPWMr)
        msg += "%03d" % (ramPWMg)
        msg += "%03d" % (ram_pwm)
        msg += 'pE'
        
        
        print "******"
        print "CPU %d" % (int(cpu))
        print "MEM %d" % (int(ram))
        print msg
        print ""
        
        ser.write(msg.encode('ascii'));
        ser.flush()
        read_value = ser.read(4)
        print read_value
        if "OK" not in read_value:
		    print 'Wrong ACK after RAM/CPU info'
		    return

if __name__ == '__main__':
    print "aPCmeterHpre " + version
    while True:
        print "Searching available ports..."
        available_ports = serial_ports()
        if not available_ports:
            print "No port is found"
        else:
            print 'Available Ports: ' + ', '.join(available_ports)
            if(connect_to_aPCmeter(available_ports) == False):
                print "There is no aPCmeter connected to this machine."
        
        
        try:
            ser.close()
        except e:
            pass
        time.sleep(1)