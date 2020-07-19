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
    ("pipe " + rng(), 3, (-2, -2)),

    ("shader", 3, (-1, -1)),
    ("shader make", 3, (0, 0)),
    ("shader list", 3, (0, 0)),
    ("shader compile", 3, (-1, -1)),
    ("shader " + rng(), 3, (-2, -2)),

    ("texture add", 3, (-1, -1)),
    ("texture add " + rng(), 3, (-2, -2)),
    ("texture rm", 3, (-1, -1)),
    ("texture rm " + rng(), 3, (-2, -2)),
    ("texture remove", 3, (-1, -1)),
    ("texture remove " + rng(), 3, (-2, -2)),
    ("texture list", 3, (0, 0)),
    ("texture " + rng(), 3, (-2, -2)),

    ("actor", 3, (-1, -1)),
    ("actor add", 3, (-1, -1)),
    ("actor rm", 3, (-1, -1)),
    ("actor remove", 3, (-1, -1)),
    ("actor list", 3, (0, 0)),
    ("actor vertex", 3, (-1, -1)),
    ("actor index", 3, (-1, -1)),
    ("actor transform", 3, (-1, -1)),
    ("actor layer", 3, (-1, -1)),
    ("actor instance", 3, (-1, -1)),
    ("actor " + rng(), 3, (-2, -2)),

    ("map", 3, (-1, -1)),
    ("map add", 3, (-1, -1)),
    ("map rm", 3, (-1, -1)),
    ("map remove", 3, (-1, -1)),
    ("map make", 3, (-1, -1)),
    ("map list", 3, (0, 0)),
    ("map addactor", 3, (-1, -1)),
    ("map rmactor", 3, (-1, -1)),
    ("map removeactor", 3, (-1, -1)),
    ("map addfont", 3, (-1, -1)),
    ("map rmfont", 3, (-1, -1)),
    ("map removefont", 3, (-1, -1)),
    ("map " + rng(), 3, (0, 0)),

    ("material", 3, (-1, -1)),
    ("material add", 3, (-1, -1)),
    ("material rm", 3, (-1, -1)),
    ("material remove", 3, (-1, -1)),
    ("material settexture", 3, (-1, -1)),
    ("material settex", 3, (-1, -1)),
    ("material setcolor", 3, (-1, -1)),
    ("material list", 3, (0, 0)),

    ("font", 3, (-1, -1)),
    ("font add", 3, (-1, -1)),
    ("font rm", 3, (-1, -1)),
    ("font remove", 3, (-1, -1)),
    ("font list", 3, (0, 0)),

}

FAILED = False

def runtest(toolpath):
    for test in TESTS:
        output = runcommand(test[0], test[1], toolpath)
        if output == test[2]:
            print("Test passed!")
        else:
            print("Failed at " + str(test) + " Output: " + str(output))
            FAILED = True


def cleanup():
    shutil.rmtree("Resources")
    for x in os.listdir("."):
        if x == "tests.py" or x == "test.glsl":
            continue
        os.remove(x)

cleanup()
for path, name, files in os.walk("../bin"):
    if "ShaderTool.exe" in files:
        toolpath = pt.abspath(pt.join(path, "ShaderTool.exe")).replace("\\", "/")
        runtest(toolpath)
cleanup()

if FAILED:
    exit(1)