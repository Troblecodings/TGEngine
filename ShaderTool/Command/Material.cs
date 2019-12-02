using Newtonsoft.Json;
using ShaderTool.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Material {

        public static Dictionary<string, MaterialData> MATERIALS;
        public static string actorPath = Program.CWD + "\\Materials.json";

        public static int MaterialCommand(string[] args) {
            AsssertNoneNull(args);
            Load(); // load the file once for efficiency, use the save command to save all changes
            switch (args[0]) {
                case "save":
                    return MaterialSave();
                case "add":
                    return MaterialAdd(GetParams(args));
                case "rm":
                    return MaterialRm(GetParams(args));
                case "make":
                    return MaterialMake();
                case "list":
                    return MaterialList();
                case "show":
                    return MaterialShow();
            }

            Console.WriteLine("Wrong parameters! Must be save/add/rm/make/list/show!");
            return WRONG_PARAMS;

        }

        public static void Load() {

            if (!File.Exists(actorPath)) {
                File.Create(actorPath).Close();
            }

            string fileContent = File.ReadAllText(actorPath);
            Dictionary<string, MaterialData> existingDict = JsonConvert.DeserializeObject<Dictionary<string, MaterialData>>(fileContent);

            if (existingDict == null)
                MATERIALS = new Dictionary<string, MaterialData>();
            else
                MATERIALS = existingDict;

        }

        public static int MaterialSave() {
            string json = JsonConvert.SerializeObject(MATERIALS, Formatting.Indented);
            Console.WriteLine(json);
            File.WriteAllText(actorPath, json);
            return SUCESS;

        }

        public static int MaterialAdd(string[] args) {

            AsssertValues(args, 1);
            if (MATERIALS.Count > 255) { // max number of materials is 256 as the index is provided as a byte
                Console.WriteLine("Maximum limit of 256 textures reached, cannot add more");
                return OVERFLOW;
            }

            string name = args[0];

            if (MATERIALS.ContainsKey(name))
                return ALREADY_EXIST;

            MaterialData newMaterial = new MaterialData {
                id = (byte)MATERIALS.Count
            };

            MATERIALS.Add(name, newMaterial);

            return SUCESS;

        }

        public static int MaterialRm(string[] args) {

            AsssertValues(args, 1);
            if (!MATERIALS.Remove(args[0])) // name could not be found
                return WRONG_PARAMS;

            return SUCESS;

        }

        public static int MaterialList() {

            Console.WriteLine("Count: " + MATERIALS.Count + "/256");
            var enumerator = MATERIALS.GetEnumerator();
            while (enumerator.MoveNext()) {
                Console.WriteLine(enumerator.Current);
            }
            return SUCESS;

        }

        public static int MaterialMake() {

            // TODO

            return 0;
        }

        public static int MaterialShow() {

            // TODO

            return 0;
        }

    }

    class MaterialData {
        public byte id;
        public float[] color;
        public uint diffuseTexture;
    }
}
