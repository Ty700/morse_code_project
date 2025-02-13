#ifndef DEVMEM_H
#define DEVMEM_H

#include <stdint.h>

void *dev_mmap(unsigned long addr, int len);
void dev_munmap(void *virt_addr, int len);

uint32_t dev_mem_read(unsigned long addr);
void dev_mem_write(unsigned long addr, uint32_t val);

#endif // DEVMEM_H