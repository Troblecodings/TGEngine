cmake_minimum_required (VERSION 3.8)

project ("TGEngine")
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

add_compile_definitions(ENGINE_NAME="TGEngine" ENGINE_VERSION=8388608 APPLICATION_NAME="TGEngineTest" DEBUG)

add_compile_options("/D_ITERATOR_DEBUG_LEVEL=0" "/MD" "/EHa")
add_link_options("/ignore:4099")

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
)

include(FetchContent)
FETCHCONTENT_DECLARE(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
  CMAKE_ARGS "-DCMAKE_MSVC_RUNTIME_LIBRARY MultiThreaded"
)
message("Downloading vulkansdk")
FETCHCONTENT_DECLARE(
	vulkansdk
	URL https://seafile.media-dienste.de/f/85da9d3e98b347a490f6/?dl=1

	BUILD_COMMAND ""
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FETCHCONTENT_MAKEAVAILABLE(googletest)
FETCHCONTENT_MAKEAVAILABLE(vulkansdk)
include_directories("${vulkansdk_SOURCE_DIR}/Include" "${vulkansdk_SOURCE_DIR}/Third-Party/Include")
link_directories("${vulkansdk_SOURCE_DIR}/Lib" "${vulkansdk_SOURCE_DIR}/Third-Party/Bin")

add_executable(TGEngineTest "test/TGTest.cpp")
target_include_directories(TGEngineTest PUBLIC "public")
add_custom_command(TARGET TGEngineTest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/test/shader/ $<TARGET_FILE_DIR:TGEngineTest>)
target_link_libraries(TGEngineTest PUBLIC TGEngine gtest_main glslang SPIRV SPIRV-Tools SPIRV-Tools-opt shaderc_combined OSDependent MachineIndependent OGLCompiler SDL2 vulkan-1)
add_test(NAME TGEngineTest COMMAND TGEngineTest)

