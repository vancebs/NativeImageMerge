#include "com_hf_nativeimagemerge_ImageMerge.h"
#include "ImageMerge.h"

#define INDEX_TRIM_TOP 0
#define INDEX_TRIM_BOTTOM 1

ImageMerge sImageCompare;

/*
 * Class:     com_hf_nativeimagemerge_ImageMerge
 * Method:    nativeCompareByFeature
 * Signature: (JJ[I)I
 */
JNIEXPORT jint JNICALL Java_com_hf_nativeimagemerge_ImageMerge_nativeCompareByFeature(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2, jintArray trimmed) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    jint trimmedArray[2];
    jint distance;

    // compare
#if defined _DEBUG_IMAGE || defined _DEBUG_PERFORMANCE
    Debugger debugger(env);
    distance = sImageCompare.compareByFeature(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), trimmedArray[INDEX_TRIM_TOP], trimmedArray[INDEX_TRIM_BOTTOM], &debugger);
#else
    distance = sImageCompare.mergeByFeature(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), trimmedArray[INDEX_TRIM_TOP], trimmedArray[INDEX_TRIM_BOTTOM]);
#endif

    // return trimmed top & bottom
    env->SetIntArrayRegion(trimmed, 0, 2, trimmedArray);

    return distance;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageMerge
 * Method:    nativeCompareByHash
 * Signature: (JJ[I)I
 */
JNIEXPORT jint JNICALL Java_com_hf_nativeimagemerge_ImageMerge_nativeCompareByHash(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2, jintArray trimmed) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    jint trimmedArray[2];
    jint distance;

    // compare
#if defined _DEBUG_IMAGE || defined _DEBUG_PERFORMANCE
    Debugger debugger(env);
    distance = sImageCompare.compareByHash(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), trimmedArray[INDEX_TRIM_TOP], trimmedArray[INDEX_TRIM_BOTTOM], &debugger);
#else
    distance = sImageCompare.mergeByHash(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), trimmedArray[INDEX_TRIM_TOP], trimmedArray[INDEX_TRIM_BOTTOM]);
#endif

    // return trimmed top & bottom
    env->SetIntArrayRegion(trimmed, 0, 2, trimmedArray);

    return distance;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageMerge
 * Method:    nativeMerge
 * Signature: (JJIII)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageMerge_nativeMerge(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2, jint trimTop, jint trimBottom, jint distance) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    NativeBitmap* pMergedBmp = new NativeBitmap();

    // merge
    sImageCompare.mergeBitmap(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), trimTop, trimBottom, distance, *pMergedBmp);

    return (jlong) pMergedBmp;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageMerge
 * Method:    nativeClipTop
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageMerge_nativeClipTop(JNIEnv * env, jclass cls, jlong ptr, jint clipLength) {
    NativeBitmap* pOutBmp = new NativeBitmap;
    sImageCompare.clipBitmapTop(NATIVE_BITMAP(ptr), clipLength, *pOutBmp);
    return (long) pOutBmp;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageMerge
 * Method:    nativeClipBottom
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageMerge_nativeClipBottom(JNIEnv * env, jclass cls, jlong ptr, jint clipLength) {
    NativeBitmap* pOutBmp = new NativeBitmap;
    sImageCompare.clipBitmapBottom(NATIVE_BITMAP(ptr), clipLength, *pOutBmp);
    return (long) pOutBmp;
}