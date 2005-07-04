#!/usr/bin/python

"""
This script works only for 'exco'
It creates the ZMW release graphs.

Old packages are needed :
   gtklib1.2-dev
   libgdk-pixbuf-dev
"""

import glob
import os
import time

curdir = os.curdir
tgz = glob.glob("/home/exco/public_html/ZMW/zmw-*.tgz")
# tgz = glob.glob("/home/exco/public_html/ZMW/zmw-*1[012].tgz")

def split_filename(f):
    name = f.split("/")[-1]
    dot = name.split(".")
    return (int(dot[0].split("-")[1]), int(dot[1]), int(dot[2]), f)


t = [split_filename(f) for f in tgz]
t.sort()
tgz = [f[3] for f in t]



def get_name(fullname):
    return fullname[fullname.rfind("/")+1:-4]


def line_of_code(line):
    line = line.strip()
    return line != "" and line != "{" and line != "}"

def count(filename):
    f = open(filename)
    c = f.readlines()
    f.close()
    c = filter( lambda line: line_of_code(line), c)    
    if len(c)>1 and (c[0]+c[1]).find("ZMW: A Zero Memory Widget Library"):
        remove = 14
    else:
        remove = 0
    return len(c) - remove

def count_sum(filenames):
    sum = 0
    for f in filenames:
        sum += count(f)
    return sum

def file_size(filename):
    try:
        return os.path.getsize(filename)/1024
    except:
        return 0

def remove_debug_options():
    for makefile in ("Makefile.config", "Makefile"):
        try:
            f = open(makefile)
            break
        except IOError:
            pass
    lines = f.readlines()
    f.close()
    have_xxx_goal = 0
    for line in range(len(lines)):
        for o in ("-O6", "-O1", "-O2", "-O3", "-Os", "-O"):
            lines[line] = lines[line].replace( o, "")
        if lines[line].find("ZMW_DEBUG") >= 0:
            lines[line] = lines[line].replace("=1", "=0")
        if lines[line].find("xxx:") >= 0:
            have_xxx_goal = 1
    if not have_xxx_goal:
        f = open(makefile, "w")
        for line in lines:
            f.write(line)
        f.write("""

xxx:xxx.o
	$(ZMW_LINK) xxx.o
""")

    f.close()

    try:
        f = open("include/zmw/zmw.h", "r")
        c = f.read()
        f.close()
        c = c.replace("ZMW_DEBUG_OPTIONS_DEFAULT 1",
                      "ZMW_DEBUG_OPTIONS_DEFAULT 0")
        f =  open("include/zmw/zmw.h", "w")
        f.write(c)
        f.close()
    except IOError:
        pass



def diff_size(old, new):
    if old == None:
        return 0
    else:
        f = os.popen("diff -w -B -r ../%s ../%s" % (old, new), "r")
        n = len(f.readlines())
        f.close()
        return n

def compile():
    os.system("rm -f xxx xxx.o ; export CC='gcc -Iinclude' ; make xxx.o xxx")

def get_number(lines, key):
    for x in lines:
        if x.find(key) >= 0:
            return float(x[ x.find("=")+1 : -1 ])
    return 0

def runtime_test():
    
    f = open("xxx.c", "w")
    f.write("""
#include "zmw/zmw.h"
#include "zmw/zmw_private.h"

int main(int argc, char *argv[])
{
    printf("%d %d %d\\n"
#ifdef ZMW_USE_BIT_RECORD
          , sizeof(Zmw_Child)
#else
          , sizeof(Zmw_Size)
#endif
          , sizeof(Zmw_Stackable_Uninheritable)
          , sizeof(Zmw_Stackable_Inheritable)
          );
  return 0 ;
}
""")
    f.close()
    compile()
    f = os.popen("./xxx", "r")
    numbers = list(f.read().split(" "))
    f.close()
    return [int(t) for t in numbers]

def runtime_speed_test(zmw_xvfbn, text="",pango_cache=0):
    if not os.path.exists("Makefile.config"):
        return [0,0,0,0,0,0,0]


    os.system("rm -f xxx.drawdone ; fgrep -v <~/HOME/WIDGET/applications/benchmarks/speed.c -e zmw_cache_size -e zmw_child_cache_size >xxx.c")
    compile()
    print "Launch"
    if text == "":
        depth = 18
    else:
        depth = 18
    
    f = os.popen("./xxx %d 8 0 %s --debug=0 --pango-cache=%d --display=%s" \
                 % (depth, text, pango_cache, zmw_xvfbn), "r")
    while 1:
        time.sleep(1)
        if os.path.exists("xxx.drawdone"):
            break
    print "Drawing done"
    os.system(". ~/HOME/WIDGET/applications/examples/utilities ; export DISPLAY=%s ; zmw_move_cursor_to 10 10 ; zmw_key $KEY_A ; export ZMW_XVFB_PID=111111111" % zmw_xvfbn)
    
    numbers = f.readlines()
    print numbers
    f.close()
    print "Application terminated"

    n = []
    for t in (("draw.#time",1), ("VmData",1), ("VmStk",1), ("VmExe",1), ("dispatch_event.#time",1), ("search.#time",1),("accelerator.#time",1)):
        n.append( get_number(numbers, t[0]) * t[1] )

    
    return n
   

stats = [('name'                    , 'Kernel (C)',
          'Widgets (C)'             , 'Regtest (C)',
          'Headers (C)'             , 'Number of regtests',
          'Documentation (Docbook)' , 'Library size (Kb)',
          'Patch size (diff)'       , 'Zmw_Child (Byte)',
          'Zmw_Uninheritable (Byte)', 'Zmw_Stackable_Inheritable (Byte)',
          'Display time (Second)'   , 'VmData (Kb)',
          'VmStk(Kb)'               , 'VmExe (Kb)',
          'Dispatch event (Second)' , 'Search widget (Second)',
          'Dispatch accelerator (Second)',
          'Display time with texts (Second)',
          'Display time with texts, pango cached size (Second)',
          )]
titles = [ [i, stats[0][i]] for i in range(len(stats[0])) ]


def create_stats():
    f = os.popen("cd ~/HOME/WIDGET/applications/examples ; . ./utilities ; start_xvfb 2048x2048 >/dev/null ; export DISPLAY=$ZMW_XVFB ; zmw_move_cursor_to 100 100 ; echo ${ZMW_XVFBN=9} ; export ZMW_XVFB_PID=111111111", "r")
    zmw_xvfbn = ":" + f.readlines()[-2][:-1]
    print "=========", zmw_xvfbn
    f.close()
    name = None
    for archive in tgz:
        old_name = name
        name = get_name(archive)
        
        os.chdir("/tmp")
        os.system("zcat %s | tar -xf -" % archive)
        os.chdir(name)
        os.system("""
        if [ ! -f include/zmw/zmw.h ]
        then
            mkdir -p include/zmw
            ln -s ../../kernel/zmw.h include/zmw
        fi
        if [ ! -f include/zmw/zmw_private.h ]
        then
            touch include/zmw/zmw_private.h
        fi
        """)
        remove_debug_options()
        os.system("make lib || make ; strip zmw.so")
        
        
        stats.append( [
            name,
            count_sum( glob.glob("kernel/*.c") ),
            count_sum( glob.glob("widgets/*.c") ),
            count_sum( glob.glob("applications/examples/*/*.c") ),
            count_sum( glob.glob("include/zmw/*.h") + glob.glob("*/*.h") ),
            len(glob.glob("applications/examples/*/.")),
            count_sum( glob.glob("doc/zmw.xml") ),
            file_size("zmw.so"),
            diff_size(old_name, name),
            ]
                      + runtime_test()
                      + runtime_speed_test(zmw_xvfbn) 
                      + [runtime_speed_test(zmw_xvfbn,"X")[0]]
                      + [runtime_speed_test(zmw_xvfbn,"X",10)[0]]
                      )
        # os.system("rm -r /tmp/%s" % name)

        os.chdir("/tmp")
        i = 0
        f = open("xxx", "w")
        for s in stats[1:]:
            f.write("%d" % i)
            i += 1
            for v in s[1:]:
                f.write(" %f" % v)
            f.write("\n")
        f.close()
    os.system("cd ~/HOME/WIDGET/applications/examples ; make stop_xvfb")

def create_plot(titles, output_file, y_label):
    print titles
    c = ""
    # Set labels
    c += "set xlabel 'ZMW version'\n" 
    c += "set ylabel '%s'\n" % y_label

    c += "set terminal png small size 800,600 xFFFFFF x000000 x000000 x000000 xFF0000 x00FF00 x0000FF x00A0FF xA000FF xFFA000 xFF00A0\n"
    c += "set output '%s'\n" % output_file
    c += "set logscale y\n"
    # Set xtics
    c += "set xtics ("
    i = 0
    for archive in tgz:
        c += " '%s' %d," % (get_name(archive)[4:], i)
        i += 1
    c = c[:-1] + ")\n"

    c += "plot"

    width = ""
    for s in titles:
        c += " '/tmp/xxx' using 1:%d title '%s' with linespoints %s," % (
            s[0]+1, s[1], width)
    c = c[:-1]

    c += "\nquit\n"

    print c

    os.system("""echo "%s" | gnuplot""" % c)


if not os.path.exists("/tmp/xxx"):
    create_stats()

create_plot(
    filter( lambda x: x[0]==7 or 8 < x[0] < 12 or 12< x[0] < 14 or x[0]==15, titles)
    , "size.png"
    , "Sizes"
    )

create_plot(
    filter( lambda x: x[0] == 12 or 16 <= x[0] <= 20 , titles)
    , "time.png"
    , "CPU Time"
    )

create_plot(
    filter( lambda x: x[0]>0 and x[0]<7 or x[0] == 8 , titles)
    , "sources.png"
    , "Number of lines"
    )





