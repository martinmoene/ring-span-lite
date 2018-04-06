@echo off & setlocal
set std=%1
if not "%std%"=="" set std=-std:%std%

cl -W3 -EHsc %std% -Dlest_FEATURE_COLOURISE=0 -I../include/nonstd ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe
endlocal

