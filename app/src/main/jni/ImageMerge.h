//
// Created by vance on 2016/8/5.
//

#ifndef NATIVEIMAGEMERGE_IMAGECOMPARE_H
#define NATIVEIMAGEMERGE_IMAGECOMPARE_H

#include "inc.h"
#include "NativeBitmap.h"
#include "Debugger.h"


class ImageCompare {
public:
    void mergeByFeature(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& merged, IN Debugger* pDebugger = NULL);
    void mergeByHash(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& merged, IN Debugger* pDebugger = NULL);

private:
    void trim(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint topOffset, OUT NativeBitmap& trimmed1, OUT NativeBitmap& trimmed2, OUT jint& trimTop, OUT jint& trimBottom);
    void mergeBitmap(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint trimTop, IN const jint trimBottom, IN const jint distance, OUT NativeBitmap& merged);
};


#endif //NATIVEIMAGEMERGE_IMAGECOMPARE_H
