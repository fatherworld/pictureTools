	package org.yanzi.camera;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;

import org.yanzi.activity.SavePalm;
import org.yanzi.util.CamParaUtil;
import org.yanzi.util.DisplayUtil;
import org.yanzi.util.FileUtil;
import org.yanzi.util.ImageUtil;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.ShutterCallback;
import android.hardware.Camera.Size;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import com.alvasystems.arsdk.ARJni;

public class CameraInterface {
	private static final String TAG = "zhangli";
	private Camera mCamera;
	private Camera.Parameters mParams;
	private boolean isPreviewing = false;
	private float mPreviwRate = -1f;
	private int mCameraId = -1;
	private boolean isGoolgeFaceDetectOn = false;
	private static CameraInterface mCameraInterface;
	private int  rotaion = -1;
	private float[] edpoint = new float[10];
	private Camera.Size mPreviewSize = null;
	private int isAutoPreviewSize = -1; //# 0 自动获取  # 1 设置宽高大小 W:1280 H：720  # 2手动设置 W 和 H 大小
	private Context context;

	public interface CamOpenOverCallback {
		public void cameraHasOpened();
	}

	private CameraInterface() {

	}

	public static synchronized CameraInterface getInstance() {
		if (mCameraInterface == null) {
			mCameraInterface = new CameraInterface();
		}
		return mCameraInterface;
	}

	/**
	 * 打开Camera
	 * 
	 * @param callback
	 */
	public void doOpenCamera(CamOpenOverCallback callback, int cameraId) {
		Log.i(TAG, "Camera open....");
		mCamera = Camera.open(cameraId);
		mCameraId = cameraId;
	 
		//setCameraDisplayOrientation(activity,cameraId,mCamera);
		if (callback != null) {
			callback.cameraHasOpened();
		}
	}

	/**
	 * 开启预览
	 * 
	 * @param holder
	 * @param previewRate
	 */
	public void doStartPreview(SurfaceHolder holder, float previewRate,Context context) {
		Log.i(TAG, "doStartPreview...");
		this.context = context;
		if (isPreviewing) {
			mCamera.stopPreview();
			return;
		}
		if (mCamera != null) {
		
			
			mParams = mCamera.getParameters();
			mParams.setPictureFormat(PixelFormat.JPEG);// 设置拍照后存储的图片格式
		
			//修改相机宽度
			
			Log.e("zhangxin", isAutoPreviewSize+"-------------------");
			switch (isAutoPreviewSize) {
			
	
			case 0:
				//最适屏幕分辨率
				CamParaUtil.getInstance().printSupportPictureSize(mParams);
				CamParaUtil.getInstance().printSupportPreviewSize(mParams);
				// 设置PreviewSize和PictureSize
				mPreviewSize= CamParaUtil.getInstance().getPropPictureSize(mParams.getSupportedPictureSizes(),previewRate, 800);
				mPreviewSize= CamParaUtil.getInstance().getPropPreviewSize(mParams.getSupportedPreviewSizes(),previewRate, 800);
				mParams.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
				mParams.setPictureSize(mPreviewSize.width, mPreviewSize.height);
				Log.e("zhangxin", "preview size(最终): " + mPreviewSize.width+ "*" + mPreviewSize.height);
				break;

			case 1:
				//1280 * 720 
				List<Camera.Size> sizeList = mParams.getSupportedPreviewSizes();
				Iterator<Camera.Size> i = sizeList.iterator();
				while (true) {
					if (i.hasNext()) {
						Camera.Size size = (Camera.Size) (i.next());
						Log.e("zhangxin", "preview size: " + size.width + "*"+ size.height);

						if (null == mPreviewSize)
							mPreviewSize = size;

						int w = size.width;
						if (w > 1200 && w < 1300) {
							mPreviewSize = size;
							break;
						}
					} else {
						Log.e("zhangxin","there is no prefer size, choosed size is "+ mPreviewSize.width + "*"+ mPreviewSize.height);
						break;
					}
				}
				Log.e("zhangxin", "preview size(最终): " + mPreviewSize.width+ "*" + mPreviewSize.height);
				mParams.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
				mParams.setPictureSize(mPreviewSize.width, mPreviewSize.height);

				break;
		 

			}
		 
			
			setCameraDisplayOrientation(context,getCameraId());
			CamParaUtil.getInstance().printSupportFocusMode(mParams);
			List<String> focusModes = mParams.getSupportedFocusModes();
			if (focusModes.contains("continuous-video")) {
				mParams.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
			}
			mCamera.setParameters(mParams);

			try {
				mCamera.setPreviewDisplay(holder);
				mCamera.startPreview();// 开启预览
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			isPreviewing = true;
			mPreviwRate = previewRate;
			doPreviewData();
			
			mParams = mCamera.getParameters(); // 重新get一次
			Log.i(TAG, "最终设置:PreviewSize--With = " + mParams.getPreviewSize().width + "Height = "+ mParams.getPreviewSize().height);
			Log.i(TAG, "最终设置:PictureSize--With = " + mParams.getPictureSize().width + "Height = "+ mParams.getPictureSize().height);
		}
	}

 

	/**
	 * 停止预览，释放Camera
	 */
	public void doStopCamera() {
		if (null != mCamera) {
			mCamera.setPreviewCallback(null);
			mCamera.stopPreview();
			isPreviewing = false;
			mPreviwRate = -1f;
			mCamera.release();
			mCamera = null;
		}
	}

	/**
	 * 拍照
	 */
	public void doTakePicture() {
		if (isPreviewing && (mCamera != null)) {
			mCamera.takePicture(null, null, mJpegPictureCallback);
		}
	}
	
	public void doPreviewData () {
		if(mCamera != null){
			mCamera.setPreviewCallback(mBytePreviewCallback);
		}
	}

	/**
	 * 获取Camera.Parameters
	 * 
	 * @return
	 */
	public Camera.Parameters getCameraParams() {
		if (mCamera != null) {
			mParams = mCamera.getParameters();
			return mParams;
		}
		return null;
	}

	/**
	 * 获取Camera实例
	 * 
	 * @return
	 */
	public Camera getCameraDevice() {
		return mCamera;
	}

	public int getCameraId() {
		return mCameraId;
	}
	
	public int getCameraWidth(){
		return mPreviewSize.width;
	}
	
	public int getCameraHight(){
		return mPreviewSize.height;
	}
	
	public int  getCameraFormat() {
		return mCamera.getParameters().getPictureFormat();
	}
	public int  getCameraOraction() {
		return rotaion;
	}
	
	

	/* 为了实现拍照的快门声音及拍照保存照片需要下面三个回调变量 */
	ShutterCallback mShutterCallback = new ShutterCallback()
	// 快门按下的回调，在这里我们可以设置类似播放“咔嚓”声之类的操作。默认的就是咔嚓。
	{
		public void onShutter() {
			// TODO Auto-generated method stub
			Log.i(TAG, "myShutterCallback:onShutter...");
		}
	};
	PictureCallback mRawCallback = new PictureCallback()
	// 拍摄的未压缩原数据的回调,可以为null
	{

		public void onPictureTaken(byte[] data, Camera camera) {
			// TODO Auto-generated method stub
			Log.i(TAG, "myRawCallback:onPictureTaken...");

		}
	};
	PictureCallback mJpegPictureCallback = new PictureCallback()
	// 对jpeg图像数据的回调,最重要的一个回调
	{
		public void onPictureTaken(byte[] data, Camera camera) {
			// TODO Auto-generated method stub
			Log.i(TAG, "myJpegCallback:onPictureTaken...");
			Bitmap b = null;
			if (null != data) {
				b = BitmapFactory.decodeByteArray(data, 0, data.length);// data是字节数据，将其解析成位图
				mCamera.stopPreview();
				isPreviewing = false;
			}
			// 保存图片到sdcard
			if (null != b) {
				// 设置FOCUS_MODE_CONTINUOUS_VIDEO)之后，myParam.set("rotation",
				// 90)失效。
				// 图片竟然不能旋转了，故这里要旋转下
				Bitmap rotaBitmap = ImageUtil.getRotateBitmap(b, 90.0f);
				FileUtil.saveBitmap(rotaBitmap);
			}
			// 再次进入预览
			mCamera.startPreview();
			isPreviewing = true;
		}
	};
	
	PreviewCallback  mBytePreviewCallback = new PreviewCallback() {
		//相机数据回调
		private int[] srcRectCood = new int[10];
		private int[] resultRet = null;
		@Override
		public void onPreviewFrame(byte[] data, Camera camera) {
		 
			//获得数据
			
			srcRectCood[0] = SavePalm.x0;
			srcRectCood[1] = SavePalm.y0;
			srcRectCood[2] = SavePalm.x1;
			srcRectCood[3] = SavePalm.y1;
			
			srcRectCood[4] = SavePalm.x2;
			srcRectCood[5] = SavePalm.y2;
			srcRectCood[6] = SavePalm.x3;
			srcRectCood[7] = SavePalm.y3;
			
			//ARJni.FRPerform(1, edpoint, data, data);
			ARJni.PalmPreform(SavePalm.palmInfo, data, SavePalm.flagNum, srcRectCood);
			//SavePalm.flagRet0 = resultRet[0];
			//SavePalm.flagRet1 = resultRet[1];
			//Log.e("zhang", "result-------" + resultRet[0] + resultRet[1]);
		}
	};
	
	
	
	
	
	public int getIsAutoPreviewSize() {
		return isAutoPreviewSize;
	}

	public void setIsAutoPreviewSize(int isAutoPreviewSize) {
		this.isAutoPreviewSize = isAutoPreviewSize;
	}

	private void setCameraDisplayOrientation(Context context, int cameraId ) {
		Activity activity = (Activity)context;
		android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
		android.hardware.Camera.getCameraInfo(cameraId, info);
		int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
		int degrees = 0;
	    switch (rotation) {
		case Surface.ROTATION_0:
			degrees = 0;
			break;
		case Surface.ROTATION_90:
			degrees = 90;
			break;
		case Surface.ROTATION_180:
			degrees = 180;
			break;
		case Surface.ROTATION_270:
			degrees = 270;
			break;
		}
		rotaion = degrees;

		Log.e("zhangxin", "相机角度-------" + degrees);
		int result;

		if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
			result = (info.orientation + degrees) % 360;
			result = (360 - result) % 360;  // compensate the mirror
		} else { // back-facing
			result = (info.orientation - degrees + 360) % 360;
		}
		Log.e("zhangxin", "转换角度-------" + result);
		mCamera.setDisplayOrientation(result);
	}


	

}
