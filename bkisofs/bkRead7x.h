/*******************************************************************************
* bkRead7x
* functions to read simple variables as described in sections 7.x of iso9660
* not including filenames (7.4, 7.5, 7.6)
* 
* if they are stored in both byte orders, the appropriate one is read into
* the parameter but the return is 2x the size of that variable
*
* */

#include "bk.h"

int read711(VolInfo* volInfo, unsigned char* value);
int read721(VolInfo* volInfo, unsigned short* value);
int read731(VolInfo* volInfo, unsigned* value);
int read733(VolInfo* volInfo, unsigned* value);
void read733FromCharArray(unsigned char* array, unsigned* value);
