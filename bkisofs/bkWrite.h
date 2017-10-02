#ifndef bkwrite_h
#define bkwrite_h

int bootInfoTableChecksum(int oldImage, FileToWrite* file, unsigned* checksum);
int countDirsOnLevel(const DirToWrite* dir, int targetLevel, int thisLevel);
int countTreeHeight(const DirToWrite* dir, int heightSoFar);
unsigned short elToritoChecksum(const unsigned char* record);
int writeByteBlock(VolInfo* volInfo, unsigned char byteToWrite, int numBytes);
int writeByteBlockFromFile(int src, VolInfo* volInfo, unsigned numBytes);
int writeDir(VolInfo* volInfo, DirToWrite* dir, int parentLbNum, 
             int parentNumBytes, int parentPosix, time_t recordingTime, 
             int filenameTypes, bool isRoot);
int writeDr(VolInfo* volInfo, BaseToWrite* dir, time_t recordingTime, bool isADir, 
            bool isSelfOrParent, bool isFirstRecord, int filenameTypes);
int writeElToritoBootCatalog(VolInfo* volInfo, 
                             bk_off_t* bootRecordSectorNumberOffset);
int writeElToritoVd(VolInfo* volInfo, bk_off_t* bootCatalogSectorNumberOffset);
int writeFileContents(VolInfo* volInfo, DirToWrite* dir, int filenameTypes);
int writeJolietStringField(VolInfo* volInfo, const char* name, size_t fieldSize);
int writeLongNM(VolInfo* volInfo, BaseToWrite* dir);
int writeLongNMsInDir(VolInfo* volInfo, DirToWrite* dir);
int writePathTable(VolInfo* volInfo, const DirToWrite* tree, bool isTypeL, 
                   int filenameType);
int writePathTableRecordsOnLevel(VolInfo* volInfo, const DirToWrite* dir, 
                                 bool isTypeL, int filenameType, 
                                 int targetLevel, int thisLevel,
                                 int* parentDirNum);
int writeRockER(VolInfo* volInfo);
int writeRockNM(VolInfo* volInfo, char* name, size_t nameLen, bool doesContinue);
int writeRockPX(VolInfo* volInfo, unsigned posixFileMode, bool isADir);
int writeRockSL(VolInfo* volInfo, SymLinkToWrite* symlink, bool doWrite);
int writeRockSP(VolInfo* volInfo);
int writeVdsetTerminator(VolInfo* volInfo);
int writeVolDescriptor(VolInfo* volInfo, bk_off_t rootDrLocation,
                       unsigned rootDrSize, bk_off_t lPathTableLoc, 
                       bk_off_t mPathTableLoc, unsigned pathTableSize, 
                       time_t creationTime, bool isPrimary);
int wroteIsolinuxBootRecord(VolInfo* volInfo, FileToWrite* file, 
                            bool* isIsolinux);

#endif
