namespace ShaderTool.Command
{
    class ShaderPipe
    {
        public string Name;
        public string[] ShaderNames;
        public Input[] Inputs;

        public ShaderPipe(string Name, string[] ShaderNames, Input[] Inputs)
        {
            this.Name = Name;
            this.ShaderNames = (string[])ShaderNames.Clone();
            this.Inputs = (Input[])Inputs.Clone();
        }

        public override string ToString()
        {
            string ShaderModule = "VkPipelineShaderStageCreateInfo " + this.Name + "Shader[" + this.ShaderNames.Length + "];\r\n";

            string Input = "const VkVertexInputAttributeDescription " + this.Name + "Input[] = {";
            foreach (Input str in Inputs)
            {
                Input += str + ",";
            }
            Input = Input.Substring(0, Input.Length - 1) + "};\r\n";

            return ShaderModule + Input 
                + "const unsigned int " + this.Name + "ShaderCount = " + ShaderNames.Length + ";\r\n" 
                + "const unsigned int " + this.Name + "InputCount = " + Inputs.Length + ";\r\n";
        }

        public string GenCreation()
        {
            string ShaderModule = "";
            uint i = 0;
            foreach (string str in ShaderNames)
            {
                ShaderModule += "    " + this.Name + "Shader[" + i++ + "] = " + str + ";\r\n";
            }
            return ShaderModule;
        }

        public string GenHeader()
        {
            return "extern VkPipelineShaderStageCreateInfo " + this.Name + "Shader[" + this.ShaderNames.Length + "];\r\n" 
                + "extern const VkVertexInputAttributeDescription " + this.Name + "Input[];\r\n" 
                + "extern const unsigned int " + this.Name + "ShaderCount;\r\n"
                + "extern const unsigned int " + this.Name + "InputCount;\r\n";
        }
    }

    class Input
    {
        public uint Id;
        public uint Offset = 0;
        public VkFormat Layout;


        public override string ToString()
        {
            return "{ " + Id + ", 0, " + Layout + "," + Offset + "}";
        }          
    }              
}