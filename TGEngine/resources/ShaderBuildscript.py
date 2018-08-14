import os
import os.path
import subprocess
from shutil import copyfile
import traceback
import sys
import struct


def readtoint(read):
    return struct.unpack('b', read)[0]


c_path = os.getcwd()
total = 0
finished = 0
for name in os.listdir(c_path):
    pth = c_path + "\\" + name
    if name.endswith(".spv"):
        os.remove(pth)
        print("Deleted " + name + " due cleanup");
for name in os.listdir("..\\TGEditor\\"):
    pth = "..\\TGEditor\\" + name
    if name.endswith(".spv"):
        os.remove(pth)
        print("Deleted " + name + " due cleanup")
shader_data = open("resources\\ShaderData.cpp", "w")
shader_data.write("#include \"ShaderData.hpp\"\n\n")
shader_data.write("std::vector<std::vector<char>> shader_data = {\n")
shader_stages = []
first = True
for name in os.listdir(c_path + "\\resources"):
    try:
        pth = c_path + "\\resources\\" + name
        if name.endswith(".glsl"):
            total += 1
            print("Detected shader: " + name)
            shaderstage = "frag"
            if name.startswith("Vertex"):
                shaderstage = "vert"
                print("Detected vertex shader")
                shader_stages.append("VK_SHADER_STAGE_VERTEX_BIT")
            else:
                shader_stages.append("VK_SHADER_STAGE_FRAGMENT_BIT")
            p = subprocess.Popen(["D:\\Vulkan\\Bin32\\glslangValidator.exe", "-V", "-o", pth.replace(".glsl", "") +
                                  ".spv", "-H", "-S",  shaderstage, pth],
                                 stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            for line in iter(p.stdout.readline, b''):
                print(">>> " + str(line.rstrip()).replace("b'", ""))
            b = True
            for line in iter(p.stderr.readline, b''):
                if b:
                    print("There were errors while compiling " + name)
                    b = False
                print(">>> " + line)
            if not b:
                continue;
            print("Reading " + pth.replace(".glsl", "") + ".spv")
            cshader = open(pth.replace(".glsl", "") + ".spv", "a")
            size = cshader.tell()
            cshader.close()
            cshader = open(pth.replace(".glsl", "") + ".spv", "rb")
            cchar = cshader.read(1)
            if not first:
                shader_data.write(",")
            first = False
            shader_data.write( "{ " + str(readtoint(cchar)))
            for x in range(0, size - 1):
                cchar = cshader.read(1)
                shader_data.write(", " + str(readtoint(cchar)))
            shader_data.write( " }\n")
            cshader.close()
            print("Successfully added shader")
            finished += 1
    except Exception:
        print("Shader " + name + " failed to Compile")
        traceback.print_exc()
        continue
shader_data.write( "};\nVkShaderStageFlagBits shader_flags[] = {\n")
first = True
for stage in shader_stages:
    if not first:
        shader_data.write(",")
    first = False
    shader_data.write(stage + "\n")
shader_data.write("};")
shader_data.close()
print("Compiled " + str(finished) + " out of " + str(total) + " shaders")
