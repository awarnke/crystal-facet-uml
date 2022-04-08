@echo set XDG_DATA_DIR="%CD%\share"
@echo set XDG_DATA_DIRS="%CD%\share\glib-2.0\schemas"
@cd share\glib-2.0\schemas
@..\..\..\bin\crystal-facet-uml.exe %1 %2 %3 %4
