#ifndef MEMFILE_H
#define MEMFILE_H

#include <sstream>
#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdint>

#include "defines.h"

class MemFile
{
    public:
        struct Error {};
        bool fast = false;
        MemFile() : stream(), file(nullptr) {}
        MemFile(void *data, size_t size) : file(nullptr), source(static_cast<const uint8_t*>(data)),
            target(static_cast<uint8_t*>(data)), capacity(size), memory(true) {}
        MemFile(const void *data, size_t size) : file(nullptr), source(static_cast<const uint8_t*>(data)),
            capacity(size), memory(true) {}
        ~MemFile() { close(); }

        MemFile(FILE* cfile) : stream(), file(cfile)
        {
            if (!file) return;

            fseek(file, 0, SEEK_END);
            long fileSize = ftell(file);
            fseek(file, 0, SEEK_SET);

            if (fileSize > 0)
            {
              std::vector<char> content(fileSize);
              fread(content.data(), 1, fileSize, file);

              stream.write(content.data(), fileSize);
              stream.seekg(0, std::ios::beg);
              stream.seekp(0, std::ios::beg);
            }
        }

        bool opened() const
        {
            return file != nullptr;
        }

        size_t write(const void* buffer, size_t size, size_t count)
        {
            if (memory) {
                if (!target || (size && count > (capacity - position) / size)) throw Error();
                if (size && count) memcpy(target + position, buffer, size * count);
                position += size * count;
                return count;
            }
            stream.write(static_cast<const char*>(buffer), size * count);
            return size;
        }

        size_t read(void* buffer, size_t size, size_t count)
        {
            if (memory) {
                if (!source || (size && count > (capacity - position) / size)) throw Error();
                if (size && count) memcpy(buffer, source + position, size * count);
                position += size * count;
                return count;
            }
            stream.read(static_cast<char*>(buffer), size * count);
            return size;
        }

        int seek(long offset, int origin)
        {
            if (memory) {
                int64_t next = offset;
                if (origin == SEEK_CUR) next += position;
                else if (origin == SEEK_END) next += capacity;
                else if (origin != SEEK_SET) throw Error();
                if (next < 0 || uint64_t(next) > capacity) throw Error();
                position = next;
                return 0;
            }
            std::ios_base::seekdir dir;

            switch (origin)
            {
                case SEEK_SET: dir = std::ios::beg; break;
                case SEEK_CUR: dir = std::ios::cur; break;
                case SEEK_END: dir = std::ios::end; break;
                default: return -1;
            }

            stream.seekg(offset, dir);
            stream.seekp(offset, dir);

            return stream.fail() ? -1 : 0;
        }

        long tell()
        {
            if (memory) return position;
            return static_cast<long>(stream.tellg());
        }

        void close()
        {
            if (!file) return;

            std::string content = stream.str();
            fwrite(content.data(), 1, content.size(), file);
            fclose(file);

            file = nullptr;
        }

    private:
        std::stringstream stream;
        FILE* file;
        const uint8_t *source = nullptr;
        uint8_t *target = nullptr;
        size_t capacity = 0, position = 0;
        bool memory = false;
};

FORCE_INLINE size_t fread(void* buffer, size_t size, size_t count, MemFile &file)
{
    return file.read(buffer, size, count);
}

FORCE_INLINE size_t fwrite(const void* buffer, size_t size, size_t count, MemFile &file)
{
    return file.write(buffer, size, count);
}

FORCE_INLINE int fseek(MemFile &file, long offset, int origin)
{
    return file.seek(offset, origin);
}

FORCE_INLINE long ftell(MemFile &file)
{
    return file.tell();
}

FORCE_INLINE void fclose(MemFile &file)
{
    return file.close();
}

#endif // MEMFILE_H
