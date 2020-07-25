#pragma once
#ifndef FRAME_EXPORTS
#define FRAME_DLLEXPORTS __declspec(dllimport)
#else
#define FRAME_DLLEXPORTS __declspec(dllexport)
#endif