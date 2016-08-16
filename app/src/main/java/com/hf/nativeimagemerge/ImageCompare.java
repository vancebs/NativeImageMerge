package com.hf.nativeimagemerge;

/**
 * Image compare
 * Created by vance on 2016/8/5.
 */
public class ImageCompare {
    public static class Feature {
        public int left;
        public int top;
        public int right;
        public int bottom;

        public int pixelCount;

        public Feature() {
        }

        public Feature(int left, int top, int right, int bottom, int pixelCount) {
            init(left, top, right, bottom, pixelCount);
        }

        public void init(int left, int top, int right, int bottom, int pixelCount) {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
            this.pixelCount = pixelCount;
        }

        public Feature(Feature src) {
            left = src.left;
            top = src.top;
            right = src.right;
            bottom = src.bottom;
            pixelCount = src.pixelCount;
        }

        @Override
        public String toString() {
            return "[" + left + ", " + top + ", " + right + ", " + bottom + "] count: " + pixelCount;
        }
    }

    /**
     * Compare two bitmap
     * @param bmp1 bitmap 1
     * @param bmp2 bitmap 2
     * @return distance between two bitmap
     */
    public NativeBitmap merge(NativeBitmap bmp1, NativeBitmap bmp2) {
        long ptr = nativeMerge(bmp1.getNativePtr(), bmp2.getNativePtr());
        return (ptr == 0) ? null : new NativeBitmap(ptr);
    }

    public NativeBitmap[] trim(NativeBitmap bmp1, NativeBitmap bmp2, int[] trimPosition) {
        long[] ptrs = nativeTrim(bmp1.getNativePtr(), bmp2.getNativePtr(), trimPosition);
        NativeBitmap[] bmps = new NativeBitmap[ptrs.length];
        for (int i=0; i<ptrs.length; i++) {
            bmps[i] = (ptrs[i] == 0) ? null : new NativeBitmap(ptrs[i]);
        }
        return bmps;
    }

    public NativeBitmap xor(NativeBitmap bmp1, NativeBitmap bmp2) {
        long ptr = nativeXor(bmp1.getNativePtr(), bmp2.getNativePtr());
        return (ptr == 0) ? null : new NativeBitmap(ptr);
    }

    public NativeBitmap generateMask(NativeBitmap xor, NativeBitmap bmp) {
        long ptr = nativeGenerateMask(xor.getNativePtr(), bmp.getNativePtr());
        return (ptr == 0) ? null : new NativeBitmap(ptr);
    }

    public Feature[] generateFeatures(NativeBitmap mask) {
        return nativeGenerateFeatures(mask.getNativePtr());
    }

    public int compareFeatures(NativeBitmap bmp1, NativeBitmap mask1, Feature[] features1, NativeBitmap bmp2, NativeBitmap mask2, Feature[] features2) {
        return nativeCompareFeatures(bmp1.getNativePtr(), mask1.getNativePtr(), features1, bmp2.getNativePtr(), mask2.getNativePtr(), features2);
    }

    public NativeBitmap mergeBitmap(NativeBitmap bmp1, NativeBitmap bmp2, int trimTop, int trimBottom, int distance) {
        long ptr = nativeMergeBitmap(bmp1.getNativePtr(), bmp2.getNativePtr(), trimTop, trimBottom, distance);
        return (ptr == 0) ? null : new NativeBitmap(ptr);
    }

    /**
     * Merge two bitmaps
     * @param bmpPtr1 bitmap 1
     * @param bmpPtr2 bitmap 2
     * @return 0: merge failed. Otherwise: merged bitmap
     */
    private static native long nativeMerge(long bmpPtr1, long bmpPtr2);

    private static native long[] nativeTrim(long bmpPtr1, long bmpPtr2, int[] trimPosition);
    private static native long nativeXor(long bmpPtr1, long bmpPtr2);
    private static native long nativeGenerateMask(long xorPtr, long bmpPtr);
    private static native Feature[] nativeGenerateFeatures(long maskPtr);
    private static native int nativeCompareFeatures(long bmpPtr1, long maskPtr1, Feature[] features1, long bmpPtr2, long maskPtr2, Feature[] features2);
    private static native long nativeMergeBitmap(long bmpPtr1, long bmpPtr2, int trimTop, int trimBottom, int distance);
}
