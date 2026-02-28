@echo off

set RAYLIB_INCLUDE=..\external\raylib\include
set RAYLIB_LIB=..\external\raylib\lib

if not exist build mkdir build
pushd build

cl ..\main.c /EHsc /O2 /MD ^
   /I%RAYLIB_INCLUDE% ^
   /Fe:subframes.exe ^
   /link /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup^
   /LIBPATH:%RAYLIB_LIB% ^
   raylib.lib opengl32.lib gdi32.lib winmm.lib kernel32.lib user32.lib shell32.lib
    
popd
