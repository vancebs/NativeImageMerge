//
// Created by vance on 2016/8/5.
//

#include "ImageCompare.h"
#include <algorithm>
#include <hash_map>

#define COLOR_MASKED 0x00000000
#define COLOR_NOT_MASKED 0xFFFFFFFFFF

#define MAX_COLOR_DIFF 10
#define MAX_COLOR_DIFF_POWER2 (MAX_COLOR_DIFF * MAX_COLOR_DIFF)

#define COLOR_DIFF_A(c1, c2) ((((c1) >> 24) & 0xFF) - (((c2) >> 24) & 0xFF))
#define COLOR_DIFF_R(c1, c2) ((((c1) >> 16) & 0xFF) - (((c2) >> 16) & 0xFF))
#define COLOR_DIFF_G(c1, c2) ((((c1) >> 8) & 0xFF) - (((c2) >> 8) & 0xFF))
#define COLOR_DIFF_B(c1, c2) (((c1) & 0xFF) - ((c2) & 0xFF))

#define MIN_FEATURE_PIXEL_COUNT 50
#define MIN_FEATURE_RECT_DIFF 0.1f
#define MAX_FEATURE_WIDTH_DIFF 1
#define MAX_FEATURE_PIXEL_COUNT_DIFF 3

#define FEATURE_INCLUDE_POINT(f, x, y) { \
    if ((x) < (f).left) (f).left = (x); \
    else if ((x) > (f).right) (f).right = (x); \
    if ((y) < (f).top) (f).top = (y); \
    else if ((y) > (f).bottom) (f).bottom = (y); \
}

void ImageCompare::merge(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& merged) {
    TIME_BEGIN;

    // trim
    jint trimTop, trimBottom;
    NativeBitmap trimmed1;
    NativeBitmap trimmed2;
    trim(bmp1, bmp2, 100, trimmed1, trimmed2, trimTop, trimBottom);

    // xor
    NativeBitmap xorBmp;
    xorBitmap(trimmed1, trimmed2, xorBmp);

    // mask
    NativeBitmap mask1, mask2;
    generateMask(xorBmp, trimmed1, mask1);
    generateMask(xorBmp, trimmed2, mask2);
    xorBmp.recycle();

    // feature
    FeatureList featureList1, featureList2;
    generateFeatures(mask1, featureList1);
    generateFeatures(mask2, featureList2);

    // compare
    jint distance = compareFeatures(trimmed1, mask1, featureList1, trimmed2, mask2, featureList2);
    trimmed1.recycle();
    trimmed2.recycle();
    mask1.recycle();
    mask2.recycle();

    // merge
    mergeBitmap(bmp1, bmp2, trimTop, trimBottom, distance, merged);

    TIME_END("merge()");
}

void ImageCompare::trim(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint topOffset,  OUT NativeBitmap& trimmed1, OUT NativeBitmap& trimmed2, OUT jint& trimTop, OUT jint& trimBottom) {
    TIME_BEGIN;

    jint width = bmp1.getWidth();
    jint height = bmp1.getHeight();

    // get pixels
    jint* pPixels1 = bmp1.getPixels();
    jint* pPixels2 = bmp2.getPixels();

    // find the first different index from top & bottom
    jint topIndex = topOffset * width;
    jint bottomIndex = width * height - 1;
    while (pPixels1[topIndex] == pPixels2[topIndex]) {
        topIndex ++;
    }
    while (pPixels1[bottomIndex] == pPixels2[bottomIndex]) {
        bottomIndex --;
    }

    // get row index
    trimTop = topIndex / width;
    trimBottom = bottomIndex / width;

    // create trimmed bitmap
    NativeBitmap::create(bmp1, trimTop, trimBottom, trimmed1);
    NativeBitmap::create(bmp2, trimTop, trimBottom, trimmed2);

    TIME_END("trim()");
}

void ImageCompare::xorBitmap(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, OUT NativeBitmap& xorBmp) {
    TIME_BEGIN;

    NativeBitmap::create(bmp1, xorBmp);

    jint* pPixels1 = xorBmp.getPixels();
    jint* pPixels2 = bmp2.getPixels();

    jint size = xorBmp.getPixelsCount();
    for (jint i=0; i<size; i++) {
        pPixels1[i] ^= pPixels2[i];
    }

    TIME_END("xor()");
}

void ImageCompare::generateMask(IN const NativeBitmap& xorBmp, IN const NativeBitmap& bmp, OUT NativeBitmap& mask) {
    TIME_BEGIN;

    NativeBitmap::create(xorBmp, mask);
    jint* pMask = mask.getPixels();
    jint width = mask.getWidth();
    jint height = mask.getHeight();
    jint index = 0;

    for (jint y=0; y<height; y++) {
        for (jint x=0; x<width; x++) {
            if (pMask[index] != COLOR_MASKED) {
                pMask[index] = isSimilarWithMaskedColor(x, y, bmp, mask, 1) ? COLOR_MASKED : COLOR_NOT_MASKED;
            }
            index ++;
        }
    }

    TIME_END("generateMask()");
}

void ImageCompare::generateFeatures(IN const NativeBitmap& mask, OUT FeatureList& outFeatures) {
    TIME_BEGIN;

    std::queue<jint> queue;
    Feature tmpFeature;
    jint width = mask.getWidth();
    jint pixelsCount = mask.getPixelsCount();
    jint index = 0;
    jint* pMaskPixels = mask.getPixels();
    for (jint i=0; i<pixelsCount; i++) {
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);

            // init tmpFeature
            tmpFeature.left = tmpFeature.right = X_FROM_INDEX(i, width);
            tmpFeature.top = tmpFeature.bottom = Y_FROM_INDEX(i, width);
            tmpFeature.pixelCount = 0;

            while (!queue.empty()) {
                // pop an index from queue
                index = queue.front();
                queue.pop();

                // ignore masked point
                if (pMaskPixels[index] == COLOR_MASKED) {
                    continue;
                }

                // find a feature
                findRect(index, queue, mask, tmpFeature);
            }

            // save feature
            if (tmpFeature.pixelCount > MIN_FEATURE_PIXEL_COUNT) { // if the rect is too small. It is not valuable
                outFeatures.push_back(tmpFeature);
            }
        }
    }

    TIME_END("generateFeatures()");
    //LOGI("==MyTest==", "outFeatures size: %d", outFeatures.size());
}

jint ImageCompare::compareFeatures(IN const NativeBitmap& bmp1, IN const NativeBitmap& mask1, IN FeatureList& features1, IN const NativeBitmap& bmp2, IN const NativeBitmap& mask2, IN FeatureList& features2) {
    TIME_BEGIN;

    std::sort(features1.begin(), features1.end(), sort_by_s);
    std::sort(features2.begin(), features2.end(), sort_by_s);

    std::hash_map<jint, jint> map;

    Feature* pF1;
    Feature* pF2;

    float diff;

    jint maxDistance = 0;
    jint maxDistanceCount = 0;
    jint distance;
    jint count;

    for (FeatureList::iterator i=features1.begin(); i != features1.end(); i++) {
        pF1 = &(*i);
        diff = 1.0f;
        for(FeatureList::iterator j=features2.begin(); j != features2.end(); j++) {
            pF2 = &(*j);
            //LOGI("==MyTest==", "feature1[%d]: %d, feature2[%d]: %d", (i - features1.begin()), pF1->pixelCount, (j - features2.begin()), pF2->pixelCount);
            if (pF1->left == pF2->left && pF1->right == pF2->right && std::abs(FEATURE_HEIGHT(*pF1) - FEATURE_HEIGHT(*pF2)) < MAX_FEATURE_WIDTH_DIFF) {
                if (pF2->pixelCount - pF1->pixelCount >= MAX_FEATURE_PIXEL_COUNT_DIFF) {
                    continue;
                } else if (pF1->pixelCount - pF2->pixelCount >= MAX_FEATURE_PIXEL_COUNT_DIFF) {
                    // unnecessary to compare next rectangles.
                    break;
                }

                // width & height of two rectangle should not different too much.
                float d = diffRect(bmp1, mask1, *pF1, bmp2, mask2, *pF2);
                if (d < diff) {
                    // save the best match result
                    diff = d;
                }
                //LOGI("==MyTest==", "diff: %f, distance: %d", diff, pF1->top - pF2->top);
                //LOGI("==MyTest==", "distance: %d, feature1: [%d, %d, %d, %d], feature2: [%d, %d, %d, %d]", pF1->top - pF2->top, pF1->left, pF1->top, pF1->right, pF1->bottom, pF2->left, pF2->top, pF2->right, pF2->bottom);
                if (diff == 0.0f) {
                    // we have found the best match feature
                    break;
                }
            } else if ((pF1->pixelCount - pF2->pixelCount) >= MAX_FEATURE_PIXEL_COUNT_DIFF) {
                // unnecessary to compare next rectangles.
                break;
            }
        }

        // save result and save the max
        if (diff < MIN_FEATURE_RECT_DIFF) {
            // generate distance and save int map
            distance = FEATURE_DISTANCE(*pF1, *pF2);
            //LOGI("==MyTest==", "distance: %d, feature1: [%d, %d, %d, %d], feature2: [%d, %d, %d, %d]", distance, pF1->left, pF1->top, pF1->right, pF1->bottom, pF2->left, pF2->top, pF2->right, pF2->bottom);

            std::hash_map<jint, jint>::iterator i = map.find(distance);
            if (i != map.end()) {
                (*i).second ++;
                count = (*i).second;
            } else {
                count = 1;
                map[distance] = count;
            }

            // save the max distance result
            if (count > maxDistanceCount) {
                maxDistance = distance;
                maxDistanceCount = count;
            }
        }
    }

    TIME_END("compareFeatures()");
    LOGI("==MyTest==", "distance: %d", maxDistance);

//    for (std::hash_map<jint, jint>::iterator i = map.begin(); i != map.end(); i++) {
//        LOGI("==MyTest==", "map [%d, %d]", i->first, i->second);
//    }

    return maxDistance;
}

void ImageCompare::mergeBitmap(IN const NativeBitmap& bmp1, IN const NativeBitmap& bmp2, IN const jint trimTop, IN const jint trimBottom, IN const jint distance, OUT NativeBitmap& merged) {
    TIME_BEGIN;

    // generate start row & end row
    jint width = bmp1.getWidth();
    jint height = bmp1.getHeight();
    jint newHeight = height + distance;
    jint sameHeight = trimBottom - trimTop + 1;

    //
    //jint y1 = 0;
    jint height1 = trimTop + distance + (sameHeight / 2);
    jint y2 = trimTop + (sameHeight / 2);
    jint height2 = bmp2.getHeight() - y2;

    // merge to out
    NativeBitmap::create(width, newHeight, merged);
    jint* pOut = merged.getPixels();
    jint* pBmp1 = bmp1.getPixels();
    jint* pBmp2 = bmp2.getPixels();
    memcpy( pOut,
            pBmp1,
            height1 * width * sizeof(jint));
    memcpy( pOut + (height1 * width),
            pBmp2 + (y2 * width),
            height2 * width * sizeof(jint));

    TIME_END("mergeBitmap()");
}

inline bool ImageCompare::isSimilarWithMaskedColor(IN const int x, IN const int y, IN const NativeBitmap& bmp, INOUT NativeBitmap& mask, int o) {
    jint* pBmpPixels = bmp.getPixels();
    jint* pMaskPixels = mask.getPixels();
    jint width = bmp.getWidth();
    jint height = bmp.getHeight();
    float count = 0;
    float diff = 0;
    jint tmpPixel;
    jint index = 0;
    jint pixel = bmp.getPixel(x, y);
    jint xStart = TRIM(x - o, 0, width - 1); //mask.trimX(x - o); // TODO
    jint xEnd = TRIM(x + o, 0, width - 1); //mask.trimX(x + o);
    jint yStart = TRIM(y - o, 0, height - 1); //mask.trimY(y - o);
    jint yEnd = TRIM(y + o, 0, height - 1); //mask.trimY(y + o);
    jint da, dr, dg, db;
    jint tx, ty;
    for (ty=yStart; ty<=yEnd; ty++) {
        for (tx=xStart; tx<=xEnd; tx++) {
            index = XY_TO_INDEX(tx, ty, width); // bmp.xyToIndex(x, y); spend too much CPU time. directly compute without call a method
            tmpPixel = pBmpPixels[index];
            if (pMaskPixels[index] == COLOR_MASKED) {
                // generate color diff
                da = COLOR_DIFF_A(pixel, tmpPixel);
                dr = COLOR_DIFF_R(pixel, tmpPixel);
                dg = COLOR_DIFF_G(pixel, tmpPixel);
                db = COLOR_DIFF_B(pixel, tmpPixel);

                diff += da*da + dr*dr + dg*dg + db*db;
                count ++;
            }

            //index ++;
        }
    }

    return (count > 0) && ((diff / count) < MAX_COLOR_DIFF_POWER2);
}

#if 1
inline void ImageCompare::findRect(IN const jint index, INOUT std::queue<jint>& queue, INOUT const NativeBitmap& mask, INOUT Feature& feature) {
    jint* pMaskPixels = mask.getPixels();
    jint width = mask.getWidth();
    jint height = mask.getHeight();
    jint x = X_FROM_INDEX(index, width);
    jint y = Y_FROM_INDEX(index, width);

    // update rect
    FEATURE_INCLUDE_POINT(feature, x, y);

    // mask the current point
    pMaskPixels[index] = COLOR_MASKED;
    feature.pixelCount ++;

    // check point around
    int nx, ny, i;
    nx = x - 1;
    if (nx >= 0 && nx < width) {
        i = index - 1;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
    nx = x + 1;
    if (nx >= 0 && nx < width) {
        i = index + 1;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
    ny = y - 1;
    if (ny >= 0 && ny < height) {
        i = index - width;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
    ny = y + 1;
    if (ny >= 0 && ny < height) {
        i = index + width;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
}
#else
void ImageCompare::findRect(IN const jint index, INOUT std::queue<jint>& queue, INOUT const NativeBitmap& mask, INOUT Feature& feature) {
    jint* pMaskPixels = mask.getPixels();
    jint width = mask.getWidth();
    jint height = mask.getHeight();
    jint x = X_FROM_INDEX(index, width);
    jint y = Y_FROM_INDEX(index, width);

    jint minX, maxX, minY, maxY;
    jint minXIndex, maxXIndex, minYIndex, maxYIndex;

    // mask current point
    pMaskPixels[index] = COLOR_MASKED;
    feature.pixelCount ++;

    for (minX=x-1,minXIndex=index-1; minX>=0 && pMaskPixels[minXIndex] != COLOR_MASKED; minX--, minXIndex--) {
        pMaskPixels[minXIndex] = COLOR_MASKED;
        feature.pixelCount ++;
    }
    minX ++;
    for (maxX=x+1,maxXIndex=index+1; maxX<width && pMaskPixels[maxXIndex] != COLOR_MASKED; maxX++, maxXIndex++) {
        pMaskPixels[maxXIndex] = COLOR_MASKED;
        feature.pixelCount ++;
    }
    maxX --;
    for (minY=y-1,minYIndex=index-width; minY>=0 && pMaskPixels[minYIndex] != COLOR_MASKED; minY--, minYIndex-=width) {
        pMaskPixels[minYIndex] = COLOR_MASKED;
        feature.pixelCount ++;
    }
    minY ++;
    for (maxY=y+1,maxYIndex=index+width; maxY<height && pMaskPixels[maxYIndex] != COLOR_MASKED; maxY--, maxYIndex+=width) {
        pMaskPixels[maxYIndex] = COLOR_MASKED;
        feature.pixelCount ++;
    }
    maxY --;

    // update rect
    FEATURE_INCLUDE_POINT(feature, minX, minY);
    FEATURE_INCLUDE_POINT(feature, maxX, maxY);

    // mark all pixels in the rect
    // TODO

    // check point around
    int nx, ny, i;
    nx = x - 1;
    ny = y - 1;
    if (nx >= minX && nx <= maxX && ny >= minY && ny <= maxY) {
        i = index - width - 1;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
    nx = x + 1;
    ny = y - 1;
    if (nx >= minX && nx <= maxX && ny >= minY && ny <= maxY) {
        i = index - width + 1;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
    nx = x - 1;
    ny = y + 1;
    if (nx >= minX && nx <= maxX && ny >= minY && ny <= maxY) {
        i = index + width - 1;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
    nx = x + 1;
    ny = y + 1;
    if (nx >= minX && nx <= maxX && ny >= minY && ny <= maxY) {
        i = index + width + 1;
        if (pMaskPixels[i] != COLOR_MASKED) {
            queue.push(i);
        }
    }
}
#endif

bool ImageCompare::sort_by_s(const Feature& obj1,const Feature& obj2) {
    return obj1.pixelCount > obj2.pixelCount;
}

float ImageCompare::diffRect(IN const NativeBitmap& bmp1, IN const NativeBitmap& mask1, IN const Feature& f1, IN const NativeBitmap& bmp2, IN const NativeBitmap& mask2, IN const Feature& f2) {
    jint diff = 0;
    jint maxDiff = (jint)(f1.pixelCount * MIN_FEATURE_RECT_DIFF);
    jint width = bmp1.getWidth();
    jint* pPixels1 = bmp1.getPixels();
    jint* pPixels2 = bmp2.getPixels();
    jint* pMask1 = mask1.getPixels();
    jint* pMask2 = mask2.getPixels();
    jint index1, index2;
    for (jint y1=f1.top,y2=f2.top; y1<=f1.bottom && y2<=f2.bottom; y1++,y2++) {
        for (jint x1=f1.left, x2=f2.left; x1<=f1.right && x2<=f2.right; x1++, x2++) { // TODO
            index1 = XY_TO_INDEX(x1, y1, width);
            index2 = XY_TO_INDEX(x2, y2, width);
            if (pMask1[index1] != COLOR_MASKED) {
                if (pMask2[index2] == COLOR_MASKED || pPixels1[index1] != pPixels2[index2]) {
                    diff ++;

                    if (diff > maxDiff) {
                        // already greater than max diff. unnecessary to check more.
                        return 1.0f;
                    }
                }
            }
        }
    }

    return ((float)diff) / f1.pixelCount;
}