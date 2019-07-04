/*
 * Copyright (c) 2014 The Chromium OS Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "lib_vpd.h"
#include "output.h" // dprintf

/* Given an encoded string, this functions decodes the length field which varies
 * from 1 byte to many bytes.
 *
 * The in points the actual byte going to be decoded. The *length returns
 * the decoded length field. The number of consumed bytes will be stroed in
 * decoded_len.
 *
 * Returns VPD_FAIL if more bit is 1, but actually reaches the end of string.
 */
int VPDdecodeLen(const s32 max_len,const u8 *in, s32 *length, s32 *decoded_len)
{
    u8 more;
    int i = 0;

    if (!length)
        return VPD_FAIL;

    if (!decoded_len)
        return VPD_FAIL;

    *length = 0;
    do {
        if (i >= max_len)
            return VPD_FAIL;

        more = in[i] & 0x80;
        *length <<= 7;
        *length |= in[i] & 0x7f;
        ++i;
    } while (more);

    *decoded_len = i;

    return VPD_OK;
}

/* Given the encoded string, this function invokes callback with extracted
 * (key, value). The *consumed will be plused the number of bytes consumed in
 * this function.
 *
 * The input_buf points to the first byte of the input buffer.
 *
 * The *consumed starts from 0, which is actually the next byte to be decoded.
 * It can be non-zero to be used in multiple calls.
 *
 * If one entry is successfully decoded, sends it to callback and returns the
 * result.
 */
int decodeVpdString(const s32 max_len, const u8 *input_buf, s32 *consumed,
            VpdDecodeCallback callback, void *callback_arg)
{
    int type;
    s32 key_len, value_len;
    s32 decoded_len;
    const u8 *key, *value;

    /* type */
    if (*consumed >= max_len)
        return VPD_FAIL;

    type = input_buf[*consumed];
    switch (type) {
    case VPD_TYPE_INFO:
    case VPD_TYPE_STRING:
        (*consumed)++;
        /* key */
        if (VPD_OK != VPDdecodeLen(max_len - *consumed,
                    &input_buf[*consumed], &key_len,
                    &decoded_len) ||
                *consumed + decoded_len >= max_len) {
                return VPD_FAIL;
        }

        *consumed += decoded_len;
        key = &input_buf[*consumed];
        *consumed += key_len;

        /* value */
        if (VPD_OK != VPDdecodeLen(max_len - *consumed,
                    &input_buf[*consumed],
                    &value_len, &decoded_len) ||
                    *consumed + decoded_len > max_len) {
                    return VPD_FAIL;
        }
        *consumed += decoded_len;
        value = &input_buf[*consumed];
        *consumed += value_len;

        if (type == VPD_TYPE_STRING)
            return callback(key, key_len, value, value_len,
                    callback_arg);

        return VPD_OK;

    default:
        return VPD_FAIL;
    break;
    }

    return VPD_OK;
}
