if not exist "./build" mkdir ./build
if not exist "./build/win" mkdir ./build/win
if not exist "./build/win" mkdir ./build/win/obj
if not exist "./build/win/asset" xcopy assets ./build/win/asset
cl -D_HAS_EXCEPTIONS=0 /MTd /MP /Fo./bin/win/obj\ /Febin/win/Aechmea.exe -I./lib/include opengl32.lib shell32.lib gdi32.lib user32.lib ./lib/bin/x64/*.lib src/*.c /link /NODEFAULTLIB:MSVCRTD.lib /NODEFAULTLIB:LIBCMT.lib
