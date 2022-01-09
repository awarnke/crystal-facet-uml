#!/bin/sh
if test -e "self_architecture.cfu1"; then
    echo "self_architecture.cfu1 exists"
    if test -e "self_architecture.json"; then
        echo "self_architecture.json is deleted..."
        rm "self_architecture.json"
    fi
    if ! crystal-facet-uml -e self_architecture.cfu1 json . ; then
        echo "ERROR at conversion: $?"
    else
        echo "self_architecture.cfu1 is deleted..."
        rm "self_architecture.cfu1"
        echo "finished."
    fi
else
    echo "ERROR: nothing to export."
fi
