using Newtonsoft.Json;
using ShaderTool.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {

    class MaterialData {
        public float[] color;
        public string diffuseTexture;
    }

    class Material {

        public static string MaterialPath = Path.Combine(Program.ResourcesFolder, @"Materials.json");
        public const uint MATERIAL_SIZE = 20; // Color is 4 * 4 bytes, diffuse texture ID is 4 bytes which equals 20 bytes

        public static int MaterialCommand(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            Load();

            switch (args[0]) {
                case "add":
                    return MaterialAdd(GetParams(args));
                case "rm":
                case "remove":
                    return MaterialRm(GetParams(args));
                case "settexture":
                case "settex":
                    return MaterialSetTexture(GetParams(args));
                case "setcolor":
                    return MaterialSetColor(GetParams(args));
                case "list":
                    return MaterialList();
            }

            Console.WriteLine("Wrong parameters! Must be save/add/rm/list/settexture/setcolor!");
            return WRONG_PARAMS;
        }

        private static int MaterialSetColor(string[] args) {
            Load();

            string name = args[0];
            if (!Cache.MATERIALS.ContainsKey(name)) {
                Console.WriteLine("{0} does not exist!", name);
                return WRONG_PARAMS;
            }

            uint output;
            if (uint.TryParse(args[1], System.Globalization.NumberStyles.HexNumber, null, out output)) {
                float r = ((output & 0xFF000000) >> 24) / 255.0f;
                float g = ((output & 0x00FF0000) >> 16) / 255.0f;
                float b = ((output & 0x0000FF00) >> 8) / 255.0f;
                float a = ((output & 0x000000FF) / 255.0f);
                Cache.MATERIALS[name].color = new float[] { r, g, b, a };
                Save();
                return SUCCESS;
            }

            Console.WriteLine("{0} is not a hex number (needs to be without 0x)!", args[1]);
            return WRONG_PARAMS;
        }

        public static int MaterialSetTexture(string[] args) {
            AssertValues(args, 2);
            
            Load();

            string name = args[0];
            if(!Cache.MATERIALS.ContainsKey(name)) {
                Console.WriteLine("{0} does not exist!", name);
                return WRONG_PARAMS;
            }

            MaterialData material = Cache.MATERIALS[name];

            if (Texture.GetExistingTextureNames().Contains(args[1])) {
                material.diffuseTexture = args[1];
                Cache.MATERIALS[name] = material;
                Save();
                return SUCCESS;
            }

            Console.WriteLine("Texture {0} does not exist, skipping", args[1]);
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

        public static void Save() {
            string json = JsonConvert.SerializeObject(Cache.MATERIALS, Formatting.Indented);
            File.WriteAllText(MaterialPath, json);
            Console.WriteLine("Saved materials!");
        }

        public static int MaterialAdd(string[] args) {
            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            if (Cache.MATERIALS.Count > 255) { // max number of materials is 256 as the index is provided as a byte
                Console.WriteLine("Maximum limit of 256 textures reached, cannot add more");
                return OVERFLOW;
            }

            string name = args[0];

            if (!AssertName(name))
                return WRONG_PARAMS;

            if (Cache.MATERIALS.ContainsKey(name)) {
                Console.WriteLine("Material {0} already exists!", name);
                return ALREADY_EXIST;
            }

            MaterialData newMaterial = new MaterialData();

            if (args.Length > 1 && Texture.GetExistingTextureNames().Contains(args[1]))
                newMaterial.diffuseTexture = args[1];

            if (args.Length == 3) {
                uint output;
                if (!uint.TryParse(args[2], System.Globalization.NumberStyles.HexNumber, null, out output)) {
                    Console.WriteLine("{0} is not a hex number!", args[2]);
                } else {
                    float r = ((output & 0xFF000000u) >> 24) / 255.0f;
                    float g = ((output & 0x00FF0000u) >> 16) / 255.0f;
                    float b = ((output & 0x0000FF00u) >> 8) / 255.0f;
                    float a = ((output & 0x000000FFu) / 255.0f);
                    newMaterial.color = new float[] { r, g, b, a };
                }
            }

            Cache.MATERIALS.Add(name, newMaterial);
            Console.WriteLine("Added new material '{0}'!", name);

            Save();
            return SUCCESS;
        }

        public static int MaterialRm(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            if (!Cache.MATERIALS.Remove(args[0])) { // name could not be found
                Console.WriteLine("Material was not found");
                return WRONG_PARAMS;
            }

            Save();
            Console.WriteLine("Material {0} was successfully removed", args[0]);
            return SUCCESS;

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

            return SUCCESS;
        }

    }

}
