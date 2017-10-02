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

#include <time.h>

#include "bkInternal.h"
#include "bkMisc.h"

void maybeUpdateProgress(VolInfo* volInfo)
{
    struct timeb timeNow;
    
    if(volInfo->progressFunction == NULL)
        return;
    
    ftime(&timeNow);
    
    if(timeNow.time - volInfo->lastTimeCalledProgress.time >= 1 ||
       timeNow.millitm - volInfo->lastTimeCalledProgress.millitm >= 100)
    {
        volInfo->progressFunction(volInfo);
        
        volInfo->lastTimeCalledProgress = timeNow;
    }
}
