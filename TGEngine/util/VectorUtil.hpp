#pragma once

#include <vector>

#define TG_VECTOR_APPEND_LIST(vector, to_append, sz) last_size = vector.size();\
vector.resize(last_size + sz);\
memcpy(vector.data() + last_size, to_append, sz * sizeof(*to_append));

#define TG_VECTOR_APPEND(vector, to_append) TG_VECTOR_APPEND_LIST(vector, to_append, 1)

#define TG_VECTOR_APPEND_NORMAL(vector, to_append) last_size = vector.size();\
vector.push_back(to_append);


extern size_t last_size;