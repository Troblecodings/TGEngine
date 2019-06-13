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
    }

    class Input
    {
        public uint Id;
        public uint Offset = 0;
        public VkFormat Layout;


        public override string ToString()
        {
            return "Input[id=" + Id + ",Offset=" + Offset + ",Layout=" + Layout.ToString();
        }
    }

}