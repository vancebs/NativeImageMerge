package com.hf.nativeimagemerge;

/**
 * Image compare
 * Created by vance on 2016/8/5.
 */
public class ImageMerge {
    public static final int INDEX_TRIM_TOP = 0;
    public static final int INDEX_TRIM_BOTTOM = 1;

    /**
     * Compare two bitmap by feature and merge
     * @param bmp1 bitmap 1
     * @param bmp2 bitmap 2
     * @param trimmed trimTop & trimBottom. see: {@link #INDEX_TRIM_TOP}, {@link #INDEX_TRIM_BOTTOM}
     * @return distance between two bitmap
     */
    public int compareByFeature(NativeBitmap bmp1, NativeBitmap bmp2, int[] trimmed) {
        return nativeCompareByFeature(bmp1.getNativePtr(), bmp2.getNativePtr(), trimmed);
    }

    /**
     * Compare two bitmap by feature and merge
     * @param bmp1 bitmap 1
     * @param bmp2 bitmap 2
     * @param trimmed trimTop & trimBottom. see: {@link #INDEX_TRIM_TOP}, {@link #INDEX_TRIM_BOTTOM}
     * @return distance between two bitmap
     */
    public int compareByHash(NativeBitmap bmp1, NativeBitmap bmp2, int[] trimmed) {
        return nativeCompareByHash(bmp1.getNativePtr(), bmp2.getNativePtr(), trimmed);
    }

    /**
     * Compare two bitmap by feature and merge
     * @param bmp1 bitmap 1
     * @param bmp2 bitmap 2
     * @param trimTop trimTop
     * @param trimBottom trimBottom
     * @param distance distance
     * @return distance between two bitmap
     */
    public NativeBitmap merge(NativeBitmap bmp1, NativeBitmap bmp2, int trimTop, int trimBottom, int distance) {
        long ptr = nativeMerge(bmp1.getNativePtr(), bmp2.getNativePtr(), trimTop, trimBottom, distance);
        return (ptr == 0) ? null : new NativeBitmap(ptr);
    }

    /**
     * Merge two bitmaps by feature compare
     * @param bmpPtr1 bitmap 1
     * @param bmpPtr2 bitmap 2
     * @param trimmed trimTop & trimBottom. see: {@link #INDEX_TRIM_TOP}, {@link #INDEX_TRIM_BOTTOM}
     * @return distance
     */
    private static native int nativeCompareByFeature(long bmpPtr1, long bmpPtr2, /* out */ int[] trimmed);

    /**
     * Merge two bitmaps by hash compare
     * @param bmpPtr1 bitmap 1
     * @param bmpPtr2 bitmap 2
     * @param trimmed trimTop & trimBottom. see: {@link #INDEX_TRIM_TOP}, {@link #INDEX_TRIM_BOTTOM}
     * @return distance
     */
    private static native int nativeCompareByHash(long bmpPtr1, long bmpPtr2, /* out */ int[] trimmed);

    /**
     * Merge two bitmaps by hash compare
     * @param bmpPtr1 bitmap 1
     * @param bmpPtr2 bitmap 2
     * @param trimTop trimTop
     * @param trimBottom trimBottom
     * @param distance distance
     * @return 0: merge failed. Otherwise: merged bitmap
     */
    private static native long nativeMerge(long bmpPtr1, long bmpPtr2, int trimTop, int trimBottom, int distance);
}
