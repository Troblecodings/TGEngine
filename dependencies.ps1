param([String]$refresh_deps="no-refresh")

Write-Output "Dependencies 1.0"
if(-not (Test-Path stb) -or ($refresh_deps -eq  "refresh")){
    if(Test-Path stb){
        Write-Output "Refresh stb"
        Remove-Item stb -Force -Recurse
    }
    Write-Output "Get stb"
    git clone https://github.com/nothings/stb
}
if (-not (Test-Path dependencies) -or ($refresh_deps -eq  "refresh")) {
    if(Test-Path dependencies){
        Write-Output "Refresh vulkan, fbxsdk, sdl"
        Remove-Item dependencies -Force -Recurse
    }
    Write-Output "Get vulkan, fbxsdk, sdl"
    New-Item dependencies -ItemType Directory
    Set-Location dependencies
    Invoke-WebRequest "https://dl.dropboxusercontent.com/s/zgaqnqdaqjkngxc/Deps.zip" -OutFile deps.zip
    Set-Location ..\
}
if (-not (Test-Path dependencies\deps) -or ($refresh_deps -eq  "refresh")) {
    if(Test-Path dependencies\deps){
        Write-Output "Refresh unpack vulkan, fbxsdk, sdl"
        Remove-Item dependencies\deps -Force -Recurse
    }
    Write-Output "Unpack vulkan, fbxsdk, sdl"
    Set-Location dependencies
    Expand-Archive -Path "deps.zip"
    Set-Location ..\
}
Write-Output "Adding to variables"
$includes = (Resolve-Path "dependencies\deps\Deps\fbx\include").ToString() + ";"
$includes += (Resolve-Path "dependencies\deps\Deps\Third-Party\Include").ToString() + ";"
$includes += (Resolve-Path "dependencies\deps\Deps\vulkan\Include").ToString() + ";"
$includes += (Resolve-Path "stb").ToString() + ";"
[Environment]::SetEnvironmentVariable("TGE_INCLUDE", $includes, "User")
$lib_names = "SDL2.lib;vulkan-1.lib;libfbxsdk-mt.lib"
[Environment]::SetEnvironmentVariable("TGE_LIB_NAMES", $lib_names, "User")
$lib_path_32 = (Resolve-Path "dependencies\deps\Deps\Third-Party\Bin32").ToString() + ";"
$lib_path_32 += (Resolve-Path "dependencies\deps\Deps\vulkan\Lib32").ToString() + ";"
$lib_path_32 += (Resolve-Path "dependencies\deps\Deps\fbx\lib\vs2015\").ToString()
[Environment]::SetEnvironmentVariable("TGE_LIBS_32", $lib_path_32, "User")
$lib_path = (Resolve-Path "dependencies\deps\Deps\Third-Party\Bin").ToString() + ";"
$lib_path += (Resolve-Path "dependencies\deps\Deps\vulkan\Lib").ToString() + ";"
$lib_path += (Resolve-Path "dependencies\deps\Deps\fbx\lib\vs2015\").ToString()
[Environment]::SetEnvironmentVariable("TGE_LIBS", $lib_path, "User")
