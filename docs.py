import os
import os.path
import re

docsfld = "docs/docs/"
preset = None

with open(docsfld + "std.html") as std:
    preset = std.read()

def lineTest(line):
    spl = line.split("(")[0]
    return '(' in line and ')' in line and re.match(r"^[^=#]+$", spl) and re.search(r"\s", spl)
    

splitcommendreg = re.compile("^\/?\*")

def scanFile(file, name):
    tmp = ""
    tmpstr = ""
    name = name.replace(".hpp", "") + ".html"
    with open(file) as fp:
        rgl = fp.read()
        for line in rgl.split(";"):
            line = line.strip()
            tmpspl = re.split("[\n\r]", line)
            detail = ""
            for item in tmpspl:
                item = item.strip()
                if splitcommendreg.match(item):
                    detail += splitcommendreg.sub("", item) + "\n"
            detail = detail.strip().replace("\n", "<br>")
            if detail.endswith("/"):
                detail = detail[0:len(detail)-1]
            
            if lineTest(line):
                tmp += "<details><summary><pre class='prettyprint'>" + tmpspl[len(tmpspl) - 1].strip() + "</pre></summary>" + detail + "</details>"
            if "struct " in line:
                tst = line.split("struct ")
                if len(tst) == 1:
                    tst = tst[0]
                else:
                    tst = tst[1]
                structname = tst.split("{")[0].strip()
                splitstr = re.split(r"struct\s+" + structname + "\s*{", rgl)
                if len(splitstr) == 1:
                    splitstr = splitstr[0]
                else:
                    splitstr = splitstr[1]
                splitstr = splitstr.split("}")[0]
                atrs = ""
                for atr in splitstr.split("\n"):
                    if re.match(r"[()#{}]|extern|namespace", atr) == None:
                        atrs += atr.strip() + "<br>"
                tmpstr += "<details><summary>" + structname + "</summary>" + detail + "\n<br><br><pre class='prettyprint'>" + atrs + "</pre></details>"
    if tmp == "" and tmpstr == "":
        return;
    print("<a href='" + name + "'>" + name.replace(".html", "") + "</a>")
    with open(docsfld + name, "w") as fp:
        fp.write(preset.replace("%DOCCONT%", tmp + "<h3>Structs</h3>\n" + tmpstr))
        


def searchTree(pth):
    for file in os.listdir(pth):
        pt = pth + "/" + file
        if os.path.isdir(pt):
            searchTree(pt)
            continue
        if file.endswith(".hpp"):
            scanFile(pt, file)


searchTree("TGEngine")
    
