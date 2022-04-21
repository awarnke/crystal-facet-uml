@rem set XDG_DATA_DIR="%CD%\share"
@rem set XDG_DATA_DIRS="%CD%\share\glib-2.0\schemas"
@echo when calling crystal-facet-uml.bat, use absolute paths in parameters
@echo e.g. crystal-facet-uml.bat -u "%%CD%%\crystal-facet-uml_mod5g-example.cfu1"
@cd share\glib-2.0\schemas
@..\..\..\bin\crystal-facet-uml.exe %1 %2 %3 %4
@cd ..\..\..