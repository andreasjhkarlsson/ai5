if not "%programfiles(x86)%"=="" call "%programfiles(x86)%\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86
if "%programfiles(x86)%"=="" call "%programfiles%\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x86
msbuild AI5Runtime.sln /t:Build /p:Configuration=Release;Platform=Win32