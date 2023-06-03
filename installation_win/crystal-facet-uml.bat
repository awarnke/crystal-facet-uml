@echo off
rem when calling crystal-facet-uml.bat, use absolute paths in parameters
rem e.g. crystal-facet-uml.bat -u "%%CD%%\crystal-facet-uml_mod5g-example.cfu1"
rem To not accidentially load a wrong library, reduce the path:
set PATH=bin
start /B bin\crystal-facet-uml.exe %1 %2 %3 %4
