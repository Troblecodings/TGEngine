import zipfile
import traceback
import os
import os.path
import urllib.request
import sys
import subprocess
import struct
import shutil


def clear(): os.system("cls")


def readtoint(read):
    return struct.unpack('b', read)[0]


vulkan = os.getenv("VULKAN_SDK")
dependencies_file = None
msg = None

def isValidFile(name):
    return name.endswith(".h") or name.endswith(".xml") or name.endswith(".hpp") or name.endswith(".md") or name.endswith(".cpp") or name.endswith(".c") or name.endswith(".bat") or name.endswith(".py") or name.endswith(".html")

def compileshader():
    global msg
    c_path = os.getcwd()
    total = 0
    finished = 0
    for name in os.listdir(c_path):
        pth = c_path + "\\" + name
        if name.endswith(".spv"):
            os.remove(pth)
            print("Deleted " + name + " due cleanup");
    for name in os.listdir("TGEngine\\resources\\"):
        pth = "TGEngine\\resources\\" + name
        if name.endswith(".spv"):
            os.remove(pth)
            print("Deleted " + name + " due cleanup")
    shader_data = open("TGEngine\\resources\\ShaderData.cpp", "w")
    shader_data.write("#include \"ShaderData.hpp\"\n\n")
    shader_data.write("std::vector<std::vector<char>> shader_data = {\n")
    shader_stages = []
    first = True
    for name in os.listdir(c_path + "\\TGEngine\\resources"):
        try:
            pth = c_path + "\\TGEngine\\resources\\" + name
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
                p = subprocess.Popen([os.getenv("VULKAN_SDK") + "\\Bin\\glslangValidator.exe", "-V", "-o", pth.replace(".glsl", "") + ".spv", "-H", "-S",  shaderstage, pth],stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                for line in iter(p.stdout.readline, b''):
                    print(">>> " + str(line.rstrip()).replace("b'", ""))
                b = True
                for line in iter(p.stderr.readline, b''):
                    if b:
                        print("There were errors while compiling " + name)
                        b = False
                    print(">>> " + line)
                if not b:
                    continue
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
            msg = "Shader " + name + " failed to Compile"
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
    msg = "Compiled " + str(finished) + " out of " + str(total) + " shaders"
    if total == finished:
        clear()
    return


def wrt(vk, src):
    print(".", end="", flush=True)
    dependencies_file.write(vk + src, arcname=src, compress_type=zipfile.ZIP_DEFLATED)


def wrtdir(vk, src):
    for str in os.listdir(vk + src):
        if os.path.isdir(vk + src + str):
            wrtdir(vk, src + str + "\\")
        else:
            wrt(vk, src + str)

files = 0
loc = 0
def find(path):
    global files
    global loc
    global msg
    data = os.listdir(path)
    for file in data:
        if "stb" not in path and "dependencies" not in path and "fbx" not in path and os.path.isdir(path + "/" + file):
            find(path + "/" + file)
        elif isValidFile(file):
            files += 1
            print("#", end="", flush=True)
            with open(path + "/" + file, "rb") as fl:
                loc += len(fl.readline())


def callback(cob, size, total):
    print(str(round(((cob * size) / total) * 100, 1)) + "%", end="\r")


def getstb():
    if os.path.exists("stb") and len(os.listdir("stb")) > 0:
        print("Updating stb")
        p = subprocess.Popen(["git", "pull"], cwd="stb")
        p.wait()
    else:
        print("Cloning stb")
        p = subprocess.Popen(["git", "clone", "https://github.com/nothings/stb"])
        p.wait()


def trigger(id):
    global msg
    global dependencies_file
    global files
    global loc
    files = 0
    loc = 0
    try:
        if id == 0:
            exit(0)
        elif id == 1:
            getstb()
            print("Downloading")
            urllib.request.urlretrieve("http://seafile.media-dienste.de/f/85da9d3e98b347a490f6/?dl=1",
                                       "Dependencies.zip", callback)
            print("Finished download         ")
            print("Deleting old")
            if os.path.exists("dependencies"):
                shutil.rmtree("dependencies")
            print("Extracting Archive")
            dependencies_file = zipfile.ZipFile("Dependencies.zip", mode="r")
            dependencies_file.extractall(path="dependencies\\")
            dependencies_file.close()
            os.remove("Dependencies.zip")
            msg = "Finished!"
            clear()
            return
        elif id == 2:
            print("Starting... this can take a while")
            dependencies_file = zipfile.ZipFile("Dependencies.zip", mode="w")
            wrtdir(vulkan, "\\Bin\\")
            wrtdir(vulkan, "\\Bin32\\")
            wrtdir(vulkan, "\\Include\\")
            wrtdir(vulkan, "\\Lib\\")
            wrtdir(vulkan, "\\Lib32\\")
            wrtdir(vulkan, "\\Third-Party\\")
            wrt(vulkan, "\\LICENSE.txt")
            wrtdir("", "fbx\\")
            dependencies_file.close()
            msg = "Finished!"
            clear()
            return
        elif id == 3:
            compileshader()
            return
        elif id == 5:
            find(os.getcwd())
            msg = "Found " + str(files) + " files\nWith " + str(loc) + " lines of code"
            clear()
            return
        elif id == 6:
            getstb()
            msg = "Finished!"
            clear()
            return
    except IOError or ValueError:
        clear()
        print(traceback.format_exc())
        return


if len(sys.argv) > 1:
    for ids in sys.argv:
        try:
            trigger(int(ids))
        except ValueError:
            continue

while True:
    print("=============================")
    print("       DEPENDENCIES 2.4      ")
    print("=============================")
    print("")
    if msg is not None:
        print(msg)
        print("")
    print("1. Get all dependencies")
    print("2. Pack dependencies")
    print("3. Compile shader")
    print("4. Deploy engine")
    print("5. Get states")
    print("6. Get stb only")
    print("0. Close")
    try:
        trigger(int(input()))
    except ValueError:
        clear()
        print(traceback.format_exc())
        continue
