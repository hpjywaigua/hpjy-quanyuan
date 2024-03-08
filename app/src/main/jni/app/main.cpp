
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unordered_map>
#include "Includes.h"
#include "Memory.h"
#include "Iconcpp.h"
#include "Icon.h"
#include "timer.h"

#include "log.h"
#include "Lionap.h"
#include "http.h"
#include "Qunmal/imgui-knobs.cpp"
#include "Icon.h"
#include <time.h>
#include "读写.h"
#include "Switch.h"
#include "纹理.h"
#include "../imgui/imgui_expand.h"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>//过缺页
#include <sys/mman.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include "other/wenmom.h"
#define targetLibName OBFUSCATE("libanort.so")
#define targetLibName OBFUSCATE("libanogs.so")
#define targetLibName OBFUSCATE("libUE4.so")
#define targetLibName OBFUSCATE("libTDataMaster.so")
#define targetLibName OBFUSCATE("libINTLFoundation.so")
#define targetLibName OBFUSCATE("libijkffmpeg.so")
#define targetLibName OBFUSCATE("libCrashSight.so")
#define targetLibName OBFUSCATE("libGCloudVoice.so")
#define targetLibName OBFUSCATE("libmarsxlog.so")
#define targetLibName OBFUSCATE("libgcloud.so")
#define targetLibName OBFUSCATE("libtersafe.so")
#define PATCH_LIB
#define HOOK_LIB
float 线条粗细;
float 骨骼粗细;
bool 自动压枪,不锁人机,不锁真人;
bool hhhh;
bool zmq;
bool hz[50];
float jzjdt=0.0;
ImFont *font;
float NumIo[50];
float yjdr=0;
float yjd=0;
float gdt=360;
float aphlev=1.0;
float domeag=500;
int FPS = 80;
bool masddd[20];
float x,y,w;
float Myz,Myy,Myx,RadarX=432,RadarY=345;
float MIDDLE;
float BOTTOM;
long tempBase, lockBase;
bool bypassLOBBY;
bool bypassLogoTenMinutes;
bool 无后;
bool 聚点;
char *vp_name = NULL;
bool 范围;
bool 防抖;
bool 防封;
bool 防闪;
ImColor Colorofblood;
int Yob=0;
bool 加速;
float 距离压枪;
ImVec4 vehicle_color = ImVec4(255.f / 255.f, 255 / 255.f, 0 / 255.f, 255 / 255.f);//NOLINT
bool 瞬击;
bool 除雾;
bool image;
float 自身坐标;
float 敌人坐标;
bool 防封1;
float zm1,zm2,tempY,tempX,bulletVelocity;
bool imagePos;
bool imageHeight;
bool imageWidth;
bool 射速;
bool 防封2;
bool imageSize;
bool ImGuiStyleVar_WindowFill;
char* wzname;
ImColor wzColor;
int index01 = 0;
int index02 = 0;
bool showLine,showWarning,showBones,showBox,showinFormationn,showHealth,showRadar,showDistance,showshouchi,showdz;
char Fcsm[1024];
int Bon=1;
char tpisWindowShowText[60];
int tpisWindowShowTime=0;
static void Helpxieam(const char* desc)
{
    ImGui::TextDisabled("狗子直装使用协议");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

float accent_colour[4] = { 173 / 255.f, 57 / 255.f, 57 / 255.f, 1.f };
static float content_animation = 0.0f;
static float dpi_scale = 1.0f;
jobject g_object;
void shut(const char* text, bool* close, float x)
{
    ImGui::SetCursorPos({ x - 170.0f,10.0f });
    if (ImGui::SmallButton(text))
       *close = true;
}
void DrawImage(int x, int y, int w, int h, ImTextureID Texture)
{
    ImGui::GetForegroundDrawList()->AddImage(Texture, ImVec2(x, y), ImVec2(x + w, y + h));
}



int lineType=1,sdrs=2,boxType=1,aimType=1,lockingPos=6,zmbwx,yp=10;
float aimScope = 350,aimsd=0.3,sqtj=1,aimpy=3,lineWidth=2.5,bonesWidth=2.5;
ImColor CircleColor        = ImColor(1.0f, 1.0f, 1.0f, 1.0f);
    //默认宽度
    float   CircleThickness    = 2.0f;
    //默认圆边数量
    int     CircleNum_segments = 0;
    float   LineThickness = 2.0f;

float Distance;

Fevcror getPointingAngleZM(uintptr_t uMyObject, uintptr_t pBase, float object_x,float object_y, float object_z, float Self_x, float Self_y, float Self_z)//需要写预判
{
    Fevcror PointingAngle;
    // 子弹速度
   float bulletVelocity = getfloat(getPtr64(getPtr64(oneself + 0xEB8) + 0x12E0) + 0x12F4);
    float PressureGun = get_3D_Distance(object_x, object_y, object_z, Self_x, Self_y, Self_z);
    float xtime = PressureGun / bulletVelocity;
    float ytime = PressureGun / bulletVelocity;// 子弹速度
    float ypx = getfloat(pBase + 0xEAC);
    float ypy = getfloat(pBase + 0xEAC + 4);
    float ypz = getfloat(pBase + 0xEAC + 8);
    object_x = object_x + ypx * xtime;
    object_y = object_y + ypy * xtime;
    object_z = object_z + 360 * ytime * ytime;
    float zbcx = object_x - Self_x;
    float zbcy = object_y - Self_y;
    float zbcz = object_z - Self_z;
	
    int state= getdword(oneself + 0x1340);
    PointingAngle.X = atan2(zbcy, zbcx) * 180 / PI;
    PointingAngle.Y = (atan2(zbcz, sqrt(zbcx * zbcx + zbcy * zbcy)) * 180 / PI)+0.1+距离压枪;
    if(kj == 1 && kj == 257 && kh != 1)
    {
        PointingAngle.X =  (atan2(zbcy, zbcx) * 180 / PI)+0.05;
        PointingAngle.Y = (atan2(zbcz, sqrt(zbcx * zbcx + zbcy * zbcy)) * 180 / PI)-0.05;
    }
    return PointingAngle;
}








float tempDis;
float 自瞄范围=120;
float 雷达X = 256;
float 雷达Y = 200;
float resistance;
ImColor bone2_color;
ImColor LiNEfcoler = ImColor(255,255,255,255);
ImColor 浅蓝 = ImColor(ImVec4(36/255.f, 249/255.f, 217/255.f, 255/255.f));
ImColor 蓝色 = ImColor(ImVec4(170/255.f, 203/255.f, 244/255.f, 0.95f));
ImColor 白色 = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 0.95f));
ImColor 浅粉 = ImColor(ImVec4(255/255.f, 200/255.f, 250/255.f, 0.95f));
ImColor 黑色 = ImColor(ImVec4(0/255.f, 0/255.f, 0/255.f, 0.7f));
ImColor 半黑 = ImColor(ImVec4(0/255.f, 0/255.f, 0/255.f, 0.18f));
ImColor 血色 = ImColor(ImVec4(0/255.f, 249/255.f, 0/255.f, 0.35f));
ImColor 红色 = ImColor(ImVec4(233/255.f, 55/255.f, 51/255.f, 0.95f));
ImColor 绿色 = ImColor(ImVec4(50/255.f, 222/215.f, 50/255.f, 0.95f));
ImColor 黄色 = ImColor(ImVec4(255/255.f, 255/255.f, 0/255.f, 0.95f));
ImColor 橘黄 = ImColor(ImVec4(255/255.f, 150/255.f, 30/255.f, 0.95f));
ImColor 粉红 = ImColor(ImVec4(220/255.f, 108/255.f, 1202/255.f, 0.95f));
ImColor 紫色 = ImColor(ImVec4(169/255.f, 120/255.f, 223/255.f, 0.95f));
ImColor whiteColor = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 1.0f));



ImU32 c_透明=IM_COL32(0, 0, 0, 0);
ImU32 c_fafafa=IM_COL32(250, 250, 250, 255);
ImU32 c_cccccc=IM_COL32(204, 204, 204, 255);
ImU32 c_c2c2c2 = IM_COL32(194, 194, 194, 255);
ImU32 c_23292e=IM_COL32(35, 41, 46, 255);
ImU32 c_4023292e=IM_COL32(35, 41, 46, 125);
ImU32 c_2f363c=IM_COL32(47, 54, 60, 255);
ImU32 c_402f363c=IM_COL32(47, 54, 60, 125);
ImU32 c_DAB123 = IM_COL32(0, 177, 0, 255);
ImVec4 半透明黑色ImVec4 = ImVec4(0.184314f, 0.211765f, 0.235294f, 0.5f);
ImVec4 半透明黑色ImVec480 = ImVec4(0.184314f, 0.211765f, 0.235294f, 0.75f);
ImVec4 ImVec423292e = ImVec4(0.137255f, 0.160784f, 0.180392f, 1.0f);
ImVec4 ImVec48023292e = ImVec4(0.137255f, 0.160784f, 0.180392f, 0.75f);
ImVec4 ImVec44023292e = ImVec4(0.137255f, 0.160784f, 0.180392f, 0.5f);


typedef struct Rect
{
    float x;
    float y;
    float w;
    float h;
};
// 获取载具
bool GetVehicleInfo(char* gname,char** name,ImVec4* color)
{
    if(strcmp(gname,"VH_BRDM_C")==0)
    {
        *name="装甲车";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Scooter_C")==0)
    {
        *name="小绵羊";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Motorcycle_C")==0||strcmp(gname,"VH_Motorcycle_1_C")==0)
    {
        *name="摩托车";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_MotorcycleCart_1_C")==0||strcmp(gname,"VH_MotorcycleCart_C")==0)
    {
        *name="三轮摩托";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"VH_Snowmobile_C")==0)
    {

        *name="雪地摩托";
        *color=vehicle_color;
        return true;
    }
    if(strcmp(gname,"BP_VH_Tuk_C")==0||strcmp(gname,"BP_VH_Tuk_1_C")==0)
    {
        *name="三轮摩托";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Buggy")!= nullptr)
    {
        *name="蹦蹦车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Dacia")!= nullptr)
    {
        *name="轿车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"UAZ")!= nullptr)
    {
        *name="吉普车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"_PickUp")!= nullptr)
    {
        *name="皮卡车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Rony")!= nullptr)
    {
        *name="皮卡车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"Mirado")!= nullptr)
    {
        *name="跑车";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"MiniBus")!= nullptr)
    {
        *name="迷你巴士";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"PG117")!= nullptr)
    {
        *name="快艇";
        *color=vehicle_color;
        return true;
    }
    if(strstr(gname,"AquaRail")!= nullptr)
    {
        *name="摩托艇";
        *color=vehicle_color;
        return true;
    }
    if (strstr(gname, "AirDropPlane")!= nullptr) {
        *name = "放置飞机";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "BP_AirDropPlane_C")!= nullptr) {
        *name = "电动滑翔机";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "Bigfoot")!= nullptr) {
        *name = "大脚车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "ATV")!= nullptr) {
        *name = "沙滩车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "CoupeRB")!= nullptr) {
        *name = "跑车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "BP_SciFi_Moto_C")!= nullptr) {
        *name = "啵啵车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "licedTrain_C")!= nullptr) {
        *name = "磁吸小火车";
        *color = vehicle_color;
        return true;
    }
    if (strstr(gname, "_Mountainbike_Training_C")!= nullptr) {
        *name = "自行车";
        *color = vehicle_color;
        return true;
    }        
    return false;
}
static float tm = 127 / 255.f;

static ImVec4 arr[] = {{144 / 255.f, 238 / 255.f, 144 / 255.f, tm},
                       {135 / 255.f, 206 / 255.f, 255 / 255.f, tm},
                       {255 / 255.f, 0 / 255.f,   0 / 255.f,   tm},
                       {0 / 255.f,   255 / 255.f, 0 / 255.f,   tm},
                       {0 / 255.f,   255 / 255.f, 127 / 255.f, tm},
                       {255 / 255.f, 182 / 255.f, 193 / 255.f, tm},
                       {218 / 255.f, 112 / 255.f, 214 / 255.f, tm},
                       {248 / 255.f, 248 / 255.f, 255 / 255.f, tm},
                       {0 / 255.f,   255 / 255.f, 255 / 255.f, tm},
                       {255 / 255.f, 165 / 255.f, 0 / 255.f,   tm},
                       {153 / 255.f, 204 / 255.f, 255 / 255.f, tm},
                       {204 / 255.f, 255 / 255.f, 153 / 255.f, tm},
                       {255 / 255.f, 255 / 255.f, 153 / 255.f, tm},
                       {255 / 255.f, 153 / 255.f, 153 / 255.f, tm},
                       {153 / 255.f, 153 / 255.f, 204 / 255.f, tm},
                       {204 / 255.f, 204 / 255.f, 204 / 255.f, tm},
                       {102 / 255.f, 204 / 255.f, 153 / 255.f, tm},
                       {255 / 255.f, 102 / 255.f, 0 / 255.f,   tm},
                       {102 / 255.f, 204 / 255.f, 204 / 255.f, tm},
                       {153 / 255.f, 204 / 255.f, 255 / 255.f, tm}
};
static int length = sizeof(arr) / 16;


struct sESPMenu {
bool 载具;
bool 手雷预;
float 雷达x=350;
float 雷达y=350;

// 124,252,0


ImColor BoxColor = {248,1,6}; //真人方框
ImColor BotBoxColor = ImColor(251,251,249);  //人机方框
ImColor LineColor = ImColor(248,1,6);//真人射线
ImColor BotLineColor = ImColor(251,251,249);//人机射线
ImColor BoneColor = ImColor(248,1,6);//真人骨骼
ImColor BotBoneColor = ImColor(251,251,249);//人机骨骼


float Zmfw=500;
int aimbotmode = 3;				// 锁定位置
int Munmo = 1;				// 锁定位置
int pdyx=1; //打击判断
int htfa=1;
int xz=1; //方框3d\2d
int xl=1;
int sc=1;
bool showItem, showShotgun, showPistol, show556, show762, showRifle, showMirror, showExpansion, showDrug, showArmor, showSubmachine, showSniper, showOtherParts, showChest, showDrop;
bool HZbox[50];
};
sESPMenu ESPMenu;

void 写入调试数据()
{
	int fd = open("/storage/emulated/0/Pz", O_WRONLY | O_CREAT,0777);
    write(fd, &ESPMenu, sizeof(ESPMenu));   // 写入文本
    close(fd);

}

void 加载调试数据()
{
	int fd = open("/storage/emulated/0/Pz", O_RDONLY);
    read(fd, &ESPMenu, sizeof(ESPMenu));   // 读取文本 
    close(fd);
}

void ShowImage(const char* filename) {
    // 加载图片
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    unsigned char* image = new unsigned char[file.tellg()];
    file.read((char*)image, file.tellg());
    file.close();

}
long int getZZ(long int addr)
{
	long int var[1] = { 0 };
	vm_readv(addr, var, 8);
	return var[0];
}


ImColor rifle_color = ImColor(ImVec4(255/255.f,0/255.f,0/255.f,255.f/255.f));
ImColor submachine_color = ImColor(ImVec4(255/255.f,110/255.f,180/255.f,255.f/255.f));
ImColor shotgun_color = ImColor(ImVec4(255/255.f,110/255.f,180/255.f,255.f/255.f));
ImColor showPistol_color = ImColor(ImVec4(255/255.f,110/255.f,180/255.f,255.f/255.f));
ImColor snipe_color = ImColor(ImVec4(144/255.f,238/255.f,0/255.f,255.f/255.f));
ImColor box_color = ImColor(ImVec4(255/255.f,192/255.f,203/255.f,255.f/255.f));
ImColor drop_color = ImColor(ImVec4(255/255.f,0/255.f,0/255.f,255.f/255.f));
ImColor drug_color = ImColor(ImVec4(255/255.f,105/255.f,180/255.f,255.f/255.f));
ImColor armor_color = ImColor(ImVec4(255/255.f,215/255.f,0/255.f,255.f/255.f));
ImColor color_556 = ImColor(ImVec4(0/255.f,255/255.f,0/255.f,255.f/255.f));
ImColor color_762 = ImColor(ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f));
ImColor color_45 = ImColor(ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f));
ImColor color_9 = ImColor(ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f));
ImColor color_12 = ImColor(ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f));
ImColor color_300 = ImColor(ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f));
ImColor color_50 = ImColor(ImVec4(255/255.f,255/255.f,0/255.f,255.f/255.f));
ImColor bone_color;
ImColor mirror_color = ImColor(ImVec4{255.f/255.f,144.f/255.f,0.f/255.f,255.f/255.f});
ImColor otherparts_color = ImColor(ImVec4{245.f/255.f,245.f/255.f,245.f/255.f,255.f/255.f});
ImColor expansion_color = ImColor(ImVec4{208.f/255.f,128.f/255.f,114.f/255.f,255.f/255.f});
ImColor aim_color = ImColor(ImVec4(255/255.f,0/255.f,0/255.f,255/255.f));
ImColor redColor = ImColor(ImVec4(255/255.f, 0/255.f, 0/255.f, 255/255.f));
ImColor greenColor = ImColor(ImVec4(50/255.f, 222/215.f, 50/255.f, 200/255.f));
ImColor yellowColor = ImColor(ImVec4(255/255.f, 215/255.f, 0/255.f, 200/255.f));
#include "call.h"
风凌 凌;







//GUI




struct UmVopng{
    ImTextureID uipnga;
    ImTextureID uipngb;
    ImTextureID uipngc;
    ImTextureID uipngd;
    ImTextureID uipnge;
    ImTextureID uipngf;
    ImTextureID uipngg;
    ImTextureID uipngh;
    ImTextureID uipngi;
    ImTextureID uipngj;
    ImTextureID uipngk;
    ImTextureID uipngl;
    
}umVopng;




bool GetGrenadeInfo(char* gname, char** name, ImColor* color)
{
    if(strstr(gname,"ProjGrenade_BP_C")!= nullptr)//手雷
    {
        *name="破片手雷 !";
        *color=redColor;
        return true;
    }
    if(strstr(gname,"ProjSmoke_BP_C")!=nullptr)
    {
        *name="烟雾弹 !";
        *color=redColor;
        return true;
    }
    if(strstr(gname,"ProjBurn_BP_C")!=nullptr)
    {
        *name="燃烧瓶 !";
        *color=redColor;
        return true;
    }  
    
    return false;
}


bool GetSuperDistanceInfo(char* gname, char** name, ImColor* color)
{
 
        if (strstr(gname, "AirDropListWrapperActor_Pickup_C") != 0)
        {
            *name="空投";
            *color=armor_color;
            return true;           // 空投
        }
    
    
        if (strstr(gname, "CG017") != 0)
        {
            *name="空投";
            *color=armor_color;
            return true;           // 空投
        }
    
    
        if (strstr(gname, "BP_AirDropBox_C") != 0)
        {
            *name="空投";
            *color=armor_color;
            return true;
        }
    
    
        if (strstr(gname, "AirDropPlane") != 0)
        {
            *name="飞机";
            *color=armor_color;
            return true;
        }
    
    return false;
}   



bool GetItemInfo(char* gname,char** name, ImColor* color)
{
    if(ESPMenu. show556)
    {
        if(strcmp(gname,"BP_Ammo_556mm_Pickup_C")== 0)
        {
            *name="5.56MM";
            *color=color_556;
            return true;
        }
    }
    if(ESPMenu. show762)
    {
        if(strcmp(gname,"BP_Ammo_762mm_Pickup_C")== 0) 
        {
            *name = "7.62MM";
            *color = color_762;
            return true;
        }
    }

    //刀
    if(strcmp(gname,"BP_WEP_Pan_Pickup_C")== 0)
    {
        *name="平底锅";
        *color=rifle_color;
        return true;
    }
    //显示步枪img
    if(ESPMenu. showRifle)
    {
        if(strcmp(gname,"BP_Pistol_Flaregun")==0)
        {
            *name="信号枪";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_AKM_Wrapper_C")==0)
        {
            *name="AKM";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_VAL_Wrapper_C")==0)
        {
            *name="VAL";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M416_Wrapper_C")==0)
        {
            *name="M416";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M16A4_Wrapper_C")==0)
        {
            *name="M16A4";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_SCAR_Wrapper_C")==0)
        {
                      *name="SCAR";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_QBZ_Wrapper_C")==0)
        {
            *name="QBZ";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_G36_Wrapper_C")==0)
        {
            *name="G36C";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_M762_Wrapper_C")==0)
        {
            *name="M762";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_Groza_Wrapper_C")==0)
        {
            *name="Groza";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Rifle_AUG_Wrapper_C")==0)
        {
            *name="AUG";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_DP28_Wrapper_C")==0)
        {
            *name="DP28";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_M249_Wrapper_C")==0)
        {
            *name="M249";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_MG3_Wrapper_C")==0)
        {
            *name="MG3";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Other_CrossBow_Wrapper_C")==0)
        {
            *name="CrossBow";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_QBU_Wrapper_C")==0)
        {
            *name="QBU";
            *color=rifle_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_VSS_Wrapper_C")==0)
        {
            *name="VSS";
            *color=shotgun_color;
            return true;
        }
    }
    //显示冲锋枪
    if(ESPMenu. showSubmachine)
    {
        if(strcmp(gname,"BP_MachineGun_UMP9_Wrapper_C")==0)
        {
            *name="UMP45";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_P90CG17_Wrapper_C")==0)
        {
            *name="P90CG17";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_TommyGun_Wrapper_C")==0)
        {
            *name="汤姆逊";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_PP19_Wrapper_C")==0)
        {
            *name="PP19";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_Uzi_Wrapper_C")==0)
        {
            *name="Uzi";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_Vector_Wrapper_C")==0)
        {
            *name="Vector";
            *color=submachine_color;
            return true;
        }
        if(strcmp(gname,"BP_MachineGun_MP5K_Wrapper_C")==0)
        {
            *name="MP5K";
            *color=shotgun_color;
            return true;
        }
        if(strstr(gname,"ProjPan_BP_C")!=nullptr)
        {
        *name="平底锅 !";
        *color=redColor;
        return true;
        }     
    }
    //显示狙击枪
    if(ESPMenu. showSniper)
    {
        if(strcmp(gname,"BP_Sniper_Kar98k_Wrapper_C")==0)
        {
            *name="Kar98k";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Win94_Wrapper_C")==0)
        {
            *name="WIN94";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Mini14_Wrapper_C")==0)
        {
            *name="Mini14";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_SKS_Wrapper_C")==0)
        {
            *name="SKS";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_M24_Wrapper_C")==0)
        {
            *name="M24";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_rifle_Mk47_Wrapper_C")==0)
        {
            *name="Mk47";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_WEP_Mk14_Pickup_C")==0)
        {
            *name="MK14";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_AWM_Wrapper_C")==0)
        {
            *name="AWM";
            *color=snipe_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_Mosin_Wrapper_C")==0)
        {
            *name="Mosin";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Sniper_AMR_Wrapper_C")==0)
        {
            *name="AMR";
            *color=snipe_color;
            return true;
        }
        
        if(strcmp(gname,"BP_Sniper_SLR_Wrapper_C")==0)
        {
            *name="SLR";
            *color=snipe_color;
            return true;
           }
    }
    //霰弹枪
    if(ESPMenu. showShotgun)
    {
        if(strcmp(gname,"BP_ShotGun_DP12_Wrapper_C")==0)
        {
            *name="DP12";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S686_Wrapper_C")==0)
        {
            *name="S686";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_SawedOff_Wrapper_C")==0)
        {
            *name="SawedOff";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S1897_Wrapper_C")==0)
        {
            *name="S1897";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_S12K_Wrapper_C")==0)
        {
            *name="S12K";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_ShotGun_SPAS-12_Wrapper_C")==0)
        {
            *name="SPAS-12";
            *color=shotgun_color;
            return true;
        }
    }
    //手枪
    if(ESPMenu. showPistol)
    {
        if(strcmp(gname,"BP_Pistol_P18C_Wrapper_C")==0)
        {
            *name="P18C";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_R1895_Wrapper_C")==0)
        {
            *name="R1895";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_P1911_Wrapper_C")==0)
        {
            *name="P1911";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_P92_Wrapper_C")==0)
        {
            *name="P92";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_R45_Wrapper_C")==0)
        {
            *name="R45";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_Vz61_Wrapper_C")==0)
        {
            *name="Vz61";
            *color=shotgun_color;
            return true;
        }
        if(strcmp(gname,"BP_Pistol_DesertEagle_Wrapper_C")==0)
        {
            *name="DesertEagle";
            *color=shotgun_color;
            return true;
        }
    }
    //倍镜
    if(ESPMenu. showMirror)
    {
        if(strcmp(gname,"BP_MZJ_8X_Pickup_C")==0)
        {
            *name="8x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_6X_Pickup_C")==0)
        {
            *name="6x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_4X_Pickup_C")==0)
        {
            *name="4x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_3X_Pickup_C")==0)
        {
            *name="3x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_2X_Pickup_C")==0)
        {
            *name="2x镜";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_HD_Pickup_C")==0)
        {
            *name="红点";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_MZJ_QX_Pickup_C")==0)
        {
            *name="全息";
            *color=otherparts_color;
            return true;
        }
    }
    //扩容
    if(ESPMenu. showExpansion)
    {
        if(strcmp(gname,"BP_DJ_Large_EQ_Pickup_C")==0)
        {
            *name="(步枪)快速扩容";
            *color=expansion_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Large_E_Pickup_C")==0)
        {
            *name="(步枪)扩容";
            *color=expansion_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Sniper_EQ_Pickup_C")==0)
        {
            *name="(狙)快速扩容";
            *color=mirror_color;
            return true;
        }
        if(strcmp(gname,"BP_DJ_Sniper_E_Pickup_C")==0)
        {
            *name="(狙)扩容";
            *color=mirror_color;
            return true;
        }
    }
    //其他配件
    if(ESPMenu. showOtherParts)
    {
        if(strcmp(gname,"BP_QK_Large_Suppressor_Pickup_C")==0)
        {
            *name="(步枪)消音器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Sniper_Suppressor_Pickup_C")==0)
        {
            *name="(狙)消音器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QT_Sniper_Pickup_C")==0)
        {
            *name="托腮板";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_ZDD_Sniper_Pickup_C")==0)
        {
            *name="(狙)子弹带";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Large_Compensator_Pickup_C")==0)
        {
            *name="(步枪)补偿器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QK_Sniper_Compensator_Pickup_C")==0)
        {
            *name="(狙)补偿器";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_Vertical_Pickup_C")==0)
        {
            *name="垂直握把";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_QT_A_Pickup_C")==0)
        {
            *name="枪托";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_Angled_Pickup_C")==0)
        {
            *name="直角握把";
            *color=otherparts_color;
            return true;
        }
        if(strcmp(gname,"BP_WB_ThumbGrip_Pickup_C")==0)
        {
            *name="拇指握把";
            *color=otherparts_color;
            return true;
        }
    }
    //药品显示
    if(ESPMenu. showDrug)
    {
        if(strcmp(gname,"Injection_Pickup_C")==0)
        {
            *name="肾上腺素";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Firstaid_Pickup_C")==0)
        {
            *name="急救包";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"FirstAidbox_Pickup_C")==0)
        {
            *name="医疗箱";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Pills_Pickup_C")==0)
        {
            *name="止痛药";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Drink_Pickup_C")==0)
        {
            *name="饮料";
            *color=drug_color;
            return true;
        }
        if(strcmp(gname,"Bandage_Pickup_C")==0)
        {
            *name="绷带";
            *color=drug_color;
            return true;
        }
    }
    //甲和头盔
    if(ESPMenu. showArmor)
    {
        if(strcmp(gname,"PickUp_BP_Helmet_Lv3_C")==0)
        {
            *name="三级头";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Armor_Lv3_C")==0)
        {
            *name="三级甲";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Bag_Lv3_B_C")==0 || strcmp(gname,"PickUp_BP_Bag_Lv3_A_C")==0 || strcmp(gname,"PickUp_BP_Bag_Lv3_C")==0 )
        {
            *name="三级包";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Helmet_Lv2_C")==0)
        {
            *name="二级头";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Armor_Lv2_C")==0)
        {
            *name="二级甲";
            *color=armor_color;
            return true;
        }
        if(strcmp(gname,"PickUp_BP_Bag_Lv2_C")==0)
        {
            *name="二级包";
            *color=armor_color;
            return true;
        }
    }  
    if (ESPMenu. showChest){
        if (strstr(gname, "CharacterDeadInventoryBox_C") != 0)
        {
            *name="盒子";
            *color=armor_color;
            return true;           // 盒子
        }
    }
    return false;
}






ImColor Yqycolo = ImColor(159,237,118);    
//获取方案资格*最笨方案

bool SetThemel(const char *label)
{
   static int style_id = 0;
   if (ImGui::Combo(label, &style_id, "界面白\0界面蓝\0界面紫\0"))
     {
	
       switch (style_id)
	
       {
	
       case 0:
	
          ImGui::StyleColorsLight();
	
          break;
       case 1:
	
          ImGui::StyleColorsDark();
	
          break;
       case 2:
          ImGui::StyleColorsClassic();

          break;
        }
	
        return true;
     }
      return false;

}
int screenWidth = 0;
int screenHeight = 0;
int ScreenX;
int ScreenY;

Vector2A Head; Vector2A Chest; Vector2A Pelvis; Vector2A Left_Shoulder; Vector2A Right_Shoulder; Vector2A Left_Elbow; Vector2A Right_Elbow;  Vector2A Left_Wrist; Vector2A Right_Wrist; Vector2A Left_Thigh; Vector2A Right_Thigh;Vector2A Left_Knee;Vector2A Right_Knee;Vector2A Left_Ankle;Vector2A Right_Ankle;
char PlayerName[100]; float Health;float PushX;float PushY;
int PlayerCount = 0,style_idx2 = 2;
#include "call.h"
bool login = false;
bool isLogin = false;
static bool isLogincw=false;	

static bool Gnzha=false;
static bool Gnzhb=false;
static bool Gnzhc=false;
static bool Gnzhd=false;
static bool Gnzhe=false;
static bool Gnzhf=false;
void Nsmfp(char*mansmsm);
static ImVec4 inactive =ImVec4(255,255,255,0);
static bool b1= false;
int Bommm=1;
int wqPage=0;
float wzPageScroll_y=517;
int wqMaxPage = 9;

char Zhanghao [100];
char Mima [100];

char *luzjna = "/storage/emulated/0/狗子/jqm";
char *luzjnb = "/storage/emulated/0/狗子/zh";
char *luzjnc = "/storage/emulated/0/狗子/mm";
char *getjqm()
{
	FILE *fp = NULL;
	static char buff[255];
	fp = fopen(luzjna, "r");
	fgets(buff, sizeof(buff), fp);
	fclose(fp);
	return buff;
}

char *getzhanghao()
{
	FILE *fp = NULL;
	static char buff[255];
	fp = fopen(luzjnb, "r");
	fgets(buff, sizeof(buff), fp);
	fclose(fp);
	return buff;
}
char *getmima()
{
	FILE *fp = NULL;
	static char buff[255];
	fp = fopen(luzjnc, "r");
	fgets(buff, sizeof(buff), fp);
	fclose(fp);
	return buff;
}








char *ver = "1.0";
char *host = "w.eydata.net";
char *wpea = "D51EAAC311B9D0D2";  //用户登录线程
char *wpeb = "A8E93FA5A81CA6AC";  //程序公告线程
char *gg = httppost(host,wpeb,"");
char *kmpath = "/storage/emulated/0/狗子/km";

char *dlfh;
char *dqfh;
char imei[40];

int hsbw=1;

bool kg=true;
bool WZbox[100];
void tostop();
bool openState = true;			// 自瞄开启
int aimingState = 1;			// 自瞄状态
float AimDistance = 300;		// 自瞄最大距离
long int Temporary = 0;			// 临时储存对象


//GUI状态
bool g_Initialized = false;
ImGuiWindow* g_window = NULL;
bool smsm[50];
//bool hz[50];
ImColor DurableColor = ImColor(255, 255, 50,180);
ImColor OilColor = ImColor(255, 255, 50,180);       
ImColor BoxColor = {255,155,155,255}; //真人方框
ImColor BotBoxColor = ImColor(0,255,0,255);  //人机方框
ImColor LineColor = ImColor(255,155,155,255);//真人射线
ImColor BotLineColor = ImColor(0,255,0,255);//人机射线
ImColor BoneColor = ImColor(255,0,0,255);//真人骨骼
ImColor BotBoneColor = ImColor(0,255,0,255);//人机骨骼
ImColor RightColor = ImColor(255,200,0,255);  //真人名
ImColor BotRightColor = ImColor(0,255,0,255);    //人机名
ImColor WarningColor = ImColor(255,155,155,255);
ImColor jlColor = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 1.0f));
ImColor BotWarningColor = ImColor(0,255,0,255);    
ImColor halflackColor = ImColor(ImVec4(0/255.f, 0/255.f, 0/255.f, 0.3f));
ImColor mzColor = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 1.0f));
ImColor health_Color = ImColor(ImVec4(255/255.f, 255/255.f, 258/255.f, 0.85f));

long int handle;
float getFloat(long int addr)
{
float var = 0;
pread64(handle, &var, 4, addr);
return var;
}

int getDword(long int addr)
{
int var = 0;
pread64(handle, &var, 4, addr);
return var;
}
//32位指针
long int lsp32(long int addr)
{
long int var = 0;
pread64(handle, &var, 4, addr);
return var;
}

//64位指针
long int lsp64(long int addr)
{
long int var = 0;
pread64(handle, &var, 8, addr);
return var;
}
//D类型
int WriteAddress_DWORD(long int addr,int value) {
pwrite64(handle, &value, 4, addr);
return 0;
}
//F类型
float WriteAddress_FLOAT(long int addr, float value) {
pwrite64(handle, &value, 4, addr);
return 0;
}
//B类型
int8_t WriteAddress_BYTE(long int addr,int value) {
pwrite64(handle, &value, 1, addr);
return 0;
}
//W类型
int16_t WriteAddress_WORD(long int addr,int value) {
pwrite64(handle, &value, 2, addr);
return 0;
}
//Q类型
int64_t WriteAddress_QWORD(long int addr,int value) {
pwrite64(handle, &value, 32, addr);
return 0;
}
//E类型
double WriteAddress_DOUBLE(long int addr, double value) {
pwrite64(handle, &value, 64, addr);
return 0;
}

void Draw3DBox(float Camera, Vector3A Max,Vector3A Min,ImColor color,float matrix[16],float LineSize, float w)
{
    Min.X = Max.X - 50.f;
    Min.Y = Max.Y - 50.f; // foot
    Min.Z = Min.Z - 10.f;

    Max.X = Max.X + 50.f;
    Max.Y = Max.Y + 50.f; // head
    Max.Z = Max.Z + 50.f;

    Vector3A v1, v2, v3, v4, v5, v6, v7, v8;
    v1 = Min;v8 = Max;v2 = Min;v3 = Min;v4 = Min;

    // 转换
    v2.X = Max.X;
    v3.Y = Max.Y;
    v4.X = Max.X;
    v4.Y = Max.Y;

    v5 = v1;
    v5.Z = Max.Z;
    v6 = v2;
    v6.Z = Max.Z;
    v7 = v3;
    v7.Z = Max.Z;

    Vector2A vv1, vv2, vv3, vv4, vv5, vv6, vv7, vv8;
    
    vv1 = WorldToScreen(v1, matrix,Camera);      
    vv2 = WorldToScreen(v2, matrix,Camera);      
    vv3 = WorldToScreen(v3, matrix,Camera);   
    vv4 = WorldToScreen(v4, matrix,Camera);      
    vv5 = WorldToScreen(v5, matrix,Camera);     
    vv6 = WorldToScreen(v6, matrix,Camera);    
    vv7 = WorldToScreen(v7, matrix,Camera);   
    vv8 = WorldToScreen(v8, matrix,Camera);
        
    ImGui::GetForegroundDrawList()->AddLine({vv1.X, vv1.Y}, {vv2.X, vv2.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv1.X, vv1.Y}, {vv3.X, vv3.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv1.X, vv1.Y}, {vv5.X, vv5.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv2.X, vv2.Y}, {vv4.X, vv4.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv2.X, vv2.Y}, {vv6.X, vv6.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv3.X, vv3.Y}, {vv4.X, vv4.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv3.X, vv3.Y}, {vv7.X, vv7.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv4.X, vv4.Y}, {vv8.X, vv8.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv5.X, vv5.Y}, {vv6.X, vv6.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv5.X, vv5.Y}, {vv7.X, vv7.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv7.X, vv7.Y}, {vv8.X, vv8.Y}, color, {LineSize});
    ImGui::GetForegroundDrawList()->AddLine({vv6.X, vv6.Y}, {vv8.X, vv8.Y}, color, {LineSize});
}



 



enum EAimTarget {
    头部 = 0,
    胸部 = 1,
    腹部 = 2,
    腿部 = 3   
};

enum EAimTrigers {
    开火 = 0,
    开镜 = 1,
    开火和开镜 = 2
};

enum LINE {
    a = 0,
    b = 1,
    c = 2
};
static LINE LINE;

struct sConfig {
    struct s人物绘制 {

        bool 绘制初始化;//注入绘制
        bool 自瞄圈;
        bool 方框;//绘制方框
        bool 射线;//绘制射线
        bool 骨骼;//绘制骨骼
        bool 信息;//绘制 名字 距离 队伍 血量
		bool 距离;//绘制距离
        bool 手持;//绘制手持
        bool 载具;//绘制载具
        bool 预警;//背敌预警
        bool 雷达;//绘制准星
        bool 准星;//绘制准
        bool 血量;//绘制血条

    };
    s人物绘制 人物绘制{0};
    
    struct sColorsESP {
        float *玩家方框颜色;
        float *玩家射线颜色;   
        float *玩家骨骼颜色;
        
        float *人机方框颜色;
        float *人机射线颜色;
        float *人机骨骼颜色;       
    };
    sColorsESP ColorsESP{0};
    
    struct sAimbot {    
    bool Aim;//开启自瞄
    bool 倒地;//开启自瞄
    //EAim 类型;
    int 瞄准部位;
    int 瞄准类型;
	bool 自瞄线;
	
    EAimTrigers 触发方式;
    float 自瞄范围=120;
    float 自瞄距离 = 100;
    };
    sAimbot 自动瞄准{0};
    
    struct sHook {  
    bool 无后聚点;//无后座力
    bool 防抖瞬击;//防抖瞬击
    bool 范围伤害;//范围伤害
    bool 平板视角;//平板视角
    bool 全局加速;//全局加速
    bool 全图除雾;//全图除雾
    bool 基址自瞄;//基址自瞄
    };
    sHook 基址功能{0};
};

sConfig Config{0};


bool Tabb=false;
bool interface3=false;
bool isDown=false;
bool isTe=false;

bool stroke = true;
void DrawStrokeText(int x, int y, ImVec4 color, const char *str) {
    if (stroke) {
        ImGui::GetForegroundDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(x - 0.1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
        ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    }
    ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
	ImGui::GetBackgroundDrawList()->AddText(NULL, 0.0f,ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, 0.0f,ImVec2(x - 0.1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, 0.0f,ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, 0.0f,ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetBackgroundDrawList()->AddText(NULL, 0.0f,ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);

}





void DrawRectFilled(int x, int y, int w, int h, ImVec4 color) {
    ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h),
                                               ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ImVec4 Color, int T) {
    ImGui::GetForegroundDrawList()->AddTriangleFilled(ImVec2(x1, y1), ImVec2(x2, y2), ImVec2(x3, y3),
                                                   ImGui::ColorConvertFloat4ToU32(Color));
}

float top1,TOP;










extern "C" {
    JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_init(JNIEnv* env, jclass cls);
    JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_step(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_imgui_Shutdown(JNIEnv* env, jobject obj);
	JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY);
	JNIEXPORT jstring JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_getWindowRect(JNIEnv *env, jobject thiz);
	JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_real(JNIEnv* env, jobject obj, jint width, jint height);
	JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_ScreenSize(JNIEnv* env, jobject obj, jint twidth, jint theight);
};

JNIEXPORT void JNICALL
Java_com_POST_KOP_GLES3JNIViewyjj1_init(JNIEnv* env, jclass cls) {
     // 设置ImGui上下文
    if(g_Initialized) return ;
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // 禁用加载/保存。磁盘上的ini文件。
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename = NULL;
	


	

	 // 设置ImGui风格
	 
	    
	 
	 
	 ImGui::StyleColorsYellow();
	 
  //  ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init();
    ImGui_ImplOpenGL3_Init("#version 300 es");
	
	jfieldID Id = env->GetStaticFieldID(cls,"fontData", "[B");
	jbyteArray byteArray = (jbyteArray)env->GetStaticObjectField(cls,Id);
	jbyte* fontData = env->GetByteArrayElements(byteArray,NULL);
	int fontDataSize= env->GetArrayLength(byteArray);
    font = io.Fonts->AddFontFromMemoryTTF(fontData, fontDataSize, 45.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    IM_ASSERT(font != NULL);
	
	ImGui::GetStyle().ScaleAllSizes(2.0f);
    ImGui::SetNextWindowBgAlpha(5.0);
    ImGuiStyle & style = ImGui::GetStyle();
    
    
    
    //
    
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;
        
        ImFontConfig CustomFont;
        CustomFont.FontDataOwnedByAtlas = false;
        
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.OversampleH = 2.5;
        icons_config.OversampleV = 2.5;
	
	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 60.0f, &icons_config, icons_ranges);
    
   
      // 内距
    style.ScaleAllSizes(1.8f);
    // 窗口菜单按钮位置(就是窗口标题的那个三角形)(-1无 0左 1右)
    style.WindowMenuButtonPosition = 0;
    // 窗体边框圆角
    style.WindowRounding = 10.0f;
    // 框架圆角(比如设置复选框的圆角)
    style.FrameRounding = 1.0f;
    // 框架描边宽度
    style.FrameBorderSize = 0.3f;
    // 滚动条圆角
    style.ScrollbarRounding = 3.0f;
    // 滚动条宽度
    style.ScrollbarSize = 60.0f;
    // 滑块圆角
    style.GrabRounding = 8.0f;
    // 滑块宽度
	style.GrabMinSize = 20.0f;
   
    g_Initialized=true;
}

JNIEXPORT void JNICALL
Java_com_POST_KOP_GLES3JNIViewyjj1_resize(JNIEnv* env, jobject obj, jint width, jint height) {
	screenWidth = (int) width;
    screenHeight = (int) height;
	glViewport(0, 0, width, height);
	ImGuiIO &io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = false;
    io.IniFilename = NULL;
	ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
}


/************分辨**************/
//获取真实分辨
JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_ScreenSize(JNIEnv* env, jobject obj, jint px, jint py) {
	ScreenX=px;
    ScreenY=py;
}

/************用户自定义**************/
//字体定义


//颜色
static ImVec4 Colbox = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 Colline = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 Colbones = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 Colinfor = ImVec4(0.0f, 128.0f, 0.0f, 0.0f);

//定义参数
static float Radius = 100.00f;

//地址定义
int ArrayCount,MyTeam,MaterialLocal,
	TeamID,State,IsBot;
	
	
	
	
	
	
int 随机颜色(){
     int 红;
     int 绿;
     int 蓝;
     int 颜色=70;
     红=(0+255*rand());
     绿=(0+255*rand());
     蓝=(0+255*rand());
     if(sizeof(红)==1) 红=红+红;
     if(sizeof(绿)==1) 绿=绿+绿;
     if(sizeof(蓝)==1) 蓝=蓝+蓝;
     return 颜色+红+绿+蓝;
}



    

int 颜色数组[100];   // 必须分配空间
void 颜色初始化(){
    srand((unsigned)time(NULL));
    for(int 数量=0;数量<100;数量++){
        颜色数组[数量]=随机颜色();
    }
}

int 获取颜色(int 预号){
    if(颜色数组[预号-1]==NULL)
        return 88303030;
    else
        return 颜色数组[预号-1];
}

int 队伍颜色(int 队伍)
{
    if(队伍<=100 && 队伍>0)
    {
        return 获取颜色(队伍);
    }
    else
    {
        return 3411641396;
    }
}

int getProcessIDK(const char *packageName)
{
	int id = -1;
	DIR *dir;
	FILE *fp;
	char filename[64];
	char cmdline[64];
	struct dirent *entry;
	dir = opendir("/proc");
	while ((entry = readdir(dir)) != NULL)
	{
		id = atoi(entry->d_name);
		if (id != 0)
		{
			sprintf(filename, "/proc/%d/cmdline", id);
			fp = fopen(filename, "r");
			if (fp)
			{
				fgets(cmdline, sizeof(cmdline), fp);
				fclose(fp);
				if (strcmp(packageName, cmdline) == 0)
				{
					return id;
				}
			}
		}
	}
	closedir(dir);
	return -1;
}




static ImVec4 ycole[] = {{144/255.f,238/255.f,144/255.f,tm},{135/255.f,206/255.f,255/255.f,tm},{255/255.f,0/255.f,0/255.f,tm},{0/255.f,255/255.f,0/255.f,tm},{0 /255.f,255/255.f, 127/255.f,tm}
                      ,{255/255.f,182/255.f,193/255.f,tm},{218/255.f,112/255.f,214/255.f,tm},{248/255.f,248/255.f,255/255.f,tm},{0/255.f,255/255.f,255/255.f,tm},{255/255.f,165/255.f,0/255.f,tm}
                      ,{153/255.f,204/255.f,255/255.f,tm},{204/255.f,255/255.f,153/255.f,tm},{255/255.f,255/255.f,153/255.f,tm},{255/255.f,153/255.f,153/255.f,tm},{153/255.f,153/255.f,204/255.f,tm}
                      ,{204/255.f,204/255.f,204/255.f,tm},{102/255.f,204/255.f,153/255.f,tm},{255/255.f,102/255.f,0/255.f,tm},{102/255.f,204/255.f,204/255.f,tm},{153/255.f,204/255.f,255/255.f,tm}
};
static int Codem=sizeof(ycole)/20;


static float tmer=255/255.f;
static ImVec4 ycolerr[] = {{144/255.f,238/255.f,144/255.f,tmer},{135/255.f,206/255.f,255/255.f,tmer},{255/255.f,0/255.f,0/255.f,tmer},{0/255.f,255/255.f,0/255.f,tmer},{0 /255.f,255/255.f, 127/255.f,tmer}
                      ,{255/255.f,182/255.f,193/255.f,tmer},{218/255.f,112/255.f,214/255.f,tmer},{248/255.f,248/255.f,255/255.f,tmer},{0/255.f,255/255.f,255/255.f,tmer},{255/255.f,165/255.f,0/255.f,tmer}
                      ,{153/255.f,204/255.f,255/255.f,tmer},{204/255.f,255/255.f,153/255.f,tmer},{255/255.f,255/255.f,153/255.f,tmer},{255/255.f,153/255.f,153/255.f,tmer},{153/255.f,153/255.f,204/255.f,tmer}
                      ,{204/255.f,204/255.f,204/255.f,tmer},{102/255.f,204/255.f,153/255.f,tmer},{255/255.f,102/255.f,0/255.f,tmer},{102/255.f,204/255.f,204/255.f,tmer},{153/255.f,204/255.f,255/255.f,tmer}
};
static int Codemr=sizeof(ycolerr)/20;








bool fd[50];
bool DrawIo[50];
ImColor 玩家方框颜色 = ImColor(255, 255, 255, 255);//自定义方框颜色
ImColor 玩家射线颜色 = ImColor(255, 0, 0, 255);//自定义射线颜色
ImColor 人机方框颜色 = ImColor(255, 255, 255, 255);//自定义方框颜色
ImColor 人机射线颜色 = ImColor(255, 255, 255, 255);//自定义射线颜色

ImColor 玩家骨骼颜色 = ImColor(255,0,0,255);//真人骨骼
ImColor 人机骨骼颜色 = ImColor(0,255,0,255);//人机骨骼



char *GetHol(int dz)
{
    switch (dz)
    {
        
    
    case 17:
        return "行走";
        break;
    case 4194320:
        return "乘坐";
        break;
        
    case 273:
        return "走着开火";
        break;
    case 67108880:
        return "翻墙";
        break;  
    case 544:
        return "蹲着换弹";
        break;
    case 576:
        return "趴着换弹";
        break;
        
    case 320:
        return "趴着开火";
        break;
       
    case 528:
        return "换弹";
        break;   
    case 4384:
        return "蹲着开火";
        break;
    case 4128:
        return "蹲着歪头";
        break;
        
    case 144:
        return "跳远";
        break;
    case 32:
        return "蹲着";
        break;     
    case 64:
        return "趴着";
        break;
    case 6552:
        return "掐雷";
        break;
        
    case 19:
        return "奔跑";
        break;
    case 4112:
        return "歪头";
        break;
    case 272:
        return "开枪";
        break;
    case 1040:
        return "开镜";
        break;
        
    case 2097168:
        return "开车";
        break;
    case 655360:
        return "自救";
        break;
    case 288:
        return "蹲着开火";
        break;
    default:
        return "0";
        break;
    }
    return nullptr;
}




std::string playerstatus(int GetEnemyState)
{    
    std::string StateName = "未知状态";
    
    if (GetEnemyState == 0) {
     StateName = "静止状态"; 
    }
    if (GetEnemyState == 1) {
     StateName = "移动中"; 
    }
    if (GetEnemyState == 8) {
     StateName = "站立"; 
    }
    if (GetEnemyState == 9) {
     StateName = "行走"; 
    }
    if (GetEnemyState == 11 || GetEnemyState == 10) {
     StateName = "奔跑"; 
    }
    if (GetEnemyState == 16) {
     StateName = "蹲下"; 
    }
    if (GetEnemyState >= 17 && GetEnemyState <= 19) {
     StateName = "蹲走"; 
    }
    if (GetEnemyState >= 33 && GetEnemyState <= 35) {
     StateName = "爬行"; 
    }
    if (GetEnemyState == 32) {
     StateName = "趴下"; 
    }
    if (GetEnemyState >= 60 && GetEnemyState <= 100) {
     StateName = "跳跃"; 
    }
    if (GetEnemyState >= 260 && GetEnemyState <= 290) {
     StateName = "换弹"; 
    }
    if (GetEnemyState >= 120 && GetEnemyState <= 170 || GetEnemyState >= 630 && GetEnemyState <= 680) {
     StateName = "射击"; 
    }
    if (GetEnemyState >= 500 && GetEnemyState <= 550 || GetEnemyState >= 1500 && GetEnemyState <= 1550) {
     StateName = "瞄准"; 
    }
    if (GetEnemyState >= 1150 && GetEnemyState <= 1170 || GetEnemyState >= 1670 && GetEnemyState <= 1700) {
     StateName = "射击"; 
    }
    if (GetEnemyState >= 1020 && GetEnemyState <= 1060 || GetEnemyState >= 1530 && GetEnemyState <= 1560) {
     StateName = "探头"; 
    }
    if (GetEnemyState >= 2040 && GetEnemyState <= 2090 || GetEnemyState >= 3080 && GetEnemyState <= 3090) {
     StateName = "切枪"; 
    }
    if (GetEnemyState >= 8200 && GetEnemyState <= 8270) {
     StateName = "挥拳"; 
    }
    if (GetEnemyState == 131072 || GetEnemyState == 131073) {
     StateName = "当场暴毙"; //倒地
    }
    if (GetEnemyState == 262144) {
     StateName = "当场暴毙"; //淘汰 
    }
    if (GetEnemyState == 524296) {
     StateName = "射击"; 
    }
    if (GetEnemyState >= 65540 && GetEnemyState <= 65580) {
     StateName = "治疗中"; 
    }
    if (GetEnemyState >= 16390 && GetEnemyState <= 16420 || GetEnemyState >= 17410 && GetEnemyState <= 17430) {
     StateName = "投掷"; 
    }
    if (GetEnemyState >= 3140000 && GetEnemyState <= 3170000) {
     StateName = "探头"; 
    }
    if (GetEnemyState >= 3146240 && GetEnemyState <= 3146250) {
     StateName = "瞄准敌人"; 
    }
    if (GetEnemyState >= 4194300 && GetEnemyState <= 4194310) {
     StateName = "游泳"; 
    }
    if (GetEnemyState == 524288) {
     StateName = "驾驶船"; 
    }
    if (GetEnemyState == 524296) {
     StateName = "驾驶车辆"; 
    }
    if (GetEnemyState == 1048584) {
     StateName = "乘坐车辆"; 
    }
    if (GetEnemyState == 1050632) {
     StateName = "收枪"; 
    }
    if (GetEnemyState == 1048576 || GetEnemyState == 1050624) {
     StateName = "乘坐船"; 
    }
    if (GetEnemyState >= 16000000 && GetEnemyState <= 17000000) {
     StateName = "攀爬"; 
    }
    if (GetEnemyState == 268435464) {
     StateName = "鼓掌"; 
    }
    if (GetEnemyState >= -2147483640 && GetEnemyState <= -2000000000) {
     StateName = "拍打坤坤"; 
    }
    
     return StateName;
}

char *GetHoldastadt9(int BoxId)
{
	switch (BoxId)
    {
	    case 16:
            return "站立";
        break;
		case 32:
            return "蹲下";
        break;
		case 64:
            return "趴下";
        break;
		case 17 | 33 | 65:
            return "移动";
        break;
		case 19 | 35 | 67:
            return "奔跑";
        break;
		case 144 | 145 | 147:
            return "跳跃";
        break;
		case 4112 | 4128:
            return "探头";
        break;        
        case 1040 | 1041 | 1056 | 1057 | 1088 | 5136 | 5137 | 5152 | 5153 :
            return "瞄准";
        break;
		case 272| 273| 288 |289| 320| 4368| 4369| 1296 |1297| 1312 |1313 |1344 |5392| 5393 |5408 |5409:
            return "开火";
        break;
		case 528| 529| 544 |545| 576:
            return "换弹";
        break;
		case 67108880 |67108881:
            return "攀爬";
        break;
		case 2097168:
            return "驾驶";
        break;
		case 41943204:
            return "乘坐";
        break;
        case 65552| 65553| 69648| 69649| 65568| 65569| 69664| 69665| 65600 |65601:
            return "投掷";
        break;
        case 16777216 |16777217| 16777219:
            return "游泳";
        break;
        case 33554448| 33554449:
            return "跳伞";
        break;
		case 262160| 262161 |262176| 262177 |262208 |262209:
            return "打药";
        break;
		case 1048576| 1048592:
            return "死亡";
        break;
		case 524303| 524304 |524288| 524289:
            return "倒地";
        break;
		case 655360:
            return "自救";
        break;
		case 2064| 2065 |2067 |2080| 2081| 2083| 2112 |2113| 2115| 6160| 6176:
            return "拉栓";
        break;
        case 32784| 32785| 32787| 32800| 32801| 32803 |32912| 32913 |32915:
            return "挥拳";
        break;
		default:
            return "移动";
		break;
	}
	return nullptr;
}











char *GetHolGunItem(int BoxId)
{
    switch (BoxId)
    {
        //突击步枪
        case 101008:
            return "M762";
      
        break;
        case 101001:
            return "AKM";
        break;
        case 101004:
            return "M416";
        break;
        case 101003:
            return "SCAR-L";
        break;
        case 101002:
            return "M16A4";
        break;
        case 101009:
            return "Mk47";
        break;
        case 101006:
            return "AUG";
        break;
        case 101005:
            return "Groza";
        break;
        case 101010:
            return "G36C";
        break;
        case 101007:
            return "QBZ";
        break;
        case 101011:
            return "AC-VAL";
        break;
        case 101012:
            return "蜜獾突击步枪";
        break;
        //连狙
        case 103009:
            return "SLR";
        break;
        case 103005:
            return "VSS";
        break;
        case 103006:
            return "Mini14";
        break;
        case 103010:
            return "QBU";
        break;
        case 103004:
            return "SKS";
        break;
        case 103007:
            return "MK14";
        break;
        case 103014:
            return "MK20-H";
        break;
        case 103013:
            return "M417";
        break;
        //连狙
        case 103012:
            return "AMR";
        break;
        case 103003:
            return "AWM";
        break;
        case 103002:
            return "M24";
        break;
        case 103011:
            return "莫甘娜辛";
        break;
        case 103001:
            return "Kar98K";
        break;
        case 103008:
            return "Win94";
        break;
        //机关枪
        case 105001:
            return "M249";
        break;
        case 105002:
            return "DP-28";
        case 105010:
            return "MG3";
        break;
        case 107001:
            return "十字弩";
        break;
        case 107007:
            return "爆炸猎弓";
        break;
        case 102001:
            return "UZI";
        break;
        case 102003:
            return "Vector";
        break;
        case 100103:
            return "PP-19";
        break;
        case 102007:
            return "MP5K";
        break;
        case 102002:
            return "UMP-45";
        break;
        case 102004:
            return "汤姆逊";
        break;
        case 102105:
            return "P90";
        break;
        case 102005:
            return "野牛";
        break;
        //霰弹枪
        case 104001:
            return "S686";
        break;
        case 104002:
            return "S1897";
        break;
        case 104003:
            return "S12K";
        break;
        case 104004:
            return "DBS";
        break;
        case 104100:
            return "SPAS-12";
        break;
        //投掷爆炸物
        case 602004:
            return "手榴弹";
        break;
        case 602003:
            return "燃烧瓶";
        break;
        case 602002:
            return "烟雾弹";
        break;
        case 602001:
            return "震撼弹";
        break;
        //近战武器
        case 108003:
            return "镰刀";
        break;
        case 108002:
            return "撬棍";
        break;
        case 108001:
            return "大砍刀";
        break;
        case 108004:
            return "平底锅";
        break;
        case 0:
            return "拳头";
        break;
        default:
            return "暂未收录";
        break;
    }
    return nullptr;
}

char *VH_name(unsigned int id)
{
	static char name[10];
	memset(name, '\0', sizeof(name));
	if (id >= 3306001 && id<= 336010)//不知道什么b原因读取不到
		strcpy(name,"小轿车");
	else if (id >= 3306001 && id <= 3306010)//另写一行
		strcpy(name, "小轿车");
	else if (id >= 3301001 && id <= 3303010)
		strcpy(name, "蹦蹦");
	else if (id >= 3334001 && id <= 3334010)
		strcpy(name, "滑翔机");
	else if (id >= 3309001 && id <= 3309001)
		strcpy(name, "三轮摩托");
	else if (id >= 3325001 && id <= 3325010)
		strcpy(name, "快艇");
	else if (id >= 3313001 && id <= 3313010)
		strcpy(name, "敞篷跑车");
	else if (id >= 3336001 && id <= 3336001)
		strcpy(name, "带蓬跑车");
	else if (id >= 3307001 && id <= 3307010)
		strcpy(name,"巴士");
	else if (id >= 3302001 && id <= 3302010)
		strcpy(name,"有篷吉普");
	else if (id >= 3303001 && id <= 3303010)
		strcpy(name,"无蓬吉普");
	else if (id >= 3304001 && id <= 3304010)
		strcpy(name,"吉普车");
	else if (id >= 3314001 && id <= 3314010)
		strcpy(name,"小型皮车");
	else if (id >= 3322001 && id <= 3322010)
		strcpy(name,"雪地吉普");
	else if (id >= 3324001 && id <= 3324010)
		strcpy(name,"雪地摩托");
	else if (id >= 3321001 && id <= 3321010)
		strcpy(name,"雪地摩托车");
	else if (id >= 3308001 && id <= 3308010)
		strcpy(name,"双轮摩托车");
	else if (id >= 3310001 && id <= 3310010)
		strcpy(name,"大型皮卡");
	else if (id >= 3315001 && id <= 3315010)
		strcpy(name,"小绵羊");
	else if (id >= 3326001 && id <= 3326010)
		strcpy(name,"摩托艇");
	else if (id >= 3329001 && id <= 3329010)
		strcpy(name,"装甲车");
	else if (id >= 3312001 && id <= 3312010)
		strcpy(name,"有蓬跑车");
	else if (id >= 3335001 && id <= 3335010)
		strcpy(name,"大脚车");
	else if (id >= 3323001 && id <= 3323010)
		strcpy(name,"三轮车");
	else if (id >= 3348001 && id <= 3348010)
		strcpy(name,"姥爷车");
	return name;
}
long MatrixAddress;
int DrawInt()
{
    pid = getPID("com.tencent.tmgp.pubgmhd");
    if (pid <= 0)
    {
        return -1;
	}
    libbase = get_module_base(pid, "libUE4.so");
     Matrix = getPtr64(getPtr64(libbase + 0xD80FB08) + 0x20) + 0x270;
    MatrixAddress = getPtr64(getPtr64(libbase + 0xD8179E0) + 0x98) + 0x320;    
    Gname = getPtr64(libbase + 0xD398080);

    return 0;
}






bool isZM;

void Box4Line(ImDrawList *draw, float thicc, int x, int y, int w, int h, int color) {
    int iw = w / 4;
    int ih = h / 4;
    // top
    draw->AddRect(ImVec2(x, y),ImVec2(x + iw, y), color, thicc);
    draw->AddRect(ImVec2(x + w - iw, y),ImVec2(x + w, y), color, thicc);
    draw->AddRect(ImVec2(x, y),ImVec2(x, y + ih), color, thicc);
    draw->AddRect(ImVec2(x + w - 1, y),ImVec2(x + w - 1, y + ih), color, thicc);;
    // bottom
    draw->AddRect(ImVec2(x, y + h),ImVec2(x + iw, y + h), color, thicc);
    draw->AddRect(ImVec2(x + w - iw, y + h),ImVec2(x + w, y + h), color, thicc);
    draw->AddRect(ImVec2(x, y + h - ih), ImVec2(x, y + h), color, thicc);
    draw->AddRect(ImVec2(x + w - 1, y + h - ih), ImVec2(x + w - 1, y + h), color, thicc);



}




void DrawPlayer(ImDrawList *Draw)
{
	int Player = 0;//计算真人数量
    int Robot = 0;//计算人机数量
	
    float top,right,left,bottom,x1,top1; 
    py = ScreenY/2;  
    px = ScreenX/2;
	//获取io库
	ImGuiIO &io = ImGui::GetIO();
    
	const ImU32 Boxcol = ImColor(Colbox);
	const ImU32 Linecol = ImColor(Colline);
	const ImU32 Bonescol = ImColor(Colbones);
	const ImU32 Inforcol = ImColor(Colinfor);
	

		int PlayerCount = 0, BotCount = 0;
	
	//-------绘制区---------//
	
    颜色初始化();
   
tempBase = 0;


tempBase = 0;
    Uworld = getPtr64(libbase + 0xD83E758); // 读取游戏世界
    Uleve = getPtr64(Uworld + 0x90);    // Uleve
    Arrayaddr = getPtr64(Uleve + 0xA0); // 对象阵列
    countddr = getdword(Uleve + 0xA8);  // 阵列数量
	oneself = getPtr641(getPtr641(getPtr641(getPtr641(getPtr64(libbase + 0xD83E758) + 0x98) + 0x88) + 0x30) + 0x2D50);  // 自身对象？
	
    Myteam = getdword(oneself + 0xA60); // 自身队伍编号
    kh = getdword(oneself + 0x1dc8);    
    kj = getdword(oneself + 0x14a0);   
    long zm = getPtr64(getPtr64(getPtr64(getPtr64(libbase + 0xD83E758) + 0x98) + 0x88) + 0x30);
     
	float tempY = getfloat(zm + 0x578);
   
	float tempX = getfloat(zm + 0x57c);    
	
	
	
    tempDis = 99999999;

	
	
	
	
	
	
	
	
	
	
	
	
    memset(matrix, 0, 16);
    vm_readv(Matrix, matrix, 16 * 4);     
    PlayerCount = 0;   

    AimCount = 0;
    AimObjCount = 0;

    for (int i = 0; i < countddr; i++)
    
    {
        Objaddr = getPtr64(Arrayaddr + 8 * i);  // 遍历数量次数

        int ClassID = getPtr64(Objaddr + 24);   // 物资id指针10h
        if (ClassID < 1 || ClassID > 2000000)
            continue;
        char ClassName[32] = "";
        long int FNameEntry = getPtr64(getPtr64(Gname + (ClassID / 0x4000) * 0x8) + (ClassID % 0x4000) * 0x8);  // 序
        vm_readv(FNameEntry + 0xC, ClassName, 32);
        Myx = getfloat(MatrixAddress - 0x10);
        Myy = getfloat(MatrixAddress - 0xC);
        Myz = getfloat(MatrixAddress - 0x8); // 矩阵坐标
            
        // 自身坐标 
        Vector3A Z;
        vm_readv(getPtr641(oneself + 0x268) + 0x1B0, &Z, sizeof(Z)); // 自己坐标
               // 敌人和物资坐标
        Vector3A D;
         vm_readv(getPtr641(Objaddr + 0x268) + 0x1B0, &D, sizeof(D)); // 对象坐标    
        // 计算人物矩阵
        camera = matrix[3] * D.X + matrix[7] * D.Y + matrix[11] * D.Z + matrix[15];

        r_x =
            px + (matrix[0] * D.X + matrix[4] * D.Y + matrix[8] * D.Z +
                      matrix[12]) / camera * px;
        r_y =
            py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z - 5) +
                      matrix[13]) / camera * py;
        r_w =
            py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z + 205) +
                      matrix[13]) / camera * py;
        //205    185
		
		   x = r_x - (r_y - r_w) / 4;
            y = r_y;
            w = (r_y - r_w) / 2;
		      MIDDLE = x + w / 2;
            float Distance =
           sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;

            left = (x + w / 2) - w / 2.6f;
            right = x + w / 1.12f;
		
		
		
        if (getfloat(Objaddr + 0x2D20) == 479.5)
        {          
         MIDDLE = x + w / 2;
            float Distance =
           sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;

            left = (x + w / 2) - w / 2.6f;
            right = x + w / 1.12f;
		
      
                                              
            if (Distance > 600)
            {
                continue;
            }
            
            int TeamID = getdword(Objaddr + 0xA60);
            //注意
            float Health = (getfloat(Objaddr + 0xDC0)/100 ) *100;;
            int isBot = getdword(Objaddr + 0xA7C);
           // int State = getdword(Objaddr + 0xf70);
            int State = getdword(Objaddr + 0x1340);
            
            resistance = getFloat(getPtr64(getPtr64(oneself + 0xe98) + 0x11d8) + 0x1718);
		    int scwq = getPtr64(getPtr64(Objaddr + 0xEB8) + 0xa28);
            int sczd = getPtr64(getPtr64(Objaddr + 0xEB8) + 0x10b8);
            int zdmax = getPtr64(getPtr64(Objaddr + 0xEB8) + 0x10bc);
			

            // 敌人向量
            Vector3A Movement;         
            vm_readv(getPtr641(Objaddr + 0x268) + 0x200, &Movement, sizeof(Movement)); // 敌人向量           
            
            float angle = getfloat(getPtr641(oneself + 0x548) + 0x564) - 90;
            Vector2A Radar = rotateCoord(angle, (Z.X - D.X) / 200, (Z.Y - D.Y) / 200);

            if (State == 1048592 || State == 1048576)
            {
                continue;   // 判断是否死亡或成盒
            }
            if (Myteam == TeamID) // 不输出自己和队友坐标
            {
                continue;
            }
            if (TeamID <= 0)
            {
                continue;
            }

            if (Myteam >= 300 || Myteam <= 0)
            {
                                  
            if (Distance > 800)
            {
                continue;
            }
            }

            location = getdword(Objaddr + 0x2f80);

            getUTF8(PlayerName, getPtr64(Objaddr + 0x9E0));

            // 阵列偏移
            Mesh = getPtr641(Objaddr + 0x5D0);
            // 骨骼阵列
            human = Mesh + 0x1A0;
            // 骨骼指针
            Bone = getPtr641(Mesh + 0x6C8) + 0x30;
            // 计算骨节
            
          int BoneCount = getPtr641(Mesh + 0x6C8+8);
            if (Bone <= 0xffff){
            continue;
            }
            int a,b,c,d,e,f,g,h,p,o;
            if (BoneCount==68)
            {
            p=33;
            o=34;
            a=13;
            b=35;
            c=55;
            d=59;
            e=56;
            f=60;
            g=57;
            h=61;
            }else{
            p=32;
            o=33;
            a=63;
            b=62;
            c=52;
            d=56;
            e=53;
            f=57;
            g=54;
            h=58;
            }
            // 计算骨节
            FTransform meshtrans = getBone(human);
            FMatrix c2wMatrix = TransformToMatrix(meshtrans);
            // 头部
            FTransform headtrans = getBone(Bone + 5 * 48);
            FMatrix boneMatrix = TransformToMatrix(headtrans);
            Vector3A relLocation = MarixToVector(MatrixMulti(boneMatrix, c2wMatrix));
            relLocation.Z += 7; // 脖子长度
            Head = WorldToScreen(relLocation, matrix, camera);
            // 胸部
            FTransform chesttrans = getBone(Bone + 4 * 48);
            FMatrix boneMatrix1 = TransformToMatrix(chesttrans);
            Vector3A relLocation1 = MarixToVector(MatrixMulti(boneMatrix1, c2wMatrix));
            Chest = WorldToScreen(relLocation1, matrix, camera);
            // 盆骨
            FTransform pelvistrans = getBone(Bone + 0 * 48);
            FMatrix boneMatrix2 = TransformToMatrix(pelvistrans);
            Vector3A LrelLocation1 = MarixToVector(MatrixMulti(boneMatrix2, c2wMatrix));
            Pelvis = WorldToScreen(LrelLocation1, matrix, camera);
            // 左肩膀
            FTransform lshtrans = getBone(Bone + 11 * 48);
            FMatrix boneMatrix3 = TransformToMatrix(lshtrans);
            Vector3A relLocation2 = MarixToVector(MatrixMulti(boneMatrix3, c2wMatrix));
            Left_Shoulder = WorldToScreen(relLocation2, matrix, camera);
            // 右肩膀
            FTransform rshtrans = getBone(Bone + p * 48);
            FMatrix boneMatrix4 = TransformToMatrix(rshtrans);
            Vector3A relLocation3 = MarixToVector(MatrixMulti(boneMatrix4, c2wMatrix));
            Right_Shoulder = WorldToScreen(relLocation3, matrix, camera);
            // 左手肘
            FTransform lelbtrans = getBone(Bone + 12 * 48);
            FMatrix boneMatrix5 = TransformToMatrix(lelbtrans);
            Vector3A relLocation4 = MarixToVector(MatrixMulti(boneMatrix5, c2wMatrix));
            Left_Elbow = WorldToScreen(relLocation4, matrix, camera);
            // 右手肘
            FTransform relbtrans = getBone(Bone + o * 48);
            FMatrix boneMatrix6 = TransformToMatrix(relbtrans);
            Vector3A relLocation5 = MarixToVector(MatrixMulti(boneMatrix6, c2wMatrix));
            Right_Elbow = WorldToScreen(relLocation5, matrix, camera);
            // 左手腕
            FTransform lwtrans = getBone(Bone + a * 48);
            FMatrix boneMatrix7 = TransformToMatrix(lwtrans);
            Vector3A relLocation6 = MarixToVector(MatrixMulti(boneMatrix7, c2wMatrix));
            Left_Wrist = WorldToScreen(relLocation6, matrix, camera);
            // 右手腕
            FTransform rwtrans = getBone(Bone + b * 48);
            FMatrix boneMatrix8 = TransformToMatrix(rwtrans);
            Vector3A relLocation7 = MarixToVector(MatrixMulti(boneMatrix8, c2wMatrix));
            Right_Wrist = WorldToScreen(relLocation7, matrix, camera);
            // 左大腿
            FTransform Llshtrans = getBone(Bone + c * 48);
            FMatrix boneMatrix9 = TransformToMatrix(Llshtrans);
            Vector3A LrelLocation2 = MarixToVector(MatrixMulti(boneMatrix9, c2wMatrix));
            Left_Thigh = WorldToScreen(LrelLocation2, matrix, camera);
            // 右大腿
            FTransform Lrshtrans = getBone(Bone + d * 48);
            FMatrix boneMatrix10 = TransformToMatrix(Lrshtrans);
            Vector3A LrelLocation3 = MarixToVector(MatrixMulti(boneMatrix10, c2wMatrix));
            Right_Thigh = WorldToScreen(LrelLocation3, matrix, camera);
            // 左膝盖
            FTransform Llelbtrans = getBone(Bone + e * 48);
            FMatrix boneMatrix11 = TransformToMatrix(Llelbtrans);
            Vector3A LrelLocation4 = MarixToVector(MatrixMulti(boneMatrix11, c2wMatrix));
            Left_Knee = WorldToScreen(LrelLocation4, matrix, camera);
            // 右膝盖
            FTransform Lrelbtrans = getBone(Bone + f * 48);
            FMatrix boneMatrix12 = TransformToMatrix(Lrelbtrans);
            Vector3A LrelLocation5 = MarixToVector(MatrixMulti(boneMatrix12, c2wMatrix));
            Right_Knee = WorldToScreen(LrelLocation5, matrix, camera);
            // 左脚腕
            FTransform Llwtrans = getBone(Bone + g * 48);
            FMatrix boneMatrix13 = TransformToMatrix(Llwtrans);
            Vector3A LrelLocation6 = MarixToVector(MatrixMulti(boneMatrix13, c2wMatrix));
            Left_Ankle = WorldToScreen(LrelLocation6, matrix, camera);
            // 右脚腕
            FTransform Lrwtrans = getBone(Bone + h * 48);
            FMatrix boneMatrix14 = TransformToMatrix(Lrwtrans);
            Vector3A LrelLocation7 = MarixToVector(MatrixMulti(boneMatrix14, c2wMatrix));
            Right_Ankle = WorldToScreen(LrelLocation7, matrix, camera);               
            // Max
            FTransform Maxs = getBone(Bone + 6 * 48);
            FMatrix boneMatrix15 = TransformToMatrix(Maxs);
            Vector3A Max = MarixToVector(MatrixMulti(boneMatrix15, c2wMatrix));		
            // Min
            FTransform Mins = getBone(Bone + 58 * 48);
            FMatrix boneMatrix16 = TransformToMatrix(Mins);
            Vector3A Min = MarixToVector(MatrixMulti(boneMatrix16, c2wMatrix));
			
            FTransform Rootz = getBone(Bone + 0 * 48);
            FMatrix boneMatrix17 = TransformToMatrix(Rootz);
            Vector3A relLocation99 = MarixToVector(MatrixMulti(boneMatrix17, c2wMatrix));
            auto Roots = WorldToScreen(relLocation99, matrix, camera);
			
            if (LrelLocation6.Z < LrelLocation7.Z)
            {
                LrelLocation7.Z = LrelLocation6.Z;
            }
			
			
			
		   

			
			
			
			
			
			
			
            
            
            top1 = Pelvis.Y - Head.Y;
            top = Pelvis.Y - top1 - w / 5;    
            
            if (Left_Ankle.Y < Right_Ankle.Y) {
                bottom = Right_Ankle.Y + w / 10;
            } else {
                bottom = Left_Ankle.Y  + w / 10;
            }
            
   
            
			
			
			
			
			//自瞄区域
			
			
                        if (Config.自动瞄准.Aim)
                 {
					   
					   if(w>0) {
                      x = x + (w / 2);

                 
                 Draw->AddRect({(screenWidth/2)-(自瞄范围/2), (screenHeight/2)-(自瞄范围/2)}, {(screenWidth/2)+(自瞄范围/2), (screenHeight/2)+(自瞄范围/2)}, ImColor(255,50,50,255),360, 0, 1.2);
				// Draw->AddCircle({px,py }, 自瞄范围, IM_COL32(255, 188, 000, 255), 0, 1.8f);
    
				 
                    float currXdis = abs(px - MIDDLE);
					
					
					
                    float currYdis = abs(py - y);
            
                    if (currXdis < 自瞄范围/2 && currYdis < 自瞄范围/2)
                    {
                        float currDis = (currXdis + currYdis) / 2;
                        if (currDis < tempDis)
                        {
                            tempDis = currDis;
                            tempBase = Objaddr;
                        }
                         if (自动压枪)
                        {
                            if(Distance > 400)
                            {
                                距离压枪=-0.22f;
                            }
                            else if (Distance > 300)
                            {
                                距离压枪=-0.2f;
                            }
                            else if (Distance > 200)
                            {
                                距离压枪=-0.1f;
                            }
                            else if (Distance > 100)
                            {
                                距离压枪=-0.05f;
                            }
                            else
                            {
                                距离压枪=0.05f;
                            }
                        }
                    }
					
		
					 if (不锁真人 == true && Health == 0)
                    {   //不锁倒地
                        lockBase = 0;
                    }
                    if (不锁人机 == true && IsBot==1)
                    {   //不锁.人机
                        IsBot = 0;
                    }
           

                    if (Objaddr == lockBase)
                    {
                    D3DVector neck;
                    if(Config.自动瞄准.瞄准部位 ==1){
                    neck = getBoneXYZ(human, Bone, 5);
                    }else if (Config.自动瞄准.瞄准部位 ==2) {
                    neck = getBoneXYZ(human, Bone, 4);
                    }else if (Config.自动瞄准.瞄准部位 ==3) {
                    neck = getBoneXYZ(human, Bone, 1); 
                    }
                    
                    
					
			
					
					
                        Draw->AddLine({px, py},{Head.X,Head.Y}, ImColor(ImVec4(36/255.f, 249/255.f, 217/255.f, 255/255.f)), 2);
					aimRotatorzm = getPointingAngleZM(oneself, lockBase, neck.X,neck.Y,neck.Z,Myx, Myy, Myz);
                        	
					
                 if(Config.自动瞄准.瞄准类型 == 3){
                 if(kh == 1 || kj == 257 || kj ==1){
                if(aimRotatorzm.X != 479.5 && aimRotatorzm.Y != 479.5 && aimRotatorzm.Y < 75.5){
                    writefloat(zm + 0x780,aimRotatorzm.Y - tempY - resistance);
                    writefloat(zm + 0x784,(aimRotatorzm.X - tempX));
                   }
                 }
                 }else if(Config.自动瞄准.瞄准类型 == 2){
                     if(kj == 257 || kj ==1){
                if(aimRotatorzm.X != 479.5 && aimRotatorzm.Y != 479.5 && aimRotatorzm.Y < 75.0){
                    writefloat(zm + 0x780,aimRotatorzm.Y - tempY - resistance);
                    writefloat(zm + 0x784,(aimRotatorzm.X - tempX));
                   }
                 }
                     
                 }else if(Config.自动瞄准.瞄准类型 == 1){
                     
                     
                     if(kh == 1){
                if(aimRotatorzm.X != 479.5 && aimRotatorzm.Y != 479.5 && aimRotatorzm.Y < 75.0){
                    writefloat(zm + 0x780,aimRotatorzm.Y - tempY - resistance);
                    writefloat(zm + 0x784,(aimRotatorzm.X - tempX));
				
					
					
					
                   }
				   
				   
				   
				   
                 }
                 }
                
                 
                 
                    }
                }
            
              }
     

         
     
		 
		 
		 
		 //绘图区域
		 
		 
		 
		 
		 
		 
			     if (Config.人物绘制.雷达 && Config.人物绘制.绘制初始化)
            {    
                if (Distance <= 300)
                {              
                    if(isBot == 1)
                    {
                        Draw->AddCircleFilled({ESPMenu.雷达x + Radar.X, ESPMenu.雷达y + Radar.Y}, {2.5}, ImColor(255,255,255));
                    }else{
                        Draw->AddCircleFilled({ESPMenu.雷达x + Radar.X, ESPMenu.雷达y + Radar.Y}, {2.5}, ImColor(255,0,0));
                    }
                }
            }
       
				     if (Config.人物绘制.雷达 && Config.人物绘制.绘制初始化){
               Draw->AddCircle(ImVec2(ESPMenu.雷达x, ESPMenu.雷达y), 150, CircleColor, CircleNum_segments, CircleThickness);  
               Draw->AddCircle(ImVec2(ESPMenu.雷达x, ESPMenu.雷达y), 40, CircleColor, CircleNum_segments, CircleThickness); 
               Draw->AddLine(ImVec2(ESPMenu.雷达x, ESPMenu.雷达y), ImVec2(ESPMenu.雷达x - 150, ESPMenu.雷达y - 150), LiNEfcoler, LineThickness);
               Draw->AddLine(ImVec2(ESPMenu.雷达x, ESPMenu.雷达y), ImVec2(ESPMenu.雷达x +150 , ESPMenu.雷达y -150), LiNEfcoler, LineThickness);
               
          }
		  
		  
	    if (IsBot==1)
            {
                Robot++;
            } else {
                Player++;
            }
            //背敌
            if (Config.人物绘制.预警 && Config.人物绘制.绘制初始化)
            {
                if (w > 0)
                {
                    x = x + (w / 2);
                    std::string ssd;
                    ssd += std::to_string((int)Distance);
                    ssd += "M";
                    auto textSize = ImGui::CalcTextSize(ssd.c_str(), 0, 25);
                    if (x + (w / 2) < 0) {
                        Draw->AddRectFilled({4, y - 23}, {90 - 2, y + 23},ImColor(255, 50, 50,255), 5, 0);
                        Draw->AddText(NULL, 30, ImVec2(15, y-14),ImColor(255, 255, 255,255), ssd.c_str());
                    } else if(x - (w / 2) > px*2) {
                        Draw->AddRectFilled({px*2 - 88, y - 23}, {px*2 - 4, y + 23},ImColor(255, 50, 50,255), 5, 0);
                        Draw->AddText(NULL, 30,ImVec2(px*2 - 70, y-14),ImColor(255, 255, 255,255), ssd.c_str());
                    } else if(y + w < 0) {
                        Draw->AddRectFilled({x - 43, 2}, {x + 43, 48},ImColor(255, 50, 50,255), 5, 0);
                        Draw->AddText(NULL, 30, ImVec2(x-30, 8),ImColor(255, 255, 255,255), ssd.c_str());
                    }
                } else {
                    if (Config.人物绘制.预警 && Config.人物绘制.绘制初始化)
                    {
                        std::string ssd;
                        ssd += std::to_string((int)Distance);
                        ssd += "M";
                        Draw->AddRectFilled({x - 43, py*2 - 48}, {x + 50, py*2 - 4},ImColor(255, 50, 50,255),5, 0);
                        Draw->AddText(NULL, 30, ImVec2(x-30, py*2 - 40.5),ImColor(255, 255, 255,255), ssd.c_str());
                    }
                }
            }
       
	
		  
		  
		  
		  
		  
		  
		  
		  
		  
		  
		  
		  
            if(w>0 && Config.人物绘制.绘制初始化)
            {                                
               
		      if (Config.人物绘制.方框 && Config.人物绘制.绘制初始化)
                {
                    // 方框
                    if (isBot == 1)
                    {
                        if (ESPMenu.xz == 2)
                        {
                        Draw3DBox(camera,relLocation,Min,ESPMenu. BotBoxColor,matrix,1.5,w);
                      
                        }else{
                        Draw->AddRect({left, top}, {right, bottom}, ESPMenu. BotBoxColor, 0, 0,2.0f);
                        }
                    }else{
                        if (ESPMenu.xz == 2)
                        {               
                       Draw3DBox(camera,relLocation,Min,ESPMenu. BoxColor,matrix,1.5,w);
                        }else{
                        Draw->AddRect({left, top}, {right, bottom}, ESPMenu. BoxColor, 0, 0,2.0f);

                        }
                    }                   
                }
			
				
				if(w>0){
				if (Config.人物绘制.距离 && Config.人物绘制.绘制初始化)  //距离
                {
                      if (isBot)
                    {
                        std::string s;
							    
                            // 距离
                            s += " ";
                            s += std::to_string((int) Distance);
							s += " m";
                            auto textSize = ImGui::CalcTextSize(s.c_str(), 0, 25);
							Draw->AddText(NULL,20, {right-43, bottom+8}, ImColor(255,155,155,255),s.c_str());                                  
				
                        
                    }else{                                                              
                        std::string s;       
                    
                        s += " ";
                        s += std::to_string((int) Distance);   
						s += "m";
                       auto textSize = ImGui::CalcTextSize(s.c_str(), 0, 25);
				       Draw->AddText(NULL,20, {right-43, bottom+8}, ImColor(255,155,155,255),s.c_str());                                
                }
				}
                }
                if (Config.人物绘制.血量 && Config.人物绘制.绘制初始化)
                {
                    Draw->AddRectFilled({MIDDLE - 105, TOP - 33}, {MIDDLE - 105 + (2.1 * Health), TOP - 28}, health_Color, 1, 0);
					
					
					
                }
				
				          if (Config.人物绘制.血量 && Config.人物绘制.绘制初始化)
                {
                    Draw->AddRectFilled({right - 110, top - 54}, {right + 110, top - 13}, ImColor(0, 0, 0,70), 5, 0);//信息背景
                    Draw->AddRectFilled({right-65,top-24}, {right+100,top-19},ImColor(255,255,255,50),5,0);//血条背景
                    if (Health > 100)
                    {
                        Colorofblood = ImColor(255, 255, 50,180);
                        Health = 100;
                    }
                    else if (Health < 0)
                    {
                        Colorofblood = ImColor(255, 50, 50,180);
                  
                    }
                    else if (Health < 21)
                    {
                        Colorofblood = ImColor(255, 50, 50,180);
                    }
                    else if (Health < 51)
                    {
                        Colorofblood = ImColor(255, 133, 133,180);
                    }
                    else if (Health < 76)
                    {
                        Colorofblood = ImColor(255, 208, 96,180);
                    }
                    else
                    {
                        Colorofblood = ImColor(255, 255, 255,180);
                    }
                    Draw->AddRectFilled({right-65,top-24}, {right-100 + 2.02f * Health,top-19},Colorofblood,5,0);
                }
			
			
            if(Config.人物绘制.射线 && Config.人物绘制.绘制初始化)//绘制射线
            {
               // if (LINE == LINE::a) {
                if (isBot == 1)
                {
                    Draw->AddLine({px, 1}, {Head.X, Head.Y}, 人机射线颜色, 线条粗细);  
                }else{
                    Draw->AddLine({px, 1}, {Head.X, Head.Y}, 玩家射线颜色, 线条粗细);  
                }
              //  }
            
                if (LINE == LINE::b) {
                if (isBot == 1)
                {
                    Draw->AddLine({px, py /2}, {Head.X, Head.Y}, 人机射线颜色, 线条粗细);  
                }else{
                    Draw->AddLine({px, py /2}, {Head.X, Head.Y}, 玩家射线颜色, 线条粗细);  
                }
                }
                
                
                if (LINE == LINE::c) {
                if (isBot == 1)
                {
                    Draw->AddLine({px, 716}, {Head.X, Head.Y}, 人机射线颜色, 线条粗细);  
                }else{
                    Draw->AddLine({px, 716}, {Head.X, Head.Y}, 玩家射线颜色, 线条粗细);  
                } 	 
				} 
				}
				}
				 
			
			
			
			if(w>0){
                
                if (Config.人物绘制.骨骼 && Config.人物绘制.绘制初始化) {
                    // 骨骼
                    if (isBot == 1)
                    {
                        Draw->AddCircle({Head.X , Head.Y}, w/14, 人机骨骼颜色, 0);    
                        Draw->AddLine({Head.X, Head.Y}, {Chest.X, Chest.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Chest.X, Chest.Y}, {Pelvis.X, Pelvis.Y},人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Chest.X, Chest.Y}, {Left_Shoulder.X,Left_Shoulder.Y},人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Chest.X, Chest.Y}, {Right_Shoulder.X,Right_Shoulder.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Shoulder.X, Left_Shoulder.Y}, {Left_Elbow.X,Left_Elbow.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Shoulder.X, Right_Shoulder.Y},{Right_Elbow.X, Right_Elbow.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Elbow.X, Left_Elbow.Y}, {Left_Wrist.X,Left_Wrist.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Elbow.X, Right_Elbow.Y}, {Right_Wrist.X,Right_Wrist.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Pelvis.X, Pelvis.Y}, {Left_Thigh.X, Left_Thigh.Y},人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Pelvis.X, Pelvis.Y}, {Right_Thigh.X,Right_Thigh.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Thigh.X, Left_Thigh.Y}, {Left_Knee.X,Left_Knee.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Thigh.X, Right_Thigh.Y}, {Right_Knee.X,Right_Knee.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Knee.X, Left_Knee.Y}, {Left_Ankle.X,Left_Ankle.Y}, 人机骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Knee.X, Right_Knee.Y}, {Right_Ankle.X,Right_Ankle.Y}, 人机骨骼颜色,骨骼粗细);                      
                    }else{
                        Draw->AddCircle({Head.X , Head.Y}, w/14, 玩家骨骼颜色, 0);    
                        Draw->AddLine({Head.X, Head.Y}, {Chest.X, Chest.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Chest.X, Chest.Y}, {Pelvis.X, Pelvis.Y},玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Chest.X, Chest.Y}, {Left_Shoulder.X,Left_Shoulder.Y},玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Chest.X, Chest.Y}, {Right_Shoulder.X,Right_Shoulder.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Shoulder.X, Left_Shoulder.Y}, {Left_Elbow.X,Left_Elbow.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Shoulder.X, Right_Shoulder.Y},{Right_Elbow.X, Right_Elbow.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Elbow.X, Left_Elbow.Y}, {Left_Wrist.X,Left_Wrist.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Elbow.X, Right_Elbow.Y}, {Right_Wrist.X,Right_Wrist.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Pelvis.X, Pelvis.Y}, {Left_Thigh.X, Left_Thigh.Y},玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Pelvis.X, Pelvis.Y}, {Right_Thigh.X,Right_Thigh.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Thigh.X, Left_Thigh.Y}, {Left_Knee.X,Left_Knee.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Thigh.X, Right_Thigh.Y}, {Right_Knee.X,Right_Knee.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Left_Knee.X, Left_Knee.Y}, {Left_Ankle.X,Left_Ankle.Y}, 玩家骨骼颜色,骨骼粗细);
                        Draw->AddLine({Right_Knee.X, Right_Knee.Y}, {Right_Ankle.X,Right_Ankle.Y}, 玩家骨骼颜色,骨骼粗细);                  
                    }
                }
                }
				                Rect box{};
                    box.x = Head.X - (Roots.Y - Head.Y) / 4;
                    box.y = Head.Y;
                    box.w = (Roots.Y - Head.Y) / 2;
                    box.h = Roots.Y - box.y;
				ImColor 颜色 = ImColor(队伍颜色((int)TeamID));
				
				
               
					  if(w>0){
                if (Config.人物绘制.信息 && Config.人物绘制.绘制初始化) {
                std::string q;
                q += GetHoldastadt9(State);
                auto textze = ImGui::CalcTextSize2(q.c_str(), 0, 29);
                Draw->AddText(NULL, 29, {x - 135, y + w + 6}, ImColor(255, 255, 255, 255), q.c_str());  //文本
                
                
                
                Draw->AddRectFilled({MIDDLE - 105, top - 43}, {MIDDLE - 105 + (2.1 * Health), top - 38}, ImColor(255, 255, 255, 255), 1, 0);
  //血量              
                Draw->AddRectFilled({MIDDLE - 105, top - 76}, {MIDDLE - 65, top - 43}, halflackColor, 1, 0);
                Draw->AddRectFilled({MIDDLE - 65, top - 76}, {MIDDLE + 105, top - 43}, ImColor(ycole[TeamID%Codem]), 1, 0);
                
                 std::string s;
                s += std::to_string((int)TeamID);
                auto textSize = ImGui::CalcTextSize2(s.c_str(), 0, 30);
                Draw->AddText(NULL, 30, {MIDDLE - 84 - (textSize.x / 2), top - 73.5}, ImColor(255, 255, 255, 255), s.c_str());
                    if (isBot == 1)
                    {
                    
                    Draw->AddText(NULL, 30,{MIDDLE - 60, top - 73.5}, ImColor(255, 255, 255, 255), "人机|RoBot");
                    } else {                 
                    Draw->AddText(NULL, 30,{MIDDLE - 60, top - 73.5}, ImColor(255, 255, 255, 255),PlayerName);          
                    }

                }
					  
					  }
				PlayerCount++;          
			  			 	  
					     
                  
			  
          
					  
                    if(w>0){
                  if (Config.人物绘制.手持 && Config.人物绘制.绘制初始化) {
                   Draw->AddRectFilled({right - 110, top - 90}, {right + 110, top - 57}, ImColor(0, 0, 0,70), 5, 0);//信息背景
                    std::string asc;
                    asc += GetHol(State);
                    asc += "|";
                    asc += GetHolGunItem(scwq);
                    asc += "|";
                    asc += std::to_string((int)sczd);
					asc += "|";
					asc += std::to_string((int) zdmax);
                    auto Size = ImGui::CalcTextSize(asc.c_str(),0, 32);
                    Draw->AddText(NULL,30, {right - (Size.x / 2)+5, top- 100}, ImColor(255, 255, 255,255), asc.c_str());
                }
		}
				
				
			}	
				
				
				
				
				
				
				
			
				 
				 
			
           
              
                                                                    
                 
                if (strstr(ClassName, "VH") || (strstr(ClassName, "PickUp_") && !strstr(ClassName, "BP"))
		        || strstr(ClassName, "Rony")|| strstr(ClassName, "AirDropListWrapperActor") || strstr(ClassName, "Mirado") || strstr(ClassName, "LadaNiva")
		        || strstr(ClassName, "AquaRail")) {//Vehicle
                ImVec4 vp_color = {0,0,0,0};
				          
				
				  if (Config.人物绘制.载具 && Config.人物绘制.绘制初始化){     
			    if (w < 0) { continue; }
                if(GetVehicleInfo(ClassName, &vp_name, &vp_color))
                {
                    long int VehiclePointer = getPtr64(Objaddr + 0xa50);
                    int Fuel = (int)(getFloat(VehiclePointer + 0x1d8) / getFloat(VehiclePointer + 0x1d4) * 100);
                    int HP = (int)(getFloat(VehiclePointer + 0x1b4) / getFloat(VehiclePointer + 0x1b4) *100);
                    
                    if (HP == 0)
                    continue;
					
                    //载具的驾驶状态
                    bool bDriveState = getdword(Objaddr + 0x17d8) == 65792;
					
                    char Temp[256] = "";
					std::string goods;   
                    goods += vp_name;
                        if(w > 0) {
                        x = x + (w / 2);
                        if (Fuel > 100)
                        {
                            //超出100血量则判定100为黄色血条
                            OilColor = ImColor(255, 255, 50,180);
                            Fuel = 100;
                        }
                        else if (Fuel < 0)
                        {
                            OilColor = ImColor(255, 50, 50,180);
                        }
                        else if (Fuel < 21)
                        {
                            OilColor = ImColor(255, 50, 50,180);
                        }
                        else if (Fuel < 51)
                        {
                            OilColor = ImColor(255, 133, 133,180);
                        }
                        else if (Fuel < 76)
                        {
                            OilColor = ImColor(255, 208, 96,180);
                        }
                        else
                        {
                            OilColor = ImColor(255, 255, 255,180);
                        }

                        if (HP > 100)
                        {
                            //超出100血量则判定100为黄色血条
                            DurableColor = ImColor(255, 255, 50,180);
                            HP = 100;
                        }
                        else if (HP < 0)
                        {
                            DurableColor = ImColor(255, 50, 50,180);
                        }
                        else if (HP < 21)
                        {
                            DurableColor = ImColor(255, 50, 50,180);
                        }
                        else if (HP < 51)
                        {
                            DurableColor = ImColor(255, 133, 133,180);
                        }
                        else if (HP < 76)
                        {
                            DurableColor = ImColor(255, 208, 96,180);
                        }
                        else
                        {
                            DurableColor = ImColor(50, 255, 50,180);
                        }
                          if (bDriveState) {
                            sprintf(Temp, "%s:%dM", goods.c_str(), int(Distance));
                            Draw->AddText(NULL,30, {x-65, y},ImColor(255, 0, 0,255),Temp);

                            Draw->AddRectFilled({x-60,y-24}, {x+55,y-16},ImColor(0,0,0,50),5,0);//耐久条背景

                            Draw->AddRectFilled({x-55,y-24}, {x-55 + 1.13f * HP,y-16},DurableColor,5,0);//耐久条
                        } else {
                            Draw->AddRectFilled({x-75,y-73}, {x+80,y-43},ImColor(0,0,0,70),5,0);//载具信息背景

                            sprintf(Temp, "%s:%dM", goods.c_str(), int(Distance));
                            Draw->AddText(NULL,30, {x-65, y-73},ImColor(255, 255, 255,255),Temp);

                            Draw->AddRectFilled({x-55,y-35}, {x+55,y-27},ImColor(0,0,0,50),5,0);//油量条背景
                            Draw->AddRectFilled({x-55,y-24}, {x+55,y-16},ImColor(0,0,0,50),5,0);//耐久条背景

                            Draw->AddRectFilled({x-55,y-35}, {x-55 + 1.13f * Fuel,y-27},OilColor,5,0);//油量条
                            Draw->AddRectFilled({x-55,y-24}, {x-55 + 1.13f * HP,y-16},DurableColor,5,0);//耐久条
                    }                    
                }	
		}
    
    
	}
	
         }     
       
			}	 
		  
           	
				 
				
	

       
	
	
	
	
	
lockBase = tempBase;    
  
		       if (PlayerCount == 0)
        {
            Draw->AddRectFilled({px-80,40},{px+80,110},ImColor(10,255,10,110),{25});
            Draw->AddRect({px-81,39},{px+81,111},ImColor(0,255,0,190),{25},0,{2.5f});  
            std::string count;    
            count += "安全 ";
            auto textSize = ImGui::CalcTextSize(count.c_str(), 0, 25);            
            Draw->AddText({px-(textSize.x/2.3),55}, ImColor(255,255,255), count.c_str());
        }else{
        Draw->AddRectFilled({px-80,40},{px+80,110},ImColor(255,10,10,110),{25});
            Draw->AddRect({px-81,39},{px+81,111},ImColor(255,0,0,190),{25},0,{2.5f});  
            std::string count;    
            count += std::to_string((int)PlayerCount);
            auto textSize = ImGui::CalcTextSize(count.c_str(), 0, 25);            
            Draw->AddText({px-(textSize.x/2.3),55}, ImColor(255,255,255), count.c_str());                    
        }   
            
					
		                                                                   
                
			  
				 
  
    	
		
   
		  
		  
    	
		
		
		
		
		
            
	if (Config.人物绘制.准星){
    Draw->AddCircleFilled({px,py}, {3/2.2/1}, ImColor(255,000,000));
	
Draw->AddLine({screenWidth/2 - 20, screenHeight/2}, {screenWidth/2 + 20, screenHeight/2}, ImColor(255, 0, 0, 255), 2.0f);
Draw->AddLine({screenWidth/2, screenHeight/2 - 20}, {screenWidth/2, screenHeight/2 + 20}, ImColor(255, 0, 0, 255), 2.0f);



	
  //  Draw->AddCircle({screenWidth/2, screenHeight/2}, 15,ImColor(255, 000, 000, 255), 0, 2.0f);
    }
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	Draw->AddText(NULL,32,{px/3,658}, ImColor(115,209,183,158), "狗子&");
	
	
	
	
	
	
	
	
	
 
if (bypassLOBBY) {//大厅防封
int APPPID = getPID("com.tencent.tmgp.pubgmhd");
char bypass[64];
sprintf(bypass, "/proc/%d/mem", APPPID);
handle = open(bypass, O_RDWR);
if (handle == 0){}
long int morishimaMK = get_module_base(APPPID, "libgcloud.so");

}

  //  ImGui::GetBackgroundDrawList()->AddCircle({screenWidth/2, screenHeight/2}, 300,ImColor(255,0,0,255),0);
}




void Cumim(){
    ImGuiStyle & style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::SetNextWindowSize({430, 260});
    ImGui::SetNextWindowPos(ImVec2((g_window->Pos.x + g_window->Size.x * 0.5f) - 430 * 0.5f, (g_window->Pos.y + g_window->Size.y * 0.5f) - 260 * 0.5f));
    ImGui::OpenPopup("提示");
    if (ImGui::BeginPopupModal("提示", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (login) {
            
        } else {
            ImGui::Text("用户名或密码错误");
            ImGui::Text("             ");
            ImGui::SameLine();
            if (ImGui::Button("关闭")) {
                LOGE("关闭unmso");
                Gnzha=false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

}


void Cuyhm(){
    ImGuiStyle & style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::SetNextWindowSize({430, 260});
    ImGui::SetNextWindowPos(ImVec2((g_window->Pos.x + g_window->Size.x * 0.5f) - 430 * 0.5f, (g_window->Pos.y + g_window->Size.y * 0.5f) - 260 * 0.5f));
    ImGui::OpenPopup("提示");
    if (ImGui::BeginPopupModal("提示", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (login) {
            
        } else {
            ImGui::Text("用户不存在");
            ImGui::Text("             ");
            ImGui::SameLine();
            if (ImGui::Button("关闭")) {
                LOGE("关闭unmso");
                Gnzhb=false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

}

bool UUnmloo()
{
    char command[256] = "";
    memset(command, 0, 256);
    if ((access("/system/bin/ifconfig", 0)) != -1){
        sprintf(command, "%s", "/system/bin/ifconfig");
    }else{
        return true;
    }
    FILE* fp = NULL;
    char line[1024] = "";
    fp = popen(command, "r");
    while (fgets(line, 1024, fp) != NULL)
    {
        if (strstr(line, "tun0") != NULL || strstr(line, "ppppp0") != NULL){
            pclose(fp);
            return true;
        }
    }
    pclose(fp);
    return false;
}

void vunjc(){
    ImGuiStyle & style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::SetNextWindowSize({430, 260});
    ImGui::SetNextWindowPos(ImVec2((g_window->Pos.x + g_window->Size.x * 0.5f) - 430 * 0.5f, (g_window->Pos.y + g_window->Size.y * 0.5f) - 260 * 0.5f));
    ImGui::OpenPopup("提示");
    if (ImGui::BeginPopupModal("提示", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (login) {
            
        } else {
            ImGui::Text("请更新版本");
            ImGui::Text("             ");
            ImGui::SameLine();
            if (ImGui::Button("关闭")) {
                LOGE("关闭unmso");
                Gnzhf=false;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

}


void Cgclll(){
    ImGuiStyle & style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::SetNextWindowSize({430, 260});
    ImGui::SetNextWindowPos(ImVec2((g_window->Pos.x + g_window->Size.x * 0.5f) - 430 * 0.5f, (g_window->Pos.y + g_window->Size.y * 0.5f) - 260 * 0.5f));
    ImGui::OpenPopup("提示");
    if (ImGui::BeginPopupModal("提示", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (login) {
            
        } else {
            ImGui::Text("登录成功");
            ImGui::Text("             ");
            ImGui::SameLine();
            if (ImGui::Button("关闭")) {
                LOGE("关闭unmso");
                    b1= true;   
                Bon=3;
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::EndPopup();
    }

}

void Cudql(){
    ImGuiStyle & style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::SetNextWindowSize({430, 260});
    ImGui::SetNextWindowPos(ImVec2((g_window->Pos.x + g_window->Size.x * 0.5f) - 430 * 0.5f, (g_window->Pos.y + g_window->Size.y * 0.5f) - 260 * 0.5f));
    ImGui::OpenPopup("提示");
    if (ImGui::BeginPopupModal("提示", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("使用时间到期,请充值");
            ImGui::Text("             ");
            ImGui::SameLine();
            if (ImGui::Button("关闭")) {
                LOGE("关闭unmso");
                Gnzhc= false;
                ImGui::CloseCurrentPopup();           
        }
        ImGui::EndPopup();
    }
}
void Cuall(){
    ImGuiStyle & style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5, 0.5);
    ImGui::SetNextWindowSize({430, 260});
    ImGui::SetNextWindowPos(ImVec2((g_window->Pos.x + g_window->Size.x * 0.5f) - 430 * 0.5f, (g_window->Pos.y + g_window->Size.y * 0.5f) - 260 * 0.5f));
    ImGui::OpenPopup("提示");
    if (ImGui::BeginPopupModal("提示", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("未知错误");
            ImGui::Text("             ");
            ImGui::SameLine();
            if (ImGui::Button("关闭")) {
                LOGE("关闭unmso");
                Gnzhd= false;
                ImGui::CloseCurrentPopup();           
        }
        ImGui::EndPopup();
    }
}


void *YCd(void *arg)
{
jzjdt=1;     
	
}

void *YCc(void *arg)
{
jzjdt=0.92;
	
}

void *YCb(void *arg)
{
jzjdt=0.52;
	
}


void *YCa(void *arg)
{
jzjdt=0.13;     
	
}

    void    ClearLog()
    {
    ImVector<char*>       Items;
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
    }
    
void putClipboardStr(JNIEnv *env, char *str) {
    jstring jstr = env->NewStringUTF(str);
    jclass glViewClass = env->GetObjectClass(g_object);
    jmethodID putClipboard = env->GetMethodID(glViewClass, "putClipboardStr","(Ljava/lang/String;)V");
    env->CallVoidMethod(g_object, putClipboard, jstr);
    env->DeleteLocalRef(jstr);
}

void InitMenu() {
 

                  if (Bon == 3) {
                if(ESPMenu.HZbox[40]){
                pid = getPID("com.tencent.tmgp.pubgmhd");
                }
                }
        if(0<tpisWindowShowTime){  //为什么要这样写
        //后面使用递减达到消失目的
        umVopng.uipngb =  createTexture("/storage/emulated/0/Anodmlop/bg.png");
            tpisWindowShowTime--;  //递减
            ImGui::SetNextWindowPos(ImVec2(ScreenX/2-150, 40), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(300, 100));
            ImGui::SetNextWindowBgAlpha(0);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
            ImGui::Begin("Qion", NULL, ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoInputs |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoScrollbar);
                ImGui::Image(umVopng.uipngb, ImGui::GetContentRegionAvail());
            ImGui::SameLine(1);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0, 0.0, 0.0, 0.0));
            ImGui::BeginChild("Qion", ImVec2(299.0f, 200.0f), false, ImGuiWindowFlags_NoMove);         
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::SetWindowFontScale(1.2f);
            ImGui::Spacing();
            ImGui::Text(tpisWindowShowText);

            ImGui::EndChild();
            ImGui::End();
        }
}
  





bool 开关(const char* label, bool* pToggle, float width) {
    const float Multiplier = 1.5f;
    const float CornerRadius = 7.0f;  // 圆角值
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImVec2 pos = window->DC.CursorPos;
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const float height = label_size.y / Multiplier + style.FramePadding.y * Multiplier;
    const float adjustedWidth = width * Multiplier;
    const float adjustedHeight = width / Multiplier;
    const ImGuiID id = window->GetID(label);
    const ImU32 shadowColor = IM_COL32(0, 0, 0, 64);
    const ImVec2 shadowOffset(3.0f, 3.0f);
    const ImVec2 shadowStartPos = ImVec2(pos.x + shadowOffset.x, pos.y + shadowOffset.y);
    const ImVec2 shadowEndPos = ImVec2(shadowStartPos.x + adjustedWidth, shadowStartPos.y + adjustedHeight);
    ImGui::GetWindowDrawList()->AddRectFilled(shadowStartPos, shadowEndPos, shadowColor, CornerRadius);
    const ImVec2 text_pos(pos.x + style.FramePadding.x, pos.y + style.FramePadding.y);
    ImGui::InvisibleButton(label, ImVec2(adjustedWidth, adjustedHeight));
    if (ImGui::IsItemClicked())
        *pToggle = !*pToggle;
    const ImU32 bg_color = ImGui::GetColorU32(*pToggle ? IM_COL32(66, 150,249,102) : IM_COL32(188, 185, 180, 255));
    const ImU32 slider_color = IM_COL32(255, 255, 255, 255);
    const ImU32 slider_hovered_color = IM_COL32(255, 255, 255, 255);
    const ImU32 slider_active_color = IM_COL32(255, 255, 255, 200);
    window->DrawList->AddRectFilled(pos, ImVec2(pos.x + adjustedWidth, pos.y + adjustedHeight), bg_color, CornerRadius);
    float t = *pToggle ? 1.0f : 0.0f;
    ImVec2 knob_pos = ImVec2(ImLerp(pos.x, pos.x + adjustedWidth - adjustedHeight + 7, t), pos.y);
    window->DrawList->AddRectFilled(knob_pos, ImVec2(knob_pos.x + adjustedHeight, knob_pos.y + adjustedHeight), ImGui::IsItemActive() ? slider_active_color : (ImGui::IsItemHovered() ? slider_hovered_color : slider_color), CornerRadius);
    return ImGui::IsItemActive();
}
void CardPanel(const char* title, const ImVec2& size, const ImVec4& bgColor, float cornerRadius,float padding, const std::function<void()>& contentFunction) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, bgColor);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, cornerRadius);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));
    ImGui::BeginChild(title, size, true);
    if (contentFunction) {
        contentFunction();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}
void CheckboxWith(const char* label, bool* pValue, float yPos) {
    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;
    ImVec2 pos = ImVec2(ImGui::GetCursorScreenPos().x, yPos);
    ImVec2 textSize = ImGui::CalcTextSize(label);
    ImVec2 bgSize = ImVec2(std::max(textSize.x, textSize.y) + style.FramePadding.x * 2.0f, textSize.y + style.FramePadding.y * 2.0f + 15.0f);
    ImVec2 checkBoxSize = ImVec2(textSize.y + style.FramePadding.y * 2.0f, textSize.y + style.FramePadding.y * 2.0f + 15.0f);
    ImU32 bgColor = c_透明;
    ImU32 borderColor = IM_COL32(255, 255, 255, 255); // 默认白色边框
    ImU32 selectedBorderColor = c_DAB123; // 选中后的黄色边框
    ImU32 triangleColor = c_DAB123; // 黄色三角形区域
    ImU32 checkMarkColor = IM_COL32(0, 0, 0, 255); // √符号颜色
    ImVec2 buttonMin = ImVec2(pos.x, pos.y);
    ImVec2 buttonMax = ImVec2(pos.x + bgSize.x, pos.y + bgSize.y);
    ImVec2 textPos = ImVec2(pos.x + style.FramePadding.x + 5.0f, pos.y + style.FramePadding.y + 5.0f);
    ImGui::SetCursorScreenPos(textPos);
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(ImRect(buttonMin, buttonMax), ImGui::GetID(label), &hovered, &held);
    if (*pValue) {
        borderColor = selectedBorderColor;
    }
    ImGui::GetWindowDrawList()->AddRect(buttonMin, buttonMax, borderColor, style.FrameRounding, ImDrawCornerFlags_All, 1.5f);
    ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + bgSize.x, pos.y + bgSize.y), bgColor, style.FrameRounding, ImDrawCornerFlags_All);
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), label);
    if (*pValue) {
        ImVec2 trianglePos = ImVec2(pos.x , pos.y );
        ImVec2 triangleP1 = trianglePos;
        ImVec2 triangleP2 = ImVec2(trianglePos.x + 40.0f, trianglePos.y);
        ImVec2 triangleP3 = ImVec2(trianglePos.x, trianglePos.y + 40.0f);
        ImGui::GetWindowDrawList()->AddTriangleFilled(triangleP1, triangleP2, triangleP3, triangleColor);
        ImVec2 checkPos = ImVec2(trianglePos.x + 3.0f, trianglePos.y - 10);
        ImGui::GetWindowDrawList()->AddText(checkPos, checkMarkColor, "o");
    }
    if (pressed) {
        *pValue = !(*pValue);
    }
}

static ImVec4 colorArray[] = {
    ImVec4(1.0f, 0.341f, 0.133f, 1.0f),  // #ff5722
    ImVec4(1.0f, 0.722f, 0.0f, 1.0f),   // #ffb800
    ImVec4(0.086f, 0.729f, 0.667f, 1.0f), // #16baaa
    ImVec4(0.117f, 0.62f, 1.0f, 1.0f),   // #1e9fff
    ImVec4(0.635f, 0.2f, 0.776f, 1.0f),  // #a233c6
    ImVec4(0.184f, 0.215f, 0.235f, 1.0f), // #2f363c
    ImVec4(0.980f, 0.980f, 0.980f, 1.0f) // #fafafa
};



void BeginDraw()
    {
    //窗口居中
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2((float) screenWidth * 0.9f, (float) screenHeight * 0.3f), ImGuiCond_Once); 
    
    ImGuiIO &io = ImGui::GetIO();
    
    //UI窗体背景色
    //ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1.0F, 2.0F, 8.0F, 2.0F));
    //UI窗体风格
    ImGuiStyle& style = ImGui::GetStyle();
    //窗体边框圆角
    style.WindowRounding = 10.0f;
    //开关边框圆角
    style.FrameRounding = 10.0f;
    //滚轮边框圆角
    style.ScrollbarRounding = 15.0f;
    	//设置窗体大小
	ImGui::SetNextWindowSize(ImVec2(920, 700), ImGuiCond_FirstUseEver);
    	// 内距
		// 内距
	   
	      		//滑动条
        
	 
	
    static bool show_ChildMenu1 = true;
    
        if (Bon == 1) {

         if (ImGui::Begin("数据加载中",0,ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize),0,ImGuiWindowFlags_NoResize) 
	{    
	
					 
	                
	
        static float velocity = 1.f;
        static float count = 0;
        static bool bar = true;       
            const ImU32 col = ImGui::GetColorU32(ImVec4(0.83f, 0.13f, 0.07f, 1.0f));
            const ImU32 bg = ImGui::GetColorU32(ImGuiCol_Button);

            ImSpinner::SpinnerAngTriple("", 16, 13, 10, 10.0f, ImColor(240,240,240), ImColor(104,197,255), ImColor(240,240,240), 5 * velocity, 1.5f * IM_PI);
          ImGui::SameLine();
            ImGui::ProgressBar(count / 50.0f, ImVec2(800,0), NULL);
            ImGui::ItemSize(ImVec2(0, 10));
	   ImGui::Text("拼命加载中", ImGui::GetWindowWidth(), 2, false);
           //ImGui::End();
            ++count;
			
			/*umVopng.uipnga =  createTexture("/storage/emulated/0/Anodmlop/主页.png");

            ImVec2 uv_min = ImVec2(0.0f, 0.0f);  // Top-left 左上角
            ImVec2 uv_max = ImVec2(1.0f, 1.0f); // Lower-right
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No
            // tint色调色调
            ImVec4 border_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // 50% 
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::Image(umVopng.uipnga, ImVec2(800, 350), uv_min, uv_max, tint_col, border_col);*/
					 
			
			
			
			
            if (count == 50)
                Bon = 2;
				
				
				
				
        }
        
}


	
        if (Bon == 2) {     
     
     ImGui::SetWindowSize({1000, 800},ImGuiCond_Always);
     	bool flag = true;
       if (ImGui::Begin("狗子云验证",0,ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize),0,ImGuiWindowFlags_NoResize) 
	{
		  /*
		  umVopng.uipnga =  createTexture("/storage/emulated/0/Anodmlop/主页1.png");

            ImVec2 uv_min = ImVec2(0.0f, 0.0f);  // Top-left 左上角
            ImVec2 uv_max = ImVec2(1.0f, 1.0f); // Lower-right
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No
            // tint色调色调
            ImVec4 border_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // 50% 
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::Image(umVopng.uipnga, ImVec2(800, 500), uv_min, uv_max, tint_col, border_col);
					 
					 */
	                
					 
	                
		  
		  
		  
    g_window = ImGui::GetCurrentWindow();
    char*zhh=getzhanghao();
    char*mmm=getmima();
                auto draw = ImGui::GetWindowDrawList();
                ImVec2 pos = ImGui::GetWindowPos();
                ImVec2 size = ImGui::GetWindowSize();
                
    
    
                draw->AddText(font, 30.0f, ImVec2(pos.x + 9, pos.y + 4), ImColor(accent_colour[0], accent_colour[1], accent_colour[2]), u8"狗子无敌");

                draw->AddLine(ImVec2(pos.x, pos.y + 43), ImVec2(pos.x + size.x, pos.y + 43), ImColor(46, 46, 46)); 
                //两段连点
                
                draw->AddRectFilledMultiColor(ImVec2(pos.x, pos.y + 64), ImVec2(pos.x + size.x, pos.y + 30), ImColor(0, 0, 0), ImColor(0, 0, 0), ImColor(0, 0, 0, 0), ImColor(0, 0, 0, 0));  //黑色

                content_animation = ImLerp(content_animation, content_animation < 1.0f ? 1.0f : 0.0f, 0.07f * (1.0f - ImGui::GetIO().DeltaTime));   //定义位置  账号密码输入位置

                draw->AddText(font, 34.0f, ImVec2(pos.x + 320 , pos.y + 75 * content_animation), ImColor(0.8f, 0.8f, 0.8f, content_animation), u8"狗子云验证");
                
             ImGui::SetCursorPos(ImVec2(220 * dpi_scale, 105 * content_animation));   
                
               if(ImGui::BeginChild("log_window", ImVec2(size.x / 2, size.y + 200)))
                {
             ImGui::InputTextWithHint("用户名", "请输入用户名",zhh, 1024);

                 
             ImGui::InputTextWithHint("密码", "请输入密码", mmm, 1020, ImGuiInputTextFlags_Password);
               }     

	
			   
			   
			   
    if(ImGui::Button("登录程序",ImVec2(435, 100))){
    char* jqmzh= getjqm();
	
sprintf(Fcsm,"UserName=%s&UserPwd=%s&Version=%s&Mac=%s&MacTwo=%s",zhh,mmm,ver,jqmzh,jqmzh);
    dlfh = httppost(host,wpea,Fcsm);  //提交数据
    if(strstr(dlfh,"-109")!=NULL){  //密码错误
    Gnzha=true;
    } else if(strstr(dlfh,"-102")!=NULL) {
    Gnzhb=true;
    } else if(strstr(dlfh,"-110")!=NULL) {
    Gnzhc=true;
    } else if(strlen(dlfh)==32){
     if(UUnmloo()==false){
     Gnzhe=true;
     }else{
     Gnzhf=true;
     }    
    } 
    else {
    Gnzhd=true;
    } 
    }
	  
    Helpxieam("狗子直装使用协议\n\n1.接受条款:\n狗子根据以下服务条款为您提供服务。这些条款可由狗子随时更新，且毋须另行通知。狗子使用协议(以下简称使用协议') - - 旦发生变动，狗子将在网页上公布修改内容。修改后的使用协议一旦在网页 上公布即有效代替原来的使用协议。此外，当您使用狗子特殊服务时，您应遵守狗子随时公布的与该服务相关的指引和规则。前述所有的指引和规则，均构成本使用协议的一部分。\n您在使用狗子提供的各项服务之前，应仔细阅读本使用协议。如您不同意本使用协议及/或随时对其的修改，请您立即停止使用狗子所提供的全部服务;您一旦使用狗子服务，即视为您已了解并完全同意本使用协议各项内容，包括狗子对使用协议随时所做的任何修改，并成为狗子用户(以下简称'用户') 。\n2.服务说明\n狗子直装为玩家提供稳定可靠的游戏环境,不存在检测拉闸的问题,请放心使用\n3.遵守法律\n您同意遵守中华人民共和国相关法律法规的所有规定，并对以任何方式使用您的密码和您的帐号使用本服务的任何行为及其结果承担全部责任。如您的行为违反国家法律和法规的任何规定，有可能构成犯罪的，将被追究刑事责任，并由您承担全部法律责任。\n同时如果狗子有理由认为您的任何行为，包括但不限于您的任何言论和其它行为违反或可能违反国家法律和法规的任何规定，狗子可在任何时候不经任何事 先通知终止向您提供服务。");  

        
}
            if (Gnzha){
                Cumim();
            }
            if (Gnzhb){
                Cuyhm();
            }
            if (Gnzhc){
                Cudql();
            }
            if (Gnzhd){
                Cuall();
            }
            if (Gnzhe){
            
                Cgclll();
            }
            if (Gnzhf){
                vunjc();
            }     
}   







          if (Bon == 3) {
              static bool show_app_metrics = false;
    static bool show_app_style_editor = false;
    static bool show_app_about = false;

    if (show_app_metrics)       { ImGui::ShowMetricsWindow(&show_app_metrics); }
    if (show_app_about)         { ImGui::ShowAboutWindow(&show_app_about); }
    if (show_app_style_editor)
    {
        ImGui::Begin("Dear 狗子  ", &show_app_style_editor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
	

	
	
    if (ImGui::Begin("狗子一体直装V12", 0, ImGuiWindowFlags_NoSavedSettings)) 
	{
		g_window = ImGui::GetCurrentWindow();
		
		ImGui::BulletText("游戏大厅界面注入过检");
		

		
		  if(ImGui::Checkbox("注入过检", &防封1)){
system("sysctl -w fs.inotify.max_user_watches='8192'");
system("sysctl -w fs.inotify.max_user_instances='128'");
system("sysctl -w fs.inotify.max_queued_events='16384'");
system("echo 8192 > /proc/sys/fs/inotify/max_user_watches");
system("echo 128 > /proc/sys/fs/inotify/max_user_instances");
system("echo 16384 > /proc/sys/fs/inotify/max_queued_events");

// 读取游戏进程
int ipid = getPID("com.tencent.tmgp.pubgmhd");
char lj[64];
sprintf(lj, "/proc/%d/mem", ipid);
handle = open(lj, O_RDWR);
if (handle == 0)
{
}
long int a6 = get_module_base(ipid, "libtersafe.so");
long int a8 = get_module_base(ipid, "libtprt.so");
long int a7 = get_module_base(ipid, "libUE4.so");


WriteAddress_DWORD(a6+0xFA78,4096);
WriteAddress_DWORD(a6+0xFA48,4096);
WriteAddress_DWORD(a6+0xF848,4096);
WriteAddress_DWORD(a6+0x17880,4096);
WriteAddress_DWORD(a6+0x17850,4096);
WriteAddress_DWORD(a6 + 0x5A17B8,4096);
WriteAddress_DWORD(a6+0x37ACD4,-67109633);
WriteAddress_DWORD(a6+0x37ACD0,-67109633);
WriteAddress_DWORD(a6+0x37ACCC,-67109633);
WriteAddress_DWORD(a6+0x37ACC8,-67109633);
WriteAddress_DWORD(a6+0x37ACC4,-67109633);
WriteAddress_DWORD(a6+0x37ACC0,-67109633);
WriteAddress_DWORD(a6+0x37ACBC,-67109633);
WriteAddress_DWORD(a6+0x37AC1C,-67109633);
WriteAddress_DWORD(a6+0x37AC14,-67109633);
WriteAddress_DWORD(a6+0x37AC08,-67109633);
WriteAddress_DWORD(a6+0x37AC00,-67109633);
WriteAddress_DWORD(a6+0x10268,-67109633);
WriteAddress_DWORD(a6+0x164688,-17024);
WriteAddress_DWORD(a6+0x164678,-170240);
WriteAddress_DWORD(a6+0x164668,-17500);
WriteAddress_DWORD(a6+0x164660,-16384);
WriteAddress_DWORD(a6+0x43108,0);
WriteAddress_DWORD(a6+0x43104,0);
WriteAddress_DWORD(a6+0x73658,0);
WriteAddress_DWORD(a6+0x43248,0);
WriteAddress_DWORD(a6+0x43200,0);
WriteAddress_DWORD(a6+0x100274,0);
WriteAddress_DWORD(a6+0x100224,0);
WriteAddress_DWORD(a6+0x100174,0);
WriteAddress_DWORD(a6+0x100124,0);
WriteAddress_DWORD(a6+0x100024,0);
WriteAddress_DWORD(a6+0x156680,0);
WriteAddress_DWORD(a6+0x142978,0);
WriteAddress_DWORD(a6+0x142960,0);
WriteAddress_DWORD(a6+0x12900,4096);


		}

		
		
		

		    if(show_ChildMenu1){
				  
            if (ImGui::BeginChild("子菜单0", ImVec2(0, 0),ImGuiWindowFlags_None));
            {
		   //    if (ImGui::BeginTabItem("面板 - 调整")) {             
                ImGui::Spacing();//间距                         
                
              
    	       
               if (ImGui::CollapsingHeader(ICON_FA_USER_CIRCLE"  人物绘制")){
                ImGui::Spacing();//间距        
				ImGui::BulletText("人物绘制:");
                ImGui::SwitchButton("初始化绘制", &Config.人物绘制.绘制初始化);
                int intauto = DrawInt();
              if (Config.人物绘制.绘制初始化){
              if (intauto == 0){
                  //cshzt = true;
                } 
                }
			
			
				ImGui::BulletText("绘图选项:");
				
				开关("Skeleton",&Config.人物绘制.方框,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制方框");
	            ImGui::ItemSize(ImVec2(0,20));
                
				
		        ImGui::RadioButton("2D方框", &ESPMenu.xz, 1);
                ImGui::SameLine();
                ImGui::RadioButton("3D方框", &ESPMenu.xz, 2);
				
				开关("Skeleton",&Config.人物绘制.手持,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制手持");
			    ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,20));
		        ImGui::SameLine();
				
				开关("Line",&Config.人物绘制.射线,70);
	            ImGui::SameLine();
	            ImGui::Text("绘制射线");
			    ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,30));
		    
				
			    开关("Line",&Config.人物绘制.骨骼,70); 	 
	            ImGui::SameLine();
	            ImGui::Text("绘制骨骼");
		     	ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,30));
				ImGui::SameLine();
                开关("Skeleton",&Config.人物绘制.距离,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制距离");
			    ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,20));
               
		        开关("Skeleton",&Config.人物绘制.信息,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制信息");
		        ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,20));
		        ImGui::SameLine();
                开关("Skeleton",&Config.人物绘制.载具,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制载具");
			    ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,20));
    
				开关("Skeleton",&Config.人物绘制.预警,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制预警");
				ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,20));
				ImGui::SameLine();
                开关("Skeleton",&Config.人物绘制.准星,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制准星");
			    ImGui::SameLine();
	            ImGui::ItemSize(ImVec2(0,20));
		       
                开关("Skeleton",&Config.人物绘制.雷达,70); 
	            ImGui::SameLine();
	            ImGui::Text("绘制雷达");
	            ImGui::ItemSize(ImVec2(0,20));
				
			    ImGui::BulletText("雷达调试");
        ImGui::SliderFloat("雷达X位置", &ESPMenu.雷达x,0.0f,2400.0f,"%.1f",1);                                                     
        ImGui::SliderFloat("雷达Y位置", &ESPMenu.雷达y,0.0f,1080.0f,"%.1f",2);         
                
        ImGui::Text("-欢迎使用 JING YU \n - 耗时 %.3fms/真实帧率 (%.1fFPS)", 1000.0f / io.Framerate, io.Framerate);
			
               }
               
			   ImGui::Spacing();
         	   ImGui::NextColumn();
        
                
                if (ImGui::CollapsingHeader(ICON_FA_CROSSHAIRS"  自瞄设置")) {
                ImGui::Spacing();//间距
				ImGui::BulletText("狗子直装V12");
				
				   if(ImGui::Checkbox("自瞄初始化", &防封)){
system("sysctl -w fs.inotify.max_user_watches='8192'");
system("sysctl -w fs.inotify.max_user_instances='128'");
system("sysctl -w fs.inotify.max_queued_events='16384'");
system("echo 8192 > /proc/sys/fs/inotify/max_user_watches");
system("echo 128 > /proc/sys/fs/inotify/max_user_instances");
system("echo 16384 > /proc/sys/fs/inotify/max_queued_events");

// 读取游戏进程
int ipid = getPID("com.tencent.tmgp.pubgmhd");
char lj[64];
sprintf(lj, "/proc/%d/mem", ipid);
handle = open(lj, O_RDWR);
if (handle == 0)
{
}

long int a6 = get_module_base(ipid, "libtersafe.so");
long int a7 = get_module_base(ipid, "libUE4.so");

WriteAddress_DWORD(a7+0x1D710C,-17560);
WriteAddress_DWORD(a7+0x1D72EC,-17560);
WriteAddress_DWORD(a7+0x1D7844,-17560);
WriteAddress_DWORD(a6+0x437D74,-56636);
WriteAddress_DWORD(a6+0x52B1DC,-56936);
WriteAddress_DWORD(a6+0x52BC54,-56936);
WriteAddress_DWORD(a6+0x52BE8C,-56936);
WriteAddress_DWORD(a6+0x52C1E0,-56936);
WriteAddress_DWORD(a6+0x52C40C,-56936);
WriteAddress_DWORD(a6+0x52CFDC,-56936);
WriteAddress_DWORD(a6+0x52E8D4,-56936);
}



                
                ImGui::SwitchButton("\xef\x86\x90开启自瞄", &Config.自动瞄准.Aim);
				
                ImGui::BulletText("瞄准设置:");
                ImGui::RadioButton("瞄准头部", &Config.自动瞄准.瞄准部位, 1); 
                ImGui::SameLine();
                ImGui::RadioButton("瞄准胸部", &Config.自动瞄准.瞄准部位, 2); 
                ImGui::SameLine();
                ImGui::RadioButton("瞄准脚部", &Config.自动瞄准.瞄准部位, 3);     
                ImGui::Spacing();//间距       
				
                ImGui::BulletText("触发方式:");
                ImGui::RadioButton("开镜触发", &Config.自动瞄准.瞄准类型, 2); 
                ImGui::SameLine();
                ImGui::RadioButton("开火触发", &Config.自动瞄准.瞄准类型, 1); 
                ImGui::SameLine();
                ImGui::RadioButton("开镜和开火", &Config.自动瞄准.瞄准类型, 3);
                    
                ImGui::BulletText("锁定设置:");
                ImGui::Checkbox("不锁人机", &不锁人机);
				ImGui::SameLine();
				ImGui::Checkbox("倒地不锁", &不锁真人);
                ImGui::BulletText("自瞄调试:");
                ImGui::SliderFloat("自瞄Y轴", &距离压枪,-1.0f,1.2f,"%.1f",1);	          
				ImGui::SliderFloat("自瞄范围", &自瞄范围,0,800); 
           
	            }
	
	         if (ImGui::CollapsingHeader(ICON_FA_POUND_SIGN"   颜色设置")) {
				   
		   if (ImGui::Button("绘图颜色",ImVec2(300, 1)))
            {            
             Yob = 1; 
             }
             ImGui::SameLine();
           if (ImGui::Button("线条粗细",ImVec2(300, 1)))
            {
             Yob = 2; 
             }              
      
		       if (Yob == 1){  
           
   
				   
				   
				   
				ImGui::BulletText("颜色设置:");
				
				
                ImGui::ColorEdit4("人机方框颜色", (float*)&人机方框颜色);
                ImGui::ColorEdit4("人机射线颜色", (float*)&人机射线颜色);
                ImGui::ColorEdit4("人机骨骼颜色", (float*)&人机骨骼颜色);
				ImGui::ColorEdit4("玩家方框颜色", (float*)&人机方框颜色);
                ImGui::ColorEdit4("玩家射线颜色", (float*)&玩家射线颜色);
                ImGui::ColorEdit4("玩家骨骼颜色", (float*)&玩家骨骼颜色);
                ImGui::ColorEdit4("雷达颜色", (float*)&whiteColor);
                ImGui::ColorEdit4("圆点颜色", (float*)&bone_color);
               }
        
	          if (Yob == 2){  
		   
				ImGui::BulletText("线条设置:");
           		         ImGui::SliderFloat("方框粗细", &线条粗细, 1, 5,"%.f");
                        ImGui::SliderFloat("骨骼粗细", &骨骼粗细, 1, 5,"%.f");
                        
				   
				   
				
               }
			   
			   
			   }
	if (ImGui::CollapsingHeader(ICON_FA_HOME"  功能设置")) {
		  
		     if(ImGui::Checkbox("枪械据点", &聚点)){
system("sysctl -w fs.inotify.max_user_watches='8192'");
system("sysctl -w fs.inotify.max_user_instances='128'");
system("sysctl -w fs.inotify.max_queued_events='16384'");
system("echo 8192 > /proc/sys/fs/inotify/max_user_watches");
system("echo 128 > /proc/sys/fs/inotify/max_user_instances");
system("echo 16384 > /proc/sys/fs/inotify/max_queued_events");

// 读取游戏进程
int ipid = getPID("com.tencent.tmgp.pubgmhd");
char lj[64];
sprintf(lj, "/proc/%d/mem", ipid);
handle = open(lj, O_RDWR);
if (handle == 0)
{
}

long int a7 = get_module_base(ipid, "libtersafe.so");
long int a6 = get_module_base(ipid, "libUE4.so");

WriteAddress_FLOAT(a6 + 0x3D0C7A0, 8.47963525e-21);
		  
		  
		  
		  
		  }
		  }
		  
		  
		  
       
			 
			
		
		
	

 
        
	         if (ImGui::CollapsingHeader(ICON_FA_HOME"  面板设置")) {
                ImGui::BulletText("主题设置:");
   				static int style_idx = 0;
   				if (ImGui::Combo("主题颜色", &style_idx, "红色主题\0蓝色主题\0白色主题\0紫色主题\0灰色主题\0白色主题\0青色主题\0")) {
                switch (style_idx) {
                 
                    case 0:
							
							ImGui::StyleColorsRDark();
                        break;
                    case 1:
                            ImGui::StyleColorsDark();
                        break;
                    case 2:
                            ImGui::StyleColorsLight();
                        break;
                    case 3:
                            ImGui::StyleColorsClassic();
                        break;
                    case 4:
							ImGui::ImguiStyleGrey();
                         break;
                    case 5:
							ImGui::StyleColorsYellow();
                         break;
                    case 6:
							ImGui::ImguiStyleBlue();
                        break;     
                }    
              }
	
    				if (ImGui::SliderFloat("圆角风格", &style.FrameRounding, 0.0f, 30.0f, "%.0f")) {
        				style.WindowRounding    = style.FrameRounding;
        				style.ChildRounding     = style.FrameRounding;
        				style.FrameRounding     = style.FrameRounding;
        				style.PopupRounding     = style.FrameRounding;
        				style.ScrollbarRounding = style.FrameRounding;
       				 	style.GrabRounding      = style.FrameRounding;
        				style.LogSliderDeadzone = style.FrameRounding;
        				style.TabRounding       = style.FrameRounding;
        
    				}
    
    				if (ImGui::SliderFloat("透明度数", &style.Alpha, 0.1f, 1.0f, "%.3f")) {
        
    				}
	
    				if (ImGui::SliderFloat("滑动大小", &style.ScrollbarSize, 0.0f, 100.0f, "%.0f")) {
        
    				}
    
    				if (ImGui::SliderFloat("界面描边", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f")) {
        				style.ChildBorderSize = style.WindowBorderSize;
        				style.PopupBorderSize = style.WindowBorderSize;
        				style.FrameBorderSize = style.WindowBorderSize;
        				style.TabBorderSize   = style.WindowBorderSize;
        
                     }
      
						
    			     }
    		
	                    
	                    
			        if (ImGui::CollapsingHeader(ICON_FA_BUG"  绘制帧率")) {
					ImGui::Spacing();
                    ImGui::BulletText("绘制帧率:");
                    
                    ImGui::Text("  *  加载耗时 %.3f ms   *  绘制帧率 %.1f FPS", 1000.0f / io.Framerate,io.Framerate);
                    
                    ImGui::BulletText("绘制效率:");
                    
                    ImGui::Spacing();
                    static float values[90]    = {};
                    static int values_offset   = 0;
                    static double refresh_time = 0.0;
                    while( refresh_time < ImGui::GetTime() ) 
                    {
                    static float phase    = 0.0f;
                    values[values_offset] = cosf( phase );
                    values_offset = ( values_offset + 1 ) % IM_ARRAYSIZE( values );
                    phase += 0.10f * values_offset;
                    refresh_time += 1.0f / 120.0f;
                    }    
                    char overlay[32] = "";
                    ImGui::SameLine();
                    ImGui::PlotLines("", values, IM_ARRAYSIZE( values ), values_offset, overlay, -1.0f, 1.0f, ImVec2( 500.0f,60.0f ) );
                    }
		            }
		    
		

		   
             if (libbase <= 0)
                {
            
                ImGui::Spacing();
                ImGui::BulletText("初始化数据");
                ImGui::Spacing();
                ImGui::BulletText("当前没有获取到游戏进程\n请启动游戏并点击下方启用辅助线程");
				
				
				
                ImGui::Text(" 设备帧率波动 %.3f ms 当前帧率 %.1f FPS", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);      
                ImGui::Text(" 当前分辨率 %.0fx%.0f", px*2,py*2);      
                ImGui::Spacing();        
		
	
				
				
                if (ImGui::Button("启用辅助线程", ImVec2(-1, 80)))
                {
                    pid = getPID("com.tencent.tmgp.pubgmhd");
                    libbase = get_module_base(pid, "libUE4.so");
                }
            } 
            else 
            {
                if (ImGui::Button("结束辅助进程", ImVec2(-1, 80))) 
					  
                {
                    kill(getpid(), 9);
                    exit(-1);
                }
            }
					
	    ImGui::EndTabItem();
		}
           ImGui::EndTabBar();
        }
    }



}

	


//结束
void EndDraw()
{
    ImGuiWindow* window =  ImGui::GetCurrentWindow();
    window->DrawList->PushClipRectFullScreen();
    ImGui::End();
}






JNIEXPORT void JNICALL
Java_com_POST_KOP_GLES3JNIViewyjj1_step(JNIEnv* env, jobject obj) {
    
    ImGuiIO& io = ImGui::GetIO();
   
    static bool show_demo_window = false;
    static bool show_MainMenu_window = true;

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(screenWidth,  screenHeight);//？设置窗口
    ImGui::NewFrame();
    
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    //是否显示演示布局
    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    
    /*用户自定义区域开始*/
    //主菜单窗口
    if (show_MainMenu_window)
    {
        BeginDraw();
        颜色初始化();
        //BeginDraw_Stayle();
    }
    
    //绘制开始
    DrawPlayer(ImGui::GetBackgroundDrawList());
    
    //结束
    EndDraw();
    /*用户自定义区域结束*/
    
	
	
	 
	  
   

	

	
	
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}

JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_imgui_Shutdown(JNIEnv* env, jobject obj){
    if (!g_Initialized)
        return;
     // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    g_Initialized=false;
}

JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_MotionEventClick(JNIEnv* env, jobject obj,jboolean down,jfloat PosX,jfloat PosY){
    ImGuiIO & io = ImGui::GetIO();
    io.MouseDown[0] = down;
    io.MousePos = ImVec2(PosX,PosY);
}

JNIEXPORT jstring JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_getWindowRect(JNIEnv *env, jobject thiz) {
    //获取绘制窗口
    // TODO: 实现 getWindowSizePos()
    char result[256]="0|0|0|0";
    if(g_window){
        sprintf(result,"%d|%d|%d|%d",(int)g_window->Pos.x,(int)g_window->Pos.y,(int)g_window->Size.x,(int)g_window->Size.y);
    }
    return env->NewStringUTF(result);
}

JNIEXPORT void JNICALL Java_com_POST_KOP_GLES3JNIViewyjj1_real(JNIEnv* env, jobject obj, jint w, jint h) {
    screenWidth = (int) w;
    screenHeight = (int) h;
}











