
﻿namespace ShaderTool.Command
{
    class ShaderPipe
    {
        public string Name;
        public string[] ShaderNames;
        public Input[] Inputs;
        public Descriptor[] Descriptors;

        public ShaderPipe(string Name, string[] ShaderNames, Input[] Inputs, Descriptor[] Descriptors)
        {
            this.Name = Name;
            this.ShaderNames = (string[])ShaderNames.Clone();
            this.Inputs = (Input[])Inputs.Clone();
            this.Descriptors = (Descriptor[])Descriptors.Clone();
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

            string Desc = "const VkDescriptorSetLayoutBinding " + this.Name + "LayoutBinding[] = {";
            foreach (Descriptor str in Descriptors)
            {
                Desc += str + ",";
            }
            Desc = Desc.Substring(0, Desc.Length - 1) + "};\r\n";

            return ShaderModule + Input + Desc
                + "const unsigned int " + this.Name + "ShaderCount = " + ShaderNames.Length + ";\r\n" 
                + "const unsigned int " + this.Name + "InputCount = " + Inputs.Length + ";\r\n"
                + "const unsigned int " + this.Name + "LayoutBindingCount = " + Descriptors.Length + ";\r\n"
                + "ShaderPipe " + this.Name + "Pipe;\r\n";
        }

        public string GenCreation()
        {
            string ShaderModule = "";
            uint i = 0;
            foreach (string str in ShaderNames)
            {
                ShaderModule += "    " + this.Name + "Shader[" + i++ + "] = " + str + ";\r\n";
            }
            ShaderModule += "    " + this.Name + "Pipe = ShaderPipe(" + this.Name + "Shader, " + this.Name + "Input, " + this.Name 
                + "LayoutBinding, " + this.Name + "ShaderCount, " + this.Name + "InputCount, " + this.Name + "LayoutBindingCount)";
            return ShaderModule;
        }

        public string GenHeader()
        {
            return "extern VkPipelineShaderStageCreateInfo " + this.Name + "Shader[" + this.ShaderNames.Length + "];\r\n" 
                + "extern const VkVertexInputAttributeDescription " + this.Name + "Input[];\r\n"
                + "extern const VkDescriptorSetLayoutBinding " + this.Name + "LayoutBinding[];\r\n"
                + "extern const unsigned int " + this.Name + "ShaderCount;\r\n"
                + "extern const unsigned int " + this.Name + "InputCount;\r\n"
                + "extern const unsigned int " + this.Name + "LayoutBindingCount;\r\n"
                + "extern ShaderPipe " + this.Name + "Pipe;\r\n";
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

    class Descriptor
    {
        public uint Binding;
        public VkDescriptorType Type;
        public VkShaderStageFlagBits flag;

        public override string ToString()
        {
            return "{ " + Binding + ", " + Type + ", 1," + flag + "}";
        }
    }
}