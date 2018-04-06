@echo off & setlocal
set std=%1
if "%std%"=="" set std=c++11

set flags=-Wpedantic -Wconversion -Wsign-conversion -Wno-unused-parameter -Wno-padded -Wno-missing-noreturn
set   gpp=g++

%gpp% -std=%std% -O2 -Wall -Wextra %flags% -o ring-span-main.t.exe -I../include/nonstd ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe
endlocal
