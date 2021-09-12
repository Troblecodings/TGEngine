cmake_minimum_required (VERSION 3.8)

project ("TGEngine")
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

add_compile_definitions(ENGINE_NAME="TGEngine" ENGINE_VERSION=8388608 APPLICATION_NAME="TGEngineTest" DEBUG)

add_compile_options("/D_ITERATOR_DEBUG_LEVEL=0" "/MD" "/EHa")
add_link_options("/ignore:4099")

include(FetchContent)
FETCHCONTENT_DECLARE(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
message("Downloading vulkansdk")
FETCHCONTENT_DECLARE(
	vulkansdk
	URL https://seafile.media-dienste.de/f/85da9d3e98b347a490f6/?dl=1

	BUILD_COMMAND ""
)

FETCHCONTENT_DECLARE(
	imgui
	URL https://github.com/ocornut/imgui/archive/refs/tags/v1.83.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FETCHCONTENT_MAKEAVAILABLE(googletest)
FETCHCONTENT_MAKEAVAILABLE(vulkansdk)
FETCHCONTENT_MAKEAVAILABLE(imgui)
include_directories("${vulkansdk_SOURCE_DIR}/Include" "${vulkansdk_SOURCE_DIR}/Third-Party/Include" "${imgui_SOURCE_DIR}")
link_directories("${vulkansdk_SOURCE_DIR}/Lib" "${vulkansdk_SOURCE_DIR}/Third-Party/Bin")

add_library (TGEngine STATIC
	# public
	"public/TGEngine.hpp"
	"public/graphics/VulkanGraphicsModule.hpp"
	"public/Module.hpp"
	"public/Error.hpp"
	"public/graphics/GameGraphicsModule.hpp"
	"public/Util.hpp"
	# private
	"private/TGEngine.cpp"
	"private/Error.cpp"
    "private/graphics/VulkanGraphicsModule.cpp"     
    "private/graphics/GameGraphicsModule.cpp"
    "private/Util.cpp"
	"private/graphics/WindowModule.cpp" 
	"private/graphics/VulkanShaderModule.cpp" 
	"public/graphics/VulkanShaderModule.hpp" 
	"public/graphics/VulkanShaderPipe.hpp" 
	"public/graphics/GameShaderModule.hpp" 
	"private/graphics/GUIModule.cpp" 
	"public/graphics/GUIModule.hpp" 
	"public/graphics/VulkanModuleDef.hpp"
	"${imgui_SOURCE_DIR}/imconfig.h"
	"${imgui_SOURCE_DIR}/imgui.cpp"
	"${imgui_SOURCE_DIR}/imgui.h"
	"${imgui_SOURCE_DIR}/imgui_demo.cpp"
	"${imgui_SOURCE_DIR}/imgui_draw.cpp"
	"${imgui_SOURCE_DIR}/imgui_internal.h"
	"${imgui_SOURCE_DIR}/imgui_tables.cpp"
	"${imgui_SOURCE_DIR}/imgui_widgets.cpp"
	"${imgui_SOURCE_DIR}/imstb_rectpack.h"
	"${imgui_SOURCE_DIR}/imstb_textedit.h"
	"${imgui_SOURCE_DIR}/imstb_truetype.h"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h"
	"${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h"
)


add_executable(TGEngineTest "test/TGTest.cpp")
target_include_directories(TGEngineTest PUBLIC "public")
add_custom_target(CopyAssetsToOutput SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/assets/
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/test/assets/ $<TARGET_FILE_DIR:TGEngineTest>/assets)
target_link_libraries(TGEngineTest PUBLIC TGEngine gtest_main glslang SPIRV SPIRV-Tools SPIRV-Tools-opt shaderc_combined OSDependent MachineIndependent OGLCompiler SDL2 vulkan-1)
add_test(NAME TGEngineTest COMMAND TGEngineTest)

