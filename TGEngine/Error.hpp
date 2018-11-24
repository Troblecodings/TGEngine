#pragma once

#include "util/Annotations.hpp"

USAGE_CODE
SINCE(0, 0, 4)
#define TGERROR(err) exit(err);

//Normal error codes
#define DEPTH_FORMAT_NOT_SUPPORTED -1
#define FILE_NOT_FOUND_OR_NO_RIGHTS -2
#define SWAPCHAIN_NOT_SUPPORTED -3
//end

#ifdef DEBUG

//Debug error codes
USAGE_DEBUG
#define MESH_NULL_IN_ACTOR -1001
//end

#define ASSERT_NONE_NULL(ptr, msg, error) if(!ptr) { std::cout << msg << std::endl; TGERROR(error) }
#define ASSERT_FILE(ptr, pth) if (!file) { std::cout << pth << std::endl; TGERROR(FILE_NOT_FOUND_OR_NO_RIGHTS) }

#else
// Null define
#define MESH_NULL_IN_ACTOR

#define ASSERT_NONE_NULL(ptr, msg, error)
#define ASSERT_FILE(ptr, pth) 
#endif
