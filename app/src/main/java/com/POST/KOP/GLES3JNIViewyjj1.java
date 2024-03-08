package com.POST.KOP;
import android.widget.Toast;
import android.content.Context;
import android.opengl.GLSurfaceView;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.io.File;
import android.content.Context;

public class GLES3JNIViewyjj1 extends GLSurfaceView implements GLSurfaceView.Renderer {
	private Context mContext;	
	static{
		//System.load("/data/data/com.POST.KOP/libCube.so");
		//System.load("/data/local/tmp/libCube.so");
		//System.load("/data/data/libCube.so");
		System.loadLibrary("Cube");
	}
	
	
	
	public static byte fontData[];
    public GLES3JNIViewyjj1(Context context) {
        super(context);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		getHolder().setFormat(-3);
        setEGLContextClientVersion(3);
        setRenderer(this);
    }
    
	public void onDrawFrame(GL10 gl) {
		step();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		resize(width, height);
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		init();
	}
	
    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        imgui_Shutdown();
		
    }
	
	public static native void init();
	public static native void resize(int width, int height);
	public static native void step();
	public static native void imgui_Shutdown();
	public static native void MotionEventClick(boolean down,float PosX,float PosY);
	public static native String getWindowRect();
	public static native void real(int width, int height);
	public static native void ScreenSize(int width, int height);
	public static native void setkm(String 账号 ,String 密码);
}
