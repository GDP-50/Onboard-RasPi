import numpy as np
def loadCourse(filename):
    bunkers_py = []
    green_py = []
    bunkerCount = -1
    with open(filename, "r") as f:
        lines = f.readlines()
        for i, line in enumerate(lines):
            line = line.strip()
            line = line.split(" ")
            if line[0] == "Bunker":
                bunkerActive = True
                greenActive = False
                teeActive = False
                points = []
                bunkers_py.append(points)
                bunkerCount += 1
            elif line[0] == "Green":
                bunkerActive = False
                greenActive = True
                teeActive = False
                points = []
            elif line[0] == "Tee":
                bunkerActive = False
                greenActive = False
                teeActive = True
            elif line[0] == "p":
                points.append([float(line[1]), float(line[2])])
                if bunkerActive:
                    bunkers_py[bunkerCount] = points
                elif greenActive:
                    green_py.append([float(line[1]), float(line[2])])
                elif teeActive:
                    tee = [float(line[1]), float(line[2])]
            else:
                print("some unexpected line beginning")
                continue

    bunkers = np.array(np.array([np.array(bunker) for bunker in bunkers_py], dtype=object), dtype=object)
    green = np.array(green_py)
    tee = np.array(tee)

    deg2meters = 2 * np.pi * 6371000 / 360

    for i, greenPoint in enumerate(green):
        rel = tee - greenPoint
        green[i] = rel * deg2meters

    for i, bunker in enumerate(bunkers):
        for j, bunkerPoint in enumerate(bunker):
            rel = tee - bunkerPoint
            bunkers[i][j] = rel * deg2meters

    largestVal = 0
    for bunker in bunkers:
        max = np.max(bunker)
        if max > largestVal:
            largestVal = max

    max = np.max(green)
    if max > largestVal:
        largestVal = max
    max = np.max(tee)
    if max > largestVal:
        largestVal = max


    for i, bunker in enumerate(bunkers):
        bunkers[i] /= 0.5 * largestVal

    green /= 0.5 * largestVal
    return green, bunkers



green, bunkers = loadCourse("Hole 1.txt")