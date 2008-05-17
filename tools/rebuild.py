#!/usr/bin/env python
import os.path, sys, string
import textile

CURDIR = os.path.curdir
SRCDIR = os.path.realpath(os.path.join(CURDIR, "txtsrc"))
OUTDIR = os.path.realpath(os.path.join(CURDIR, "www"))

def read(path):
    fo = open(path, "rb")
    d = fo.read()
    fo.close()
    return d

def write(path, data):
    fo = open(path, "wb")
    fo.write(data)
    fo.close()

SCRIPTDIR = os.path.realpath(os.path.basename(sys.argv[0]))

g_header = None
def header():
    global g_header
    if g_header is None:
        g_header = read(os.path.join(SRCDIR, "_header.html"))
    return g_header

g_footer = None
def footer():
    global g_footer
    if g_footer is None:
        g_footer = read(os.path.join(SRCDIR, "_footer.html"))
    return g_footer

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

def is_comment(line): return line.startswith("#")
def is_sep(line): return 0 == len(line.strip())
def is_key(line): return ':' in line

# Load a source *.textile file
# Returns (<txt>, <dict>) tuple, where <dict> is a dictionary of
# name/value pairs in the file and the <txt> is the rest of the file
def parsesrc(srcpath):
    (ST_START, ST_BODY) = range(2)
    keys = {}
    lines = []
    state = ST_START
    for l in open(srcpath, "rb"):
        if ST_START == state:
            if is_comment(l): continue
            if is_sep(l):
                state = ST_BODY
                continue
            if not is_key(l):
                print("Expected name: value line, got:\n'%s'" % l)
                raise Exception("Malformed file %s" % srcpath)
        if ST_BODY == state:
            lines.append(l)
    txt = string.join(lines, "")
    return (txt, keys)

def dofile(srcpath):
    if not issrcfile(srcpath):
        print("Skipping '%s'" % srcpath)
        return
    dstpath = outfilename(srcpath)
    if dstpath is None:
        print("Ignoring file '%s'" % srcpath)
        return
    (txt, keys) = parsesrc(srcpath)
    title = ""
    if "Title" in keys:
        title = keys["Title"]
    hdr = header()
    hdr = hdr.replace("$title", title)
    ftr = footer()
    html = textile.textile(txt)
    write(dstpath, hdr + html + ftr)

def main():
    verify_dir_exists(SRCDIR)
    ensure_dir(OUTDIR)
    files = [os.path.join(SRCDIR, f) for f in os.listdir(SRCDIR)]
    map(dofile, [f for f in files if os.path.isfile(f)])

if __name__ == "__main__":
    main()
