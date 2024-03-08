
#include "imgui.h"
#include "imgui_internal.h"


// Those light colors are better suited with a thicker font than the default one + FrameBorder
void ImGui::ImguiStyleGrey(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImColor(0, 0, 0, 255);
    colors[ImGuiCol_TextDisabled]           = ImColor(153, 153, 153, 255);
    colors[ImGuiCol_WindowBg]               = ImColor(183, 183, 183, 255);
    colors[ImGuiCol_ChildBg]                = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_PopupBg]                = ImColor(255, 255, 255, 250);
    colors[ImGuiCol_Border]                 = ImColor(0, 0, 0, 77);
    colors[ImGuiCol_BorderShadow]           = ImColor(0, 0, 0, 0);
    colors[ImGuiCol_FrameBg]                = ImColor(152, 152, 152, 255);
    colors[ImGuiCol_FrameBgHovered]         = ImColor(158, 158, 158, 102);
    colors[ImGuiCol_FrameBgActive]          = ImColor(192, 221, 255, 171);
    colors[ImGuiCol_TitleBg]                = ImColor(245, 245, 245, 255);
    colors[ImGuiCol_TitleBgActive]          = ImColor(119, 119, 119, 255);
    colors[ImGuiCol_TitleBgCollapsed]       = ImColor(189, 189, 189, 130);
    colors[ImGuiCol_MenuBarBg]              = ImColor(219, 219, 219, 255);
    colors[ImGuiCol_ScrollbarBg]            = ImColor(250, 250, 250, 135);
    colors[ImGuiCol_ScrollbarGrab]          = ImColor(176, 176, 176, 204);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImColor(125, 125, 125, 204);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImColor(125, 125, 125, 255);
    colors[ImGuiCol_CheckMark]              = ImColor(0, 0, 0, 255);
    colors[ImGuiCol_SliderGrab]             = ImColor(0, 0, 0, 199);
    colors[ImGuiCol_SliderGrabActive]       = ImColor(117, 138, 204, 153);
    colors[ImGuiCol_Button]                 = ImColor(47, 47, 47, 102);
    colors[ImGuiCol_ButtonHovered]          = ImColor(162, 162, 162, 255);
    colors[ImGuiCol_ButtonActive]           = ImColor(15, 135, 250, 255);
    colors[ImGuiCol_Header]                 = ImColor(66, 150, 250, 79);
    colors[ImGuiCol_HeaderHovered]          = ImColor(66, 150, 250, 204);
    colors[ImGuiCol_HeaderActive]           = ImColor(66, 150, 250, 255);//
    colors[ImGuiCol_Separator]              = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImColor(214, 214, 214, 255);//
    colors[ImGuiCol_TabHovered]             = ImColor(112, 112, 112, 237);//
    colors[ImGuiCol_TabActive]              = ImColor(112, 112, 112, 237);//
    colors[ImGuiCol_TabUnfocused]           = ImLerp(colors[ImGuiCol_Tab],          colors[ImGuiCol_TitleBg], 0.80f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImLerp(colors[ImGuiCol_TabActive],    colors[ImGuiCol_TitleBg], 0.40f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_NavHighlight]           = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}