#include "stb_image.h"
#include <android/input.h>
#include "imgui_expand.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-pointer-cast"
static const ImS32 IM_S32_MIN = INT_MIN;    // (-2147483647 - 1), (0x80000000);
static const ImS32 IM_S32_MAX = INT_MAX;    // (2147483647), (0x7FFFFFFF)
static const ImU32 IM_U32_MIN = 0;
static const ImU32 IM_U32_MAX = UINT_MAX;   // (0xFFFFFFFF)
#ifdef LLONG_MIN
static const ImS64 IM_S64_MIN = LLONG_MIN;  // (-9223372036854775807ll - 1ll);
static const ImS64 IM_S64_MAX = LLONG_MAX;  // (9223372036854775807ll);
#else
static const ImS64          IM_S64_MIN = -9223372036854775807LL - 1;
static const ImS64          IM_S64_MAX = 9223372036854775807LL;
#endif
static const ImU64 IM_U64_MIN = 0;
#ifdef ULLONG_MAX
static const ImU64 IM_U64_MAX = ULLONG_MAX; // (0xFFFFFFFFFFFFFFFFull);
#else
static const ImU64          IM_U64_MAX = (2ULL * 9223372036854775807LL + 1);
#endif

static const char *PatchFormatStringFloatToInt1(const char *fmt) {
    if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) { // Fast legacy path for "%.0f" which is expected to be the most common case.
        return "%d";
    }
    const char *fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
    const char *fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
    if (fmt_end > fmt_start && fmt_end[-1] == 'f') {
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        if (fmt_start == fmt && fmt_end[0] == 0) {
            return "%d";
        }
        ImGuiContext &g = *GImGui;
        ImFormatString(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), "%.*s%%d%s", (int) (fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
        return g.TempBuffer;
#else
        IM_ASSERT(0 && "DragInt(): Invalid format string!"); // Old versions used a default parameter of "%.0f", please replace with e.g. "%d"
#endif
    }
    return fmt;
}

static ImVec2 rotateCoord(float angle, ImVec2 coord) {
    float s = sin(angle * M_PI / 180);
    float c = cos(angle * M_PI / 180);

    return ImVec2(coord.x * c + coord.y * s, -coord.x * s + coord.y * c);
}

namespace ImGui {
    InputState inputState;
}

InputState *ImGui::getInputState( ) {
    return &inputState;
};

bool ImGui::CustomInputText(const char *label, char *buf, size_t buf_size, const ImVec2 &size_arg, bool is_password) {
    char *newLabel = (char *) malloc(strlen(label) + buf_size + 1);
    memset(newLabel, 0, strlen(label) + buf_size + 1);
    if (is_password) {
        char *mask = (char *) malloc(buf_size);
        memset(mask, 0, buf_size);
        for (int i = 0 ; i < strlen(buf) ; i++) {
            mask[i] = '*';
        }
        sprintf(newLabel, "%s: %s", label, mask);
        free(mask);
    } else {
        sprintf(newLabel, "%s: %s", label, buf);
    }

    PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
    PushStyleColor(ImGuiCol_ButtonActive, ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive));
    PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
    bool state = Button(newLabel, size_arg);
    PopStyleColor(3);
    PopStyleVar(1);
    if (state) {
        inputState.inputText = buf;
        inputState.inputTextLength = buf_size;
        inputState.id = GetItemID( );
        inputState.isFocus = true;
    }
    free(newLabel);
    return state;
}

void ImGui::helpMarker(const char *desc) {
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "(?)");
    if (ImGui::IsItemHovered( )) {
        ImGui::BeginTooltip( );
        ImGui::PushTextWrapPos(ImGui::GetFontSize( ) * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos( );
        ImGui::EndTooltip( );
    }
}

float ImGui::calcTextSizeSingle(const char *text,const char *text_end, float font_size) {
    ImGuiContext &g = *GImGui;

    ImFont *font = g.Font;

    ImVec2 text_size;
    if (font_size == 0) {
        text_size = font->CalcTextSizeA(font->FontSize, FLT_MAX, -1.0f, text, text_end, NULL);
    } else {
        text_size = font->CalcTextSizeA(font_size, FLT_MAX, -1.0f, text, text_end, NULL);
    }

    text_size.x = IM_FLOOR(text_size.x + 0.99999f);

    return text_size.x;
}

void ImGui::setCustomTheme() {
    ImGuiStyle *style = &ImGui::GetStyle( );

    style->WindowRounding = 8.0f;//窗口圆角
    style->WindowBorderSize = 1.0f;//窗口边框
    style->FramePadding = ImVec2(16.0f, 16.0f);//组件内边距
    style->WindowPadding = ImVec2(16.0f, 16.0f);//窗口内边距

    style->ScrollbarRounding = 8.0f;//滚动条大小
    style->FrameRounding = 8.0f;
    style->ItemSpacing = ImVec2(16.0f, 16.0f);
    style->ItemInnerSpacing = ImVec2(16.0f, 16.0f);
    style->GrabMinSize = 56.0f;
    style->GrabRounding = 8.0f;


    ImGui::StyleColorsClassic();

    ImVec4 *colors = style->Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.64f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.27f, 0.27f, 0.54f, 0.80f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.63f, 0.80f);
}




bool ImGui::ImageTextButton(const char *label, ImTextureID user_texture_id, const ImVec2 &button_size, const ImVec2 &uv0, const ImVec2 &uv1, int frame_padding, const ImVec4 &bg_col, const ImVec4 &tint_col) {
    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = g.CurrentWindow;
    if (window->SkipItems) {
        return false;
    }

    // Default to using texture ID as ID. User can still push string/integer prefixes.
    PushID((void *) (intptr_t) user_texture_id);
    const ImGuiID id = window->GetID(label);
    PopID( );

    const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float) frame_padding, (float) frame_padding) : g.Style.FramePadding;
    return ImageTextButtonEx(label, id, user_texture_id, button_size, uv0, uv1, padding, bg_col, tint_col);
}

bool ImGui::ImageTextButtonEx(const char *label, ImGuiID id, ImTextureID texture_id, const ImVec2 &button_size, const ImVec2 &uv0, const ImVec2 &uv1, const ImVec2 &padding, const ImVec4 &bg_col, const ImVec4 &tint_col) {
    ImGuiContext &g = *GImGui;
    ImGuiWindow *window = GetCurrentWindow( );
    if (window->SkipItems) {
        return false;
    }

    const ImGuiStyle &style = g.Style;
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    label_size.x += g.FontSize + style.FramePadding.x;

    float w = label_size.x + padding.x * 2.0f;
    float h = label_size.y + padding.y * 2.0f;

    ImRect bb(window->DC.CursorPos, window->DC.CursorPos + label_size + padding * 2.0f);
    if (button_size.x != 0.0f) {
        bb.Max.x = window->DC.CursorPos.x + button_size.x;
        w = button_size.x;
    }
    if (button_size.y != 0.0f) {
        bb.Max.y = window->DC.CursorPos.y + button_size.y;
        h = button_size.y;
    }

    ItemSize(bb);
    if (!ItemAdd(bb, id)) {
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render
    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float) ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
    if (bg_col.w > 0.0f) {
        window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, GetColorU32(bg_col));
    }

    ImVec2 font_size = CalcTextSize(label, NULL, true);


    ImVec2 image_size = ImVec2(font_size.y, font_size.y);
    ImVec2 image_coord = ImVec2((bb.Min.x + w * style.ButtonTextAlign.x) - font_size.x / 2 - padding.x,bb.Min.y + h / 2.0f - image_size.y * style.ButtonTextAlign.y);

    window->DrawList->AddImage(texture_id, image_coord, image_coord + image_size, uv0, uv1, GetColorU32(tint_col));

    if (g.LogEnabled) {
        LogSetNextTextDecoration("[", "]");
    }
    ImVec2 text_size = ImVec2((bb.Min.x + w * style.ButtonTextAlign.x) - font_size.x / 2 - padding.x / 2 + image_size.x / 2, bb.Min.y);
    RenderTextClipped(text_size + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, ImVec2(0.0f,style.ButtonTextAlign.y), &bb);

    return pressed;
}


void ImGui::BulletColorText(ImVec4 col, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    BulletColorTextV(fmt, col, args);
    va_end(args);
}

// Text with a little bullet aligned to the typical tree node.
void ImGui::BulletColorTextV(const char *fmt, ImVec4 col, va_list args) {
    ImGuiWindow *window = GetCurrentWindow( );
    if (window->SkipItems) {
        return;
    }

    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;

    const char *text_begin = g.TempBuffer;
    const char *text_end = text_begin + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
    const ImVec2 label_size = CalcTextSize(text_begin, text_end, false);
    const ImVec2 total_size = ImVec2(g.FontSize + (label_size.x > 0.0f ? (label_size.x + style.FramePadding.x * 2) : 0.0f), label_size.y);  // Empty text doesn't add padding
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(total_size, 0.0f);
    const ImRect bb(pos, pos + total_size);
    if (!ItemAdd(bb, 0)) {
        return;
    }

    // Render
    ImU32 text_col = GetColorU32(col);
    RenderBullet(window->DrawList, bb.Min + ImVec2(style.FramePadding.x + g.FontSize * 0.5f, g.FontSize * 0.5f), text_col);
    PushStyleColor(ImGuiCol_Text, col);
    RenderText(bb.Min + ImVec2(g.FontSize + style.FramePadding.x * 2, 0.0f), text_begin, text_end, false);
    PopStyleColor( );
}

bool ImGui::Switchbox(const char *label, bool *v, float width) {
    ImGuiWindow *window = GetCurrentWindow( );
    if (window->SkipItems) {
        return false;
    }

    ImGuiContext &g = *GImGui;
    const ImGuiStyle &style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(width > 0.0f ? width : 0.0f, label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id)) {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed) {
        *v = !(*v);
        MarkItemEdited(id);
    }

    const float switch_size = GetFrameHeight( ) * 0.7f;//label_size.y * 1.3f;

    ImVec2 switch_bb = ImVec2(pos.x + width - switch_size * 2.0f, pos.y + GetFrameHeight( ) / 2 - switch_size / 2);


    const ImRect check_bb(switch_bb, switch_bb + ImVec2(switch_size * 2, switch_size));
    RenderNavHighlight(total_bb, id);

    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32(*v ? ImGuiCol_CheckMark : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, switch_size / 2);

    if (*v) {

        const ImRect check_bb1(switch_bb + ImVec2(switch_size, 0), switch_bb + ImVec2(switch_size * 2, switch_size));
        ImVec2 pad(ImMax(1.0f, IM_FLOOR(switch_size / 4.2f)), ImMax(1.0f, IM_FLOOR(switch_size / 4.2f)));
        window->DrawList->AddRectFilled(check_bb1.Min + pad, check_bb1.Max - pad, GetColorU32(ImGuiCol_FrameBg), switch_size / 4.2f);
    } else {
        /*RenderFrame(check_bb.Min, check_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, switch_size / 2);
*/
        /*ImU32 check_col = ImColor(255,255,255,255);*/
        const ImRect check_bb1(switch_bb, switch_bb + ImVec2(switch_size, switch_size));
        ImVec2 pad(ImMax(1.0f, IM_FLOOR(switch_size / 4.2f)), ImMax(1.0f, IM_FLOOR(switch_size / 4.2f)));
        window->DrawList->AddRectFilled(check_bb1.Min + pad, check_bb1.Max - pad, GetColorU32(ImGuiCol_CheckMark), switch_size / 4.2f);
    }

    ImVec2 label_pos = ImVec2(pos.x, pos.y + style.FramePadding.y);
    if (g.LogEnabled) {
        LogRenderedText(&label_pos, *v ? "[x]" : "[ ]");
    }
    if (width > 0.0f) {
        RenderText(label_pos, label);
    }

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

bool ImGui::Radiobox(const char* label, bool active)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, nullptr, true);

    const float square_sz = GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
        return false;

    ImVec2 center = check_bb.GetCenter();
    center.x = IM_ROUND(center.x);
    center.y = IM_ROUND(center.y);
    const float radius = (square_sz * 0.9f - 1.0f) * 0.5f;

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
        MarkItemEdited(id);

    RenderNavHighlight(total_bb, id);
    window->DrawList->AddCircleFilled(center, radius, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg));
    if (active)
    {
        RenderNavHighlight(total_bb, id);
        window->DrawList->AddCircleFilled(center, radius, GetColorU32(ImGuiCol_CheckMark));

        const float pad1 = ImMax(1.0f, IM_FLOOR(square_sz / 4.0f));
        RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad1, pad1), GetColorU32(ImGuiCol_FrameBg), square_sz - pad1 * 2.0f);
    }

    if (style.FrameBorderSize > 0.0f)
    {
        window->DrawList->AddCircle(center + ImVec2(1, 1), radius, GetColorU32(ImGuiCol_BorderShadow), 0, style.FrameBorderSize);
        window->DrawList->AddCircle(center, radius, GetColorU32(ImGuiCol_Border), 0, style.FrameBorderSize);
    }

    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
    if (g.LogEnabled)
        LogRenderedText(&label_pos, active ? "(x)" : "( )");
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

// FIXME: This would work nicely if it was a public template, e.g. 'template<T> RadioButton(const char* label, T* v, T v_button)', but I'm not sure how we would expose it..
bool ImGui::Radiobox(const char* label, int* v, int v_button)
{
    const bool pressed = Radiobox(label, *v == v_button);
    if (pressed)
        *v = v_button;
    return pressed;
}

bool ImGui::MyCheckbox(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight( ) * 0.7f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    ImVec2 switch_bb = ImVec2(pos.x, pos.y + GetFrameHeight( ) / 2 - square_sz / 2);

    const ImRect check_bb(switch_bb, switch_bb + ImVec2(square_sz, square_sz));
    RenderNavHighlight(total_bb, id);
    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
    ImU32 check_col = GetColorU32(ImGuiCol_CheckMark);
    bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
    if (mixed_value)
    {
        // Undocumented tristate/mixed/indeterminate checkbox (#2644)
        // This may seem awkwardly designed because the aim is to make ImGuiItemFlags_MixedValue supported by all widgets (not just checkbox)
        ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
        window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
    }
    else if (*v)
    {
        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
        RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
    }
    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, pos.y + style.FramePadding.y);
    if (g.LogEnabled)
        LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
    if (label_size.x > 0.0f)
        RenderText(label_pos, label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}




void ImGui::RenderTextClippedEx(ImDrawList* draw_list,ImU32 col, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_display_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
    // Perform CPU side clipping for single clipped element to avoid using scissor state
    ImVec2 pos = pos_min;
    const ImVec2 text_size = text_size_if_known ? *text_size_if_known : CalcTextSize(text, text_display_end, false, 0.0f);

    const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
    const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
    bool need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
    if (clip_rect) // If we had no explicit clipping rectangle then pos==clip_min
        need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

    // Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
    if (align.x > 0.0f) pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
    if (align.y > 0.0f) pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

    // Render
    if (need_clipping)
    {
        ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
        draw_list->AddText(NULL, 0.0f, pos, col, text, text_display_end, 0.0f, &fine_clip_rect);
    }
    else
    {
        draw_list->AddText(NULL, 0.0f, pos, col, text, text_display_end, 0.0f, NULL);
    }
}

void ImGui::RenderColorTextClipped(ImU32 col,const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
    // Hide anything after a '##' string
    const char* text_display_end = FindRenderedTextEnd(text, text_end);
    const int text_len = (int)(text_display_end - text);
    if (text_len == 0)
        return;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    RenderTextClippedEx(window->DrawList, col,pos_min, pos_max, text, text_display_end, text_size_if_known, align, clip_rect);
    if (g.LogEnabled)
        LogRenderedText(&pos_min, text, text_display_end);
}

void CustomImDrawList::drawLine(float x1, float y1, float x2, float y2, ImVec4 color, float size) {
    ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2),ImGui::ColorConvertFloat4ToU32(color), size);
}

void CustomImDrawList::drawText(int x, int y, ImVec4 color, const char *str,ImFont* font,bool isCenter) {
    if (font != nullptr) ImGui::PushFont(font);
    if (isCenter) {
        float textSize = ImGui::calcTextSizeSingle(str, nullptr);
        x -= textSize / 2.f;
    }
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 0.1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), str);
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
    if (font != nullptr) ImGui::PopFont();
}

void CustomImDrawList::drawText(const char* str, int x, int y, ImVec4 color,ImFont* font,bool isCenter) {
    if (font != nullptr) ImGui::PushFont(font);
    if (isCenter) {
        float textSize = ImGui::calcTextSizeSingle(str, nullptr);
        x -= textSize / 2.f;
    }
    ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), str);
    if (font != nullptr) ImGui::PopFont();
}


void CustomImDrawList::drawImage(int x, int y, int w, int h, ImTextureID Texture) {
    ImGui::GetOverlayDrawList()->AddImage(Texture, ImVec2(x, y), ImVec2(x + w, y + h));
}

void CustomImDrawList::drawRect(int x, int y, int w, int h, ImVec4 color,int size) {
    ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(w, h),ImGui::ColorConvertFloat4ToU32(color), 0, 0, size);
}

void CustomImDrawList::drawRectFilled(int x, int y, int w, int h,ImVec4 color) {
    ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(w, h),ImGui::ColorConvertFloat4ToU32(color), 0, 0);
}

void CustomImDrawList::drawCircle(int x, int y, float radius, ImVec4 color, int segments, int thickness) {
    ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius,ImGui::ColorConvertFloat4ToU32(color), segments,thickness);
}

void CustomImDrawList::drawCircleFilled(int x, int y, int radius, ImVec4 color, int segments) {
    ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius,ImGui::ColorConvertFloat4ToU32(color), segments);
}

void CustomImDrawList::drawCircleArc(const ImVec2& center, float radius, const ImVec2& radian, ImU32 col, int num_segments, float thickness){
   // ImGui::GetOverlayDrawList()->AddCircle(center, radius,radian, col,num_segments,thickness);
}


void CustomImDrawList::AddCustomCircle(const ImVec2 &pos, ImU32 cir_bgcol,ImU32 cir_col, float radius, float thickness, float max_progress, float progress, float font_size, ImU32 font_col, const char *text_begin, const char *text_end) {
    ImGui::GetOverlayDrawList()->AddCircle(pos, radius, cir_bgcol, 0,thickness);
    ImGui::GetOverlayDrawList()->PathArcTo({pos.x, pos.y}, radius, 0, (IM_PI * 2.0f) * (progress / max_progress));
    ImGui::GetOverlayDrawList()->PathStroke(cir_col, 2, thickness);
    ImGui::GetOverlayDrawList()->PathClear();

    ImGui::GetOverlayDrawList()->AddText(nullptr, font_size, ImVec2(pos.x - ImGui::calcTextSizeSingle(text_begin, text_end, font_size) / 2,pos.y - font_size / 2), font_col, text_begin, text_end);
}
#pragma clang diagnostic pop
