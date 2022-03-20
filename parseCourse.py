from ctypes import *
import numpy as np
from numpy.ctypeslib import ndpointer
lib_file = "c_dll.so"
c_funcs = cdll.LoadLibrary(lib_file)
loadCourse = c_funcs.loadCourse
loadCourse.argtypes = [c_char_p(),
                       POINTER(c_int),
                       POINTER(c_int),
                       ndpointer(dtype=c_int, ndim=1, flags='C_CONTIGUOUS')]

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

printMat = c_funcs.print_matrix
printMat.argtypes = [c_int, c_int, ndpointer(dtype=c_int, ndim=2, flags="C_CONTIGUOUS")]

mat = np.zeros(shape=(5,5)).astype(c_int)
mat[0][3] = 10
mat[2][2] = 4
print("printing")
print(mat)
print("done printing")
c_int_p = POINTER(c_int)
mat_p = np.ascontiguousarray(mat).ctypes.data_as(POINTER(c_int))

printMat(5,5, mat)

coursePath = "green3.poly"
greenSize = c_int(0)
greenPos = np.zeros(3, dtype=c_double)
bunkerCount = c_int(0)
bunkerSizes = np.zeros(128, dtype=c_int)

loadCourse(c_char_p(coursePath.encode("utf-8")), byref(greenSize), byref(bunkerCount), bunkerSizes)


scaleVal = c_double(0)
green = np.ndarray(dtype=c_double, shape=(greenSize.value, 3))
bunkerMaxSize = np.max(bunkerSizes)
bunkers = np.ndarray(shape=(bunkerCount.value, bunkerMaxSize, 3), dtype=c_double)
#bunkers = np.array(np.array([np.array(bunker) for bunker in bunkers_py], dtype=object), dtype=object)
teePos = np.zeros(3, dtype=c_double)

prepareCourse(coursePath.encode("utf-8"), greenSize.value, green, bunkerCount.value, bunkerMaxSize, bunkers, bunkerSizes, teePos, byref(scaleVal))