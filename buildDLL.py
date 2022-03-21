import os

def buildDLL():
    header_files = os.path.join(os.curdir, "dll/*.h")
    source_files = os.path.join(os.curdir, "dll/*.c")
    os.system("gcc -fPIC -shared -o c_dll.so %s %s" % (header_files, source_files))

if __name__ == "__main__":
    buildDLL()