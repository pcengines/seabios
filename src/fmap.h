// Flashmap support.
//
// Copyright (C) 2019 3mdeb
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#ifndef _FMAP_H
#define _FMAP_H

#define FMAP_SIGNATURE		"__FMAP__"
#define FMAP_VER_MAJOR		1	/* this header's FMAP minor version */
#define FMAP_VER_MINOR		1	/* this header's FMAP minor version */
#define FMAP_STRLEN		32	/* maximum length for strings, */
					/* including null-terminator */

enum fmap_flags {
    FMAP_AREA_STATIC		= 1 << 0,
    FMAP_AREA_COMPRESSED	= 1 << 1,
    FMAP_AREA_RO            = 1 << 2,
    FMAP_AREA_PRESERVE		= 1 << 3,
};

/* Mapping of volatile and static regions in firmware binary */
struct fmap_area {
    u32 offset;			/* offset relative to base */
    u32 size;			/* size in bytes */
    u8  name[FMAP_STRLEN];	/* descriptive name */
    u16 flags;			/* flags for this area */
} PACKED;

struct fmap {
    u8  signature[8];		/* "__FMAP__" (0x5F5F464D41505F5F) */
    u8  ver_major;		/* major version */
    u8  ver_minor;		/* minor version */
    u64 base;			/* address of the firmware binary */
    u32 size;			/* size of firmware binary in bytes */
    u8  name[FMAP_STRLEN];	/* name of this firmware binary */
    u16 nareas;			/* number of areas described by
				fmap_areas[] below */
    struct fmap_area areas[];
} PACKED;

struct region {
	size_t offset;
	size_t size;
};

void find_fmap_directory(void);
int fmap_locate_area(const char *name, struct region *ar);

#endif
