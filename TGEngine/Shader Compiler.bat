@echo off
D:\Vulkan\1.0.65.0\Bin32\glslangValidator -H -S vert vertexshader.glsl
D:\Vulkan\1.0.65.0\Bin32\glslangValidator -H -S frag fragmentshader.glsl
copy vert.spv "D:\Documents\Visual Studio 2017\Projects\TGEngine\TGEditor\vert.spv"
copy frag.spv "D:\Documents\Visual Studio 2017\Projects\TGEngine\TGEditor\frag.spv"
pause