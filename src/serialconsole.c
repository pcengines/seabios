// serial keyboard handler
//
// Copyright (C) 2014  Eltan B.V.
//
// This file may be distributed under the terms of the GNU LGPLv3 license.

#include "util.h" // kbc_enqueue_key

#if CONFIG_SEABIOS_SERIAL_CONSOLE
static u8 UartToScanCode[] VAR16 = {
//    x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xa    xb    xc    xd    xe    xf
//  =====================================================================================================
    0x0f, 0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, // 0x
    0x19, 0x10, 0x13, 0x1f, 0x14, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c, 0x01, 0x00, 0x00, 0x00, 0x00, // 1x
    0x39, 0x02, 0x28, 0x04, 0x05, 0x06, 0x08, 0x28, 0x0a, 0x0b, 0x09, 0x0d, 0x33, 0x0c, 0x34, 0x35, // 2x
    0x00, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x27, 0x27, 0x33, 0x0d, 0x34, 0x35, // 3x
    0x03, 0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, // 4x
    0x19, 0x10, 0x13, 0x1f, 0x14, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c, 0x1a, 0x2b, 0x1b, 0x07, 0x0c, // 5x
    0x29, 0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, // 6x
    0x19, 0x10, 0x13, 0x1f, 0x14, 0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c, 0x1a, 0x2b, 0x1b, 0x29, 0x00, // 7x
};

void
uart_keyboard_handler(void)
{
    u8 rx_buf[5], rx_bytes = 0, ascii_code = 0, scan_code = 0;

    // check to see if there is a active serial port
    if (inb(CONFIG_DEBUG_SERIAL_PORT + SEROFF_LSR) == 0xFF)
        return;

    while (inb(CONFIG_DEBUG_SERIAL_PORT + SEROFF_LSR) & 0x01) {
        if (rx_bytes > sizeof(rx_buf)) {
            dprintf(1, "uart_check_keystrokes: error too many bytes are available\n");
            while (inb(CONFIG_DEBUG_SERIAL_PORT + SEROFF_LSR) & 0x01)
                inb(CONFIG_DEBUG_SERIAL_PORT + SEROFF_DATA);
            return;
       }
       else
           rx_buf[rx_bytes++] = inb(CONFIG_DEBUG_SERIAL_PORT + SEROFF_DATA);
    }

    if (!rx_bytes)
        return;

    if (rx_bytes == 1) {
        ascii_code = rx_buf[0];
        if (ascii_code >= ARRAY_SIZE(UartToScanCode)) {
            dprintf(3, "uart_check_keystrokes: error UartToScanCode out of bounds index\n");
            return;
        }
        scan_code = GET_GLOBAL(UartToScanCode[ascii_code]);
        kbc_enqueue_key(scan_code, ascii_code);
    }
    else if (rx_bytes == 2) { // assume it's actually 2 single-byte keystrokes
        ascii_code = rx_buf[0];
        if (ascii_code >= ARRAY_SIZE(UartToScanCode)) {
            dprintf(3, "uart_check_keystrokes: error in 1/2 byte UartToScanCode out of bounds index\n");
            return;
        }
        scan_code = GET_GLOBAL(UartToScanCode[ascii_code]);
        kbc_enqueue_key(scan_code, ascii_code);

        ascii_code = rx_buf[1];
        if (ascii_code >= ARRAY_SIZE(UartToScanCode)) {
            dprintf(3, "uart_check_keystrokes: error in 2/2 UartToScanCode out of bounds index\n");
            return;
        }
        scan_code = GET_GLOBAL(UartToScanCode[ascii_code]);
        kbc_enqueue_key(scan_code, ascii_code);
    }
    else if (rx_bytes == 3) {
        if ((rx_buf[0] == 0x1b) && (rx_buf[1] == 0x4f)) { // F1-F12
            ascii_code = 0;
            if ((rx_buf[2] >= 0x50) && (rx_buf[2] <= 0x59))
                scan_code = (rx_buf[2] - 0x50)  +0x3b;
            else if ((rx_buf[2] >= 0x5a) && (rx_buf[2] <= 0x5b))
                scan_code = (rx_buf[2] - 0x5a) + 0x85;
            else {
                dprintf(3, "uart_check_keystrokes: error in Fkey handling %x\n",rx_buf[2]);
                return;
            }
        }
        else if ((rx_buf[0] == 0x1b) && (rx_buf[1] == 0x5b)) { // cursor keys
            ascii_code = 0xe0;
            if      (rx_buf[2] == 0x41) scan_code = 0x48; // UP
            else if (rx_buf[2] == 0x42) scan_code = 0x50; // DOWN
            else if (rx_buf[2] == 0x43) scan_code = 0x4d; // LEFT
            else if (rx_buf[2] == 0x44) scan_code = 0x4b; // RIGHT
            else {
                dprintf(3, "uart_check_keystrokes: error in cursor handling %x\n",rx_buf[2]);
                return;
            }
        }
        else {
            dprintf(3, "uart_check_keystrokes: error in 3 byte key sequence\n");
            return;
        }
        kbc_enqueue_key(scan_code, ascii_code);
    }
    else if (rx_bytes == 4) {
        if ((rx_buf[0] == 0x1b) && (rx_buf[1] == 0x5b) && (rx_buf[2] == 0x33) && (rx_buf[3] == 0x7e)) { // DEL
            ascii_code = 0xe0;
            scan_code = 0x53;
            kbc_enqueue_key(scan_code, ascii_code);
        }
        else {
            dprintf(3, "uart_check_keystrokes: unhandled 4 byte keystroke ");
            dprintf(3, "%x %x %x %x\n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3]);
            return;
        }
    }
    /* these 5 byte scan codes are used by some terminal emulators */
    else if (rx_bytes == 5) {
        if ((rx_buf[0] == 0x1b) && (rx_buf[1] == 0x5b) && (rx_buf[2] == 0x32) && (rx_buf[4] == 0x7e)) { // F9-F12
            ascii_code = 0x00;
            if      (rx_buf[3] == 0x30) scan_code = 0x43; // F9
            else if (rx_buf[3] == 0x31) scan_code = 0x44; // F10
            else if (rx_buf[3] == 0x33) scan_code = 0x85; // F11
            else if (rx_buf[3] == 0x34) scan_code = 0x86; // F12
            else {
                dprintf(3, "uart_check_keystrokes: unhandled 5 byte keystroke F9-F12 ");
                dprintf(3, "%x %x %x %x %x\n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);
                return;
            }
        }
        else if ((rx_buf[0] == 0x1b) && (rx_buf[1] == 0x5b) && (rx_buf[2] == 0x31) && (rx_buf[4] == 0x7e)) { // F1-F8
            ascii_code = 0x00;
            if      (rx_buf[3] == 0x31) scan_code = 0x3B; // F1
            else if (rx_buf[3] == 0x32) scan_code = 0x3C; // F2
            else if (rx_buf[3] == 0x33) scan_code = 0x3D; // F3
            else if (rx_buf[3] == 0x34) scan_code = 0x3E; // F4
            else if (rx_buf[3] == 0x35) scan_code = 0x3F; // F5
            else if (rx_buf[3] == 0x37) scan_code = 0x40; // F6
            else if (rx_buf[3] == 0x38) scan_code = 0x41; // F7
            else if (rx_buf[3] == 0x39) scan_code = 0x42; // F8
            else {
                dprintf(3, "uart_check_keystrokes: unhandled 5 byte keystroke F1-F8 ");
                dprintf(3, "%x %x %x %x %x\n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);
                return;
            }
        }
        else {
            dprintf(3, "uart_check_keystrokes: unhandled 5 byte keystroke ");
            dprintf(3, "%x %x %x %x %x\n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);
            return;
        }
        kbc_enqueue_key(scan_code, ascii_code);
    }
    else {
        dprintf(3, "uart_check_keystrokes: unhandled rx_bytes = %x\n",rx_bytes);
        dprintf(3, "%x %x %x %x %x\n",rx_buf[0],rx_buf[1],rx_buf[2],rx_buf[3],rx_buf[4]);
        return;
    }
}
#endif /* CONFIG_SEABIOS_SERIAL_CONSOLE */
