using System;
using System.IO;
using System.Collections.Generic;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command
{
    struct MapData
    {
        string[] actorNames;
    }

    class Maps
    {
        public static Dictionary<string, MapData> mapData;
        public const uint TGR_VERSION = 1;

        public static int MapMake(string[] args)
        {
            AsssertNoneNull(args);

            string name = args[0];
            if (!mapData.ContainsKey(name)) {
                Console.WriteLine("{0} is not a map.", name);
                return Error.WRONG_PARAMS;
            }

            string resourceFile = Program.CWD + "\\" + name + ".tgr";

            File.Delete(resourceFile);

            Stream resourceStream = File.OpenWrite(resourceFile);
            resourceStream.Write(BitConverter.GetBytes(TGR_VERSION));

            return Error.SUCCESS;
        }

    }
}
