import zipfile
import traceback
import os
import os.path
import urllib.request
import sys


def clear(): os.system("cls")


vulkan = os.getenv("VULKAN_SDK")
dependencies_file = None
msg = None


def wrt(vk, src):
    print(".", end="", flush=True)
    dependencies_file.write(vk + src, arcname=src, compress_type=zipfile.ZIP_DEFLATED)


def wrtdir(vk, src):
    for str in os.listdir(vk + src):
        if os.path.isdir(vk + src + str):
            wrtdir(vk, src + str + "\\")
        else:
            wrt(vk, src + str)


def trigger(id):
    try:
        if id == 0:
            exit(0)
        elif id == 1:
            print("Downloading")
            urllib.request.urlretrieve("http://seafile.media-dienste.de/f/85da9d3e98b347a490f6/?dl=1",
                                       "Dependencies.zip")
            print("Extract Archive")
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
    print("       DEPENDENCIES 2.0      ")
    print("=============================")
    print("")
    if not msg == None:
        print(msg)
        print("")
    print("1. Get dependencies")
    print("2. Pack dependencies")
    print("3. Compile shader")
    print("4. Deploy engine")
    print("0. Close")
    try:
        trigger(int(input()))
    except ValueError:
        clear()
        print(traceback.format_exc())
        continue
