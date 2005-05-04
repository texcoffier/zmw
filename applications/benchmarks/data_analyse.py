#!/usr/bin/python

# + titre complet
# Rename label
# Make other plots

import os
import math
import sys
import glob

try:
    filename = sys.argv[1]
except:
    filename = glob.glob("speed.2*")
    filename.sort()
    filename = filename[-1]


f = open(filename, "r")
file = f.read()[:-2]
f.close()

data = []
values = {}
for bloc in file.split("\n\n"):
    d = {}
    for line in bloc.split("\n"):        
        (name, value) = line.split("=")
        d[name] = value
        try:
            values[name][value] = None
        except:
            values[name] = {value: None}            
    data.append(d)

# Data is a list of dictionnaries

def constraints_verified(bloc, constraints):
    for c in constraints:
        if bloc[c[0]] != c[1]:
            return 0
    return 1
    

def generate(data, constraints, output):
    t = []
    for bloc in data:
        if constraints_verified(bloc, constraints):
            t.append( [ bloc[o] for o in output] )
    return t

def plot(file, data, constraints, output):
    lines = generate(data, constraints, output)    
    for line in lines:
        for value in line:
            file.write(value + " ")
        file.write("\n")
    return len(lines)

#
#
#

def ploter(varx, vary, varz, x_ok = lambda x: 1, z_ok = lambda x: 1
           , logscale = "z", image_name = None, time = "zmw_action_draw.#time"
           , time_label = "CPU time"
           ):
    n = 0
    c = ""
    zz = map(lambda xx: int(xx), values[varz].keys())
    zz.sort()
    for z in zz:
        if not z_ok(z):
            continue
        nb_sample = 0
        z = str(z)
        
        filename = "xxx.%s.%s%s" % (varx, varz, z)
        n += 1
        f = open(filename, "w")
        for x in values[varx].keys():
            if x_ok(x):
                if plot(f, data
                        , ((varx, x),(varz, z))
                        , (vary, time)
                        ):
                    f.write("\n")
                    nb_sample += 1
    
        f.close()
        if nb_sample:
            c += "'%s' title '%s %s' with lines," % (filename, z, varz)
    c = c[:-1]

    plot_command = """
set logscale %s
set xlabel '%s'
set ylabel '%s'
plot %s
""" % (logscale, vary, time_label, c)

    if image_name != None:
        terminal = """
set terminal png
set size 1,1
set output '%s'
""" % image_name
        os.system("""echo "%s%s\nquit" | gnuplot""" % (terminal, plot_command))
    else:
        os.system("""( echo "%s" ; sleep 1000) | gnuplot &""" % plot_command)
    



print "For 2 children (n is the number of leaves)"
print "        n       time     time/n time/(n*log(n))"
for line in generate(data, (("cache size","0"), ("#children","2"))
                     ,("#leaf_widget", "zmw_action_draw.#time")):
    nb_leaf = int(line[0])
    time = float(line[1])
    if nb_leaf < 2048:
        continue
    print "%9d %10.2f %10g %g" % (nb_leaf, time, 100000*time/nb_leaf
                                  , 100000*time/nb_leaf/math.log(nb_leaf))




ploter("#leaf_widget", "cache size", "#children"
       , x_ok = lambda x: int(x) == 262144
       , logscale="x"
       , image_name = "cache_size.png"
       , time_label = "CPU time to display"
       )

ploter("cache size", "#leaf_widget", "#children"
       , x_ok = lambda x: int(x) == 0
       , logscale="xy"
       , image_name = "leave.png"
       , time_label = "CPU time to display"
       )



ploter("#leaf_widget", "cache size", "#children"
       , x_ok = lambda x: int(x) == 262144
       , logscale="x"
       , image_name = "cache_size_event.png"
       , time = "zmw_action_dispatch_event.#time"
       , time_label = "CPU time to dispatch event"
       )

ploter("cache size", "#leaf_widget", "#children"
       , x_ok = lambda x: int(x) == 0
       , logscale="xy"
       , image_name = "leave_event.png"
       , time = "zmw_action_dispatch_event.#time"
       , time_label = "CPU time to dispatch event"
       )


ploter("#leaf_widget", "cache size", "#children"
       , x_ok = lambda x: int(x) == 262144
       , logscale="x"
       , image_name = "cache_size_accel.png"
       , time = "zmw_action_dispatch_accelerator.#time"
       , time_label = "CPU time to dispatch accelerator"
       )

ploter("cache size", "#leaf_widget", "#children"
       , x_ok = lambda x: int(x) == 0
       , logscale="xy"
       , image_name = "leave_accel.png"
       , time = "zmw_action_dispatch_accelerator.#time"
       , time_label = "CPU time to dispatch accelerator"
       )


ploter("cache size", "#leaf_widget", "#children"
       , x_ok = lambda x: int(x) == 0
       , logscale="x"
       , time = "VmData"
       , time_label = "Heap usage in Kb"
       , image_name = "memory_data.png"
       )

ploter("cache size", "#leaf_widget", "#children"
       , x_ok = lambda x: int(x) == 0
       , logscale="x"
       , time = "VmStk"
       , time_label = "Stack usage in Kb"
       , image_name = "memory_stack.png"
       )


if 0:
    ploter("#leaf_widget", "cache size", "#children", logscale="x")
    ploter("cache size", "#leaf_widget", "#children")
    ploter("cache size", "#children", "#leaf_widget", z_ok=lambda z: z >= 1024)





    
