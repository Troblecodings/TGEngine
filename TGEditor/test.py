import os
import json
import subprocess

cwd = os.getcwd() + "\\resource\\glTF-Sample-Models"

result = open(cwd + "\\result.txt", "w")

succ = 0
fail = 0
cats = {}
catsf = {}

with open(cwd + "\\2.0\\model-index.json", "r") as index:
    models = json.load(index)
    for obj in models:
        result.write("================================\n")
        result.write(obj["name"] + "\n")
        nofail = True
        for vari in obj["variants"]:
            if vari == "glTF-Draco":
                print("Skipping draco")
                continue
            filename = obj["variants"][vari]
            pth = cwd + "\\2.0\\" + obj["name"] + "\\" + vari + "\\" + filename
            exe = cwd + "/../../run/x64/Release/TGEditor.exe"
            p1 = subprocess.Popen([exe, pth])
            ex = p1.wait()
            if ex == 0:
                succ += 1
                result.write(vari + ": Success\n")
                if vari in cats:
                    cats[vari] += 1
                else:
                    cats[vari] = 1
            else:
                fail += 1
                result.write(vari + ": Failed " + str(ex) + "\n")
                nofail = False
                if vari in catsf:
                    catsf[vari] += 1
                else:
                    if vari not in cats:
                        cats [vari] = 0
                    catsf[vari] = 1

        if nofail:
            result.write("\nSuccess\n")
        else:
            result.write("\nFailed\n")
        result.flush()

result.write("\n\n--------------------------\n")
result.write("Success: " + str(succ) + " (" + str(round(succ / (succ + fail) * 100)) + ")\n")
result.write("Fail: " + str(fail) + " (" + str(round(fail / (succ + fail) * 100)) + ")\n\n")
for x in cats:
    if cats[x] == 0:
        result.write(x + " Failed all " + str(catsf[x]) + "\n")
        continue
    if x in catsf:
        result.write(x + ": Success " + str(cats[x]) + " (" + str(round(cats[x] / (cats[x] + catsf[x]) * 100)) + "%)\n")
    else:
        result.write(x + ": Succeded all " + str(cats[x]) + "\n")

result.close()
