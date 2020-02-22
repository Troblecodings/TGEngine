using System;
using System.Linq;
using System.Text.RegularExpressions;
using static ShaderTool.Error;

namespace ShaderTool.Util {
    class Util {

        // Array left shift
        public static string[] GetParams(string[] args) {

            if (args.Length > 1)
                return args.Skip(1).ToArray();
            else
                return null;

        }

        // Checks if the array is larger as a given size
        public static bool AssertValues(string[] args, int minlen = 1) {

            if (args == null || args.Length < minlen) {
                Console.WriteLine("Not enough arguments!");

                if (!Program.IsInConsoleMode)
                    Environment.Exit(NOT_ENOUGH_PARAMS);

                return false;
            }

            return true;
        }

        public static bool AssertName(string name) {

            Regex nonAlphaNumericCharacters = new Regex(@"\W");

            if (name.Length < 2) {

                Console.WriteLine("Name has to be at least two (2) characters!");
                return false;

            } else if (nonAlphaNumericCharacters.IsMatch(name)) {

                Console.WriteLine("Name can only contain alphanumeric characters!");
                return false;

            } else {

                return true;

            }
        }
    }
}
