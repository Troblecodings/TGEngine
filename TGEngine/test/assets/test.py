import os
import json

d = {}

for x in os.listdir("./"):
    if x.endswith(".json"):
        with open(x) as f:
            d = json.load(f)
            lst = d["codes"]
            d["codes"] = [{"code": lst}]
        with open(x.replace(".vert", ".json"), "w") as fo:
            json.dump(d, fo, indent=1)
