package com.POST.KOP;

import java.io.IOException;
import java.io.File;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.FileInputStream;
import android.content.pm.PackageManager;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;
import java.io.InputStream;
import java.io.FileOutputStream;

public class Tools {

	/*申请root权限*/
    public static void 申请ROOT() {
        try {
            Runtime.getRuntime().exec("su");
        } catch (IOException e) {}
    }

	/*检测手机是否有ROOT*/
	public static boolean 检测ROOT() {
		File file=null;
		String[]paths={"/system/bin/","/system/xbin/"};
		try {
			for (String path:paths) {
				file = new File(path + "su");
				if (file.exists() && file.canExecute()) {
					return true;
				} else {
					return false;
				}
			}
		} catch (Exception x) {
			x.printStackTrace();
		}
		return false;
	}

	/*创建文件*/
	public static void 创建文件(String path) {
		File mFile=new File(path);
		if (mFile.exists()) {
			mFile.delete();
		}
		try {
			mFile.createNewFile();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/*创建文件夹*/
	public static void 创建文件夹(String 路径) {
		File file=new File(路径);
		if (!file.exists()) {
			file.mkdirs();
		}
	}

	/*写入内容到文件*/
    public static void 写入文件(String 路径, String 内容) {
        try {
            FileWriter fw = new FileWriter(路径);
            fw.write(内容);
            fw.close();
        } catch (IOException e) {}
    }

	/*删除某个文件*/
	public static boolean deleteFile(String filename) {
        return new File(filename).delete();
    }

	public static boolean 删除文件(String folder) {
        if (folder == null || folder.length() == 0 || folder.trim().length() == 0) {
            return true;
        }
        File file = new File(folder);
        if (!file.exists()) {
            return true;
        }
        if (file.isFile()) {
            return file.delete();
        }
        if (!file.isDirectory()) {
            return false;
        }
        for (File f : file.listFiles()) {
            if (f.isFile()) {
                f.delete();
            } else if (f.isDirectory()) {
                deleteFile(f.getAbsolutePath());
            }
        }
        return file.delete();
    }

	/*读取文件里的内容*/
	public static String 读取文件(String 路径) {
		String str="";
		try {
			File urlFile=new File(路径);
			InputStreamReader isr=new InputStreamReader(new FileInputStream(urlFile), "UTF-8");
			BufferedReader br=new BufferedReader(isr);
			String mimeTypeLine=null;
			while ((mimeTypeLine = br.readLine()) != null) {
				str = str + mimeTypeLine;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return str;
	}

	/*执行shell命令*/
    public static void RunShell(String 命令) {
        String s = 命令;

        try {
            Runtime.getRuntime().exec(s, null, null);//执行
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

	/*写出资源文件*/
	public static boolean 写出资源文件(Context context, String outPath, String fileName) {
		File file=new File(outPath);
		if (!file.exists()) {
			if (!file.mkdirs()) {
				return false;
			}
		}
		try {
			InputStream inputStream=context.getAssets().open(fileName);
			File outFile=new File(file, fileName);
			FileOutputStream fileOutputStream=new FileOutputStream(outFile);
			byte[] buffer=new byte[1024];
			int byteRead;
			while (-1 != (byteRead = inputStream.read(buffer))) {
				fileOutputStream.write(buffer, 0, byteRead);
			}
			inputStream.close();
			fileOutputStream.flush();
			fileOutputStream.close();
			return true;
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
	}

	/*打开软件*/
	public static boolean 打开软件(Context context, String 包名) {
		PackageManager packageManager=context.getPackageManager();
		Intent Intent=new Intent();
		Intent = packageManager.getLaunchIntentForPackage(包名);
		context.startActivity(Intent);
		return true;
	}
}
