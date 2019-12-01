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

            string name = args[0];
            string path = Program.CWD + "\\" + name + "Actor.json";

            if (!File.Exists(path))
                File.Create(path).Close();
            
            // TODO

            return SUCESS;
        }

        // TODO actorrm
        // TODO actormake
        // TODO actorlist
        // TODO actorshow

    }

    class ActorData {
        string name;
        float[][] localTransform;
        uint[] indices;
        float[] vertices;
        byte material;
        uint indexCount;
        ushort vertexCount;
    }
}
