#ifndef SAVESTATE_H
#define SAVESTATE_H

#include <cstdint>
#include <string>

#include "../core.h"
#include "../memfile.h"

class SaveState
{
  public:
    SaveState(Core* core) : core(core) {}
    void save(MemFile &file);
    void load(MemFile &file);

  private:
    Core* core;
    static const char *stateTag;
    static const uint32_t stateVersion;
};

#endif // SAVESTATE_H
