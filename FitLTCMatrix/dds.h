#pragma once

enum PixelFormat
{
    DDS_FORMAT_R16G16B16A16_FLOAT = 0,
    DDS_FORMAT_R32G32B32A32_FLOAT = 1
};

bool SaveDDS(char const* path, PixelFormat format, unsigned texelSizeInBytes, unsigned width, unsigned height, void const* data);
