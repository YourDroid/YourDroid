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
#include <sys/types.h>
#include <sys/stat.h>

#include "bkInternal.h"
#include "bkIoWrappers.h"

void bkClose(int file)
{
    close(file);
}

int bkFstat(int file, BkStatStruct* statStruct)
{
    return fstat(file, statStruct);
}

size_t bkRead(int file, void* dest, size_t numBytes)
{
    return read(file, dest, numBytes);
}

/******************************************************************************
* bkSeekSet()
* */
bk_off_t bkSeekSet(int file, bk_off_t offset, int origin)
{
    return lseek(file, offset, origin);
}

/******************************************************************************
* bkSeekTell()
* */
bk_off_t bkSeekTell(int file)
{
    return lseek(file, 0, SEEK_CUR);
}

int bkStat(const char* pathAndName, BkStatStruct* statStruct)
{
    return stat(pathAndName, statStruct);
}

size_t bkWrite(int file, const void* src, size_t numBytes)
{
    return write(file, src, numBytes);
}

size_t readRead(VolInfo* volInfo, void* dest, size_t numBytes)
{
    return read(volInfo->imageForReading, dest, numBytes);
}

/******************************************************************************
* readSeekSet()
* Seek set for reading from the iso
* */
bk_off_t readSeekSet(VolInfo* volInfo, bk_off_t offset, int origin)
{
    return lseek(volInfo->imageForReading, offset, origin);
}

/******************************************************************************
* readSeekTell()
* Seek tell for reading from the iso
* */
bk_off_t readSeekTell(VolInfo* volInfo)
{
    return lseek(volInfo->imageForReading, 0, SEEK_CUR);
}
