#Requires -RunAsAdministrator
param([switch]$refresh,[switch]$refresh_vulkan,[String]$vulkan_dir="./dependencies/vulkan",[switch]$reinstall_vulkan)

Write-Output "Dependencies 2.0"
if(-not (Test-Path stb) -or $refresh){
    if(Test-Path stb){
        Write-Output "Refresh stb"
        Remove-Item stb -Force -Recurse
    }
    Write-Output "Get stb"
    git clone https://github.com/nothings/stb
}
if (-not (Test-Path dependencies) -or $refresh) {
    if(Test-Path dependencies){
        Write-Output "Refresh dependencies.zip"
        Remove-Item dependencies -Force -Recurse
    }
    Write-Output "Get dependencies.zip"
    New-Item dependencies -ItemType Directory
    Invoke-WebRequest "seafile.media-dienste.de/f/5884c0fc8d854dd5aefb/?dl=1" -OutFile dependencies.zip -ErrorAction Stop
    if(Test-Path dependencies){
        Write-Output "Refresh dependencies.zip"
        Remove-Item dependencies -Force -Recurse
    }
    Write-Output "Unpack dependencies.zip"
    Expand-Archive -Path "dependencies.zip"
    Remove-Item "dependencies.zip" 
}
#if(-not (Test-Path dependencies) -or $refresh -or $reinstall_vulkan){
#    if($env:VULKAN_SDK -and (Test-Path $env:VULKAN_SDK)){
#        Write-Output "Deinstalling old vulkan"
#        Invoke-Expression "& $env:VULKAN_SDK/Uninstall.exe /S" | Out-Null
#    }
#    Write-Output "Start vulkan installation"
#    if(-not (Test-Path dependencies/vulkan)){
#        New-Item dependencies/vulkan -ItemType Directory
#    }
#    $dir = (Resolve-Path $vulkan_dir).ToString()
#    Write-Output "Intall folder $dir. Switch path with -vulkan_dir [path] (Currently not working | Path: C:/Vulkan )"
#    Set-Location dependencies
#    .\Installer.exe /S /D=$dir
#}
Write-Output "Refreshing enviromental variables"
[Environment]::GetEnvironmentVariables("Machine")
Write-Output "Adding to variables"
$includes = (Resolve-Path "dependencies\fbx\include").ToString() + ";"
$includes += "$env:VULKAN_SDK\Third-Party\Include;"
$includes += "$env:VULKAN_SDK\Include;"
$includes += (Resolve-Path "stb").ToString() + ";"
[Environment]::SetEnvironmentVariable("TGE_INCLUDE", $includes, "Machine")
$lib_names = "SDL2.lib;vulkan-1.lib;libfbxsdk-mt.lib"
[Environment]::SetEnvironmentVariable("TGE_LIB_NAMES", $lib_names, "Machine")
$lib_path_32 = "$env:VULKAN_SDK\Third-Party\Bin32;"
$lib_path_32 += "$env:VULKAN_SDK\Lib32;"
$lib_path_32 += (Resolve-Path "dependencies\fbx\lib\vs2015\").ToString()
[Environment]::SetEnvironmentVariable("TGE_LIBS_32", $lib_path_32, "Machine")
$lib_path = "$env:VULKAN_SDK\Third-Party\Bin;"
$lib_path += "$env:VULKAN_SDK\Lib;"
$lib_path += (Resolve-Path "dependencies\fbx\lib\vs2015\").ToString()
[Environment]::SetEnvironmentVariable("TGE_LIBS", $lib_path, "Machine")
Write-Output "Finished"
