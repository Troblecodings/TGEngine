#pragma once

#include <vector>

#define TG_VECTOR_APPEND_LIST(vector, to_append, sz) lastSize = vector.size();\
vector.resize(lastSize + sz);\
memcpy(vector.data() + lastSize, to_append, sz * sizeof(*to_append));

#define TG_VECTOR_APPEND(vector, to_append) TG_VECTOR_APPEND_LIST(vector, to_append, 1)

#define TG_VECTOR_APPEND_NORMAL(vector, to_append) lastSize = vector.size();\
vector.push_back(to_append);

#define TG_VECTOR_SIZE_AND_RESIZE(last_size, vector) lastSize = vector.size();\
vector.resize(lastSize + 1);

#define TG_VECTOR_GET_SIZE_AND_RESIZE(vector) lastSize = vector.size();\
vector.resize(lastSize + 1);

extern size_t lastSize;