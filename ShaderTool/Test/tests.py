import os
import os.path as pt
import subprocess
import random as rnd
import re
import shutil

VERBOSE = False

CLEANUPLIST = ("ShaderData.cpp", "ShaderData.hpp", "ShaderPipes.cpp",
               "ShaderPipes.hpp")


def cleanup():
    if os.path.exists("Resources"): shutil.rmtree("Resources")
    for x in CLEANUPLIST:
        if os.path.exists(x): os.remove(x)


def runcommand(command, console, toolpath):
    '''
    Runs a command in the given mode
    1 for console only
    2 for arguments only
    3 for both
    '''
    result = [0, 0]
    if console & 1 == 1:
        pr = subprocess.Popen(toolpath,
                              stdout=subprocess.PIPE,
                              stdin=subprocess.PIPE,
                              stderr=subprocess.PIPE)
        outs, errs = pr.communicate(bytes(command + "\nexit\n", "utf-8"))
        outstr = str(outs)
        if "Exit code " in outstr:
            result[0] = int(
                re.split(r"(\\r|\\n)",
                         outstr.split("Exit code ")[1])[0].strip())
        pr.wait()
    if console & 2 == 2:
        if VERBOSE:
            rtc = subprocess.call(toolpath + " " + command)
        else:
            rtc = subprocess.call(toolpath + " " + command,
                                  stdout=subprocess.PIPE,
                                  stdin=subprocess.PIPE,
                                  stderr=subprocess.PIPE)
        if rtc > 2**31 - 1:
            # Convert to signed int
            rtc -= 2**32
        result[1] = rtc
    return tuple(result)


def rng(regex=r"[\W]", size=0):
    if size == 0:
        size = rnd.randint(3, 20)
    resultarray = str()
    while len(resultarray) < size:
        resultarray += re.sub(regex,
                              "",
                              str(chr(rnd.randint(0x10, 128))),
                              flags=re.M)
    if len(resultarray) > size:
        resultarray = resultarray[:size]
    return resultarray


TESTS = [
    # General
    (rng(), 3, (-2, -2)),
    ("setcwd", 3, (-1, -1)),
    ("settool", 3, (0, 0)),
    ("setcwd " + pt.abspath(os.getcwd()), 3, (0, 0)),
    # Pipeline tests
    ("pipe", 3, (-1, -1)),
    ("pipe create", 3, (-1, -1)),
    ("pipe delete", 3, (-1, -1)),
    ("pipe show", 3, (-1, -1)),
    ("pipe update", 3, (-1, -1)),
    ("pipe list", 3, (0, 0)),
    ("pipe make", 3, (0, 0)),
    ("pipe " + rng(), 3, (-2, -2)),
    # Shader tests
    ("shader", 3, (-1, -1)),
    ("shader make", 3, (0, 0)),
    ("shader list", 3, (0, 0)),
    ("shader compile", 3, (-1, -1)),
    ("shader " + rng(), 3, (-2, -2)),
    # Texture tests
    ("texture add", 3, (-1, -1)),
    ("texture add \"" + rng() + "\"", 3, (-2, -2)),
    ("texture add " + rng(), 3, (-2, -2)),
    ("texture add \"test.png\"", 1, (0, 0)),
    ("texture rm test", 1, (0, 0)),
    ("texture add \"test.png\"", 2, (0, 0)),
    ("texture rm test", 2, (0, 0)),
    ("texture add \"test.png\"", 1, (0, 0)),
    ("texture remove test", 1, (0, 0)),
    ("texture add \"test.png\"", 2, (0, 0)),
    ("texture remove test", 2, (0, 0)),
    ("texture add \"test.png\"", 2, (0, 0)),
    ("texture add \"test.png\"", 3, (-2, -2)),
    ("texture rm", 3, (-1, -1)),
    ("texture rm " + rng(), 3, (-2, -2)),
    ("texture rm " + rng(r"[\W]"), 3, (-2, -2)),
    ("texture remove", 3, (-1, -1)),
    ("texture remove " + rng(), 3, (-2, -2)),
    ("texture list", 3, (0, 0)),
    ("texture " + rng(), 3, (-2, -2)),
    # Material Tests
    ("material", 3, (-1, -1)),
    ("material " + rng(), 3, (-2, -2)),
    ("material add", 3, (-1, -1)),
    ("material add " + rng(), 3, (-1, -1)),
    ("material add " + rng(r"[\w]") + " test", 3, (-2, -2)),
    ("material add test " + rng(), 3, (-2, -2)),
    ("material add test test", 1, (0, 0)),
    ("material rm test", 1, (0, 0)),
    ("material add test test", 2, (0, 0)),
    ("material rm test", 2, (0, 0)),
    ("material rm " + rng(), 3, (-2, -2)),
    ("material remove " + rng(), 3, (-2, -2)),
    ("material add test test", 1, (0, 0)),
    ("material remove test", 1, (0, 0)),
    ("material add test test " + rng(), 3, (-2, -2)),
    ("material add testmaterial test", 1, (0, 0)),
    ("material add testmaterial test", 3, (-2, -2)),
    ("material add test2 test " + rng(r"[^a-fA-F0-9]", 6), 1, (0, 0)),
    ("material add test3 test " + rng(r"[^a-fA-F0-9]", 6), 2, (0, 0)),
    ("material rm", 3, (-1, -1)),
    ("material remove", 3, (-1, -1)),
    ("material settexture", 3, (-1, -1)),
    ("material settexture test", 3, (-1, -1)),
    ("material settexture test2 test", 3, (0, 0)),
    ("material settexture " + rng() + " test", 3, (-2, -2)),
    ("material settexture test2 " + rng(), 3, (-2, -2)),
    ("material settex", 3, (-1, -1)),
    ("material settex test", 3, (-1, -1)),
    ("material settex test2 test", 3, (0, 0)),
    ("material settex " + rng() + " test", 3, (-2, -2)),
    ("material settex test2 " + rng(), 3, (-2, -2)),
    ("material setcolor", 3, (-1, -1)),
    ("material setcolor " + rng(), 3, (-1, -1)),
    ("material setcolor test3 " + rng(), 3, (-2, -2)),
    ("material setcolor test3 " + rng(r"[^a-fA-F0-9]", 6), 3, (0, 0)),
    ("material list", 3, (0, 0)),
    # Actor tests
    ("actor", 3, (-1, -1)),
    ("actor add", 3, (-1, -1)),
    ("actor add " + rng(), 3, (-1, -1)),
    ("actor add testactor testmaterial", 1, (0, 0)),
    ("actor add testactor2 testmaterial", 2, (0, 0)),
    ("actor rm", 3, (-1, -1)),
    ("actor remove", 3, (-1, -1)),
    ("actor list", 3, (0, 0)),
    ("actor vertex", 3, (-1, -1)),
    ("actor index", 3, (-1, -1)),
    ("actor transform", 3, (-1, -1)),
    ("actor layer", 3, (-1, -1)),
    ("actor instance", 3, (-1, -1)),
    ("actor " + rng(), 3, (-2, -2)),
    # Map tests
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
    ("map make " + rng(), 3, (-2, -2)),
    ("map add " + rng(), 2, (0, 0)),
    ("map add " + rng(r"[\w\s]"), 3, (-2, -2)),
    ("map add test", 1, (0, 0)),
    ("map add test", 3, (-2, -2)),
    ("map rm test", 1, (0, 0)),
    ("map add test", 2, (0, 0)),
    ("map rm test", 2, (0, 0)),
    ("map add test", 2, (0, 0)),
    ("map remove test", 2, (0, 0)),
    ("map add test", 2, (0, 0)),
    ("map remove test", 2, (0, 0)),
    ("map remove " + rng(), 3, (-2, -2)),
    ("map " + rng(), 3, (-2, -2)),
    # Font tests
    ("font", 3, (-1, -1)),
    ("font add", 3, (-1, -1)),
    ("font rm", 3, (-1, -1)),
    ("font remove", 3, (-1, -1)),
    ("font list", 3, (0, 0)),
]

FAILED = 0
RUNS = 0


def runtest(toolpath):
    global FAILED
    global RUNS
    RUNS += 1
    cleanup()
    for test in TESTS:
        output = runcommand(test[0], test[1], toolpath)
        if output != test[2]:
            print("Failed at " + str(test) + " Output: " + str(output))
            FAILED += 1


for path, name, files in os.walk("../bin"):
    if "ShaderTool.exe" in files:
        toolpath = pt.abspath(pt.join(path,
                                      "ShaderTool.exe")).replace("\\", "/")
        runtest(toolpath)
cleanup()

if RUNS == 0:
    raise Exception("No executable found")

if FAILED != 0:
    raise Exception(
        str(FAILED) + " out of " + str(len(TESTS) * RUNS) + " tests failed!")
else:
    print("Everything passed!")