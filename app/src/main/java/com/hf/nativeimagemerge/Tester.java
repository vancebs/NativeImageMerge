package com.hf.nativeimagemerge;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;

/**
 * Created by vance on 2016/8/4.
 */
public class Tester {
    private static final String DIR = "1";
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

        // merge by feature
        NativeBitmap bmp = imageMerge.mergeByHash(bmp1, bmp2);

        // out put
        BitmapDecoder.writeImage(PATH_IMG_OUT, bmp);
    }
}
