//
// Created by vance on 2016/8/6.
//

#ifndef NATIVEIMAGEMERGE_INC_H
#define NATIVEIMAGEMERGE_INC_H

#include <stdio.h>
#include <jni.h>
#include <vector>

#define _DEBUG
#include "log.h"

// debug performance
#ifdef _DEBUG
#include <time.h>
#define TIME_BEGIN clock_t __begin = clock()
#define TIME_END(tag) LOGD(tag, "# %dms", ((clock() - __begin) / 1000))
#else
#define TIME_BEGIN
#define TIME_END(tag)
#endif

#define IN
#define OUT
#define INOUT

#define NATIVE_BITMAP(pBmp) (*((NativeBitmap*)(pBmp)))




#endif //NATIVEIMAGEMERGE_INC_H
