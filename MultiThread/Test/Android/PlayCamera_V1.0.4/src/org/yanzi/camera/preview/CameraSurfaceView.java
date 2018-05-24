package org.yanzi.camera.preview;

import org.yanzi.activity.SavePalm;
import org.yanzi.camera.CameraInterface;
import org.yanzi.util.DisplayUtil;

import com.alvasystems.arsdk.ARJni;

import android.app.Activity;
import android.content.Context;
import android.graphics.Camera;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CameraSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
	private static final String TAG = "yanzi";
	CameraInterface mCameraInterface;
	Context mContext;
	SurfaceHolder mSurfaceHolder;
	private int mFlag = -1;
	//int orcation;
	//int cameraId;
	private	 android.hardware.Camera.Size s;
	public CameraSurfaceView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		mContext = context;
		mSurfaceHolder = getHolder();
		mSurfaceHolder.setFormat(PixelFormat.TRANSPARENT);//translucent半透明 transparent透明
		mSurfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		mSurfaceHolder.addCallback(this);
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		Log.i(TAG, "surfaceCreated...");
		CameraInterface.getInstance().doOpenCamera(null, CameraInfo.CAMERA_FACING_BACK);//CameraInfo.CAMERA_FACING_BACK
	
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,int height) {
		
		CameraInterface.getInstance().setIsAutoPreviewSize(DisplayUtil.isAutoPre);	
		CameraInterface.getInstance().doStartPreview(mSurfaceHolder, 1.333f,mContext);
		
		int mCameraId =	CameraInterface.getInstance().getCameraId();
		int mFormat = CameraInterface.getInstance().getCameraFormat();
		int mHeight = CameraInterface.getInstance().getCameraHight();
		int mWidth = CameraInterface.getInstance().getCameraWidth();
		int mOrcation = CameraInterface.getInstance().getCameraOraction();
		
		//ARJni.FRSetDataInfo(mWidth, mHeight, mFormat, mOrcation, mCameraId);
		ARJni.PalmSetDataInfo(SavePalm.palmInfo, mWidth, mHeight, 11, mOrcation, mCameraId);
		
		Point screenMetrics = DisplayUtil.getScreenMetrics(mContext);
	 
		Log.e("zhangxin", "布screenMetrics文件orcation="+ mOrcation +" width="+mWidth + " height=" + mHeight + "-id= "+ mCameraId+"screenMetrics.x"+screenMetrics.x+"screenMetrics.y"+screenMetrics.y);
		//Log.e("zhangxin", "布局文件orcation="+ mOrcation +" width="+mWidth + " height=" + mHeight + "-id= "+ mCameraId);
		//Log.e("zhangxin222", "设定文件orcation="+ mOrcation +" width="+width + " height=" + height + "-id= "+ mCameraId);
	}
	
	

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		// TODO Auto-generated method stub
		Log.i(TAG, "surfaceDestroyed...");
		CameraInterface.getInstance().doStopCamera();
	}
	
	
	public SurfaceHolder getSurfaceHolder(){
		return mSurfaceHolder;
	}
	
}
