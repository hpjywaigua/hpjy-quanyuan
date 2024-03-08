//
//  ImguiTools.h
//  Dolphins
//
//  Created by 明笙 on 2022/4/24.
//

#ifndef ImguiTools_h
#define ImguiTools_h


#define IM_FLOOR(_VAL)                  ((float)(int)(_VAL))

#include <GLES3/gl3.h>


#include "imgui.h"
#include "imgui_internal.h"

struct ImageTexture {
    GLuint id;
    int width;
    int height;
    bool exist;
};

struct InputState {
    int id;
    bool isInput = false;
    bool isFocus = false;
    char *inputText;
    size_t inputTextLength;
};



static inline ImVec2 operator*(const ImVec2 &lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }

static inline ImVec2 operator/(const ImVec2 &lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }

static inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }

static inline ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

static inline ImVec2 operator*(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }

static inline ImVec2 operator/(const ImVec2 &lhs, const ImVec2 &rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }


static inline ImVec2 &operator*=(ImVec2 &lhs, const float rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

static inline ImVec2 &operator/=(ImVec2 &lhs, const float rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}

static inline ImVec2 &operator+=(ImVec2 &lhs, const ImVec2 &rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

static inline ImVec2 &operator-=(ImVec2 &lhs, const ImVec2 &rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

static inline ImVec2 &operator*=(ImVec2 &lhs, const ImVec2 &rhs) {
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    return lhs;
}

static inline ImVec2 &operator/=(ImVec2 &lhs, const ImVec2 &rhs) {
    lhs.x /= rhs.x;
    lhs.y /= rhs.y;
    return lhs;
}

static inline ImVec4 operator+(const ImVec4 &lhs, const ImVec4 &rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }

static inline ImVec4 operator-(const ImVec4 &lhs, const ImVec4 &rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }

static inline ImVec4 operator*(const ImVec4 &lhs, const ImVec4 &rhs) { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }


namespace ImGui {
    //提示文本
    void helpMarker(const char *desc);

    //设置Imgui样式
    void setCustomTheme();

    InputState *getInputState( );

    IMGUI_API bool CustomInputText(const char *label, char *buf, size_t buf_size,const ImVec2 &size_arg = ImVec2(0, 0),bool is_password = false);

    //获取绘制文字宽度
    float calcTextSizeSingle(const char *text,const char *text_end, float font_size = 0);

    //TextureInfo loadTextureFromMemory(const unsigned char *fileData, size_t fileDataSize);

    IMGUI_API bool ImageTextButton(const char *label, ImTextureID user_texture_id, const ImVec2 &button_size = ImVec2(0, 0), const ImVec2 &uv0 = ImVec2(0, 0), const ImVec2 &uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4 &bg_col = ImVec4(0, 0, 0, 0), const ImVec4 &tint_col = ImVec4(1, 1, 1, 1));

    IMGUI_API bool ImageTextButtonEx(const char *label, ImGuiID id, ImTextureID texture_id, const ImVec2 &button_size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec2 &padding, const ImVec4 &bg_col, const ImVec4 &tint_col);

    IMGUI_API void BulletColorText(ImVec4 col, const char *fmt, ...);

    IMGUI_API void BulletColorTextV(const char *fmt, ImVec4 col, va_list args) IM_FMTLIST(1);

    IMGUI_API bool Switchbox(const char* label, bool* v,float width);

    IMGUI_API bool Radiobox(const char* label, bool active);

    IMGUI_API bool Radiobox(const char* label, int* v, int v_button);

    IMGUI_API bool MyCheckbox(const char* label, bool* v);

    IMGUI_API bool MySlider(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

    bool MySliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);

    IMGUI_API bool MyFloatSlider(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);

    IMGUI_API bool MyIntSlider(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);

    bool MyFloatSliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);

    bool SliderBehavior(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb,float grab_sz);

    void RenderTextClippedEx(ImDrawList* draw_list,ImU32 col, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_display_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect);

    void RenderColorTextClipped(ImU32 col,const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);

    template<typename T, typename SIGNED_T, typename FLOAT_T> bool  SliderBehaviorT(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, T* v, T v_min, T v_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb,float grab_sz);
}

namespace CustomImDrawList {

    IMGUI_API void drawLine(float x1, float y1, float x2, float y2, ImVec4 color, float size);

    IMGUI_API void drawText(int x, int y, ImVec4 color, const char *str,ImFont* font = NULL,bool isCenter = false);

    IMGUI_API void drawText(const char* str, int x, int y, ImVec4 color,ImFont* font = NULL,bool isCenter = false);

    IMGUI_API void drawImage(int x, int y, int w, int h, ImTextureID Texture);

    IMGUI_API void drawRect(int x, int y, int w, int h, ImVec4 color,int size);

    IMGUI_API void drawRectFilled(int x, int y, int w, int h,ImVec4 color);
    
    IMGUI_API void drawCircleArc(const ImVec2& center, float radius, const ImVec2& radian, ImU32 col, int num_segments, float thickness);
    
    IMGUI_API void drawCircle(int x, int y, float radius, ImVec4 color, int segments, int thickness);

    IMGUI_API void drawCircleFilled(int x, int y, int radius, ImVec4 color, int segments);

    IMGUI_API void AddCustomCircle(const ImVec2 &pos,ImU32 cir_bgcol,ImU32 cir_col,float radius,float thickness,float max_progress,float progress,float font_size,ImU32 font_col,const char *text_begin, const char *text_end);
};


#endif /* ImguiTools_h */
