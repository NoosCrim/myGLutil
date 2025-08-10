#include "common.hpp"

bool mGLu::ReadTxtFile(const char* const path, std::string &out)
{
    FILE* f = fopen(path, "rb");
    if(!f)
        return false;

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    std::string text(size, '\0');
    fread(text.data(), 1, size, f);
    fclose(f);

    out = std::move(text);

    return true;
}