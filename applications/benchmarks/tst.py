#!/usr/bin/python

import math

def o1(n):
    if n <= 1:
        return 1
    return 2 * o1(n/2)

def o2(n):
    if n <= 1:
        return 1
    return 2 * o2(n/2) + 10

def o3(n):
    if n <= 1:
        return 1
    return 3 * o3(n/2)

def o4(n):
    if n <= 1:
        return 1
    return 2 * o4(n/2) + n




for o in [o1, o2, o3, o4]:
    print o.__name__, "     n       o(n)     o(n)/n  o(n)/(n*log(n))"
    try:
        for i in map(lambda x: 2**x, range(1,30)):
            print "%9d %10d %10f %10f" % (i, o(i), float(o(i))/i
                                          , float(o(i))/(i*math.log(i))
                                          )
    except:
        pass

