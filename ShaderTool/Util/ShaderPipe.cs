namespace ShaderTool
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
            string ShaderModule = "new VkPipelineShaderStageCreateInfo[]{";
            foreach (string str in ShaderNames) {
                ShaderModule += str + ",";
            }
            ShaderModule = ShaderModule.Substring(0, ShaderModule.Length - 1) + "}";

            string Input = "new VkVertexInputAttributeDescription[]{";
            foreach (Input str in Inputs)
            {
                Input += str + ",";
            }
            Input = Input.Substring(0, Input.Length - 1) + "}";

            return ShaderModule + "," + ShaderNames.Length + "," + Input + "," + Inputs.Length;
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