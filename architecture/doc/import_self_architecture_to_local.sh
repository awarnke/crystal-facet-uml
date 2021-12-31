#!/bin/sh
if test -e "self_architecture.json"; then
    echo "self_architecture.json exists"
    if test -e "self_architecture.cfu1"; then
        echo "self_architecture.cfu1 is deleted..."
        rm "self_architecture.cfu1"
    fi
    if ! crystal-facet-uml -i self_architecture.cfu1 update self_architecture.json; then
        echo "ERROR at conversion: $?"
    else
        if ! crystal-facet-uml -t self_architecture.cfu1; then
            echo "ERROR: converted data is corrupt: $?, please repair."
        fi
    fi
fi
