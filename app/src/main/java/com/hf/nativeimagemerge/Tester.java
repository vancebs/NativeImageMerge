package com.hf.nativeimagemerge;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;

/**
 * Created by vance on 2016/8/4.
 */
public class Tester {
    private static final boolean STEP_TEST = false;
    private static final String DIR = "1";
    private static final String PATH_DIR = "/sdcard/SupershotTest/" + DIR;
    private static final String PATH_IMG1 = PATH_DIR + "/1.png";
    private static final String PATH_IMG2 = PATH_DIR + "/2.png";
    private static final String PATH_IMG_OUT = PATH_DIR + "/out.png";
    private static final String PATH_IMG_TRIMMED1 = PATH_DIR + "/trimmed1.png";
    private static final String PATH_IMG_TRIMMED2 = PATH_DIR + "/trimmed2.png";
    private static final String PATH_IMG_XOR = PATH_DIR + "/xor.png";
    private static final String PATH_IMG_MASK1 = PATH_DIR + "/mask1.png";
    private static final String PATH_IMG_MASK2 = PATH_DIR + "/mask2.png";
    private static final String PATH_IMG_FEATURE1 = PATH_DIR + "/feature1.png";
    private static final String PATH_IMG_FEATURE2 = PATH_DIR + "/feature2.png";

    private Paint mPaint = null;

    public void test() {
        Log.i("==MyTest==", "From com.hf.featurecompare.nativeway");

        ImageCompare imageCompare = new ImageCompare();

        // read
        NativeBitmap bmp1 = BitmapDecoder.readImage(PATH_IMG1);
        NativeBitmap bmp2 = BitmapDecoder.readImage(PATH_IMG2);

        if (STEP_TEST) {
            // trim
            int[] trimPosition = new int[2];
            NativeBitmap[] trimmed = imageCompare.trim(bmp1, bmp2, trimPosition);
            NativeBitmap trimmed1 = trimmed[0];
            NativeBitmap trimmed2 = trimmed[1];
            BitmapDecoder.writeImage(PATH_IMG_TRIMMED1, trimmed1);
            BitmapDecoder.writeImage(PATH_IMG_TRIMMED2, trimmed2);

            // xor
            NativeBitmap xor = imageCompare.xor(trimmed1, trimmed2);
            BitmapDecoder.writeImage(PATH_IMG_XOR, xor);

            // mask
            NativeBitmap mask1 = imageCompare.generateMask(xor, trimmed1);
            NativeBitmap mask2 = imageCompare.generateMask(xor, trimmed2);
            xor.recycle();
            BitmapDecoder.writeImage(PATH_IMG_MASK1, mask1);
            BitmapDecoder.writeImage(PATH_IMG_MASK2, mask2);

            // feature
            ImageCompare.Feature[] features1 = imageCompare.generateFeatures(mask1);
            ImageCompare.Feature[] features2 = imageCompare.generateFeatures(mask2);
            Bitmap featureShow1 = trimmed1.toBitmap();
            Bitmap featureShow2 = trimmed2.toBitmap();
            drawFeature(featureShow1, features1);
            drawFeature(featureShow2, features2);
            BitmapDecoder.writeImage(PATH_IMG_FEATURE1, featureShow1);
            BitmapDecoder.writeImage(PATH_IMG_FEATURE2, featureShow2);
            featureShow1.recycle();
            featureShow2.recycle();

            // compare
            int distance = imageCompare.compareFeatures(trimmed1, mask1, features1, trimmed2, mask2, features2);
            trimmed1.recycle();
            trimmed2.recycle();
            mask1.recycle();
            mask2.recycle();

            // merge
            NativeBitmap merged = imageCompare.mergeBitmap(bmp1, bmp2, trimPosition[0], trimPosition[1], distance);
            bmp1.recycle();
            bmp2.recycle();
            BitmapDecoder.writeImage(PATH_IMG_OUT, merged);
        } else {
            // merge
            NativeBitmap bmp = imageCompare.merge(bmp1, bmp2);

            // out put
            BitmapDecoder.writeImage(PATH_IMG_OUT, bmp);
        }
    }

    private void drawFeature(Bitmap bmp, ImageCompare.Feature[] features) {
        Canvas canvas = new Canvas(bmp);
        Paint paint = getPaint();

        for (ImageCompare.Feature feature : features) {
            canvas.drawRect(feature.left, feature.top, feature.right, feature.bottom, paint);
        }
    }

    private Paint getPaint() {
        if (mPaint == null) {
            mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
            mPaint.setStyle(Paint.Style.STROKE);
            mPaint.setStrokeWidth(1.0f);
            mPaint.setColor(0xFFFF0000);
        }
        return mPaint;
    }
}
