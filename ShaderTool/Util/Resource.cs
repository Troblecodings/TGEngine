using System;
using System.Collections.Generic;
using System.Text;

namespace ShaderTool.Util
{
  
    struct TextureDesc
    {
        public long offset;
        public long size;
    }

    class Resource
    {
        public List<string> texturenames = new List<string>();
        public List<TextureDesc> texturedesc = new List<TextureDesc>();
    }

    class Cache
    {
        public static Resource PRELOAD = null;
    }
}
