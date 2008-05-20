#!/usr/bin/env python

# This script generates the info about sizes of various versions
# of ParsedStr class in a format suitable for including with
# optimization_story.textile

import sys, os.path, subprocess, string, re

SCRIPTDIR = os.path.realpath(sys.argv[0])
SCRIPTDIR = os.path.dirname(SCRIPTDIR)
SRCDIR = os.path.join(SCRIPTDIR, "..", "src")
TXTSRCDIR = os.path.join(SCRIPTDIR, "..", "txtsrc")
OBJDIR = os.path.join(SRCDIR, "obj-small")

def is_mac(): return sys.platform == 'darwin'
def is_win(): return sys.platform in ("win32", "cygwin")
def is_linux(): return sys.platform.startswith("linux") # on my Ubuntu it's "linux2"
def is_cygwin(): return sys.platform == "cygwin"

SYSTEM = "linux"
if is_mac():
    SYSTEM = "mac"
elif not is_linux():
    print("Not mac or linux. System is '%s'. Aborting." % sys.platform)
    sys.exit(1)

OUTFILEPATH = os.path.join(TXTSRCDIR, "parsedstr-size-stats-%s.html" % SYSTEM)

def log(txt):
    print txt,

# like cmdrun() but throws an exception on failure
def run_cmd_throw(*args):
    cmd = " ".join(args)
    log("Running '%s'\n" % cmd)
    cmdproc = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    res = cmdproc.communicate()
    errcode = cmdproc.returncode
    if 0 != errcode:
        print "Failed with error code %d" % errcode
        print "Stdout:"
        print res[0]
        print "Stderr:"
        print res[1]
        raise Exception("'%s' failed with error code %d" % (cmd, errcode))
    return (res[0], res[1])

def cd(dir):
    log("cd to '%s'\n" % dir)
    os.chdir(dir)

# maps name of executable to user-readable name
FILES_TO_REPORT = ["parsed_str_dummy_stripped", "A no-op version", "parsed_str_opt_no_offsets_stripped", "Getting rid of offsets", "parsed_str_stl_stripped", "Naive STL version", "parsed_str_opt_alloc_stripped", "Optimizing allocations of strings",   "parsed_str_opt_offsets_stripped", "Offsets instead of pointers", "parsed_str_unopt_stripped", "Naive non-STL version", "parsed_str_opt_common_stripped", "Optimizing for common case", "parsed_str_opt_one_array_stripped", "One array instead of two", "parsed_str_opt_no_offsets_no_dup_stripped", "Avoiding copying the string"]

class FileInfo(object):
    def __init__(self, name, readablename, size):
        self.name = name
        self.readablename = readablename
        self.size = size
        self.size_vs_smallest = 0
        self.sizeof = 0

    def dump(self):
        print("name:     %s" % self.name)
        print("readable: %s" % self.readablename)
        print("size:     %d vs smallest: %d" % (self.size, self.size_vs_smallest))
        print("sizeof:   %d" % self.sizeof)

def write(path, data):
    fo = open(path, "wb")
    fo.write(data)
    fo.close()

def html_from_filesinfo(filesinfo):
    lines = []
    lines.append("<center>")
    lines.append("<table cellspacing=4>")
    lines.append("<tr>")
    lines.append("<th>Version</th>")
    lines.append("<th>File size</th>")
    lines.append("<th>File size delta</th>")
    lines.append("<th>sizeof(ParsedStr)</th>")
    lines.append("</tr>")
    for fi in filesinfo:
        lines.append("<tr>")
        lines.append("<td>%s</td>" % fi.readablename)
        lines.append("<td align=center>%d</td>" % fi.size)
        lines.append("<td align=center>%d</td>" % fi.size_vs_smallest)
        lines.append("<td align=center>%d</td>" % fi.sizeof)
        lines.append("</tr>")
    lines.append("<tr align=center bgcolor=#dedede><td colspan='4'><font size=-1>%s</font></td></tr>" % gcc_version())
    lines.append("</table>")
    lines.append("</center>")
    lines.append("")
    return string.join(lines, "\n")

def gcc_version():
    (stdout, stderr) = run_cmd_throw("gcc", "--version")
    lines = string.split(stdout, "\n")
    ver = lines[0].strip()
    return ver

def get_parsed_str_sizeof(exe):
    (stdout, stderr) = run_cmd_throw(exe)
    reg = "sizeof\(ParsedStr\)=(\d+)"
    regcomp = re.compile(reg, re.MULTILINE)
    m = regcomp.search(stdout)
    sizeoftxt = m.group(1)
    #print sizeoftxt
    return int(sizeoftxt)

def main():
    cd(SRCDIR)
    #run_cmd_throw("make", "rebuild")
    files = [os.path.join(OBJDIR, f) for f in os.listdir(OBJDIR)]
    filesinfo = []
    for filepath in files:
        filename = os.path.basename(filepath)
        if filename not in FILES_TO_REPORT:
            continue
        filesize = os.path.getsize(filepath)
        readablename = FILES_TO_REPORT[FILES_TO_REPORT.index(filename) + 1]
        fi = FileInfo(filename, readablename, filesize)
        fi.sizeof = get_parsed_str_sizeof(filepath)
        filesinfo.append(fi)
    filesinfo.sort(lambda x,y: cmp(x.size, y.size))
    smallest_size = filesinfo[0].size
    for fi in filesinfo:
        fi.size_vs_smallest = fi.size - smallest_size
    map(FileInfo.dump, filesinfo)
    html = html_from_filesinfo(filesinfo)
    #print html
    write(OUTFILEPATH, html)

if __name__ == "__main__":
    main()
