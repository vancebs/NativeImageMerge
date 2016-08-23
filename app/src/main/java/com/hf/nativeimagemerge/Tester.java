package com.hf.nativeimagemerge;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;

/**
 * Created by vance on 2016/8/4.
 */
public class Tester {
    private static final String DIR = "0";
    private static final String PATH_DIR = "/sdcard/SupershotTest/" + DIR;
    private static final String PATH_IMG1 = PATH_DIR + "/1.png";
    private static final String PATH_IMG2 = PATH_DIR + "/2.png";
    private static final String PATH_IMG_OUT = PATH_DIR + "/out.png";

    public void test() {
        Log.i("==MyTest==", "From com.hf.featurecompare.nativeway");

        ImageMerge imageMerge = new ImageMerge();

        // read
        NativeBitmap bmp1 = BitmapDecoder.readImage(PATH_IMG1);
        NativeBitmap bmp2 = BitmapDecoder.readImage(PATH_IMG2);

        // merge by hash
        final boolean[] result = new boolean[1];
        NativeBitmap bmp = imageMerge.mergeByHash(bmp1, bmp2, new ImageMerge.OnCompareFinishedListener() {
            @Override
            public boolean OnCompareFinished(int distance) {
                if (distance > 700) {
                    result[0] = false;
                    Log.i("==MyTest==", "hash compare failed");
                    return false;
                } else {
                    result[0] = true;
                    return true;
                }
            }
        });

        // merge by feature if necessary
        if (!result[0]) {
            bmp = imageMerge.mergeByFeature(bmp1, bmp2, null);
        }

        // out put
        BitmapDecoder.writeImage(PATH_IMG_OUT, bmp);
    }
}
