from base64 import encode
from ctypes import *
import numpy as np
from numpy.ctypeslib import ndpointer
import os

## IMPORT LIBRARY ##
lib_file = os.path.join(os.curdir, "c_dll.so")
c_funcs = cdll.LoadLibrary(lib_file)

## LOAD COURSE ##
loadCourse = c_funcs.loadCourse
loadCourse.argtypes = [c_char_p(),
                       POINTER(c_int),
                       POINTER(c_int),
                       ndpointer(dtype=c_int, ndim=1, flags='C_CONTIGUOUS')]

## PREPARE COURSE ##
prepareCourse = c_funcs.prepareCourse
prepareCourse.argtypes = [c_char_p(),
                          c_int,
                          ndpointer(dtype=c_double, ndim=2, flags='C_CONTIGUOUS'),
                          c_int,
                          c_int,
                          ndpointer(dtype=c_double, ndim=3, flags='C_CONTIGUOUS'),
                          ndpointer(dtype=c_int, ndim=1, flags='C_CONTIGUOUS'),
                          ndpointer(dtype=c_double, ndim=1, flags='C_CONTIGUOUS'),
                          POINTER(c_double)]

## SETUP I2C ##
setupI2C = c_funcs.setupI2C
setupI2C.argtypes = [c_int]
setupI2C.restypes = [c_int]

## READ I2C ##
readI2C = c_funcs.readI2C
readI2C.argtypes = [c_int]
readI2C.restypes = [c_int]

## WRITE I2C ##
writeI2C = c_funcs.writeI2C
writeI2C.argtypes = [c_int]
writeI2C.restypes = [c_int]

## SET MOTOR SPEED ##
setMotorSpeed = c_funcs.setMotorSpeed
setMotorSpeed.argtypes = [c_int, c_int]

## ENCODER VALUES ##
encoderValues = c_funcs.encoderValues
encoderValues.argtypes = [POINTER(c_long), POINTER(c_long)]

## RESET ENCODER VALUES ##
resetEncoderValues = c_funcs.resetEncoderValues