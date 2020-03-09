using System.Diagnostics;
using System.IO;
using System;
using static ShaderTool.Error;
using static ShaderTool.Util.Util;

namespace ShaderTool.Command {
    class Shader {

        private static string Path;
        private static string LastShader;
        private static bool Error = false;

        public static int ShaderCommand(string[] args) {

            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;

            CheckShader();
            switch (args[0]) {
                case "make":
                    return ShaderMake();
                case "list":
                    return ShaderList();
                case "compile":
                    return ShaderCompile(GetParams(args));

            }
            return WRONG_PARAMS;
        }

        public static int ShaderCompile(string[] args) {
            if (!AssertValues(args))
                return NOT_ENOUGH_PARAMS;
            Compile(args[0]);
            Make();
            return SUCCESS;
        }

        public static int ShaderList() {
            Array.ForEach(Directory.GetFiles(Program.CWD, "*.glsl"), path => Console.WriteLine(path.Replace(Program.CWD + System.IO.Path.DirectorySeparatorChar, "")));
            return SUCCESS;
        }

        public static void CheckShader() {
            string vulkanPath = Environment.GetEnvironmentVariable("VULKAN_SDK");
            if (Directory.Exists(vulkanPath)) {
                Path = vulkanPath + System.IO.Path.DirectorySeparatorChar;
            } else {
                Console.WriteLine("Vulkan path not found was " + vulkanPath);
                Environment.Exit(WRONG_PARAMS);
            }
        }

        public static int ShaderMake() {
            string[] files = Directory.GetFiles(Program.CWD, "*.glsl");
            foreach (string path in files) {
                int i = Compile(path);
                if (i != SUCCESS) {
                    return i;
                }
            }
            Make();
            return SUCCESS;
        }

        public static void Make() {
            string[] files = Directory.GetFiles(Program.CWD, "*.spv");

            string dataHpp = System.IO.Path.Combine(Program.CWD, "ShaderData.hpp");
            string dataCpp = System.IO.Path.Combine(Program.CWD, "ShaderData.cpp");

            File.Delete(dataHpp);
            File.Delete(dataCpp);

            Stream hppStream = File.OpenWrite(dataHpp);
            Stream cppStream = File.OpenWrite(dataCpp);
            StreamWriter shaderDataHPP = new StreamWriter(hppStream);
            StreamWriter shaderDataCPP = new StreamWriter(cppStream);

            shaderDataCPP.WriteLine("#include \"ShaderData.hpp\"\r\n");
            shaderDataHPP.WriteLine("#pragma once\r\n#include \"../pipeline/ShaderCreation.hpp\"\r\nvoid initShader();\r\n");
            foreach (string path in files) {
                string name = path.Replace(Program.CWD, "").Replace(".spv", "").Replace("\\", "").Replace("/", "");
                shaderDataHPP.WriteLine("extern unsigned char " + name + "Module[];\r\nextern VkPipelineShaderStageCreateInfo " + name + ";");
                shaderDataCPP.Write("VkPipelineShaderStageCreateInfo " + name + ";\r\nunsigned char " + name + "Module[] = { ");
                Array.ForEach(File.ReadAllBytes(path), byt => shaderDataCPP.Write(byt + ","));
                shaderDataCPP.Flush();
                cppStream.Seek(-1, SeekOrigin.Current);
                shaderDataCPP.WriteLine("};");
                shaderDataCPP.Flush();
            }
            shaderDataCPP.WriteLine("\r\nvoid initShader() {");
            foreach (string path in files) {
                FileInfo fileInfo = new FileInfo(path);
                long length = fileInfo.Length;
                string name = path.Replace(Program.CWD, "").Replace(".spv", "").Replace("\\", "").Replace("/", "");
                string shaderstage = path.Contains("Vertex") ? "VK_SHADER_STAGE_VERTEX_BIT" : "VK_SHADER_STAGE_FRAGMENT_BIT";
                shaderDataCPP.WriteLine(name + " = createShader(" + name + "Module, " + shaderstage + ", " + length + ");");
            }
            shaderDataCPP.WriteLine("}");
            shaderDataHPP.Close();
            shaderDataCPP.Close();

        }

        private static int Compile(string path) {
            LastShader = System.IO.Path.GetFileNameWithoutExtension(path);
            Error = false;
            Process pr = new Process();
            pr.StartInfo.FileName = Path + "Bin\\glslangValidator.exe";
            pr.StartInfo.Arguments = "-V -o " + path.Replace(".glsl", "") + ".spv -S " + (path.Contains("Vertex") ? "vert" : "frag") + " " + path;
            pr.StartInfo.UseShellExecute = false;
            pr.StartInfo.RedirectStandardOutput = true;
            pr.StartInfo.RedirectStandardError = true;
            pr.ErrorDataReceived += Pr_ErrorDataReceived;
            pr.OutputDataReceived += Pr_OutputDataReceived;
            pr.Start();
            pr.BeginOutputReadLine();
            pr.BeginErrorReadLine();
            pr.WaitForExit();
            if (Error)
                return COMPILER_ERROR;
            return SUCCESS;
        }

        private static void Pr_OutputDataReceived(object sender, DataReceivedEventArgs e) {
            if (e.Data == null || e.Data.Length <= 0) return;
            Console.WriteLine("[" + LastShader + "] " + e.Data);
        }

        private static void Pr_ErrorDataReceived(object sender, DataReceivedEventArgs e) {
            if (e.Data == null || e.Data.Length <= 0) return;
            Error = true;
            Console.WriteLine("[ERROR! | " + LastShader + "] " + e.Data);
        }
    }
}
