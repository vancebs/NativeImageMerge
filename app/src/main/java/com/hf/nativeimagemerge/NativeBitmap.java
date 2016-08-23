package com.hf.nativeimagemerge;

import android.graphics.Bitmap;

/**
 * Created by vance on 2016/8/4.
 */
public class NativeBitmap {
    private long mNativePtr;

    /*package*/ NativeBitmap(long ptr) {
        mNativePtr = ptr;
    }

    public static NativeBitmap create(int width, int height) {
        return new NativeBitmap(nativeCreate(width, height));
    }

    public static NativeBitmap create(NativeBitmap src) {
        return new NativeBitmap(nativeCreate(src.mNativePtr));
    }

    public static NativeBitmap create(NativeBitmap src, int startRow, int endRow) {
        return new NativeBitmap(nativeCreate(src.mNativePtr, startRow, endRow));
    }

    public static NativeBitmap createFromBitmap(Bitmap bitmap) {
        return new NativeBitmap(nativeFromBitmap(bitmap));
    }

    public long getNativePtr() {
        return mNativePtr;
    }

    public void recycle() {
        nativeRecycle(mNativePtr);
    }

    public Bitmap toBitmap() {
        return (Bitmap) nativeToBitmap(mNativePtr);
    }

    public int getWidth() {
        return nativeGetWidth(mNativePtr);
    }

    public int getHeight() {
        return nativeGetHeight(mNativePtr);
    }

    private static native long nativeCreate(int width, int height);
    private static native long nativeCreate(long srcPtr);
    private static native long nativeCreate(long srcPtr, int startRow, int endRow);
    private static native void nativeRecycle(long ptr);
    private static native Object nativeToBitmap(long ptr);
    private static native long nativeFromBitmap(Object bitmap);

    private static native int nativeGetWidth(long ptr);
    private static native int nativeGetHeight(long ptr);

    static {
        System.loadLibrary("ImageMerge");
    }
}
