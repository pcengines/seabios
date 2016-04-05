// Access to pseudo "file" interface for configuration information.
//
// Copyright (C) 2012  Kevin O'Connor <kevin@koconnor.net>
// Copyright (C) 2015  Eltan B.V.
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include "config.h" // CONFIG_*
#include "malloc.h" // free
#include "output.h" // dprintf
#include "romfile.h" // struct romfile_s
#include "string.h" // memcmp
#include "fw/coreboot.h" // cbfs_romfile overrides

static struct romfile_s *RomfileRoot VARVERIFY32INIT;

void
romfile_add(struct romfile_s *file)
{
    dprintf(3, "Add romfile: %s (size=%d)\n", file->name, file->size);
    file->next = RomfileRoot;
    RomfileRoot = file;
}

// Search for the specified file.
static struct romfile_s *
__romfile_findprefix(const char *prefix, int prefixlen, struct romfile_s *prev)
{
    struct romfile_s *cur = RomfileRoot;
    if (prev)
        cur = prev->next;
    while (cur) {
        if (memcmp(prefix, cur->name, prefixlen) == 0) {

        	int dont_hide;
        	char *data = get_cbmem_file( (char *) cur->name, &dont_hide );	// Check if we should hide this
        	if (!data) 		return cur;	// no override for this file
        	if (dont_hide) 	return cur;	// We know it and shouldn't hide
        }
        cur = cur->next;
    }
    return NULL;
}

struct romfile_s *
romfile_findprefix(const char *prefix, struct romfile_s *prev)
{
    return __romfile_findprefix(prefix, strlen(prefix), prev);
}

struct romfile_s *
romfile_find(const char *name)
{
    return __romfile_findprefix(name, strlen(name) + 1, NULL);
}

// Helper function to find, malloc_tmphigh, and copy a romfile.  This
// function adds a trailing zero to the malloc'd copy.
void *
romfile_loadfile(const char *name, int *psize)
{
    char *data = get_cbmem_file( (char *) name, psize );			// First try to read a file override from cbmem

    if ( data ) {

        if ( !*psize ) {

            dprintf(3, "Hiding romfile '%s'\n", name);
        	return NULL;						// Hide the file
        }
    	return data;
    }

    struct romfile_s *file = romfile_find(name);
    if (!file)
        return NULL;

    int filesize = file->size;
    if (!filesize)
        return NULL;

    data = malloc_tmphigh(filesize+1);
    if (!data) {
        warn_noalloc();
        return NULL;
    }

    dprintf(5, "Copying romfile '%s' (len %d)\n", name, filesize);
    int ret = file->copy(file, data, filesize);
    if (ret < 0) {
        free(data);
        return NULL;
    }
    if (psize)
        *psize = filesize;
    data[filesize] = '\0';
    return data;
}

// Attempt to load an integer from the given file - return 'defval'
// if unsuccessful.
u64
romfile_loadint(const char *name, u64 defval)
{
    int  size;
	char *data = get_cbmem_file( (char *) name, &size );			// First try to read a file override from cbmem

    if ( data ) {

        u64 val = 0;

        switch (size){

        	case 1:
        	case 2:
        	case 4:
        	case 8:
                memcpy(&val, data, size);
        		break;

        	default:
                return defval;
        }
    	return val;
    }

    struct romfile_s *file = romfile_find(name);
    if (!file)
        return defval;

    int filesize = file->size;
    if (!filesize || filesize > sizeof(u64) || (filesize & (filesize-1)))
        // Doesn't look like a valid integer.
        return defval;

    u64 val = 0;
    int ret = file->copy(file, &val, sizeof(val));
    if (ret < 0)
        return defval;
    return val;
}
