import os
import os.path as pt
import subprocess
import random as rnd
import re


def runcommand(command, console, toolpath):
    '''
    Runs a command in the given mode
    1 for console only
    2 for arguments only
    3 for both
    '''
    result = []
    if console & 1 == 1:
        pr = subprocess.Popen(toolpath, text=True, shell=True)
        result.append(pr.communicate(input=command))
    if console & 2 == 2:
        print(toolpath + " " + command)
        result.append(subprocess.call(toolpath + " " + command))
    return tuple(result)


def rng(regex=r"[^\w]"):
    strsize = rnd.randint(0, 20)
    resultarray = str()
    for x in range(strsize):
        resultarray
    re.sub(regex, "", resultarray)
    return resultarray + "x"


def runtest(toolpath):
    print(runcommand("actor add " + rng(), 3, toolpath))


for path, name, files in os.walk("../bin"):
    if "ShaderTool.exe" in files:
        toolpath = pt.join(path, "ShaderTool.exe")
        runtest(toolpath)
