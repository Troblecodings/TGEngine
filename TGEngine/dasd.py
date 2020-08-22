import os

for fldr in ("public", "private"):
    for root, dirs, files in os.walk(fldr):
        for file in files:
            if file.endswith("cpp") or file.endswith("hpp"):
                print("\"" + os.path.join(root, file).replace("\\", "/") + "\"")
