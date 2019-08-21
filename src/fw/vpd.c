// VPD interface support.
//
// Copyright (C) 2018  Libretrend LDA
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include "bregs.h" // bregs
#include "block.h" // MAXDESCSIZE
#include "byteorder.h" // be32_to_cpu
#include "config.h" // CONFIG_*
#include "malloc.h" // free
#include "output.h" // dprintf
#include "romfile.h" // romfile_findprefix
#include "stacks.h" // yield
#include "string.h" // memcpy
#include "util.h" // find_cb_table

#include "vpd.h"
#include "lib_vpd.h"
#include "vpd_tables.h" // vpd_gets

#define CB_TAG_VPD 0x002c
#define CROSVPD_CBMEM_MAGIC 0x43524f53
#define CROSVPD_CBMEM_VERSION 0x0001
#define MAX_INPUT 60
#define ENTER_KEY 28

struct cbmem_vpd {
    u32 magic;
    u32 version;
    u32 ro_size;
    u32 rw_size;
    u8 blob[0];
    /* The blob contains both RO and RW data. It starts with RO (0 ..
     * ro_size) and then RW (ro_size .. ro_size+rw_size).
     */
};

struct cb_ref {
    u32 tag;
    u32 size;
    u64 cbmem_addr;
};

struct vpd_gets_arg {
    const u8 *key;
    const u8 *value;
    s32 key_len, value_len;
    int matched;
};

static struct cbmem_vpd* find_vpd(void)
{
    struct cb_header *cbh = find_cb_table();
    if (!cbh) {
        dprintf(1, "Unable to find coreboot table\n");
        return NULL;
    }

    struct cb_ref *cbref = find_cb_subtable(cbh, CB_TAG_VPD);
    if (!cbref) {
        dprintf(1, "Unable to find VPD table\n");
        return NULL;
    }
    struct cbmem_vpd *cb_vpd = (void*)(u32)cbref->cbmem_addr;

    dprintf(1, "VPD found @ %llx\n", cbref->cbmem_addr);
    dprintf(1, "magic: %x version: %x ro_size: %x rw_size: %x\n",
            cb_vpd->magic, cb_vpd->version, cb_vpd->ro_size, cb_vpd->rw_size);


    if ((cb_vpd->magic != CROSVPD_CBMEM_MAGIC) &&
        (cb_vpd->version != CROSVPD_CBMEM_VERSION)) {
        dprintf(1, "Wrong VPD CBMEM magic\n");
        return NULL;
    }

    if ((cb_vpd->ro_size == 0) && (cb_vpd->rw_size == 0)) {
        dprintf(1, "VPD uninitialized or empty\n");
        return NULL;
    }

    return cb_vpd;
}

static int vpd_gets_callback(const u8 *key, s32 key_len, const u8 *value,
                            s32 value_len, void *arg)
{
    struct vpd_gets_arg *result = (struct vpd_gets_arg *)arg;

    if (key_len != result->key_len ||
        memcmp(key, result->key, key_len) != 0)
        /* Returns VPD_OK to continue parsing. */
        return VPD_OK;

    result->matched = 1;
    result->value = value;
    result->value_len = value_len;
    /* Returns VPD_FAIL to stop parsing. */
    return VPD_FAIL;
}

const void *vpd_find_key(const char *key, int *size, enum vpd_region region)
{
    struct vpd_gets_arg arg = {0};
    int consumed = 0;
    const struct cbmem_vpd *vpd;

    vpd = find_vpd();
    if (!vpd)
        return NULL;

    arg.key = (const u8 *)key;
    arg.key_len = strlen(key);

    if (region == VPD_ANY || region == VPD_RO)
        while (VPD_OK == decodeVpdString(vpd->ro_size, vpd->blob,
               &consumed, vpd_gets_callback, &arg)) {
        /* Iterate until found or no more entries. */
        }

    if (!arg.matched && region != VPD_RO)
        while (VPD_OK == decodeVpdString(vpd->rw_size,
               vpd->blob + vpd->ro_size, &consumed,
               vpd_gets_callback, &arg)) {
        /* Iterate until found or no more entries. */
        }

    if (!arg.matched)
        return NULL;

    *size = arg.value_len;
    return arg.value;
}

char *vpd_gets(const char *key, char *buffer, int size, enum vpd_region region)
{
    const void *string_address;
    int string_size;

    string_address = vpd_find_key(key, &string_size, region);

    if (!string_address)
        return NULL;

    if (size > (string_size + 1)) {
        memcpy(buffer, string_address, string_size);
        buffer[string_size] = '\0';
    } else {
        memcpy(buffer, string_address, size - 1);
        buffer[size - 1] = '\0';
    }
    return buffer;
}

