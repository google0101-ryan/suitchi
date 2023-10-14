#pragma once

#include "util/defs.h"


struct SegmentHeader
{
    uint32_t offset;
    uint32_t size;
};

struct Section
{
    SegmentHeader hdr;
    std::span<uint8_t> mem;
};

struct Executable
{
    Section code, ro, data;
};

class NROLoader
{
private:

    struct NROHeader
    {
        uint32_t unused0;
        uint32_t mod0_offs;
        uint64_t unused1;

        uint32_t magic;
        uint32_t version;
        uint32_t size;
        uint32_t flags;
        SegmentHeader executable[3];

        uint32_t bss_size;
        uint32_t unused2;
        char mod_id[0x20];
        uint32_t dso_handle_offs;
        uint32_t unused3;
        SegmentHeader dyninfo[3];
    } header;
public:
    Suitchi::result<Executable> LoadNRO(std::string name);
};