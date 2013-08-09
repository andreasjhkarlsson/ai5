cd AI5Compiler
python build_exe.py build
rmdir ..\wscite\bin /s /q
mkdir ..\wscite\bin
mkdir ..\wscite\bin\ai5-include
copy build\exe.win32-3.3\* ..\wscite\bin\
copy ai5-include\* ..\wscite\bin\ai5-include\
cd ..
cd AI5Runtime
call build_release.bat
cd ..
copy AI5Runtime\Release\AI5Runtime.exe wscite\bin\ai5r.exe
copy AI5Runtime\Release\*.dll wscite\bin\
pause