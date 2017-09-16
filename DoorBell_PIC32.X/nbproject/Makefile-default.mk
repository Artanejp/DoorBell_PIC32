#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/main.c ../src/doorbell.c /home/whatisthis/src/DoorBell_PIC32/firmware/src/callbacks.c /home/whatisthis/src/DoorBell_PIC32/firmware/src/lm01.c ../../../../microchip/harmony/v2_04/framework/crypto/src/ecc.c ../../../../microchip/harmony/v2_04/framework/crypto/src/arc4.c ../../../../microchip/harmony/v2_04/framework/crypto/src/pwdbased.c ../../../../microchip/harmony/v2_04/framework/crypto/src/tfm.c ../../../../microchip/harmony/v2_04/framework/crypto/src/asn.c ../../../../microchip/harmony/v2_04/framework/crypto/src/des3.c ../../../../microchip/harmony/v2_04/framework/crypto/src/rsa.c ../../../../microchip/harmony/v2_04/framework/crypto/src/aes.c ../../../../microchip/harmony/v2_04/framework/crypto/src/md5.c ../../../../microchip/harmony/v2_04/framework/crypto/src/sha.c ../../../../microchip/harmony/v2_04/framework/crypto/src/sha256.c ../../../../microchip/harmony/v2_04/framework/crypto/src/sha512.c ../../../../microchip/harmony/v2_04/framework/crypto/src/hmac.c ../../../../microchip/harmony/v2_04/framework/crypto/src/hash.c ../../../../microchip/harmony/v2_04/framework/crypto/src/compress.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/adler32.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/compress.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/crc32.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/deflate.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/infback.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inffast.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inflate.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inftrees.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/trees.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/uncompr.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/zutil.c ../../../../microchip/harmony/v2_04/framework/crypto/src/crypto.c ../../../../microchip/harmony/v2_04/framework/crypto/src/coding.c ../../../../microchip/harmony/v2_04/framework/crypto/src/error.c ../../../../microchip/harmony/v2_04/framework/crypto/src/integer.c ../../../../microchip/harmony/v2_04/framework/crypto/src/logging.c ../../../../microchip/harmony/v2_04/framework/crypto/src/memory.c ../../../../microchip/harmony/v2_04/framework/crypto/src/misc.c ../../../../microchip/harmony/v2_04/framework/crypto/src/wc_port.c ../../../../microchip/harmony/v2_04/framework/crypto/src/wolfmath.c ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart.c ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_read_write.c ../../../../microchip/harmony/v2_04/framework/system/common/src/sys_queue.c ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console.c ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_uart.c ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_usb_cdc.c ../../../../microchip/harmony/v2_04/framework/system/dma/src/sys_dma.c ../../../../microchip/harmony/v2_04/framework/system/int/src/sys_int_pic32.c ../../../../microchip/harmony/v2_04/framework/system/random/src/sys_random.c ../../../../microchip/harmony/v2_04/framework/system/tmr/src/sys_tmr.c ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device.c ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs_device.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc_acm.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_remote_wakeup.c ../src/system_config/default/framework/driver/i2c/src/drv_i2c_static_buffer_model.c ../src/system_config/default/framework/driver/i2c/src/drv_i2c_mapping.c ../src/system_config/default/framework/driver/oc/src/drv_oc_mapping.c ../src/system_config/default/framework/driver/oc/src/drv_oc_static.c ../src/system_config/default/framework/driver/tmr/src/drv_tmr_static.c ../src/system_config/default/framework/driver/tmr/src/drv_tmr_mapping.c ../src/system_config/default/framework/system/clk/src/sys_clk_pic32mx.c ../src/system_config/default/framework/system/devcon/src/sys_devcon.c ../src/system_config/default/framework/system/devcon/src/sys_devcon_pic32mx.c ../src/system_config/default/framework/system/ports/src/sys_ports_static.c ../src/system_config/default/framework/system/reset/src/sys_reset.c ../src/system_config/default/framework/system/rtcc/sys_rtcc.c ../src/system_config/default/framework/system/wdt/sys_wdt.c ../src/system_config/default/system_init.c ../src/system_config/default/system_interrupt.c ../src/system_config/default/system_exceptions.c ../src/system_config/default/system_tasks.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/doorbell.o ${OBJECTDIR}/_ext/2073426327/callbacks.o ${OBJECTDIR}/_ext/2073426327/lm01.o ${OBJECTDIR}/_ext/2026377719/ecc.o ${OBJECTDIR}/_ext/2026377719/arc4.o ${OBJECTDIR}/_ext/2026377719/pwdbased.o ${OBJECTDIR}/_ext/2026377719/tfm.o ${OBJECTDIR}/_ext/2026377719/asn.o ${OBJECTDIR}/_ext/2026377719/des3.o ${OBJECTDIR}/_ext/2026377719/rsa.o ${OBJECTDIR}/_ext/2026377719/aes.o ${OBJECTDIR}/_ext/2026377719/md5.o ${OBJECTDIR}/_ext/2026377719/sha.o ${OBJECTDIR}/_ext/2026377719/sha256.o ${OBJECTDIR}/_ext/2026377719/sha512.o ${OBJECTDIR}/_ext/2026377719/hmac.o ${OBJECTDIR}/_ext/2026377719/hash.o ${OBJECTDIR}/_ext/2026377719/compress.o ${OBJECTDIR}/_ext/1803927764/adler32.o ${OBJECTDIR}/_ext/1803927764/compress.o ${OBJECTDIR}/_ext/1803927764/crc32.o ${OBJECTDIR}/_ext/1803927764/deflate.o ${OBJECTDIR}/_ext/1803927764/infback.o ${OBJECTDIR}/_ext/1803927764/inffast.o ${OBJECTDIR}/_ext/1803927764/inflate.o ${OBJECTDIR}/_ext/1803927764/inftrees.o ${OBJECTDIR}/_ext/1803927764/trees.o ${OBJECTDIR}/_ext/1803927764/uncompr.o ${OBJECTDIR}/_ext/1803927764/zutil.o ${OBJECTDIR}/_ext/2026377719/crypto.o ${OBJECTDIR}/_ext/2026377719/coding.o ${OBJECTDIR}/_ext/2026377719/error.o ${OBJECTDIR}/_ext/2026377719/integer.o ${OBJECTDIR}/_ext/2026377719/logging.o ${OBJECTDIR}/_ext/2026377719/memory.o ${OBJECTDIR}/_ext/2026377719/misc.o ${OBJECTDIR}/_ext/2026377719/wc_port.o ${OBJECTDIR}/_ext/2026377719/wolfmath.o ${OBJECTDIR}/_ext/384031356/drv_usart.o ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o ${OBJECTDIR}/_ext/1269840353/sys_queue.o ${OBJECTDIR}/_ext/660284755/sys_console.o ${OBJECTDIR}/_ext/660284755/sys_console_uart.o ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o ${OBJECTDIR}/_ext/1463730/sys_dma.o ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o ${OBJECTDIR}/_ext/1956271351/sys_random.o ${OBJECTDIR}/_ext/1329393135/sys_tmr.o ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o ${OBJECTDIR}/_ext/1388296520/usb_device.o ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o ${OBJECTDIR}/_ext/340578644/sys_devcon.o ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o ${OBJECTDIR}/_ext/822048611/sys_ports_static.o ${OBJECTDIR}/_ext/68765530/sys_reset.o ${OBJECTDIR}/_ext/476726740/sys_rtcc.o ${OBJECTDIR}/_ext/123173373/sys_wdt.o ${OBJECTDIR}/_ext/1688732426/system_init.o ${OBJECTDIR}/_ext/1688732426/system_interrupt.o ${OBJECTDIR}/_ext/1688732426/system_exceptions.o ${OBJECTDIR}/_ext/1688732426/system_tasks.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/doorbell.o.d ${OBJECTDIR}/_ext/2073426327/callbacks.o.d ${OBJECTDIR}/_ext/2073426327/lm01.o.d ${OBJECTDIR}/_ext/2026377719/ecc.o.d ${OBJECTDIR}/_ext/2026377719/arc4.o.d ${OBJECTDIR}/_ext/2026377719/pwdbased.o.d ${OBJECTDIR}/_ext/2026377719/tfm.o.d ${OBJECTDIR}/_ext/2026377719/asn.o.d ${OBJECTDIR}/_ext/2026377719/des3.o.d ${OBJECTDIR}/_ext/2026377719/rsa.o.d ${OBJECTDIR}/_ext/2026377719/aes.o.d ${OBJECTDIR}/_ext/2026377719/md5.o.d ${OBJECTDIR}/_ext/2026377719/sha.o.d ${OBJECTDIR}/_ext/2026377719/sha256.o.d ${OBJECTDIR}/_ext/2026377719/sha512.o.d ${OBJECTDIR}/_ext/2026377719/hmac.o.d ${OBJECTDIR}/_ext/2026377719/hash.o.d ${OBJECTDIR}/_ext/2026377719/compress.o.d ${OBJECTDIR}/_ext/1803927764/adler32.o.d ${OBJECTDIR}/_ext/1803927764/compress.o.d ${OBJECTDIR}/_ext/1803927764/crc32.o.d ${OBJECTDIR}/_ext/1803927764/deflate.o.d ${OBJECTDIR}/_ext/1803927764/infback.o.d ${OBJECTDIR}/_ext/1803927764/inffast.o.d ${OBJECTDIR}/_ext/1803927764/inflate.o.d ${OBJECTDIR}/_ext/1803927764/inftrees.o.d ${OBJECTDIR}/_ext/1803927764/trees.o.d ${OBJECTDIR}/_ext/1803927764/uncompr.o.d ${OBJECTDIR}/_ext/1803927764/zutil.o.d ${OBJECTDIR}/_ext/2026377719/crypto.o.d ${OBJECTDIR}/_ext/2026377719/coding.o.d ${OBJECTDIR}/_ext/2026377719/error.o.d ${OBJECTDIR}/_ext/2026377719/integer.o.d ${OBJECTDIR}/_ext/2026377719/logging.o.d ${OBJECTDIR}/_ext/2026377719/memory.o.d ${OBJECTDIR}/_ext/2026377719/misc.o.d ${OBJECTDIR}/_ext/2026377719/wc_port.o.d ${OBJECTDIR}/_ext/2026377719/wolfmath.o.d ${OBJECTDIR}/_ext/384031356/drv_usart.o.d ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o.d ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o.d ${OBJECTDIR}/_ext/1269840353/sys_queue.o.d ${OBJECTDIR}/_ext/660284755/sys_console.o.d ${OBJECTDIR}/_ext/660284755/sys_console_uart.o.d ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o.d ${OBJECTDIR}/_ext/1463730/sys_dma.o.d ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o.d ${OBJECTDIR}/_ext/1956271351/sys_random.o.d ${OBJECTDIR}/_ext/1329393135/sys_tmr.o.d ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o.d ${OBJECTDIR}/_ext/1388296520/usb_device.o.d ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o.d ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o.d ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o.d ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o.d ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o.d ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o.d ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o.d ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o.d ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o.d ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o.d ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o.d ${OBJECTDIR}/_ext/340578644/sys_devcon.o.d ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o.d ${OBJECTDIR}/_ext/822048611/sys_ports_static.o.d ${OBJECTDIR}/_ext/68765530/sys_reset.o.d ${OBJECTDIR}/_ext/476726740/sys_rtcc.o.d ${OBJECTDIR}/_ext/123173373/sys_wdt.o.d ${OBJECTDIR}/_ext/1688732426/system_init.o.d ${OBJECTDIR}/_ext/1688732426/system_interrupt.o.d ${OBJECTDIR}/_ext/1688732426/system_exceptions.o.d ${OBJECTDIR}/_ext/1688732426/system_tasks.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/doorbell.o ${OBJECTDIR}/_ext/2073426327/callbacks.o ${OBJECTDIR}/_ext/2073426327/lm01.o ${OBJECTDIR}/_ext/2026377719/ecc.o ${OBJECTDIR}/_ext/2026377719/arc4.o ${OBJECTDIR}/_ext/2026377719/pwdbased.o ${OBJECTDIR}/_ext/2026377719/tfm.o ${OBJECTDIR}/_ext/2026377719/asn.o ${OBJECTDIR}/_ext/2026377719/des3.o ${OBJECTDIR}/_ext/2026377719/rsa.o ${OBJECTDIR}/_ext/2026377719/aes.o ${OBJECTDIR}/_ext/2026377719/md5.o ${OBJECTDIR}/_ext/2026377719/sha.o ${OBJECTDIR}/_ext/2026377719/sha256.o ${OBJECTDIR}/_ext/2026377719/sha512.o ${OBJECTDIR}/_ext/2026377719/hmac.o ${OBJECTDIR}/_ext/2026377719/hash.o ${OBJECTDIR}/_ext/2026377719/compress.o ${OBJECTDIR}/_ext/1803927764/adler32.o ${OBJECTDIR}/_ext/1803927764/compress.o ${OBJECTDIR}/_ext/1803927764/crc32.o ${OBJECTDIR}/_ext/1803927764/deflate.o ${OBJECTDIR}/_ext/1803927764/infback.o ${OBJECTDIR}/_ext/1803927764/inffast.o ${OBJECTDIR}/_ext/1803927764/inflate.o ${OBJECTDIR}/_ext/1803927764/inftrees.o ${OBJECTDIR}/_ext/1803927764/trees.o ${OBJECTDIR}/_ext/1803927764/uncompr.o ${OBJECTDIR}/_ext/1803927764/zutil.o ${OBJECTDIR}/_ext/2026377719/crypto.o ${OBJECTDIR}/_ext/2026377719/coding.o ${OBJECTDIR}/_ext/2026377719/error.o ${OBJECTDIR}/_ext/2026377719/integer.o ${OBJECTDIR}/_ext/2026377719/logging.o ${OBJECTDIR}/_ext/2026377719/memory.o ${OBJECTDIR}/_ext/2026377719/misc.o ${OBJECTDIR}/_ext/2026377719/wc_port.o ${OBJECTDIR}/_ext/2026377719/wolfmath.o ${OBJECTDIR}/_ext/384031356/drv_usart.o ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o ${OBJECTDIR}/_ext/1269840353/sys_queue.o ${OBJECTDIR}/_ext/660284755/sys_console.o ${OBJECTDIR}/_ext/660284755/sys_console_uart.o ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o ${OBJECTDIR}/_ext/1463730/sys_dma.o ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o ${OBJECTDIR}/_ext/1956271351/sys_random.o ${OBJECTDIR}/_ext/1329393135/sys_tmr.o ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o ${OBJECTDIR}/_ext/1388296520/usb_device.o ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o ${OBJECTDIR}/_ext/340578644/sys_devcon.o ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o ${OBJECTDIR}/_ext/822048611/sys_ports_static.o ${OBJECTDIR}/_ext/68765530/sys_reset.o ${OBJECTDIR}/_ext/476726740/sys_rtcc.o ${OBJECTDIR}/_ext/123173373/sys_wdt.o ${OBJECTDIR}/_ext/1688732426/system_init.o ${OBJECTDIR}/_ext/1688732426/system_interrupt.o ${OBJECTDIR}/_ext/1688732426/system_exceptions.o ${OBJECTDIR}/_ext/1688732426/system_tasks.o

# Source Files
SOURCEFILES=../src/main.c ../src/doorbell.c /home/whatisthis/src/DoorBell_PIC32/firmware/src/callbacks.c /home/whatisthis/src/DoorBell_PIC32/firmware/src/lm01.c ../../../../microchip/harmony/v2_04/framework/crypto/src/ecc.c ../../../../microchip/harmony/v2_04/framework/crypto/src/arc4.c ../../../../microchip/harmony/v2_04/framework/crypto/src/pwdbased.c ../../../../microchip/harmony/v2_04/framework/crypto/src/tfm.c ../../../../microchip/harmony/v2_04/framework/crypto/src/asn.c ../../../../microchip/harmony/v2_04/framework/crypto/src/des3.c ../../../../microchip/harmony/v2_04/framework/crypto/src/rsa.c ../../../../microchip/harmony/v2_04/framework/crypto/src/aes.c ../../../../microchip/harmony/v2_04/framework/crypto/src/md5.c ../../../../microchip/harmony/v2_04/framework/crypto/src/sha.c ../../../../microchip/harmony/v2_04/framework/crypto/src/sha256.c ../../../../microchip/harmony/v2_04/framework/crypto/src/sha512.c ../../../../microchip/harmony/v2_04/framework/crypto/src/hmac.c ../../../../microchip/harmony/v2_04/framework/crypto/src/hash.c ../../../../microchip/harmony/v2_04/framework/crypto/src/compress.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/adler32.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/compress.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/crc32.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/deflate.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/infback.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inffast.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inflate.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inftrees.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/trees.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/uncompr.c ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/zutil.c ../../../../microchip/harmony/v2_04/framework/crypto/src/crypto.c ../../../../microchip/harmony/v2_04/framework/crypto/src/coding.c ../../../../microchip/harmony/v2_04/framework/crypto/src/error.c ../../../../microchip/harmony/v2_04/framework/crypto/src/integer.c ../../../../microchip/harmony/v2_04/framework/crypto/src/logging.c ../../../../microchip/harmony/v2_04/framework/crypto/src/memory.c ../../../../microchip/harmony/v2_04/framework/crypto/src/misc.c ../../../../microchip/harmony/v2_04/framework/crypto/src/wc_port.c ../../../../microchip/harmony/v2_04/framework/crypto/src/wolfmath.c ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart.c ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_read_write.c ../../../../microchip/harmony/v2_04/framework/system/common/src/sys_queue.c ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console.c ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_uart.c ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_usb_cdc.c ../../../../microchip/harmony/v2_04/framework/system/dma/src/sys_dma.c ../../../../microchip/harmony/v2_04/framework/system/int/src/sys_int_pic32.c ../../../../microchip/harmony/v2_04/framework/system/random/src/sys_random.c ../../../../microchip/harmony/v2_04/framework/system/tmr/src/sys_tmr.c ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device.c ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs_device.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc_acm.c ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_remote_wakeup.c ../src/system_config/default/framework/driver/i2c/src/drv_i2c_static_buffer_model.c ../src/system_config/default/framework/driver/i2c/src/drv_i2c_mapping.c ../src/system_config/default/framework/driver/oc/src/drv_oc_mapping.c ../src/system_config/default/framework/driver/oc/src/drv_oc_static.c ../src/system_config/default/framework/driver/tmr/src/drv_tmr_static.c ../src/system_config/default/framework/driver/tmr/src/drv_tmr_mapping.c ../src/system_config/default/framework/system/clk/src/sys_clk_pic32mx.c ../src/system_config/default/framework/system/devcon/src/sys_devcon.c ../src/system_config/default/framework/system/devcon/src/sys_devcon_pic32mx.c ../src/system_config/default/framework/system/ports/src/sys_ports_static.c ../src/system_config/default/framework/system/reset/src/sys_reset.c ../src/system_config/default/framework/system/rtcc/sys_rtcc.c ../src/system_config/default/framework/system/wdt/sys_wdt.c ../src/system_config/default/system_init.c ../src/system_config/default/system_interrupt.c ../src/system_config/default/system_exceptions.c ../src/system_config/default/system_tasks.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/doorbell.o: ../src/doorbell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/doorbell.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/doorbell.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/doorbell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1360937237/doorbell.o.d" -o ${OBJECTDIR}/_ext/1360937237/doorbell.o ../src/doorbell.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2073426327/callbacks.o: /home/whatisthis/src/DoorBell_PIC32/firmware/src/callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2073426327" 
	@${RM} ${OBJECTDIR}/_ext/2073426327/callbacks.o.d 
	@${RM} ${OBJECTDIR}/_ext/2073426327/callbacks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2073426327/callbacks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2073426327/callbacks.o.d" -o ${OBJECTDIR}/_ext/2073426327/callbacks.o /home/whatisthis/src/DoorBell_PIC32/firmware/src/callbacks.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2073426327/lm01.o: /home/whatisthis/src/DoorBell_PIC32/firmware/src/lm01.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2073426327" 
	@${RM} ${OBJECTDIR}/_ext/2073426327/lm01.o.d 
	@${RM} ${OBJECTDIR}/_ext/2073426327/lm01.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2073426327/lm01.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2073426327/lm01.o.d" -o ${OBJECTDIR}/_ext/2073426327/lm01.o /home/whatisthis/src/DoorBell_PIC32/firmware/src/lm01.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/ecc.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/ecc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/ecc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/ecc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/ecc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/ecc.o.d" -o ${OBJECTDIR}/_ext/2026377719/ecc.o ../../../../microchip/harmony/v2_04/framework/crypto/src/ecc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/arc4.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/arc4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/arc4.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/arc4.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/arc4.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/arc4.o.d" -o ${OBJECTDIR}/_ext/2026377719/arc4.o ../../../../microchip/harmony/v2_04/framework/crypto/src/arc4.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/pwdbased.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/pwdbased.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/pwdbased.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/pwdbased.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/pwdbased.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/pwdbased.o.d" -o ${OBJECTDIR}/_ext/2026377719/pwdbased.o ../../../../microchip/harmony/v2_04/framework/crypto/src/pwdbased.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/tfm.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/tfm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/tfm.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/tfm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/tfm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/tfm.o.d" -o ${OBJECTDIR}/_ext/2026377719/tfm.o ../../../../microchip/harmony/v2_04/framework/crypto/src/tfm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/asn.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/asn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/asn.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/asn.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/asn.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/asn.o.d" -o ${OBJECTDIR}/_ext/2026377719/asn.o ../../../../microchip/harmony/v2_04/framework/crypto/src/asn.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/des3.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/des3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/des3.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/des3.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/des3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/des3.o.d" -o ${OBJECTDIR}/_ext/2026377719/des3.o ../../../../microchip/harmony/v2_04/framework/crypto/src/des3.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/rsa.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/rsa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/rsa.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/rsa.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/rsa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/rsa.o.d" -o ${OBJECTDIR}/_ext/2026377719/rsa.o ../../../../microchip/harmony/v2_04/framework/crypto/src/rsa.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/aes.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/aes.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/aes.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/aes.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/aes.o.d" -o ${OBJECTDIR}/_ext/2026377719/aes.o ../../../../microchip/harmony/v2_04/framework/crypto/src/aes.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/md5.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/md5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/md5.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/md5.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/md5.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/md5.o.d" -o ${OBJECTDIR}/_ext/2026377719/md5.o ../../../../microchip/harmony/v2_04/framework/crypto/src/md5.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/sha.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/sha.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/sha.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/sha.o.d" -o ${OBJECTDIR}/_ext/2026377719/sha.o ../../../../microchip/harmony/v2_04/framework/crypto/src/sha.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/sha256.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/sha256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha256.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha256.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/sha256.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/sha256.o.d" -o ${OBJECTDIR}/_ext/2026377719/sha256.o ../../../../microchip/harmony/v2_04/framework/crypto/src/sha256.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/sha512.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/sha512.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha512.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha512.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/sha512.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/sha512.o.d" -o ${OBJECTDIR}/_ext/2026377719/sha512.o ../../../../microchip/harmony/v2_04/framework/crypto/src/sha512.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/hmac.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hmac.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hmac.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/hmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/hmac.o.d" -o ${OBJECTDIR}/_ext/2026377719/hmac.o ../../../../microchip/harmony/v2_04/framework/crypto/src/hmac.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/hash.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/hash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hash.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hash.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/hash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/hash.o.d" -o ${OBJECTDIR}/_ext/2026377719/hash.o ../../../../microchip/harmony/v2_04/framework/crypto/src/hash.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/compress.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/compress.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/compress.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/compress.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/compress.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/compress.o.d" -o ${OBJECTDIR}/_ext/2026377719/compress.o ../../../../microchip/harmony/v2_04/framework/crypto/src/compress.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/adler32.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/adler32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/adler32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/adler32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/adler32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/adler32.o.d" -o ${OBJECTDIR}/_ext/1803927764/adler32.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/adler32.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/compress.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/compress.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/compress.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/compress.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/compress.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/compress.o.d" -o ${OBJECTDIR}/_ext/1803927764/compress.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/compress.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/crc32.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/crc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/crc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/crc32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/crc32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/crc32.o.d" -o ${OBJECTDIR}/_ext/1803927764/crc32.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/crc32.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/deflate.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/deflate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/deflate.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/deflate.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/deflate.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/deflate.o.d" -o ${OBJECTDIR}/_ext/1803927764/deflate.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/deflate.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/infback.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/infback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/infback.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/infback.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/infback.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/infback.o.d" -o ${OBJECTDIR}/_ext/1803927764/infback.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/infback.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/inffast.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inffast.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inffast.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inffast.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/inffast.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/inffast.o.d" -o ${OBJECTDIR}/_ext/1803927764/inffast.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inffast.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/inflate.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inflate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inflate.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inflate.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/inflate.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/inflate.o.d" -o ${OBJECTDIR}/_ext/1803927764/inflate.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inflate.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/inftrees.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inftrees.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inftrees.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inftrees.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/inftrees.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/inftrees.o.d" -o ${OBJECTDIR}/_ext/1803927764/inftrees.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inftrees.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/trees.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/trees.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/trees.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/trees.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/trees.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/trees.o.d" -o ${OBJECTDIR}/_ext/1803927764/trees.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/trees.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/uncompr.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/uncompr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/uncompr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/uncompr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/uncompr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/uncompr.o.d" -o ${OBJECTDIR}/_ext/1803927764/uncompr.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/uncompr.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/zutil.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/zutil.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/zutil.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/zutil.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/zutil.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/zutil.o.d" -o ${OBJECTDIR}/_ext/1803927764/zutil.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/zutil.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/crypto.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/crypto.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/crypto.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/crypto.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/crypto.o.d" -o ${OBJECTDIR}/_ext/2026377719/crypto.o ../../../../microchip/harmony/v2_04/framework/crypto/src/crypto.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/coding.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/coding.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/coding.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/coding.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/coding.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/coding.o.d" -o ${OBJECTDIR}/_ext/2026377719/coding.o ../../../../microchip/harmony/v2_04/framework/crypto/src/coding.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/error.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/error.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/error.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/error.o.d" -o ${OBJECTDIR}/_ext/2026377719/error.o ../../../../microchip/harmony/v2_04/framework/crypto/src/error.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/integer.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/integer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/integer.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/integer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/integer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/integer.o.d" -o ${OBJECTDIR}/_ext/2026377719/integer.o ../../../../microchip/harmony/v2_04/framework/crypto/src/integer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/logging.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/logging.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/logging.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/logging.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/logging.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/logging.o.d" -o ${OBJECTDIR}/_ext/2026377719/logging.o ../../../../microchip/harmony/v2_04/framework/crypto/src/logging.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/memory.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/memory.o.d" -o ${OBJECTDIR}/_ext/2026377719/memory.o ../../../../microchip/harmony/v2_04/framework/crypto/src/memory.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/misc.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/misc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/misc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/misc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/misc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/misc.o.d" -o ${OBJECTDIR}/_ext/2026377719/misc.o ../../../../microchip/harmony/v2_04/framework/crypto/src/misc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/wc_port.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/wc_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wc_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wc_port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/wc_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/wc_port.o.d" -o ${OBJECTDIR}/_ext/2026377719/wc_port.o ../../../../microchip/harmony/v2_04/framework/crypto/src/wc_port.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/wolfmath.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/wolfmath.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wolfmath.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wolfmath.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/wolfmath.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/wolfmath.o.d" -o ${OBJECTDIR}/_ext/2026377719/wolfmath.o ../../../../microchip/harmony/v2_04/framework/crypto/src/wolfmath.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/384031356/drv_usart.o: ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/384031356" 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/384031356/drv_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/384031356/drv_usart.o.d" -o ${OBJECTDIR}/_ext/384031356/drv_usart.o ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o: ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/384031356" 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o.d" -o ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o: ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_read_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/384031356" 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o.d 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o.d" -o ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_read_write.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1269840353/sys_queue.o: ../../../../microchip/harmony/v2_04/framework/system/common/src/sys_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1269840353" 
	@${RM} ${OBJECTDIR}/_ext/1269840353/sys_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269840353/sys_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1269840353/sys_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1269840353/sys_queue.o.d" -o ${OBJECTDIR}/_ext/1269840353/sys_queue.o ../../../../microchip/harmony/v2_04/framework/system/common/src/sys_queue.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/660284755/sys_console.o: ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/660284755" 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console.o.d 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/660284755/sys_console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/660284755/sys_console.o.d" -o ${OBJECTDIR}/_ext/660284755/sys_console.o ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/660284755/sys_console_uart.o: ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/660284755" 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/660284755/sys_console_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/660284755/sys_console_uart.o.d" -o ${OBJECTDIR}/_ext/660284755/sys_console_uart.o ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_uart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o: ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_usb_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/660284755" 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o.d" -o ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_usb_cdc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1463730/sys_dma.o: ../../../../microchip/harmony/v2_04/framework/system/dma/src/sys_dma.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1463730" 
	@${RM} ${OBJECTDIR}/_ext/1463730/sys_dma.o.d 
	@${RM} ${OBJECTDIR}/_ext/1463730/sys_dma.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1463730/sys_dma.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1463730/sys_dma.o.d" -o ${OBJECTDIR}/_ext/1463730/sys_dma.o ../../../../microchip/harmony/v2_04/framework/system/dma/src/sys_dma.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/187263429/sys_int_pic32.o: ../../../../microchip/harmony/v2_04/framework/system/int/src/sys_int_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/187263429" 
	@${RM} ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/187263429/sys_int_pic32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/187263429/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o ../../../../microchip/harmony/v2_04/framework/system/int/src/sys_int_pic32.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1956271351/sys_random.o: ../../../../microchip/harmony/v2_04/framework/system/random/src/sys_random.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1956271351" 
	@${RM} ${OBJECTDIR}/_ext/1956271351/sys_random.o.d 
	@${RM} ${OBJECTDIR}/_ext/1956271351/sys_random.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1956271351/sys_random.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1956271351/sys_random.o.d" -o ${OBJECTDIR}/_ext/1956271351/sys_random.o ../../../../microchip/harmony/v2_04/framework/system/random/src/sys_random.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1329393135/sys_tmr.o: ../../../../microchip/harmony/v2_04/framework/system/tmr/src/sys_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1329393135" 
	@${RM} ${OBJECTDIR}/_ext/1329393135/sys_tmr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1329393135/sys_tmr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1329393135/sys_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1329393135/sys_tmr.o.d" -o ${OBJECTDIR}/_ext/1329393135/sys_tmr.o ../../../../microchip/harmony/v2_04/framework/system/tmr/src/sys_tmr.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1621444347/drv_usbfs.o: ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1621444347" 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1621444347/drv_usbfs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1621444347/drv_usbfs.o.d" -o ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o: ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1621444347" 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o.d" -o ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs_device.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc_acm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc_acm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_remote_wakeup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_remote_wakeup.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o: ../src/system_config/default/framework/driver/i2c/src/drv_i2c_static_buffer_model.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/12144542" 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o.d 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o.d" -o ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o ../src/system_config/default/framework/driver/i2c/src/drv_i2c_static_buffer_model.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o: ../src/system_config/default/framework/driver/i2c/src/drv_i2c_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/12144542" 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o.d" -o ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o ../src/system_config/default/framework/driver/i2c/src/drv_i2c_mapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o: ../src/system_config/default/framework/driver/oc/src/drv_oc_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1047219354" 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o.d" -o ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o ../src/system_config/default/framework/driver/oc/src/drv_oc_mapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1047219354/drv_oc_static.o: ../src/system_config/default/framework/driver/oc/src/drv_oc_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1047219354" 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047219354/drv_oc_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1047219354/drv_oc_static.o.d" -o ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o ../src/system_config/default/framework/driver/oc/src/drv_oc_static.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o: ../src/system_config/default/framework/driver/tmr/src/drv_tmr_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1407244131" 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o.d" -o ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o ../src/system_config/default/framework/driver/tmr/src/drv_tmr_static.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o: ../src/system_config/default/framework/driver/tmr/src/drv_tmr_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1407244131" 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o.d" -o ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o ../src/system_config/default/framework/driver/tmr/src/drv_tmr_mapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o: ../src/system_config/default/framework/system/clk/src/sys_clk_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/639803181" 
	@${RM} ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o.d" -o ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o ../src/system_config/default/framework/system/clk/src/sys_clk_pic32mx.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/340578644/sys_devcon.o: ../src/system_config/default/framework/system/devcon/src/sys_devcon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/340578644" 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/340578644/sys_devcon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/340578644/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/340578644/sys_devcon.o ../src/system_config/default/framework/system/devcon/src/sys_devcon.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o: ../src/system_config/default/framework/system/devcon/src/sys_devcon_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/340578644" 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o.d" -o ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o ../src/system_config/default/framework/system/devcon/src/sys_devcon_pic32mx.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/822048611/sys_ports_static.o: ../src/system_config/default/framework/system/ports/src/sys_ports_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/822048611" 
	@${RM} ${OBJECTDIR}/_ext/822048611/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/822048611/sys_ports_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/822048611/sys_ports_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/822048611/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/822048611/sys_ports_static.o ../src/system_config/default/framework/system/ports/src/sys_ports_static.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/68765530/sys_reset.o: ../src/system_config/default/framework/system/reset/src/sys_reset.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/68765530" 
	@${RM} ${OBJECTDIR}/_ext/68765530/sys_reset.o.d 
	@${RM} ${OBJECTDIR}/_ext/68765530/sys_reset.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/68765530/sys_reset.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/68765530/sys_reset.o.d" -o ${OBJECTDIR}/_ext/68765530/sys_reset.o ../src/system_config/default/framework/system/reset/src/sys_reset.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/476726740/sys_rtcc.o: ../src/system_config/default/framework/system/rtcc/sys_rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/476726740" 
	@${RM} ${OBJECTDIR}/_ext/476726740/sys_rtcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/476726740/sys_rtcc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/476726740/sys_rtcc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/476726740/sys_rtcc.o.d" -o ${OBJECTDIR}/_ext/476726740/sys_rtcc.o ../src/system_config/default/framework/system/rtcc/sys_rtcc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/123173373/sys_wdt.o: ../src/system_config/default/framework/system/wdt/sys_wdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/123173373" 
	@${RM} ${OBJECTDIR}/_ext/123173373/sys_wdt.o.d 
	@${RM} ${OBJECTDIR}/_ext/123173373/sys_wdt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/123173373/sys_wdt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/123173373/sys_wdt.o.d" -o ${OBJECTDIR}/_ext/123173373/sys_wdt.o ../src/system_config/default/framework/system/wdt/sys_wdt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_init.o: ../src/system_config/default/system_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_init.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_init.o ../src/system_config/default/system_init.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_interrupt.o: ../src/system_config/default/system_interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_interrupt.o ../src/system_config/default/system_interrupt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_exceptions.o: ../src/system_config/default/system_exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_exceptions.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_exceptions.o ../src/system_config/default/system_exceptions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_tasks.o: ../src/system_config/default/system_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_tasks.o ../src/system_config/default/system_tasks.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/doorbell.o: ../src/doorbell.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/doorbell.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/doorbell.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/doorbell.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1360937237/doorbell.o.d" -o ${OBJECTDIR}/_ext/1360937237/doorbell.o ../src/doorbell.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2073426327/callbacks.o: /home/whatisthis/src/DoorBell_PIC32/firmware/src/callbacks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2073426327" 
	@${RM} ${OBJECTDIR}/_ext/2073426327/callbacks.o.d 
	@${RM} ${OBJECTDIR}/_ext/2073426327/callbacks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2073426327/callbacks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2073426327/callbacks.o.d" -o ${OBJECTDIR}/_ext/2073426327/callbacks.o /home/whatisthis/src/DoorBell_PIC32/firmware/src/callbacks.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2073426327/lm01.o: /home/whatisthis/src/DoorBell_PIC32/firmware/src/lm01.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2073426327" 
	@${RM} ${OBJECTDIR}/_ext/2073426327/lm01.o.d 
	@${RM} ${OBJECTDIR}/_ext/2073426327/lm01.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2073426327/lm01.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2073426327/lm01.o.d" -o ${OBJECTDIR}/_ext/2073426327/lm01.o /home/whatisthis/src/DoorBell_PIC32/firmware/src/lm01.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/ecc.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/ecc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/ecc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/ecc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/ecc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/ecc.o.d" -o ${OBJECTDIR}/_ext/2026377719/ecc.o ../../../../microchip/harmony/v2_04/framework/crypto/src/ecc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/arc4.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/arc4.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/arc4.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/arc4.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/arc4.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/arc4.o.d" -o ${OBJECTDIR}/_ext/2026377719/arc4.o ../../../../microchip/harmony/v2_04/framework/crypto/src/arc4.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/pwdbased.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/pwdbased.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/pwdbased.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/pwdbased.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/pwdbased.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/pwdbased.o.d" -o ${OBJECTDIR}/_ext/2026377719/pwdbased.o ../../../../microchip/harmony/v2_04/framework/crypto/src/pwdbased.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/tfm.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/tfm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/tfm.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/tfm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/tfm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/tfm.o.d" -o ${OBJECTDIR}/_ext/2026377719/tfm.o ../../../../microchip/harmony/v2_04/framework/crypto/src/tfm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/asn.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/asn.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/asn.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/asn.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/asn.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/asn.o.d" -o ${OBJECTDIR}/_ext/2026377719/asn.o ../../../../microchip/harmony/v2_04/framework/crypto/src/asn.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/des3.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/des3.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/des3.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/des3.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/des3.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/des3.o.d" -o ${OBJECTDIR}/_ext/2026377719/des3.o ../../../../microchip/harmony/v2_04/framework/crypto/src/des3.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/rsa.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/rsa.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/rsa.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/rsa.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/rsa.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/rsa.o.d" -o ${OBJECTDIR}/_ext/2026377719/rsa.o ../../../../microchip/harmony/v2_04/framework/crypto/src/rsa.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/aes.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/aes.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/aes.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/aes.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/aes.o.d" -o ${OBJECTDIR}/_ext/2026377719/aes.o ../../../../microchip/harmony/v2_04/framework/crypto/src/aes.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/md5.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/md5.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/md5.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/md5.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/md5.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/md5.o.d" -o ${OBJECTDIR}/_ext/2026377719/md5.o ../../../../microchip/harmony/v2_04/framework/crypto/src/md5.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/sha.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/sha.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/sha.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/sha.o.d" -o ${OBJECTDIR}/_ext/2026377719/sha.o ../../../../microchip/harmony/v2_04/framework/crypto/src/sha.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/sha256.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/sha256.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha256.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha256.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/sha256.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/sha256.o.d" -o ${OBJECTDIR}/_ext/2026377719/sha256.o ../../../../microchip/harmony/v2_04/framework/crypto/src/sha256.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/sha512.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/sha512.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha512.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/sha512.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/sha512.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/sha512.o.d" -o ${OBJECTDIR}/_ext/2026377719/sha512.o ../../../../microchip/harmony/v2_04/framework/crypto/src/sha512.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/hmac.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/hmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hmac.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hmac.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/hmac.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/hmac.o.d" -o ${OBJECTDIR}/_ext/2026377719/hmac.o ../../../../microchip/harmony/v2_04/framework/crypto/src/hmac.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/hash.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/hash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hash.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/hash.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/hash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/hash.o.d" -o ${OBJECTDIR}/_ext/2026377719/hash.o ../../../../microchip/harmony/v2_04/framework/crypto/src/hash.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/compress.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/compress.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/compress.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/compress.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/compress.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/compress.o.d" -o ${OBJECTDIR}/_ext/2026377719/compress.o ../../../../microchip/harmony/v2_04/framework/crypto/src/compress.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/adler32.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/adler32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/adler32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/adler32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/adler32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/adler32.o.d" -o ${OBJECTDIR}/_ext/1803927764/adler32.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/adler32.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/compress.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/compress.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/compress.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/compress.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/compress.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/compress.o.d" -o ${OBJECTDIR}/_ext/1803927764/compress.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/compress.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/crc32.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/crc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/crc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/crc32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/crc32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/crc32.o.d" -o ${OBJECTDIR}/_ext/1803927764/crc32.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/crc32.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/deflate.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/deflate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/deflate.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/deflate.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/deflate.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/deflate.o.d" -o ${OBJECTDIR}/_ext/1803927764/deflate.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/deflate.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/infback.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/infback.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/infback.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/infback.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/infback.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/infback.o.d" -o ${OBJECTDIR}/_ext/1803927764/infback.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/infback.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/inffast.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inffast.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inffast.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inffast.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/inffast.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/inffast.o.d" -o ${OBJECTDIR}/_ext/1803927764/inffast.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inffast.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/inflate.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inflate.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inflate.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inflate.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/inflate.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/inflate.o.d" -o ${OBJECTDIR}/_ext/1803927764/inflate.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inflate.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/inftrees.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inftrees.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inftrees.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/inftrees.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/inftrees.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/inftrees.o.d" -o ${OBJECTDIR}/_ext/1803927764/inftrees.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/inftrees.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/trees.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/trees.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/trees.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/trees.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/trees.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/trees.o.d" -o ${OBJECTDIR}/_ext/1803927764/trees.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/trees.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/uncompr.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/uncompr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/uncompr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/uncompr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/uncompr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/uncompr.o.d" -o ${OBJECTDIR}/_ext/1803927764/uncompr.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/uncompr.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1803927764/zutil.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/zutil.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1803927764" 
	@${RM} ${OBJECTDIR}/_ext/1803927764/zutil.o.d 
	@${RM} ${OBJECTDIR}/_ext/1803927764/zutil.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1803927764/zutil.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1803927764/zutil.o.d" -o ${OBJECTDIR}/_ext/1803927764/zutil.o ../../../../microchip/harmony/v2_04/framework/crypto/src/zlib-1.2.7/zutil.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/crypto.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/crypto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/crypto.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/crypto.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/crypto.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/crypto.o.d" -o ${OBJECTDIR}/_ext/2026377719/crypto.o ../../../../microchip/harmony/v2_04/framework/crypto/src/crypto.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/coding.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/coding.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/coding.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/coding.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/coding.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/coding.o.d" -o ${OBJECTDIR}/_ext/2026377719/coding.o ../../../../microchip/harmony/v2_04/framework/crypto/src/coding.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/error.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/error.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/error.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/error.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/error.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/error.o.d" -o ${OBJECTDIR}/_ext/2026377719/error.o ../../../../microchip/harmony/v2_04/framework/crypto/src/error.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/integer.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/integer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/integer.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/integer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/integer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/integer.o.d" -o ${OBJECTDIR}/_ext/2026377719/integer.o ../../../../microchip/harmony/v2_04/framework/crypto/src/integer.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/logging.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/logging.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/logging.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/logging.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/logging.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/logging.o.d" -o ${OBJECTDIR}/_ext/2026377719/logging.o ../../../../microchip/harmony/v2_04/framework/crypto/src/logging.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/memory.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/memory.o.d" -o ${OBJECTDIR}/_ext/2026377719/memory.o ../../../../microchip/harmony/v2_04/framework/crypto/src/memory.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/misc.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/misc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/misc.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/misc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/misc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/misc.o.d" -o ${OBJECTDIR}/_ext/2026377719/misc.o ../../../../microchip/harmony/v2_04/framework/crypto/src/misc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/wc_port.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/wc_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wc_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wc_port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/wc_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/wc_port.o.d" -o ${OBJECTDIR}/_ext/2026377719/wc_port.o ../../../../microchip/harmony/v2_04/framework/crypto/src/wc_port.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/2026377719/wolfmath.o: ../../../../microchip/harmony/v2_04/framework/crypto/src/wolfmath.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2026377719" 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wolfmath.o.d 
	@${RM} ${OBJECTDIR}/_ext/2026377719/wolfmath.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2026377719/wolfmath.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/2026377719/wolfmath.o.d" -o ${OBJECTDIR}/_ext/2026377719/wolfmath.o ../../../../microchip/harmony/v2_04/framework/crypto/src/wolfmath.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/384031356/drv_usart.o: ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/384031356" 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/384031356/drv_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/384031356/drv_usart.o.d" -o ${OBJECTDIR}/_ext/384031356/drv_usart.o ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o: ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/384031356" 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o.d" -o ${OBJECTDIR}/_ext/384031356/drv_usart_buffer_queue.o ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_buffer_queue.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o: ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_read_write.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/384031356" 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o.d 
	@${RM} ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o.d" -o ${OBJECTDIR}/_ext/384031356/drv_usart_read_write.o ../../../../microchip/harmony/v2_04/framework/driver/usart/src/dynamic/drv_usart_read_write.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1269840353/sys_queue.o: ../../../../microchip/harmony/v2_04/framework/system/common/src/sys_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1269840353" 
	@${RM} ${OBJECTDIR}/_ext/1269840353/sys_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/1269840353/sys_queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1269840353/sys_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1269840353/sys_queue.o.d" -o ${OBJECTDIR}/_ext/1269840353/sys_queue.o ../../../../microchip/harmony/v2_04/framework/system/common/src/sys_queue.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/660284755/sys_console.o: ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/660284755" 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console.o.d 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/660284755/sys_console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/660284755/sys_console.o.d" -o ${OBJECTDIR}/_ext/660284755/sys_console.o ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/660284755/sys_console_uart.o: ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/660284755" 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/660284755/sys_console_uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/660284755/sys_console_uart.o.d" -o ${OBJECTDIR}/_ext/660284755/sys_console_uart.o ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_uart.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o: ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_usb_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/660284755" 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o.d" -o ${OBJECTDIR}/_ext/660284755/sys_console_usb_cdc.o ../../../../microchip/harmony/v2_04/framework/system/console/src/sys_console_usb_cdc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1463730/sys_dma.o: ../../../../microchip/harmony/v2_04/framework/system/dma/src/sys_dma.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1463730" 
	@${RM} ${OBJECTDIR}/_ext/1463730/sys_dma.o.d 
	@${RM} ${OBJECTDIR}/_ext/1463730/sys_dma.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1463730/sys_dma.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1463730/sys_dma.o.d" -o ${OBJECTDIR}/_ext/1463730/sys_dma.o ../../../../microchip/harmony/v2_04/framework/system/dma/src/sys_dma.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/187263429/sys_int_pic32.o: ../../../../microchip/harmony/v2_04/framework/system/int/src/sys_int_pic32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/187263429" 
	@${RM} ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o.d 
	@${RM} ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/187263429/sys_int_pic32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/187263429/sys_int_pic32.o.d" -o ${OBJECTDIR}/_ext/187263429/sys_int_pic32.o ../../../../microchip/harmony/v2_04/framework/system/int/src/sys_int_pic32.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1956271351/sys_random.o: ../../../../microchip/harmony/v2_04/framework/system/random/src/sys_random.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1956271351" 
	@${RM} ${OBJECTDIR}/_ext/1956271351/sys_random.o.d 
	@${RM} ${OBJECTDIR}/_ext/1956271351/sys_random.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1956271351/sys_random.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1956271351/sys_random.o.d" -o ${OBJECTDIR}/_ext/1956271351/sys_random.o ../../../../microchip/harmony/v2_04/framework/system/random/src/sys_random.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1329393135/sys_tmr.o: ../../../../microchip/harmony/v2_04/framework/system/tmr/src/sys_tmr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1329393135" 
	@${RM} ${OBJECTDIR}/_ext/1329393135/sys_tmr.o.d 
	@${RM} ${OBJECTDIR}/_ext/1329393135/sys_tmr.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1329393135/sys_tmr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1329393135/sys_tmr.o.d" -o ${OBJECTDIR}/_ext/1329393135/sys_tmr.o ../../../../microchip/harmony/v2_04/framework/system/tmr/src/sys_tmr.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1621444347/drv_usbfs.o: ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1621444347" 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o.d 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1621444347/drv_usbfs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1621444347/drv_usbfs.o.d" -o ${OBJECTDIR}/_ext/1621444347/drv_usbfs.o ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o: ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1621444347" 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o.d" -o ${OBJECTDIR}/_ext/1621444347/drv_usbfs_device.o ../../../../microchip/harmony/v2_04/framework/driver/usb/usbfs/src/dynamic/drv_usbfs_device.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc_acm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device_cdc_acm.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_cdc_acm.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o: ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_remote_wakeup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1388296520" 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o.d" -o ${OBJECTDIR}/_ext/1388296520/usb_device_remote_wakeup.o ../../../../microchip/harmony/v2_04/framework/usb/src/dynamic/usb_device_remote_wakeup.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o: ../src/system_config/default/framework/driver/i2c/src/drv_i2c_static_buffer_model.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/12144542" 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o.d 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o.d" -o ${OBJECTDIR}/_ext/12144542/drv_i2c_static_buffer_model.o ../src/system_config/default/framework/driver/i2c/src/drv_i2c_static_buffer_model.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o: ../src/system_config/default/framework/driver/i2c/src/drv_i2c_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/12144542" 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o.d" -o ${OBJECTDIR}/_ext/12144542/drv_i2c_mapping.o ../src/system_config/default/framework/driver/i2c/src/drv_i2c_mapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o: ../src/system_config/default/framework/driver/oc/src/drv_oc_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1047219354" 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o.d" -o ${OBJECTDIR}/_ext/1047219354/drv_oc_mapping.o ../src/system_config/default/framework/driver/oc/src/drv_oc_mapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1047219354/drv_oc_static.o: ../src/system_config/default/framework/driver/oc/src/drv_oc_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1047219354" 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047219354/drv_oc_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1047219354/drv_oc_static.o.d" -o ${OBJECTDIR}/_ext/1047219354/drv_oc_static.o ../src/system_config/default/framework/driver/oc/src/drv_oc_static.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o: ../src/system_config/default/framework/driver/tmr/src/drv_tmr_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1407244131" 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o.d" -o ${OBJECTDIR}/_ext/1407244131/drv_tmr_static.o ../src/system_config/default/framework/driver/tmr/src/drv_tmr_static.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o: ../src/system_config/default/framework/driver/tmr/src/drv_tmr_mapping.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1407244131" 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o.d 
	@${RM} ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o.d" -o ${OBJECTDIR}/_ext/1407244131/drv_tmr_mapping.o ../src/system_config/default/framework/driver/tmr/src/drv_tmr_mapping.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o: ../src/system_config/default/framework/system/clk/src/sys_clk_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/639803181" 
	@${RM} ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o.d" -o ${OBJECTDIR}/_ext/639803181/sys_clk_pic32mx.o ../src/system_config/default/framework/system/clk/src/sys_clk_pic32mx.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/340578644/sys_devcon.o: ../src/system_config/default/framework/system/devcon/src/sys_devcon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/340578644" 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon.o.d 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/340578644/sys_devcon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/340578644/sys_devcon.o.d" -o ${OBJECTDIR}/_ext/340578644/sys_devcon.o ../src/system_config/default/framework/system/devcon/src/sys_devcon.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o: ../src/system_config/default/framework/system/devcon/src/sys_devcon_pic32mx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/340578644" 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o.d 
	@${RM} ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o.d" -o ${OBJECTDIR}/_ext/340578644/sys_devcon_pic32mx.o ../src/system_config/default/framework/system/devcon/src/sys_devcon_pic32mx.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/822048611/sys_ports_static.o: ../src/system_config/default/framework/system/ports/src/sys_ports_static.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/822048611" 
	@${RM} ${OBJECTDIR}/_ext/822048611/sys_ports_static.o.d 
	@${RM} ${OBJECTDIR}/_ext/822048611/sys_ports_static.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/822048611/sys_ports_static.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/822048611/sys_ports_static.o.d" -o ${OBJECTDIR}/_ext/822048611/sys_ports_static.o ../src/system_config/default/framework/system/ports/src/sys_ports_static.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/68765530/sys_reset.o: ../src/system_config/default/framework/system/reset/src/sys_reset.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/68765530" 
	@${RM} ${OBJECTDIR}/_ext/68765530/sys_reset.o.d 
	@${RM} ${OBJECTDIR}/_ext/68765530/sys_reset.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/68765530/sys_reset.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/68765530/sys_reset.o.d" -o ${OBJECTDIR}/_ext/68765530/sys_reset.o ../src/system_config/default/framework/system/reset/src/sys_reset.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/476726740/sys_rtcc.o: ../src/system_config/default/framework/system/rtcc/sys_rtcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/476726740" 
	@${RM} ${OBJECTDIR}/_ext/476726740/sys_rtcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/476726740/sys_rtcc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/476726740/sys_rtcc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/476726740/sys_rtcc.o.d" -o ${OBJECTDIR}/_ext/476726740/sys_rtcc.o ../src/system_config/default/framework/system/rtcc/sys_rtcc.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/123173373/sys_wdt.o: ../src/system_config/default/framework/system/wdt/sys_wdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/123173373" 
	@${RM} ${OBJECTDIR}/_ext/123173373/sys_wdt.o.d 
	@${RM} ${OBJECTDIR}/_ext/123173373/sys_wdt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/123173373/sys_wdt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/123173373/sys_wdt.o.d" -o ${OBJECTDIR}/_ext/123173373/sys_wdt.o ../src/system_config/default/framework/system/wdt/sys_wdt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_init.o: ../src/system_config/default/system_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_init.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_init.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_init.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_init.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_init.o ../src/system_config/default/system_init.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_interrupt.o: ../src/system_config/default/system_interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_interrupt.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_interrupt.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_interrupt.o ../src/system_config/default/system_interrupt.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_exceptions.o: ../src/system_config/default/system_exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_exceptions.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_exceptions.o ../src/system_config/default/system_exceptions.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1688732426/system_tasks.o: ../src/system_config/default/system_tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1688732426" 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1688732426/system_tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1688732426/system_tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config/default" -I"../src/default" -I"../../../../microchip/harmony/v2_04/framework" -I"../src/system_config/default/framework" -I"../../../../microchip/harmony/v2_04/framework/math/libq" -MMD -MF "${OBJECTDIR}/_ext/1688732426/system_tasks.o.d" -o ${OBJECTDIR}/_ext/1688732426/system_tasks.o ../src/system_config/default/system_tasks.c    -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../microchip/harmony/v2_04/bin/framework/math/libq/libq_c_mips32_mx_l.a ../../../../microchip/harmony/v2_04/bin/framework/peripheral/PIC32MX250F128B_peripherals.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../../../../microchip/harmony/v2_04/bin/framework/math/libq/libq_c_mips32_mx_l.a ../../../../microchip/harmony/v2_04/bin/framework/peripheral/PIC32MX250F128B_peripherals.a      -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=2048,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../microchip/harmony/v2_04/bin/framework/math/libq/libq_c_mips32_mx_l.a ../../../../microchip/harmony/v2_04/bin/framework/peripheral/PIC32MX250F128B_peripherals.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ../../../../microchip/harmony/v2_04/bin/framework/math/libq/libq_c_mips32_mx_l.a ../../../../microchip/harmony/v2_04/bin/framework/peripheral/PIC32MX250F128B_peripherals.a      -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--gc-sections,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/DoorBell_PIC32.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
