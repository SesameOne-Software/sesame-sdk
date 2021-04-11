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

set compiler_flags=/LD /w /O2 /Ob3 /TC /GS- /guard:cf-
set linker_flags=/OUT:"bin\sesame-sdk.dll"
set build_targets=lib\csgo-sdk\*.c lib\hashmap\*.c lib\sds\*.c lib\vector\*.c lib\subhook\*.c lib\yyjson\*.c src\*.c src\hooks\*.c
set preprocessor=/D"_USE_MATH_DEFINES" /D"SUBHOOK_STATIC" /D"SUBHOOK_SEPARATE_SOURCE_FILES"
set include_dirs=/I"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include" /I"."
set libs=kernel32.lib user32.lib advapi32.lib d3d9.lib d3dx9.lib
set library_dirs=/LIBPATH:"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86"

cl.exe %compiler_flags% %preprocessor% %include_dirs% %build_targets% /link %library_dirs% %libs% %linker_flags%

del *.obj *.pdb