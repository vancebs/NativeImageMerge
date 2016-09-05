package com.hf.nativeimagemerge;

import android.graphics.Bitmap;

/**
 * NativeBitmap
 * Created by vance on 2016/8/4.
 */
public class NativeBitmap {
    private long mNativePtr = 0;

    /*package*/ NativeBitmap(long ptr) {
        mNativePtr = ptr;
    }

    /**
     * Create NativeBitmap
     * @param width width
     * @param height height
     * @return NativeBitmap
     */
    public static NativeBitmap create(int width, int height) {
        return new NativeBitmap(nativeCreate(width, height));
    }

    /**
     * Create NativeBitmap
     * @param src src NativeBitmap
     * @return NativeBitmap
     */
    public static NativeBitmap create(NativeBitmap src) {
        return new NativeBitmap(nativeCreate(src.mNativePtr));
    }

    /**
     * Create NativeBitmap
     * @param src src NativeBitmap
     * @param startRow start row index
     * @param endRow end row index
     * @return NativeBitmap
     */
    public static NativeBitmap create(NativeBitmap src, int startRow, int endRow) {
        return new NativeBitmap(nativeCreate(src.mNativePtr, startRow, endRow));
    }

    /**
     * Create a uninitialized NativeBitmap
     * @return NativeBitmap
     */
    public static NativeBitmap createUninitialized() {
        return new NativeBitmap(nativeCreateUninitialized());
    }

    /**
     * Create NativeBitmap from Bitmap
     * @param bitmap bitmap
     * @return NativeBitmap
     */
    public static NativeBitmap createFromBitmap(Bitmap bitmap) {
        return new NativeBitmap(nativeFromBitmap(bitmap));
    }

    /**
     * Get the native pointer
     * @return native pointer
     */
    public long getNativePtr() {
        return mNativePtr;
    }

    /**
     * Recycle native bitmap
     */
    public void recycle() {
        if (mNativePtr != 0) {
            nativeRecycle(mNativePtr);
            mNativePtr = 0;
        }
    }

    /**
     * Convert NativeBitmap to Bitmap
     * @return Bitmap
     */
    public Bitmap toBitmap() {
        return (Bitmap) nativeToBitmap(mNativePtr);
    }

    /**
     * Get NativeBitmap width
     * @return width
     */
    @SuppressWarnings("unused")
    public int getWidth() {
        return nativeGetWidth(mNativePtr);
    }

    /**
     * Get NativeBitmap height
     * @return height
     */
    @SuppressWarnings("unused")
    public int getHeight() {
        return nativeGetHeight(mNativePtr);
    }

    private static native long nativeCreate(int width, int height);
    private static native long nativeCreate(long srcPtr);
    private static native long nativeCreate(long srcPtr, int startRow, int endRow);
    private static native long nativeCreateUninitialized();
    private static native void nativeRecycle(long ptr);
    private static native Object nativeToBitmap(long ptr);
    private static native long nativeFromBitmap(Object bitmap);

    private static native int nativeGetWidth(long ptr);
    private static native int nativeGetHeight(long ptr);

    static {
        System.loadLibrary("ImageMerge");
    }
}
