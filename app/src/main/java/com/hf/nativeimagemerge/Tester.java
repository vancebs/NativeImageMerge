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
    private static final String PATH_IMG_OUT = PATH_DIR + "/out.png";

    public void test() {
        Log.i("==MyTest==", "From com.hf.featurecompare.nativeway");

        ImageMerge imageMerge = new ImageMerge();
        int[] trimmed = new int[2];
        int distance;

        // read
        NativeBitmap bmp1 = BitmapDecoder.readImage(PATH_IMG1);
        NativeBitmap bmp2 = BitmapDecoder.readImage(PATH_IMG2);

        // compare by hash
        distance = imageMerge.compareByHash(bmp1, bmp2, trimmed);

        // compare by feature if necessary
        if (distance > 700) {
            distance = imageMerge.compareByFeature(bmp1, bmp2, trimmed);
        }

        // merge
        NativeBitmap bmp = imageMerge.merge(bmp1, bmp2, trimmed[ImageMerge.INDEX_TRIM_TOP], trimmed[ImageMerge.INDEX_TRIM_BOTTOM], distance);

        // out put
        BitmapDecoder.writeImage(PATH_IMG_OUT, bmp);

        // clean up
        bmp1.recycle();
        bmp2.recycle();
        bmp.recycle();
    }
}
