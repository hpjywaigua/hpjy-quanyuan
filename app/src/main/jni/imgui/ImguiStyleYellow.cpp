//
// Created by Mahe on 2022/9/25.
//

#include "imgui.h"
#include "imgui_internal.h"


// Those light colors are better suited with a thicker font than the default one + FrameBorder
void ImGui::StyleColorsYellow(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->Colors[ImGuiCol_Text]                  = ImColor(0, 0, 0, 255);
    style->Colors[ImGuiCol_WindowBg]              = ImColor(240, 240, 240, 255);
    style->Colors[ImGuiCol_Border]                = ImColor(144, 145, 147, 255);
    style->Colors[ImGuiCol_FrameBg]               = ImColor(195, 206, 210, 255);       
    style->Colors[ImGuiCol_FrameBgActive]               = ImColor(242, 167, 144, 255);
    style->Colors[ImGuiCol_FrameBgHovered]               = ImColor(242, 167, 144, 255);    
    style->Colors[ImGuiCol_PopupBg]               = ImColor(195, 206, 212, 255);     
    style->Colors[ImGuiCol_Button]                   = ImColor(195, 206, 212, 255);
    style->Colors[ImGuiCol_ButtonActive]             = ImColor(252, 210, 89, 255);
    style->Colors[ImGuiCol_ButtonHovered]            = ImColor(252, 210, 89, 255);   
    style->Colors[ImGuiCol_TitleBg]               = ImColor(240, 240, 240, 255);
    style->Colors[ImGuiCol_TitleBgActive]         = ImColor(240, 240, 240, 255);
    style->Colors[ImGuiCol_TitleBgCollapsed]      = ImColor(240, 240, 240, 255);
    style->Colors[ImGuiCol_CheckMark]             = ImColor(144, 145, 147, 255);
}