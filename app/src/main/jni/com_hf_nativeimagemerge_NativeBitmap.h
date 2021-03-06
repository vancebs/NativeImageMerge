/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "inc.h"
/* Header for class com_hf_nativeimagemerge_NativeBitmap */

#ifndef _Included_com_hf_nativeimagemerge_NativeBitmap
#define _Included_com_hf_nativeimagemerge_NativeBitmap
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeCreate
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeCreate__II
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeCreate
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeCreate__J
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeCreate
 * Signature: (JII)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeCreate__JII
  (JNIEnv *, jclass, jlong, jint, jint);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeCreate
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeCreateUninitialized
        (JNIEnv *, jclass);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeRecycle
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeRecycle
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeToBitmap
 * Signature: (J)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeToBitmap
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeFromBitmap
 * Signature: (Ljava/lang/Object;)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeFromBitmap
  (JNIEnv *, jclass, jobject);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeGetWidth
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeGetWidth
  (JNIEnv *, jclass, jlong);

/*
 * Class:     com_hf_nativeimagemerge_NativeBitmap
 * Method:    nativeGetHeight
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_com_hf_nativeimagemerge_NativeBitmap_nativeGetHeight
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
