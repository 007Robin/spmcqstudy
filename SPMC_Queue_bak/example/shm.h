#pragma once
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <cstring>
#include "../SPMCQueue.h"

struct Msg
{
  uint64_t tsc;
  uint64_t i;
};

using Q = SPMCQueue<Msg, 1024>;

// To get cpu cycles
inline uint64_t rdtsc() {
  return __builtin_ia32_rdtsc();
}

// 
inline Q* shmmap(const char* filename) {
  // TODO: read https://zhuanlan.zhihu.com/p/149277008 to understand
  // why use shm_open not open
  int fd = shm_open(filename, O_CREAT | O_RDWR, 0666);
  if (fd == -1) {
    std::cerr << "shm_open failed: " << strerror(errno) << std::endl;
    return nullptr;
  }
  if (ftruncate(fd, sizeof(Q))) {
    std::cerr << "ftruncate failed: " << strerror(errno) << std::endl;
    close(fd);
    return nullptr;
  }
  // mmap return a void pointer points to an address, here we cast it to
  // Q type so it is a pointer to address of Q.
  Q* ret = (Q*)mmap(0, sizeof(Q), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  close(fd);
  if (ret == MAP_FAILED) {
    std::cerr << "mmap failed: " << strerror(errno) << std::endl;
    return nullptr;
  }
  return ret;
}
