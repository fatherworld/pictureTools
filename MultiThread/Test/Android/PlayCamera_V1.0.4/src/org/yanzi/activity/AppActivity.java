package org.yanzi.activity;

import com.alvasystems.arsdk.ARJni;

import android.app.Application;
import android.content.Context;
import android.util.Log;

public class AppActivity extends Application {

	private Context context;

	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		Log.e("zhangxin", "First init ");
		ARJni.JniInit(context);
		Log.e("zhangxin", "First JniInit ");

		ARJni.PalmInit();
	}

}
