#include "pm.h"
#include "mproc.h"
#include <stdio.h>

int do_getlcapid()
{ 
  pid_t pid1 = m_in.m_pids_lca.pid1;
  pid_t pid2 = m_in.m_pids_lca.pid2;
  pid_t pid_curr;
  
  int index_pid1_in_mproc = -1; //index in array mproc
  int index_pid2_in_mproc = -1;
  for (int i = 0; i < 256; i++) { //search in mproc for index of processes
    pid_curr = mproc[i].mp_pid;
    if (pid_curr == pid1 && (mproc[i].mp_flags & IN_USE)) {
      index_pid1_in_mproc = i;
    }
    if (pid_curr == pid2 && (mproc[i].mp_flags & IN_USE)) {
      index_pid2_in_mproc = i;
    }
  }
  if (index_pid1_in_mproc == -1 || index_pid2_in_mproc == -1) {
    return EINVAL;
  }
  int i1 = 0, i2 = 0; //index of array, how long to root
  int too_root_pid1[256];
  int too_root_pid2[256];
  int index_temp = index_pid1_in_mproc; //from root
  while (mproc[index_temp].mp_parent != index_temp) {
    index_temp = mproc[index_temp].mp_parent;
    pid_t parent = mproc[index_temp].mp_pid;
    too_root_pid1[i1] = parent;
    i1++;
  }
  index_temp = index_pid2_in_mproc;
  while (mproc[index_temp].mp_parent != index_temp) {
    index_temp = mproc[index_temp].mp_parent;
    pid_t parent = mproc[index_temp].mp_pid;
    too_root_pid2[i2] = parent;
    i2++;
  }

  if (i1 == 0 || i2 == 0) {
    return ESRCH;
  }
  i1--; //because array is completed from index 0 to i1-1 at too_root_pid[i1-1] is root
  i2--;
  int end = 0;
  if (too_root_pid1[i1] != too_root_pid2[i2]) {
    return -1;
  }
  while (end == 0) {
    if (i1 < 0 || i2 < 0) {
      return too_root_pid1[i1+1];
    }
    if (too_root_pid1[i1] != too_root_pid2[i2]) {
      end = 1;
    }
    else {
      i1--;
      i2--;
    }
  }
  return too_root_pid1[i1+1];
}
