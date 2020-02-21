using System.Collections.Generic;

namespace ShaderTool.Util {

    class TextureDesc {
        public long offset = 0;
        public long size = 0;
    }

    class Resource {
        public Dictionary<string, TextureDesc> textures = new Dictionary<string, TextureDesc>();
    }

    class Cache {
        public static Resource PRELOAD = null;
        public static Dictionary<string, Command.MaterialData> MATERIALS = null;
    }
}
