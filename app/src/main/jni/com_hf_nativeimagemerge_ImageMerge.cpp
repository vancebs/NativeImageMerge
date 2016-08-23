#include "com_hf_nativeimagemerge_ImageMerge.h"
#include "ImageMerge.h"

ImageCompare sImageCompare;

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeMerge
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageMerge_nativeMergeByFeature(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2, jobject listener) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    // create listener
    OnCompareFinishedListener myListener(env, listener);

    // Do merge
    NativeBitmap* pBmp = new NativeBitmap();

#if defined _DEBUG_IMAGE || defined _DEBUG_PERFORMANCE
    Debugger debugger(env);
    sImageCompare.mergeByFeature(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), *pBmp, myListener, &debugger);
#else
    sImageCompare.mergeByFeature(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), *pBmp, myListener);
#endif

    return (jlong) pBmp;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeMerge
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageMerge_nativeMergeByHash(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2, jobject listener) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    // create listener
    OnCompareFinishedListener myListener(env, listener);

    // Do merge
    NativeBitmap* pBmp = new NativeBitmap();

#if defined _DEBUG_IMAGE || defined _DEBUG_PERFORMANCE
    Debugger debugger(env);
    sImageCompare.mergeByHash(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), *pBmp, myListener, &debugger);
#else
    sImageCompare.mergeByHash(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), *pBmp, myListener);
#endif

    return (jlong) pBmp;
}