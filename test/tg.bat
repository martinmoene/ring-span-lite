@echo off & setlocal
set std=%1
if "%std%"=="" set std=c++11

set flags=-Wpedantic -Wconversion -Wsign-conversion -Wno-unused-parameter -Wno-padded -Wno-missing-noreturn
set   gpp=g++

%gpp% -std=%std% -O2 -Wall -Wextra %flags% -o ring-span-lite.t.exe -I../include/nonstd ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe
endlocal
