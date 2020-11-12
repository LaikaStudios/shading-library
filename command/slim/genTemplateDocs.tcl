##
##  Copyright(c) 2011 Laika, developed under contract by mitchpraterLLC.
##  All rights reserved unless otherwise granted. This program or documentation
##  contains proprietary, confidential information and trade secrets of Laika
##  and/or mitchpraterLLC. Use of copyright notice does not imply publication.
##  
##  By possessing this software, you agree to the following terms and conditions.
##  You may not resell this software, but you may freely distribute it to others.
##  You may use it for whatever purposes you wish. There are no warranties of any
##  kind. You may make modifications, however any derivation may only be distributed
##  to others under the same freedoms granted to you herein, it must retain this
##  notice and agreement, and the modifications noted. We also ask that you make
##  such modifications available to Laika, mitchpraterLLC, or to the computer
##  graphics community as a whole through a public forum.
##
#
#   Generate web page documentation for loaded templates.
#   Uses Twiki formatting for the generated documentation.
#   The output is designed to be attached to a topic and included in the Twiki page.
#
proc _removePostfix {name} {
    regsub {_[0123456789]*$} $name {} name
    return $name
}

proc _removeItem {theItem theList} {
    set result {}
    foreach item $theList {
        if {$item != $theItem} {
            lappend result $item
        }
    }
    return $result
}

proc _uniqueList {theList} {
    set uniqueList {}
    foreach item $theList {
        if {[lsearch -exact $uniqueList $item] == -1} {
            lappend uniqueList $item
        }
    }
    return $uniqueList
}


proc genAllTemplateDocs {filename} {

    set pal [slim CreatePalette -new]
    set templates [slim GetTemplates]

    set funcNames {}
    foreach template $templates {
        set func [$pal CreateInstance -template $template]
        set funcName [$func GetLabel]

        set funcHand($funcName) $func
        set type [$func GetType]
        set funcType($funcName) $type
        set tags [_removeItem $type [$func GetTags]]
        set funcTags($funcName) [_removeItem laika $tags]
        set funcProg($funcName) [$func GetProgenitor]
        set funcDesc($funcName) [$func GetDescription]

        if {[lsearch -exact $tags laika] != -1 || [regsub laika [$func GetProgenitor] {} dummy]} {
            set funcFactory($funcName) Laika
        } else {
            set funcFactory($funcName) ""
        }

        if {[slim GetAppearances -name $funcName -type _Attachable] != {}} {
            set funcCategory($funcName) Attachable
        } else {
            set funcCategory($funcName) Subordinate
        }

        lappend funcNames $funcName
    }
    set funcNames [lsort -ascii -increasing $funcNames]

    set outFile [open $filename w]
    puts $outFile "<noautolink>"

    puts $outFile "%TABLE{sort=\"on\"}%"
    puts $outFile "| *Attachable* ||||"
    puts $outFile "| *Type* | *Name* | *Description* |"

    foreach funcName $funcNames {
        set name [_removePostfix $funcName]

        # First Output Laika Attachable templates.
        if {$funcFactory($funcName) == "Laika" && $funcCategory($funcName) == "Attachable"} {
            puts $outFile "| $funcType($funcName) %BR% %BR% $funcTags($funcName) | $name | $funcDesc($funcName) |"
        }
    }

    puts $outFile "</noautolink>"
    puts $outFile "<noautolink>"

    puts $outFile "%TABLE{sort=\"on\"}%"
    puts $outFile "| *Subordinate* ||||"
    puts $outFile "| *Type* | *Name* | *Description* |"

    foreach funcName $funcNames {
        set name [_removePostfix $funcName]

        # Then Output Laika Subordinate templates.
        if {$funcFactory($funcName) == "Laika" && $funcCategory($funcName) == "Subordinate"} {
            puts $outFile "| $funcType($funcName) %BR% %BR% $funcTags($funcName) | $name | $funcDesc($funcName) |"
        }
    }

    puts $outFile "</noautolink>"
    close $outFile

    $pal Delete 1
}


proc genTypeTemplateDocs {filename} {

    set types {float color vector normal point manifold manifold_s multiple override lightblocker}
#set types {float}

    set outFile [open $filename w]

    set pal [slim CreatePalette -new]
    set allTypes [lsort [slim GetTemplates -types]]

#set types $allTypes

    foreach type $types {
        ::RAT::LogMsg INFO "------------------------------------------------------------"
        ::RAT::LogMsg INFO $type
        ::RAT::LogMsg INFO "------------------------------------------------------------"

        set templates [lsort [slim GetTemplates -type $type]]

        set funcNames {}
        foreach template $templates {
            set func [$pal CreateInstance -template $template]
            set funcName [$func GetLabel]

            set funcHand($funcName) $func
            set funcTags($funcName) [_uniqueList [concat [$func GetTags] [$func GetType]]]

            lappend funcNames $funcName
        }
        set funcNames [lsort -ascii -increasing $funcNames]

        foreach funcName $funcNames {
            if {[lsearch $funcTags($funcName) laika] != -1} {
                ::RAT::LogMsg INFO "[_removePostfix $funcName]: $funcTags($funcName) $funcHand($funcName)"
            }
        }
    }

    $pal Delete 1

}
