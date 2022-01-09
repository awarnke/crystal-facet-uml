#!/bin/sh
if test -e "quality.cfu1"; then
    echo "quality.cfu1 exists"
    if test -e "quality.json"; then
        echo "quality.json is deleted..."
        rm "quality.json"
    fi
    if ! crystal-facet-uml -e quality.cfu1 json . ; then
        echo "ERROR at conversion: $?"
    else
        echo "quality.cfu1 is deleted..."
        rm "quality.cfu1"
        echo "finished."
    fi
else
    echo "ERROR: nothing to export."
fi
