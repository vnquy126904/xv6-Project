#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h" // Chú ý kiến trúc RISC-V, điều chỉnh nếu khác
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "sysinfo.h"

extern struct {
    struct spinlock lock;
    uint64 loadavg[3];
} loadavg_info;

uint64
sys_sysinfo(void)
{
    struct proc *p = myproc();

    struct sysinfo info;
    uint64 info_addr; // user pointer to struct stat
    argaddr(0, &info_addr);

    info.freemem = kcollect_free();
    info.nproc = collect_proc_num();

    acquire(&loadavg_info.lock);
    info.loadavg[0] = loadavg_info.loadavg[0];
    info.loadavg[1] = loadavg_info.loadavg[1];
    info.loadavg[2] = loadavg_info.loadavg[2];
    release(&loadavg_info.lock);

    if(copyout(p->pagetable, info_addr, (char*)&info, sizeof(info)) < 0){
        return -1;
    }
    return 0;
}