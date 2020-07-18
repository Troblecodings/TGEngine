import os
import os.path as pt
import subprocess
import random as rnd
import re
import shutil

def runcommand(command, console, toolpath):
    '''
    Runs a command in the given mode
    1 for console only
    2 for arguments only
    3 for both
    '''
    result = [0, 0]
    if console & 1 == 1:
        pr = subprocess.Popen(toolpath, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
        outs, errs = pr.communicate(bytes(command + "\nexit\n", "utf-8"))
        outstr = str(outs)
        if "Exit code " in outstr:
            result[0] = int(re.split(r"(\\r|\\n)", outstr.split("Exit code ")[1])[0].strip())
    if console & 2 == 2:
        rtc = subprocess.call(toolpath + " " + command, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
        # This is bullshit
        # Who the fuck thought that negative numbers could come out of this
        if rtc == 0:
            result[1] = 0
        else:
            result[1] = rtc - 2**32
    return tuple(result)


def rng(regex=r"[\W]"):
    strsize = rnd.randint(0, 20)
    resultarray = str()
    for x in range(strsize):
        resultarray += chr(rnd.randint(0x40, 0x72))
    resultarray = re.sub(regex, "", resultarray, flags=re.M)
    return resultarray + "x"

TESTS = {
    ("map make " + rng(), 3, (-2, -2)),
    (rng(), 3, (-2, -2)),
    ("map add " + rng(), 2, (0, 0)),
    ("map add test", 1, (0, 0)),
    ("map add " + rng(r"[\w\s]"), 3, (-2, -2)),
    ("map add", 3, (-1, -1)),
    ("map", 3, (-1, -1)),

    ("setcwd", 3, (-1, -1)),
    ("settool", 3, (0, 0)),
    ("setcwd " + pt.abspath(os.getcwd()), 3, (0, 0)),

    ("pipe", 3, (-1, -1)),
    ("pipe create", 3, (-1, -1)),
    ("pipe delete", 3, (-1, -1)),
    ("pipe show", 3, (-1, -1)),
    ("pipe update", 3, (-1, -1)),
    ("pipe list", 3, (0, 0)),
    ("pipe make", 3, (0, 0)),

    ("shader", 3, (-1, -1)),
    ("texture", 3, (-1, -1)),
    ("actor", 3, (-1, -1)),
    ("map", 3, (-1, -1)),
    ("material", 3, (-1, -1)),
    ("font", 3, (-1, -1)),
}

FAILED = False

def runtest(toolpath):
    shutil.rmtree("Resources")
    for x in os.listdir("."):
        if x == "tests.py":
            continue
        os.remove(x)
    for test in TESTS:
        output = runcommand(test[0], test[1], toolpath)
        if output == test[2]:
            print("Test passed!")
        else:
            print("Failed at " + str(test) + " Output: " + str(output))
            FAILED = True


for path, name, files in os.walk("../bin"):
    if "ShaderTool.exe" in files:
        toolpath = pt.abspath(pt.join(path, "ShaderTool.exe")).replace("\\", "/")
        runtest(toolpath)

if FAILED:
    exit(1)