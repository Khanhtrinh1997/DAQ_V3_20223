1. Toolchain supported
===================
- IAR embedded Workbench  9.20.4


2. IAR Config
===================
Copy and replace file "FlashIMXRT1050_EVK_FlexSPI.out" to:
C:\Program Files\IAR Systems\Embedded Workbench 9.0\arm\config\flashloader\NXP


3. DAQV3 Debug, or Build Config
===================
If debug, set macro: XIP_BOOT_HEADER_ENABLE=1
If build and flash via JFlash, set macro: XIP_BOOT_HEADER_ENABLE=0, and flash Bootloader.hex file.


