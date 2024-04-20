call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86 10.0.18362.0 -vcvars_ver=14.16
MSBuild.exe MM2Hook.sln /p:configuration=debug /p:platform=x86