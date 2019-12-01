//
// Created by yuvalgelles on 19/11/2019.
//
using namespace std;

#ifndef SMASH_DEFINES_H
#define SMASH_DEFINES_H

#include <unistd.h>
#include <wait.h>
#include <sstream>

//#define DEBUG_MODE
#ifdef DEBUG_MODE
#define debug(str) std::cout<<str<<std::endl
#else
#define debug(str)
#endif

#endif //SMASH_DEFINES_H
