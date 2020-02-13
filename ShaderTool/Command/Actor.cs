using Newtonsoft.Json;
using ShaderTool.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {

    class ActorData {
        public string name;
        public float[][] localTransform;
        public uint[] indices;
        public float[] vertices;
        public string materialName; // will be used to iterate over the materials list and then assign a byte index when making
        public uint indexCount;
        public uint vertexCount;
        public byte layerId;
    }

    class Actor {

        public static int ActorCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "add":
                    return ActorAdd(GetParams(args)); // param 1: name, param 2: material
                case "rm":
                case "remove":
                    return ActorRm(GetParams(args));
                case "list":
                    return ActorList();
            }

            Console.WriteLine("Wrong parameters! Must be add/rm/make/list!");
            return WRONG_PARAMS;
        }

        public static int ActorAdd(string[] args) {
            AsssertValues(args, 2); // has to be two params, one for name and one for material

            if (!Directory.Exists(Program.ResourcesFolder)) {
                Directory.CreateDirectory(Program.ResourcesFolder);
            }

            string actorName = args[0];
            string materialName = args[1];

            if (!AssertName(actorName)) {
                return WRONG_PARAMS;
            }

            string path = Program.ResourcesFolder + @"\" + actorName + @"_Actor.json";

            Material.Load();

            if (!Cache.MATERIALS.ContainsKey(materialName)) {
                Console.WriteLine("Material {0} was not found", materialName);
                return WRONG_PARAMS;
            }

            if (!File.Exists(path)) {
                File.Create(path).Close();
            }

            ActorData newActor = new ActorData {
                name = actorName,
                materialName = materialName
            };

            File.WriteAllText(path, JsonConvert.SerializeObject(newActor, Formatting.Indented));
            Console.WriteLine("Added new actor {0}", actorName);
            return SUCCESS;
        }

        public static int ActorRm(string[] args) {
            AsssertValues(args, 1);

            string actorName = args[0];
            string filePath = Program.ResourcesFolder + @"\" + actorName + "_Actor.json";

            if (!File.Exists(filePath)) {
                Console.WriteLine("Actor {0} was not found", actorName);
                return WRONG_PARAMS;
            } else {
                File.Delete(filePath);
                Console.WriteLine("Actor {0} was deleted", actorName);
                return SUCCESS;
            }
        }

        public static int ActorList() {

            string[] fileList = Directory.GetFiles(Program.ResourcesFolder);
            List<string> filteredList = new List<string>();

            if (fileList.Length == 0) {
                Console.WriteLine("No actors added yet.");
            } else {
                Regex regexFileName = new Regex(@"\\(\w+)_Actor\.json");
                
                foreach (string filePath in fileList) {
                    if (regexFileName.IsMatch(filePath)) {
                        string actorName = regexFileName.Match(filePath).Groups[1].Value;
                        filteredList.Add(actorName);
                    }
                }

                if (filteredList.Count == 0) {
                    Console.WriteLine("No actors added yet.");
                } else {
                    Console.WriteLine("Count: {0}", filteredList.Count);
                    filteredList.ForEach(name => Console.WriteLine(" - " + name));
                }
                
            }

            return SUCCESS;
        }
    }

}
