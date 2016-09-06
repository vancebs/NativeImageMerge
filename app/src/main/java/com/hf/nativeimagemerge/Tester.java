package com.hf.nativeimagemerge;

import android.os.Environment;
import android.util.Log;

/**
 * Tester
 * Created by vance on 2016/8/4.
 */
public class Tester {
    private static final String DIR = "1";
    private static final String PATH_DIR = Environment.getExternalStorageDirectory() + "/SupershotTest/" + DIR;
    private static final String PATH_IMG1 = PATH_DIR + "/1.png";
    private static final String PATH_IMG2 = PATH_DIR + "/2.png";
    private static final String PATH_IMG3 = PATH_DIR + "/3.png";
    private static final String PATH_IMG_OUT = PATH_DIR + "/out.png";

    private static final int TYPE_UNKNOWN = 0;
    private static final int TYPE_HASH = 1;
    private static final int TYPE_FEATURE = 2;

    public void test() {
        Log.i("==MyTest==", "From com.hf.featurecompare.nativeway");

        ImageMerge imageMerge = new ImageMerge();
        int topLen = -1;
        int bottomLen = -1;
        int[] trimmed = new int[2];
        int distance;
        int[] type = new int[1];
        type[0] = TYPE_UNKNOWN;


        // read
        NativeBitmap bmps[] = {
                BitmapDecoder.readImage(PATH_IMG1),
                BitmapDecoder.readImage(PATH_IMG2),
                BitmapDecoder.readImage(PATH_IMG3)
        };

        // merge
        NativeBitmap merged = bmps[0];
        NativeBitmap tmp;
        for (int i=1; i<bmps.length; i++) {
            // get distance
            distance = compare(imageMerge, bmps[i-1], bmps[i], trimmed, type);

            if (topLen == -1) { // only save the first
                topLen = trimmed[ImageMerge.INDEX_TRIM_TOP];
            }
            bottomLen = trimmed[ImageMerge.INDEX_TRIM_BOTTOM]; // we should save the last

            // merge
            tmp = imageMerge.merge(merged, bmps[i], trimmed[ImageMerge.INDEX_TRIM_TOP], trimmed[ImageMerge.INDEX_TRIM_BOTTOM], distance);
            merged.recycle();
            merged = tmp;
        }

        // out put
        BitmapDecoder.writeImage(PATH_IMG_OUT, merged);

        // test clip
        NativeBitmap clipTopBmp = imageMerge.clipTop(bmps[2], topLen);
        NativeBitmap clipBottomBmp = imageMerge.clipBottom(bmps[2], bottomLen);
        BitmapDecoder.writeImage(PATH_DIR + "/clipTop.png", clipTopBmp);
        BitmapDecoder.writeImage(PATH_DIR + "/clipBottomBmp.png", clipBottomBmp);
        clipTopBmp.recycle();
        clipBottomBmp.recycle();

        // clean up
        for (int i=0; i<bmps.length; i++) {
            bmps[i].recycle();
        }
        merged.recycle();
    }

    private int compare(ImageMerge imageMerge, NativeBitmap bmp1, NativeBitmap bmp2, int[] trimmed, int[] type) {
        int distance;
        switch (type[0]) {
            case TYPE_HASH:
                distance = imageMerge.compareByHash(bmp1, bmp2, trimmed);
                break;
            case TYPE_FEATURE:
                distance = imageMerge.compareByFeature(bmp1, bmp2, trimmed);
                break;
            case TYPE_UNKNOWN:
            default:
                // compare by hash
                distance = imageMerge.compareByHash(bmp1, bmp2, trimmed);

                // compare by feature if necessary
                if (distance > 700) {
                    distance = imageMerge.compareByFeature(bmp1, bmp2, trimmed);
                    type[0] = TYPE_FEATURE;
                }
        }
        return distance;
    }
}
