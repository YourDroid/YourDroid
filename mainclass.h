#ifndef MAINCLASS_H
#define MAINCLASS_H
#define OS 1 // 0 - linux, 1 - windows
#include <string>

class mainclass
{
    bool tbios;
    bool arch;
    const bool os = OS;
#if OS == 1
    bool winv;
    const char *pathconf = "yourdroid\\config.dat";
    const char *pathset = "yourdroid";
#elif OS ==0
    const QString pathconf = "yourdroid/config.dat";
    const QString pathset = "yourdroid";
#endif
public:
    mainclass();
    int exec(int, char**);
    void setting();
    bool defbios();
    bool defarch();
#if OS == 1
    bool defwinv();
#endif
};

#endif // MAINCLASS_H
