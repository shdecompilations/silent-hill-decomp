#include "pc/fs/pc_fs.h"
#ifdef PC_PORT
#include <stdio.h>
// Tim loader — TIM is Sony's texture format (libgpu TIM)
// On PSX this uploads to VRAM; on PC we convert to s_PcTexture
void Fs_QueueStartReadTimFile(e_FsFile file, int buf, void* desc) {
    PcFs_QueueStartReadTim(file, buf, desc);
}
#endif
