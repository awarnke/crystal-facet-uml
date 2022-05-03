#!/bin/sh
if test -e "mouse_droid.json"; then
    echo "mouse_droid.json exists"
    if test -e "mouse_droid.cfu1"; then
        echo "mouse_droid.cfu1 is deleted in 5 secs..."
        sleep 2
        echo "mouse_droid.cfu1 is deleted in 3 secs..."
        sleep 2
        echo "mouse_droid.cfu1 is deleted in 1 secs..."
        sleep 1
        rm "mouse_droid.cfu1"
    fi
    echo "preparations finished; importing now..."
    if ! crystal-facet-uml -i mouse_droid.cfu1 add mouse_droid.json; then
        echo "ERROR at conversion: $?"
    else
        if ! crystal-facet-uml -t mouse_droid.cfu1; then
            echo "ERROR: converted data is corrupt: $?, please repair."
        else
            rm "mouse_droid.json"
            echo "import finished; starting now..."
            crystal-facet-uml -u mouse_droid.cfu1

            echo "using finished; exporting now..."
            if test -e "mouse_droid.json"; then
                echo "ERROR: mouse_droid.json was somehow re-created. Possibly by a parallel git checkout."
                echo "ERROR: export cancelled. Manual export and merge needed!"
            else
                if ! crystal-facet-uml -e mouse_droid.cfu1 json . ; then
                    echo "ERROR at conversion: $?"
                else
                    echo "mouse_droid.cfu1 is deleted..."
                    rm "mouse_droid.cfu1"
                    echo "finished."
                fi
            fi
        fi
    fi
else
    echo "ERROR: nothing to import."
fi
