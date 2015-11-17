import pyb

#initialise servo. Rename to math leg joints
servo1 = pyb.Servo(1)
servo2 = pyb.Servo(2)
servo3 = pyb.Servo(3)
servo4 = pyb.Servo(4)

#Simple ot position angles using the .angle function.
#First number = angle, Second Number = time taken (milliseconds)
servo1.angle(50, 1000)
