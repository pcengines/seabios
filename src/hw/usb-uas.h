#ifndef __USB_UAS_H
#define __USB_UAS_H

struct disk_op_s;
int uas_process_op(struct disk_op_s *op);
struct usbdevice_s;
struct drive_s;
int usb_uas_setup(struct usbdevice_s *usbdev);
int usb_uas_present(struct drive_s *drive);

#endif /* __USB_UAS_H */
