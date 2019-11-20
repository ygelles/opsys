//
// Created by yuvalgelles on 19/11/2019.
//

#ifndef SMASH_DEFINES_H
#define SMASH_DEFINES_H

#include <unistd.h>


#define DEBUG_MODE
#ifdef DEBUG_MODE
#define debug(str) std::cout<<str<<std::endl
#else
#define debug(str)
#endif

#define errorMsgSys(sys) perror("smash error:" sys "failed")
#define errorMsg(fn,msg) perror("smash error: " fn ":" msg)
#endif //SMASH_DEFINES_H
