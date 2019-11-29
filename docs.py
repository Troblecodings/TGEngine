import os
import os.path
import re

docsfld = "docs/docs/"
preset = None

with open(docsfld + "std.html") as std:
    preset = std.read()

if preset == None:
    print("Couldn't read preset")
    exit(-1)

def lineTest(line):
    spl = line.split("(")[0]
    return '(' in line and ')' in line and ';' in line and re.match(r"^[^=#]+$", spl) and re.search(r"\s", spl)

def scanFile(file, name):
    tmp = ""
    name = name.replace(".hpp", "") + ".html"
    with open(file) as fp:
        for line in fp.readlines():
            if lineTest(line):
                tmp += " <details><summary>" + line + "</summary></details>"
    if tmp == "":
        return;
    print("<a href='" + name + "'>" + name.replace(".html", "") + "</a>")
    with open(docsfld + name, "w") as fp:
        fp.write(preset.replace("%DOCCONT%", tmp))
        


def searchTree(pth):
    for file in os.listdir(pth):
        pt = pth + "/" + file
        if os.path.isdir(pt):
            searchTree(pt)
            continue
        if file.endswith(".hpp"):
            scanFile(pt, file)


searchTree("TGEngine")
    
