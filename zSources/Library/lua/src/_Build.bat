
del *.o
del *.obj
del *.lib
del *.exp
del *.dll
call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat" x64
::call D:\zVS2015\Common7\Tools\vsvars32.bat
cl /MD /O2 /c /DLUA_BUILD_AS_DLL *.c
ren lua.obj lua.o
ren luac.obj luac.o
link /DLL /IMPLIB:lua52.lib /OUT:lua52.dll *.obj 
link /OUT:lua.exe lua.o lua52.lib
lib /OUT:lua.lib *.obj
link /OUT:luac.exe luac.o lua.lib
pause