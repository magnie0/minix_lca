#include <lib.h>
#include <unistd.h>
#include <string.h>
pid_t getlcapid(pid_t pid_1, pid_t pid_2)
{
    message m;

    memset(&m, 0, sizeof(m));
    m.m_pids_lca.pid1 = pid_1;
    m.m_pids_lca.pid2 = pid_2;

    return _syscall(PM_PROC_NR, PM_GETLCAPID, &m);
    
}
