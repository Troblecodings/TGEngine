using System;
using static ShaderTool.Error;
using static ShaderTool.Util;

namespace ShaderTool
{
    class Program
    {

        public static string CWD;

        public static int Execute(string id, string[] arg)
        {
            switch (id) {
                case "pipe":
                    return Pipe.PipeCommand(arg);
            }

            Console.WriteLine("Wrong parameters! Possible: pipe!");
            return WRONG_PARAMS;
        }

        public static int Main(string[] args)
        {
            if(args.Length < 1)
            {
                Console.WriteLine("No input given please enter!");
                args = Console.ReadLine().Split(" ");
            }

            if(args.Length < 1) {
                Console.WriteLine("Not enough parameter!");
                return NOT_ENOUGH_PARAMS;
            }

            CWD = Environment.CurrentDirectory + "../../../../../TGEngine/resources";
            return Execute(args[0], GetParams(args));
        }

    }
}
