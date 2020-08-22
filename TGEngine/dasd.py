import os

for fldr in ("public", "private"):
    for root, dirs, files in os.walk(fldr):
        for file in files:
            if file.endswith("cpp") or file.endswith("hpp"):
                path = os.path.join(root, file).replace("\\", "/")
                lst = []
                with open(path) as fp:
                    for line in fp.readlines():
                        if "#include" in line and not ("private" in line or "public" in line or "<" in line):
                            substitute = "#include \""
                            pth = line.split("\"")[1]
                            for x in range(path.count("/")):
                                substitute += "../"
                            substitute += os.path.normpath(root.replace("private", "public") + "/" + pth).replace("\\", "/") + "\"\n"
                            print(substitute)
                            lst.append(substitute)
                        else:
                            lst.append(line)
                with open(path, "w") as fp:
                    for ln in lst:
                        fp.write(ln)
