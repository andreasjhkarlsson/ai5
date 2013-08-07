cd AI5Compiler
python build_exe.py build
copy build\exe.win32-3.3\* ..\wscite\bin\
copy ai5-include\* ..\wscite\bin\ai5-include\
cd ..
copy AI5Runtime\Release\AI5Runtime.exe wscite\bin\ai5r.exe
copy AI5Runtime\Release\*.dll wscite\bin\
pause