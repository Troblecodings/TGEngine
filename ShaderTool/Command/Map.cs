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
            List<string> newTextureNames = new List<string>();

            foreach (string actorName in map.actorNames) {

                string actorPath = Path.Combine(Program.ResourcesFolder, actorName + @"_Actor.json");
                string actorFileContent = File.ReadAllText(actorPath);
                ActorData actor = JsonConvert.DeserializeObject<ActorData>(actorFileContent);
                string materialName = actor.materialName;

                if (!newMaterialNames.Contains(materialName) && materialName != null) {
                    newMaterialNames.Add(materialName);
                }

                Material.Load();
                uint textureID = Cache.MATERIALS[materialName].diffuseTexture;

                Texture.Load();
                string textureName = Cache.PRELOAD.textures.Keys.ToArray()[textureID];
                if (!newTextureNames.Contains(textureName) && textureName != null) {
                    newTextureNames.Add(textureName);
                }
            }

            map.materialNames = newMaterialNames.ToArray();
            map.textureNames = newTextureNames.ToArray();
        }

        public static void AddActors(MapData map, string[] actorsToAdd) {
            AssertValues(actorsToAdd, 1);

            if (map.actorNames == null)
                map.actorNames = new string[0];
            if (map.materialNames == null)
                map.materialNames = new string[0];

            // We don't know how many actors are valid so I'm using a list to compensate for that
            List<string> actorNames = map.actorNames.ToList();

            foreach (string actorName in actorsToAdd) {
                string actorPath = Path.Combine(Program.ResourcesFolder, actorName + @"_Actor.json");

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
            AssertValues(args, 1);

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
            AssertValues(args, 1);

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
            string mapFilePath = Program.ResourcesFolder + "\\" + mapName + ".json";
            string resourceFilePath = Program.ResourcesFolder + "\\" + mapName + ".tgr";

            // .tgr file format documentation:
            // https://troblecodings.com/fileformat.html

            if (!File.Exists(mapFilePath)) {
                Console.WriteLine("{0} is not a map.", mapName);
                return WRONG_PARAMS;
            }

            File.Delete(resourceFilePath);

            Stream resourceStream = File.OpenWrite(resourceFilePath);
            resourceStream.Write(BitConverter.GetBytes(TGR_VERSION));

            MapData mapData = Load(mapName);

            Material.Load();

            int textureWriteStatus = AddTexturesToResource(resourceStream, mapData);
            if (textureWriteStatus != SUCCESS)
                return textureWriteStatus;

            int materialWriteStatus = AddMaterialsToResource(resourceStream, mapData);
            if (materialWriteStatus != SUCCESS)
                return materialWriteStatus;

            int actorWriteStatus = AddActorsToResource(resourceStream, mapData);
            if (actorWriteStatus != SUCCESS)
                return actorWriteStatus;

            resourceStream.Close();

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
            AssertValues(args, 2);

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
            AssertValues(args, 2);

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

        private static int AddTexturesToResource(Stream resourceStream, MapData mapData) {

            string[] mapTextureNames = mapData.textureNames;

            if (mapTextureNames == null)
                return SUCCESS;

            foreach (string mapTextureName in mapTextureNames) {

                string textureFilePath = Program.ResourcesFolder + "\\" + mapTextureName + ".tgx";

                if (!File.Exists(textureFilePath)) {
                    Console.WriteLine("{0} is not a texture.", mapTextureName);
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

            foreach (string materialName in mapData.materialNames) {

                // Material not found
                if (!Cache.MATERIALS.ContainsKey(materialName)
                  || Cache.MATERIALS[materialName] == null) { // Not sure if this null check is even necessary, but better safe than sorry

                    Console.WriteLine("Material {0} was not found!", materialName);
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

                // Write diffuse texture to file
                resourceStream.Write(BitConverter.GetBytes(materialData.diffuseTexture));

            }

            resourceStream.Write(BitConverter.GetBytes(0xFFFFFFFF));

            return SUCCESS;

        }

        private static int AddActorsToResource(Stream resourceStream, MapData mapData) {

            string[] allMaterialNames = Cache.MATERIALS.Keys.ToArray();

            // Write the data names into the resource file
            foreach (string actorName in mapData.actorNames) {

                string actorFilePath = Program.ResourcesFolder + @"\" + actorName + "_Actor.json";

                if (!File.Exists(actorFilePath)) {
                    Console.WriteLine("{0} is not a valid actor!", actorName);
                    continue;
                }

                ActorData actorData = JsonConvert.DeserializeObject<ActorData>(File.ReadAllText(actorFilePath));

                // Write size of this block in bytes to file
                // 2 + 2 * (4 + 8) = 26
                uint actorDataSize = 26 + (actorData.indexCount + actorData.vertexCount) * 4;
                resourceStream.Write(BitConverter.GetBytes(actorDataSize));

                // Write the local transform as a 4x4 matrix into the file
                for (int i = 0; i < 4; i++)
                    for (int y = 0; y < 4; y++)
                        resourceStream.Write(BitConverter.GetBytes(actorData.localTransform == null ? 0f : actorData.localTransform[i][y]));

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

                // Write 0, because we don't want to supply the pointer -> Nullpointer
                // reserved for further use
                resourceStream.Write(BitConverter.GetBytes(0l));

                // Write the amount of vertex points into the file
                resourceStream.Write(BitConverter.GetBytes(actorData.vertexCount));

                // Write 0, because we don't want to supply the pointer -> Nullpointer
                // reserved for further use
                resourceStream.Write(BitConverter.GetBytes(0l));

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
