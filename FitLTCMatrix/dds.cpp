#include "dds.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <cstddef>

#pragma pack(push, 1)
struct DDS_PIXELFORMAT
{
    uint32_t dwSize;
    uint32_t dwFlags;
    uint32_t dwFourCC;
    uint32_t dwRGBBitCount;
    uint32_t dwRBitMask;
    uint32_t dwGBitMask;
    uint32_t dwBBitMask;
    uint32_t dwABitMask;
};

struct DDS_HEADER
{
    uint32_t        dwSize;
    uint32_t        dwFlags;
    uint32_t        dwHeight;
    uint32_t        dwWidth;
    uint32_t        dwPitchOrLinearSize;
    uint32_t        dwDepth;
    uint32_t        dwMipMapCount;
    uint32_t        dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    uint32_t        dwCaps;
    uint32_t        dwCaps2;
    uint32_t        dwCaps3;
    uint32_t        dwCaps4;
    uint32_t        dwReserved2;
};
#pragma pack(pop)

uint32_t const DDS_MAGIC                        = 0x20534444; 
uint32_t const DDS_HEADER_FLAGS_TEXTURE         = 0x00001007; 
uint32_t const DDS_HEADER_FLAGS_PITCH           = 0x00000008;
uint32_t const DDS_SURFACE_FLAGS_TEXTURE        = 0x00001000; 
uint32_t const DDS_PF_FLAGS_FOURCC              = 0x00000004;
uint32_t const DDS_RESOURCE_DIMENSION_TEXTURE2D = 3;

DDS_PIXELFORMAT const DDSPF_RGBA16F = { sizeof(DDS_PIXELFORMAT), DDS_PF_FLAGS_FOURCC, 113, 0, 0, 0, 0, 0 };
DDS_PIXELFORMAT const DDSPF_RGBA32F = { sizeof(DDS_PIXELFORMAT), DDS_PF_FLAGS_FOURCC, 116, 0, 0, 0, 0, 0 };

DDS_PIXELFORMAT const* GetDDSPixelFormat(PixelFormat format)
{
    switch (format)
    {
        case DDS_FORMAT_R16G16B16A16_FLOAT: return &DDSPF_RGBA16F;
        case DDS_FORMAT_R32G32B32A32_FLOAT: return &DDSPF_RGBA32F;
    }

    return nullptr;
}

bool SaveDDS(char const* path, PixelFormat format, unsigned texelSizeInBytes, unsigned width, unsigned height, void const* data)
{
    FILE* f = fopen(path, "wb");
    if (!f)
        return false;

    fwrite(&DDS_MAGIC, sizeof(DDS_MAGIC), 1, f);

    DDS_PIXELFORMAT const* ddspf = GetDDSPixelFormat(format);
    if (ddspf == nullptr)
        return false;

    DDS_HEADER hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.dwSize              = sizeof(hdr);
    hdr.dwFlags             = DDS_HEADER_FLAGS_TEXTURE | DDS_HEADER_FLAGS_PITCH;
    hdr.dwHeight            = height;
    hdr.dwWidth             = width;
    hdr.dwDepth             = 1;
    hdr.dwMipMapCount       = 1;
    hdr.dwPitchOrLinearSize = width*texelSizeInBytes;
    hdr.ddspf               = *ddspf;
    hdr.dwCaps              = DDS_SURFACE_FLAGS_TEXTURE;
    fwrite(&hdr, sizeof(hdr), 1, f);

    fwrite(data, width * height * texelSizeInBytes, 1, f);

    fclose(f);

    return true;
}