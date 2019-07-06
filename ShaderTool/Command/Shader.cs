using System.Diagnostics;
using System.IO;
using System;
using static ShaderTool.Error;
using static ShaderTool.Util;

namespace ShaderTool.Command {
    class Shader {

        private static string Path;
        private static string LastShader;
        private static bool Error = false;

        public static int ShaderCommand(string[] args) {
            AsssertNoneNull(args);
            CheckShader();
            switch (args[0]) {
                case "make":
                    return ShaderMake();

            }
            return WRONG_PARAMS;
        }

        public static void CheckShader() {
            string vulkanPath = Environment.GetEnvironmentVariable("VULKAN_SDK");
            if (Directory.Exists(vulkanPath)) {
                Path = vulkanPath + "\\";
            } else {
                Console.WriteLine("Vulkan path not found was " + vulkanPath);
                Environment.Exit(WRONG_PARAMS);
            }
        }

        public static int ShaderMake() {
            string[] files = Directory.GetFiles(Program.CWD, "*.glsl");
            Console.WriteLine(files.Length);
            foreach (string path in files) {
                int i = Compile(path);
                if (i != SUCESS) {
                    return i;
                }
            }
            return SUCESS;
        }

        private static int Compile(string path) {
            LastShader = path.Replace(Program.CWD + "\\", "").Replace(".glsl", "");
            Error = false;
            Process pr = new Process();
            pr.StartInfo.FileName = Path + "Bin\\glslangValidator.exe";
            pr.StartInfo.Arguments = "-V -o " + path.Replace(".glsl", "") + ".spv -S " + (path.Contains("Vertex") ? "vert":"frag") + " " + path;
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
            return SUCESS;
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
