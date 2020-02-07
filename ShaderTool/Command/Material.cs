using Newtonsoft.Json;
using ShaderTool.Util;
using System;
using System.Collections.Generic;
using System.IO;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Material {
        public static string MaterialPath = Program.ResourcesFolder + @"\Materials.json";

        public static int MaterialCommand(string[] args) {
            AsssertNoneNull(args);
            Load(); // load the file once for efficiency, use the save command to save all changes
            switch (args[0]) {
                case "save":
                    return MaterialSave();
                case "add":
                    return MaterialAdd(GetParams(args));
                case "rm":
                case "remove":
                    return MaterialRm(GetParams(args));
                case "make":
                    return MaterialMake();
                case "list":
                    return MaterialList();
            }

            Console.WriteLine("Wrong parameters! Must be save/add/rm/make/list!");
            return WRONG_PARAMS;

        }

        public static void Load() {

            if (!Directory.Exists(Program.ResourcesFolder)) {
                Directory.CreateDirectory(Program.ResourcesFolder);
            }

            if (!File.Exists(MaterialPath)) {
                File.Create(MaterialPath).Close();
                Cache.MATERIALS = new Dictionary<string, MaterialData>();
                return;
            } else if (Cache.MATERIALS == null) {
                string fileContent = File.ReadAllText(MaterialPath);
                if (fileContent == "" || fileContent == "{ }") {
                    Cache.MATERIALS = new Dictionary<string, MaterialData>();
                    return;
                } else {
                    Dictionary<string, MaterialData> existingDict = JsonConvert.DeserializeObject<Dictionary<string, MaterialData>>(fileContent);
                    Cache.MATERIALS = existingDict;
                    return;
                }

            }

        }

        public static int MaterialSave() {
            string json = JsonConvert.SerializeObject(Cache.MATERIALS, Formatting.Indented);
            Console.WriteLine(json);
            File.WriteAllText(MaterialPath, json);
            Console.WriteLine("Saved materials!");
            return SUCESS;
        }

        public static int MaterialAdd(string[] args) {

            AsssertValues(args, 1);

            if (Cache.MATERIALS.Count > 255) { // max number of materials is 256 as the index is provided as a byte
                Console.WriteLine("Maximum limit of 256 textures reached, cannot add more");
                return OVERFLOW;
            }

            string name = args[0];

            if (!AssertName(name)) {
                return WRONG_PARAMS;
            }

            if (Cache.MATERIALS.ContainsKey(name)) {
                Console.WriteLine("Material {0} already exists!", name);
                return ALREADY_EXIST;
            }

            MaterialData newMaterial = new MaterialData();
            Cache.MATERIALS.Add(name, newMaterial);
            Console.WriteLine("Added new material {0}, don't forget to use \"material save\"", name);
            return SUCESS;

        }

        public static int MaterialRm(string[] args) {

            AsssertValues(args, 1);
            if (!Cache.MATERIALS.Remove(args[0])) { // name could not be found
                Console.WriteLine("Material was not found");
                return WRONG_PARAMS;
            }

            Console.WriteLine("Material {0} was successfully removed", args[0]);
            return SUCESS;

        }

        public static int MaterialList() {

            Console.WriteLine("Count: " + Cache.MATERIALS.Count + "/256");
            if (Cache.MATERIALS.Count == 0) {
                Console.WriteLine("No materials added, use \"material add <name>\" to add a new material");
            } else {
                foreach (KeyValuePair<string, MaterialData> material in Cache.MATERIALS) {
                    Console.WriteLine(" - " + material.Key);
                }
            }

            return SUCESS;
        }

        public static int MaterialMake() {

            // TODO

            Console.WriteLine("Not implemented yet.");
            return 0;
        }

    }

    class MaterialData {
        public float[] color;
        public uint diffuseTexture;
    }
}
