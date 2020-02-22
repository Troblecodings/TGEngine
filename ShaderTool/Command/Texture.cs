using Newtonsoft.Json;
using ShaderTool.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Texture {

        public static int TextureCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "add":
                    return TextureAdd(GetParams(args));
                case "rm":
                    return TextureRm(GetParams(args));
                case "list":
                    return TextureList();
            }

            Console.WriteLine("Wrong params! Possible: add/rm/list/import");
            return WRONG_PARAMS;
        }

        public static int TextureAdd(string[] args) {

            AssertValues(args, 1);

            // All paths are required to be put in double quotes as paths with spaces in them would break
            string input = string.Join(' ', args);

            if (!input.Contains('"')) { // Allow a single path without spaces
                Console.WriteLine("Paths could not be read! (did you forget to wrap the paths in quotes?)");
                return WRONG_PARAMS;
            }

            string[] texturePaths = input.Replace(@"\", @"/")
                                         .Split('"')
                                         .Where(path => !string.IsNullOrWhiteSpace(path))
                                         .ToArray();

            if (!Directory.Exists(Program.ResourcesFolder))
                Directory.CreateDirectory(Program.ResourcesFolder);

            foreach (string texturePath in texturePaths) {

                if (!File.Exists(texturePath)) {
                    Console.WriteLine("Texture {0} could not be found, skipping", texturePath);
                    continue;
                }

                string fileName = Path.GetFileNameWithoutExtension(texturePath) + ".tgx";

                if (GetExistingTextureNames().Contains(fileName)) {
                    Console.WriteLine("Texture {0} already exists, skipping", texturePath);
                    continue;
                }

                File.Copy(texturePath, Path.Combine(Program.ResourcesFolder, fileName));

            }

            return SUCCESS;
        }

        public static int TextureRm(string[] args) {

            AssertValues(args, 1);

            // If the file name contains a space then it's being taken care of with this
            string fileName = string.Join(" ", args);

            if (!GetExistingTextureNames().Contains(fileName)) {
                Console.WriteLine("Texture {0} was not found!", fileName);
                return WRONG_PARAMS;
            } else {
                File.Delete(Path.Combine(Program.ResourcesFolder, fileName + ".tgx"));
            }

            Console.WriteLine("Texture {0} was successfully removed!", fileName);
            return SUCCESS;

        }

        public static int TextureList() {
            string[] textures = GetExistingTextureNames();
            Console.WriteLine("Count: {0} textures", textures.Length);

            if (textures.Length == 0)
                Console.WriteLine("No textures added, use 'texture add \"<path>\"' to add a new texture");
            else
                foreach (string texture in textures)
                    Console.WriteLine("- {0}", texture);

            return 0;
        }

        public static string[] GetExistingTextureNames() {

            string[] textures = Directory.GetFiles(Program.ResourcesFolder)
                                         .Where(file => file.EndsWith(".tgx"))
                                         .Select(path => Path.GetFileNameWithoutExtension(path))
                                         .ToArray();

            return textures;

        }

    }

}
