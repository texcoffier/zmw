#!/usr/bin/env python

"""Only for exco

Check if the new released version seems OK.

"""

import os
import sys
import stat

tmp = "XXX-EXCO-XXX"


def file_content(filename):
    f = open(filename, "r")
    c = f.read()
    f.close()
    return c
    

version = os.popen("make displayversion", "r").read()[:-1]

print "VERSION:", version

name = "zmw-%s" % version

tgz = "/home/exco/public_html/ZMW/" + name + ".tgz"

if not os.path.exists(tgz):
    print "TGZ is not done"
    sys.exit(1)

if os.stat(tgz)[stat.ST_MODE] & stat.S_IROTH == 0:
    print "TGZ is not readable by everybody!"

if file_content("/home/exco/public_html/ZMW/Welcome.html").find('"%s.tgz"' % name) == -1:
    print "TGZ is not an URL inside Welcome.html page"

if file_content("/home/exco/public_html/ZMW/zmw/zmw.html").find('(ZMW %s)' %version) == -1:
    print "The online documentation does not contains the last release number"


os.chdir(os.getenv("HOME"))
os.mkdir(tmp)
os.chdir(tmp)

files = os.popen("zcat <%s | tar -xvf -" % tgz, "r").readlines()

files = [ file[:-1] for file in files]

def normal_file(name):
    for extension in [".c", ".h", ".xml", ".py", ".png", "/",
                      ".fig", ".config",
                      "/Makefile",
                      "/COPYING",
                      "/ChangeLog",
                      "/README",
                      "/TODO",
                      "/applications/book/TODO",
                      "/applications/book/fichiers.lib",
                      "/applications/book/locale/fr/LC_MESSAGES/zmwbook.po",
                      "/applications/book/locale/zmwbook.pot",
                      "/applications/book/book-preferences.dtd",
                      "/applications/book/book-preferences.xsl",
                      "/applications/book/exco.lib",
                      "/applications/benchmarks/README-bit",
                      "/applications/benchmarks/.fvwm2rc",
                      "/applications/examples/regression_test",
                      "/applications/examples/README",
                      "/applications/examples/.fvwm2rc",
                      "/applications/examples/filechooser/tst/a",
                      "/applications/examples/filechooser/tst/b",
                      "/applications/examples/filechooser/tst/aa",
                      "/applications/examples/utilities",
                      "/applications/circuit/CIRCUITS/no_name.circuit",
                      "/applications/circuit/CIRCUITS/bug1.circuit",
                      "/dictionary",
                      "/doc/zmw.css",
                      "/doc/zmw.dsssl",
                      "/doc/examples",
                      "/nightbuild.html",
                      "/scripts/zmw-diff",
                      "/scripts/zmw-tar",
                      ]:
        if name.endswith(extension):
            return True
    return False

files = [ file for file in files if not normal_file(file) ]

if len(files) != 0:
    print "Files that may not be put in the archive:"
    for file in files:
        print "\t" + file

os.chdir(name)

version2 = os.popen("make displayversion", "r").read()[:-1]
if version != version2:
    print "VERSION inside TGZ is not the same", version2


options = os.popen("make displayoptions", "r").read()[:-1]
if options != "PROFILING=-O6":
    print "Compile option inside TGZ Makefile.config are not as expected", options

if file_content("doc/zmw.xml").find('(&ZMW; %s)' %version) == -1:
    print "The XML documentation does not contains the last release number"


c = file_content("include/zmw/zmw.h")
for t in (
    "ZMW_DEBUG_OPTIONS_DEFAULT 0",
    "ZMW_DEBUG_NAME                 ZMW_DEBUG_OPTIONS_DEFAULT",
    "ZMW_DEBUG_ASSERT               ZMW_DEBUG_OPTIONS_DEFAULT",
    "ZMW_DEBUG_INSIDE_ZMW_PARAMETER ZMW_DEBUG_OPTIONS_DEFAULT",
    "ZMW_DEBUG_STORE_WIDGET_TYPE    ZMW_DEBUG_OPTIONS_DEFAULT",
    "ZMW_PROFILING                  ZMW_DEBUG_OPTIONS_DEFAULT",
    "ZMW_USE_BIT_RECORD 1",
    ):
    if c.find(t) == -1:
        print "zmw.h does not contains", t








os.chdir("../..")
os.system("rm -r " + tmp)



