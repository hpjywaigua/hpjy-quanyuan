package com.POST.KOP;


import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.graphics.drawable.GradientDrawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.content.ClipboardManager;
import android.app.Activity;
import android.content.ClipData;
import android.content.ClipDescription;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Process;
import android.provider.Settings;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.text.SpannableString;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.NetworkInterface;
import java.net.URL;
import java.security.MessageDigest;
import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.UUID;
import org.json.JSONException;
import org.json.JSONObject;
import static android.widget.RelativeLayout.ALIGN_PARENT_RIGHT;
import static java.lang.System.loadLibrary;
import android.graphics.drawable.*;
import android.view.*;
import android.widget.*;
import android.graphics.*;
import android.view.Display;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import com.POST.KOP.GLES3JNIViewyjj1;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.NetworkInterface;
import java.net.ProtocolException;
import java.net.URL;
import java.util.Collections;
import java.util.List;
import android.widget.ImageView;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.widget.RelativeLayout;
import android.content.DialogInterface;
import java.util.zip.ZipInputStream;
import android.graphics.PorterDuff;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.view.Gravity;
import android.view.View;
//import android.view.serVideoView;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.TextView;
import android.widget.Toast;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;
import android.net.ConnectivityManager;
import com.POST.KOP.ClipboardUtil;
import android.net.NetworkInfo;


public class MainActivity extends Activity {
    EditText carmi;
    String content="";
    String APPKEY;
    String random;
    String path = Environment.getExternalStorageDirectory() + "/Android/";//保存文件路径
    static String 当前版本号="1.0";
    String 接口="",接口2,返回,返回2,返回3,返回值,设备码;
    String 返回在线;
    String 发送数据;
    String 转换a;
    String 返回查询;
    static String 版本号="1.0";
    private String km;
    private boolean 解绑1=false , 登录1=false;
    private boolean imsuop=false;
    private boolean 检测=false;
    private boolean dakao=false;

    private Point point;
    private WindowManager manager;
    private WindowManager.LayoutParams vParams;
    private View vTouch;
    private WindowManager 绘制窗口;
    public static int 屏幕宽,屏幕高;
    public  int shska;
    private Context cont;
    private Context mContext;   
    private LinearLayout layout;
    private ImageView 主图;
    private EditText 卡密框;
    private TextView 文本;
    private  TextView 充值公告文本;
    private  TextView 在线人数文本;

    private  TextView wmsma;
    private EditText edit;
    private EditText edite;
    private int screenWidth;
    private Button 登录按钮;
    private Button 解绑按钮;
    private Button 游戏按钮;
    static String 卡密存储路径="/storage/emulated/0/狗子/km";
    private  String 选择c = "曹尼玛";
    private GradientDrawable 卡密圆角;
    private GradientDrawable 按钮圆角;
    private String 到期时间返回;
    private Button 验证码按钮;
    private Button shiyong;
    private GradientDrawable 点击圆角;
    private GradientDrawable 验证码圆角;
    private GradientDrawable 空白圆角;
    private GradientDrawable 验证码按钮圆角;
    public static String code = "";
    public static String emailcode = "";
    static String 密码;    
    private String 账号;
    static LinearLayout linearLayouts;
    static String chaosongren;    
    private String youjianbiaoti;
    private int  bmbb;;
    private  String 选择 = "1";
    private  String 判断b = "1";
    private String 判断="0";
    private String 选择文本 = "0";
    private String 发送软件 = "邮件类应用";
    private String 发送信息;
    private String 账号备份 = "A743507468";
    private String 充值卡;
    private String 邮箱;
    private String 邮箱a;
    private String 新密码;
    static String 机器码;
    static String makals;
    private String gonggao文本;
    public static final String old_pg_name = "ropl.momo.item";
    private static EditText zhanghao;
    private static EditText mima;
    private static  Button dengluui;
    private static Button zhaohui;
    private static Button chongzhi;
    private static Button qitamianban;
    private static Button zhuce;
    private static Button qita;
    private static TextView gonggao;   
    private static ImageView qingchu;
    private static ImageView qiehuan;
    static  AlertDialog.Builder dialogs,dialogs1;
    static AlertDialog alertDialogs,alertDialogs1;














    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        //透明状态栏和导航栏
        if (Build.VERSION.SDK_INT >= 19) {
            //透明状态栏
            this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN);
            this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS);
            //透明导航栏
            this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION);
        }


        if (Build.VERSION.SDK_INT >= 19) {
            this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        }
        WindowManager 窗口=(WindowManager)getSystemService(Context.WINDOW_SERVICE);
        assert 窗口 != null;
        Display 分辨率=窗口.getDefaultDisplay();
        Point 输出分辨=new Point();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            分辨率.getRealSize(输出分辨);
        }
        if (输出分辨.x > 输出分辨.y) {
            屏幕宽 = 输出分辨.x;
            屏幕高 = 输出分辨.y;
        } else {
            屏幕宽 = 输出分辨.y;
            屏幕高 = 输出分辨.x;
        }
        GLES3JNIViewyjj1.ScreenSize(屏幕宽, 屏幕高);


        if (isVpnUsed()) {
            无网络(this);
        }

        checkNeedPermissions();
        Main();

        //setContentView(R.layout.activity_main);
        new Thread(new zxrs()){}.start();
        new Thread(new GG()){}.start();
        new Handler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    new Thread(new banbenjiance()){}.start();  
                }
            }, 1 * 1000); //1000毫s是1s

        banbenjc(this);



        requestPermission();

        Tools.创建文件夹("/storage/emulated/0/狗子");
        Tools.创建文件夹("/storage/emulated/0/Anodmlop");

		
        try{
            unzip(getAssets().open("sohookpng.zip"), "/storage/emulated/0/");
        }catch(IOException e){
            e.printStackTrace();
        }  


    }






    private void Main()
    {
        this.mContext = this;

        GradientDrawable 公告圆角 = new GradientDrawable();
        公告圆角.setCornerRadius(20);
        公告圆角.setColor(0xff69c5ff);
        公告圆角.setStroke(4, 0xFF000000);

        GradientDrawable 卡密圆角 = new GradientDrawable();
        卡密圆角.setCornerRadius(20);
        卡密圆角.setColor(0xfff5f5f7);
        卡密圆角.setStroke(3, 0xFF000000);

        验证码圆角 = new GradientDrawable();
        验证码圆角.setCornerRadius(20);
        验证码圆角.setColor(0xfff5f5f7);
        验证码圆角.setStroke(3, 0xFF000000);

        点击圆角 = new GradientDrawable();
        点击圆角.setCornerRadius(20);
        点击圆角.setColor(0xfff5f5f7);
        点击圆角.setStroke(3, 0xFF000000);

        GradientDrawable 卡密框圆角 = new GradientDrawable();


        GradientDrawable 按钮圆角 = new GradientDrawable();
        按钮圆角.setColor(0xff69c4ff);
        按钮圆角.setCornerRadius(25);
        按钮圆角.setStroke(3, 0xFF000000);

        验证码按钮圆角 = new GradientDrawable();
        验证码按钮圆角.setColor(0xff69c4ff);
        验证码按钮圆角.setCornerRadius(25);
        验证码按钮圆角.setStroke(3, 0xFF000000);

        LinearLayout layout = new LinearLayout(mContext);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        layout.setLayoutParams(params);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(0xffb5d4ff);
        layout.setGravity(Gravity.CENTER);


        ImageView 主图 = new ImageView(mContext);
        主图.setLayoutParams(new LinearLayout.LayoutParams(350, 350));
        try
        {
            InputStream mInputStream = mContext.getAssets().open("tgalo.png");
            Bitmap bitmap = BitmapFactory.decodeStream(mInputStream);
            主图.setImageBitmap(bitmap);
        } catch (IOException e) {}
        layout.addView(主图);


		
		
		
		
		
		

        Button dengluui = new Button(mContext);
        dengluui.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams dengluui间距= (LayoutParams) dengluui.getLayoutParams();
        dengluui.setTextColor(0xFF000000);
        dengluui.setTextSize(15);
        dengluui.setGravity(Gravity.CENTER);
        dengluui.setBackground(按钮圆角);
        dengluui.setText("登陆 狗子");
        dengluui间距.leftMargin = 100;
        dengluui间距.rightMargin = 100;
        dengluui间距.topMargin = 50;
        layout.addView(dengluui);

        Button 返回按钮 = new Button(mContext);
        返回按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 返回间距= (LayoutParams) 返回按钮.getLayoutParams();
        返回按钮.setTextColor(0xFF000000);
        返回按钮.setTextSize(15);
        返回按钮.setGravity(Gravity.CENTER);
        返回按钮.setBackground(按钮圆角);
        返回按钮.setText("注册 账号");
        返回间距.leftMargin = 100;
        返回间距.rightMargin = 100;
        返回间距.topMargin = 50;        
        layout.addView(返回按钮);

        Button skakskal按钮 = new Button(mContext);
        skakskal按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams skakskal间距= (LayoutParams) skakskal按钮.getLayoutParams();
        skakskal按钮.setTextColor(0xFF000000);
        skakskal按钮.setTextSize(15);
        skakskal按钮.setGravity(Gravity.CENTER);
        skakskal按钮.setBackground(按钮圆角);
        skakskal按钮.setText("充值 卡密");
        skakskal间距.leftMargin = 100;
        skakskal间距.rightMargin = 100;
        skakskal间距.topMargin = 50;      
        layout.addView(skakskal按钮);


        Button skskdkkd = new Button(mContext);
        skskdkkd.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams skskdkkd间距= (LayoutParams) skskdkkd.getLayoutParams();
        skskdkkd.setTextColor(0xFF000000);
        skskdkkd.setTextSize(15);
        skskdkkd.setGravity(Gravity.CENTER);
        skskdkkd.setBackground(按钮圆角);
        skskdkkd.setText("反馈与联系");
        skskdkkd间距.leftMargin = 100;
        skskdkkd间距.rightMargin = 100;
        skskdkkd间距.topMargin = 50;      
        layout.addView(skskdkkd);


        setContentView(layout);

        dengluui.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Login页面();


                }
            });     

        返回按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    注册页面();

                }
            });
        skakskal按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    充值页面();

                }
            });


        skskdkkd.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    反馈联系(MainActivity.this);

                }
            });


    }
    private void 更新页面()
    {
        this.mContext = this;



        GradientDrawable 公告圆角 = new GradientDrawable();
        公告圆角.setCornerRadius(25);
        公告圆角.setColor(0xff69c5ff);
        公告圆角.setStroke(5, 0xFF000000);

        GradientDrawable 卡密圆角 = new GradientDrawable();
        卡密圆角.setCornerRadius(25);
        卡密圆角.setColor(0xfff5f5f7);
        卡密圆角.setStroke(3, 0xFF000000);

        验证码圆角 = new GradientDrawable();
        验证码圆角.setCornerRadius(25);
        验证码圆角.setColor(0xfff5f5f7);
        验证码圆角.setStroke(3, 0xFF000000);

        点击圆角 = new GradientDrawable();
        点击圆角.setCornerRadius(25);
        点击圆角.setColor(0xfff5f5f7);
        点击圆角.setStroke(3, 0xFF000000);

        GradientDrawable 卡密框圆角 = new GradientDrawable();


        GradientDrawable 按钮圆角 = new GradientDrawable();
        按钮圆角.setColor(0xff69c4ff);
        按钮圆角.setCornerRadius(25);
        按钮圆角.setStroke(3, 0xFF000000);

        验证码按钮圆角 = new GradientDrawable();
        验证码按钮圆角.setColor(0xff69c4ff);
        验证码按钮圆角.setCornerRadius(25);
        验证码按钮圆角.setStroke(3, 0xFF000000);

        LinearLayout layout = new LinearLayout(mContext);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        layout.setLayoutParams(params);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(0xffb5d4ff);
        layout.setGravity(Gravity.CENTER);



        LinearLayout 公告 = new LinearLayout(mContext);
        LayoutParams 公告params = new LayoutParams(850, 500);
        公告.setLayoutParams(公告params);
        公告.setOrientation(LinearLayout.VERTICAL);
        公告.setBackground(公告圆角);
        公告.setGravity(Gravity.CENTER);
        layout.addView(公告);

        TextView 文本 = new TextView(mContext);
        LayoutParams 文本params = new LayoutParams(850, 500);
        公告.setLayoutParams(文本params);
        文本.setText(">版本为旧版本<");
        文本.setTextColor(0xFF000000);
        文本.setTextSize(15);
        文本.setGravity(Gravity.CENTER);
        公告.addView(文本);

        LinearLayout 分界线 = new LinearLayout(mContext);
        LayoutParams 分界线_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线.setLayoutParams(分界线_params);
        分界线.setBackgroundColor(0xFF000000);
        分界线_params.leftMargin = 100;
        分界线_params.rightMargin = 100;
        公告.addView(分界线);

        TextView 文本2 = new TextView(mContext);
        LayoutParams 文本2params = new LayoutParams(850, 500);
        公告.setLayoutParams(文本2params);
        文本2.setText("请您更新新版的狗子");
        文本2.setTextColor(0xFF000000);
        文本2.setTextSize(15);
        文本2.setGravity(Gravity.CENTER);
        公告.addView(文本2);

        LinearLayout 分界线2 = new LinearLayout(mContext);
        LayoutParams 分界线2_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线2.setLayoutParams(分界线2_params);
        分界线2.setBackgroundColor(0xFF000000);
        分界线2_params.leftMargin = 100;
        分界线2_params.rightMargin = 100;
        公告.addView(分界线2);



        Button 注册按钮 = new Button(mContext);
        注册按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 注册间距= (LayoutParams) 注册按钮.getLayoutParams();
        注册按钮.setTextColor(0xFF000000);
        注册按钮.setTextSize(15);
        注册按钮.setGravity(Gravity.CENTER);
        注册按钮.setBackground(按钮圆角);
        注册按钮.setText("更新新版本");
        注册间距.leftMargin = 150;
        注册间距.rightMargin = 150;
        注册间距.topMargin = 50;
        layout.addView(注册按钮);

        Button 返回按钮 = new Button(mContext);
        返回按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 返回间距= (LayoutParams) 返回按钮.getLayoutParams();
        返回按钮.setTextColor(0xFF000000);
        返回按钮.setTextSize(15);
        返回按钮.setGravity(Gravity.CENTER);
        返回按钮.setBackground(按钮圆角);
        返回按钮.setText("退出软件");
        返回间距.leftMargin = 150;
        返回间距.rightMargin = 150;
        返回间距.topMargin = 50;
        layout.addView(返回按钮);

        setContentView(layout);

        注册按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {           
                    String qun = "https://www.123pan.com/s/31VcVv-h28Rd.html";
                    startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(qun)));  
                    showToast("网盘密码：6666"); 
                }                    
            });


        返回按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Intent intent = new Intent();
                    intent.setClassName("com.excelliance.dlaid", "com.excekdlliance.kxqp.ui.MainAivity");//随意跳转至闪退
                    startActivity(intent);
                    finish();


                }
            });



    }   


    private void 充值页面()
    {
        this.mContext = this;



        GradientDrawable 公告圆角 = new GradientDrawable();
        公告圆角.setCornerRadius(25);
        公告圆角.setColor(0xff69c5ff);
        公告圆角.setStroke(5, 0xFF000000);

        GradientDrawable 卡密圆角 = new GradientDrawable();
        卡密圆角.setCornerRadius(25);
        卡密圆角.setColor(0xfff5f5f7);
        卡密圆角.setStroke(3, 0xFF000000);

        验证码圆角 = new GradientDrawable();
        验证码圆角.setCornerRadius(25);
        验证码圆角.setColor(0xfff5f5f7);
        验证码圆角.setStroke(3, 0xFF000000);

        点击圆角 = new GradientDrawable();
        点击圆角.setCornerRadius(25);
        点击圆角.setColor(0xfff5f5f7);
        点击圆角.setStroke(3, 0xFF000000);

        GradientDrawable 卡密框圆角 = new GradientDrawable();


        GradientDrawable 按钮圆角 = new GradientDrawable();
        按钮圆角.setColor(0xff69c4ff);
        按钮圆角.setCornerRadius(25);
        按钮圆角.setStroke(3, 0xFF000000);

        验证码按钮圆角 = new GradientDrawable();
        验证码按钮圆角.setColor(0xff69c4ff);
        验证码按钮圆角.setCornerRadius(25);
        验证码按钮圆角.setStroke(3, 0xFF000000);

        LinearLayout layout = new LinearLayout(mContext);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        layout.setLayoutParams(params);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(0xffb5d4ff);
        layout.setGravity(Gravity.CENTER);



        LinearLayout 公告 = new LinearLayout(mContext);
        LayoutParams 公告params = new LayoutParams(850, 500);
        公告.setLayoutParams(公告params);
        公告.setOrientation(LinearLayout.VERTICAL);
        公告.setBackground(公告圆角);
        公告.setGravity(Gravity.CENTER);
        layout.addView(公告);

        TextView 文本 = new TextView(mContext);
        LayoutParams 文本params = new LayoutParams(850, 500);
        公告.setLayoutParams(文本params);
        文本.setText("☞在线人数☜");
        文本.setTextColor(0xFF000000);
        文本.setTextSize(15);
        文本.setGravity(Gravity.CENTER);
        公告.addView(文本);

        LinearLayout 分界线 = new LinearLayout(mContext);
        LayoutParams 分界线_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线.setLayoutParams(分界线_params);
        分界线.setBackgroundColor(0xFF000000);
        分界线_params.leftMargin = 100;
        分界线_params.rightMargin = 100;
        公告.addView(分界线);

        TextView 在线人数文本 = new TextView(mContext);
        LayoutParams 在线人数文本params = new LayoutParams(850, 500); 
        公告.setLayoutParams(在线人数文本params);    
        //  在线人数文本.setText("298");
        在线人数文本.setText(380+返回在线);
        在线人数文本.setTextColor(0xFF000000);
        在线人数文本.setTextSize(15);
        在线人数文本.setGravity(Gravity.CENTER);
        公告.addView(在线人数文本);

        LinearLayout 分界线2 = new LinearLayout(mContext);
        LayoutParams 分界线2_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线2.setLayoutParams(分界线2_params);
        分界线2.setBackgroundColor(0xFF000000);
        分界线2_params.leftMargin = 100;
        分界线2_params.rightMargin = 100;
        公告.addView(分界线2);

        LinearLayout 邮箱布局 = new LinearLayout(mContext);
        LayoutParams 邮箱params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        邮箱布局.setLayoutParams(邮箱params);
        邮箱布局.setOrientation(LinearLayout.HORIZONTAL);
        邮箱布局.setGravity(Gravity.CENTER);
        邮箱布局.setBackground(卡密圆角);
        邮箱params.leftMargin = 50;
        邮箱params.rightMargin = 50;
        邮箱params.topMargin = 50;
        layout.addView(邮箱布局);


        ImageView 邮箱前图 = new ImageView(mContext);
        邮箱前图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream mInputStream = mContext.getAssets().open("tx.png");
            Bitmap bitmap = BitmapFactory.decodeStream(mInputStream);
            邮箱前图.setImageBitmap(bitmap);
        } catch (IOException e) {}
        邮箱布局.addView(邮箱前图);

        final EditText 邮箱框 = new EditText(mContext);
        LayoutParams 邮箱间距 = new LayoutParams(700, LayoutParams.WRAP_CONTENT);
        邮箱框.setLayoutParams(邮箱间距);
        邮箱框.setSingleLine(true);
        邮箱框.setTextColor(0xFF000000);
        邮箱框.setText(Tools.读取文件("/storage/emulated/0/狗子/zh"));
        邮箱框.setHint("请输入账号......"); 
        邮箱框.setHintTextColor(0xFF000000);
        邮箱框.setBackground(卡密框圆角);
        邮箱布局.addView(邮箱框);

        final ImageView 邮箱图 = new ImageView(mContext);
        邮箱图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t2 = mContext.getAssets().open("qc.png");
            Bitmap p2 = BitmapFactory.decodeStream(t2);
            邮箱图.setImageBitmap(p2);
        } catch (IOException e) {}
        邮箱布局.addView(邮箱图);

        LinearLayout 卡密布局 = new LinearLayout(mContext);
        LayoutParams 卡密params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        卡密布局.setLayoutParams(卡密params);
        卡密布局.setOrientation(LinearLayout.HORIZONTAL);
        卡密布局.setGravity(Gravity.CENTER);
        卡密布局.setBackground(卡密圆角);
        卡密params.leftMargin = 50;
        卡密params.rightMargin = 50;
        卡密params.topMargin = 50;
        layout.addView(卡密布局);


        ImageView 卡密前图 = new ImageView(mContext);
        卡密前图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t3 = mContext.getAssets().open("s.png");
            Bitmap p3 = BitmapFactory.decodeStream(t3);
            卡密前图.setImageBitmap(p3);
        } catch (IOException e) {}
        卡密布局.addView(卡密前图);

        final EditText 卡密框 = new EditText(mContext);
        LayoutParams 卡密间距 = new LayoutParams(700, LayoutParams.WRAP_CONTENT);
        卡密框.setLayoutParams(卡密间距);
        卡密框.setSingleLine(true);
        卡密框.setTextColor(0xFF000000);
        卡密框.setHint("请输入充值卡......");
        卡密框.setHintTextColor(0xFF000000);
        卡密框.setBackground(卡密框圆角);
        卡密布局.addView(卡密框);

        final ImageView 卡密图 = new ImageView(mContext);
        卡密图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t3 = mContext.getAssets().open("xs.png");
            Bitmap p3 = BitmapFactory.decodeStream(t3);
            卡密图.setImageBitmap(p3);
        } catch (IOException e) {}
        卡密布局.addView(卡密图);









        Button 注册按钮 = new Button(mContext);
        注册按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 注册间距= (LayoutParams) 注册按钮.getLayoutParams();
        注册按钮.setTextColor(0xFF000000);
        注册按钮.setTextSize(15);
        注册按钮.setGravity(Gravity.CENTER);
        注册按钮.setBackground(按钮圆角);
        注册按钮.setText("充 值");
        注册间距.leftMargin = 150;
        注册间距.rightMargin = 150;
        注册间距.topMargin = 50;
        layout.addView(注册按钮);

        Button 返回按钮 = new Button(mContext);
        返回按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 返回间距= (LayoutParams) 返回按钮.getLayoutParams();
        返回按钮.setTextColor(0xFF000000);
        返回按钮.setTextSize(15);
        返回按钮.setGravity(Gravity.CENTER);
        返回按钮.setBackground(按钮圆角);
        返回按钮.setText("返 回");
        返回间距.leftMargin = 150;
        返回间距.rightMargin = 150;
        返回间距.topMargin = 50;
        layout.addView(返回按钮);

        setContentView(layout);

        注册按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {



                    返回值="UserName="+邮箱框;
                    账号 = 邮箱框.getText().toString();
                    充值卡 = 卡密框.getText().toString();
                    接口 = "UserName=" + 账号 + "&CardPwd=" + 充值卡;
                    new Thread(new C()){}.start();
                    Tools.写入文件("/storage/emulated/0/狗子/km",充值卡);
                }
            });











        返回按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Main();

                }
            });


        卡密图.setOnClickListener(new View.OnClickListener() {

                private boolean 开关=true;
                @Override
                public void onClick(View v) {
                    if(开关){
                        //如果选中，显示密码
                        try {
                            InputStream t3 = mContext.getAssets().open("xs.png");
                            Bitmap p3 = BitmapFactory.decodeStream(t3);
                            卡密图.setImageBitmap(p3);
                        } catch (IOException e) {}
                        卡密框.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                        开关=false;
                    }else{
                        //否则隐藏密
                        try {
                            InputStream t3 = mContext.getAssets().open("yc.png");
                            Bitmap p3 = BitmapFactory.decodeStream(t3);
                            卡密图.setImageBitmap(p3);
                        } catch (IOException e) {}
                        卡密框.setTransformationMethod(PasswordTransformationMethod.getInstance());
                        开关=true;

                    }

                }
            });

        邮箱图.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View view) {
                    邮箱框.setText("");
                }

            });
    }


    private void 注册页面()
    {
        this.mContext = this;



        GradientDrawable 公告圆角 = new GradientDrawable();
        公告圆角.setCornerRadius(25);
        公告圆角.setColor(0xff69c5ff);
        公告圆角.setStroke(5, 0xFF000000);

        GradientDrawable 卡密圆角 = new GradientDrawable();
        卡密圆角.setCornerRadius(25);
        卡密圆角.setColor(0xfff5f5f7);
        卡密圆角.setStroke(3, 0xFF000000);

        验证码圆角 = new GradientDrawable();
        验证码圆角.setCornerRadius(25);
        验证码圆角.setColor(0xfff5f5f7);
        验证码圆角.setStroke(3, 0xFF000000);

        点击圆角 = new GradientDrawable();
        点击圆角.setCornerRadius(25);
        点击圆角.setColor(0xfff5f5f7);
        点击圆角.setStroke(3, 0xFF000000);

        GradientDrawable 卡密框圆角 = new GradientDrawable();


        GradientDrawable 按钮圆角 = new GradientDrawable();
        按钮圆角.setColor(0xff69c4ff);
        按钮圆角.setCornerRadius(25);
        按钮圆角.setStroke(3, 0xFF000000);

        验证码按钮圆角 = new GradientDrawable();
        验证码按钮圆角.setColor(0xff69c4ff);
        验证码按钮圆角.setCornerRadius(25);
        验证码按钮圆角.setStroke(3, 0xFF000000);

        LinearLayout layout = new LinearLayout(mContext);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        layout.setLayoutParams(params);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(0xffb5d4ff);
        layout.setGravity(Gravity.CENTER);



        LinearLayout 公告 = new LinearLayout(mContext);
        LayoutParams 公告params = new LayoutParams(850, 500);
        公告.setLayoutParams(公告params);
        公告.setOrientation(LinearLayout.VERTICAL);
        公告.setBackground(公告圆角);
        公告.setGravity(Gravity.CENTER);
        layout.addView(公告);

        TextView 文本 = new TextView(mContext);
        LayoutParams 文本params = new LayoutParams(850, 500);
        公告.setLayoutParams(文本params);
        文本.setText("注册主页");
        文本.setTextColor(0xFF000000);
        文本.setTextSize(15);
        文本.setGravity(Gravity.CENTER);
        公告.addView(文本);

        LinearLayout 分界线 = new LinearLayout(mContext);
        LayoutParams 分界线_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线.setLayoutParams(分界线_params);
        分界线.setBackgroundColor(0xFF000000);
        分界线_params.leftMargin = 100;
        分界线_params.rightMargin = 100;
        公告.addView(分界线);

        TextView 文本2 = new TextView(mContext);
        LayoutParams 文本2params = new LayoutParams(850, 500);
        公告.setLayoutParams(文本2params);
        文本2.setText(">狗子注册福利<\n账号注册随机赠送时间");
        文本2.setTextColor(0xFF000000);
        文本2.setTextSize(15);
        文本2.setGravity(Gravity.CENTER);
        公告.addView(文本2);

        LinearLayout 分界线2 = new LinearLayout(mContext);
        LayoutParams 分界线2_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线2.setLayoutParams(分界线2_params);
        分界线2.setBackgroundColor(0xFF000000);
        分界线2_params.leftMargin = 100;
        分界线2_params.rightMargin = 100;
        公告.addView(分界线2);

        LinearLayout 邮箱布局 = new LinearLayout(mContext);
        LayoutParams 邮箱params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        邮箱布局.setLayoutParams(邮箱params);
        邮箱布局.setOrientation(LinearLayout.HORIZONTAL);
        邮箱布局.setGravity(Gravity.CENTER);
        邮箱布局.setBackground(卡密圆角);
        邮箱params.leftMargin = 50;
        邮箱params.rightMargin = 50;
        邮箱params.topMargin = 50;
        layout.addView(邮箱布局);


        ImageView 邮箱前图 = new ImageView(mContext);
        邮箱前图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream mInputStream = mContext.getAssets().open("tx.png");
            Bitmap bitmap = BitmapFactory.decodeStream(mInputStream);
            邮箱前图.setImageBitmap(bitmap);
        } catch (IOException e) {}
        邮箱布局.addView(邮箱前图);

        final EditText 邮箱框 = new EditText(mContext);
        LayoutParams 邮箱间距 = new LayoutParams(700, LayoutParams.WRAP_CONTENT);
        邮箱框.setLayoutParams(邮箱间距);
        邮箱框.setSingleLine(true);
        邮箱框.setTextColor(0xFF000000);
        邮箱框.setText(Tools.读取文件("/storage/emulated/0/狗子/zh"));
        邮箱框.setHint("请输入账号......");
        邮箱框.setHintTextColor(0xFF000000);
        邮箱框.setBackground(卡密框圆角);
        邮箱布局.addView(邮箱框);

        final ImageView 邮箱图 = new ImageView(mContext);
        邮箱图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t2 = mContext.getAssets().open("qc.png");
            Bitmap p2 = BitmapFactory.decodeStream(t2);
            邮箱图.setImageBitmap(p2);
        } catch (IOException e) {}
        邮箱布局.addView(邮箱图);

        LinearLayout 卡密布局 = new LinearLayout(mContext);
        LayoutParams 卡密params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        卡密布局.setLayoutParams(卡密params);
        卡密布局.setOrientation(LinearLayout.HORIZONTAL);
        卡密布局.setGravity(Gravity.CENTER);
        卡密布局.setBackground(卡密圆角);
        卡密params.leftMargin = 50;
        卡密params.rightMargin = 50;
        卡密params.topMargin = 50;
        layout.addView(卡密布局);


        ImageView 卡密前图 = new ImageView(mContext);
        卡密前图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t3 = mContext.getAssets().open("s.png");
            Bitmap p3 = BitmapFactory.decodeStream(t3);
            卡密前图.setImageBitmap(p3);
        } catch (IOException e) {}
        卡密布局.addView(卡密前图);

        final EditText 卡密框 = new EditText(mContext);
        LayoutParams 卡密间距 = new LayoutParams(700, LayoutParams.WRAP_CONTENT);
        卡密框.setLayoutParams(卡密间距);
        卡密框.setSingleLine(true);
        卡密框.setTextColor(0xFF000000);
        卡密框.setText(Tools.读取文件("/storage/emulated/0/狗子/mm"));
        卡密框.setHint("请输入密码......");
        卡密框.setHintTextColor(0xFF000000);
        卡密框.setBackground(卡密框圆角);
        卡密布局.addView(卡密框);

        final ImageView 卡密图 = new ImageView(mContext);
        卡密图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t3 = mContext.getAssets().open("xs.png");
            Bitmap p3 = BitmapFactory.decodeStream(t3);
            卡密图.setImageBitmap(p3);
        } catch (IOException e) {}
        卡密布局.addView(卡密图);






        LinearLayout yxa布局 = new LinearLayout(mContext);
        LayoutParams yxaparams = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        yxa布局.setLayoutParams(yxaparams);
        yxa布局.setOrientation(LinearLayout.HORIZONTAL);
        yxa布局.setGravity(Gravity.CENTER);
        yxa布局.setBackground(卡密圆角);
        yxaparams.leftMargin = 50;
        yxaparams.rightMargin = 50;
        yxaparams.topMargin = 50;
        layout.addView(yxa布局);


        ImageView 邮箱前图a = new ImageView(mContext);
        邮箱前图a.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t3 = mContext.getAssets().open("youxiang.png");
            Bitmap p3 = BitmapFactory.decodeStream(t3);
            邮箱前图a.setImageBitmap(p3);
        } catch (IOException e) {}
        yxa布局.addView(邮箱前图a);

        final EditText 邮箱框a = new EditText(mContext);
        LayoutParams 邮箱框a间距 = new LayoutParams(700, LayoutParams.WRAP_CONTENT);
        邮箱框a.setLayoutParams(邮箱框a间距);
        邮箱框a.setSingleLine(true);
        邮箱框a.setTextColor(0xFF000000);
        邮箱框a.setHint("请输入邮箱......");
        邮箱框a.setHintTextColor(0xFF000000);
        邮箱框a.setBackground(卡密框圆角);
        yxa布局.addView(邮箱框a);




        Button 注册按钮 = new Button(mContext);
        注册按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 注册间距= (LayoutParams) 注册按钮.getLayoutParams();
        注册按钮.setTextColor(0xFF000000);
        注册按钮.setTextSize(15);
        注册按钮.setGravity(Gravity.CENTER);
        注册按钮.setBackground(按钮圆角);
        注册按钮.setText("注册");
        注册间距.leftMargin = 150;
        注册间距.rightMargin = 150;
        注册间距.topMargin = 50;
        layout.addView(注册按钮);

        Button 返回按钮 = new Button(mContext);
        返回按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 返回间距= (LayoutParams) 返回按钮.getLayoutParams();
        返回按钮.setTextColor(0xFF000000);
        返回按钮.setTextSize(15);
        返回按钮.setGravity(Gravity.CENTER);
        返回按钮.setBackground(按钮圆角);
        返回按钮.setText("返 回");
        返回间距.leftMargin = 150;
        返回间距.rightMargin = 150;
        返回间距.topMargin = 50;
        layout.addView(返回按钮);

        setContentView(layout);

        注册按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {

                    账号 = 邮箱框.getText().toString();
                    密码 = 卡密框.getText().toString();
                    邮箱 = 邮箱框a.getText().toString();
                    机器码 = Settings.System.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
                    接口 = "UserName=" + 账号 + "&UserPwd=" + 密码 +"&Email=" +邮箱+"&Mac=" + 机器码;
                    new Thread(new D()){}.start();
                }



            });


        返回按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Main();
                }
            });


        卡密图.setOnClickListener(new View.OnClickListener() {

                private boolean 开关=true;
                @Override
                public void onClick(View v) {
                    if(开关){
                        //如果选中，显示密码
                        try {
                            InputStream t3 = mContext.getAssets().open("xs.png");
                            Bitmap p3 = BitmapFactory.decodeStream(t3);
                            卡密图.setImageBitmap(p3);
                        } catch (IOException e) {}
                        卡密框.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                        开关=false;
                    }else{
                        //否则隐藏密
                        try {
                            InputStream t3 = mContext.getAssets().open("yc.png");
                            Bitmap p3 = BitmapFactory.decodeStream(t3);
                            卡密图.setImageBitmap(p3);
                        } catch (IOException e) {}
                        卡密框.setTransformationMethod(PasswordTransformationMethod.getInstance());
                        开关=true;

                    }

                }
            });
        邮箱图.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View view) {
                    邮箱框.setText("");
                }

            });
    }

    private void Login页面()
    {
        this.mContext = this;

        GradientDrawable 公告圆角 = new GradientDrawable();
        公告圆角.setCornerRadius(25);
        公告圆角.setColor(0xff69c5ff);
        公告圆角.setStroke(5, 0xFF000000);

        GradientDrawable 卡密圆角 = new GradientDrawable();
        卡密圆角.setCornerRadius(25);
        卡密圆角.setColor(0xfff5f5f7);
        卡密圆角.setStroke(3, 0xFF000000);

        验证码圆角 = new GradientDrawable();
        验证码圆角.setCornerRadius(25);
        验证码圆角.setColor(0xfff5f5f7);
        验证码圆角.setStroke(3, 0xFF000000);

        点击圆角 = new GradientDrawable();
        点击圆角.setCornerRadius(25);
        点击圆角.setColor(0xfff5f5f7);
        点击圆角.setStroke(3, 0xFF000000);

        GradientDrawable 卡密框圆角 = new GradientDrawable();


        GradientDrawable 按钮圆角 = new GradientDrawable();
        按钮圆角.setColor(0xff69c4ff);
        按钮圆角.setCornerRadius(25);
        按钮圆角.setStroke(3, 0xFF000000);

        验证码按钮圆角 = new GradientDrawable();
        验证码按钮圆角.setColor(0xff69c4ff);
        验证码按钮圆角.setCornerRadius(25);
        验证码按钮圆角.setStroke(3, 0xFF000000);

        LinearLayout layout = new LinearLayout(mContext);
        LayoutParams params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        layout.setLayoutParams(params);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setBackgroundColor(0xffb5d4ff);
        layout.setGravity(Gravity.CENTER);



        LinearLayout 公告 = new LinearLayout(mContext);
        LayoutParams 公告params = new LayoutParams(850, 500);
        公告.setLayoutParams(公告params);
        公告.setOrientation(LinearLayout.VERTICAL);
        公告.setBackground(公告圆角);
        公告.setGravity(Gravity.CENTER);
        layout.addView(公告);

        TextView 文本 = new TextView(mContext);
        LayoutParams 文本params = new LayoutParams(850, 500);
        公告.setLayoutParams(文本params);
        文本.setText(">>>程序公告<<<");
        文本.setTextColor(0xFF000000);
        文本.setTextSize(15);
        文本.setGravity(Gravity.CENTER);
        公告.addView(文本);

        LinearLayout 分界线 = new LinearLayout(mContext);
        LayoutParams 分界线_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线.setLayoutParams(分界线_params);
        分界线.setBackgroundColor(0xFF000000);
        分界线_params.leftMargin = 100;
        分界线_params.rightMargin = 100;
        公告.addView(分界线);

        TextView 充值公告文本 = new TextView(mContext);
        LayoutParams 充值公告文本params = new LayoutParams(850, 500);
        公告.setLayoutParams(充值公告文本params);
        充值公告文本.setText(gonggao文本);
        充值公告文本.setTextColor(0xFF000000);
        充值公告文本.setTextSize(15);
        充值公告文本.setGravity(Gravity.CENTER);
        公告.addView(充值公告文本);

        LinearLayout 分界线2 = new LinearLayout(mContext);
        LayoutParams 分界线2_params = new LayoutParams(LayoutParams.MATCH_PARENT, 5);
        分界线2.setLayoutParams(分界线2_params);
        分界线2.setBackgroundColor(0xFF000000);
        分界线2_params.leftMargin = 100;
        分界线2_params.rightMargin = 100;
        公告.addView(分界线2);

        LinearLayout 邮箱布局 = new LinearLayout(mContext);
        LayoutParams 邮箱params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        邮箱布局.setLayoutParams(邮箱params);
        邮箱布局.setOrientation(LinearLayout.HORIZONTAL);
        邮箱布局.setGravity(Gravity.CENTER);
        邮箱布局.setBackground(卡密圆角);
        邮箱params.leftMargin = 50;
        邮箱params.rightMargin = 50;
        邮箱params.topMargin = 50;
        layout.addView(邮箱布局);


        ImageView 邮箱前图 = new ImageView(mContext);
        邮箱前图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream mInputStream = mContext.getAssets().open("tx.png");
            Bitmap bitmap = BitmapFactory.decodeStream(mInputStream);
            邮箱前图.setImageBitmap(bitmap);
        } catch (IOException e) {}
        邮箱布局.addView(邮箱前图);

        final EditText 邮箱框 = new EditText(mContext);
        LayoutParams 邮箱间距 = new LayoutParams(700, LayoutParams.WRAP_CONTENT);
        邮箱框.setLayoutParams(邮箱间距);
        邮箱框.setSingleLine(true);
        邮箱框.setTextColor(0xFF000000);
        邮箱框.setHint("请输入账号......");
        邮箱框.setText(Tools.读取文件("/storage/emulated/0/狗子/zh"));
        邮箱框.setHintTextColor(0xFF000000);
        邮箱框.setBackground(卡密框圆角);
        邮箱布局.addView(邮箱框);

        final ImageView 邮箱图 = new ImageView(mContext);
        邮箱图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t2 = mContext.getAssets().open("qc.png");
            Bitmap p2 = BitmapFactory.decodeStream(t2);
            邮箱图.setImageBitmap(p2);
        } catch (IOException e) {}
        邮箱布局.addView(邮箱图);

        LinearLayout 卡密布局 = new LinearLayout(mContext);
        LayoutParams 卡密params = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT);
        卡密布局.setLayoutParams(卡密params);
        卡密布局.setOrientation(LinearLayout.HORIZONTAL);
        卡密布局.setGravity(Gravity.CENTER);
        卡密布局.setBackground(卡密圆角);
        卡密params.leftMargin = 50;
        卡密params.rightMargin = 50;
        卡密params.topMargin = 50;
        layout.addView(卡密布局);


        ImageView 卡密前图 = new ImageView(mContext);
        卡密前图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t3 = mContext.getAssets().open("s.png");
            Bitmap p3 = BitmapFactory.decodeStream(t3);
            卡密前图.setImageBitmap(p3);
        } catch (IOException e) {}
        卡密布局.addView(卡密前图);

        final EditText 卡密框 = new EditText(mContext);
        LayoutParams 卡密间距 = new LayoutParams(700, LayoutParams.WRAP_CONTENT);
        卡密框.setLayoutParams(卡密间距);
        卡密框.setSingleLine(true);
        卡密框.setText(Tools.读取文件("/storage/emulated/0/狗子/mm"));
        卡密框.setTextColor(0xFF000000);
        卡密框.setHint("请输入密码......");
        卡密框.setHintTextColor(0xFF000000);
        卡密框.setBackground(卡密框圆角);
        卡密布局.addView(卡密框);

        final ImageView 卡密图 = new ImageView(mContext);
        卡密图.setLayoutParams(new LayoutParams(100, LayoutParams.MATCH_PARENT));
        try {
            InputStream t3 = mContext.getAssets().open("xs.png");
            Bitmap p3 = BitmapFactory.decodeStream(t3);
            卡密图.setImageBitmap(p3);
        } catch (IOException e) {}
        卡密布局.addView(卡密图);









        Button dengluui = new Button(mContext);
        dengluui.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams dengluui间距= (LayoutParams) dengluui.getLayoutParams();
        dengluui.setTextColor(0xFF000000);
        dengluui.setTextSize(15);
        dengluui.setGravity(Gravity.CENTER);
        dengluui.setBackground(按钮圆角);
        dengluui.setText("登 录");
        dengluui间距.leftMargin = 150;
        dengluui间距.rightMargin = 150;
        dengluui间距.topMargin = 50;
        layout.addView(dengluui);

        Button 返回按钮 = new Button(mContext);
        返回按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        LayoutParams 返回间距= (LayoutParams) 返回按钮.getLayoutParams();
        返回按钮.setTextColor(0xFF000000);
        返回按钮.setTextSize(15);
        返回按钮.setGravity(Gravity.CENTER);
        返回按钮.setBackground(按钮圆角);
        返回按钮.setText("返 回");
        返回间距.leftMargin = 150;
        返回间距.rightMargin = 150;
        返回间距.topMargin = 50;
        layout.addView(返回按钮);

		Button 登录按钮 = new Button(mContext);
		登录按钮.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
		LinearLayout.LayoutParams 登录间距= (LinearLayout.LayoutParams) 登录按钮.getLayoutParams();
		登录按钮.setTextColor(0xFF00F0F0);
		登录按钮.setTextSize(15);
		登录按钮.setGravity(Gravity.CENTER);
		登录按钮.setBackground(按钮圆角);
		登录按钮.setText("进入游戏");
		登录间距.leftMargin = 150;
		登录间距.rightMargin = 150;
		登录间距.topMargin = 50;
        layout.addView(登录按钮);

        setContentView(layout);

        dengluui.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {






                    账号 = 邮箱框.getText().toString();
                    密码 = 卡密框.getText().toString();
                    if (邮箱框.length() == 0) {
                        Toast.makeText(mContext, "请输入账号", Toast.LENGTH_SHORT).show();
                    } 
                    else {
                        返回值="UserName="+邮箱框;
                        机器码 = Settings.System.getString(getContentResolver(), Settings.Secure.ANDROID_ID);
                        接口 = "UserName=" + 账号 + "&UserPwd=" + 密码 + "&Version=" + 版本号 + "&Mac=" + 机器码 + "&MacTwo=" + 机器码;
                        Start();

                    }

                }
            });


        返回按钮.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {

                    Main();
                }
            });
		登录按钮.setOnClickListener(new View.OnClickListener() {

                @Override 
                public void onClick(View view) {
					



					Intent intent = new Intent();
					intent.setClassName("com.tencent.tmgp.pubgmhd","com.epicgames.ue4.SplashActivity");//
					startActivity(intent);

				}
			});
		
		
			
        卡密图.setOnClickListener(new View.OnClickListener() {

                private boolean 开关=true;
                @Override
                public void onClick(View v) {
                    if(开关){
                        //如果选中，显示密码
                        try {
                            InputStream t3 = mContext.getAssets().open("xs.png");
                            Bitmap p3 = BitmapFactory.decodeStream(t3);
                            卡密图.setImageBitmap(p3);
                        } catch (IOException e) {}
                        卡密框.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
                        开关=false;
                    }else{
                        //否则隐藏密
                        try {
                            InputStream t3 = mContext.getAssets().open("yc.png");
                            Bitmap p3 = BitmapFactory.decodeStream(t3);
                            卡密图.setImageBitmap(p3);
                        } catch (IOException e) {}
                        卡密框.setTransformationMethod(PasswordTransformationMethod.getInstance());
                        开关=true;

                    }

                }
            });
        邮箱图.setOnClickListener(new View.OnClickListener() {

                @Override
                public void onClick(View view) {
                    邮箱框.setText("");
                }

            });
    }


    public static boolean 写出assets资源文件(Context context, String outPath, String fileName) {
        File file = new File(outPath);
        if (!file.exists()) {
            if (!file.mkdirs()) {
                Log.e("--Method--", "copyAssetsSingleFile: cannot create directory.");
                return false;
            }
        }
        try {
            InputStream inputStream = context.getAssets().open(fileName);
            File outFile = new File(file, fileName);
            FileOutputStream fileOutputStream = new FileOutputStream(outFile);
            // Transfer bytes from inputStream to fileOutputStream
            byte[] buffer = new byte[1024];
            int byteRead;
            while (-1 != (byteRead = inputStream.read(buffer))) {
                fileOutputStream.write(buffer, 0, byteRead);
            }
            inputStream.close();
            fileOutputStream.flush();
            fileOutputStream.close();
            return true;
        } catch (IOException e) {

            return false;
        }
    }

    public static void RunShell(String shell) {
        String s = shell;

        try {
            Runtime.getRuntime().exec(s, null, null);//执行
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void checkNeedPermissions() {
        //6.0以上需要动态申请权限
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED
            || ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED) {
            //多个权限一起申请
            ActivityCompat.requestPermissions(this, new String[]{
                                                  Manifest.permission.WRITE_EXTERNAL_STORAGE,
                                                  Manifest.permission.READ_EXTERNAL_STORAGE
                                              }, 1);
        }
    }


    public static void RoundCard(View id, int inColor , int width, int color, float radius) {
        GradientDrawable background = new GradientDrawable();
        background.setColor(inColor);
        background.setStroke(width, color);
        background.setCornerRadius(radius);
        id.setBackgroundDrawable(background);
    }

    private void showToast(String str) {
        Toast.makeText(mContext, str, Toast.LENGTH_LONG).show();    
    }

    public static String readtext(String strFilePath) {
        String path = strFilePath;
        String content = ""; //文件内容字符串
        //打开文件
        File file = new File(path);
        //如果path是传递过来的参数，可以做一个非目录的判断
        if (file.isDirectory()) {
        } else {
            try {
                InputStream instream = new FileInputStream(file); 
                if (instream != null) {
                    InputStreamReader inputreader = new InputStreamReader(instream);
                    BufferedReader buffreader = new BufferedReader(inputreader);
                    String line;
                    //分行读取
                    while ((line = buffreader.readLine()) != null) {
                        content += line ;
                    }                
                    instream.close();
                }
            } catch (java.io.FileNotFoundException e) {
            } catch (IOException e) {
            }
        }
        return content;
    }


    Handler hd=new Handler(){

        @Override
        public void handleMessage(Message msg) {

            super.handleMessage(msg);

            switch (msg.what) {
                case 1:

                    if (返回.equals("-1")) {
                        showToast("网络链接失败.");
                    } else if (返回.equals("-2")) {
                        showToast("请填写程序密钥.");
                    } else if (返回.equals("-3")) {
                        showToast("数据异常.");
                    } else if (返回.equals("-4")) {
                        showToast("数据异常.");
                    } else if (返回.equals("-5")) {
                        showToast("错误的参数,请检查参数是否正确.");
                    } else if (返回.equals("-6")) {
                        showToast("还未登录");
                    } else if (返回.equals("-7")) {
                        showToast("私人服务器,没有权限进行登录.");
                    } else if (返回.equals("-8")) {
                        showToast("账户余额不足.");
                    } else if (返回.equals("-9")) {
                        showToast("注册用户达到上限.");
                    } else if (返回.equals("-10")) {
                        showToast("VIP插件,非VIP无法使用.");
                    } else if (返回.equals("-11")) {
                        showToast("开启自动状态检测失败,还未登陆!");
                    } else if (返回.equals("-12")) {
                        showToast("开启自动状态检测失败!");
                    } else if (返回.equals("-13")) {
                        showToast("动态算法只支持独立服务器调用.");
                    } else if (返回.equals("-14")) {
                        showToast("错误的调用.");
                    } else if (返回.equals("-15")) {
                        showToast("频繁调用,请等待10分钟后再做尝试.");
                    } else if (返回.equals("-16")) {
                        showToast("接口未开启.");
                    } else if (返回.equals("-17")) {
                        showToast("错误的调用方式,请确认后台接口的调用方式.");
                    } else if (返回.equals("-18")) {
                        showToast("服务器内部错误,请联系管理员解决.");
                    } else if (返回.equals("-19")) {
                        showToast("接口调用失败,调用次数不足.");
                    } else if (返回.equals("-20")) {
                        showToast("变量数据不存在.");
                    } else if (返回.equals("-21")) {
                        showToast("机器码一样,无需转绑.");
                    } else if (返回.equals("-23")) {
                        showToast("此接口开启了强制算法,但是没使用.");
                    } else if (返回.equals("-101")) {
                        showToast("用户名填写错误,必须以字母开头6-16位字母或数字!");
                    } else if (返回.equals("-102")) {
                        showToast("用户不存在.");
                    } else if (返回.equals("-103")) {
                        showToast("请先登陆再调用此方法.");
                    } else if (返回.equals("6585")) {
                        showToast("密码填写错误,请输入6-16位密码!");
                    } else if (返回.equals("-105")) {
                        showToast("邮箱填写错误,请正确输入邮箱!");
                    } else if (返回.equals("-106")) {
                        showToast("用户名重复.");
                    } else if (返回.equals("-107")) {
                        showToast("邮箱重复.");
                    } else if (返回.equals("-108")) {
                        showToast("新密码输入错误.");
                    } else if (返回.equals("-109")) {
                        showToast("密码错误");
                    } else if (返回.equals("-110")) {
                        showToast("用户使用时间已到期,请充值");
                    } else if (返回.equals("-111")) {
                        showToast("用户未在绑定的设备上登陆.");
                    } else if (返回.equals("-112")) {
                        showToast("用户在别的地方登陆.");
                    } else if (返回.equals("-113")) {
                        showToast("过期时间有误.");
                    } else if (返回.equals("-114")) {
                        showToast("登录数据不存在");
                    } else if (返回.equals("-115")) {
                        showToast("用户已被禁用.");
                    } else if (返回.equals("-116")) {
                        showToast("密码修改申请过于频繁.");
                    } else if (返回.equals("-117")) {
                        showToast("未输入机器码.");
                    } else if (返回.equals("-118")) {
                        showToast("重绑次数超过限制.");
                    } else if (返回.equals("-119")) {
                        showToast("使用天数不足,重绑失败.");
                    } else if (返回.equals("-120")) {
                        showToast("注册失败,注册次数超过限制.");
                    } else if (返回.equals("-121")) {
                        showToast("用户机器码不能超过32位.");
                    } else if (返回.equals("-122")) {
                        showToast("用户已经被删除");
                    } else if (返回.equals("-123")) {
                        showToast("用户密码输入错误");
                    } else if (返回.equals("-124")) {
                        showToast("用户登录数达到最大");
                    } else if (返回.equals("-125")) {
                        showToast("错误的用户操作类别");
                    } else if (返回.equals("-126")) {
                        showToast("过期时间变更记录创建失败");
                    } else if (返回.equals("-127")) {
                        showToast("用户充值失败");
                    } else if (返回.equals("-128")) {
                        showToast("用户数据超过最大限制");
                    } else if (返回.equals("-129")) {
                        showToast("用户被开发者禁止使用,请咨询开发者是否被拉到黑名单");
                    } else if (返回.equals("-131")) {
                        showToast("用户使用次数不足");
                    } else if (返回.equals("-132")) {
                        showToast("用户使用点数不足");
                    } else if (返回.equals("-201")) {
                        showToast("程序不存在");
                    } else if (返回.equals("-202")) {
                        showToast("程序密钥输入错误");
                    } else if (返回.equals("-203")) {
                        showToast("程序版本号错误");
                    } else if (返回.equals("-204")) {
                        showToast("程序版本不存在");
                    } else if (返回.equals("-205")) {
                        showToast("用户未申请使用程序");
                    } else if (返回.equals("-206")) {
                        showToast("程序版本需要更新");
                    } else if (返回.equals("-207")) {
                        showToast("程序版本已停用");
                    } else if (返回.equals("-208")) {
                        showToast("程序未开启后台接口功能.(可在“程序”的“修改”界面开启后台接口功能)");
                    } else if (返回.equals("-209")) {
                        showToast("程序接口密码错误");
                    } else if (返回.equals("-210")) {
                        showToast("程序停止新用户注册");
                    } else if (返回.equals("-211")) {
                        showToast("程序不允许用户机器码转绑");
                    } else if (返回.equals("-301")) {
                        showToast("卡密输入错误");
                    } else if (返回.equals("-302")) {
                        showToast("卡密不存在");
                    } else if (返回.equals("-303")) {
                        showToast("卡密已经使用");
                    } else if (返回.equals("-304")) {
                        showToast("卡密已经过期");
                    } else if (返回.equals("-305")) {
                        showToast("卡密已经冻结");
                    } else if (返回.equals("-306")) {
                        showToast("卡密已经退换");
                    } else if (返回.equals("-308")) {
                        showToast("卡密已经换卡");
                    } else if (返回.equals("-401")) {
                        showToast("单码卡密错误");
                    } else if (返回.equals("-402")) {
                        showToast("单码卡密机器码错误");
                    } else if (返回.equals("-403")) {
                        showToast("单码卡密IP错误");
                    } else if (返回.equals("404")) {
                        showToast("单码卡密类型错误");
                    } else if (返回.equals("405")) {
                        showToast("单码卡密被禁用");
                    } else if (返回.equals("406")) {
                        showToast("单码卡密不存在");
                    } else if (返回.equals("407")) {
                        showToast("单码卡密未激活");
                    } else if (返回.equals("408")) {
                        showToast("单码卡密已经使用");
                    } else if (返回.equals("409")) {
                        showToast("单码充值卡密错误");
                    } else if (返回.equals("410")) {
                        showToast("单码卡密过期");
                    } else if (返回.equals("420")) {
                        showToast("单码卡密在别的设备上登录");
                    } else if (返回.equals("421")) {
                        showToast("单码卡密超过最大登录数,如果确定已经下线,请等60分钟后重试");
                    } else if (返回.equals("422")) {
                        showToast("单码IP一样,无需转绑");
                    } else if (返回.equals("-501")) {
                        showToast("单码管理信息错误");
                    } else if (返回.equals("-502")) {
                        showToast("单码机器码转绑次数超过限制");
                    } else if (返回.equals("-503")) {
                        showToast("单码机器码转绑后将过期");
                    } else if (返回.equals("-504")) {
                        showToast("单码IP转绑次数超过限制");
                    } else if (返回.equals("-505")) {
                        showToast("单码IP转绑后将过期");
                    } else if (返回.equals("-506")) {
                        showToast("单码未开启机器码验证,无需转绑.");
                    } else if (返回.equals("-507")) {
                        showToast("单码未开启IP地址验证,无需转绑");
                    } else if (返回.equals("101")) {
                        showToast("充值成功!填写的推荐人不存在");
                    } else if (返回.equals("102")) {
                        showToast("充值成功!填写推荐人获赠时间失败");
                    } else if (返回.equals("103")) {
                        showToast("充值成功!添加推荐信息失败");
                    } else if (返回.equals("104")) {
                        showToast("充值成功!推荐人获赠时间失败");
                    } else if (返回.equals("105")) {
                        showToast("充值成功!充值的卡密类别不支持推荐人功能");
                    } else if (返回.equals("106")) {
                        showToast("充值成功!充值的卡密类别推荐功能已关闭");
                    } else if (返回.equals("107")) {
                        showToast("充值成功!成功使用推荐功能");
                    } else if (返回.equals("108")) {
                        showToast("充值成功!但是填写的推荐人无效");

                    }
                    //返回成功
                    else if (返回.equals("1")) {

                        showToast("注册成功");
                    }
                    else if(返回.length() == 32){
                        showToast("IMGUI窗口创建成功");
                        Tools.写入文件("/storage/emulated/0/狗子/zh",账号);
                        Tools.写入文件("/storage/emulated/0/狗子/id",账号);
                        Tools.写入文件("/storage/emulated/0/狗子/mm",密码);
                        Tools.写入文件("/storage/emulated/0/狗子/jqm",机器码);
                        Start();

                    }
                    break;
                case 2:
                    if (返回.equals("1")) {
                        Toast.makeText(MainActivity.this, "重置密码成功", Toast.LENGTH_LONG).show();
                    } else if (返回.equals("-116")) {
                        showToast("密码修改申请过于频繁.");
                    } else {                   
                        Toast.makeText(MainActivity.this, "重置密码失败", Toast.LENGTH_LONG).show();
                    }

                    break;
                case 3:

                    if (返回3.equals("-21")) {
                        Toast.makeText(MainActivity.this, "解绑成功", Toast.LENGTH_LONG).show();
                    } else if (返回.equals("-21")) {
                        showToast("机器码一样,无需转绑.");
                    }
                    if (返回3.equals("-118")) {
                        Toast.makeText(MainActivity.this, "重绑次数超过限制.", Toast.LENGTH_LONG).show();
                    }
                    if (返回3.equals("-117")) {
                        Toast.makeText(MainActivity.this, "卡密未绑定机械码.", Toast.LENGTH_LONG).show();
                    }                     
                    break;
                case 4:  
                    if (返回.equals("1")) {
                        Toast.makeText(MainActivity.this, "密码已发送至您邮箱", Toast.LENGTH_LONG).show();
                    }   else if (返回.equals("0")) {
                        Toast.makeText(MainActivity.this, "账号/邮箱错误", Toast.LENGTH_LONG).show();
                    } else if (返回.equals("-101")) {
                        showToast("用户名填写错误,必须以字母开头6-16位字母或数字!");              } else if (返回.equals("6585")) {
                        showToast("密码填写错误,请输入6-16位密码!");
                    } 
                    else
                    {
                        Toast.makeText(MainActivity.this, "服务器错误", Toast.LENGTH_LONG).show(); 
                    }                               
                    break;
                case 5:  

                    if (返回.equals("1")) {
                        alertDialogs.dismiss();
                        Toast.makeText(MainActivity.this, "版本为最新版本", Toast.LENGTH_LONG).show();
                    }  else if (返回.equals("0")) {
                        alertDialogs.dismiss();
                        Toast.makeText(MainActivity.this, "版本为旧版本", Toast.LENGTH_LONG).show();
                        更新页面();
                    } else {  
                        alertDialogs.dismiss();
                        Toast.makeText(MainActivity.this, "版本停用或服务器错误", Toast.LENGTH_LONG).show();             
                        更新页面();
                    }
                    break;
                case 6:  
                    if (返回.equals("1")) {
                        Toast.makeText(MainActivity.this, "退出登录成功", Toast.LENGTH_LONG).show();
                    }  else if (返回.equals("-101")) {
                        showToast("用户名填写错误,必须以字母开头6-16位字母或数字!"); 
                    }  else if (返回.equals("-103")) {
                        showToast("您还没有登录呢");
                    } else if (返回.equals("-112")) {
                        showToast("可能用户不存在>或您无权利退出登录");
                    }   else  {

                    }
                    break;
                case 7:  
                    if (返回.equals("1")) {
                        Toast.makeText(MainActivity.this, "转绑成功", Toast.LENGTH_LONG).show();
                    }  else  if (返回.equals("-118")) {
                        Toast.makeText(MainActivity.this, "重绑次数超过限制.", Toast.LENGTH_LONG).show();
                    }  else  if (返回.equals("-101")) {
                        Toast.makeText(MainActivity.this, "用户名填写错误,必须以字母开头6-16位字母或数字!", Toast.LENGTH_LONG).show();
                    }  else  if (返回.equals("-123")) {
                        Toast.makeText(MainActivity.this, "用户密码输入错误.", Toast.LENGTH_LONG).show();
                    } else  if (返回.equals("-102")) {
                        Toast.makeText(MainActivity.this, "用户不存在.", Toast.LENGTH_LONG).show();
                    }  else  if (返回.equals("-117")) {
                        Toast.makeText(MainActivity.this, "账号未绑定机械码.", Toast.LENGTH_LONG).show();
                    } else if (返回.equals("-21")) {
                        showToast("机器码一样,无需转绑.");
                    } else 
                    {
                        Toast.makeText(MainActivity.this, "未知错误.", Toast.LENGTH_LONG).show(); 
                    }
                    break;
                case 8:
                    if(返回.equals("1")) {
                        Toast.makeText(MainActivity.this, "数据发送成功/作者会留意", Toast.LENGTH_LONG).show();
                    }else if (返回.equals("-101")) {
                        showToast("用户名填写错误,必须以字母开头6-16位字母或数字!");
                    }  else if (返回.equals("-103")) {
                        showToast("请先登陆您的账号");
                    } else if (返回.equals("-216")) {
                        showToast("文本内容涉敏感词汇");
                    } else if (返回.equals("-114")) {
                        showToast("请先登录账号");
                    } 
                    else
                    {
                        Toast.makeText(MainActivity.this, "数据发送失败/无法解析", Toast.LENGTH_LONG).show();
                    }
                    break;
                case 9:
                    if (返回查询.equals("-101")) {
                        showToast("用户名填写错误,必须以字母开头6-16位字母或数字!");
                    }  else if (返回查询.equals("-103")) {
                        showToast("请先登陆您的账号");
                    } 
                    break;
                case 10:
                    if (返回.equals("1")) {
                        showToast("清除成功");
                    } else if (返回.equals("-101")) {
                        showToast("用户名填写错误,必须以字母开头6-16位字母或数字!");
                    }  else  {
                        showToast("服务器繁忙/错误/未知错误");
                    } 
                    break;
                case 11:
                    if (返回.equals("-301")) {
                        showToast("充值卡输入有误");
                    }  else  {

                    } 
                    break;
                case 12:
                    if (返回.equals("-123")) {
                        showToast("用户密码输入错误");
                    }  else if (返回.equals("-101")) {
                        showToast("用户名填写错误,必须以字母开头6-16位字母或数字!");
                    } else if (返回.equals("-134")) {
                        showToast("用户不存在");
                    }  else {

                    } 
                    break;
                case 13:
                    if (返回.equals("-114")) {
                        showToast("数据不存在/已被清楚");
                    }  else if (返回.equals("-103")) {
                        showToast("请登录账号");
                    } else {

                    }

                    break;  

            }
        }
    };
    class J implements Runnable {

        @Override
        public void run() {
            try {
                //用户登录wep
                URL url=new URL("http://w.eydata.net/78E3EC3A8B37B52E");
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);//单位毫秒
                HttpURLConnection.setRequestMethod("POST");//方法:post
                OutputStream outputStream = HttpURLConnection.getOutputStream();
                String byteString="";
                byteString = 接口;
                outputStream.write(byteString.getBytes());//提交数据
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                while ((String = BufferedReader.readLine()) != null) {
                    StringBuffer.append(String);
                }
                返回 = StringBuffer.toString();
                转换a = StringBuffer.toString();
                Message ee=new Message();
                ee.what = 1;
                hd.sendMessage(ee);
            } catch (IOException e) {}

        }
    }
    class C implements Runnable {

        @Override
        public void run() {
            try {
                //充值wep
                URL url=new URL("http://w.eydata.net/08BA4AD8833DC7DD");
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);//单位毫秒
                HttpURLConnection.setRequestMethod("POST");//方法:post
                OutputStream outputStream = HttpURLConnection.getOutputStream();
                String byteString="";
                byteString = 接口;
                outputStream.write(byteString.getBytes());//提交数据
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                while ((String = BufferedReader.readLine()) != null) {
                    StringBuffer.append(String);
                }
                返回 = StringBuffer.toString();
                Message ee=new Message();
                ee.what = 1;
                hd.sendMessage(ee);
            } catch (IOException e) {}

        }
    }

    class D implements Runnable {

        @Override
        public void run() {
            try {
                //用户注册
                URL url=new URL("http://w.eydata.net/65A449ADCF744B5C");
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);//单位毫秒
                HttpURLConnection.setRequestMethod("POST");//方法:post
                OutputStream outputStream = HttpURLConnection.getOutputStream();
                String byteString="";
                byteString = 接口;
                outputStream.write(byteString.getBytes());//提交数据
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                while ((String = BufferedReader.readLine()) != null) {
                    StringBuffer.append(String);
                }
                返回 = StringBuffer.toString();
                Message ee=new Message();
                ee.what = 1;
                hd.sendMessage(ee);
            } catch (IOException e) {}

        }
    }
    class zxrs implements Runnable {

        @Override
        public void run() {
            try {
                //在线人数
                URL url=new URL("http://w.eydata.net/87A15A13293F99DC");
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);//单位毫秒
                HttpURLConnection.setRequestMethod("POST");//方法:post
                OutputStream outputStream = HttpURLConnection.getOutputStream();
                String byteString= "UserName=" + 账号备份 + "&Type=" + "3";               
                outputStream.write(byteString.getBytes());//提交数据
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                while ((String = BufferedReader.readLine()) != null) {
                    StringBuffer.append(String);
                }                
                返回在线 = StringBuffer.toString();
                //  
            } catch (IOException e) {}

        }
    }
    class GG implements Runnable {

        @Override
        public void run() {
            try {
                //程序公告wep
                URL url=new URL("http://w.eydata.net/A8E93FA5A81CA6AC");
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);//单位毫秒
                try {
                    HttpURLConnection.setRequestMethod("POST");
                } catch (ProtocolException e) {}//方法:post
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                try {
                    while ((String = BufferedReader.readLine()) != null) {
                        StringBuffer.append(String);
                    }
                } catch (IOException e) {}
                String 公告内容 = StringBuffer.toString();
                gonggao文本=公告内容;
                Tools.写入文件("/storage/emulated/0/狗子/gg",""+公告内容);

            } catch (IOException e) {}
        }
    }
    class banbenjiance implements Runnable {

        @Override
        public void run() {
            try {
                //版本检测
                URL url=new URL("http://w.eydata.net/DFCADCF064DDE025");
                HttpURLConnection HttpURLConnection=(HttpURLConnection) url.openConnection();
                HttpURLConnection.setReadTimeout(9000);//单位毫秒
                HttpURLConnection.setRequestMethod("POST");//方法:post
                OutputStream outputStream = HttpURLConnection.getOutputStream();
                String byteString="";
                byteString = "ver=" + 版本号;
                outputStream.write(byteString.getBytes());//提交数据
                BufferedReader BufferedReader=new BufferedReader(new InputStreamReader(HttpURLConnection.getInputStream()));
                String String="";
                StringBuffer StringBuffer=new StringBuffer();
                while ((String = BufferedReader.readLine()) != null) {
                    StringBuffer.append(String);
                }
                返回 = StringBuffer.toString();
                Message ee=new Message();
                ee.what = 5;
                hd.sendMessage(ee);
            } catch (IOException e) {}

        }
    }



    public static void 反馈联系(final Context context) {
        new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Looper.prepare();
                        linearLayouts = new LinearLayout(context);
                        linearLayouts.setOrientation(1);
                        linearLayouts.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
                        linearLayouts.setPadding(40, 40, 40, 40);
                        linearLayouts.setGravity(Gravity.CENTER);
                        dialogs = new AlertDialog.Builder(context, 5);

                        LinearLayout 公告 = new LinearLayout(context);
                        LayoutParams 公告params = new LayoutParams(850, 500);
                        公告.setLayoutParams(公告params);
                        公告.setOrientation(LinearLayout.VERTICAL);
                        公告.setGravity(Gravity.CENTER);












                        TextView wmsma = new TextView(context);
                        wmsma.setTextColor(Color.BLACK);
                        wmsma.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        wmsma.setText("TALK");
                        wmsma.setTextSize(20.0f);
                        wmsma.setGravity(5);
                        wmsma.setTextColor(0xFF999F9E);


                        GradientDrawable 角 = new GradientDrawable();

                        Button ekekdk按钮 = new Button(context);
                        ekekdk按钮.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        ekekdk按钮.setTextColor(Color.BLACK);
                        ekekdk按钮.setTextSize(15);
                        ekekdk按钮.setGravity(Gravity.CENTER);
                        ekekdk按钮.setBackground(角);
                        ekekdk按钮.setText("网盘链接");
                        公告.addView(ekekdk按钮);

/*
                        Button slwlsld按钮 = new Button(context);
                        slwlsld按钮.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        slwlsld按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
                        slwlsld按钮.setTextColor(Color.BLACK);
                        slwlsld按钮.setTextSize(15);
                        slwlsld按钮.setGravity(Gravity.CENTER);
                        slwlsld按钮.setBackground(角);
                        slwlsld按钮.setText("网盘链接");
                        公告.addView(slwlsld按钮);
*/

                        Button wpwprlro按钮 = new Button(context);
                        wpwprlro按钮.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        wpwprlro按钮.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
                        wpwprlro按钮.setTextColor(Color.BLACK);
                        wpwprlro按钮.setTextSize(15);
                        wpwprlro按钮.setGravity(Gravity.CENTER);
                        wpwprlro按钮.setBackground(角);
                        wpwprlro按钮.setText("文本信息-作者说明");
                        公告.addView(wpwprlro按钮);






                        final   TextView qpepdpdl = new TextView(context);

                        wpwprlro按钮.setOnClickListener(new View.OnClickListener() {

                                @Override 
                                public void onClick(View view) {

                                    qpepdpdl.setTextColor(Color.BLACK);
                                    qpepdpdl.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                                    qpepdpdl.setText("开发者：宇将军，本职专归属权为作者所有，禁止倒卖二改，");
                                    qpepdpdl.setTextSize(20.0f);
                                    qpepdpdl.setGravity(5);








                                }});
/*

                        slwlsld按钮.setOnClickListener(new View.OnClickListener() {
                                @Override 
                                public void onClick(View view) {
                                    Miscellaneous.联系QQ(context, "337769080");
                                    alertDialogs.dismiss();

                                }});
*/

                        ekekdk按钮.setOnClickListener(new View.OnClickListener() {
                                @Override 
                                public void onClick(View view) {


                                    alertDialogs.dismiss();
                                    Uri 链接=Uri.parse("https://www.123pan.com/s/sLELVv-XCgYd.html");
                                    Intent 打开链接=new Intent(Intent.ACTION_VIEW, 链接);
                                    context.startActivity(打开链接);





                                }});


                        linearLayouts.addView(wmsma);        
                        linearLayouts.addView(公告);
                        linearLayouts.addView(qpepdpdl);       
                        dialogs.setCancelable(false);
                        dialogs.setView(linearLayouts);
                        alertDialogs = dialogs.show();
                        Looper.loop();
                    } catch (Exception e) {
                    }
                }
            }).start(); 
    }




    public static void banbenjc(final Context context) {
        new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Looper.prepare();
                        linearLayouts = new LinearLayout(context);
                        linearLayouts.setOrientation(1);
                        linearLayouts.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
                        linearLayouts.setPadding(40, 40, 40, 40);
                        linearLayouts.setGravity(Gravity.CENTER);
                        dialogs = new AlertDialog.Builder(context, 5);

                        LinearLayout 公告 = new LinearLayout(context);
                        LayoutParams 公告params = new LayoutParams(850, 500);
                        公告.setLayoutParams(公告params);
                        公告.setOrientation(LinearLayout.VERTICAL);
                        公告.setGravity(Gravity.CENTER);












                        TextView wmsma = new TextView(context);
                        wmsma.setTextColor(Color.BLACK);
                        wmsma.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        wmsma.setText("(环境部署检测中)");
                        wmsma.setTextSize(23.0f);
                        wmsma.setGravity(5);
                        wmsma.setTextColor(0xFF999F9E);


                        final   ProgressBar amdlelwl = new ProgressBar(context);
                        LinearLayout.LayoutParams amdlelwlParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                        amdlelwl.setLayoutParams(amdlelwlParams);
                        公告.addView(amdlelwl);


                        final TextView texties = new TextView(context);
                        texties.setTextColor(Color.BLACK);
                        texties.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        texties.setText("狗子版本检测中");
                        texties.setTextSize(23.0f);
                        texties.setGravity(5);
                        公告.addView(texties);




                        //alertDialogs.dismiss();    
                        linearLayouts.addView(wmsma);        
                        linearLayouts.addView(公告);

                        dialogs.setCancelable(false);
                        dialogs.setView(linearLayouts);
                        alertDialogs = dialogs.show();
                        Looper.loop();
                    } catch (Exception e) {
                    }
                }
            }).start(); 
    }


    public static void 无网络(final Context context) {
        new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Looper.prepare();
                        linearLayouts = new LinearLayout(context);
                        linearLayouts.setOrientation(1);
                        linearLayouts.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
                        linearLayouts.setPadding(40, 40, 40, 40);
                        linearLayouts.setGravity(Gravity.CENTER);
                        dialogs = new AlertDialog.Builder(context, 5);

                        TextView wmsma = new TextView(context);
                        wmsma.setTextColor(Color.BLACK);
                        wmsma.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        wmsma.setText("无网络连接/请检测网络环境是否正常");
                        wmsma.setTextSize(20.0f);
                        wmsma.setGravity(5);
                        wmsma.setTextColor(0xFF999F9E);
                        linearLayouts.addView(wmsma);
                        dialogs.setCancelable(false);
                        dialogs.setView(linearLayouts);
                        alertDialogs = dialogs.show();
                        Looper.loop();
                    } catch (Exception e) {
                    }
                }
            }).start(); 
    }

    public static void apk检测哦(final Context context) {
        new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Looper.prepare();
                        linearLayouts = new LinearLayout(context);
                        linearLayouts.setOrientation(1);
                        linearLayouts.setLayoutParams(new LinearLayout.LayoutParams(-1, -1));
                        linearLayouts.setPadding(40, 40, 40, 40);
                        linearLayouts.setGravity(Gravity.CENTER);
                        dialogs = new AlertDialog.Builder(context, 5);

                        TextView wmsma = new TextView(context);
                        wmsma.setTextColor(Color.BLACK);
                        wmsma.setLayoutParams(new LinearLayout.LayoutParams(-2, -2));
                        wmsma.setText("请卸载破解软件>卸载后请重新打开");
                        wmsma.setTextSize(20.0f);
                        wmsma.setGravity(5);
                        wmsma.setTextColor(0xFF999F9E);
                        linearLayouts.addView(wmsma);
                        dialogs.setCancelable(false);
                        dialogs.setView(linearLayouts);
                        alertDialogs = dialogs.show();
                        Looper.loop();
                    } catch (Exception e) {
                    }
                }
            }).start(); 
    }


    public static boolean isVpnUsed() {//vpn检测
        try {
            List<NetworkInterface> nis = Collections.list(NetworkInterface.getNetworkInterfaces());
            if (nis != null) {
                for (NetworkInterface intf : nis) {
                    if (!intf.isUp() || intf.getInterfaceAddresses().size() == 0) {
                        continue;
                    }
                    if ("tun0".equals(intf.getName()) || "ppp0".equals(intf.getName())) {
                        return true; // The VPN is up
                    }
                }
            }
        } catch (Throwable e) {
            e.printStackTrace();
        }
        return false;
    }

    /*
     public static void loadImage(Context context, String assetsPath, String savePath){
     try {
     String fileNames[] = context.getAssets().list(assetsPath);// 获取assets目录下的所有文件及目录名
     if (fileNames.length > 0) {// 如果是目录
     File file = new File(savePath);
     if (!file.exists()) {
     file.mkdirs();// 如果文件夹不存在，则递归
     }
     for (String fileName : fileNames) {
     loadImage(context, assetsPath + "/" + fileName,
     savePath + "/" + fileName);
     }
     } else {// 如果是文件
     InputStream is = context.getAssets().open(assetsPath);
     FileOutputStream fos = new FileOutputStream(new File(savePath));
     byte[] buffer = new byte[1024];
     int byteCount = 0;
     while ((byteCount = is.read(buffer)) != -1) {// 循环从输入流读取
     // buffer字节
     fos.write(buffer, 0, byteCount);// 将读取的输入流写入到输出流
     }
     fos.flush();// 刷新缓冲区
     is.close();
     fos.close();
     }
     } catch (Exception e) {
     // TODO Auto-generated catch block
     e.printStackTrace();
     }
     }

     */


    public static void loadImage(Context context, String assetsPath, String savePath){
        try {
            String fileNames[] = context.getAssets().list(assetsPath);// 获取assets目录下的所有文件及目录名
            if (fileNames.length > 0) {// 如果是目录
                File file = new File(savePath);
                if (!file.exists()) {
                    file.mkdirs();// 如果文件夹不存在，则递归
                }
                for (String fileName : fileNames) {
                    loadImage(context, assetsPath + "/" + fileName,
                              savePath + "/" + fileName);
                }
            } else {// 如果是文件
                InputStream is = context.getAssets().open(assetsPath);
                FileOutputStream fos = new FileOutputStream(new File(savePath));
                byte[] buffer = new byte[1024];
                int byteCount = 0;
                while ((byteCount = is.read(buffer)) != -1) {// 循环从输入流读取
                    // buffer字节
                    fos.write(buffer, 0, byteCount);// 将读取的输入流写入到输出流
                }
                fos.flush();// 刷新缓冲区
                is.close();
                fos.close();
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }


    /**
     * 拷贝asset文件到指定路径，可变更文件名
     *
     * @param context   context
     * @param assetName asset文件
     * @param savePath  目标路径
     * @param saveName  目标文件名
     */
    public static void copyFileFromAssets(Context context, String assetName, String savePath, String saveName) {
        // 若目标文件夹不存在，则创建
        File dir = new File(savePath);
        if (!dir.exists()) {
            if (!dir.mkdir()) {
                Log.d("FileUtils", "mkdir error: " + savePath);
                return;
            }
        }

        // 拷贝文件
        String filename = savePath + "/" + saveName;
        File file = new File(filename);
        if (!file.exists()) {
            try {
                InputStream inStream = context.getAssets().open(assetName);
                FileOutputStream fileOutputStream = new FileOutputStream(filename);

                int byteread;
                byte[] buffer = new byte[1024];
                while ((byteread = inStream.read(buffer)) != -1) {
                    fileOutputStream.write(buffer, 0, byteread);
                }
                fileOutputStream.flush();
                inStream.close();
                fileOutputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            Log.d("FileUtils", "[copyFileFromAssets] copy asset file: " + assetName + " to : " + filename);
        } else {
            Log.d("FileUtils", "[copyFileFromAssets] file is exist: " + filename);
        }
    }

    /**
     * 拷贝asset目录下所有文件到指定路径
     *
     * @param context    context
     * @param assetsPath asset目录
     * @param savePath   目标目录
     */
    public static void copyFilesFromAssets(Context context, String assetsPath, String savePath) {
        try {
            // 获取assets指定目录下的所有文件
            String[] fileList = context.getAssets().list(assetsPath);
            if (fileList != null && fileList.length > 0) {
                File file = new File(savePath);
                // 如果目标路径文件夹不存在，则创建
                if (!file.exists()) {
                    if (!file.mkdirs()) {
                        Log.d("FileUtils", "mkdir error: " + savePath);
                        return;
                    }
                }
                for (String fileName : fileList) {
                    copyFileFromAssets(context, assetsPath + "/" + fileName, savePath, fileName);
                }
            }
        } catch (Exception e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }




    private void unzip (InputStream is, String dir) throws IOException

    {

        File dest = new File(dir);

        if ( !dest.isDirectory())

            throw new IOException("Invalid Unzip destination " + dest);

        if(null == is){

            throw new IOException("InputStream is null");

        }

        ZipInputStream zip = new ZipInputStream(is);

        ZipEntry ze;

        while ( (ze = zip.getNextEntry()) != null ) {

            final String path = dest.getAbsolutePath()

                + File.separator + ze.getName();

// Create any entry folders

            String zeName = ze.getName();

            char cTail = zeName.charAt(zeName.length() - 1);

            if ( cTail == File.separatorChar)

            {

                File file = new File(path);

                if(!file.exists()){

                    if ( !file.mkdirs() ){

                        throw new IOException("Unable to create folder " + file);

                    }

                }

                continue;

            }

            FileOutputStream fout = new FileOutputStream(path);

            byte[] bytes = new byte[1024];

            int c;

            while((c = zip.read(bytes)) != -1){

                fout.write(bytes,0, c);

            }

            zip.closeEntry();

            fout.close();

        }

    }        










    public void Start() {
        try {

            InputStream in =getAssets().open("Font.ttf");
            GLES3JNIViewyjj1.fontData = new byte[in.available()];
            in.read(GLES3JNIViewyjj1.fontData);     
        } catch (Exception e) {}
        绘制窗口 = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        //双悬浮方案，解决触摸问题
        manager = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
        vParams = getAttributes(false);
        WindowManager.LayoutParams wParams = getAttributes(true);
        GLES3JNIViewyjj1 display = new GLES3JNIViewyjj1(this);
        vTouch = new View(this);
        manager.addView(vTouch, vParams);
        manager.addView(display, wParams);
//Nuakmon
        final WindowManager.LayoutParams lp = new WindowManager.LayoutParams();

        lp.flags = WindowManager.LayoutParams.FLAG_SECURE |//防截屏
            WindowManager.LayoutParams.FLAG_DITHER | //抖动(过录屏)
            WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE |//不接受触控
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
            WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
//                        WindowManager.LayoutParams.FLAG_SPLIT_TOUCH |
            WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED |//硬件加速
            WindowManager.LayoutParams.FLAG_FULLSCREEN |//隐藏状态栏导航栏以全屏(貌似没什么用)
            WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS |//忽略屏幕边界
            WindowManager.LayoutParams.FLAG_LAYOUT_ATTACHED_IN_DECOR |//显示在状态栏上方(貌似高版本无效
            WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN;



        //获取悬浮窗触摸
        vTouch.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    int action = event.getAction();
                    switch (action) {
                        case MotionEvent.ACTION_MOVE:
                        case MotionEvent.ACTION_DOWN:
                        case MotionEvent.ACTION_UP:
                            GLES3JNIViewyjj1.MotionEventClick(action != MotionEvent.ACTION_UP, event.getRawX(), event.getRawY());
                            break;
                        default:
                            break;
                    }
                    return false;
                }
            });
        final Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    try {
                        String rect[] = GLES3JNIViewyjj1.getWindowRect().split("\\|");
                        vParams.x = Integer.parseInt(rect[0]);
                        vParams.y = Integer.parseInt(rect[1]);
                        vParams.width = Integer.parseInt(rect[2]);
                        vParams.height = Integer.parseInt(rect[3]);
                        manager.updateViewLayout(vTouch, vParams);

                    } catch (Exception e) {
                    }
                    handler.postDelayed(this, 20);
                }
            }, 20);
    }
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        point = new Point();
        绘制窗口.getDefaultDisplay().getRealSize(point);
        GLES3JNIViewyjj1.real(point.x, point.y);
        Log.e("NDK", "屏幕旋转了");
    }

    private WindowManager.LayoutParams getAttributes(boolean isWindow) {
        WindowManager.LayoutParams params = new WindowManager.LayoutParams();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            params.type = WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY;
        } else {
            params.type = WindowManager.LayoutParams.TYPE_PHONE;
        }
        params.flags = WindowManager.LayoutParams.FLAG_FULLSCREEN | WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS | WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;

        if (isWindow) {
            params.flags |= WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL | WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE;
        }
        params.format = PixelFormat.RGBA_8888;            // 设置图片格式，效果为背景透明
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            params.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
        }
        params.gravity = Gravity.LEFT | Gravity.TOP;        // 调整悬浮窗显示的停靠位置为左侧置顶
        params.x = params.y = 0;
        params.width = params.height = isWindow ? WindowManager.LayoutParams.MATCH_PARENT : 0;
        return params;
    }
    public static String 读取文件(String path) {
        String str = "";
        try {
            File urlFile = new File(path);
            InputStreamReader isr = new InputStreamReader(new FileInputStream(urlFile), "UTF-8");
            BufferedReader br = new BufferedReader(isr);

            String mimeTypeLine = null;
            while ((mimeTypeLine = br.readLine()) != null) {
                str = str + mimeTypeLine;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return str;
    }



    public void requestPermission() {

        boolean isGranted = true;

        if (android.os.Build.VERSION.SDK_INT >= 23) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.RESTART_PACKAGES) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.REQUEST_COMPANION_RUN_IN_BACKGROUND) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.SYSTEM_ALERT_WINDOW) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }

            if (!isGranted) {
                this.requestPermissions(
                    new String[]{Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission
                        .ACCESS_FINE_LOCATION,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.RECORD_AUDIO,
                        Manifest.permission.RESTART_PACKAGES,
                        Manifest.permission.REQUEST_COMPANION_RUN_IN_BACKGROUND,
                        Manifest.permission.REQUEST_IGNORE_BATTERY_OPTIMIZATIONS,
                        Manifest.permission.SYSTEM_ALERT_WINDOW,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    102);
            }
        }
        if (Build.VERSION.SDK_INT >= 23) {
            if (!Settings.canDrawOverlays(MainActivity.this)) {
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                                           Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, 111);
            }
        }
        //    alertDialogs.dismiss(); 
    }






} 

 

