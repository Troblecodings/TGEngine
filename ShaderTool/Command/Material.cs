using System;
using System.Collections.Generic;
using System.Text;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Material {

        public static int MaterialCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "add":
                    return MaterialAdd(GetParams(args));
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

        public static int MaterialAdd(string[] args) {
            AsssertNoneNull(args);
            return 0;
        }

    }

    class MaterialData {
        float[] color;
        uint diffuseTexture;
    }
}
