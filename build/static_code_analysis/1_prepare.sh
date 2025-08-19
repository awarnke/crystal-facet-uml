#!/bin/sh
echo "You possibly need to remove the constant GLIB_DEPRECATED_ENUMERATOR_IN_2_26 from /usr/include/glib-2.0/gobject/gparam.h"
echo "You possibly need to adapt the constant def of 2G_SPAWN_ERROR_2BIG2 in /usr/include/glib-2.0/glib/gspawn.h"
export PATH=$PATH:~/carbondisk/bin/cov-analysis-linux64-2024.6.1/bin
