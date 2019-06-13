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
            CWD = Environment.CurrentDirectory + "../../../../../TGEngine/resources";

            if (args.Length < 1)
            {
                while (true) {
                    Console.WriteLine("Enter input!");
                    args = Console.ReadLine().Split(" ");
                    AsssertValues(args, 1);
                    int rcode = Execute(args[0], GetParams(args));
                    if(rcode != 0)
                    {
                        return rcode;
                    }
                }
            }
            AsssertValues(args, 1);
            return Execute(args[0], GetParams(args));
        }

    }
}
