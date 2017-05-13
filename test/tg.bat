@setlocal
@set std=%1
@if "%std%"=="" set std=c++11
g++ -std=%std% -O2 -Wall -Wextra -Wconversion -Wsign-conversion -Wno-unused-parameter -Dlest_FEATURE_COLOURISE=0 -o ring-span-lite.t.exe -I../include/nonstd ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe
@endlocal

