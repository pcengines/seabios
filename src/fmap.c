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

#define ROM_END         ((void *)0xFFFFFFFF)

#define CB_TAG_BOOT_MEDIA_PARAMS 0x0030

struct cb_boot_media_params {
	u32 tag;
	u32 size;
	/* offsets are relative to start of boot media */
	u64 fmap_offset;
	u64 cbfs_offset;
	u64 cbfs_size;
	u64 boot_media_size;
};

void find_fmap_directory(void)
{
    struct cb_boot_media_params *cbbmp;
    
    cbbmp = find_cb_subtable(cbh, CB_TAG_BOOT_MEDIA_PARAMS);

    if (!cbbmp) {
        dprintf(1, "Boot Media Params not found\n");
        return;
    }

    if (cbbmp->fmap_offset != 0 && cbbmp->boot_media_size != 0) {
        fmap_entry  = (void *)(ROM_END - cbbmp->boot_media_size + 1);
        fmap_entry += (void *)cbbmp->fmap_offset;
        dprintf(1, "FMAP found @ %p\n", fmap_entry);
    } else {
        dprintf(1, "FMAP not found\n");
    }
}

int fmap_locate_area(const char *name, struct region *ar)
{
    size_t offset;

    if (!fmap_entry)
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

        ar->offset = (u32)ROM_BEGIN + area->offset;
        ar->size = area->size;
        free(area);
        return 0;
    }

    free(area);
    dprintf(1, "FMAP: area %s not found\n", name);

    return -1;
}
