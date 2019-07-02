using System.Diagnostics;
using System.IO;
using System.Reflection;
using static ShaderTool.Error;
using static ShaderTool.Util;

namespace ShaderTool.Command {
    class Shader {

        public static int ShaderCommand(string[] args) {
            AsssertNoneNull(args);
            switch (args[0]) {
                case "make":
                    return ShaderMake();

            }
            return SUCESS;
        }

        public static int ShaderMake() {
            string[] files = Directory.GetFiles(Program.CWD, "*.glsl");
            // Extract shader compiler
            Stream reader = Assembly.GetExecutingAssembly().GetManifestResourceStream("ShaderTool.glslangValidator.exe");
            reader.Seek(0, SeekOrigin.End);
            byte[] buffer = new byte[reader.Position];
            reader.Seek(0, SeekOrigin.Begin);
            reader.Read(buffer, 0, buffer.Length);
            File.WriteAllBytes("compiler.exe", buffer);
            //
            foreach (string path in files) {
                int i = Compile(path);
                if (i != SUCESS) {
                    return i;
                }
            }
            return SUCESS;
        }

        private static int Compile(string path) {
            Process.Start("compiler.exe", "");
            return SUCESS;
        }

    }
}
