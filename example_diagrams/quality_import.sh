#!/bin/sh
if test -e "quality.json"; then
    echo "quality.json exists"
    if test -e "quality.cfu1"; then
        echo "quality.cfu1 is deleted..."
        rm "quality.cfu1"
    fi
    if ! crystal-facet-uml -i quality.cfu1 add quality.json; then
        echo "ERROR at conversion: $?"
    else
        if ! crystal-facet-uml -t quality.cfu1; then
            echo "ERROR: converted data is corrupt: $?, please repair."
        else
            echo "finished."
        fi
    fi
else
    echo "ERROR: nothing to import."
fi
