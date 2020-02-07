using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;
using Newtonsoft.Json;

namespace ShaderTool.Command {

    class MapData {
        public string[] actorNames;
        public string[] materialNames;
        public string[] textures;
    }

    class Map {

        public const uint TGR_VERSION = 1;

        public static int MapCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "add":
                    return MapAdd(GetParams(args));
                case "rm":
                case "remove":
                    return MapRm(GetParams(args));
                case "make":
                    return MapMake(GetParams(args));
                case "list":
                    return MapList();
                case "addactor":
                    return MapAddActor(GetParams(args));
                case "rmactor":
                case "removeactor":
                    return MapRmActor(GetParams(args));
            }

            Console.WriteLine("Wrong parameters! Must be save/add/rm/make/list!");
            return WRONG_PARAMS;

        }

        public static MapData Load(string mapName) {
            string resourceFilePath = Program.ResourcesFolder + "\\" + mapName + ".json";

            if (!File.Exists(resourceFilePath))
                return null;

            string fileContent = File.ReadAllText(resourceFilePath);
            MapData map = JsonConvert.DeserializeObject<MapData>(fileContent);

            return map;
        }

        public static void Save(string mapName, MapData map) {
            string resourceFilePath = Program.ResourcesFolder + "\\" + mapName + ".json";
            File.WriteAllText(resourceFilePath, JsonConvert.SerializeObject(map, Formatting.Indented));
        }

        public static void UpdateMaterials(MapData map) {

            List<string> newMaterialNames = new List<string>();

            foreach (string actorName in map.actorNames) {

                string actorPath = Program.ResourcesFolder + @"\" + actorName + @"_Actor.json";
                string actorFileContent = File.ReadAllText(actorPath);
                ActorData actor = JsonConvert.DeserializeObject<ActorData>(actorFileContent);
                string materialName = actor.materialName;

                if (!newMaterialNames.Contains(materialName) && materialName != null) {
                    newMaterialNames.Add(materialName);
                }

            }

            map.materialNames = newMaterialNames.ToArray();
            Console.WriteLine("Updated material names");
        }

        public static void AddActors(MapData map, string[] actorsToAdd) {
            AsssertValues(actorsToAdd, 1);

            if (map.actorNames == null)
                map.actorNames = new string[0];
            if (map.materialNames == null)
                map.materialNames = new string[0];

            // We don't know how many actors are valid so I'm using a list to compensate for that
            List<string> actorNames = map.actorNames.ToList();

            foreach (string actorName in actorsToAdd) {
                string actorPath = Program.ResourcesFolder + @"\" + actorName + @"_Actor.json";

                if (!File.Exists(actorPath)) {
                    Console.WriteLine("Actor {0} not found, skipping", actorName);
                    continue;
                }

                if (actorNames.Contains(actorName)) {
                    Console.WriteLine("Actor {0} already added, skipping", actorName);
                    continue;
                }

                actorNames.Add(actorName);
                Console.WriteLine("Added Actor {0}", actorName);

            }

            map.actorNames = actorNames.ToArray();
            UpdateMaterials(map);
        }

        public static int MapAdd(string[] args) {
            AsssertValues(args, 1);

            if (!Directory.Exists(Program.ResourcesFolder)) {
                Directory.CreateDirectory(Program.ResourcesFolder);
            }

            string mapName = args[0];
            string[] actors = GetParams(args);
            string mapFilePath = Program.ResourcesFolder + "\\" + mapName + ".json";

            if (!AssertName(mapName)) {
                return WRONG_PARAMS;
            }

            if (!File.Exists(mapFilePath)) {
                File.Create(mapFilePath).Close();
            }

            MapData newMap = new MapData();

            // GetParams() sets the value to null instead of leaving an empty array if the length is 0
            if (actors != null)
                AddActors(newMap, actors);

            Save(mapName, newMap);
            Console.WriteLine("Added new map {0}", mapName);
            return SUCCESS;
        }

        public static int MapRm(string[] args) {
            AsssertValues(args, 1);

            string mapName = args[0];
            string mapFilePath = Program.ResourcesFolder + "\\" + mapName + ".json";

            if (!File.Exists(mapFilePath)) {
                Console.WriteLine("Map {0} was not found", mapName);
                return WRONG_PARAMS;
            } else {
                File.Delete(mapFilePath);
                Console.WriteLine("Map {0} was deleted", mapName);
                return SUCCESS;
            }
        }

        public static int MapMake(string[] args) {
            AsssertNoneNull(args);

            string mapName = args[0];
            string resourceFilePath = Program.ResourcesFolder + "\\" + mapName + ".tgr";
            if (!File.Exists(resourceFilePath)) {
                Console.WriteLine("{0} is not a map.", mapName);
                return WRONG_PARAMS;
            }

            File.Delete(resourceFilePath);

            Stream resourceStream = File.OpenWrite(resourceFilePath);
            resourceStream.Write(BitConverter.GetBytes(TGR_VERSION));

            // TODO

            return SUCCESS;
        }

        public static int MapList() {
            string[] fileList = Directory.GetFiles(Program.ResourcesFolder);
            List<string> filteredList = new List<string>();

            if (fileList.Length == 0) {
                Console.WriteLine("No actors added yet.");
            } else {
                foreach (string filePath in fileList) {
                    if (Path.GetFileName(filePath).EndsWith(".json")
                        && !Path.GetFileName(filePath).EndsWith("_Actor.json")
                        && Path.GetFileName(filePath) != "Materials.json") {
                        filteredList.Add(Path.GetFileNameWithoutExtension(filePath));
                    }
                }

                if (filteredList.Count == 0) {
                    Console.WriteLine("No maps added yet.");
                } else {
                    Console.WriteLine("Count: {0}", filteredList.Count);
                    filteredList.ForEach(mapName => Console.WriteLine(" - " + mapName));
                }

            }

            return SUCCESS;
        }

        public static int MapAddActor(string[] args) {
            AsssertValues(args, 2);

            string mapName = args[0];
            string[] actorsToAdd = GetParams(args);

            MapData map = Load(mapName);

            if (map == null) {
                Console.WriteLine("Map {0} not found", mapName);
                return WRONG_PARAMS;
            }

            AddActors(map, actorsToAdd);
            Save(mapName, map);

            return SUCCESS;
        }

        public static int MapRmActor(string[] args) {
            AsssertValues(args, 2);

            string mapName = args[0];
            string[] actorsToRemove = GetParams(args);

            MapData map = Load(mapName);

            List<string> actorNames = map.actorNames.ToList();

            if (map == null) {
                Console.WriteLine("Map {0} not found", mapName);
                return WRONG_PARAMS;
            }

            bool changed = false;

            foreach (string actorName in actorsToRemove) {
                if (actorNames.Contains(actorName)) {
                    actorNames.Remove(actorName);
                    changed = true;
                    Console.WriteLine("Removed actor {0} from map {1}", actorName, mapName);
                } else {
                    Console.WriteLine("Map {0} does not contain actor {1}, skipping", mapName, actorName);
                }
            }

            if (changed) {
                map.actorNames = actorNames.ToArray();
                UpdateMaterials(map);
                Save(mapName, map);
            }

            return SUCCESS;
        }

    }
}
