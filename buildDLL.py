import os

def buildDLL():
    files = os.path.join(os.curdir, "dll/*.c")
    os.system("gcc -fPIC -shared -o c_dll.so %s" % files)