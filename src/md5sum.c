/*
 * Calc/Check MD5 HASH SUM.
 * (C) 2017 Kyuma Ohta <whatisthis.sowhat _at_ gmail.com >
 * This is part of "DoorBell_PIC32" . You must have MPLAB HARMONY to use this routines.
 * License : Apache OSS License.
 */

#include "doorbell.h"

extern DOORBELL_DATA doorbellData;

// These are specified to DoorBell_PIC32.
void CALC_MD5Sum(void)
{
    unsigned int datasize = sizeof (DOORBELL_REAL_DATA_T);
    CRYPT_MD5_Initialize(&(doorbellData.md5_context));
    CRYPT_MD5_DataSizeSet(&(doorbellData.md5_context), datasize);
    CRYPT_MD5_DataAdd(&(doorbellData.md5_context), (const unsigned char*) &(doorbellData.realdata), datasize);
    CRYPT_MD5_Finalize(&(doorbellData.md5_context), doorbellData.data_md5sum);
}

bool CHECK_MD5Sum(void)
{
    unsigned char tmpmd5[MD5_DIGEST_SIZE];
    unsigned int datasize = sizeof (DOORBELL_REAL_DATA_T);
    CRYPT_MD5_Initialize(&(doorbellData.md5_context));
    CRYPT_MD5_DataSizeSet(&(doorbellData.md5_context), datasize);
    CRYPT_MD5_DataAdd(&(doorbellData.md5_context), (const unsigned char*) &(doorbellData.realdata), datasize);
    CRYPT_MD5_Finalize(&(doorbellData.md5_context), tmpmd5);
    if (memcmp(tmpmd5, doorbellData.data_md5sum, MD5_DIGEST_SIZE) != 0) {
        return false;
    }
    return true;
}
