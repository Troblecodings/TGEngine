cmake_minimum_required (VERSION 3.8)

project ("TGEngine")
set (CMAKE_CXX_STANDARD 20)

add_compile_definitions(ENGINE_NAME="TGEngine" ENGINE_VERSION=8388608 APPLICATION_NAME="TGEngineTest")

add_library (TGEngine STATIC
	# public
	"public/TGEngine.hpp"
	# private
	"private/TGEngine.cpp"
  "public/graphics/GraphicsModule.hpp" "private/graphics/GraphicsModule.cpp" "public/Module.hpp" "public/Error.hpp" "private/Error.cpp")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(TGEngineTest "test/TGTest.cpp")
target_include_directories(TGEngineTest PUBLIC "public" "../dependencies/Include" "../dependencies/Third-Party/Include")
target_link_libraries(TGEngineTest PUBLIC TGEngine gtest_main
	"${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/Third-Party/Bin/SDL2.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/../dependencies/Lib/vulkan-1.lib"
)
add_test(NAME TGEngineTest COMMAND TGEngineTest)

target_include_directories(TGEngine PUBLIC "../dependencies/Include" "../dependencies/Third-Party/Include")