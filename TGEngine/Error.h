#pragma once

USAGE_CODE
SINCE(0, 0, 4)
#define TGERROR(err) exit(err);

#define DEPTH_FORMAT_NOT_SUPPORTED -1
#define FILE_NOT_FOUND_OR_NO_RIGHTS -2

#ifdef DEBUG
#define ASSERT_FILE(ptr, pth) if (!file) { std::printf(pth); TGERROR(FILE_NOT_FOUND_OR_NO_RIGHTS) }
#else
#define ASSERT_FILE(ptr, pth) 
#endif
