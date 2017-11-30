#pragma once

#ifdef TGE_EXPORTS
#define TGE __declspec(dllexport)
#else
#define TGE __declspec(libexport)
#endif