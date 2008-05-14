#!/usr/bin/env python
import os.path

CURDIR = os.path.curdir
SRCDIR = os.path.realpath(os.path.join(CURDIR, "txtsrc"))
OUTDIR = os.path.realpath(os.path.join(CURDIR, "www"))

def dir_exists(path): return os.path.exists(path) and os.path.isdir(path)

def verify_dir_exists(path):
    if not dir_exists(path):
        print("dir '%s' doesn't exists" % path)
        sys.exit(1)

def issrcfile(path):
    return path.endswith(".txt")

def outfilename(path):
    base = os.path.basename(path)
    base = base.replace(".txt", ".html")
    return os.path.join(OUTDIR, base)

def dofile(srcpath):
    if not issrcfile(srcpath):
        print("Skipping '%s'" % srcpath)
        return
    dstpath = outfilename(srcpath)
    print(dstpath)

def main():
    files = [os.path.join(SRCDIR, f) for f in os.listdir(SRCDIR)]
    files = [f for f in files if os.path.isfile(f)]
    for f in files:
        dofile(f)

if __name__ == "__main__":
    main()
