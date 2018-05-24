package org.yanzi.ui;

import org.yanzi.activity.SavePalm;
import org.yanzi.camera.CameraInterface;
import org.yanzi.playcamera.R;
import org.yanzi.util.Util;

import com.alvasystems.arsdk.ARJni;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.hardware.Camera.CameraInfo;
import android.hardware.Camera.Face;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.ImageView;

public class FaceView extends ImageView {
	private static final String TAG = "YanZi";
	private Context mContext;
	private Paint mLinePaint;
	private Paint mLinePaint1;
	private Face[] mFaces;
	private Matrix mMatrix = new Matrix();
	private RectF mRect = new RectF();
	private Drawable mFaceIndicator = null;
	float[] pts = {SavePalm.x1,SavePalm.y1,SavePalm.x0,SavePalm.y0};
	private int[] result = null;
	private int palmNum = 0;
	
	public FaceView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
		initPaint();
		mContext = context;
		mFaceIndicator = getResources().getDrawable(R.drawable.ic_face_find_2);
	}


	public void setFaces(Face[] faces){
		this.mFaces = faces;
		invalidate();
	}
	public void clearFaces(){
		mFaces = null;
		invalidate();
	}
	

	@Override
	protected void onDraw(Canvas canvas) {
		
		canvas.drawPoints(pts, mLinePaint);
		
		//canvas.drawRect(SavePalm.x0,SavePalm.y0,SavePalm.x1,SavePalm.y1, mLinePaint);
		
		//canvas.drawRect(SavePalm.x2,SavePalm.y2,SavePalm.x3,SavePalm.y3, mLinePaint);
		
		//result = ARJni.GetPalmFlag(SavePalm.palmInfo);
		//Log.e("zhang", "result---" + SavePalm.x0,SavePalm.y0,SavePalm.x1 - SavePalm.x0,SavePalm.y1 - SavePalm.y0, SavePalm.x2,SavePalm.y2,SavePalm.x3 - SavePalm.x2,SavePalm.y3 - SavePalm.y2);
		//System.out.println("Native library lib111111111" + SavePalm.flagRet + ".so loaded");
		//if(SavePalm.flagRet0 == 1)
		{
			//canvas.drawPoint((SavePalm.x0 + SavePalm.x1) / 2.0f,(SavePalm.y0 + SavePalm.y1) / 2.0f, mLinePaint1);
		}
		
		//if(SavePalm.flagRet1 == 1)
		{
			//canvas.drawPoint((SavePalm.x2 + SavePalm.x3) / 2.0f,(SavePalm.y2 + SavePalm.y3) / 2.0f, mLinePaint1);
		}
			
		
		
//		// TODO Auto-generated method stub
//		if(mFaces == null || mFaces.length < 1){
//			return;
//		}
//		boolean isMirror = false;
//		int Id = CameraInterface.getInstance().getCameraId();
//		if(Id == CameraInfo.CAMERA_FACING_BACK){
//			isMirror = false; //后置Camera无需mirror
//		}else if(Id == CameraInfo.CAMERA_FACING_FRONT){
//			isMirror = true;  //前置Camera需要mirror
//		}
//		Util.prepareMatrix(mMatrix, isMirror, 90, getWidth(), getHeight());
//		canvas.save();
//		mMatrix.postRotate(0); //Matrix.postRotate默认是顺时针
//		canvas.rotate(-0);   //Canvas.rotate()默认是逆时针 
//		for(int i = 0; i< mFaces.length; i++){
//			mRect.set(mFaces[i].rect);
//			mMatrix.mapRect(mRect);
//            mFaceIndicator.setBounds(Math.round(mRect.left), Math.round(mRect.top),Math.round(mRect.right), Math.round(mRect.bottom));
//            mFaceIndicator.draw(canvas);
//			//canvas.drawRect(mRect, mLinePaint);
//		}

		
		
		canvas.restore();		
		super.onDraw(canvas);
		invalidate();
		
		
	}

	private void initPaint(){
		mLinePaint = new Paint(Paint.ANTI_ALIAS_FLAG);
		int color = Color.rgb(98, 212, 68);
		mLinePaint.setColor(color);
		mLinePaint.setStyle(Style.STROKE);
		mLinePaint.setStrokeWidth(20f);
		mLinePaint.setAlpha(180);
		
		
		mLinePaint1 = new Paint(Paint.ANTI_ALIAS_FLAG);
		int color1 = Color.rgb(255, 0, 0);
		mLinePaint1.setColor(color1);
		mLinePaint1.setStyle(Style.STROKE);
		mLinePaint1.setStrokeWidth(20f);
		mLinePaint1.setAlpha(180);
	}
}
