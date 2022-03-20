import numpy as np
from ctypes import *
from C_wrappers import *


coursePath = "Hole 1.txt"
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