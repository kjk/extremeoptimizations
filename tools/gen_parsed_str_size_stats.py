#!/usr/bin/env python

# This script generates the info about sizes of various versions
# of ParsedStr class in a format suitable for including with
# optimization_story.textile
#
# TODO: also capture sizeof(ParsedStr) for each version (by runing each command
# and parsing its stdout)
#
# TODO: also include version of the compiler used

import sys, os.path, subprocess, string

SCRIPTDIR = os.path.realpath(sys.argv[0])
SCRIPTDIR = os.path.dirname(SCRIPTDIR)
SRCDIR = os.path.join(SCRIPTDIR, "..", "src")
TXTSRCDIR = os.path.join(SCRIPTDIR, "..", "txtsrc")
OBJDIR = os.path.join(SRCDIR, "obj-small")
OUTFILEPATH = os.path.join(TXTSRCDIR, "parsedstr-size-stats.html")

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
FILES_TO_REPORT = ["parsed_str_dummy_stripped", "no-op", "parsed_str_opt_no_offsets_stripped", "no offsets", "parsed_str_stl_stripped", "naive stl", "parsed_str_opt_alloc_stripped", "optimized allocation",   "parsed_str_opt_offsets_stripped", "offsets", "parsed_str_unopt_stripped", "naive non-stl", "parsed_str_opt_common_stripped", "common case", "parsed_str_opt_one_array_stripped", "one array"]

class FileInfo(object):
    def __init__(self, name, readablename, size):
        self.name = name
        self.readablename = readablename
        self.size = size
        self.size_vs_smallest = 0

    def dump(self):
        print("name:     %s" % self.name)
        print("readable: %s" % self.readablename)
        print("size:     %d vs smallest: %d" % (self.size, self.size_vs_smallest))

def write(path, data):
    fo = open(path, "wb")
    fo.write(data)
    fo.close()

def html_from_filesinfo(filesinfo):
    lines = []
    lines.append("<center>")
    lines.append("<table>")
    lines.append("<tr>")
    lines.append("<th>Version</th>")
    lines.append("<th>File size</th>")
    lines.append("<th>File size delta</th>")
    lines.append("</tr>")
    for fi in filesinfo:
        lines.append("<tr>")
        lines.append("<td>%s</td>" % fi.readablename)
        lines.append("<td>%d</td>" % fi.size)
        lines.append("<td>%d</td>" % fi.size_vs_smallest)
        lines.append("</tr>")
    lines.append("</table>")
    lines.append("</center>")
    lines.append("")
    return string.join(lines, "\n")

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
        filesinfo.append(fi)
    filesinfo.sort(lambda x,y: cmp(x.size, y.size))
    smallest_size = filesinfo[0].size
    for fi in filesinfo:
        fi.size_vs_smallest = fi.size - smallest_size
    #map(FileInfo.dump, filesinfo)
    html = html_from_filesinfo(filesinfo)
    #print html
    write(OUTFILEPATH, html)

if __name__ == "__main__":
    main()
