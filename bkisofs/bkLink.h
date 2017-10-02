int addToHardLinkTable(VolInfo* volInfo, bk_off_t position, char* pathAndName, 
                       unsigned size, bool onImage, BkHardLink** newLink);
int filesAreSame(VolInfo* volInfo, int file1, bk_off_t posFile1, 
                 int file2, bk_off_t posFile2, unsigned size);
int findInHardLinkTable(VolInfo* volInfo, bk_off_t position, 
                        char* pathAndName, unsigned size,
                        bool onImage, BkHardLink** foundLink);
int readFileHead(VolInfo* volInfo, bk_off_t position, char* pathAndName, 
                 bool onImage, unsigned char* dest, int numBytes);
void resetWriteStatus(BkHardLink* fileLocations);
