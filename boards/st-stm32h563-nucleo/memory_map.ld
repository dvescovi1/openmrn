/* Minimal linker script placeholder for STM32H563 (flash/ram sizes must be tailored) */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2048K
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 640K
}

SECTIONS
{
  .text :
  {
    *(.isr_vector)
    *(.text*)
    *(.rodata*)
  } > FLASH

  .data : AT (ADDR(.text) + SIZEOF(.text))
  {
    *(.data*)
  } > RAM

  .bss :
  {
    *(.bss*)
    *(COMMON)
  } > RAM
}
