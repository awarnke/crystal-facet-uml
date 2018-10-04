#
# Regular cron jobs for the crystal-facet-uml package
#
0 4	* * *	root	[ -x /usr/bin/crystal-facet-uml_maintenance ] && /usr/bin/crystal-facet-uml_maintenance
