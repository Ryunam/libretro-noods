#include "savestate.h"

#include <cstring>

const char* SaveState::stateTag = "NDSR";
const uint32_t SaveState::stateVersion = 5;

void SaveState::save(MemFile &file)
{
  core->gpu.stopThread();
  core->gpu3DRenderer.finishFrame();
  file.write(stateTag, 1, 4);
  file.write(&stateVersion, sizeof(stateVersion), 1);
  core->saveState(file);
  core->memory.saveState(file);
  core->cartridgeGba.saveState(file);
  core->cartridgeNds.saveState(file);
  core->cp15.saveState(file);
  core->divSqrt.saveState(file);
  core->dma[0].saveState(file);
  core->dma[1].saveState(file);
  core->gpu.saveState(file);
  core->gpu2D[0].saveState(file);
  core->gpu2D[1].saveState(file);
  core->gpu3D.saveState(file);
  core->gpu3DRenderer.saveState(file);
  core->hleArm7.saveState(file);
  core->hleBios[0].saveState(file);
  core->hleBios[1].saveState(file);
  core->hleBios[2].saveState(file);
  core->interpreter[0].saveState(file);
  core->interpreter[1].saveState(file);
  core->ipc.saveState(file);
  core->rtc.saveState(file);
  core->spi.saveState(file);
  core->spu.saveState(file);
  core->timers[0].saveState(file);
  core->timers[1].saveState(file);
  core->wifi.saveState(file);
  core->input.saveState(file);
}

void SaveState::load(MemFile &file)
{
  char tag[4];
  uint32_t version;
  file.read(tag, 1, 4);
  file.read(&version, sizeof(version), 1);
  if (memcmp(tag, stateTag, 4) || version != stateVersion) throw MemFile::Error();
  core->gpu.stopThread();
  core->gpu3DRenderer.finishFrame();
  bool dsiMode = core->dsiMode, gbaMode = core->gbaMode;
  core->loadState(file);
  core->memory.loadState(file, dsiMode != core->dsiMode || gbaMode != core->gbaMode);
  core->cartridgeGba.loadState(file);
  core->cartridgeNds.loadState(file);
  core->cp15.loadState(file);
  core->divSqrt.loadState(file);
  core->dma[0].loadState(file);
  core->dma[1].loadState(file);
  core->gpu.loadState(file);
  core->gpu2D[0].loadState(file);
  core->gpu2D[1].loadState(file);
  core->gpu3D.loadState(file);
  core->gpu3DRenderer.loadState(file);
  core->hleArm7.loadState(file);
  core->hleBios[0].loadState(file);
  core->hleBios[1].loadState(file);
  core->hleBios[2].loadState(file);
  core->interpreter[0].loadState(file);
  core->interpreter[1].loadState(file);
  core->ipc.loadState(file);
  core->rtc.loadState(file);
  core->spi.loadState(file);
  core->spu.loadState(file);
  core->timers[0].loadState(file);
  core->timers[1].loadState(file);
  core->wifi.loadState(file);
  core->input.loadState(file);
  core->updateRun();
}
