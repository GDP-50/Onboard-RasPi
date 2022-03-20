from C_wrappers import *
from ctypes import *

MD25Address = c_int(0x58)
setupI2C(MD25Address)
setMotorSpeed(100, 200)