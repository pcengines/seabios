// Flashmap support.
//
// Copyright (C) 2019 3mdeb
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include "malloc.h" // malloc
#include "string.h" // memcpy
#include "output.h" // dprintf
#include "fmap.h"   // find_fmap_directory

static void* fmap_entry = NULL;

void find_fmap_directory(void)
{
    void *rom_begin = 0xFF800000;
    struct fmap *fmap;

    fmap = (struct fmap *)rom_begin;

    while (fmap < 0xFFFFFFE0) {
        if (!memcmp(fmap->signature, FMAP_SIGNATURE,
            sizeof(fmap->signature))) {
                fmap_entry = (void *)fmap;
                dprintf(1, "FMAP found @ 0x%p\n", fmap_entry);
                break;
            }
        fmap += 0x16;
    }
}

int fmap_locate_area(const char *name, struct region *ar)
{
    size_t offset;

    if (!fmap_entry)
        return -1;

    /* Start reading the areas just after fmap header. */
    offset = sizeof(struct fmap);

    while (1) {
        struct fmap_area *area = malloc_tmp(sizeof(*area));
        if (!area) {
            warn_noalloc();
            break;
        }
        iomemcpy(area, fmap_entry + offset, sizeof(*area));

        if (area == NULL)
            return -1;

        if (strcmp((const char *)area->name, name)) {
            free(area);
            offset += sizeof(struct fmap_area);
            continue;
        }

        dprintf(1, "FMAP: area %s found @ 0x%p (%d bytes)\n",
               name, area->offset, area->size);

        ar->offset = area->offset;
        ar->size = area->size;

        return 0;
    }

    printk(BIOS_DEBUG, "FMAP: area %s not found\n", name);

    return -1;
}