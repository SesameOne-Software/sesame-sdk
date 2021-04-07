@echo off
if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
) else (
    if exist "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
    ) else (
        call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x86
    )
)

set compiler_flags=/LD /Od /Zi /TC /I include
set linker_flags=/OUT:bin\sesame-sdk.dll
set build_targets=lib\csgo-sdk\*.c lib\hashmap\*.c lib\str\*.c lib\vector\*.c lib\minhook\*.c lib\minhook\hde\*.c src\*.c
set include_dirs=/I"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include" /I"."

cl.exe %compiler_flags% %include_dirs% %build_targets% /link %linker_flags%

del *.obj *.pdb