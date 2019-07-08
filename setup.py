import zipfile
import traceback
import os
import os.path
import urllib.request
import sys
import subprocess
import struct
import shutil
from shutil import copyfile

def clear(): os.system("cls")


def readtoint(read):
    return struct.unpack('b', read)[0]


vulkan = os.getenv("VULKAN_SDK")
dependencies_file = None
msg = None

def isValidFile(name):
    return name.endswith(".h") or name.endswith(".xml") or name.endswith(".hpp") or name.endswith(".md") or name.endswith(".cpp") or name.endswith(".c") or name.endswith(".bat") or name.endswith(".py") or name.endswith(".html") or name.endswith(".cs")

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
            print("Thanks to David Quenzer for giving me access to his cloud storage!")
            getstb()
            print("Checking version!")
            version = ""
            if os.path.exists("dependencies/dependencie_version.txt"):
                with open("dependencies/dependencie_version.txt") as versionfile:
                    version = versionfile.read();
            urllib.request.urlretrieve("http://seafile.media-dienste.de/f/fd8adf05cce34c5086d0/?dl=1",
                                       "dependencies/dependencie_version.txt", callback)
            with open("dependencies/dependencie_version.txt") as versionfile:
                if versionfile.read() == version:
                    msg = "Already up to date!"
                    clear()
                    return
            print("Downloading... this can take a while")
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
            urllib.request.urlretrieve("http://seafile.media-dienste.de/f/fd8adf05cce34c5086d0/?dl=1",
                                       "dependencies/dependencie_version.txt", callback)
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
        elif id == 4:
            print("Starting deployment...")
            dependencies_file = zipfile.ZipFile("Release.zip", mode="w")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x64\\Debug\\TGEngine.lib")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x86\\Debug\\TGEngine.lib")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x64\\Release\\TGEngine.lib")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x86\\Release\\TGEngine.lib")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x64\\Debug\\TGEngine.pdb")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x86\\Debug\\TGEngine.pdb")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x64\\Release\\TGEngine.pdb")
            wrt(os.getcwd() + "\\TGEngine\\run", "\\x86\\Release\\TGEngine.pdb")
            wrt(os.getcwd(), "\\LICENSE")
            dependencies_file.close()
            msg = "Finished!"
            clear()
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
        elif id == 7:
            print("Put in project name")
            prname = input()
            os.mkdir(prname)
            for x in os.listdir("default_project"):
                shutil.copyfile("default_project/" + x, prname + "/" + x.replace("%projectname%", prname))
            for x in os.listdir(prname):
                fls = ""
                with open(prname + "/" + x, "r") as file:
                    for line in file:
                        fls += line.replace("%projectname%", prname) + "\n"
                with open(prname + "/" + x, "w") as file:
                    file.write(fls)
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
    print("4. Deploy engine")
    print("5. Get states")
    print("6. Get stb only")
    print("7. Create project")
    print("0. Close")
    try:
        trigger(int(input()))
    except ValueError:
        clear()
        print(traceback.format_exc())
        continue
