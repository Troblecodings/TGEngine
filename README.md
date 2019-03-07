<h1>TGEngine</h1>

<h3>What is the TGEngine?</h3>

It should be or better to say it will be a Game Engine, hopefully...

<h3>Status</h3>

still working on it...<br>
For updates please follow the [official twitter account of Troblecodings](https://twitter.com/Troblecodings)

<h3>Build your own</h3>

To compile your own build you have to install the dependencies.<br>
To do so you need tu run <strong>python setup.py</strong>.<br>
There you have to select <strong>1 Get dependencies</strong><br>
Now you can open the VisualStudios Project file and compile the Engine.

<h3>Build Status</h3>

| Platform | Build Status |
|:--------:|:------------:|
| Windows (Visual Studio 2017) | [![Windows Build status](https://ci.appveyor.com/api/projects/status/xkqlankoj873h3xh?svg=true)](https://ci.appveyor.com/project/MrTroble/tgengine) |
| Linux | ![Linux Build Status](https://img.shields.io/badge/build-not%20passing-red.svg) |

<h3>Libraries</h3>

I am using [LunarG's Vulkan-SDK](https://vulkan.lunarg.com/sdk/home).<br>
Current built version: <strong>1.1.101</strong><br>
Current Vulkan API version: <strong>1.1</strong>
<br>
<br>
and Autodesk [FBX SDK](https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-2)<br>
Current built version: <strong>2019.2</strong>
<br>
<br>
For image loading and font loading is [STB](https://github.com/nothings/stb) currently being used.<br>
This is planned to be replaced in the future.<br>
*SIDE NOTE: The file library [TGVL](https://github.com/Troblecodings/TGVertex) maybe needed in the future to compile newer versions of the TGEngine 
but currently it is not supported!*

<h3>License</h3>

Can be found in the [License File](https://github.com/MrTroble/TGEngine/blob/master/LICENSE)
<br>
<h4>Library</h4>

Every library used is Open-Source or free to use.<br>
<br>
STB is published under a [Public domain](https://github.com/nothings/stb) License (or MIT)<br>
LunarG's Vulkan-SDK is published under [Creative Commons](https://creativecommons.org/licenses/by-nd/4.0/), see [LunarG.com](https://vulkan.lunarg.com/doc/sdk/1.1.82.0/windows/getting_started.html)<br>
Vulkan itself is distributed under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0), see Vulkan.h<br>
The FBX Model loader library SDK from Autodesk is [Licensed](http://download.autodesk.com/us/fbx/2019/2019.0/FBX%20SDK%202019%20About%20Box%20Final.pdf) under there owne license.

All other brand names, product names or trademarks belong to their respective holders.

<h3>Wanna contribute?</h3>

Everyone is welcome to help in development!<br>
I am thankful for every improvment, addition or fix.

***Please review the [Code of Conduct](https://github.com/MrTroble/TGEngine/blob/master/CODE_OF_CONDUCT.md) before contributing in anyway to the repository!***

Additional information can be found [here](https://github.com/MrTroble/TGEngine/blob/master/CONTRIBUTING.md)
