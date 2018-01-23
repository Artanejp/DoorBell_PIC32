/*
 * Calc/Check MD5 HASH SUM.
 * (C) 2017 Kyuma Ohta <whatisthis.sowhat _at_ gmail.com >
 * This is part of "DoorBell_PIC32" . You must have MPLAB HARMONY to use this routines.
 * License : Apache OSS License.
 */

#include "doorbell.h"

extern DOORBELL_DATA *getDoorbellData(void);

// These are specified to DoorBell_PIC32.
void CALC_MD5Sum(void)
{
    DOORBELL_DATA *pdd = getDoorbellData();
    if(pdd == NULL) return;
    unsigned int datasize = sizeof (DOORBELL_REAL_DATA_T);
    CRYPT_MD5_Initialize(&(pdd->md5_context));
    CRYPT_MD5_DataSizeSet(&(pdd->md5_context), datasize);
    CRYPT_MD5_DataAdd(&(pdd->md5_context), (const unsigned char*) &(pdd->realdata), datasize);
    CRYPT_MD5_Finalize(&(pdd->md5_context), pdd->data_md5sum);
}

bool CHECK_MD5Sum(void)
{
    DOORBELL_DATA *pdd = getDoorbellData();
    if(pdd == NULL) return false;
    unsigned char tmpmd5[MD5_DIGEST_SIZE];
    unsigned int datasize = sizeof (DOORBELL_REAL_DATA_T);
    CRYPT_MD5_Initialize(&(pdd->md5_context));
    CRYPT_MD5_DataSizeSet(&(pdd->md5_context), datasize);
    CRYPT_MD5_DataAdd(&(pdd->md5_context), (const unsigned char*) &(pdd->realdata), datasize);
    CRYPT_MD5_Finalize(&(pdd->md5_context), tmpmd5);
    if (memcmp(tmpmd5, pdd->data_md5sum, MD5_DIGEST_SIZE) != 0) {
        return false;
    }
    return true;
}
