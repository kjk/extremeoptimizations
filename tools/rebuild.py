#!/usr/bin/env python
import os.path, sys, string, shutil
import textile

SCRIPT_DIR = os.path.dirname(__file__)
BASE_DIR = os.path.realpath(os.path.join(SCRIPT_DIR, ".."))
TXTSRCDIR = os.path.join(BASE_DIR, "txtsrc")
SRCDIR = os.path.join(BASE_DIR, "src")
OUTDIR = os.path.join(BASE_DIR, "www")
OUTSRCDIR = os.path.join(BASE_DIR, "www", "src")

def read(path):
    fo = open(path, "rb")
    d = fo.read()
    fo.close()
    return d

def write(path, data):
    fo = open(path, "wb")
    fo.write(data)
    fo.close()

g_header = None
def header():
    global g_header
    if g_header is None:
        g_header = read(os.path.join(TXTSRCDIR, "_header.html"))
    return g_header

g_footer = None
def footer():
    global g_footer
    if g_footer is None:
        g_footer = read(os.path.join(TXTSRCDIR, "_footer.html"))
    return g_footer

g_header_src = None
def headersrc():
    global g_header_src
    if g_header_src is None:
        g_header_src = read(os.path.join(TXTSRCDIR, "_header_src.html"))
    return g_header_src

g_footer_src = None
def footersrc():
    global g_footer_src
    if g_footer_src is None:
        g_footer_src = read(os.path.join(TXTSRCDIR, "_footer_src.html"))
    return g_footer_src

def dir_exists(path): return os.path.exists(path) and os.path.isdir(path)
def file_exists(path): return os.path.exists(path) and os.path.isfile(path)
def copy_file(src,dst): shutil.copy(src, dst)
def verify_dir_exists(path):
    if not dir_exists(path):
        txt = "dir '%s' doesn't exists" % path
        print(txt)
        raise Exception(txt)

def verify_file_exists(path):
    if not file_exists(path):
        txt = "file '%s' doesn't exists" % path
        print(txt)
        raise Exception(txt)

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
    for suff in VALID_SUFFS:
        if base.endswith(suff):
            base = base.replace(suff, ".html")
            return os.path.join(OUTDIR, base)
    return None

def tmpfilename(path): return path + ".tmp"

def is_comment(line): return line.startswith("#")
def is_sep(line): return 0 == len(line.strip())
def is_key(line): return ':' in line
def is_includesrc(line): return line.startswith("@includesrc ")
def is_includetxt(line): return line.startswith("@includetxt ")

def readlines(filename, start, end):
    lines = []
    line_no = 1
    for l in open(filename, "rb"):
        if line_no > end:
            return string.join(lines, "")
        if line_no >= start:
            lines.append(l)
        line_no += 1
    return string.join(lines, "")

g_token = "asdflkasdfasdf:\n"
# A lame way to generate unique string that we can then string.replace()
# with destination text
def get_token():
    global g_token
    token = g_token
    g_token = g_token[:-2] + "a:\n"
    #print("Token: '%s'\n" % token)
    return token

# Returns a tuple (filename, filecontent)
def do_includesrc(line):
    parts = line.split(" ")
    if 4 != len(parts):
        txt = "Malformated @includesrc line:\n'%s'" % line
        print(txt)
        raise Exception(txt)
    filename = parts[1]
    startline = int(parts[2])
    endline = int(parts[3])
    filepath = os.path.join(SRCDIR, filename)
    verify_file_exists(filepath)
    txt = readlines(filepath, startline, endline)
    return (filepath, txt)

def do_includetxt(line, lines):
    parts = line.split(" ")
    if 2 != len(parts):
        txt = "Malformated @includetxt line:\n'%s'" % line
        print(txt)
        raise Exception(txt)
    filename = parts[1].strip()
    filepath = os.path.join(TXTSRCDIR, filename)
    txt = read(filepath)
    lines.append(txt)

g_do_tokens = True

# Generate sth. like this:
# "ParsedStrTest.cpp":src/ParsedStrTest.cpp.html ("raw":src/ParsedStrTest.cpp.txt):
def src_textile_link(basename):
    return '"%s":src/%s.html ("raw":src/%s.txt):' % (basename, basename, basename)

def src_html_link(basename):
    return "<a href='src/%s.html'>%s</a> (<a href='src/%s.txt'>raw</a>):" % (basename, basename, basename)

# Load a source *.textile file
# Returns (<txt>, <dict>) tuple, where <dict> is a dictionary of
# name/value pairs in the file and the <txt> is the rest of the file
def parse_textile(srcpath):
    (ST_START, ST_BODY) = range(2)
    keys = {}
    lines = []
    state = ST_START
    tokens = {}
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
            if is_includesrc(l):
                (filepath, txt) = do_includesrc(l)
                token = get_token()
                tokens[token] = (filepath, txt)
                if g_do_tokens:
                    lines.append(token)
                else:
                    txt = "<pre><code>\n" + txt + "</code></pre>\n"
                    lines.append(txt)
            elif is_includetxt(l):
                do_includetxt(l, lines)
            else:
                lines.append(l)
    txt = string.join(lines, "")
    return (txt, tokens, keys)

def htmlify(text):
    text = text.replace("&","&amp;")
    text = text.replace("<","&lt;")
    text = text.replace(">","&gt;")
    return text

# Return a <code class=$x> where $x is a class name understood by highlight.js
# We auto-detect $x from file name.
def code_for_filename(filename):
    ext_to_classname = { 
        ".cpp" : "cpp",
        ".h" : "cpp",
        ".c" : "cpp"}
    for ext in ext_to_classname.keys():
        if filename.endswith(ext):
            return '<code class="' + ext_to_classname[ext] + '">'
    return "<code>"

def dofile(srcpath):
    if not issrcfile(srcpath):
        print("Skipping '%s'" % srcpath)
        return
    dstpath = outfilename(srcpath)
    if dstpath is None:
        print("Ignoring file '%s'" % srcpath)
        return
    tmppath = tmpfilename(srcpath)
    (txt, tokens, keys) = parse_textile(srcpath)
    title = ""
    if "Title" in keys:
        title = keys["Title"]
    hdr = header()
    hdr = hdr.replace("$title", title)
    ftr = footer()        
    #write(tmppath, txt)
    html = textile.textile(txt)
    if g_do_tokens:
        #print tokens.keys()
        for token in tokens.keys():
            codetxt = tokens[token][1]
            filename = os.path.basename(tokens[token][0])
            c = code_for_filename(tokens[token][0])
            srclinktxt = src_html_link(filename)
            codehtml = srclinktxt + "\n<pre>" + c + "\n" + htmlify(codetxt) + "</code></pre>\n"
            token = token.strip()
            html = html.replace(token, codehtml)
    write(dstpath, hdr + html + ftr)

def issourcecodefile(path):
    for ext in [".cpp", ".c", ".h", "makefile"]:
        if path.endswith(ext): return True
    return False

def dosrcfile(srcpath):
    if not issourcecodefile(srcpath):
        print("Skipping '%s'" % srcpath)
        return
    base = os.path.basename(srcpath)
    txtpath = os.path.join(OUTSRCDIR, base) + ".txt"
    htmlpath = os.path.join(OUTSRCDIR, base) + ".html"
    copy_file(srcpath, txtpath)
    hdr = headersrc()
    hdr = hdr.replace("$title", base)
    html = hdr + "<pre>" + code_for_filename(srcpath) + "\n" + htmlify(read(srcpath)) + "\n</code></pre>\n"
    html = html + footersrc()
    write(htmlpath, html)

def main():
    verify_dir_exists(TXTSRCDIR)
    verify_dir_exists(SRCDIR)
    ensure_dir(OUTDIR)
    ensure_dir(OUTSRCDIR)
    files = [os.path.join(TXTSRCDIR, f) for f in os.listdir(TXTSRCDIR)]
    map(dofile, [f for f in files if os.path.isfile(f)])
    files = [os.path.join(SRCDIR, f) for f in os.listdir(SRCDIR)]
    map(dosrcfile, [f for f in files if os.path.isfile(f)])

if __name__ == "__main__":
    main()
