@echo off & setlocal
set std=%1
if "%std%"=="" set std=c++11

set     clang=C:\Program Files\LLVM\bin\clang
set     flags=-Wpedantic -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-padded -Wno-missing-noreturn -Wno-documentation-unknown-command -Wno-documentation-deprecated-sync -Wno-documentation -Wno-weak-vtables -Wno-missing-prototypes -Wno-missing-variable-declarations -Wno-exit-time-destructors -Wno-global-constructors

"%clang%" -std=%std% -O2 -Wall -Wextra -Wno-unused-parameter %flags% -Dlest_FEATURE_COLOURISE=0 -fms-compatibility-version=19.00 -isystem "%VCInstallDir%include" -isystem "%WindowsSdkDir_71A%include" -I../include/nonstd -o ring-span-lite.t.exe -I../include/nonstd ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe
endlocal
