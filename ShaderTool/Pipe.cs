using System;
using System.IO;
using Newtonsoft.Json;
using System.Text.RegularExpressions;
using static ShaderTool.Error;
using static ShaderTool.Util;

namespace ShaderTool
{
    /**
     *  Command: pipe
     *  Creates pipe json data
     */
    class Pipe
    {
        public static int PipeCommand(string[] args)
        {
            AsssertNoneNull(args);

            switch (args[0])
            {
                case "create":
                    return PipeCreate(GetParams(args));
                case "list":
                    return PipeList(null);
                case "delete":
                    return PipeDelete(GetParams(args));
            }

            Console.WriteLine("Wrong parameters! Must be create/list/delete!");
            return WRONG_PARAMS;
        }

        // Create pipe
        public static int PipeCreate(string[] args)
        {
            AsssertNoneNull(args);
            AsssertValues(args, 2);

            string Name = args[0];
            string[] Shader = GetParams(args);

            string FileName = Program.CWD + "\\" + Name + "Pipe.json";

            if (File.Exists(FileName))
            {
                Console.WriteLine("Pipe already exist with the name " + Name);
                return ALREADY_EXIST;
            }

            string[] vertex = Array.FindAll(Shader, pth => pth.StartsWith("Vertex"));

            if (vertex.Length != 1)
            {
                Console.WriteLine("VertexShader count != 1");
                return WRONG_SHADER_COUNT;
            }

            string[] ot = Array.FindAll(File.ReadAllLines(Program.CWD + "\\" + vertex[0] + ".glsl"), line => line.Contains(" in ") && line.Contains("layout"));
            Input[] Inputs = new Input[ot.Length];

            Regex rx = new Regex("[^0-9]");

            Array.ForEach(ot, line => {
                uint Id = 0;
                string Strid = rx.Replace(line.Split("layout")[1].Split(")")[0], "");
                if (!UInt32.TryParse(Strid, out Id))
                {
                    Console.WriteLine("Vertex input id not found");
                    Environment.Exit(VERTEX_INPUT_ERR);
                }
                Inputs[Id] = new Input();
                Inputs[Id].Id = Id;

                string layout = line.Split(" in ")[1].Split(" ")[0];
                Inputs[Id].Layout = VulkanLookups.LookUp(layout);
            });

            for (int i = 1; i < Inputs.Length; i++)
            {
                Inputs[i].Offset = Inputs[i - 1].Offset + VulkanLookups.LookupSize(Inputs[i - 1].Layout);
            }

            File.Create(FileName).Close();
            string str = JsonConvert.SerializeObject(new ShaderPipe(Name, Shader, Inputs));
            File.WriteAllText(FileName, str);
            return SUCESS;
        }

        // List pipes
        public static int PipeList(string[] args)
        {
            Array.ForEach(Directory.GetFiles(Program.CWD, "*Pipe.json"), pth => Console.WriteLine(pth.Replace(Program.CWD + "\\", "")));
            return SUCESS;
        }

        // Delete pipeline
        public static int PipeDelete(string[] args)
        {
            AsssertNoneNull(args);
            string Name = args[0];
            File.Delete(Program.CWD + "\\" + Name + "Pipe.json");
            return SUCESS;
        }

    }
}
