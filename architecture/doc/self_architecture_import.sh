#!/bin/sh
if test -e "self_architecture.json"; then
    echo "self_architecture.json exists"
    if test -e "self_architecture.cfu1"; then
        echo "self_architecture.cfu1 is deleted..."
        sleep 5
        rm "self_architecture.cfu1"
    fi
    if ! crystal-facet-uml -i self_architecture.cfu1 add self_architecture.json; then
        echo "ERROR at conversion: $?"
    else
        if ! crystal-facet-uml -t self_architecture.cfu1; then
            echo "ERROR: converted data is corrupt: $?, please repair."
        else
            echo "finished."
        fi
    fi
else
    echo "ERROR: nothing to import."
fi
