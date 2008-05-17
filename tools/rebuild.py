#!/usr/bin/env python
import os.path, sys
import textile

CURDIR = os.path.curdir
SRCDIR = os.path.realpath(os.path.join(CURDIR, "txtsrc"))
OUTDIR = os.path.realpath(os.path.join(CURDIR, "www"))

def dir_exists(path): return os.path.exists(path) and os.path.isdir(path)

def verify_dir_exists(path):
    if not dir_exists(path):
        print("dir '%s' doesn't exists" % path)
        sys.exit(1)

def ensure_dir(path):
    if not dir_exists(path):
        os.mkdir(path)

VALID_SUFFS = (".txt", ".textile")
def issrcfile(path):
    for suff in VALID_SUFFS:
        if path.endswith(suff):
            return True
    return False

def outfilename(path):
    base = os.path.basename(path)
    print base
    for suff in VALID_SUFFS:
        if base.endswith(suff):
            base = base.replace(suff, ".html")
            return os.path.join(OUTDIR, base)
    return None

def read(path):
    fo = open(path, "rb")
    d = fo.read()
    fo.close()
    return d

def write(path, data):
    fo = open(path, "wb")
    fo.write(data)
    fo.close()

def dofile(srcpath):
    if not issrcfile(srcpath):
        print("Skipping '%s'" % srcpath)
        return
    dstpath = outfilename(srcpath)
    if dstpath is None:
        print("Ignoring file '%s'" % srcpath)
        return
    txt = read(srcpath)
    html = textile.textile(txt)
    write(dstpath, html)

def main():
    ensure_dir(OUTDIR)
    files = [os.path.join(SRCDIR, f) for f in os.listdir(SRCDIR)]
    map(dofile, [f for f in files if os.path.isfile(f)])

if __name__ == "__main__":
    main()
