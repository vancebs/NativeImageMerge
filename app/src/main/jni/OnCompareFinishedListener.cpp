//
// Created by vance on 2016/8/23.
//

#include "OnCompareFinishedListener.h"

OnCompareFinishedListener::OnCompareFinishedListener(JNIEnv* env, jobject listener) :
    mpEnv(env),
    mListener(listener)
{
}

jboolean OnCompareFinishedListener::onCompareFinished(jint distance) {
    if (mListener != NULL) {
        jclass clazz = mpEnv->FindClass("com/hf/nativeimagemerge/ImageMerge$OnCompareFinishedListener");
        jmethodID method = mpEnv->GetMethodID(clazz, "OnCompareFinished", "(I)Z");
        jboolean result = mpEnv->CallBooleanMethod(mListener, method, distance);
        mpEnv->DeleteLocalRef(clazz);
        return result;
    } else {
        return true;
    }
}