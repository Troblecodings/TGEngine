using Newtonsoft.Json;
using ShaderTool.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {

    class ActorData {
        public string  name;
        public float[] localTransform;
        public uint[]  indices;
        public float[] vertices;
        public string  materialName; // will be used to iterate over the materials list and then assign a byte index when making
        public uint    indexCount;
        public uint    vertexCount;
        public byte    layerId;
    }

    class Actor {

        public static float[] IDENTITY_MATRIX = {
            1f, 0f, 0f, 0f,
            0f, 1f, 0f, 0f,
            0f, 0f, 1f, 0f,
            0f, 0f, 0f, 1f,
        };

        public static int ActorCommand(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            switch (args[0]) {
                case "add":
                    return ActorAdd(GetParams(args)); // param 1: name, param 2: material
                case "rm":
                case "remove":
                    return ActorRm(GetParams(args));
                case "list":
                    return ActorList();
                case "vertex":
                    return ActorVertex(GetParams(args));
                case "index":
                    return ActorIndex(GetParams(args));
                case "transform":
                    return ActorTransform(GetParams(args));
                case "layer":
                    return ActorLayer(GetParams(args));
            }

            Console.WriteLine("Wrong parameters! Must be add/rm/make/list!");
            return WRONG_PARAMS;
        }

        private static int Update(string[] args, int size, Func<ActorData, ActorData> func) {
            if (!AssertValues(args, size))
                return NOT_ENOUGH_PARAMS;

            string actorName = args[0];
            string filePath = Path.Combine(Program.ResourcesFolder, actorName + @"_Actor.json");

            if (!File.Exists(filePath)) {
                Console.WriteLine("Actor {0} was not found", actorName);
                return WRONG_PARAMS;
            }

            ActorData data = JsonConvert.DeserializeObject<ActorData>(File.ReadAllText(filePath));

            try {
                data = func(data);
            } catch {
                Console.WriteLine("There was an internal error, please check your input!");
                return WRONG_PARAMS;
            }

            File.WriteAllText(filePath, JsonConvert.SerializeObject(data, Formatting.Indented));

            return SUCCESS;
        }

        public static int ActorTransform(string[] args) {
            return Update(args, 17, act => {
                act.localTransform = new float[16];
                for (int i = 1; i < args.Length; i++)
                    act.localTransform[i - 1] = float.Parse(args[i]);
                return act;
            });
        }

        public static int ActorVertex(string[] args) {
            return Update(args, 3, act => {
                act.vertexCount = uint.Parse(args[1]);
                act.vertices = new float[args.Length - 2];
                for (int i = 2; i < args.Length; i++)
                    act.vertices[i - 2] = float.Parse(args[i]);
                return act;
            });
        }

        public static int ActorIndex(string[] args) {
            return Update(args, 2, act => {
                act.indexCount = (uint)args.Length - 1;
                act.indices = new uint[act.indexCount];
                for (int i = 1; i < args.Length; i++)
                    act.indices[i - 1] = uint.Parse(args[i]);
                return act;
            });
        }

        public static int ActorLayer(string[] args) {
            return Update(args, 2, act => {
                act.layerId = byte.Parse(args[1]);
                return act;
            });
        }

        public static int ActorAdd(string[] args) {

            // has to be two params, one for name and one for material
            if (!AssertValues(args, 2))
                return NOT_ENOUGH_PARAMS;

            if (!Directory.Exists(Program.ResourcesFolder))
                Directory.CreateDirectory(Program.ResourcesFolder);

            string actorName = args[0];
            string materialName = args[1];

            if (!AssertName(actorName))
                return WRONG_PARAMS;

            string path = Path.Combine(Program.ResourcesFolder, actorName + @"_Actor.json");

            Material.Load();

            if (!Cache.MATERIALS.ContainsKey(materialName)) {
                Console.WriteLine("Material {0} was not found", materialName);
                return WRONG_PARAMS;
            }

            if (!File.Exists(path))
                File.Create(path).Close();

            ActorData newActor = new ActorData();
            newActor.name = actorName;
            newActor.materialName = materialName;
            newActor.layerId = 0;
            newActor.localTransform = IDENTITY_MATRIX;
            newActor.indexCount = 0;
            newActor.vertexCount = 0;
            newActor.vertices = new float[] { };
            newActor.indices = new uint[] { };

            File.WriteAllText(path, JsonConvert.SerializeObject(newActor, Formatting.Indented));
            Console.WriteLine("Added new actor {0}", actorName);
            return SUCCESS;
        }

        public static int ActorRm(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            string actorName = args[0];
            string filePath = Path.Combine(Program.ResourcesFolder, actorName + @"_Actor.json");

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
