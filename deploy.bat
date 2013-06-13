cd AI5Compiler
python build_exe.py build
copy build\exe.win32-3.3\* ..\wscite\bin\
cd ..
copy AI5Runtime\x64\Release\AI5Runtime.exe wscite\bin\ai5r.exe