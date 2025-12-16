MEMORY
{
  FLASH      (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  BOOTLOADER (rx)  : ORIGIN = 0x08100000, LENGTH = 512K
  SPIFFS     (r)   : ORIGIN = 0x08180000, LENGTH = 512K
  RAM        (rwx) : ORIGIN = 0x20000000, LENGTH = 640K
}

__flash_start = ORIGIN(FLASH);
__flash_end = ORIGIN(FLASH) + LENGTH(FLASH);
__flash_fs_start = ORIGIN(SPIFFS);
__flash_fs_sector_start = 8;
__flash_fs_end = ORIGIN(SPIFFS) + LENGTH(SPIFFS);
__bootloader_start = ORIGIN(BOOTLOADER);
__app_header_offset = 0x270;
__bootloader_magic_ptr = ORIGIN(RAM);
