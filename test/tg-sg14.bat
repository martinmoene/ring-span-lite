@setlocal
@set std=%1
@if "%std%"=="" set std=c++14
g++ -std=%std% -O2 -Wall -Wextra -Wno-unused-parameter -Dlest_FEATURE_COLOURISE=0 -I../include -Dnsrs_CONFIG_STRICT_P0059 -Dnsrs_RING_SPAN_LITE_T_HEADER=\"ring-span-sg14.t.hpp\" -o ring-span-main.t.exe ring-span-main.t.cpp ring-span.t.cpp && ring-span-main.t.exe
@endlocal

