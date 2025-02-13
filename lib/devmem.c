// Maps a address space and lets you read/write to it.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdint.h>

#include "devmem.h"

static int devmem_fd;

/* Request access */
void *dev_mmap(unsigned long addr, int len)
{
    void *map_base;
    off_t offset;

    devmem_fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (devmem_fd == -1)
    {
        perror("Error opening /dev/mem");
        return NULL;
    }

    offset = addr & ~(sysconf(_SC_PAGE_SIZE) - 1);
    map_base = mmap(NULL, len + addr - offset, PROT_READ | PROT_WRITE, MAP_SHARED, devmem_fd, offset);
    if (map_base == MAP_FAILED)
    {
        perror("Error running mmap");
        close(devmem_fd);
        return NULL;
    }

    return map_base + addr - offset;
}

/* Freeing mem mapped resources */
void dev_munmap(void *virt_addr, int len)
{
    uintptr_t addr;

    if (devmem_fd == -1)
    {
        printf("'/dev/mem' is already closed\n");
        return;
    }

    /* page align */
    addr = (((uintptr_t)virt_addr) & ~(sysconf(_SC_PAGE_SIZE) - 1));

    munmap((void *)addr, len + (uintptr_t)virt_addr - addr);
    
    close(devmem_fd);
}

/* Read from space */
uint32_t dev_mem_read(unsigned long addr)
{
    uint32_t val;
    void *virt_addr;

    virt_addr = dev_mmap(addr, 4);
    if (virt_addr == NULL)
    {
        printf("readl addr map failed\n");
        return 0;
    }

    val = *(uint32_t *)virt_addr;

    dev_munmap(virt_addr, 4);

    return val;
}

/* Write to space */
void dev_mem_write(unsigned long addr, uint32_t val)
{
    void *virt_addr;

    virt_addr = dev_mmap(addr, 4);
    if (virt_addr == NULL)
    {
        printf("writel addr map failed\n");
        return;
    }

    *(uint32_t *)virt_addr = val;

    dev_munmap(virt_addr, 4);
}