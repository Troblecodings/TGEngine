using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Map {
        public static int MapCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "add":
                    return MapAdd(GetParams(args));
                case "rm":
                    return 0;
                case "make":
                    return 0;
                case "list":
                    return 0;
                case "show":
                    return 0;
                case "addactor":
                    return 0;
            }

            Console.WriteLine("Wrong parameters! Must be add/rm/make/list/show/addactor!");
            return WRONG_PARAMS;
        }

        public static int MapAdd(string[] args) {
            //AsssertNoneNull(args);

            //string Name = args[0];
            //string Path = Program.CWD + "\\" + Name + "Map.json";

            //if (!File.Exists(Path)) {
            //    File.Create(Path).Close();
            //}

            //string FileContent = File.ReadAllText(Path);
            //MapData Map = JsonConvert.DeserializeObject<MapData>(FileContent);

            //MapData NewMap = new MapData();
            //NewMap.name = Name;
            //NewMap.actors = GetActors(Path);

            // TODO

            return SUCESS;
        }

        public static ActorData[] GetActors(string Path) {
            // TODO
            return new ActorData[0];
        }

        public static MaterialData[] GetMaterials(string[] Paths) {
            // TODO
            return new MaterialData[0];
        }

        public static TextureData[] GetTextures(string[] Paths) {
            // TODO
            return new TextureData[0];
        }

        // TODO mapaddactor

    }

    class MapData {
        public string name;
        public ActorData[] actors;
        public MaterialData[] materials;
        public TextureData[] textures;
    }

    class TextureData {
        byte[] data;
        int x;
        int y;
        int comp;
    }
}
