from C_wrappers import *
from ctypes import *

MD25Address = c_int(0x58)
setupI2C(MD25Address)

writeI2C(13)
software = readI2C(1)
print("SOftware: ")
print(software)

setMotorSpeed(100, 200)
