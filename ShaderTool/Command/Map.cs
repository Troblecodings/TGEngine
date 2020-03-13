using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ShaderTool.Util;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {

    class MapData {
        public string[] actorNames;
        public string[] materialNames;
        public string[] textureNames;
        public Dictionary<string, float> fontNames;
    }

    class Map {

        public const uint TGR_VERSION = 2;

        public static int MapCommand(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

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
                case "addfont":
                    return MapAddFont(GetParams(args));
                case "rmfont":
                case "removefont":
                    return MapRmFont(GetParams(args));
            }

            Console.WriteLine("Wrong parameters! Must be add/rm/make/list!");
            return WRONG_PARAMS;

        }

        private static int MapRmFont(string[] args) {
            if (!AssertValues(args, 2))
                return NOT_ENOUGH_PARAMS;

            string mapName = args[0];

            MapData map = Load(mapName);

            if (map == null) {
                Console.WriteLine("Map '{0}' not found", mapName);
                return WRONG_PARAMS;
            }

            for (int i = 1; i < args.Length; i++) {
                if (!map.fontNames.ContainsKey(args[i])) {
                    Console.WriteLine("Font {0} is not part of the map {1}, skipping!", args[i], mapName);
                    continue;
                }
                map.fontNames.Remove(args[i]);
            }

            Save(mapName, map);

            return SUCCESS;
        }

        private static int MapAddFont(string[] args) {
            if (!AssertValues(args, 3))
                return NOT_ENOUGH_PARAMS;

            string mapName = args[0];

            MapData map = Load(mapName);

            if (map == null) {
                Console.WriteLine("Map '{0}' not found", mapName);
                return WRONG_PARAMS;
            }

            if (map.fontNames == null)
                map.fontNames = new Dictionary<string, float>();

            for (int i = 1; i < (args.Length - 1) / 2 + 1; i++) {
                string path = Path.Combine(Program.ResourcesFolder, args[i * 2 - 1]);
                if (!File.Exists(path)) {
                    Console.WriteLine("Font '{0}' not found, skipping", path);
                    continue;
                }

                float fontSize = 0;
                if(!float.TryParse(args[i * 2], out fontSize)) {
                    Console.WriteLine("'{0}' is not a valid float, skipping!", args[i * 2]);
                    continue;
                }

                map.fontNames.Add(args[i * 2 - 1], fontSize);
            }

            Save(mapName, map);

            return SUCCESS;

        }

        public static MapData Load(string mapName) {
            string resourceFilePath = Path.Combine(Program.ResourcesFolder, mapName + ".json");

            if (!File.Exists(resourceFilePath))
                return null;

            string fileContent = File.ReadAllText(resourceFilePath);
            MapData map = JsonConvert.DeserializeObject<MapData>(fileContent);
            UpdateMaterials(map);

            return map;
        }

        public static void Save(string mapName, MapData map) {
            string resourceFilePath = Path.Combine(Program.ResourcesFolder, mapName + ".json");
            File.WriteAllText(resourceFilePath, JsonConvert.SerializeObject(map, Formatting.Indented));
        }

        public static void UpdateMaterials(MapData map) {

            List<string> newMaterialNames = new List<string>();
            List<string> newTextureNames = new List<string>();

            foreach (string actorName in map.actorNames) {

                string actorPath = Path.Combine(Program.ResourcesFolder, actorName + @"_Actor.json");
                string actorFileContent = File.ReadAllText(actorPath);
                ActorData actor = JsonConvert.DeserializeObject<ActorData>(actorFileContent);
                string materialName = actor.materialName;

                if (!newMaterialNames.Contains(materialName) && materialName != null)
                    newMaterialNames.Add(materialName);

                Material.Load();

                string diffuseTexture = Cache.MATERIALS[materialName].diffuseTexture;
                if (!newTextureNames.Contains(diffuseTexture) && diffuseTexture != null)
                    newTextureNames.Add(diffuseTexture);

            }

            map.materialNames = newMaterialNames.ToArray();
            map.textureNames = newTextureNames.ToArray();
        }

        public static void AddActors(MapData map, string[] actorsToAdd) {

            if (map.actorNames == null)
                map.actorNames = new string[0];
            if (map.materialNames == null)
                map.materialNames = new string[0];

            // We don't know how many actors are valid so I'm using a list to compensate for that
            List<string> actorNames = map.actorNames.ToList();

            foreach (string actorName in actorsToAdd) {
                string actorPath = Path.Combine(Program.ResourcesFolder, actorName + @"_Actor.json");

                if (!File.Exists(actorPath)) {
                    Console.WriteLine("Actor '{0}' not found, skipping", actorName);
                    continue;
                }

                if (actorNames.Contains(actorName)) {
                    Console.WriteLine("Actor '{0}' already added, skipping", actorName);
                    continue;
                }

                actorNames.Add(actorName);
                Console.WriteLine("Added Actor '{0}'", actorName);

            }

            map.actorNames = actorNames.ToArray();
            UpdateMaterials(map);
        }

        public static int MapAdd(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            if (!Directory.Exists(Program.ResourcesFolder))
                Directory.CreateDirectory(Program.ResourcesFolder);

            string mapName = args[0];
            string[] actors = GetParams(args);
            string mapFilePath = Path.Combine(Program.ResourcesFolder, mapName + ".json");

            if (!AssertName(mapName))
                return WRONG_PARAMS;

            if (!File.Exists(mapFilePath))
                File.Create(mapFilePath).Close();

            MapData newMap = new MapData();

            // GetParams() sets the value to null instead of leaving an empty array if the length is 0
            if (actors != null)
                AddActors(newMap, actors);

            Save(mapName, newMap);
            Console.WriteLine("Added new map '{0}'", mapName);
            return SUCCESS;
        }

        public static int MapRm(string[] args) {
            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            string mapName = args[0];
            string mapFilePath = Path.Combine(Program.ResourcesFolder, mapName + ".json");

            if (!File.Exists(mapFilePath)) {
                Console.WriteLine("Map '{0}' was not found", mapName);
                return WRONG_PARAMS;
            } else {
                File.Delete(mapFilePath);
                Console.WriteLine("Map '{0}' was deleted", mapName);
                return SUCCESS;
            }
        }

        public static int MapMake(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            string mapName = args[0];
            string mapFilePath = Path.Combine(Program.ResourcesFolder, mapName + ".json");
            string resourceFilePath = Path.Combine(Program.ResourcesFolder, mapName + ".tgr");

            // .tgr file format documentation:
            // https://troblecodings.com/fileformat.html

            if (!File.Exists(mapFilePath)) {
                Console.WriteLine("'{0}' is not a map.", mapName);
                return WRONG_PARAMS;
            }

            File.Delete(resourceFilePath);

            Stream resourceStream = File.OpenWrite(resourceFilePath);
            resourceStream.Write(BitConverter.GetBytes(TGR_VERSION));

            MapData mapData = Load(mapName);

            Material.Load();

            int status = SUCCESS;
            status = AddTexturesToResource(resourceStream, mapData);
            if (status != SUCCESS)
                return status;

            status = AddMaterialsToResource(resourceStream, mapData);
            if (status != SUCCESS)
                return status;

            status = AddActorsToResource(resourceStream, mapData);
            if (status != SUCCESS)
                return status;

            status = AddFontToResource(resourceStream, mapData);
            if (status != SUCCESS)
                return status;

            status = AddAnimationToResource(resourceStream, mapData);
            if (status != SUCCESS)
                return status;

            resourceStream.Close();

            return SUCCESS;
        }

        private static int AddFontToResource(Stream resourceStream, MapData mapData) {
            resourceStream.Write(BitConverter.GetBytes(mapData.fontNames.Count));

            foreach ((string font, float fontSize) in mapData.fontNames) {
                string path = Path.Combine(Program.ResourcesFolder, font);

                if(!File.Exists(path)) {
                    Console.WriteLine("Fontfile {0} not found, skipping!");
                    continue;
                }

                byte[] data = File.ReadAllBytes(path);
                resourceStream.Write(BitConverter.GetBytes(data.Length));
                resourceStream.Write(BitConverter.GetBytes(fontSize));
                resourceStream.Write(data);
            }

            resourceStream.Write(BitConverter.GetBytes(0xFFFFFFFF));
            return SUCCESS;
        }

        private static int AddAnimationToResource(Stream resourceStream, MapData mapData) {
            // Default skip
            resourceStream.Write(BitConverter.GetBytes(0));
            resourceStream.Write(BitConverter.GetBytes(0xFFFFFFFF));
            return SUCCESS;
        }

        public static int MapList() {
            string[] fileList = Directory.GetFiles(Program.ResourcesFolder);
            List<string> filteredList = new List<string>();

            if (fileList.Length == 0) {
                Console.WriteLine("No content added yet.");
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
            if (!AssertValues(args, 2))
                return NOT_ENOUGH_PARAMS;

            string mapName = args[0];
            string[] actorsToAdd = GetParams(args);

            MapData map = Load(mapName);

            if (map == null) {
                Console.WriteLine("Map '{0}' not found", mapName);
                return WRONG_PARAMS;
            }

            AddActors(map, actorsToAdd);
            Save(mapName, map);

            return SUCCESS;
        }

        public static int MapRmActor(string[] args) {

            if (!AssertValues(args, 2))
                return NOT_ENOUGH_PARAMS;

            string mapName = args[0];
            string[] actorsToRemove = GetParams(args);

            MapData map = Load(mapName);

            List<string> actorNames = map.actorNames.ToList();

            if (map == null) {
                Console.WriteLine("Map '{0}' not found", mapName);
                return WRONG_PARAMS;
            }

            bool changed = false;

            foreach (string actorName in actorsToRemove) {
                if (actorNames.Contains(actorName)) {
                    actorNames.Remove(actorName);
                    changed = true;
                    Console.WriteLine("Removed actor '{0}' from map '{1}'", actorName, mapName);
                } else {
                    Console.WriteLine("Map '{0}' does not contain actor '{1}', skipping", mapName, actorName);
                }
            }

            if (changed) {
                map.actorNames = actorNames.ToArray();
                UpdateMaterials(map);
                Save(mapName, map);
            }

            return SUCCESS;
        }

        private static int AddTexturesToResource(Stream resourceStream, MapData mapData) {

            string[] mapTextureNames = mapData.textureNames;

            if (mapTextureNames == null)
                return SUCCESS;

            resourceStream.Write(BitConverter.GetBytes(mapData.textureNames.Length));

            foreach (string mapTextureName in mapTextureNames) {

                string textureFilePath = Path.Combine(Program.ResourcesFolder, mapTextureName + ".tgx");

                if (!File.Exists(textureFilePath)) {
                    Console.WriteLine("'{0}' is not a texture.", mapTextureName);
                    return WRONG_PARAMS;
                }

                byte[] textureData = File.ReadAllBytes(textureFilePath);
                resourceStream.Write(BitConverter.GetBytes(textureData.Length));
                resourceStream.Write(textureData);

            }

            resourceStream.Write(BitConverter.GetBytes(0xFFFFFFFF));

            return SUCCESS;
        }

        private static int AddMaterialsToResource(Stream resourceStream, MapData mapData) {

            resourceStream.Write(BitConverter.GetBytes(mapData.materialNames.Length));

            foreach (string materialName in mapData.materialNames) {

                // Material not found
                if (!Cache.MATERIALS.ContainsKey(materialName)
                  || Cache.MATERIALS[materialName] == null) { // Not sure if this null check is even necessary, but better safe than sorry

                    Console.WriteLine("Material '{0}' was not found!", materialName);
                    continue;

                }

                // Write size of this block in bytes to file
                resourceStream.Write(BitConverter.GetBytes(Material.MATERIAL_SIZE));

                MaterialData materialData = Cache.MATERIALS[materialName];

                // Write color to file, provided as an R,G,B,A float array
                if (materialData.color == null)
                    for (int i = 0; i < 4; i++)
                        resourceStream.Write(BitConverter.GetBytes(1f));
                else
                    foreach (float value in materialData.color)
                        resourceStream.Write(BitConverter.GetBytes(value));

                // Refresh texture cache
                Cache.TEXTURES = Texture.GetExistingTextureNames();

                // Find index from texture name
                string texture = materialData.diffuseTexture;

                uint textureIndex = 0;

                if (texture == null)
                    Console.WriteLine("No texture provided, using default");
                else
                    textureIndex = (uint)Cache.TEXTURES.ToList().IndexOf(texture);

                // Write diffuse texture to file
                resourceStream.Write(BitConverter.GetBytes(textureIndex));

            }

            resourceStream.Write(BitConverter.GetBytes(0xFFFFFFFF));
            return SUCCESS;

        }

        private static int AddActorsToResource(Stream resourceStream, MapData mapData) {

            string[] allMaterialNames = Cache.MATERIALS.Keys.ToArray();

            resourceStream.Write(BitConverter.GetBytes(mapData.actorNames.Length));

            // Write the data names into the resource file
            foreach (string actorName in mapData.actorNames) {

                string actorFilePath = Program.ResourcesFolder + @"\" + actorName + "_Actor.json";

                if (!File.Exists(actorFilePath)) {
                    Console.WriteLine("'{0}' is not a valid actor!", actorName);
                    continue;
                }

                ActorData actorData = JsonConvert.DeserializeObject<ActorData>(File.ReadAllText(actorFilePath));

                uint actorDataSize = (uint)actorData.vertices.Length * 4;
                resourceStream.Write(BitConverter.GetBytes(actorDataSize));

                // Write the local transform as a 4x4 matrix into the file
                for (int y = 0; y < 16; y++)
                    resourceStream.Write(BitConverter.GetBytes(actorData.localTransform[y]));

                // Find the material ID from the material name
                byte id;

                for (id = 0; id < Cache.MATERIALS.Keys.Count; id++)
                    if (allMaterialNames[id] == actorData.materialName)
                        break;

                // Just take the last material in case none was supplied
                // We can avoid excessive error handling that way
                resourceStream.WriteByte(id);

                // Write the layer id to the file (e.g. 0 for game actors and 1 for UI actors)
                resourceStream.WriteByte(actorData.layerId);

                // Write the index count so that we know how many indices are in the actor
                resourceStream.Write(BitConverter.GetBytes(actorData.indexCount));

                // Write the amount of vertex points into the file
                resourceStream.Write(BitConverter.GetBytes(actorData.vertexCount));

                // Write all indices to file
                if (actorData.indexCount != 0) // Not sure how to handle this?
                    foreach (int index in actorData.indices)
                        resourceStream.Write(BitConverter.GetBytes(index));

                // Write all vertices to the file
                if (actorData.vertexCount != 0) // Not sure how to handle this?
                    foreach (float vertex in actorData.vertices)
                        resourceStream.Write(BitConverter.GetBytes(vertex));


            }

            resourceStream.Write(BitConverter.GetBytes(0xFFFFFFFF));

            return SUCCESS;

        }

    }
}
