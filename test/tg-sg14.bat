@setlocal
@set std=%1
@if "%std%"=="" set std=c++14
g++ -std=%std% -O2 -Wall -Wextra -Wno-unused-parameter -o ring-span-lite.t.exe -I../include/sg14 -Dnsrs_SG14 -Dnsrs_RING_SPAN_LITE_T_HEADER=\"ring-span-sg14.t.hpp\" ring-span-lite.t.cpp ring-span.t.cpp && ring-span-lite.t.exe
@endlocal

