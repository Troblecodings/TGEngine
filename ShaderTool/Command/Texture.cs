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

        public const string TEXTURE_FILE_EXTENSION = ".tgx";

        public static int TextureCommand(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            switch (args[0]) {
                case "add":
                    return TextureAdd(GetParams(args));
                case "rm":
                case "remove":
                    return TextureRm(GetParams(args));
                case "list":
                    return TextureList();
            }

            Console.WriteLine("Wrong params! Possible: add/rm/list/import");
            return WRONG_PARAMS;
        }

        public static string GetFilePath(string textureName) => Path.Combine(Program.ResourcesFolder, textureName + TEXTURE_FILE_EXTENSION);

        public static int TextureAdd(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            string[] texturePaths = args;
            if (Program.IsInConsoleMode) { // Args does the parsing already
                // All paths are required to be put in double quotes as paths with spaces in them would break
                string input = string.Join(' ', args);

                if (!input.Contains('"')) { // Allow a single path without spaces
                    Console.WriteLine("Paths could not be read! (did you forget to wrap the paths in quotes?)");
                    return WRONG_PARAMS;
                }

                texturePaths = input.Replace(@"\", @"/")
                                             .Split('"')
                                             .Where(path => !string.IsNullOrWhiteSpace(path))
                                             .ToArray();
            }

            if (!Directory.Exists(Program.ResourcesFolder))
                Directory.CreateDirectory(Program.ResourcesFolder);

            int returncode = SUCCESS;

            foreach (string texturePath in texturePaths) {

                if (!File.Exists(texturePath)) {
                    Console.WriteLine("Texture '{0}' could not be found, skipping", texturePath);
                    returncode = WRONG_PARAMS;
                    continue;
                }

                string fileName = Path.GetFileNameWithoutExtension(texturePath).Replace(" ", "_");

                if (GetExistingTextureNames().Contains(fileName)) {
                    Console.WriteLine("Texture '{0}' already exists, skipping", fileName);
                    returncode = WRONG_PARAMS;
                    continue;
                }

                File.Copy(texturePath, Texture.GetFilePath(fileName));
                Console.WriteLine("Texture '{0}' was successfully added!", fileName);
            }

            return returncode;
        }

        public static int TextureRm(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            // If the file name contains a space then it's being taken care of with this
            string fileName = string.Join(" ", args);

            string path = GetFilePath(fileName);
            if (!File.Exists(path)) {
                Console.WriteLine("Texture '{0}' was not found!", fileName);
                return WRONG_PARAMS;
            }

            File.Delete(path);
            Console.WriteLine("Texture '{0}' was successfully removed!", fileName);
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
