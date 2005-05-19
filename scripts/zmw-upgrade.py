#!/usr/bin/env python
# -*- python -*-

"""
This script update all the ".c", ".h" and ".xml" files
in and under the current directory,
in order to make it compatible with the current version of the ZMW library.

- It renames functions

This script is in Python because I am sure that in the future,
there will be some not trivial translation to do.

"""

import os
import sys
import shutil
import re

r = raw_input("Have you made a backup of this directory (yes or no)? ")
if r != "yes":
    print "So I do nothing, I am too dangerous"
    sys.exit()


#
# Use "print0" in order to have no problems with filenames with bad chars
#
p = os.popen("find . \( -name '*.[ch]' -o -name '*.xml' \) -print0")
filenames = p.read()
p.close()
filenames = filenames[:-1].split("\0") 
#
# Simple word renaming
#
rename = [
 ["zmw_run"                            , "zmw_main"                        ],
 ["zmw_exit"                           , "zmw_main_quit"                   ],
 ["zmw_box_horizontal"                 , "zmw_hbox"                        ],
 ["zmw_box_vertical"                   , "zmw_vbox"                        ],
 ["zmw_box_horizontal_activable"       , "zmw_hbox_activable"              ],
 ["zmw_box_vertical_activable"         , "zmw_vbox_activable"              ],
 ["zmw_toggle"                         , "zmw_check_button"                ],
 ["zmw_toggle_int"                     , "zmw_check_button_int"            ],
 ["zmw_toggle_char"                    , "zmw_check_button_char"           ],
 ["zmw_toggle_bits"                    , "zmw_check_button_bits"           ],
 ["zmw_toggle_bits_int"                , "zmw_check_button_bits_int"       ],
 ["zmw_toggle_bits_char"               , "zmw_check_button_bits_char"      ],
 ["zmw_toggle_with_label"              , "zmw_check_button_with_label"     ],
 ["zmw_toggle_int_with_label"          , "zmw_check_button_int_with_label" ],
 ["zmw_toggle_char_with_label"         , "zmw_check_button_char_with_label"],
 ["zmw_toggle_bits_with_label"         , "zmw_check_button_bits_with_label"],
 ["zmw_toggle_bits_int_with_label"  , "zmw_check_button_bits_int_with_label"],
 ["zmw_toggle_bits_char_with_label" , "zmw_check_button_bits_char_with_label"],
 ["zmw_radio"                          , "zmw_radio_button"                ],
 ["zmw_radio_with_label"               , "zmw_radio_button_with_label"     ],
 ["zmw_scrollbar_vertical"             , "zmw_vscrollbar"                  ],
 ["zmw_scrollbar_horizontal"           , "zmw_hscrollbar"                  ],
 ["zmw_scrollbar_vertical_with_delta"  , "zmw_vscrollbar_with_delta"       ],
 ["zmw_scrollbar_horizontal_with_delta", "zmw_hscrollbar_with_delta"       ],
 ["zmw_text"                           , "zmw_label"                       ],
 ["zmw_text_editable"                  , "zmw_entry"                       ],
 ["zmw_filechooser"                    , "zmw_file_selection"              ],
 ["zmw_box"                            , "zmw_fixed"                       ],
 ["zmw_popup"                          , "zmw_menu"                        ],
 ["zmw_popup_without_accelerators"     , "zmw_menu_without_accelerators"   ],
 ["zmw_text_editable_with_cursor"      , "zmw_entry_with_cursor"           ],
 ["zmw_popup_with_detached"            , "zmw_menu_with_detached"          ],
 ["zmw_text_editable_with_cursor_and_start",
  "zmw_entry_with_cursor_and_start"],
 ["Zmw_Size"                           , "Zmw_Child"                       ],
    ]
#
#
#
print "Check potential problems before upgrading the sources"
for filename in filenames:
    if os.path.exists(filename + ".bak"):
        print "\n" * 3
        print "I will work only if there is not .bak file in the file tree"
        print "\trm `find . -name '*.bak'`"
        print "\n" * 3
        sys.exit(1)

    
    f = open(filename, "r")
    content = f.read()
    f.close()

    for a in rename:
        old,new = a
        r = re.compile("\\b" + new + "\\b")
        if r.search(content) != None:
            # print "Found %s in %s" % (new, filename)
            a[0] = None

rename = filter(lambda a: a[0], rename)
#
#
#
if rename:
    print "Replacement that will be done in the files:"
    for old,new in rename:
        print "%35s ==> %35s" % (old,new)
else:
    print "No replacement to be done"
        
#
# Make the substitutions
#
for filename in filenames:
    f = open(filename, "r")
    content = f.read()
    f.close()

    new_content = content

    #
    # Simple renaming
    #
    for (old,new) in rename:        
        r = re.compile("\\b" + old + "\\b")
        new_content = r.sub(new, new_content)
    
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

        





