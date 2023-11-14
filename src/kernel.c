#include "odi/odi.h"
#include "odi/core/debug.h"

#include "bootservices/bootservices.h"

#include "arch/simd.h"
#include "arch/gdt.h"
#include "io/interrupts.h"
#include "memory/memory.h"
#include "memory/paging.h"
#include "memory/heap.h"
#include "util/printf.h"

#define STR_MAX_SIZE 65536

void _start(void) {
    init_simd();
    init_memory();
    init_paging();
    init_heap();
    init_gdt();
    init_interrupts();
    
    odi_autoconf((void*)get_rsdp_address());

    odi_debug_list_devices();

    u8 buffer[512] = {0};
    odi_device_t hd = odi_open("hd0");
    if (hd == NULL) {
        printf("Failed to open hd0\n");
        while(1);
    }

    u64 size = odi_read(hd, 0, 1, buffer);
    printf("Read %d sectors from hd0\n", size);

    for (int i = 0; i < 512; i++) {
        printf("%2x ", buffer[i]);
        if (i % 16 == 15) {
            printf("\n");
        }
    }
    printf("\n");

    while(1);
}