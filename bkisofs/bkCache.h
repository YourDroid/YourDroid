int wcSeekForward(VolInfo* volInfo, bk_off_t numBytes);
int wcSeekSet(VolInfo* volInfo, bk_off_t position);
bk_off_t wcSeekTell(VolInfo* volInfo);
int wcWrite(VolInfo* volInfo, const char* block, size_t numBytes);
