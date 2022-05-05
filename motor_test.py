from C_wrappers import *
from ctypes import *
import time

endMD49Serial()
MD49SerialInit()
MD49SoftwareVersion()
#setMode(c_char(0))
#MD49VI()
startTime = time.time()
while time.time() - startTime < 0.5:
    driveMotors(c_char(150),c_char(150))

driveMotors(c_char(128), c_char(128))
endMD49Serial()
exit()