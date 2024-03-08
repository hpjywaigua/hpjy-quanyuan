#ifndef ESP_H
#define ESP_H

jmethodID 屏高,屏宽,方框,圆角方框,射线,名字,阵营,距离,
血条,背敌,文字,敌人,人数,骨骼,头骨,图片,图片2,物资;

using namespace std;

class Color {
public:
    int colour;
    Color() {
        this->colour = 0;
    }
    Color(int colour) {
    this->colour = colour;
    }
};

class 风凌 {
private:
    JNIEnv *_参数;
    jobject _绘画;
    jobject _绘制;

public:
    风凌() {
        _参数 = nullptr;
        _绘画 = nullptr;
        _绘制 = nullptr;
    }

    风凌(JNIEnv *参数, jobject 绘画, jobject 绘制) {
        this->_参数 = 参数;
        this->_绘画 = 绘画;
        this->_绘制 = 绘制;
    }
	
	int 获取宽度() const {
        jclass 绘图 = _参数 -> GetObjectClass(_绘画);
        屏宽 = _参数 -> GetMethodID(绘图, "getWidth", "()I");
		return _参数 -> CallIntMethod(_绘画, 屏宽);
        return 0;
    }
	
    int 获取高度() const {
        jclass 绘图 = _参数 -> GetObjectClass(_绘画);
        屏高 = _参数 -> GetMethodID(绘图, "getHeight", "()I");
		return _参数 -> CallIntMethod(_绘画, 屏高);
        return 0;
    }
	
    void 绘制方框(Color colour, float x1,float y1,float x2,float y2) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         方框 = _参数 -> GetMethodID(绘图, "绘制方框", "(Landroid/graphics/Canvas;IFFFF)V");
         _参数 -> CallVoidMethod(_绘画, 方框, _绘制, colour, x1, y1, x2, y2);
    }
    
    void 绘制圆角方框(Color colour, float x1,float y1,float x2,float y2,int rx,int ry) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         圆角方框 = _参数 -> GetMethodID(绘图, "绘制圆角方框", "(Landroid/graphics/Canvas;IFFFFII)V");
         _参数 -> CallVoidMethod(_绘画, 圆角方框, _绘制, colour, x1, y1, x2, y2,rx,ry);
    }
    
    void 绘制射线(Color colour, float x1,float y1,float x2,float y2) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         射线 = _参数 -> GetMethodID(绘图, "绘制射线", "(Landroid/graphics/Canvas;IFFFF)V");
         _参数 -> CallVoidMethod(_绘画, 射线, _绘制, colour, x1, y1, x2, y2);
    }
    
    void 绘制骨骼(Color colour, float x1,float y1,float x2,float y2) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         骨骼 = _参数 -> GetMethodID(绘图, "绘制骨骼", "(Landroid/graphics/Canvas;IFFFF)V");
         _参数 -> CallVoidMethod(_绘画, 骨骼, _绘制, colour, x1, y1, x2, y2);
    }
    
    void 绘制头骨(Color colour, float x1,float y1,float x2) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         头骨 = _参数 -> GetMethodID(绘图, "绘制头骨", "(Landroid/graphics/Canvas;IFFF)V");
         _参数 -> CallVoidMethod(_绘画, 头骨, _绘制, colour, x1, y1, x2);
    }
    
    void 绘制名字(char *txt, float x,float y) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         名字 = _参数 -> GetMethodID(绘图, "绘制名字", "(Landroid/graphics/Canvas;Ljava/lang/String;FF)V");//String就是字符这里代表了名字
         jstring s = _参数 -> NewStringUTF(txt);
         _参数 -> CallVoidMethod(_绘画, 名字, _绘制, s, x, y);
         _参数 -> DeleteLocalRef(s);
    }
   
	void 绘制阵营(int txt, float x,float y) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         阵营 = _参数 -> GetMethodID(绘图, "绘制阵营", "(Landroid/graphics/Canvas;IFF)V");//int float float
         _参数 -> CallVoidMethod(_绘画, 阵营, _绘制, txt, x, y);
    }
    
    void 绘制距离(float m, float x,float y) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         距离 = _参数 -> GetMethodID(绘图, "绘制距离", "(Landroid/graphics/Canvas;FFF)V");//float float float
         _参数 -> CallVoidMethod(_绘画, 距离, _绘制, m, x, y);
    }
	
    void 绘制血条(int hp, float x, float y) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         血条 = _参数 -> GetMethodID(绘图, "绘制血条", "(Landroid/graphics/Canvas;IFF)V");//int float float
         _参数 -> CallVoidMethod(_绘画, 血条, _绘制, hp, x, y);
    }
    
    void 绘制背敌(Color colour, float x, float y, float d) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         背敌 = _参数 -> GetMethodID(绘图, "绘制背敌", "(Landroid/graphics/Canvas;IFFF)V");//int float float float float
         _参数 -> CallVoidMethod(_绘画, 背敌, _绘制, colour, x, y, d);
    }
    
    void 背敌文字(float m, float x,float y) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         文字 = _参数 -> GetMethodID(绘图, "背敌文字", "(Landroid/graphics/Canvas;FFF)V");//float float float
         _参数 -> CallVoidMethod(_绘画, 文字, _绘制, m, x, y);
    }
    
    void 雷达敌人(int 人机, float x, float y) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         敌人 = _参数 -> GetMethodID(绘图, "雷达敌人", "(Landroid/graphics/Canvas;IFF)V");// int float float
         _参数 -> CallVoidMethod(_绘画, 敌人, _绘制, 人机, x, y);
    }
    
   void 人数绘制(int text, float x) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         人数 = _参数 -> GetMethodID(绘图, "人数绘制", "(Landroid/graphics/Canvas;IF)V");//IF意思就是I是int F是float 这里的int代表人数 float 代表坐标x
         _参数 -> CallVoidMethod(_绘画, 人数, _绘制, text, x);
    }
   
    void 绘制图片(int id ,float x,float y){
        jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         图片 = _参数 -> GetMethodID(绘图, "绘制图片", "(Landroid/graphics/Canvas;IFF)V");
         _参数 -> CallVoidMethod(_绘画, 图片, _绘制,id, x, y);
    }
    
    void 绘制图片2(int id ,float x,float y){
        jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         图片2 = _参数 -> GetMethodID(绘图, "绘制图片2", "(Landroid/graphics/Canvas;FF)V");
         _参数 -> CallVoidMethod(_绘画, 图片2, _绘制, id, x, y);
    }
    
    void 物资名字(float m, char *txt, float x, float y) {
         jclass 绘图 = _参数 -> GetObjectClass(_绘画);
         物资 = _参数 -> GetMethodID(绘图, "物资名字", "(Landroid/graphics/Canvas;FLjava/lang/String;FF)V");
         jstring s = _参数 -> NewStringUTF(txt);
         _参数 -> CallVoidMethod(_绘画, 物资, _绘制, m, s, x, y);
    }
};

#endif
