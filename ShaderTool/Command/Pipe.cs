using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    /**
     *  Command: pipe
     *  Creates pipe json data
     */
    class Pipe {
        public static int PipeCommand(string[] args) {
            AsssertNoneNull(args);

            switch (args[0]) {
                case "create":
                    return PipeCreate(GetParams(args));
                case "list":
                    return PipeList();
                case "delete":
                    return PipeDelete(GetParams(args));
                case "make":
                    return PipeMake();
                case "show":
                    return PipeShow(GetParams(args));
                case "update":
                    return PipeUpdate(GetParams(args));
            }

            Console.WriteLine("Wrong parameters! Must be create/list/delete/make/show/update!");
            return WRONG_PARAMS;
        }

        // Updates inputs
        public static int PipeUpdate(string[] args) {
            AsssertNoneNull(args);
            string Path = System.IO.Path.Combine(Program.CWD, args[0] + "Pipe.json");
            if (File.Exists(Path)) {
                // Getting old Pipe
                string Text = File.ReadAllText(Path);
                ShaderPipe Pipe = JsonConvert.DeserializeObject<ShaderPipe>(Text);
                string VertexShader = Array.Find(Pipe.ShaderNames, Name => Name.StartsWith("Vertex"));
                string VsPath = Program.CWD + "\\" + VertexShader + ".glsl";
                if (File.Exists(VsPath)) {
                    // Updating inputs
                    Pipe.Inputs = GetInputs(VertexShader);
                    // Updatign descriptors
                    Pipe.Descriptors = (Descriptor[])GetDescriptors(Pipe.ShaderNames).Clone();

                    File.WriteAllText(Path, JsonConvert.SerializeObject(Pipe, Formatting.Indented));
                    return SUCCESS;
                }
                Console.WriteLine("Shader doesn't exist!");
                return SHADER_DOESNT_EXIST;
            }
            Console.WriteLine("Pipe doesn't exist!");
            return SHADER_DOESNT_EXIST;
        }

        public static int PipeShow(string[] args) {
            AsssertNoneNull(args);
            string Path = System.IO.Path.Combine(Program.CWD, args[0] + "Pipe.json");
            if (File.Exists(Path)) {
                string Text = File.ReadAllText(Path);
                ShaderPipe Pipe = JsonConvert.DeserializeObject<ShaderPipe>(Text);
                Console.WriteLine();
                Console.WriteLine(Pipe.Name);
                Console.WriteLine("Shader count " + Pipe.ShaderNames.Length);
                Console.WriteLine("Input count " + Pipe.Inputs.Length);
                Console.WriteLine("Descriptor count " + Pipe.Descriptors.Length);
                Console.WriteLine();
                Array.ForEach(Pipe.ShaderNames, Console.WriteLine);
                Console.WriteLine();
                Array.ForEach(Pipe.Inputs, Inpt => Console.WriteLine(Inpt.Id + ": layout=" + Inpt.Layout + ", offset=" + Inpt.Offset));
                Console.WriteLine();
                Array.ForEach(Pipe.Descriptors, Desc => Console.WriteLine(Desc.Binding + ": flag=" + Desc.flag + ", type=" + Desc.Type));
                Console.WriteLine();
                return SUCCESS;
            }
            Console.WriteLine("Pipe doesn't exist! " + Path);
            return SHADER_DOESNT_EXIST;
        }

        // Create pipe
        public static int PipeCreate(string[] args) {
            AsssertNoneNull(args);
            AsssertValues(args, 2);

            string Name = args[0];
            string[] Shader = GetParams(args);

            string FileName = Path.Combine(Program.CWD, Name + "Pipe.json");

            // Check if pipe already exists
            if (File.Exists(FileName)) {
                Console.WriteLine("Pipe already exist with the name " + Name);
                return ALREADY_EXIST;
            }

            foreach (string FileN in Shader) {
                if (!File.Exists(Path.Combine(Program.CWD, FileN + ".glsl"))) {
                    Console.WriteLine(FileN + " doesn't exist!");
                    return SHADER_DOESNT_EXIST;
                }
            }

            // Get vertex shader
            string[] Vertex = Array.FindAll(Shader, pth => pth.StartsWith("Vertex"));

            // Error if count 0 or more then one is defined
            if (Vertex.Length != 1) {
                Console.WriteLine("VertexShader count != 1");
                return WRONG_SHADER_COUNT;
            }

            Input[] Inputs = GetInputs(Vertex[0]);

            Descriptor[] descriptors = GetDescriptors(Shader);

            // Create shader pipe
            File.Create(FileName).Close();
            string str = JsonConvert.SerializeObject(new ShaderPipe(Name, Shader, Inputs, descriptors), Formatting.Indented);
            File.WriteAllText(FileName, str);
            return SUCCESS;
        }

        private static Input[] GetInputs(string Path) {
            // Getting lines with input
            string[] InputLines = Array.FindAll(File.ReadAllLines(System.IO.Path.Combine(Program.CWD, Path + ".glsl")), line => line.Contains(" in ") && line.Contains("layout"));
            Input[] Inputs = new Input[InputLines.Length];

            Regex rx = new Regex("[^0-9]");
            // Processing inputs
            Array.ForEach(InputLines, line => {
                uint Id = 0;
                string Strid = rx.Replace(line.Split("layout")[1].Split(")")[0], "");
                if (!UInt32.TryParse(Strid, out Id)) {
                    Console.WriteLine("Vertex input id not found");
                    Environment.Exit(VERTEX_INPUT_ERR);
                }
                Inputs[Id] = new Input {
                    Id = Id
                };

                string layout = line.Split(" in ")[1].Split(" ")[0];
                Inputs[Id].Layout = VulkanLookups.LookUp(layout);
            });

            // Calculate offsets
            for (int i = 1; i < Inputs.Length; i++) {
                Inputs[i].Offset = Inputs[i - 1].Offset + VulkanLookups.LookupSize(Inputs[i - 1].Layout);
            }
            return Inputs;
        }

        private static Descriptor[] GetDescriptors(string[] Paths) {
            List<Descriptor> list = new List<Descriptor>();

            foreach (string path in Paths) {
                // Getting lines with descriptors
                string[] DesciptorLines = Array.FindAll(File.ReadAllLines(Path.Combine(Program.CWD, path + ".glsl")), line => line.Contains("binding") && line.Contains("layout"));

                Regex rx = new Regex("[^0-9]");
                // Processing desciptors
                Array.ForEach(DesciptorLines, line => {
                    uint Id = 0;
                    string Strid = rx.Replace(line.Split("layout")[1].Split(")")[0], "");
                    if (!UInt32.TryParse(Strid, out Id)) {
                        Console.WriteLine("Descriptor id not found!");
                        Environment.Exit(VERTEX_INPUT_ERR);
                    }
                    Descriptor desc = new Descriptor {
                        Binding = Id,
                        Type = VulkanLookups.GetTypeFromLine(line),
                        flag = VulkanLookups.GetFlagBitsAfterName(path)
                    };
                    list.Add(desc);
                });
            }
            return list.ToArray();
        }

        // List pipes
        public static int PipeList() {
            Array.ForEach(Directory.GetFiles(Program.CWD, "*Pipe.json"), pth => Console.WriteLine(pth.Replace(Program.CWD + Path.DirectorySeparatorChar, "").Replace("Pipe.json", "")));
            return SUCCESS;
        }

        // Delete pipeline
        public static int PipeDelete(string[] args) {
            AsssertNoneNull(args);
            string Name = args[0];
            File.Delete(Path.Combine(Program.CWD, Name + "Pipe.json"));
            return SUCCESS;
        }

        public static int PipeMake() {
            // Get path
            string HeaderFileName = Path.Combine(Program.CWD, "ShaderPipes.hpp");
            string SourceFileName = Path.Combine(Program.CWD, "ShaderPipes.cpp");

            // Recreate files
            StreamWriter HeaderWriter = File.CreateText(HeaderFileName);
            StreamWriter SourceWriter = File.CreateText(SourceFileName);

            // Auto generated header and source
            HeaderWriter.WriteLine("#pragma once");
            HeaderWriter.WriteLine("#include \"ShaderData.hpp\"");
            HeaderWriter.WriteLine("// Auto generated / Don't change");
            HeaderWriter.WriteLine();

            SourceWriter.WriteLine("#include \"ShaderPipes.hpp\"");
            SourceWriter.WriteLine("// Auto generated / Don't change");
            SourceWriter.WriteLine();

            string[] PipeFiles = Directory.GetFiles(Program.CWD, "*Pipe.json");

            ShaderPipe[] ShaderPipes = new ShaderPipe[PipeFiles.Length];
            // Load shader pipes
            for (int i = 0; i < PipeFiles.Length; i++) {
                string Text = File.ReadAllText(PipeFiles[i]);
                ShaderPipes[i] = JsonConvert.DeserializeObject<ShaderPipe>(Text);
                SourceWriter.WriteLine(ShaderPipes[i]);
                HeaderWriter.WriteLine(ShaderPipes[i].GenHeader());
            }

            // Initalize shader pipes
            HeaderWriter.WriteLine();
            HeaderWriter.WriteLine("void initShaderPipes();");
            SourceWriter.WriteLine();
            SourceWriter.WriteLine("void initShaderPipes(){");

            Array.ForEach(ShaderPipes, Pipe => SourceWriter.WriteLine(Pipe.GenCreation()));

            SourceWriter.WriteLine("}");
            SourceWriter.Close();
            HeaderWriter.Close();
            return SUCCESS;
        }

    }
}
