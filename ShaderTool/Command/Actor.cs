using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Actor {

        public static int ActorCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "add":
                    return ActorAdd(GetParams(args)); // param 1: name, param 2: material
                case "rm":
                    return 0;
                case "make":
                    return 0;
                case "list":
                    return 0;
                case "show":
                    return 0;
            }

            Console.WriteLine("Wrong parameters! Must be add/rm/make/list/show!");
            return WRONG_PARAMS;
        }

        public static int ActorAdd(string[] args) {
            AsssertValues(args, 2); // has to be two params, one for name and one for material

            string actorName = args[0];
            string materialName = args[1];
            string path = Program.CWD + "\\" + actorName + "Actor.json";

            Material.Load();
            bool success = Material.MATERIALS.TryGetValue(materialName, out MaterialData materialData);

            if (!success)
                return WRONG_PARAMS; // material was not found

            if (!File.Exists(path))
                File.Create(path).Close();

            ActorData newActor = new ActorData {
                name = actorName,
                material = materialData.id
            };

            JsonConvert.SerializeObject(newActor);

            File.WriteAllText(path, JsonConvert.SerializeObject(newActor, Formatting.Indented));
            return SUCESS;
        }

        // TODO actorrm
        // TODO actormake
        // TODO actorlist
        // TODO actorshow

    }

    class ActorData {
        public string name;
        public float[][] localTransform;
        public uint[] indices;
        public float[] vertices;
        public byte material;
        public uint indexCount;
        public ushort vertexCount;
    }
}
