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
import platform

osn = platform.system()
if osn == "Windows":
    def clear():
        os.system("cls")
else:
    def clear():
        os.system("clear")


vulkan = os.getenv("VULKAN_SDK")
dependencies_file = None
msg = None


def isValidFile(name):
    return name.endswith(".h") or name.endswith(".xml") or name.endswith(
        ".hpp") or name.endswith(".md") or name.endswith(
            ".cpp") or name.endswith(".c") or name.endswith(
                ".bat") or name.endswith(".py") or name.endswith(
                    ".html") or name.endswith(".cs")


def wrt(vk, src, tp=""):
    print(".", end="", flush=True)
    dependencies_file.write(vk + src,
                            arcname=tp + src,
                            compress_type=zipfile.ZIP_DEFLATED)


def wrtdir(vk, src, tp=""):
    for str2 in os.listdir(vk + src):
        if os.path.isdir(vk + src + str2):
            wrtdir(vk, src + str2 + "\\", tp)
        else:
            wrt(vk, src + str2, tp)


files = 0
loc = 0


def find(path):
    global files
    global loc
    global msg
    data = os.listdir(path)
    for file in data:
        if "stb" not in path and "dependencies" not in path and "fbx" not in path and os.path.isdir(
                path + "/" + file):
            find(path + "/" + file)
        elif isValidFile(file):
            files += 1
            print("#", end="", flush=True)
            with open(path + "/" + file, "rb") as fl:
                loc += len(fl.readline())


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
            print("Starting... this can take a while")
            dependencies_file = zipfile.ZipFile("Dependencies.zip", mode="w")
            updateSubmodules()
            wrtdir(vulkan, "\\Include\\")
            wrtdir(vulkan, "\\Lib\\")
            wrtdir(vulkan, "\\Lib32\\")
            wrtdir(vulkan, "\\Third-Party\\")
            wrt(vulkan, "\\LICENSE.txt")
            dependencies_file.close()
            msg = "Finished!"
            clear()
            return
        elif id == 2:
            find(os.getcwd())
            msg = "Found " + str(files) + " files\nWith " + str(
                loc) + " lines of code"
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
    print("       DEPENDENCIES 3.0      ")
    print("=============================")
    print("")
    if msg is not None:
        print(msg)
        print("")
    print("1. Get all dependencies")
    print("2. Get states")
    print("0. Close")
    try:
        trigger(int(input()))
    except ValueError:
        clear()
        print(traceback.format_exc())
        continue
