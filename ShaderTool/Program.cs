using ShaderTool.Command;
using System;
using static ShaderTool.Error;
using static ShaderTool.Util;

namespace ShaderTool {
    class Program {

        public static string CWD;

        public static int Execute(string id, string[] arg) {
            switch (id) {
                case "pipe":
                    return Pipe.PipeCommand(arg);
                case "shader":
                    return Shader.ShaderCommand(arg);
            }

            Console.WriteLine("Wrong parameters! Possible: pipe!");
            return WRONG_PARAMS;
        }

        public static int Main(string[] args) {

            args = GetParamas(args);
            if (args.Length < 1) {
                while (true) {
                    Console.Write(">>> ");
                    args = Console.ReadLine().Split(" ");
                    args = GetParamas(args);
                    AsssertValues(args, 1);
                    int rcode = Execute(args[0], GetParams(args));
                    Console.WriteLine("Exite code " + rcode);
                }
            }
            AsssertValues(args, 1);
            return Execute(args[0], GetParams(args));
        }

        public static string[] GetParamas(string[] args) {
            CWD = Environment.CurrentDirectory;
            for (int i = 0; i < args.Length - 1; i++) {
                if (args[i] == "--dir") {
                    string path = args[i + 1];
                    if (path == "tool") {
                        CWD += "../../../../../TGEngine/resources";
                    } else {
                        CWD = path;
                    }
                    string[] cpy = (string[])args.Clone();
                    args = Array.FindAll(cpy, str => str != "--dir" && str != path);
                    break;
                }
            }
            return args;
        }

    }
}
