// Flashmap support.
//
// Copyright (C) 2019 3mdeb
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include "malloc.h" // malloc
#include "string.h" // memcpy
#include "output.h" // dprintf
#include "util.h"   // find_cb_subtable
#include "fmap.h"   // find_fmap_directory

static void* fmap_entry = NULL;
static void* rom_begin = NULL;

#define ROM_BEGIN       ((void *)0xFF800000)
#define ROM_END         ((void *)0xFFFFFFFF)

void find_fmap_directory(void)
{
    struct cb_boot_media_params *cbbmp;

    // Find coreboot table.
    struct cb_header *cbh = find_cb_table();

    if (!cbh) {
        dprintf(1, "coreboot table not found\n");
        return;
    }

    cbbmp = find_cb_subtable(cbh, CB_TAG_BOOT_MEDIA_PARAMS);

    if (!cbbmp) {
        dprintf(1, "Boot Media Params not found\n");
        return;
    }

    if (cbbmp->fmap_offset != 0 && cbbmp->boot_media_size != 0) {
        rom_begin = (void *)(ROM_END - cbbmp->boot_media_size + 1);
        fmap_entry = (void *)((u32)rom_begin + (u32)cbbmp->fmap_offset);
        dprintf(1, "FMAP found @ %p\n", fmap_entry);
    } else {
        dprintf(1, "FMAP not found\n");
    }
    dprintf(1, "FMAP not found\n");
}

int fmap_locate_area(const char *name, struct region *ar)
{
    size_t offset;

    if (!fmap_entry || !rom_begin)
        return -1;

    /* Start reading the areas just after fmap header. */
    offset = sizeof(struct fmap);

    struct fmap_area *area = malloc_tmp(sizeof(*area));
    if (!area) {
        warn_noalloc();
        return -1;
    }

    while (1) {
        iomemcpy(area, fmap_entry + offset, sizeof(*area));

        if (strcmp((const char *)area->name, name)) {
            offset += sizeof(struct fmap_area);
            continue;
        }

        dprintf(1, "FMAP: area %s found @ %p (%d bytes)\n",
                name, (void *) area->offset, area->size);

        ar->offset = (u32)rom_begin + area->offset;
        ar->size = area->size;
        free(area);
        return 0;
    }

    free(area);
    dprintf(1, "FMAP: area %s not found\n", name);

    return -1;
}
