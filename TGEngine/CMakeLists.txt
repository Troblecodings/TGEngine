cmake_minimum_required (VERSION 3.8)

project ("TGEngine")
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_INSTALL_PREFIX "$(pwd)/install")

add_compile_definitions(ENGINE_NAME="TGEngine" ENGINE_VERSION=8388608 APPLICATION_NAME="TGEngineTest")

add_library (TGEngine STATIC
	# public
	"public/TGEngine.hpp"
	"public/graphics/VulkanGraphicsModule.hpp"
	"public/Module.hpp"
	"public/Error.hpp"
	"public/graphics/GameGraphicsModule.hpp"
	# private
	"private/TGEngine.cpp"
	"private/Error.cpp"
    "private/graphics/VulkanGraphicsModule.cpp"     
    "private/graphics/GameGraphicsModule.cpp"
 "public/Util.hpp" "private/Util.cpp")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_Declare(
  glslang
  URL https://github.com/KhronosGroup/glslang/archive/refs/tags/master-tot.zip
  INSTALL_COMMAND "./update_glslang_sources.py"
  CONFIGURE_COMMAND "cmake . -DCMAKE_INSTALL_PREFIX=\"$(pwd)/install"
  BUILD_COMMAND "${CMAKE_COMMAND} --build . --config Release --target install"
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(glslang)

add_executable(TGEngineTest "test/TGTest.cpp")
target_include_directories(TGEngineTest PUBLIC "public" "../dependencies/Include" "../dependencies/Third-Party/Include")
target_link_libraries(TGEngineTest PUBLIC TGEngine gtest_main glslang SPIRV
	"${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/Third-Party/Bin/SDL2.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/Lib/vulkan-1.lib"
)
add_test(NAME TGEngineTest COMMAND TGEngineTest)

target_include_directories(TGEngine PUBLIC "../dependencies/Include" "../dependencies/Third-Party/Include")