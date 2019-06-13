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
                    return PipeList();
                case "delete":
                    return PipeDelete(GetParams(args));
                case "make":
                    return PipeMake();
            }

            Console.WriteLine("Wrong parameters! Must be create/list/delete/make!");
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

            // Check if pipe already exists
            if (File.Exists(FileName))
            {
                Console.WriteLine("Pipe already exist with the name " + Name);
                return ALREADY_EXIST;
            }

            foreach(string FileN in Shader)
            {
                
            }

            // Get vertex shader
            string[] Vertex = Array.FindAll(Shader, pth => pth.StartsWith("Vertex"));

            // Error if count 0 or more then one is defined
            if (Vertex.Length != 1)
            {
                Console.WriteLine("VertexShader count != 1");
                return WRONG_SHADER_COUNT;
            }

            // Getting lines with input
            string[] InputLines = Array.FindAll(File.ReadAllLines(Program.CWD + "\\" + Vertex[0] + ".glsl"), line => line.Contains(" in ") && line.Contains("layout"));
            Input[] Inputs = new Input[InputLines.Length];

            Regex rx = new Regex("[^0-9]");
            // Processing inputs
            Array.ForEach(InputLines, line => {
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

            // Calculate offsets
            for (int i = 1; i < Inputs.Length; i++)
            {
                Inputs[i].Offset = Inputs[i - 1].Offset + VulkanLookups.LookupSize(Inputs[i - 1].Layout);
            }

            // Create shader pipe
            File.Create(FileName).Close();
            string str = JsonConvert.SerializeObject(new ShaderPipe(Name, Shader, Inputs), Formatting.Indented);
            File.WriteAllText(FileName, str);
            return SUCESS;
        }

        // List pipes
        public static int PipeList()
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

        public static int PipeMake()
        {
            string HeaderFileName = Program.CWD + "\\ShaderPipes.hpp";
            string SourceFileName = Program.CWD + "\\ShaderPipes.cpp";

            StreamWriter HeaderWriter = File.CreateText(HeaderFileName);
            StreamWriter SourceWriter = File.CreateText(SourceFileName);

            HeaderWriter.WriteLine("#pragma once");
            HeaderWriter.WriteLine("#include \"ShaderData.hpp\"");
            HeaderWriter.WriteLine();

            SourceWriter.WriteLine("#include \"ShaderPipes.hpp\"");
            SourceWriter.WriteLine();

            string[] PipeFiles = Directory.GetFiles(Program.CWD, "*Pipe.json");

            ShaderPipe[] ShaderPipes = new ShaderPipe[PipeFiles.Length];

            for (int i = 0; i < PipeFiles.Length; i++) {
                string Text = File.ReadAllText(PipeFiles[i]);
                ShaderPipes[i] = JsonConvert.DeserializeObject<ShaderPipe>(Text);
            }

            Array.ForEach(ShaderPipes, pipe => { SourceWriter.WriteLine("ShaderPipe " + pipe.Name + ";"); HeaderWriter.WriteLine("extern ShaderPipe " + pipe.Name + ";"); } );

            HeaderWriter.WriteLine();
            SourceWriter.WriteLine();
            HeaderWriter.WriteLine("void initShaderPipes();");
            SourceWriter.WriteLine("void initShaderPipes() {");

            Array.ForEach(ShaderPipes, pipe => SourceWriter.WriteLine("    " + pipe.Name + " = createShaderPipe(" + pipe + ");"));
            SourceWriter.WriteLine("}");

            SourceWriter.Close();
            HeaderWriter.Close();
            return SUCESS;
        }

    }
}
