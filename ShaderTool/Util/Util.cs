using System;
using static ShaderTool.Error;

namespace ShaderTool.Util {
    class Util {
        // Array left shift
        public static string[] GetParams(string[] old_args) {
            string[] args_copy = null;
            if (old_args.Length > 1) {
                args_copy = new string[old_args.Length - 1];
                for (int i = 1; i < old_args.Length; i++) {
                    args_copy[i - 1] = old_args[i];
                }
            }
            return args_copy;
        }

        // Assert if the value is null
        public static void AsssertNoneNull(string[] args) {
            if (args == null) {
                Console.WriteLine("Not enough arguments!");
                if (!Program.console)
                    Environment.Exit(NOT_ENOUGH_PARAMS);
            }
        }

        // Checks if the array is bigger as a given size
        public static void AsssertValues(string[] args, int minlen) {
            if (args == null || args.Length < minlen) {
                Console.WriteLine("Not enough arguments!");
                if(!Program.console)
                    Environment.Exit(NOT_ENOUGH_PARAMS);
            }
        }
    }
}
