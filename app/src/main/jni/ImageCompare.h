//
// Created by vance on 2016/8/5.
//

#ifndef NATIVEIMAGEMERGE_IMAGECOMPARE_H
#define NATIVEIMAGEMERGE_IMAGECOMPARE_H

#include "inc.h"
#include "NativeBitmap.h"
#include "Queue.h"
#include <queue>

#define FEATURE_WIDTH(f) ((f).right - (f).left + 1)
#define FEATURE_HEIGHT(f) ((f).bottom - (f).top + 1)
//#define FEATURE_S(f) (FEATURE_WIDTH(f) * FEATURE_HEIGHT(f))
#define FEATURE_DISTANCE(f1, f2) std::abs((f2).top - (f1).top)


struct Feature {
    jint left;
    jint top;
    jint right;
    jint bottom;

    jint pixelCount;
};

typedef std::vector<Feature> FeatureList;

class ImageCompare {
public:
    void merge(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& merged);

    void trim(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint topOffset, OUT NativeBitmap& trimmed1, OUT NativeBitmap& trimmed2, OUT jint& trimTop, OUT jint& trimBottom);
    void xorBitmap(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& xorBmp);
    void generateMask(IN const NativeBitmap& xorBmp, IN const NativeBitmap& bmp, OUT NativeBitmap& mask);
    void generateFeatures(IN const NativeBitmap& mask, OUT FeatureList& outFeatures);
    jint compareFeatures(IN const NativeBitmap& bmp1, IN const NativeBitmap& mask1, IN FeatureList& features1, IN const NativeBitmap& bmp2, IN const NativeBitmap& mask2, IN  FeatureList& features2);
    void mergeBitmap(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint trimTop, IN const jint trimBottom, IN const jint distance, OUT NativeBitmap& merged);

private:
    bool isSimilarWithMaskedColor(IN const int x, IN const int y, IN const NativeBitmap& bmp, INOUT NativeBitmap& mask, int o);
    void findRect(IN const int index, INOUT std::queue<jint>& queue, INOUT const NativeBitmap& mask, INOUT  Feature& feature);

    bool static sort_by_s(const Feature& obj1,const Feature& obj2);
    float diffRect(IN const NativeBitmap& bmp1, IN const NativeBitmap& mask1, IN const Feature& f1, IN const NativeBitmap& bmp2, IN const NativeBitmap& mask2, IN const Feature& f2);
};


#endif //NATIVEIMAGEMERGE_IMAGECOMPARE_H
