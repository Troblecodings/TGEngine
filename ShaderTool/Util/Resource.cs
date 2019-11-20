using System;
using System.Collections.Generic;
using System.Text;

namespace ShaderTool.Util
{
  
    class TextureDesc
    {
        public long offset = 0;
        public long size = 0;
    }

    class Resource
    {
        public Dictionary<string, TextureDesc> texturs = new Dictionary<string, TextureDesc>();
    }

    class Cache
    {
        public static Resource PRELOAD = null;
    }
}
