call "%VS110COMNTOOLS%\VsDevCmd.bat"
msbuild WinHTTP.sln /t:Rebuild /p:Configuration=Release /p:Platform="Win32"
msbuild WinHTTP.sln /t:Build /p:Configuration=Debug /p:Platform="Win32"
