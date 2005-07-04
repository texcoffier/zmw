#!/usr/bin/env python
# -*- python -*-

"""
This script update all the ".c", ".h" and ".xml" files
in and under the current directory,
in order to make it compatible with the current version of the ZMW library.

YOU MUST NOT APPLY THIS SCRIPT ON THE DISTRIBUTED LIBRARY

This script is in Python because I am sure that in the future,
there will be some not trivial translation to do.

"""

import os
import sys
import shutil
import sre

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

filenames = [filename for filename in filenames if not filename.startswith("./kernel/")]
filenames = [filename for filename in filenames if not filename.startswith("./utilities/")]
filenames = [filename for filename in filenames if not filename.startswith("./widgets/")]

filenames.remove("./include/zmw/zmw.h")
filenames.remove("./include/zmw/zmw_private.h")
filenames.remove("./include/zmw/zmw_generated_private.h")
filenames.remove("./include/zmw/zmw_generated_public.h")

# To test only one filename
# filenames = ["toto"]

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
 ["zmw_size_string"                    , "zmw_child_string"                ],
 ["zmw_cache_init"                     , "zmw_child_cache_init"            ],
 ["zmw_cache_size"                     , "zmw_child_cache_size"            ],
 ["zmw_cache_free"                     , "zmw_child_cache_free"            ],
 ["zmw_cache_get_size_real"            , "zmw_child_cache_get_size_real"   ],
 ["zmw_cache_get_size"                 , "zmw_child_cache_get_size"        ],
 ["zmw_cache_set_size_real"            , "zmw_child_cache_set_size_real"   ],
 ["zmw_cache_set_size"                 , "zmw_child_cache_set_size"        ],
 ["zmw_cache_check"                    , "zmw_child_cache_check"           ],
 ["ZMW_WIDGET_TOP"                     , "zmw_is_widget_top()"             ],
 ["ZMW_CHILD_REQUIRED_PADDED_WIDTH"    , "zmw_child__required_padded_width" ],
 ["ZMW_CHILD_REQUIRED_PADDED_HEIGHT"   , "zmw_child__required_padded_height"],
 ["zmw_alignement_horizontal_make"     , "zmw_alignment_horizontal_make"],
 ["zmw_alignement_vertical_make"       , "zmw_alignment_vertical_make"],
 ["zmw_sensible"                       , "zmw_sensitive" ],
]

for before, after in (
    ("required.x", "required_x"),
    ("required.y", "required_y"),
    ("required.width", "required_width"),
    ("required.height", "required_height"),
    ("required", "required"),
    ("allocated.x", "allocated_x"),
    ("allocated.y", "allocated_y"),
    ("allocated.width", "allocated_width"),
    ("allocated.height", "allocated_height"),
    ("allocated", "allocated"),
    ("min.x", "min_x"),
    ("min.y", "min_y"),
    ("min.width", "min_width"),
    ("min.height", "min_height"),
    ("min", "min"),
    ("used_to_compute_parent_size", "used_by_parent"),
    ("current_state.padding_width", "widget_padding_width"),
    ("current_state.horizontal_expand", "horizontal_expand"),
    ("current_state.vertical_expand", "vertical_expand"),
    ("current_state.horizontal_alignment", "horizontal_alignment"),
    ("current_state.vertical_alignment", "vertical_alignment"),
    ("invisible", "invisible"),
    ):
    rename.append([r"ZMW_CHILDREN\[([^]]*)\]." + before + r"[ \t]*=[ \t]*([^;!=]+)", r"zmw_child__" + after + r"_set(\1,\2)"])
    rename.append([r"ZMW_CHILDREN\[([^]]*)\]." + before                            , r"zmw_child__" + after + r"_get(\1)"])



for before, after in [
    ("ZMW_FILE"                              , "zmw_file"                      ),
    ("ZMW_LINE"                              , "zmw_line"                      ),
    ("ZMW_TYPE"                              , "zmw_type"                      ),
    ("ZMW_FOCUS->value"                      , "zmw_focus_value"               ),
    ("ZMW_FOCUS"                             , "zmw_focus"                     ),
    ("ZMW_ACTION"                            , "zmw_action"                    ),
    ("ZMW_AUTO_RESIZE"                       , "zmw_auto_resize"               ),
    ("ZMW_SUBACTION"                         , "zmw_subaction"                 ),
    ("ZMW_DEBUG"                             , "zmw_debug"                     ),
    ("ZMW_WINDOW"                            , "zmw_window"                    ),
    ("ZMW_EXTERNAL_STATE"                    , "zmw_external_state"            ),
    ("zmw.profiling_displayed"               , "zmw_zmw_profiling_displayed"   ),
    ("zMw[-1].u.parent_to_child.window"      , "zmw_parent__window"            ),
    ("zMw[-1].u.call_number"                 , "zmw_parent__call_number"       ),
    ("zMw[-1].u.children"                    , "zmw_parent__children"          ),
    ("zMw[-1].u.nb_of_children"              , "zmw_parent__nb_of_children"    ),
    ("zMw[-1].u.nb_of_children_max"          , "zmw_parent__nb_of_children_max"),
    ("zMw[-1].u.parent_to_child"             , "zmw_parent__parent_to_child"   ),
    ("zMw->u.size"                           , "zmw_size"                      ),
    ("ZMW_PARENT_SIZE.child_activated"       , "zmw_parent__children_activated"),
    ("ZMW_PARENT_SIZE.event_in_rectangle"    , "zmw_parent__event_in_rectangle"),
    ("ZMW_PARENT_SIZE.hash"                  , "zmw_parent__hash_key"          ),
    ("ZMW_PARENT_SIZE.sensible"              , "zmw_parent__sensitived"        ),
    ("ZMW_PARENT_SIZE.allocated.x"           , "zmw_parent__allocated_x"       ),
    ("ZMW_PARENT_SIZE.allocated.y"           , "zmw_parent__allocated_y"       ),
    ("ZMW_PARENT_SIZE.allocated.width"       , "zmw_parent__allocated_width"   ),
    ("ZMW_PARENT_SIZE.allocated.height"      , "zmw_parent__allocated_height"  ),
    ("ZMW_PARENT_SIZE.do_not_map_window"     , "zmw_parent__do_not_map_window" ),
    ("ZMW_SIZE_PADDING_WIDTH"                , "zmw_widget_padding_width"             ),
    ("ZMW_SIZE_ALLOCATED.x"                  , "zmw_allocated_x"               ),
    ("ZMW_SIZE_ALLOCATED.y"                  , "zmw_allocated_y"               ),
    ("ZMW_SIZE_ALLOCATED.width"              , "zmw_allocated_width"           ),
    ("ZMW_SIZE_ALLOCATED.height"             , "zmw_allocated_height"          ),
    ("ZMW_SIZE_ALLOCATED"                    , "zmw_allocated"                 ),
    ("ZMW_SIZE_MIN.x"                        , "zmw_min_x"                     ),
    ("ZMW_SIZE_MIN.y"                        , "zmw_min_y"                     ),
    ("ZMW_SIZE_MIN.width"                    , "zmw_min_width"                 ),
    ("ZMW_SIZE_MIN.height"                   , "zmw_min_height"                ),
    ("ZMW_SIZE_MIN"                          , "zmw_min"                       ),
    ("ZMW_SIZE_REQUIRED.x"                   , "zmw_required_x"                ),
    ("ZMW_SIZE_REQUIRED.y"                   , "zmw_required_y"                ),
    ("ZMW_SIZE_REQUIRED.width"               , "zmw_required_width"            ),
    ("ZMW_SIZE_REQUIRED.height"              , "zmw_required_height"           ),
    ("ZMW_SIZE_REQUIRED"                     , "zmw_required"                  ),
    ("ZMW_ASKED.x"                           , "zmw_asked_x"                   ),
    ("ZMW_ASKED.y"                           , "zmw_asked_y"                   ),
    ("ZMW_ASKED.width"                       , "zmw_asked_width"               ),
    ("ZMW_ASKED.height"                      , "zmw_asked_height"              ),
    ("ZMW_ASKED"                             , "zmw_asked"                     ),
    ("ZMW_SIZE_HASH"                         , "zmw_hash_key"                  ),
    ("ZMW_SIZE_EVENT_IN_RECTANGLE"           , "zmw_event_in_rectangle"        ),
    ("ZMW_SIZE_EVENT_IN_CHILDREN"            , "zmw_event_in_children"         ),
    ("ZMW_SIZE_INVISIBLE"                    , "zmw_invisible"                 ),
    ("ZMW_SIZE_SENSIBLE"                     , "zmw_sensitived"                ),
    ("ZMW_SIZE_DO_NOT_MAP_WINDOW"            , "zmw_do_not_map_window"         ),
    ("ZMW_SIZE_CHILD_ACTIVATED"              , "zmw_children_activated"        ),
    ("ZMW_SIZE_ACTIVATED"                    , "zmw_activated"                 ),
    ("ZMW_SIZE_CHANGED"                      , "zmw_changed"                   ),
    ("ZMW_SIZE_TIP_VISIBLE"                  , "zmw_tip_visible"               ),
    ("ZMW_SIZE_FOCUSED"                      , "zmw_focused"                   ),
    ("ZMW_SIZE_PASS_THROUGH"                 , "zmw_pass_through"              ),
    ("ZMW_SENSIBLE"                          , "zmw_sensitive"                 ),
    ("ZMW_FONT_FAMILY"                       , "zmw_font_family"               ),
    ("ZMW_FONT_SIZE"                         , "zmw_font_size"                 ),
    ("ZMW_FONT_WEIGHT"                       , "zmw_font_weight"               ),
    ("ZMW_FONT_STYLE"                        , "zmw_font_style"                ),
    ("ZMW_FONT"                              , "zmw_font"                      ),
    ("ZMW_NB_OF_CHILDREN"                    , "zmw_nb_of_children"            ),
    ("ZMW_USED_TO_COMPUTE_PARENT_SIZE"       , "zmw_used_by_parent"            ),
    ("ZMW_HORIZONTAL_EXPAND"                 , "zmw_widget_horizontal_expand"  ),
    ("ZMW_VERTICAL_EXPAND"                   , "zmw_widget_vertical_expand"    ),
    ("ZMW_SIZE_HORIZONTAL_EXPAND"            , "zmw_horizontaly_expanded"      ),
    ("ZMW_SIZE_VERTICAL_EXPAND"              , "zmw_verticaly_expanded"        ),
    ("ZMW_SIZE_HORIZONTAL_ALIGNMENT"         , "zmw_widget_horizontal_alignment"),
    ("ZMW_SIZE_VERTICAL_ALIGNMENT"           , "zmw_widget_vertical_alignment" ),
    ("ZMW_NAME"                              , "zmw_name"                      ),
    ("ZMW_NAME_INDEX"                        , "zmw_name_index"                ),
    ("ZMW_NAME_SEPARATOR"                    , "zmw_name_separator"            ),
    ("ZMW_CHILD_NUMBER"                      , "zmw_child_number"              ),
    ("ZMW_CHILDREN[i].event_in_children"     , "zmw_child__event_in_children_get(i)"  ),
    ("ZMW_CALL_NUMBER"                       , "zmw_call_number"               ),
    ("ZMW_MENU_STATE"                        , "zmw_menu_state"                ),
    ("ZMW_DO_NOT_EXECUTE_POP"                , "zmw_do_not_execute_pop"        ),
    ("ZMW_FOCUS_WIDTH"                       , "zmw_focus_width"               ),
    ("ZMW_BORDER_WIDTH"                      , "zmw_border_width"              ),
    ("ZMW_SIZE_CURRENT_STATE"                , "zmw_current_state"             ),
    ("ZMW_SIZE.current_state"                , "zmw_widget_current_state"      ),
    ("ZMW_CLIPPING.x"                        , "zmw_clipping_x"                ),
    ("ZMW_CLIPPING.y"                        , "zmw_clipping_y"                ),
    ("ZMW_CLIPPING.width"                    , "zmw_clipping_width"            ),
    ("ZMW_CLIPPING.height"                   , "zmw_clipping_height"           ),
    ("ZMW_CLIPPING"                          , "zmw_clipping"                  ),
    ("zMw->u.parent_to_child"                , "zmw_parent_to_child"           ),
    ("ZMW_SIZE"                              , "zmw_size"                      ),
    ("ZMW_COLORS"                            , "zmw_colors"                    ),
    ("ZMW_GC"                                , "zmw_gc"                        ),
    ("zMw"                                   , "zmw_state"                     ),


    ("ZMW_PROFILING_DISPLAYED"               , "zmw_zmw_profiling_displayed"   ),
    ("zmw.event"                             , "zmw_zmw_event"                 ),
    ("zmw.x"                                 , "zmw_zmw_x"                     ),
    ("zmw.y"                                 , "zmw_zmw_y"                     ),
    ("zmw.x_root"                            , "zmw_zmw_x_root"                ),
    ("zmw.y_root"                            , "zmw_zmw_y_root"                ),
    ("zmw.window"                            , "zmw_zmw_window"                ),
    ("zmw.focus"                             , "zmw_zmw_focus_with_cursor"     ),
    ("zmw.button_pressed"                    , "zmw_zmw_button_pressed"        ),
    ("zmw.key_pressed"                       , "zmw_zmw_key_pressed"           ),
    ("zmw.tip_displayed"                     , "zmw_zmw_tip_displayed"         ),
    ("zmw.tips_yet_displayed"                , "zmw_zmw_tips_yet_displayed"    ),
    ("zmw.still_yet_displayed"               , "zmw_zmw_still_yet_displayed"   ),
    ("zmw.zmw_table"                         , "zmw_zmw_table"                 ),
    ("zmw.raised"                            , "zmw_zmw_raised"                ),
    ("zmw.event_removed"                     , "zmw_zmw_event_removed"         ),
    ("&zmw.widget_to_trace"                  , "zmw_zmw_widget_to_trace_get"   ),
    ("&zmw.found"                            , "zmw_zmw_found"                 ),
    ("zmw.external_do_not_make_init"         , "zmw_zmw_external_do_not_make_init"),

    ]:
    before = before.replace("[", r"\[")
    before = before.replace("]", r"\]")
    before = before.replace(".", r"\.")
    before = before.replace("*", r"\*")
    rename.append([before + r"[+][+]", r"(*" + after + r"_get_ptr())++"])
    rename.append([before + r"[-][-]", r"(*" + after + r"_get_ptr())--"])
    rename.append([r"([^*a-z-A-Z0-9_])" + before + r"[ \t]*=[ \t]*([^;!=]+)", r"\1" + after + r"_set(\2)"])
    rename.append([before, after + r"_get()"])


    

# To test only some replacement
# rename = [ x for x in rename if x[0].find("ZMW_SIZE_MIN\\.width") >= 0 ]

#
# Compile regexp
#

for r in rename:
    old,new = r

    if old[0] != '(':
        r.append( sre.compile(r"\b" + old + r"(?=\W)" ))
    else:
        r.append( sre.compile(old + r"(?=\W)" ))
    
    new = new.replace(r"(\1,\2)","")
    new = new.replace(r"(\1)","")
    new = new.replace(r"\1","")
    new = new.replace(r"(\2)","")
    new = new.replace(r"\2","")
    new = new.replace("(",r"\(")
    new = new.replace(")",r"\)")
    new = new.replace("*",r"\*")
    new = new.replace("+",r"\+")
    r.append( sre.compile("\\b" + new + "\\b") )

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
        old,new,old_comp, new_comp = a
        if new_comp.search(content) != None:
            print "Found %-30s in %s" % (new, filename)
            a[0] = None

    rename = filter(lambda a: a[0], rename)


if rename:
    if False:
        print "Replacement that will be done in the files:"
        for old,new,oldc,newc in rename:
            print "%39s %39s" % (old,new)
        else:
            if False:
                for old,new,oldc,newc in rename:
                    if old.find("FOCUS") >=0:
                        print "%39s %39s" % (old,new)
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
    for (old,new,r,unused) in rename:
        new_content = r.sub(new, new_content)

    new_content = new_content.replace(" );", ") ;")
    
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

