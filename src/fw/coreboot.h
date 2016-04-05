//*****************************************************************************
//
// Copyright (c) 2015 Eltan B.V.  All rights reserved.
// Software License Agreement
//
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// ELTAN SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, 
// OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
//
//*****************************************************************************

#ifndef __COREBOOT_H
#define __COREBOOT_H

struct cbmem_entry {
	u32 magic;
	u32 id;
	u64 base;
	u64 size;
};

//
// Intended for cbfs file overrides
//
struct file_container {
    u32  file_signature;   /* "FILE" */
    u32  file_size;        /* size of file_data[] */
    char file_name[32];
    char file_data[0];     /* Variable size */
};

#define CBMEM_ID_FILE  			0x46494c45 //'FILE'
#define CB_TAG_FILE 			0x25
#define CBMEM_MAGIC         	0x434f5245

struct cbfile_record {
    u32 tag;
    u32 size;
    u64 forward;
};
char * get_cbmem_file( char * filename, int * size );

#endif	//__COREBOOT_H
