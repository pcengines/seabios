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

#define ROM_BEGIN       ((void *)0xFF800000)
#define ROM_END         ((void *)0xFFFFFFFF)

void find_fmap_directory(void)
{
    void *offset = ROM_BEGIN;

    while (offset <= ROM_END) {
        if (!memcmp(offset, FMAP_SIGNATURE, 8)) {
                fmap_entry = offset;
                dprintf(1, "FMAP found @ %p\n", fmap_entry);
                return;
            }
        /* Currently FMAP signature is assumed to be 0x100 bytes aligned */
        offset += 0x100;
    }
    dprintf(1, "FMAP not found\n");
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

        dprintf(1, "FMAP: area %s found @ %p (%d bytes)\n",
                name, (void *) area->offset, area->size);

        ar->offset = (u32)ROM_BEGIN + area->offset;
        ar->size = area->size;
        free(area);
        return 0;
    }

    free(area);
    dprintf(1, "FMAP: area %s not found\n", name);

    return -1;
}
