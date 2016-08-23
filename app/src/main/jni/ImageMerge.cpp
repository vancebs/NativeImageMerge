//
// Created by vance on 2016/8/5.
//

#include "ImageMerge.h"
#include "FeatureCompare.h"
#include "HashCompare.h"

#define DEBUG_IMG_TRIMMED1     "trimmed1.png"
#define DEBUG_IMG_TRIMMED2     "trimmed2.png"

void ImageCompare::mergeByFeature(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& merged, IN OnCompareFinishedListener& listener, IN Debugger* pDebugger) {
    TRACE_INIT(pDebugger);

    // trim
    TRACE_BEGIN(pDebugger);
    NativeBitmap trimmed1, trimmed2;
    jint trimTop, trimBottom;
    trim(bmp1, bmp2, 100, trimmed1, trimmed2, trimTop, trimBottom);
    TRACE_END(pDebugger, "trim");
    PRINT_IMAGE(pDebugger, DEBUG_IMG_TRIMMED1, trimmed1);
    PRINT_IMAGE(pDebugger, DEBUG_IMG_TRIMMED2, trimmed2);

    // generate distance
    FeatureCompare compare;
    jint distance = compare.compare(trimmed1, trimmed2, pDebugger);
    trimmed1.recycle();
    trimmed2.recycle();
    LOGI("==MyTest==", "distance: %d", distance);

    // call listener
    TRACE_BEGIN(pDebugger);
    jboolean shouldMerge = listener.onCompareFinished(distance);
    LOGI("==MyTest==", "should merge: %s", shouldMerge ? "true" : "false");
    TRACE_END(pDebugger, "call listener");

    // merge
    TRACE_BEGIN(pDebugger);
    if (shouldMerge) {
        mergeBitmap(bmp1, bmp2, trimTop, trimBottom, distance, merged);
    }
    TRACE_END(pDebugger, "merge");

    TRACE_FINISH(pDebugger, "finished");
}

void ImageCompare::mergeByHash(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& merged, IN OnCompareFinishedListener& listener, IN Debugger* pDebugger) {
    TRACE_INIT(pDebugger);

    // trim
    TRACE_BEGIN(pDebugger);
    NativeBitmap trimmed1, trimmed2;
    jint trimTop, trimBottom;
    trim(bmp1, bmp2, 100, trimmed1, trimmed2, trimTop, trimBottom);
    TRACE_END(pDebugger, "trim");
    PRINT_IMAGE(pDebugger, DEBUG_IMG_TRIMMED1, trimmed1);
    PRINT_IMAGE(pDebugger, DEBUG_IMG_TRIMMED2, trimmed2);

    // compare
    HashCompare compare;
    jint distance = compare.compare(bmp1, bmp2, pDebugger);
    trimmed1.recycle();
    trimmed2.recycle();
    LOGI("==MyTest==", "distance: %d", distance);

    // call listener
    TRACE_BEGIN(pDebugger);
    jboolean shouldMerge = listener.onCompareFinished(distance);
    LOGI("==MyTest==", "should merge: %s", shouldMerge ? "true" : "false");
    TRACE_END(pDebugger, "call listener");

    // merge
    TRACE_BEGIN(pDebugger);
    if (shouldMerge) {
        mergeBitmap(bmp1, bmp2, trimTop, trimBottom, distance, merged);
    }
    TRACE_END(pDebugger, "merge");

    TRACE_FINISH(pDebugger, "finished");
}

void ImageCompare::trim(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint topOffset,  OUT NativeBitmap& trimmed1, OUT NativeBitmap& trimmed2, OUT jint& trimTop, OUT jint& trimBottom) {
    jint width = bmp1.getWidth();
    jint height = bmp1.getHeight();
    jint pixelsCount = bmp1.getPixelsCount();

    // get pixels
    jint* pPixels1 = bmp1.getPixels();
    jint* pPixels2 = bmp2.getPixels();

    // find the first different index from top & bottom
    jint topIndex = topOffset * width;
    jint bottomIndex = pixelsCount - 1;
    while (pPixels1[topIndex] == pPixels2[topIndex]) {
        topIndex ++;
    }
    while (pPixels1[bottomIndex] == pPixels2[bottomIndex]) {
        bottomIndex --;
    }

    // get row index that should be
    jint trimmedTopRowIndex = topIndex / width;
    jint trimmedBottomRowIndex = bottomIndex / width;

    // create trimmed bitmap
    NativeBitmap::create(bmp1, trimmedTopRowIndex, trimmedBottomRowIndex, trimmed1);
    NativeBitmap::create(bmp2, trimmedTopRowIndex, trimmedBottomRowIndex, trimmed2);

    // generate the number of rows that should be trimmed from top & bottom
    trimTop = trimmedTopRowIndex;
    trimBottom = height - trimmedBottomRowIndex - 1;
}

void ImageCompare::mergeBitmap(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint trimTop, IN const jint trimBottom, IN const jint distance, OUT NativeBitmap& merged) {
    // generate start row & end row
    jint width = bmp1.getWidth();
    jint height1 = bmp1.getHeight();
    jint height2 = bmp2.getHeight();
    jint newHeight = height1 + distance;
    jint sameHeight = height2 - trimBottom - trimTop - distance;

    //
    jint newY1 = 0;
    jint newHeight1 = height1 - trimBottom - (sameHeight / 2);
    jint newY2 = trimTop + sameHeight - (sameHeight / 2);
    jint newHeight2 = height2 - newY2;

    //LOGI("==MyTest==", "trimTop: %d, trimBottom: %d, newY1: %d, newHeight1: %d, newY2: %d, newHeight2: %d", trimTop, trimBottom, newY1, newHeight1, newY2, newHeight2);

    // merge to out
    NativeBitmap::create(width, newHeight, merged);
    jint* pOut = merged.getPixels();
    jint* pBmp1 = bmp1.getPixels();
    jint* pBmp2 = bmp2.getPixels();
    memcpy( pOut,
            pBmp1 + (newY1 * width),
            newHeight1 * width * sizeof(jint));
    memcpy( pOut + (newHeight1 * width),
            pBmp2 + (newY2 * width),
            newHeight2 * width * sizeof(jint));
}