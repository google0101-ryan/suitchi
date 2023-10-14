#include "nro.h"

std::vector<uint8_t> buf;

Suitchi::result<Executable> NROLoader::LoadNRO(std::string name)
{
    std::ifstream file(name, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        return Suitchi::error(Errors::FILE_NOT_FOUND);
    }

    size_t size = file.tellg();
    buf.resize(size);
    file.seekg(0, std::ios::beg);
    file.read((char*)buf.data(), size);
    file.close();

    header = *(NROHeader*)buf.data();

    if (strncmp((char*)&header.magic, "NRO0", 4) != 0)
    {
        return Suitchi::error(Errors::INVALID_MAGIC);
    }

    auto code_hdr = header.executable[0];
    auto code = std::span<uint8_t>(buf.begin()+code_hdr.offset, code_hdr.size);

    auto ro_hdr = header.executable[1];
    auto ro = std::span<uint8_t>(buf.begin()+ro_hdr.offset, ro_hdr.size);

    auto data_hdr = header.executable[2];
    auto data = std::span<uint8_t>(buf.begin()+data_hdr.offset, data_hdr.size);

    printf("NRO Info:\n");
    printf("\tLoaded code from 0x%08x to 0x%08x\n", code_hdr.offset, code_hdr.offset+code_hdr.size);
    printf("\tLoaded ro from 0x%08x to 0x%08x\n", ro_hdr.offset, ro_hdr.offset+ro_hdr.size);
    printf("\tLoaded data from 0x%08x to 0x%08x\n", data_hdr.offset, data_hdr.offset+data_hdr.size);

    Executable exe;
    exe.code.hdr = code_hdr;
    exe.code.mem = code;
    exe.ro.hdr = ro_hdr;
    exe.ro.mem = ro;
    exe.data.hdr = data_hdr;
    exe.data.mem = data;

    return exe;
}