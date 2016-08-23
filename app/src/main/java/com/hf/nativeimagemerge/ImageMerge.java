package com.hf.nativeimagemerge;

/**
 * Image compare
 * Created by vance on 2016/8/5.
 */
public class ImageMerge {
    public interface OnCompareFinishedListener {
        boolean OnCompareFinished(int distance);
    }

    /**
     * Compare two bitmap by feature and merge
     * @param bmp1 bitmap 1
     * @param bmp2 bitmap 2
     * @return distance between two bitmap
     */
    public NativeBitmap mergeByFeature(NativeBitmap bmp1, NativeBitmap bmp2, OnCompareFinishedListener listener) {
        long ptr = nativeMergeByFeature(bmp1.getNativePtr(), bmp2.getNativePtr(), listener);
        return (ptr == 0) ? null : new NativeBitmap(ptr);
    }

    /**
     * Compare two bitmap by feature and merge
     * @param bmp1 bitmap 1
     * @param bmp2 bitmap 2
     * @return distance between two bitmap
     */
    public NativeBitmap mergeByHash(NativeBitmap bmp1, NativeBitmap bmp2, OnCompareFinishedListener listener) {
        long ptr = nativeMergeByHash(bmp1.getNativePtr(), bmp2.getNativePtr(), listener);
        return (ptr == 0) ? null : new NativeBitmap(ptr);
    }

    /**
     * Merge two bitmaps by feature compare
     * @param bmpPtr1 bitmap 1
     * @param bmpPtr2 bitmap 2
     * @return 0: merge failed. Otherwise: merged bitmap
     */
    private static native long nativeMergeByFeature(long bmpPtr1, long bmpPtr2, OnCompareFinishedListener listener);

    /**
     * Merge two bitmaps by hash compare
     * @param bmpPtr1 bitmap 1
     * @param bmpPtr2 bitmap 2
     * @return 0: merge failed. Otherwise: merged bitmap
     */
    private static native long nativeMergeByHash(long bmpPtr1, long bmpPtr2, OnCompareFinishedListener listener);

//    /**
//     * Merge two bitmaps by hash compare
//     * @param bmpPtr1 bitmap 1
//     * @param bmpPtr2 bitmap 2
//     * @return 0: merge failed. Otherwise: merged bitmap
//     */
//    private static native long nativeMerge(long bmpPtr1, long bmpPtr2, int distance);
}
