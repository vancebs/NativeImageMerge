//
// Created by vance on 2016/8/23.
//

#ifndef NATIVEIMAGEMERGE_ONCOMPAREFINISHEDLISTENER_H
#define NATIVEIMAGEMERGE_ONCOMPAREFINISHEDLISTENER_H


#include "inc.h"

class OnCompareFinishedListener {
private:
    JNIEnv* mpEnv;
    jobject mListener;

public:
    OnCompareFinishedListener(JNIEnv* env, jobject listener);
    jboolean onCompareFinished(jint distance);
};


#endif //NATIVEIMAGEMERGE_ONCOMPAREFINISHEDLISTENER_H
