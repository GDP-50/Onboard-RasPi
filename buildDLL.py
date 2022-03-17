import os

def buildDLL():
    header_files = os.path.join(os.curdir, "dll/*.h")
    source_files = os.path.join(os.curdir, "dll/*.c")
    os.system("gcc -v -fPIC -shared -o c_dll.so %s %s" % header_files, source_files)