
#include "imgui.h"
#include "imgui_internal.h"


// Those light style->Colors are better suited with a thicker font than the default one + FrameBorder
void ImGui::ImguiStyleBlue(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->Colors[ImGuiCol_TitleBg] = ImColor(255, 101, 53, 255);
    style->Colors[ImGuiCol_TitleBgActive] = ImColor(76, 125, 205, 200);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(ImVec4(1.0, 1.0, 1.0, 0.55));
    style->Colors[ImGuiCol_Text] = ImColor(0,0,0,255);
    style->Colors[ImGuiCol_Button] = ImColor(ImVec4(76/255.0, 125/255.0, 205/255.0, 200/255.0));
    style->Colors[ImGuiCol_ButtonActive] = ImColor(ImVec4(0.619, 0.313, 0.685, 0.5));
    style->Colors[ImGuiCol_ButtonHovered] = ImColor(ImVec4(0.619, 0.313, 0.685, 0.5));
    style->Colors[ImGuiCol_FrameBg] = ImColor(255,255,255,180);   
    style->Colors[ImGuiCol_FrameBgActive] = ImColor(ImVec4(0.619, 0.313, 0.685, 0.5));   
    style->Colors[ImGuiCol_FrameBgHovered] = ImColor(ImVec4(0.619, 0.313, 0.685, 0.5));
    style->Colors[ImGuiCol_Header] = ImVec4(76/255.0, 125/255.0, 205/255.0, 200/255.0);    
    style->Colors[ImGuiCol_HeaderActive] = ImColor(ImVec4(0.619, 0.313, 0.685, 0.5));  
    style->Colors[ImGuiCol_HeaderHovered] = ImColor(ImVec4(0.619, 0.313, 0.685, 0.5));
}
