using System;
using System.Collections.Generic;
using System.Text;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Actor {

        public static int ActorCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "add":
                    return ActorAdd(GetParams(args));
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
            AsssertNoneNull(args);
            return 0;
        }
    }

    class ActorData {
        float[][] localTransform;
        uint[] indices;
        float[] vertices;
        byte material;
        uint indexCount;
        ushort vertexCount;
    }
}
