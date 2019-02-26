#pragma once

#include "util/Annotations.hpp"

USAGE_CODE
SINCE(0, 0, 4)
#define TGERROR(err) std::cout << "ERROR in " << __FILE__ << " in line " << __LINE__ << std::endl; exit(err);

//Normal error codes
#define TG_ERR_DEPTH_FORMAT_NOT_SUPPORTED -1
#define TG_ERR_SWAPCHAIN_NOT_SUPPORTED -3
//end

#define ASSERT_NONE_NULL(ptr, msg, error) if(!ptr) { std::cout << msg << std::endl; TGERROR(error) }

//Debug error codes
USAGE_DEBUG
#define TG_ERR_MESH_NULL_IN_ACTOR -1001
#define TG_ERR_FILE_NOT_FOUND_OR_NO_RIGHTS -1002
#define TG_ERR_DB_NULLPTR -1003
//end

#ifdef DEBUG
#define ASSERT_NONE_NULL_DB(ptr, msg, error) if(!ptr) { std::cout << msg << std::endl; TGERROR(error) }
#else

//Null define
#define ASSERT_NONE_NULL_DB(ptr, msg, error)
#define ASSERT_FILE(ptr, pth) 
#endif
