using Newtonsoft.Json;
using ShaderTool.Util;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {

    class Font {

        public static int FontCommand(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            switch (args[0]) {
                case "add":
                    return FontAdd(GetParams(args));
                case "rm":
                case "remove":
                    return FontRm(GetParams(args));
                case "list":
                    return FontList();
            }

            Console.WriteLine("Wrong parameters! Must be save/add/rm/list!");
            return WRONG_PARAMS;
        }

        public static int FontAdd(string[] args) {
            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            string input = string.Join(' ', args);

            if (!input.Contains('"')) { // Allow a single path without spaces
                Console.WriteLine("Paths could not be read! (did you forget to wrap the paths in quotes?)");
                return WRONG_PARAMS;
            }

            string[] fontPaths = input.Replace(@"\", @"/")
                                         .Split('"')
                                         .Where(path => !string.IsNullOrWhiteSpace(path))
                                         .ToArray();

            if (!Directory.Exists(Program.ResourcesFolder))
                Directory.CreateDirectory(Program.ResourcesFolder);

            foreach (string fontPath in fontPaths) {

                if (!File.Exists(fontPath)) {
                    Console.WriteLine("Font '{0}' could not be found, skipping", fontPath);
                    continue;
                }

                string fileName = Path.GetFileName(fontPath).Replace(" ", "_");

                if (Directory.GetFiles(Program.ResourcesFolder, "*.ttf").Contains(fileName)) {
                    Console.WriteLine("Font '{0}' already exists, skipping", fileName);
                    continue;
                }

                File.Copy(fontPath, Path.Combine(Program.ResourcesFolder, fileName));
                Console.WriteLine("Font '{0}' was successfully added!", fileName);

            }
            return SUCCESS;
        }

        public static int FontRm(string[] args) {
            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            foreach (string x in args) {
                if (!File.Exists(x)) {
                    Console.WriteLine("Font {0} does not exist, skipping!", x);
                    continue;
                }

                File.Delete(x);
            }
            return SUCCESS;
        }

        public static int FontList() {
            Array.ForEach(Directory.GetFiles(Program.ResourcesFolder, "*.ttf"), Console.WriteLine);
            return SUCCESS;
        }

    }

}
