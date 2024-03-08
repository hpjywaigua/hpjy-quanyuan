//
// Created by Administrator on 2022/3/15.
//

#include "imgui.h"
#include "imgui_internal.h"


// Those light colors are better suited with a thicker font than the default one + FrameBorder
void ImGui::StyleColorsRDark(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImColor(255, 255, 255, 255);
    colors[ImGuiCol_TextDisabled]           = ImColor(128, 128, 141, 255);
    colors[ImGuiCol_WindowBg]               = ImColor(0, 0, 0, 240);
    colors[ImGuiCol_ChildBg]                = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_PopupBg]                = ImColor(0, 0, 0, 240);
    colors[ImGuiCol_Border]                 = ImColor(0, 0, 0, 128);
    colors[ImGuiCol_BorderShadow]           = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_FrameBg]                = ImColor(89, 36, 36, 138);
    colors[ImGuiCol_FrameBgHovered]         = ImColor(255, 124, 0, 255);
    colors[ImGuiCol_FrameBgActive]          = ImColor(255, 0, 0, 171);
    colors[ImGuiCol_TitleBg]                = ImColor(10, 10, 0, 255);
    colors[ImGuiCol_TitleBgActive]          = ImColor(219, 29, 29, 255);
    colors[ImGuiCol_TitleBgCollapsed]       = ImColor(232, 20, 20, 130);
    colors[ImGuiCol_MenuBarBg]              = ImColor(36, 36, 0, 255);
    colors[ImGuiCol_ScrollbarBg]            = ImColor(0, 0, 0, 135);
    colors[ImGuiCol_ScrollbarGrab]          = ImColor(255, 124, 0, 255);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImColor(255, 0, 0, 255);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImColor(255, 0, 0, 255);
    colors[ImGuiCol_CheckMark]              = ImColor(255, 0, 0, 255);
    colors[ImGuiCol_SliderGrab]             = ImColor(255, 124, 0, 255);
    colors[ImGuiCol_SliderGrabActive]       = ImColor(255, 0, 0, 255);
    colors[ImGuiCol_Button]                 = ImColor(232, 20, 20, 255);
    colors[ImGuiCol_ButtonHovered]          = ImColor(249, 13, 13, 106);
    colors[ImGuiCol_ButtonActive]           = ImColor(232, 20, 20, 255);
    colors[ImGuiCol_Header]                 = ImColor(66, 150, 123, 79);
    colors[ImGuiCol_HeaderHovered]          = ImColor(66, 150, 255, 204);
    colors[ImGuiCol_HeaderActive]           = ImColor(66, 150, 250, 255);
    colors[ImGuiCol_Separator]              = ImColor(110, 110, 208, 128);
    colors[ImGuiCol_SeparatorHovered]       = ImColor(26, 102, 191, 199);
    colors[ImGuiCol_SeparatorActive]        = ImColor(26, 102, 191, 255);
    colors[ImGuiCol_ResizeGrip]             = ImColor(255, 0, 0, 51);
    colors[ImGuiCol_ResizeGripHovered]      = ImColor(255, 0, 0, 171);
    colors[ImGuiCol_ResizeGripActive]       = ImColor(255, 0, 0, 242);
    colors[ImGuiCol_Tab]                    = ImColor(255, 0, 0, 50);
    colors[ImGuiCol_TabHovered]             = ImColor(255, 0, 0, 204);
    colors[ImGuiCol_TabActive]              = ImColor(255, 0, 0, 255);
    colors[ImGuiCol_TabUnfocused]           = ImColor(17, 26, 38, 248);
    colors[ImGuiCol_TabUnfocusedActive]     = ImColor(35, 67, 108, 255);
    colors[ImGuiCol_PlotLines]              = ImColor(156, 156, 156, 255);
    colors[ImGuiCol_PlotLinesHovered]       = ImColor(255, 110, 94, 255);
    colors[ImGuiCol_PlotHistogram]          = ImColor(230, 179, 0, 255);
    colors[ImGuiCol_PlotHistogramHovered]   = ImColor(255, 153, 0, 255);
    colors[ImGuiCol_TableHeaderBg]          = ImColor(48, 48, 13, 255);
    colors[ImGuiCol_TableBorderStrong]      = ImColor(79, 79, 54, 255);
    colors[ImGuiCol_TableBorderLight]       = ImColor(59, 59, 64, 255);
    colors[ImGuiCol_TableRowBg]             = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_TableRowBgAlt]          = ImColor(255, 255, 255, 15);
    colors[ImGuiCol_TextSelectedBg]         = ImColor(66, 150, 109, 89);
    colors[ImGuiCol_DragDropTarget]         = ImColor(255, 255, 0, 230);
    colors[ImGuiCol_NavHighlight]           = ImColor(66, 150, 250, 255);
    colors[ImGuiCol_NavWindowingHighlight]  = ImColor(255, 255, 47, 179);
    colors[ImGuiCol_NavWindowingDimBg]      = ImColor(204, 204, 204, 51);
    colors[ImGuiCol_ModalWindowDimBg]       = ImColor(204, 204, 204, 89);
}