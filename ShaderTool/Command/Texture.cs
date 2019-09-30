using System;
using System.Collections.Generic;
using System.Text;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;
using Newtonsoft.Json;
using System.IO;
using ShaderTool.Util;

namespace ShaderTool.Command
{
    class Texture
    {

        public static int TextureCommand(string[] args)
        {
            AsssertNoneNull(args);

            switch (args[0])
            {
                case "add":
                    return Add(GetParams(args));
                case "rm":
                    return Rm(GetParams(args));
                case "import":
                    return Import(GetParams(args));
                case "list":
                    return List();
            }

            Console.WriteLine("Wrong params! Possible: add/rm/list");
            return WRONG_PARAMS;
        }

        private static void Load()
        {
            string path = Program.CWD + "/Resources.json";
            Console.WriteLine(path);
            if (Cache.PRELOAD == null) {
                if (File.Exists(path))
                    Cache.PRELOAD = JsonConvert.DeserializeObject<Resource>(File.ReadAllText(path));
                else
                    Cache.PRELOAD = new Resource();
            }
        }

        private static void Flush()
        {
            File.WriteAllText(Program.CWD + "/Resources.json", JsonConvert.SerializeObject(Cache.PRELOAD, Formatting.Indented));
        }

        public static int Import(string[] args)
        {
            AsssertValues(args, 1);
            if (!File.Exists(args[0]))
            {
                Console.WriteLine("File " + args[0] + " could not be found!");
                return WRONG_PARAMS;
            }
            string[] exnamesplit = args[0].Replace("\\", "/").Split("/");
            string filename = exnamesplit[exnamesplit.Length - 1].Split(".")[0];
            int rtc = Add(new string[] { filename });
            if (rtc != 0)
                return rtc;
            byte[] tex = File.ReadAllBytes(args[0]);
            FileStream stream = File.Open(Program.CWD + "\\Resources.tgr", FileMode.Append);
            TextureDesc texture;
            texture.offset = stream.Position;
            texture.size = tex.Length;
            stream.Write(tex, 0, tex.Length);
            Cache.PRELOAD.texturedesc.Add(texture);
            return WRONG_PARAMS;
        }

        public static int Add(string[] args)
        {
            AsssertValues(args, 1);
            Load();
            if (Cache.PRELOAD.texturenames.Contains(args[0]))
                return WRONG_PARAMS;
            Cache.PRELOAD.texturenames.Add(args[0]);
            Flush();
            return 0;
        }

        public static int Rm(string[] args)
        {
            AsssertValues(args, 1);
            Load();
            if (!Cache.PRELOAD.texturenames.Remove(args[0]))
                return WRONG_PARAMS;
            Flush();
            return 0;
        }

        public static int List()
        {
            Load();
            Cache.PRELOAD.texturenames.ForEach(Console.WriteLine);
            return 0;
        }
    }
}
