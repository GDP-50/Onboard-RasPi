from ctypes import *

dll = cdll.LoadLibrary("c_dll.so")

setupI2C = dll.setupI2C
setupI2C.argtypes = [c_int]
setMotorSpeed = dll.setMotorSpeed
setMotorSpeed.argtypes = [POINTER(c_long)]
