/******************************* LICENCE **************************************
* Any code in this file may be redistributed or modified under the terms of
* the GNU General Public Licence as published by the Free Software 
* Foundation; version 2 of the licence.
****************************** END LICENCE ***********************************/

/******************************************************************************
* Author:
* Andrew Smith, http://littlesvr.ca/misc/contactandrew.php
*
* Contributors:
* 
******************************************************************************/

#include <stdio.h>

#include "bkRead7x.h"
#include "bk.h"
#include "bkIoWrappers.h"

int read711(VolInfo* volInfo, unsigned char* value)
{
    return readRead(volInfo, value, 1);
}

int read721(VolInfo* volInfo, unsigned short* value)
{
    int rc;
    unsigned char array[2];
    
    rc = readRead(volInfo, array, 2);
    if(rc != 2)
        return rc;
    
    *value = array[1];
    *value <<= 8;
    *value |= array[0];
    
    return rc;
}

int read731(VolInfo* volInfo, unsigned* value)
{
    int rc;
    unsigned char array[4];
    
    rc = readRead(volInfo, array, 4);
    if(rc != 4)
        return rc;
    
    *value = array[3];
    *value <<= 8;
    *value |= array[2];
    *value <<= 8;
    *value |= array[1];
    *value <<= 8;
    *value |= array[0];
    
    return rc;
}

int read733(VolInfo* volInfo, unsigned* value)
{
    int rc;
    unsigned char both[8];
    
    rc = readRead(volInfo, both, 8);
    if(rc != 8)
        return rc;
    
    read733FromCharArray(both, value);
    
    return rc;
}

void read733FromCharArray(unsigned char* array, unsigned* value)
{
    *value = array[3];
    *value <<= 8;
    *value |= array[2];
    *value <<= 8;
    *value |= array[1];
    *value <<= 8;
    *value |= array[0];
}
