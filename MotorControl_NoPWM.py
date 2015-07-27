from pyb import Timer
from pyb import Pin
from network import WLAN
from pyb import Sleep

def Setup_Pins()
 # initialize GPIO7 and GPIO 8 in gpio mode (af=0) and make output
 # These will be the standard pins required for motor control
 LMotorA = Pin('GPIO7', af=0, mode=Pin.OUT)
 RMotorA = Pin('GPIO8', af=0, mode=Pin.OUT)
 # PWM pins, although not configured in this iteration of code
 LMotorB = Pin('GPIO9', af=0, mode=Pin.OUT)
 RMotorB = Pin('GPIO10', af=0, mode=Pin.OUT)
 LMotorA.low()
 RMotorA.low()
 LMotorB.low()
 RMotorB.low()

def MotorTest()
 print "Turning motors on"
 LMotorA.high()
 RMotorA.high()
 LMotorB.low()
 RMotorB.low()
 sleep(2)
 
 print "Stopping motor"
 LMotorA.low()
 RMotorA.low()
 LMotorB.low()
 RMotorB.low())
 sleep(2)


Setup_Pins()

While True:
 MotorTest()
