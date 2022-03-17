import numpy as np

caddyPos = np.array([1,2,0])
golferPos = np.array([4,5,0])
followDist = 5 / 111,139
target = golferPos

def control():
    
    
    
def shouldMove(golferPos, caddyPos):
    chordVec = golferPos - caddyPos
    distance = np.norm(chordVec)
    if distance < followDist:
        return false
    