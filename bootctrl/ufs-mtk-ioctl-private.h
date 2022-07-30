/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef UFS_MTK_IOCTL_PRIVATE_H_
#define UFS_MTK_IOCTL_PRIVATE_H_
enum query_opcode {
  UPIU_QUERY_OPCODE_READ_DESC = 0x1,
  UPIU_QUERY_OPCODE_READ_ATTR = 0x3,
  UPIU_QUERY_OPCODE_WRITE_ATTR = 0x4,
  UPIU_QUERY_OPCODE_READ_FLAG = 0x5,
};
enum attr_idn {
  QUERY_ATTR_IDN_BOOT_LUN_EN = 0x00,
  QUERY_ATTR_IDN_DEVICE_FFU_STATUS = 0x14,
};
enum flag_idn {
  QUERY_FLAG_IDN_PERMANENTLY_DISABLE_FW_UPDATE = 0xB,
};
enum desc_idn {
  QUERY_DESC_IDN_DEVICE = 0x0,
  QUERY_DESC_IDN_STRING = 0x5,
};
enum ufs_desc_max_size {
  QUERY_DESC_DEVICE_MAX_SIZE = 0x40,
};
enum ufs_feature_support_list {
  UFS_FEATURES_FFU = 0x1,
};
enum device_desc_param {
  DEVICE_DESC_PARAM_MANF_ID = 0x18,
  DEVICE_DESC_UFS_FEATURES_SUPPORT = 0x1F,
  DEVICE_DESC_PARAM_PRL = 0x2A,
};
#endif
