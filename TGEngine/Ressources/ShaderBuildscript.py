import os
import os.path
import subprocess
from shutil import copyfile
import traceback
import sys

c_path = os.getcwd();
total = 0;
finished = 0;
for name in os.listdir(c_path):
    pth = c_path + "\\" + name;
    if(name.endswith(".spv")):
        print(os.remove(pth));
for name in os.listdir(c_path):
    try:
        pth = c_path + "\\" + name;
        if(name.endswith(".glsl")):
            total += 1;
            print("Detected shader: " + name)
            shaderstage = "vert";
            if(name.startswith("fragmentshader")):
                shaderstage = "frag"
                print("Detected fragmentshader")
            p = subprocess.Popen(["D:\\Vulkan\\Bin32\\glslangValidator.exe", "-V", "-o", name.replace(".glsl", "") + ".spv", "-H", "-S",  shaderstage, pth], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            for line in iter(p.stdout.readline, b''):
                print(">>> " + str(line.rstrip()).replace("b'", ""))
            b = True;
            for line in iter(p.stderr.readline, b''):
                if b:
                    print("There were errors while compiling " + name)
                    b = False
                print(">>> " + line)
            if not b:
                continue;
            copyfile(pth.replace(".glsl", "") + ".spv", "D:\\Documents\\Visual Studio 2017\\Projects\\TGEngine\\TGEditor\\" + name.replace(".glsl", "") + ".spv")
            print("Successfully copyed file")
            finished += 1;
    except(Exception):
        print("Shader " + name + " failed to Compile")
        traceback.print_exc()
        continue;
print("Compiled " + str(finished) + " out of " + str(total) + " shaders")
    
