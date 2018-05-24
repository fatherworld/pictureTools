package com.alvasystems.arsdk;

import android.util.Log;

public class ARJni {

	public static final native void JniInit(Object mgr);

	public static native int FRSetDataInfo(int width, int height, int format, int orientation,int cameraid);
	
	public static native int FRPerform(int faceNumber, float [] boxpoints, byte[] prviewYuv, byte[] prviewDepth);
	
	public static native long PalmInit();
	
	public static native int PalmSetDataInfo(long pInfo, int width, int height, int format, int orientation,int cameraid);
	
	public static native int[] PalmPreform(long pInfo, byte[] previewYuv, int rectNum, int[] srcRectCood);
	
	public static native int GetPalmNum(long pInfo);
	
	public static native int[] GetPalmFlag(long pInfo);
	
	private static boolean loadLibrary(String nLibName) {
		try {
			System.loadLibrary(nLibName);
			System.out.println("Native library lib111111111" + nLibName+ ".so loaded");
			return true;
		} catch (UnsatisfiedLinkError ulee) {
			System.err.println("The library lib222222222" + nLibName+ ".so could not be loaded\n" + ulee);
		} catch (SecurityException se) {
			System.err.println("The library lib333333" + nLibName+ ".so was not allowed to be loaded\n" + se);
		}

		return false;
	}

	static {

		Log.e("zhangxin", "++++++++++++--------.so......");
		System.out.println("zhangli ......lib111111111");
		if (!loadLibrary("gnustl_shared")) {
			Log.e("zhangxin", "2");
			System.exit(1);
		}
		if (!loadLibrary("PalmPerfTest")) {// //arsdk alva_fr
			Log.e("zhangxin", "1");
			System.exit(1);
		}
		
		Log.e("zhangxin", "++++++++++++--------.so......succeed ");
	}

}
