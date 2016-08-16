#include "com_hf_nativeimagemerge_ImageCompare.h"
#include "ImageCompare.h"

#define FEATURE_CLASS(pEnv) (pEnv)->FindClass("com/hf/nativeimagemerge/ImageCompare$Feature")
#define FEATURE_CONSTRUCTOR(pEnv, clazz) (pEnv)->GetMethodID((clazz), "<init>", "()V")
#define FEATURE_CONSTRUCTOR_IIIII(pEnv, clazz) (pEnv)->GetMethodID((clazz), "<init>", "(IIIII)V")
#define FEATURE_FIELD_LEFT(pEnv, clazz) (pEnv)->GetFieldID((clazz), "left", "I")
#define FEATURE_FIELD_TOP(pEnv, clazz) (pEnv)->GetFieldID((clazz), "top", "I")
#define FEATURE_FIELD_RIGHT(pEnv, clazz) (pEnv)->GetFieldID((clazz), "right", "I")
#define FEATURE_FIELD_BOTTOM(pEnv, clazz) (pEnv)->GetFieldID((clazz), "bottom", "I")
#define FEATURE_FIELD_PIXEL_COUNT(pEnv, clazz) (pEnv)->GetFieldID((clazz), "pixelCount", "I")

ImageCompare sImageCompare;

jobjectArray toFeatureArrayObject(JNIEnv * env, IN FeatureList& features);
void fromFeatureArrayObject(JNIEnv * env, jobjectArray array, OUT FeatureList& features);
jobjectArray newEmptyFeatureArrayObject(JNIEnv * env);

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeMerge
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageCompare_nativeMerge(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    // Do merge
    NativeBitmap* pBmp = new NativeBitmap();
    sImageCompare.merge(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), *pBmp);

    return (jlong) pBmp;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeTrim
 * Signature: (JJ)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_hf_nativeimagemerge_ImageCompare_nativeTrim(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2, jintArray trimPosition) {
    jlongArray array = env->NewLongArray(2);
    if (bmp1 == 0 || bmp2 == 0) {
        return array;
    }

    // Do trim
    jint trim[2];
    NativeBitmap* pTrimmed1 = new NativeBitmap();
    NativeBitmap* pTrimmed2 = new NativeBitmap();
    sImageCompare.trim(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), 100, *pTrimmed1, *pTrimmed2, trim[0], trim[1]);

    jlong buff[2];
    buff[0] = (long) pTrimmed1;
    buff[1] = (long) pTrimmed2;
    env->SetLongArrayRegion(array, 0, 2, buff);

    env->SetIntArrayRegion(trimPosition, 0, 2, trim);

    return array;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeXor
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageCompare_nativeXor(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    // Do xor
    NativeBitmap* pBmp = new NativeBitmap();
    sImageCompare.xorBitmap(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), *pBmp);

    return (jlong) pBmp;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeGenerateMask
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageCompare_nativeGenerateMask(JNIEnv * env, jclass cls, jlong xorBmp, jlong bmp) {
    if (xorBmp == 0 || bmp == 0) {
        return 0;
    }

    // Do mask
    NativeBitmap* pMask = new NativeBitmap();
    sImageCompare.generateMask(NATIVE_BITMAP(xorBmp), NATIVE_BITMAP(bmp), *pMask);

    return (jlong) pMask;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeGenerateFeatures
 * Signature: (J)[Lcom/hf/nativeimagemerge/ImageCompare/Feature;
 */
JNIEXPORT jobjectArray JNICALL Java_com_hf_nativeimagemerge_ImageCompare_nativeGenerateFeatures(JNIEnv * env, jclass cls, jlong maskBmp) {
    if (maskBmp == 0) {
        return newEmptyFeatureArrayObject(env);
    }

    // Generate features
    FeatureList features;
    sImageCompare.generateFeatures(NATIVE_BITMAP(maskBmp), features);

    return toFeatureArrayObject(env, features);
}

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeCompareFeatures
 * Signature: (J[Lcom/hf/nativeimagemerge/ImageCompare/Feature;J[Lcom/hf/nativeimagemerge/ImageCompare/Feature;)I
 */
JNIEXPORT jint JNICALL Java_com_hf_nativeimagemerge_ImageCompare_nativeCompareFeatures(JNIEnv * env, jclass cls, jlong bmp1, jlong mask1, jobjectArray features1, jlong bmp2, jlong mask2, jobjectArray features2) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0; // not match
    }

    NativeBitmap* pBmp1 = (NativeBitmap*) bmp1;
    NativeBitmap* pBmp2 = (NativeBitmap*) bmp2;
    NativeBitmap* pMask1 = (NativeBitmap*) mask1;
    NativeBitmap* pMask2 = (NativeBitmap*) mask2;

    FeatureList featureList1;
    FeatureList featureList2;
    fromFeatureArrayObject(env, features1, featureList1);
    fromFeatureArrayObject(env, features2, featureList2);

    // Compare features
    jint distance = sImageCompare.compareFeatures(*pBmp1, *pMask1, featureList1, *pBmp2, *pMask2, featureList2);

    return distance;
}

/*
 * Class:     com_hf_nativeimagemerge_ImageCompare
 * Method:    nativeMergeBitmap
 * Signature: (JJI)J
 */
JNIEXPORT jlong JNICALL Java_com_hf_nativeimagemerge_ImageCompare_nativeMergeBitmap(JNIEnv * env, jclass cls, jlong bmp1, jlong bmp2, jint trimTop, jint trimBottom, jint distance) {
    if (bmp1 == 0 || bmp2 == 0) {
        return 0;
    }

    // Merge bitmap
    NativeBitmap* pMerged = new NativeBitmap();
    sImageCompare.mergeBitmap(NATIVE_BITMAP(bmp1), NATIVE_BITMAP(bmp2), trimTop, trimBottom, distance, *pMerged);

    return (jlong) pMerged;
}

jobjectArray toFeatureArrayObject(JNIEnv * env, IN FeatureList& features) {
    jobject obj;
    Feature* pFeature;
    jclass featureClass = FEATURE_CLASS(env);
    jobjectArray array = env->NewObjectArray(features.size(), featureClass, NULL);
    jsize index = 0;
    for (FeatureList::iterator i = features.begin(); i != features.end(); i++, index++) {
        pFeature = &(*i);

        // add obj to array
        obj = env->NewObject(featureClass, FEATURE_CONSTRUCTOR_IIIII(env, featureClass), pFeature->left, pFeature->top, pFeature->right, pFeature->bottom, pFeature->pixelCount);
        env->SetObjectArrayElement(array, index, obj);

        env->DeleteLocalRef(obj);
    }

    return array;
}

void fromFeatureArrayObject(JNIEnv * env, jobjectArray array, OUT FeatureList& features) {
    jsize size = env->GetArrayLength(array);
    Feature feature;
    jclass featureClass = FEATURE_CLASS(env);
    jobject obj;
    for(jsize i=0; i<size; i++) {
        obj = env->GetObjectArrayElement(array, i);

        feature.left = env->GetIntField(obj, FEATURE_FIELD_LEFT(env, featureClass));
        feature.top = env->GetIntField(obj, FEATURE_FIELD_TOP(env, featureClass));
        feature.right = env->GetIntField(obj, FEATURE_FIELD_RIGHT(env, featureClass));
        feature.bottom = env->GetIntField(obj, FEATURE_FIELD_BOTTOM(env, featureClass));
        feature.pixelCount = env->GetIntField(obj, FEATURE_FIELD_PIXEL_COUNT(env, featureClass));

        features.push_back(feature);

        env->DeleteLocalRef(obj);
    }
}

jobjectArray newEmptyFeatureArrayObject(JNIEnv * env) {
    jclass featureClass = FEATURE_CLASS(env);
    return env->NewObjectArray(0, featureClass, env->NewObject(featureClass, FEATURE_CONSTRUCTOR(env, featureClass)));
}
