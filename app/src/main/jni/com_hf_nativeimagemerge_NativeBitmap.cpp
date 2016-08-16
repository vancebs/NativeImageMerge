#include "com_hf_nativeimagemerge_NativeBitmap.h"
#include "NativeBitmap.h"

/*
 * Class:     com_hf_featurecompare_nativeway_NativeBitmap
 * Method:    nativeCreate
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeCreate__II(JNIEnv* env, jclass cls, jint width, jint height) {
    return (jlong)(& NativeBitmap::create(width, height, *(new NativeBitmap())));
}

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeCreate
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeCreate__J(JNIEnv * env, jclass cls, jlong ptrSrc) {
    return (jlong)(& NativeBitmap::create(NATIVE_BITMAP(ptrSrc), *(new NativeBitmap())));
}

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeCreate
 * Signature: (JII)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeCreate__JII(JNIEnv * env, jclass cls, jlong ptrSrc, jint startRow, jint endRow) {
    return (jlong)(& NativeBitmap::create(NATIVE_BITMAP(ptrSrc), startRow, endRow, *(new NativeBitmap())));
}

/*
 * Class:     com_hf_featurecompare_nativeway_NativeBitmap
 * Method:    recycle
 * Signature: (II)J
 */
JNIEXPORT void JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeRecycle(JNIEnv* env, jclass cls, jlong ptr) {
    if (ptr != 0) {
        NativeBitmap* pBmp = (NativeBitmap*)ptr;
        delete pBmp;
    }
}

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    toBitmap
 * Signature: (J)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeToBitmap(JNIEnv* env, jclass cls, jlong ptr) {
    const NativeBitmap& bmp = NATIVE_BITMAP(ptr);
    jint width = bmp.getWidth();
    jint height = bmp.getHeight();
    jsize pixelsCount = bmp.getPixelsCount();
    jint* pPixels = bmp.getPixels();

    // get config
    jclass clsConfig = env->FindClass("android/graphics/Bitmap$Config");
    jfieldID ID_ARGB_8888 = env->GetStaticFieldID(clsConfig, "ARGB_8888", "Landroid/graphics/Bitmap$Config;");
    jobject ARGB_8888 = env->GetStaticObjectField(clsConfig, ID_ARGB_8888);

    // create bitmap
    jclass clsBitmap = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmap = env->GetStaticMethodID(clsBitmap,"createBitmap", "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jobject bitmap = env->CallStaticObjectMethod(clsBitmap, createBitmap, width, height, ARGB_8888);

    // make int array
    jintArray pixels = env->NewIntArray(pixelsCount);
    env->SetIntArrayRegion(pixels, 0, pixelsCount, pPixels);

    // copy pixels
    jmethodID setPixels = env->GetMethodID(clsBitmap, "setPixels", "([IIIIIII)V");
    env->CallVoidMethod(bitmap, setPixels, pixels, 0, width, 0, 0, width, height);

    return bitmap;
}

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    fromBitmap
 * Signature: (Ljava/lang/Object;)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeFromBitmap(JNIEnv * env, jclass cls, jobject obj) {
    jint width;
    jint height;
    jsize pixelsCount;

    jclass clsBitmap = env->FindClass("android/graphics/Bitmap");

    // get width
    jmethodID getWidth = env->GetMethodID(clsBitmap, "getWidth", "()I");
    width = env->CallIntMethod(obj, getWidth);

    // get height
    jmethodID getHeight = env->GetMethodID(clsBitmap, "getHeight", "()I");
    height = env->CallIntMethod(obj, getHeight);

    // pixelsCount
    pixelsCount = width * height;

    // make int array
    jintArray pixels = env->NewIntArray(pixelsCount);

    // get pixels
    jmethodID getPixels = env->GetMethodID(clsBitmap, "getPixels", "([IIIIIII)V");
    env->CallVoidMethod(obj, getPixels, pixels, 0, width, 0, 0, width, height);

    NativeBitmap& bmp = NativeBitmap::create(width, height, *(new NativeBitmap()));
    env->GetIntArrayRegion(pixels, 0, pixelsCount, bmp.getPixels());

    return (jlong) &bmp;
}

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeGetWidth
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeGetWidth(JNIEnv * env, jclass cls, jlong ptr) {
    return NATIVE_BITMAP(ptr).getWidth();
}

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeGetHeight
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeGetHeight(JNIEnv * env, jclass cls, jlong ptr) {
    return NATIVE_BITMAP(ptr).getHeight();
}
