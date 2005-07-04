#!/usr/bin/env python
# -*- python -*-

print """
DO NOT USE THIS
"""

exit(1)


import os
import sys
import shutil

p = os.popen("find . \( -name '*.[ch]' -o -name '*.xml' \) -print0")
filenames = p.read()
p.close()
filenames = filenames[:-1].split("\0")

rename = []

for i in ("horizontal_expand", "vertical_expand",
          "horizontal_alignment", "vertical_alignment",
          "current_state",
          "padding_width",
          ):
    i += '_'
    for j in ("", "child__", "parent__"):
        
        rename.append(["zmw_"+j           +i, "zmw_"+j + "cs_" + i]) 
        rename.append(["zmw_"+j+ "widget_"+i, "zmw_"+j         + i]) 


for filename in filenames:
    f = open(filename, "r")
    content = f.read()
    f.close()

    new_content = content

    #
    # Simple renaming
    #
    for (old,new) in rename:
        new_content = new_content.replace(old, new)
    
    if content != new_content:
        backup = filename + ".bak"
        if os.path.exists(backup):
            print backup, " exists, I do not want to overwrite it"
            print "So I abort the procedure"
            sys.exit()
        print filename, "is backuped and modified"
        shutil.copy2(filename, backup)
        f = open(filename, "w")
        f.write(new_content)
        f.close()

