// dear imgui, v1.85 WIP
// (drawing and font code)

/*

Index of this file:

// [SECTION] STB libraries implementation
// [SECTION] Style functions
// [SECTION] ImDrawList
// [SECTION] ImDrawListSplitter
// [SECTION] ImDrawData
// [SECTION] Helpers ShadeVertsXXX functions
// [SECTION] ImFontConfig
// [SECTION] ImFontAtlas
// [SECTION] ImFontAtlas glyph ranges helpers
// [SECTION] ImFontGlyphRangesBuilder
// [SECTION] ImFont
// [SECTION] ImGui Internal Render Helpers
// [SECTION] Decompression code
// [SECTION] Default font data (ProggyClean.ttf)

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui_internal.h"
#ifdef IMGUI_ENABLE_FREETYPE
#include "misc/freetype/imgui_freetype.h"
#endif

#include <stdio.h>      // vsnprintf, sscanf, printf
#if !defined(alloca)
#if defined(__GLIBC__) || defined(__sun) || defined(__APPLE__) || defined(__NEWLIB__)
#include <alloca.h>     // alloca (glibc uses <alloca.h>. Note that Cygwin may have _WIN32 defined, so the order matters here)
#elif defined(_WIN32)
#include <malloc.h>     // alloca
#if !defined(alloca)
#define alloca _alloca  // for clang with MS Codegen
#endif
#else
#include <stdlib.h>     // alloca
#endif
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127)     // condition expression is constant
#pragma warning (disable: 4505)     // unreferenced local function has been removed (stb stuff)
#pragma warning (disable: 4996)     // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#pragma warning (disable: 6255)     // [Static Analyzer] _alloca indicates failure by raising a stack overflow exception.  Consider using _malloca instead.
#pragma warning (disable: 26451)    // [Static Analyzer] Arithmetic overflow : Using operator 'xxx' on a 4 byte value and then casting the result to a 8 byte value. Cast the value to the wider type before calling operator 'xxx' to avoid overflow(io.2).
#pragma warning (disable: 26812)    // [Static Analyzer] The enum type 'xxx' is unscoped. Prefer 'enum class' over 'enum' (Enum.3). [MSVC Static Analyzer)
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'                      // not all warnings are known by all Clang versions and they tend to be rename-happy.. so ignoring warnings triggers new warnings on some configuration. Great!
#endif
#if __has_warning("-Walloca")
#pragma clang diagnostic ignored "-Walloca"                         // warning: use of function '__builtin_alloca' is discouraged
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning: use of old-style cast                            // yes, they are more terse.
#pragma clang diagnostic ignored "-Wfloat-equal"                    // warning: comparing floating point with == or != is unsafe // storing and comparing against same constants ok.
#pragma clang diagnostic ignored "-Wglobal-constructors"            // warning: declaration requires a global destructor         // similar to above, not sure what the exact difference is.
#pragma clang diagnostic ignored "-Wsign-conversion"                // warning: implicit conversion changes signedness
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning: zero as null pointer constant                    // some standard header variations use #define NULL 0
#pragma clang diagnostic ignored "-Wcomma"                          // warning: possible misuse of comma operator here
#pragma clang diagnostic ignored "-Wreserved-id-macro"              // warning: macro name is a reserved identifier
#pragma clang diagnostic ignored "-Wdouble-promotion"               // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: implicit conversion from 'xxx' to 'float' may lose precision
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpragmas"                  // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wunused-function"          // warning: 'xxxx' defined but not used
#pragma GCC diagnostic ignored "-Wdouble-promotion"         // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"               // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#pragma GCC diagnostic ignored "-Wstack-protector"          // warning: stack protector not protecting local variables: variable length buffer
#pragma GCC diagnostic ignored "-Wclass-memaccess"          // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

//-------------------------------------------------------------------------
// [SECTION] STB libraries implementation
//-------------------------------------------------------------------------

// Compile time options:
//#define IMGUI_STB_NAMESPACE           ImStb
//#define IMGUI_STB_TRUETYPE_FILENAME   "my_folder/stb_truetype.h"
//#define IMGUI_STB_RECT_PACK_FILENAME  "my_folder/stb_rect_pack.h"
//#define IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION
//#define IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION

#ifdef IMGUI_STB_NAMESPACE
namespace IMGUI_STB_NAMESPACE
{
#endif

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4456)                             // declaration of 'xx' hides previous local declaration
#pragma warning (disable: 6011)                             // (stb_rectpack) Dereferencing NULL pointer 'cur->next'.
#pragma warning (disable: 6385)                             // (stb_truetype) Reading invalid data from 'buffer':  the readable size is '_Old_3`kernel_width' bytes, but '3' bytes may be read.
#pragma warning (disable: 28182)                            // (stb_rectpack) Dereferencing NULL pointer. 'cur' contains the same NULL value as 'cur->next' did.
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wmissing-prototypes"
#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#pragma clang diagnostic ignored "-Wcast-qual"              // warning: cast from 'const xxxx *' to 'xxx *' drops const qualifier
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"              // warning: comparison is always true due to limited range of data type [-Wtype-limits]
#pragma GCC diagnostic ignored "-Wcast-qual"                // warning: cast from type 'const xxxx *' to type 'xxxx *' casts away qualifiers
#endif

#ifndef STB_RECT_PACK_IMPLEMENTATION                        // in case the user already have an implementation in the _same_ compilation unit (e.g. unity builds)
#ifndef IMGUI_DISABLE_STB_RECT_PACK_IMPLEMENTATION          // in case the user already have an implementation in another compilation unit
#define STBRP_STATIC
#define STBRP_ASSERT(x)     do { IM_ASSERT(x); } while (0)
#define STBRP_SORT          ImQsort
#define STB_RECT_PACK_IMPLEMENTATION
#endif
#ifdef IMGUI_STB_RECT_PACK_FILENAME
#include IMGUI_STB_RECT_PACK_FILENAME
#else
#include "imstb_rectpack.h"
#endif
#endif

#ifdef  IMGUI_ENABLE_STB_TRUETYPE
#ifndef STB_TRUETYPE_IMPLEMENTATION                         // in case the user already have an implementation in the _same_ compilation unit (e.g. unity builds)
#ifndef IMGUI_DISABLE_STB_TRUETYPE_IMPLEMENTATION           // in case the user already have an implementation in another compilation unit
#define STBTT_malloc(x,u)   ((void)(u), IM_ALLOC(x))
#define STBTT_free(x,u)     ((void)(u), IM_FREE(x))
#define STBTT_assert(x)     do { IM_ASSERT(x); } while(0)
#define STBTT_fmod(x,y)     ImFmod(x,y)
#define STBTT_sqrt(x)       ImSqrt(x)
#define STBTT_pow(x,y)      ImPow(x,y)
#define STBTT_fabs(x)       ImFabs(x)
#define STBTT_ifloor(x)     ((int)ImFloorSigned(x))
#define STBTT_iceil(x)      ((int)ImCeil(x))
#define STBTT_STATIC
#define STB_TRUETYPE_IMPLEMENTATION
#else
#define STBTT_DEF extern
#endif
#ifdef IMGUI_STB_TRUETYPE_FILENAME
#include IMGUI_STB_TRUETYPE_FILENAME
#else
#include "imstb_truetype.h"
#endif
#endif
#endif // IMGUI_ENABLE_STB_TRUETYPE

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#if defined(__clang__)
#pragma clang diagnostic pop
#endif

#if defined(_MSC_VER)
#pragma warning (pop)
#endif

#ifdef IMGUI_STB_NAMESPACE
} // namespace ImStb
using namespace IMGUI_STB_NAMESPACE;
#endif

//-----------------------------------------------------------------------------
// [SECTION] Style functions
//-----------------------------------------------------------------------------

void ImGui::StyleColorsDark(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]              = colors[ImGuiCol_Border];
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImLerp(colors[ImGuiCol_Header],       colors[ImGuiCol_TitleBgActive], 0.80f);
    colors[ImGuiCol_TabHovered]             = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_TabActive]              = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
    colors[ImGuiCol_TabUnfocused]           = ImLerp(colors[ImGuiCol_Tab],          colors[ImGuiCol_TitleBg], 0.80f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImLerp(colors[ImGuiCol_TabActive],    colors[ImGuiCol_TitleBg], 0.40f);
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void ImGui::StyleColorsClassic(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.11f, 0.11f, 0.14f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.50f, 0.50f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.43f, 0.43f, 0.43f, 0.39f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.47f, 0.47f, 0.69f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.42f, 0.41f, 0.64f, 0.69f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.27f, 0.27f, 0.54f, 0.83f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.32f, 0.32f, 0.63f, 0.87f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.41f, 0.39f, 0.80f, 0.60f);
    colors[ImGuiCol_Button]                 = ImVec4(0.35f, 0.40f, 0.61f, 0.62f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.40f, 0.48f, 0.71f, 0.79f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.46f, 0.54f, 0.80f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
    colors[ImGuiCol_Separator]              = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.78f, 0.82f, 1.00f, 0.60f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.78f, 0.82f, 1.00f, 0.90f);
    colors[ImGuiCol_Tab]                    = ImLerp(colors[ImGuiCol_Header],       colors[ImGuiCol_TitleBgActive], 0.80f);
    colors[ImGuiCol_TabHovered]             = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_TabActive]              = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
    colors[ImGuiCol_TabUnfocused]           = ImLerp(colors[ImGuiCol_Tab],          colors[ImGuiCol_TitleBg], 0.80f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImLerp(colors[ImGuiCol_TabActive],    colors[ImGuiCol_TitleBg], 0.40f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.27f, 0.27f, 0.38f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.45f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.26f, 0.26f, 0.28f, 1.00f);   // Prefer using Alpha=1.0 here
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight]           = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}

// Those light colors are better suited with a thicker font than the default one + FrameBorder
void ImGui::StyleColorsLight(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    colors[ImGuiCol_Text]                   = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
    colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.46f, 0.54f, 0.80f, 0.60f);
    colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImLerp(colors[ImGuiCol_Header],       colors[ImGuiCol_TitleBgActive], 0.90f);
    colors[ImGuiCol_TabHovered]             = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_TabActive]              = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
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

//-----------------------------------------------------------------------------
// [SECTION] ImDrawList
//-----------------------------------------------------------------------------

ImDrawListSharedData::ImDrawListSharedData()
{
    memset(this, 0, sizeof(*this));
    for (int i = 0; i < IM_ARRAYSIZE(ArcFastVtx); i++)
    {
        const float a = ((float)i * 2 * IM_PI) / (float)IM_ARRAYSIZE(ArcFastVtx);
        ArcFastVtx[i] = ImVec2(ImCos(a), ImSin(a));
    }
    ArcFastRadiusCutoff = IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(IM_DRAWLIST_ARCFAST_SAMPLE_MAX, CircleSegmentMaxError);
}

void ImDrawListSharedData::SetCircleTessellationMaxError(float max_error)
{
    if (CircleSegmentMaxError == max_error)
        return;

    IM_ASSERT(max_error > 0.0f);
    CircleSegmentMaxError = max_error;
    for (int i = 0; i < IM_ARRAYSIZE(CircleSegmentCounts); i++)
    {
        const float radius = (float)i;
        CircleSegmentCounts[i] = (ImU8)((i > 0) ? IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(radius, CircleSegmentMaxError) : 0);
    }
    ArcFastRadiusCutoff = IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(IM_DRAWLIST_ARCFAST_SAMPLE_MAX, CircleSegmentMaxError);
}

// Initialize before use in a new frame. We always have a command ready in the buffer.
void ImDrawList::_ResetForNewFrame()
{
    // Verify that the ImDrawCmd fields we want to memcmp() are contiguous in memory.
    // (those should be IM_STATIC_ASSERT() in theory but with our pre C++11 setup the whole check doesn't compile with GCC)
    IM_ASSERT(IM_OFFSETOF(ImDrawCmd, ClipRect) == 0);
    IM_ASSERT(IM_OFFSETOF(ImDrawCmd, TextureId) == sizeof(ImVec4));
    IM_ASSERT(IM_OFFSETOF(ImDrawCmd, VtxOffset) == sizeof(ImVec4) + sizeof(ImTextureID));

    CmdBuffer.resize(0);
    IdxBuffer.resize(0);
    VtxBuffer.resize(0);
    Flags = _Data->InitialFlags;
    memset(&_CmdHeader, 0, sizeof(_CmdHeader));
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.resize(0);
    _TextureIdStack.resize(0);
    _Path.resize(0);
    _Splitter.Clear();
    CmdBuffer.push_back(ImDrawCmd());
    _FringeScale = 1.0f;
}

void ImDrawList::_ClearFreeMemory()
{
    CmdBuffer.clear();
    IdxBuffer.clear();
    VtxBuffer.clear();
    Flags = ImDrawListFlags_None;
    _VtxCurrentIdx = 0;
    _VtxWritePtr = NULL;
    _IdxWritePtr = NULL;
    _ClipRectStack.clear();
    _TextureIdStack.clear();
    _Path.clear();
    _Splitter.ClearFreeMemory();
}

ImDrawList* ImDrawList::CloneOutput() const
{
    ImDrawList* dst = IM_NEW(ImDrawList(_Data));
    dst->CmdBuffer = CmdBuffer;
    dst->IdxBuffer = IdxBuffer;
    dst->VtxBuffer = VtxBuffer;
    dst->Flags = Flags;
    return dst;
}

void ImDrawList::AddDrawCmd()
{
    ImDrawCmd draw_cmd;
    draw_cmd.ClipRect = _CmdHeader.ClipRect;    // Same as calling ImDrawCmd_HeaderCopy()
    draw_cmd.TextureId = _CmdHeader.TextureId;
    draw_cmd.VtxOffset = _CmdHeader.VtxOffset;
    draw_cmd.IdxOffset = IdxBuffer.Size;

    IM_ASSERT(draw_cmd.ClipRect.x <= draw_cmd.ClipRect.z && draw_cmd.ClipRect.y <= draw_cmd.ClipRect.w);
    CmdBuffer.push_back(draw_cmd);
}

// Pop trailing draw command (used before merging or presenting to user)
// Note that this leaves the ImDrawList in a state unfit for further commands, as most code assume that CmdBuffer.Size > 0 && CmdBuffer.back().UserCallback == NULL
void ImDrawList::_PopUnusedDrawCmd()
{
    if (CmdBuffer.Size == 0)
        return;
    ImDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount == 0 && curr_cmd->UserCallback == NULL)
        CmdBuffer.pop_back();
}

void ImDrawList::AddCallback(ImDrawCallback callback, void* callback_data)
{
    ImDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    IM_ASSERT(curr_cmd->UserCallback == NULL);
    if (curr_cmd->ElemCount != 0)
    {
        AddDrawCmd();
        curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    }
    curr_cmd->UserCallback = callback;
    curr_cmd->UserCallbackData = callback_data;

    AddDrawCmd(); // Force a new command after us (see comment below)
}

// Compare ClipRect, TextureId and VtxOffset with a single memcmp()
#define ImDrawCmd_HeaderSize                        (IM_OFFSETOF(ImDrawCmd, VtxOffset) + sizeof(unsigned int))
#define ImDrawCmd_HeaderCompare(CMD_LHS, CMD_RHS)   (memcmp(CMD_LHS, CMD_RHS, ImDrawCmd_HeaderSize))    // Compare ClipRect, TextureId, VtxOffset
#define ImDrawCmd_HeaderCopy(CMD_DST, CMD_SRC)      (memcpy(CMD_DST, CMD_SRC, ImDrawCmd_HeaderSize))    // Copy ClipRect, TextureId, VtxOffset

// Try to merge two last draw commands
void ImDrawList::_TryMergeDrawCmds()
{
    ImDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    ImDrawCmd* prev_cmd = curr_cmd - 1;
    if (ImDrawCmd_HeaderCompare(curr_cmd, prev_cmd) == 0 && curr_cmd->UserCallback == NULL && prev_cmd->UserCallback == NULL)
    {
        prev_cmd->ElemCount += curr_cmd->ElemCount;
        CmdBuffer.pop_back();
    }
}

// Our scheme may appears a bit unusual, basically we want the most-common calls AddLine AddRect etc. to not have to perform any check so we always have a command ready in the stack.
// The cost of figuring out if a new command has to be added or if we can merge is paid in those Update** functions only.
void ImDrawList::_OnChangedClipRect()
{
    // If current command is used with different settings we need to add a new command
    ImDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &_CmdHeader.ClipRect, sizeof(ImVec4)) != 0)
    {
        AddDrawCmd();
        return;
    }
    IM_ASSERT(curr_cmd->UserCallback == NULL);

    // Try to merge with previous command if it matches, else use current command
    ImDrawCmd* prev_cmd = curr_cmd - 1;
    if (curr_cmd->ElemCount == 0 && CmdBuffer.Size > 1 && ImDrawCmd_HeaderCompare(&_CmdHeader, prev_cmd) == 0 && prev_cmd->UserCallback == NULL)
    {
        CmdBuffer.pop_back();
        return;
    }

    curr_cmd->ClipRect = _CmdHeader.ClipRect;
}

void ImDrawList::_OnChangedTextureID()
{
    // If current command is used with different settings we need to add a new command
    ImDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != _CmdHeader.TextureId)
    {
        AddDrawCmd();
        return;
    }
    IM_ASSERT(curr_cmd->UserCallback == NULL);

    // Try to merge with previous command if it matches, else use current command
    ImDrawCmd* prev_cmd = curr_cmd - 1;
    if (curr_cmd->ElemCount == 0 && CmdBuffer.Size > 1 && ImDrawCmd_HeaderCompare(&_CmdHeader, prev_cmd) == 0 && prev_cmd->UserCallback == NULL)
    {
        CmdBuffer.pop_back();
        return;
    }

    curr_cmd->TextureId = _CmdHeader.TextureId;
}

void ImDrawList::_OnChangedVtxOffset()
{
    // We don't need to compare curr_cmd->VtxOffset != _CmdHeader.VtxOffset because we know it'll be different at the time we call this.
    _VtxCurrentIdx = 0;
    ImDrawCmd* curr_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    //IM_ASSERT(curr_cmd->VtxOffset != _CmdHeader.VtxOffset); // See #3349
    if (curr_cmd->ElemCount != 0)
    {
        AddDrawCmd();
        return;
    }
    IM_ASSERT(curr_cmd->UserCallback == NULL);
    curr_cmd->VtxOffset = _CmdHeader.VtxOffset;
}

int ImDrawList::_CalcCircleAutoSegmentCount(float radius) const
{
    // Automatic segment count
    const int radius_idx = (int)(radius + 0.999999f); // ceil to never reduce accuracy
    if (radius_idx < IM_ARRAYSIZE(_Data->CircleSegmentCounts))
        return _Data->CircleSegmentCounts[radius_idx]; // Use cached value
    else
        return IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(radius, _Data->CircleSegmentMaxError);
}

// Render-level scissoring. This is passed down to your render function but not used for CPU-side coarse clipping. Prefer using higher-level ImGui::PushClipRect() to affect logic (hit-testing and widget culling)
void ImDrawList::PushClipRect(ImVec2 cr_min, ImVec2 cr_max, bool intersect_with_current_clip_rect)
{
    ImVec4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
    if (intersect_with_current_clip_rect)
    {
        ImVec4 current = _CmdHeader.ClipRect;
        if (cr.x < current.x) cr.x = current.x;
        if (cr.y < current.y) cr.y = current.y;
        if (cr.z > current.z) cr.z = current.z;
        if (cr.w > current.w) cr.w = current.w;
    }
    cr.z = ImMax(cr.x, cr.z);
    cr.w = ImMax(cr.y, cr.w);

    _ClipRectStack.push_back(cr);
    _CmdHeader.ClipRect = cr;
    _OnChangedClipRect();
}

void ImDrawList::PushClipRectFullScreen()
{
    PushClipRect(ImVec2(_Data->ClipRectFullscreen.x, _Data->ClipRectFullscreen.y), ImVec2(_Data->ClipRectFullscreen.z, _Data->ClipRectFullscreen.w));
}

void ImDrawList::PopClipRect()
{
    _ClipRectStack.pop_back();
    _CmdHeader.ClipRect = (_ClipRectStack.Size == 0) ? _Data->ClipRectFullscreen : _ClipRectStack.Data[_ClipRectStack.Size - 1];
    _OnChangedClipRect();
}

void ImDrawList::PushTextureID(ImTextureID texture_id)
{
    _TextureIdStack.push_back(texture_id);
    _CmdHeader.TextureId = texture_id;
    _OnChangedTextureID();
}

void ImDrawList::PopTextureID()
{
    _TextureIdStack.pop_back();
    _CmdHeader.TextureId = (_TextureIdStack.Size == 0) ? (ImTextureID)NULL : _TextureIdStack.Data[_TextureIdStack.Size - 1];
    _OnChangedTextureID();
}

// Reserve space for a number of vertices and indices.
// You must finish filling your reserved data before calling PrimReserve() again, as it may reallocate or
// submit the intermediate results. PrimUnreserve() can be used to release unused allocations.
void ImDrawList::PrimReserve(int idx_count, int vtx_count)
{
    // Large mesh support (when enabled)
    IM_ASSERT_PARANOID(idx_count >= 0 && vtx_count >= 0);
    if (sizeof(ImDrawIdx) == 2 && (_VtxCurrentIdx + vtx_count >= (1 << 16)) && (Flags & ImDrawListFlags_AllowVtxOffset))
    {
        // FIXME: In theory we should be testing that vtx_count <64k here.
        // In practice, RenderText() relies on reserving ahead for a worst case scenario so it is currently useful for us
        // to not make that check until we rework the text functions to handle clipping and large horizontal lines better.
        _CmdHeader.VtxOffset = VtxBuffer.Size;
        _OnChangedVtxOffset();
    }

    ImDrawCmd* draw_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    draw_cmd->ElemCount += idx_count;

    int vtx_buffer_old_size = VtxBuffer.Size;
    VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
    _VtxWritePtr = VtxBuffer.Data + vtx_buffer_old_size;

    int idx_buffer_old_size = IdxBuffer.Size;
    IdxBuffer.resize(idx_buffer_old_size + idx_count);
    _IdxWritePtr = IdxBuffer.Data + idx_buffer_old_size;
}

// Release the a number of reserved vertices/indices from the end of the last reservation made with PrimReserve().
void ImDrawList::PrimUnreserve(int idx_count, int vtx_count)
{
    IM_ASSERT_PARANOID(idx_count >= 0 && vtx_count >= 0);

    ImDrawCmd* draw_cmd = &CmdBuffer.Data[CmdBuffer.Size - 1];
    draw_cmd->ElemCount -= idx_count;
    VtxBuffer.shrink(VtxBuffer.Size - vtx_count);
    IdxBuffer.shrink(IdxBuffer.Size - idx_count);
}

// Fully unrolled with inline call to keep our debug builds decently fast.
void ImDrawList::PrimRect(const ImVec2& a, const ImVec2& c, ImU32 col)
{
    ImVec2 b(c.x, a.y), d(a.x, c.y), uv(_Data->TexUvWhitePixel);
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void ImDrawList::PrimRectUV(const ImVec2& a, const ImVec2& c, const ImVec2& uv_a, const ImVec2& uv_c, ImU32 col)
{
    ImVec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

void ImDrawList::PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
    ImDrawIdx idx = (ImDrawIdx)_VtxCurrentIdx;
    _IdxWritePtr[0] = idx; _IdxWritePtr[1] = (ImDrawIdx)(idx+1); _IdxWritePtr[2] = (ImDrawIdx)(idx+2);
    _IdxWritePtr[3] = idx; _IdxWritePtr[4] = (ImDrawIdx)(idx+2); _IdxWritePtr[5] = (ImDrawIdx)(idx+3);
    _VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
    _VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
    _VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
    _VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
    _VtxWritePtr += 4;
    _VtxCurrentIdx += 4;
    _IdxWritePtr += 6;
}

// On AddPolyline() and AddConvexPolyFilled() we intentionally avoid using ImVec2 and superfluous function calls to optimize debug/non-inlined builds.
// - Those macros expects l-values and need to be used as their own statement.
// - Those macros are intentionally not surrounded by the 'do {} while (0)' idiom because even that translates to runtime with debug compilers.
#define IM_NORMALIZE2F_OVER_ZERO(VX,VY)     { float d2 = VX*VX + VY*VY; if (d2 > 0.0f) { float inv_len = ImRsqrt(d2); VX *= inv_len; VY *= inv_len; } } (void)0
#define IM_FIXNORMAL2F_MAX_INVLEN2          100.0f // 500.0f (see #4053, #3366)
#define IM_FIXNORMAL2F(VX,VY)               { float d2 = VX*VX + VY*VY; if (d2 > 0.000001f) { float inv_len2 = 1.0f / d2; if (inv_len2 > IM_FIXNORMAL2F_MAX_INVLEN2) inv_len2 = IM_FIXNORMAL2F_MAX_INVLEN2; VX *= inv_len2; VY *= inv_len2; } } (void)0

// TODO: Thickness anti-aliased lines cap are missing their AA fringe.
// We avoid using the ImVec2 math operators here to reduce cost to a minimum for debug/non-inlined builds.
void ImDrawList::AddPolyline(const ImVec2* points, const int points_count, ImU32 col, ImDrawFlags flags, float thickness)
{
    if (points_count < 2)
        return;

    const bool closed = (flags & ImDrawFlags_Closed) != 0;
    const ImVec2 opaque_uv = _Data->TexUvWhitePixel;
    const int count = closed ? points_count : points_count - 1; // The number of line segments we need to draw
    const bool thick_line = (thickness > _FringeScale);

    if (Flags & ImDrawListFlags_AntiAliasedLines)
    {
        // Anti-aliased stroke
        const float AA_SIZE = _FringeScale;
        const ImU32 col_trans = col & ~IM_COL32_A_MASK;

        // Thicknesses <1.0 should behave like thickness 1.0
        thickness = ImMax(thickness, 1.0f);
        const int integer_thickness = (int)thickness;
        const float fractional_thickness = thickness - integer_thickness;

        // Do we want to draw this line using a texture?
        // - For now, only draw integer-width lines using textures to avoid issues with the way scaling occurs, could be improved.
        // - If AA_SIZE is not 1.0f we cannot use the texture path.
        const bool use_texture = (Flags & ImDrawListFlags_AntiAliasedLinesUseTex) && (integer_thickness < IM_DRAWLIST_TEX_LINES_WIDTH_MAX) && (fractional_thickness <= 0.00001f) && (AA_SIZE == 1.0f);

        // We should never hit this, because NewFrame() doesn't set ImDrawListFlags_AntiAliasedLinesUseTex unless ImFontAtlasFlags_NoBakedLines is off
        IM_ASSERT_PARANOID(!use_texture || !(_Data->Font->ContainerAtlas->Flags & ImFontAtlasFlags_NoBakedLines));

        const int idx_count = use_texture ? (count * 6) : (thick_line ? count * 18 : count * 12);
        const int vtx_count = use_texture ? (points_count * 2) : (thick_line ? points_count * 4 : points_count * 3);
        PrimReserve(idx_count, vtx_count);

        // Temporary buffer
        // The first <points_count> items are normals at each line point, then after that there are either 2 or 4 temp points for each line point
        ImVec2* temp_normals = (ImVec2*)alloca(points_count * ((use_texture || !thick_line) ? 3 : 5) * sizeof(ImVec2)); //-V630
        ImVec2* temp_points = temp_normals + points_count;

        // Calculate normals (tangents) for each line segment
        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            float dx = points[i2].x - points[i1].x;
            float dy = points[i2].y - points[i1].y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i1].x = dy;
            temp_normals[i1].y = -dx;
        }
        if (!closed)
            temp_normals[points_count - 1] = temp_normals[points_count - 2];

        // If we are drawing a one-pixel-wide line without a texture, or a textured line of any width, we only need 2 or 3 vertices per point
        if (use_texture || !thick_line)
        {
            // [PATH 1] Texture-based lines (thick or non-thick)
            // [PATH 2] Non texture-based lines (non-thick)

            // The width of the geometry we need to draw - this is essentially <thickness> pixels for the line itself, plus "one pixel" for AA.
            // - In the texture-based path, we don't use AA_SIZE here because the +1 is tied to the generated texture
            //   (see ImFontAtlasBuildRenderLinesTexData() function), and so alternate values won't work without changes to that code.
            // - In the non texture-based paths, we would allow AA_SIZE to potentially be != 1.0f with a patch (e.g. fringe_scale patch to
            //   allow scaling geometry while preserving one-screen-pixel AA fringe).
            const float half_draw_size = use_texture ? ((thickness * 0.5f) + 1) : AA_SIZE;

            // If line is not closed, the first and last points need to be generated differently as there are no normals to blend
            if (!closed)
            {
                temp_points[0] = points[0] + temp_normals[0] * half_draw_size;
                temp_points[1] = points[0] - temp_normals[0] * half_draw_size;
                temp_points[(points_count-1)*2+0] = points[points_count-1] + temp_normals[points_count-1] * half_draw_size;
                temp_points[(points_count-1)*2+1] = points[points_count-1] - temp_normals[points_count-1] * half_draw_size;
            }

            // Generate the indices to form a number of triangles for each line segment, and the vertices for the line edges
            // This takes points n and n+1 and writes into n+1, with the first point in a closed line being generated from the final one (as n+1 wraps)
            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx; // Vertex index for start of line segment
            for (int i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
            {
                const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1; // i2 is the second point of the line segment
                const unsigned int idx2 = ((i1 + 1) == points_count) ? _VtxCurrentIdx : (idx1 + (use_texture ? 2 : 3)); // Vertex index for end of segment

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                dm_x *= half_draw_size; // dm_x, dm_y are offset to the outer edge of the AA area
                dm_y *= half_draw_size;

                // Add temporary vertexes for the outer edges
                ImVec2* out_vtx = &temp_points[i2 * 2];
                out_vtx[0].x = points[i2].x + dm_x;
                out_vtx[0].y = points[i2].y + dm_y;
                out_vtx[1].x = points[i2].x - dm_x;
                out_vtx[1].y = points[i2].y - dm_y;

                if (use_texture)
                {
                    // Add indices for two triangles
                    _IdxWritePtr[0] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 1); // Right tri
                    _IdxWritePtr[3] = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[4] = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 0); // Left tri
                    _IdxWritePtr += 6;
                }
                else
                {
                    // Add indexes for four triangles
                    _IdxWritePtr[0] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[1] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[2] = (ImDrawIdx)(idx1 + 2); // Right tri 1
                    _IdxWritePtr[3] = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[4] = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[5] = (ImDrawIdx)(idx2 + 0); // Right tri 2
                    _IdxWritePtr[6] = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[7] = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[8] = (ImDrawIdx)(idx1 + 0); // Left tri 1
                    _IdxWritePtr[9] = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1); // Left tri 2
                    _IdxWritePtr += 12;
                }

                idx1 = idx2;
            }

            // Add vertexes for each point on the line
            if (use_texture)
            {
                // If we're using textures we only need to emit the left/right edge vertices
                ImVec4 tex_uvs = _Data->TexUvLines[integer_thickness];
                /*if (fractional_thickness != 0.0f) // Currently always zero when use_texture==false!
                {
                    const ImVec4 tex_uvs_1 = _Data->TexUvLines[integer_thickness + 1];
                    tex_uvs.x = tex_uvs.x + (tex_uvs_1.x - tex_uvs.x) * fractional_thickness; // inlined ImLerp()
                    tex_uvs.y = tex_uvs.y + (tex_uvs_1.y - tex_uvs.y) * fractional_thickness;
                    tex_uvs.z = tex_uvs.z + (tex_uvs_1.z - tex_uvs.z) * fractional_thickness;
                    tex_uvs.w = tex_uvs.w + (tex_uvs_1.w - tex_uvs.w) * fractional_thickness;
                }*/
                ImVec2 tex_uv0(tex_uvs.x, tex_uvs.y);
                ImVec2 tex_uv1(tex_uvs.z, tex_uvs.w);
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = temp_points[i * 2 + 0]; _VtxWritePtr[0].uv = tex_uv0; _VtxWritePtr[0].col = col; // Left-side outer edge
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 1]; _VtxWritePtr[1].uv = tex_uv1; _VtxWritePtr[1].col = col; // Right-side outer edge
                    _VtxWritePtr += 2;
                }
            }
            else
            {
                // If we're not using a texture, we need the center vertex as well
                for (int i = 0; i < points_count; i++)
                {
                    _VtxWritePtr[0].pos = points[i];              _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col;       // Center of line
                    _VtxWritePtr[1].pos = temp_points[i * 2 + 0]; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col_trans; // Left-side outer edge
                    _VtxWritePtr[2].pos = temp_points[i * 2 + 1]; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col_trans; // Right-side outer edge
                    _VtxWritePtr += 3;
                }
            }
        }
        else
        {
            // [PATH 2] Non texture-based lines (thick): we need to draw the solid line core and thus require four vertices per point
            const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;

            // If line is not closed, the first and last points need to be generated differently as there are no normals to blend
            if (!closed)
            {
                const int points_last = points_count - 1;
                temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
                temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
                temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 0] = points[points_last] + temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
                temp_points[points_last * 4 + 1] = points[points_last] + temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 2] = points[points_last] - temp_normals[points_last] * (half_inner_thickness);
                temp_points[points_last * 4 + 3] = points[points_last] - temp_normals[points_last] * (half_inner_thickness + AA_SIZE);
            }

            // Generate the indices to form a number of triangles for each line segment, and the vertices for the line edges
            // This takes points n and n+1 and writes into n+1, with the first point in a closed line being generated from the final one (as n+1 wraps)
            // FIXME-OPT: Merge the different loops, possibly remove the temporary buffer.
            unsigned int idx1 = _VtxCurrentIdx; // Vertex index for start of line segment
            for (int i1 = 0; i1 < count; i1++) // i1 is the first point of the line segment
            {
                const int i2 = (i1 + 1) == points_count ? 0 : (i1 + 1); // i2 is the second point of the line segment
                const unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : (idx1 + 4); // Vertex index for end of segment

                // Average normals
                float dm_x = (temp_normals[i1].x + temp_normals[i2].x) * 0.5f;
                float dm_y = (temp_normals[i1].y + temp_normals[i2].y) * 0.5f;
                IM_FIXNORMAL2F(dm_x, dm_y);
                float dm_out_x = dm_x * (half_inner_thickness + AA_SIZE);
                float dm_out_y = dm_y * (half_inner_thickness + AA_SIZE);
                float dm_in_x = dm_x * half_inner_thickness;
                float dm_in_y = dm_y * half_inner_thickness;

                // Add temporary vertices
                ImVec2* out_vtx = &temp_points[i2 * 4];
                out_vtx[0].x = points[i2].x + dm_out_x;
                out_vtx[0].y = points[i2].y + dm_out_y;
                out_vtx[1].x = points[i2].x + dm_in_x;
                out_vtx[1].y = points[i2].y + dm_in_y;
                out_vtx[2].x = points[i2].x - dm_in_x;
                out_vtx[2].y = points[i2].y - dm_in_y;
                out_vtx[3].x = points[i2].x - dm_out_x;
                out_vtx[3].y = points[i2].y - dm_out_y;

                // Add indexes
                _IdxWritePtr[0]  = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[1]  = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[2]  = (ImDrawIdx)(idx1 + 2);
                _IdxWritePtr[3]  = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[4]  = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[5]  = (ImDrawIdx)(idx2 + 1);
                _IdxWritePtr[6]  = (ImDrawIdx)(idx2 + 1); _IdxWritePtr[7]  = (ImDrawIdx)(idx1 + 1); _IdxWritePtr[8]  = (ImDrawIdx)(idx1 + 0);
                _IdxWritePtr[9]  = (ImDrawIdx)(idx1 + 0); _IdxWritePtr[10] = (ImDrawIdx)(idx2 + 0); _IdxWritePtr[11] = (ImDrawIdx)(idx2 + 1);
                _IdxWritePtr[12] = (ImDrawIdx)(idx2 + 2); _IdxWritePtr[13] = (ImDrawIdx)(idx1 + 2); _IdxWritePtr[14] = (ImDrawIdx)(idx1 + 3);
                _IdxWritePtr[15] = (ImDrawIdx)(idx1 + 3); _IdxWritePtr[16] = (ImDrawIdx)(idx2 + 3); _IdxWritePtr[17] = (ImDrawIdx)(idx2 + 2);
                _IdxWritePtr += 18;

                idx1 = idx2;
            }

            // Add vertices
            for (int i = 0; i < points_count; i++)
            {
                _VtxWritePtr[0].pos = temp_points[i * 4 + 0]; _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col_trans;
                _VtxWritePtr[1].pos = temp_points[i * 4 + 1]; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col;
                _VtxWritePtr[2].pos = temp_points[i * 4 + 2]; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col;
                _VtxWritePtr[3].pos = temp_points[i * 4 + 3]; _VtxWritePtr[3].uv = opaque_uv; _VtxWritePtr[3].col = col_trans;
                _VtxWritePtr += 4;
            }
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else
    {
        // [PATH 4] Non texture-based, Non anti-aliased lines
        const int idx_count = count * 6;
        const int vtx_count = count * 4;    // FIXME-OPT: Not sharing edges
        PrimReserve(idx_count, vtx_count);

        for (int i1 = 0; i1 < count; i1++)
        {
            const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
            const ImVec2& p1 = points[i1];
            const ImVec2& p2 = points[i2];

            float dx = p2.x - p1.x;
            float dy = p2.y - p1.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            dx *= (thickness * 0.5f);
            dy *= (thickness * 0.5f);

            _VtxWritePtr[0].pos.x = p1.x + dy; _VtxWritePtr[0].pos.y = p1.y - dx; _VtxWritePtr[0].uv = opaque_uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr[1].pos.x = p2.x + dy; _VtxWritePtr[1].pos.y = p2.y - dx; _VtxWritePtr[1].uv = opaque_uv; _VtxWritePtr[1].col = col;
            _VtxWritePtr[2].pos.x = p2.x - dy; _VtxWritePtr[2].pos.y = p2.y + dx; _VtxWritePtr[2].uv = opaque_uv; _VtxWritePtr[2].col = col;
            _VtxWritePtr[3].pos.x = p1.x - dy; _VtxWritePtr[3].pos.y = p1.y + dx; _VtxWritePtr[3].uv = opaque_uv; _VtxWritePtr[3].col = col;
            _VtxWritePtr += 4;

            _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx + 1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx + 2);
            _IdxWritePtr[3] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[4] = (ImDrawIdx)(_VtxCurrentIdx + 2); _IdxWritePtr[5] = (ImDrawIdx)(_VtxCurrentIdx + 3);
            _IdxWritePtr += 6;
            _VtxCurrentIdx += 4;
        }
    }
}

// We intentionally avoid using ImVec2 and its math operators here to reduce cost to a minimum for debug/non-inlined builds.
void ImDrawList::AddConvexPolyFilled(const ImVec2* points, const int points_count, ImU32 col)
{
    if (points_count < 3)
        return;

    const ImVec2 uv = _Data->TexUvWhitePixel;

    if (Flags & ImDrawListFlags_AntiAliasedFill)
    {
        // Anti-aliased Fill
        const float AA_SIZE = _FringeScale;
        const ImU32 col_trans = col & ~IM_COL32_A_MASK;
        const int idx_count = (points_count - 2)*3 + points_count * 6;
        const int vtx_count = (points_count * 2);
        PrimReserve(idx_count, vtx_count);

        // Add indexes for fill
        unsigned int vtx_inner_idx = _VtxCurrentIdx;
        unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + ((i - 1) << 1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_inner_idx + (i << 1));
            _IdxWritePtr += 3;
        }

        // Compute normals
        ImVec2* temp_normals = (ImVec2*)alloca(points_count * sizeof(ImVec2)); //-V630
        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            const ImVec2& p0 = points[i0];
            const ImVec2& p1 = points[i1];
            float dx = p1.x - p0.x;
            float dy = p1.y - p0.y;
            IM_NORMALIZE2F_OVER_ZERO(dx, dy);
            temp_normals[i0].x = dy;
            temp_normals[i0].y = -dx;
        }

        for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
        {
            // Average normals
            const ImVec2& n0 = temp_normals[i0];
            const ImVec2& n1 = temp_normals[i1];
            float dm_x = (n0.x + n1.x) * 0.5f;
            float dm_y = (n0.y + n1.y) * 0.5f;
            IM_FIXNORMAL2F(dm_x, dm_y);
            dm_x *= AA_SIZE * 0.5f;
            dm_y *= AA_SIZE * 0.5f;

            // Add vertices
            _VtxWritePtr[0].pos.x = (points[i1].x - dm_x); _VtxWritePtr[0].pos.y = (points[i1].y - dm_y); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;        // Inner
            _VtxWritePtr[1].pos.x = (points[i1].x + dm_x); _VtxWritePtr[1].pos.y = (points[i1].y + dm_y); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;  // Outer
            _VtxWritePtr += 2;

            // Add indexes for fringes
            _IdxWritePtr[0] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1)); _IdxWritePtr[1] = (ImDrawIdx)(vtx_inner_idx + (i0 << 1)); _IdxWritePtr[2] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1));
            _IdxWritePtr[3] = (ImDrawIdx)(vtx_outer_idx + (i0 << 1)); _IdxWritePtr[4] = (ImDrawIdx)(vtx_outer_idx + (i1 << 1)); _IdxWritePtr[5] = (ImDrawIdx)(vtx_inner_idx + (i1 << 1));
            _IdxWritePtr += 6;
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
    else
    {
        // Non Anti-aliased Fill
        const int idx_count = (points_count - 2)*3;
        const int vtx_count = points_count;
        PrimReserve(idx_count, vtx_count);
        for (int i = 0; i < vtx_count; i++)
        {
            _VtxWritePtr[0].pos = points[i]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
            _VtxWritePtr++;
        }
        for (int i = 2; i < points_count; i++)
        {
            _IdxWritePtr[0] = (ImDrawIdx)(_VtxCurrentIdx); _IdxWritePtr[1] = (ImDrawIdx)(_VtxCurrentIdx + i - 1); _IdxWritePtr[2] = (ImDrawIdx)(_VtxCurrentIdx + i);
            _IdxWritePtr += 3;
        }
        _VtxCurrentIdx += (ImDrawIdx)vtx_count;
    }
}

void ImDrawList::_PathArcToFastEx(const ImVec2& center, float radius, int a_min_sample, int a_max_sample, int a_step)
{
    if (radius <= 0.0f)
    {
        _Path.push_back(center);
        return;
    }

    // Calculate arc auto segment step size
    if (a_step <= 0)
        a_step = IM_DRAWLIST_ARCFAST_SAMPLE_MAX / _CalcCircleAutoSegmentCount(radius);

    // Make sure we never do steps larger than one quarter of the circle
    a_step = ImClamp(a_step, 1, IM_DRAWLIST_ARCFAST_TABLE_SIZE / 4);

    const int sample_range = ImAbs(a_max_sample - a_min_sample);
    const int a_next_step = a_step;

    int samples = sample_range + 1;
    bool extra_max_sample = false;
    if (a_step > 1)
    {
        samples            = sample_range / a_step + 1;
        const int overstep = sample_range % a_step;

        if (overstep > 0)
        {
            extra_max_sample = true;
            samples++;

            // When we have overstep to avoid awkwardly looking one long line and one tiny one at the end,
            // distribute first step range evenly between them by reducing first step size.
            if (sample_range > 0)
                a_step -= (a_step - overstep) / 2;
        }
    }

    _Path.resize(_Path.Size + samples);
    ImVec2* out_ptr = _Path.Data + (_Path.Size - samples);

    int sample_index = a_min_sample;
    if (sample_index < 0 || sample_index >= IM_DRAWLIST_ARCFAST_SAMPLE_MAX)
    {
        sample_index = sample_index % IM_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (sample_index < 0)
            sample_index += IM_DRAWLIST_ARCFAST_SAMPLE_MAX;
    }

    if (a_max_sample >= a_min_sample)
    {
        for (int a = a_min_sample; a <= a_max_sample; a += a_step, sample_index += a_step, a_step = a_next_step)
        {
            // a_step is clamped to IM_DRAWLIST_ARCFAST_SAMPLE_MAX, so we have guaranteed that it will not wrap over range twice or more
            if (sample_index >= IM_DRAWLIST_ARCFAST_SAMPLE_MAX)
                sample_index -= IM_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const ImVec2 s = _Data->ArcFastVtx[sample_index];
            out_ptr->x = center.x + s.x * radius;
            out_ptr->y = center.y + s.y * radius;
            out_ptr++;
        }
    }
    else
    {
        for (int a = a_min_sample; a >= a_max_sample; a -= a_step, sample_index -= a_step, a_step = a_next_step)
        {
            // a_step is clamped to IM_DRAWLIST_ARCFAST_SAMPLE_MAX, so we have guaranteed that it will not wrap over range twice or more
            if (sample_index < 0)
                sample_index += IM_DRAWLIST_ARCFAST_SAMPLE_MAX;

            const ImVec2 s = _Data->ArcFastVtx[sample_index];
            out_ptr->x = center.x + s.x * radius;
            out_ptr->y = center.y + s.y * radius;
            out_ptr++;
        }
    }

    if (extra_max_sample)
    {
        int normalized_max_sample = a_max_sample % IM_DRAWLIST_ARCFAST_SAMPLE_MAX;
        if (normalized_max_sample < 0)
            normalized_max_sample += IM_DRAWLIST_ARCFAST_SAMPLE_MAX;

        const ImVec2 s = _Data->ArcFastVtx[normalized_max_sample];
        out_ptr->x = center.x + s.x * radius;
        out_ptr->y = center.y + s.y * radius;
        out_ptr++;
    }

    IM_ASSERT_PARANOID(_Path.Data + _Path.Size == out_ptr);
}

void ImDrawList::_PathArcToN(const ImVec2& center, float radius, float a_min, float a_max, int num_segments)
{
    if (radius <= 0.0f)
    {
        _Path.push_back(center);
        return;
    }

    // Note that we are adding a point at both a_min and a_max.
    // If you are trying to draw a full closed circle you don't want the overlapping points!
    _Path.reserve(_Path.Size + (num_segments + 1));
    for (int i = 0; i <= num_segments; i++)
    {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        _Path.push_back(ImVec2(center.x + ImCos(a) * radius, center.y + ImSin(a) * radius));
    }
}

// 0: East, 3: South, 6: West, 9: North, 12: East
void ImDrawList::PathArcToFast(const ImVec2& center, float radius, int a_min_of_12, int a_max_of_12)
{
    if (radius <= 0.0f)
    {
        _Path.push_back(center);
        return;
    }
    _PathArcToFastEx(center, radius, a_min_of_12 * IM_DRAWLIST_ARCFAST_SAMPLE_MAX / 12, a_max_of_12 * IM_DRAWLIST_ARCFAST_SAMPLE_MAX / 12, 0);
}

void ImDrawList::PathArcTo(const ImVec2& center, float radius, float a_min, float a_max, int num_segments)
{
    if (radius <= 0.0f)
    {
        _Path.push_back(center);
        return;
    }

    if (num_segments > 0)
    {
        _PathArcToN(center, radius, a_min, a_max, num_segments);
        return;
    }

    // Automatic segment count
    if (radius <= _Data->ArcFastRadiusCutoff)
    {
        const bool a_is_reverse = a_max < a_min;

        // We are going to use precomputed values for mid samples.
        // Determine first and last sample in lookup table that belong to the arc.
        const float a_min_sample_f = IM_DRAWLIST_ARCFAST_SAMPLE_MAX * a_min / (IM_PI * 2.0f);
        const float a_max_sample_f = IM_DRAWLIST_ARCFAST_SAMPLE_MAX * a_max / (IM_PI * 2.0f);

        const int a_min_sample = a_is_reverse ? (int)ImFloorSigned(a_min_sample_f) : (int)ImCeil(a_min_sample_f);
        const int a_max_sample = a_is_reverse ? (int)ImCeil(a_max_sample_f) : (int)ImFloorSigned(a_max_sample_f);
        const int a_mid_samples = a_is_reverse ? ImMax(a_min_sample - a_max_sample, 0) : ImMax(a_max_sample - a_min_sample, 0);

        const float a_min_segment_angle = a_min_sample * IM_PI * 2.0f / IM_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const float a_max_segment_angle = a_max_sample * IM_PI * 2.0f / IM_DRAWLIST_ARCFAST_SAMPLE_MAX;
        const bool a_emit_start = (a_min_segment_angle - a_min) != 0.0f;
        const bool a_emit_end = (a_max - a_max_segment_angle) != 0.0f;

        _Path.reserve(_Path.Size + (a_mid_samples + 1 + (a_emit_start ? 1 : 0) + (a_emit_end ? 1 : 0)));
        if (a_emit_start)
            _Path.push_back(ImVec2(center.x + ImCos(a_min) * radius, center.y + ImSin(a_min) * radius));
        if (a_mid_samples > 0)
            _PathArcToFastEx(center, radius, a_min_sample, a_max_sample, 0);
        if (a_emit_end)
            _Path.push_back(ImVec2(center.x + ImCos(a_max) * radius, center.y + ImSin(a_max) * radius));
    }
    else
    {
        const float arc_length = ImAbs(a_max - a_min);
        const int circle_segment_count = _CalcCircleAutoSegmentCount(radius);
        const int arc_segment_count = ImMax((int)ImCeil(circle_segment_count * arc_length / (IM_PI * 2.0f)), (int)(2.0f * IM_PI / arc_length));
        _PathArcToN(center, radius, a_min, a_max, arc_segment_count);
    }
}

ImVec2 ImBezierCubicCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, float t)
{
    float u = 1.0f - t;
    float w1 = u * u * u;
    float w2 = 3 * u * u * t;
    float w3 = 3 * u * t * t;
    float w4 = t * t * t;
    return ImVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x, w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y);
}

ImVec2 ImBezierQuadraticCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, float t)
{
    float u = 1.0f - t;
    float w1 = u * u;
    float w2 = 2 * u * t;
    float w3 = t * t;
    return ImVec2(w1 * p1.x + w2 * p2.x + w3 * p3.x, w1 * p1.y + w2 * p2.y + w3 * p3.y);
}

// Closely mimics ImBezierCubicClosestPointCasteljau() in imgui.cpp
static void PathBezierCubicCurveToCasteljau(ImVector<ImVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tess_tol, int level)
{
    float dx = x4 - x1;
    float dy = y4 - y1;
    float d2 = (x2 - x4) * dy - (y2 - y4) * dx;
    float d3 = (x3 - x4) * dy - (y3 - y4) * dx;
    d2 = (d2 >= 0) ? d2 : -d2;
    d3 = (d3 >= 0) ? d3 : -d3;
    if ((d2 + d3) * (d2 + d3) < tess_tol * (dx * dx + dy * dy))
    {
        path->push_back(ImVec2(x4, y4));
    }
    else if (level < 10)
    {
        float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
        float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
        float x34 = (x3 + x4) * 0.5f, y34 = (y3 + y4) * 0.5f;
        float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
        float x234 = (x23 + x34) * 0.5f, y234 = (y23 + y34) * 0.5f;
        float x1234 = (x123 + x234) * 0.5f, y1234 = (y123 + y234) * 0.5f;
        PathBezierCubicCurveToCasteljau(path, x1, y1, x12, y12, x123, y123, x1234, y1234, tess_tol, level + 1);
        PathBezierCubicCurveToCasteljau(path, x1234, y1234, x234, y234, x34, y34, x4, y4, tess_tol, level + 1);
    }
}

static void PathBezierQuadraticCurveToCasteljau(ImVector<ImVec2>* path, float x1, float y1, float x2, float y2, float x3, float y3, float tess_tol, int level)
{
    float dx = x3 - x1, dy = y3 - y1;
    float det = (x2 - x3) * dy - (y2 - y3) * dx;
    if (det * det * 4.0f < tess_tol * (dx * dx + dy * dy))
    {
        path->push_back(ImVec2(x3, y3));
    }
    else if (level < 10)
    {
        float x12 = (x1 + x2) * 0.5f, y12 = (y1 + y2) * 0.5f;
        float x23 = (x2 + x3) * 0.5f, y23 = (y2 + y3) * 0.5f;
        float x123 = (x12 + x23) * 0.5f, y123 = (y12 + y23) * 0.5f;
        PathBezierQuadraticCurveToCasteljau(path, x1, y1, x12, y12, x123, y123, tess_tol, level + 1);
        PathBezierQuadraticCurveToCasteljau(path, x123, y123, x23, y23, x3, y3, tess_tol, level + 1);
    }
}

void ImDrawList::PathBezierCubicCurveTo(const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, int num_segments)
{
    ImVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        PathBezierCubicCurveToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, _Data->CurveTessellationTol, 0); // Auto-tessellated
    }
    else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
            _Path.push_back(ImBezierCubicCalc(p1, p2, p3, p4, t_step * i_step));
    }
}

void ImDrawList::PathBezierQuadraticCurveTo(const ImVec2& p2, const ImVec2& p3, int num_segments)
{
    ImVec2 p1 = _Path.back();
    if (num_segments == 0)
    {
        PathBezierQuadraticCurveToCasteljau(&_Path, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, _Data->CurveTessellationTol, 0);// Auto-tessellated
    }
    else
    {
        float t_step = 1.0f / (float)num_segments;
        for (int i_step = 1; i_step <= num_segments; i_step++)
            _Path.push_back(ImBezierQuadraticCalc(p1, p2, p3, t_step * i_step));
    }
}

IM_STATIC_ASSERT(ImDrawFlags_RoundCornersTopLeft == (1 << 4));
static inline ImDrawFlags FixRectCornerFlags(ImDrawFlags flags)
{
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    // Legacy Support for hard coded ~0 (used to be a suggested equivalent to ImDrawCornerFlags_All)
    //   ~0   --> ImDrawFlags_RoundCornersAll or 0
    if (flags == ~0)
        return ImDrawFlags_RoundCornersAll;

    // Legacy Support for hard coded 0x01 to 0x0F (matching 15 out of 16 old flags combinations)
    //   0x01 --> ImDrawFlags_RoundCornersTopLeft (VALUE 0x01 OVERLAPS ImDrawFlags_Closed but ImDrawFlags_Closed is never valid in this path!)
    //   0x02 --> ImDrawFlags_RoundCornersTopRight
    //   0x03 --> ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight
    //   0x04 --> ImDrawFlags_RoundCornersBotLeft
    //   0x05 --> ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersBotLeft
    //   ...
    //   0x0F --> ImDrawFlags_RoundCornersAll or 0
    // (See all values in ImDrawCornerFlags_)
    if (flags >= 0x01 && flags <= 0x0F)
        return (flags << 4);

    // We cannot support hard coded 0x00 with 'float rounding > 0.0f' --> replace with ImDrawFlags_RoundCornersNone or use 'float rounding = 0.0f'
#endif

    // If this triggers, please update your code replacing hardcoded values with new ImDrawFlags_RoundCorners* values.
    // Note that ImDrawFlags_Closed (== 0x01) is an invalid flag for AddRect(), AddRectFilled(), PathRect() etc...
    IM_ASSERT((flags & 0x0F) == 0 && "Misuse of legacy hardcoded ImDrawCornerFlags values!");

    if ((flags & ImDrawFlags_RoundCornersMask_) == 0)
        flags |= ImDrawFlags_RoundCornersAll;

    return flags;
}

void ImDrawList::PathRect(const ImVec2& a, const ImVec2& b, float rounding, ImDrawFlags flags)
{
    flags = FixRectCornerFlags(flags);
    rounding = ImMin(rounding, ImFabs(b.x - a.x) * ( ((flags & ImDrawFlags_RoundCornersTop)  == ImDrawFlags_RoundCornersTop)  || ((flags & ImDrawFlags_RoundCornersBottom) == ImDrawFlags_RoundCornersBottom) ? 0.5f : 1.0f ) - 1.0f);
    rounding = ImMin(rounding, ImFabs(b.y - a.y) * ( ((flags & ImDrawFlags_RoundCornersLeft) == ImDrawFlags_RoundCornersLeft) || ((flags & ImDrawFlags_RoundCornersRight)  == ImDrawFlags_RoundCornersRight)  ? 0.5f : 1.0f ) - 1.0f);

    if (rounding <= 0.0f || (flags & ImDrawFlags_RoundCornersMask_) == ImDrawFlags_RoundCornersNone)
    {
        PathLineTo(a);
        PathLineTo(ImVec2(b.x, a.y));
        PathLineTo(b);
        PathLineTo(ImVec2(a.x, b.y));
    }
    else
    {
        const float rounding_tl = (flags & ImDrawFlags_RoundCornersTopLeft)     ? rounding : 0.0f;
        const float rounding_tr = (flags & ImDrawFlags_RoundCornersTopRight)    ? rounding : 0.0f;
        const float rounding_br = (flags & ImDrawFlags_RoundCornersBottomRight) ? rounding : 0.0f;
        const float rounding_bl = (flags & ImDrawFlags_RoundCornersBottomLeft)  ? rounding : 0.0f;
        PathArcToFast(ImVec2(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
        PathArcToFast(ImVec2(b.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
        PathArcToFast(ImVec2(b.x - rounding_br, b.y - rounding_br), rounding_br, 0, 3);
        PathArcToFast(ImVec2(a.x + rounding_bl, b.y - rounding_bl), rounding_bl, 3, 6);
    }
}

void ImDrawList::AddLine(const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    PathLineTo(p1 + ImVec2(0.5f, 0.5f));
    PathLineTo(p2 + ImVec2(0.5f, 0.5f));
    PathStroke(col, 0, thickness);
}

// p_min = upper-left, p_max = lower-right
// Note we don't render 1 pixels sized rectangles properly.
void ImDrawList::AddRect(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawFlags flags, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    if (Flags & ImDrawListFlags_AntiAliasedLines)
        PathRect(p_min + ImVec2(0.50f, 0.50f), p_max - ImVec2(0.50f, 0.50f), rounding, flags);
    else
        PathRect(p_min + ImVec2(0.50f, 0.50f), p_max - ImVec2(0.49f, 0.49f), rounding, flags); // Better looking lower-right corner and rounded non-AA shapes.
    PathStroke(col, ImDrawFlags_Closed, thickness);
}

void ImDrawList::AddRectFilled(const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding, ImDrawFlags flags)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;
    if (rounding <= 0.0f || (flags & ImDrawFlags_RoundCornersMask_) == ImDrawFlags_RoundCornersNone)
    {
        PrimReserve(6, 4);
        PrimRect(p_min, p_max, col);
    }
    else
    {
        PathRect(p_min, p_max, rounding, flags);
        PathFillConvex(col);
    }
}

// p_min = upper-left, p_max = lower-right
void ImDrawList::AddRectFilledMultiColor(const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left)
{
    if (((col_upr_left | col_upr_right | col_bot_right | col_bot_left) & IM_COL32_A_MASK) == 0)
        return;

    const ImVec2 uv = _Data->TexUvWhitePixel;
    PrimReserve(6, 4);
    PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 1)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 2));
    PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 2)); PrimWriteIdx((ImDrawIdx)(_VtxCurrentIdx + 3));
    PrimWriteVtx(p_min, uv, col_upr_left);
    PrimWriteVtx(ImVec2(p_max.x, p_min.y), uv, col_upr_right);
    PrimWriteVtx(p_max, uv, col_bot_right);
    PrimWriteVtx(ImVec2(p_min.x, p_max.y), uv, col_bot_left);
}

void ImDrawList::AddQuad(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathLineTo(p4);
    PathStroke(col, ImDrawFlags_Closed, thickness);
}

void ImDrawList::AddQuadFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathLineTo(p4);
    PathFillConvex(col);
}

void ImDrawList::AddTriangle(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathStroke(col, ImDrawFlags_Closed, thickness);
}

void ImDrawList::AddTriangleFilled(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathLineTo(p2);
    PathLineTo(p3);
    PathFillConvex(col);
}

void ImDrawList::AddCircle(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0 || radius <= 0.0f)
        return;

    if (num_segments <= 0)
    {
        // Use arc with automatic segment count
        _PathArcToFastEx(center, radius - 0.5f, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
        _Path.Size--;
    }
    else
    {
        // Explicit segment count (still clamp to avoid drawing insanely tessellated shapes)
        num_segments = ImClamp(num_segments, 3, IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);

        // Because we are filling a closed shape we remove 1 from the count of segments/points
        const float a_max = (IM_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        PathArcTo(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
    }

    PathStroke(col, ImDrawFlags_Closed, thickness);
}

void ImDrawList::AddCircleFilled(const ImVec2& center, float radius, ImU32 col, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0 || radius <= 0.0f)
        return;

    if (num_segments <= 0)
    {
        // Use arc with automatic segment count
        _PathArcToFastEx(center, radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
        _Path.Size--;
    }
    else
    {
        // Explicit segment count (still clamp to avoid drawing insanely tessellated shapes)
        num_segments = ImClamp(num_segments, 3, IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX);

        // Because we are filling a closed shape we remove 1 from the count of segments/points
        const float a_max = (IM_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
        PathArcTo(center, radius, 0.0f, a_max, num_segments - 1);
    }

    PathFillConvex(col);
}

// Guaranteed to honor 'num_segments'
void ImDrawList::AddNgon(const ImVec2& center, float radius, ImU32 col, int num_segments, float thickness)
{
    if ((col & IM_COL32_A_MASK) == 0 || num_segments <= 2)
        return;

    // Because we are filling a closed shape we remove 1 from the count of segments/points
    const float a_max = (IM_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(center, radius - 0.5f, 0.0f, a_max, num_segments - 1);
    PathStroke(col, ImDrawFlags_Closed, thickness);
}

// Guaranteed to honor 'num_segments'
void ImDrawList::AddNgonFilled(const ImVec2& center, float radius, ImU32 col, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0 || num_segments <= 2)
        return;

    // Because we are filling a closed shape we remove 1 from the count of segments/points
    const float a_max = (IM_PI * 2.0f) * ((float)num_segments - 1.0f) / (float)num_segments;
    PathArcTo(center, radius, 0.0f, a_max, num_segments - 1);
    PathFillConvex(col);
}

// Cubic Bezier takes 4 controls points
void ImDrawList::AddBezierCubic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, ImU32 col, float thickness, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathBezierCubicCurveTo(p2, p3, p4, num_segments);
    PathStroke(col, 0, thickness);
}

// Quadratic Bezier takes 3 controls points
void ImDrawList::AddBezierQuadratic(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness, int num_segments)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    PathLineTo(p1);
    PathBezierQuadraticCurveTo(p2, p3, num_segments);
    PathStroke(col, 0, thickness);
}

void ImDrawList::AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    if (text_end == NULL)
        text_end = text_begin + strlen(text_begin);
    if (text_begin == text_end)
        return;

    // Pull default font/size from the shared ImDrawListSharedData instance
    if (font == NULL)
        font = _Data->Font;
    if (font_size == 0.0f)
        font_size = _Data->FontSize;

    IM_ASSERT(font->ContainerAtlas->TexID == _CmdHeader.TextureId);  // Use high-level ImGui::PushFont() or low-level ImDrawList::PushTextureId() to change font.

    ImVec4 clip_rect = _CmdHeader.ClipRect;
    if (cpu_fine_clip_rect)
    {
        clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
        clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
        clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
        clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
    }
    font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}

void ImDrawList::AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
    AddText(NULL, 0.0f, pos, col, text_begin, text_end);
}

void ImDrawList::AddImage(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min, const ImVec2& uv_max, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    const bool push_texture_id = user_texture_id != _CmdHeader.TextureId;
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimRectUV(p_min, p_max, uv_min, uv_max, col);

    if (push_texture_id)
        PopTextureID();
}

void ImDrawList::AddImageQuad(ImTextureID user_texture_id, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& uv1, const ImVec2& uv2, const ImVec2& uv3, const ImVec2& uv4, ImU32 col)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    const bool push_texture_id = user_texture_id != _CmdHeader.TextureId;
    if (push_texture_id)
        PushTextureID(user_texture_id);

    PrimReserve(6, 4);
    PrimQuadUV(p1, p2, p3, p4, uv1, uv2, uv3, uv4, col);

    if (push_texture_id)
        PopTextureID();
}

void ImDrawList::AddImageRounded(ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min, const ImVec2& uv_max, ImU32 col, float rounding, ImDrawFlags flags)
{
    if ((col & IM_COL32_A_MASK) == 0)
        return;

    flags = FixRectCornerFlags(flags);
    if (rounding <= 0.0f || (flags & ImDrawFlags_RoundCornersMask_) == ImDrawFlags_RoundCornersNone)
    {
        AddImage(user_texture_id, p_min, p_max, uv_min, uv_max, col);
        return;
    }

    const bool push_texture_id = user_texture_id != _CmdHeader.TextureId;
    if (push_texture_id)
        PushTextureID(user_texture_id);

    int vert_start_idx = VtxBuffer.Size;
    PathRect(p_min, p_max, rounding, flags);
    PathFillConvex(col);
    int vert_end_idx = VtxBuffer.Size;
    ImGui::ShadeVertsLinearUV(this, vert_start_idx, vert_end_idx, p_min, p_max, uv_min, uv_max, true);

    if (push_texture_id)
        PopTextureID();
}


//-----------------------------------------------------------------------------
// [SECTION] ImDrawListSplitter
//-----------------------------------------------------------------------------
// FIXME: This may be a little confusing, trying to be a little too low-level/optimal instead of just doing vector swap..
//-----------------------------------------------------------------------------

void ImDrawListSplitter::ClearFreeMemory()
{
    for (int i = 0; i < _Channels.Size; i++)
    {
        if (i == _Current)
            memset(&_Channels[i], 0, sizeof(_Channels[i]));  // Current channel is a copy of CmdBuffer/IdxBuffer, don't destruct again
        _Channels[i]._CmdBuffer.clear();
        _Channels[i]._IdxBuffer.clear();
    }
    _Current = 0;
    _Count = 1;
    _Channels.clear();
}

void ImDrawListSplitter::Split(ImDrawList* draw_list, int channels_count)
{
    IM_UNUSED(draw_list);
    IM_ASSERT(_Current == 0 && _Count <= 1 && "Nested channel splitting is not supported. Please use separate instances of ImDrawListSplitter.");
    int old_channels_count = _Channels.Size;
    if (old_channels_count < channels_count)
    {
        _Channels.reserve(channels_count); // Avoid over reserving since this is likely to stay stable
        _Channels.resize(channels_count);
    }
    _Count = channels_count;

    // Channels[] (24/32 bytes each) hold storage that we'll swap with draw_list->_CmdBuffer/_IdxBuffer
    // The content of Channels[0] at this point doesn't matter. We clear it to make state tidy in a debugger but we don't strictly need to.
    // When we switch to the next channel, we'll copy draw_list->_CmdBuffer/_IdxBuffer into Channels[0] and then Channels[1] into draw_list->CmdBuffer/_IdxBuffer
    memset(&_Channels[0], 0, sizeof(ImDrawChannel));
    for (int i = 1; i < channels_count; i++)
    {
        if (i >= old_channels_count)
        {
            IM_PLACEMENT_NEW(&_Channels[i]) ImDrawChannel();
        }
        else
        {
            _Channels[i]._CmdBuffer.resize(0);
            _Channels[i]._IdxBuffer.resize(0);
        }
    }
}

void ImDrawListSplitter::Merge(ImDrawList* draw_list)
{
    // Note that we never use or rely on _Channels.Size because it is merely a buffer that we never shrink back to 0 to keep all sub-buffers ready for use.
    if (_Count <= 1)
        return;

    SetCurrentChannel(draw_list, 0);
    draw_list->_PopUnusedDrawCmd();

    // Calculate our final buffer sizes. Also fix the incorrect IdxOffset values in each command.
    int new_cmd_buffer_count = 0;
    int new_idx_buffer_count = 0;
    ImDrawCmd* last_cmd = (_Count > 0 && draw_list->CmdBuffer.Size > 0) ? &draw_list->CmdBuffer.back() : NULL;
    int idx_offset = last_cmd ? last_cmd->IdxOffset + last_cmd->ElemCount : 0;
    for (int i = 1; i < _Count; i++)
    {
        ImDrawChannel& ch = _Channels[i];

        // Equivalent of PopUnusedDrawCmd() for this channel's cmdbuffer and except we don't need to test for UserCallback.
        if (ch._CmdBuffer.Size > 0 && ch._CmdBuffer.back().ElemCount == 0)
            ch._CmdBuffer.pop_back();

        if (ch._CmdBuffer.Size > 0 && last_cmd != NULL)
        {
            ImDrawCmd* next_cmd = &ch._CmdBuffer[0];
            if (ImDrawCmd_HeaderCompare(last_cmd, next_cmd) == 0 && last_cmd->UserCallback == NULL && next_cmd->UserCallback == NULL)
            {
                // Merge previous channel last draw command with current channel first draw command if matching.
                last_cmd->ElemCount += next_cmd->ElemCount;
                idx_offset += next_cmd->ElemCount;
                ch._CmdBuffer.erase(ch._CmdBuffer.Data); // FIXME-OPT: Improve for multiple merges.
            }
        }
        if (ch._CmdBuffer.Size > 0)
            last_cmd = &ch._CmdBuffer.back();
        new_cmd_buffer_count += ch._CmdBuffer.Size;
        new_idx_buffer_count += ch._IdxBuffer.Size;
        for (int cmd_n = 0; cmd_n < ch._CmdBuffer.Size; cmd_n++)
        {
            ch._CmdBuffer.Data[cmd_n].IdxOffset = idx_offset;
            idx_offset += ch._CmdBuffer.Data[cmd_n].ElemCount;
        }
    }
    draw_list->CmdBuffer.resize(draw_list->CmdBuffer.Size + new_cmd_buffer_count);
    draw_list->IdxBuffer.resize(draw_list->IdxBuffer.Size + new_idx_buffer_count);

    // Write commands and indices in order (they are fairly small structures, we don't copy vertices only indices)
    ImDrawCmd* cmd_write = draw_list->CmdBuffer.Data + draw_list->CmdBuffer.Size - new_cmd_buffer_count;
    ImDrawIdx* idx_write = draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size - new_idx_buffer_count;
    for (int i = 1; i < _Count; i++)
    {
        ImDrawChannel& ch = _Channels[i];
        if (int sz = ch._CmdBuffer.Size) { memcpy(cmd_write, ch._CmdBuffer.Data, sz * sizeof(ImDrawCmd)); cmd_write += sz; }
        if (int sz = ch._IdxBuffer.Size) { memcpy(idx_write, ch._IdxBuffer.Data, sz * sizeof(ImDrawIdx)); idx_write += sz; }
    }
    draw_list->_IdxWritePtr = idx_write;

    // Ensure there's always a non-callback draw command trailing the command-buffer
    if (draw_list->CmdBuffer.Size == 0 || draw_list->CmdBuffer.back().UserCallback != NULL)
        draw_list->AddDrawCmd();

    // If current command is used with different settings we need to add a new command
    ImDrawCmd* curr_cmd = &draw_list->CmdBuffer.Data[draw_list->CmdBuffer.Size - 1];
    if (curr_cmd->ElemCount == 0)
        ImDrawCmd_HeaderCopy(curr_cmd, &draw_list->_CmdHeader); // Copy ClipRect, TextureId, VtxOffset
    else if (ImDrawCmd_HeaderCompare(curr_cmd, &draw_list->_CmdHeader) != 0)
        draw_list->AddDrawCmd();

    _Count = 1;
}

void ImDrawListSplitter::SetCurrentChannel(ImDrawList* draw_list, int idx)
{
    IM_ASSERT(idx >= 0 && idx < _Count);
    if (_Current == idx)
        return;

    // Overwrite ImVector (12/16 bytes), four times. This is merely a silly optimization instead of doing .swap()
    memcpy(&_Channels.Data[_Current]._CmdBuffer, &draw_list->CmdBuffer, sizeof(draw_list->CmdBuffer));
    memcpy(&_Channels.Data[_Current]._IdxBuffer, &draw_list->IdxBuffer, sizeof(draw_list->IdxBuffer));
    _Current = idx;
    memcpy(&draw_list->CmdBuffer, &_Channels.Data[idx]._CmdBuffer, sizeof(draw_list->CmdBuffer));
    memcpy(&draw_list->IdxBuffer, &_Channels.Data[idx]._IdxBuffer, sizeof(draw_list->IdxBuffer));
    draw_list->_IdxWritePtr = draw_list->IdxBuffer.Data + draw_list->IdxBuffer.Size;

    // If current command is used with different settings we need to add a new command
    ImDrawCmd* curr_cmd = (draw_list->CmdBuffer.Size == 0) ? NULL : &draw_list->CmdBuffer.Data[draw_list->CmdBuffer.Size - 1];
    if (curr_cmd == NULL)
        draw_list->AddDrawCmd();
    else if (curr_cmd->ElemCount == 0)
        ImDrawCmd_HeaderCopy(curr_cmd, &draw_list->_CmdHeader); // Copy ClipRect, TextureId, VtxOffset
    else if (ImDrawCmd_HeaderCompare(curr_cmd, &draw_list->_CmdHeader) != 0)
        draw_list->AddDrawCmd();
}

//-----------------------------------------------------------------------------
// [SECTION] ImDrawData
//-----------------------------------------------------------------------------

// For backward compatibility: convert all buffers from indexed to de-indexed, in case you cannot render indexed. Note: this is slow and most likely a waste of resources. Always prefer indexed rendering!
void ImDrawData::DeIndexAllBuffers()
{
    ImVector<ImDrawVert> new_vtx_buffer;
    TotalVtxCount = TotalIdxCount = 0;
    for (int i = 0; i < CmdListsCount; i++)
    {
        ImDrawList* cmd_list = CmdLists[i];
        if (cmd_list->IdxBuffer.empty())
            continue;
        new_vtx_buffer.resize(cmd_list->IdxBuffer.Size);
        for (int j = 0; j < cmd_list->IdxBuffer.Size; j++)
            new_vtx_buffer[j] = cmd_list->VtxBuffer[cmd_list->IdxBuffer[j]];
        cmd_list->VtxBuffer.swap(new_vtx_buffer);
        cmd_list->IdxBuffer.resize(0);
        TotalVtxCount += cmd_list->VtxBuffer.Size;
    }
}

// Helper to scale the ClipRect field of each ImDrawCmd.
// Use if your final output buffer is at a different scale than draw_data->DisplaySize,
// or if there is a difference between your window resolution and framebuffer resolution.
void ImDrawData::ScaleClipRects(const ImVec2& fb_scale)
{
    for (int i = 0; i < CmdListsCount; i++)
    {
        ImDrawList* cmd_list = CmdLists[i];
        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            ImDrawCmd* cmd = &cmd_list->CmdBuffer[cmd_i];
            cmd->ClipRect = ImVec4(cmd->ClipRect.x * fb_scale.x, cmd->ClipRect.y * fb_scale.y, cmd->ClipRect.z * fb_scale.x, cmd->ClipRect.w * fb_scale.y);
        }
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Helpers ShadeVertsXXX functions
//-----------------------------------------------------------------------------

// Generic linear color gradient, write to RGB fields, leave A untouched.
void ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
{
    ImVec2 gradient_extent = gradient_p1 - gradient_p0;
    float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
    const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
    const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
    const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
    const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
    const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
    for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
    {
        float d = ImDot(vert->pos - gradient_p0, gradient_extent);
        float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        int r = (int)(col0_r + col_delta_r * t);
        int g = (int)(col0_g + col_delta_g * t);
        int b = (int)(col0_b + col_delta_b * t);
        vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (vert->col & IM_COL32_A_MASK);
    }
}

// Distribute UV over (a, b) rectangle
void ImGui::ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp)
{
    const ImVec2 size = b - a;
    const ImVec2 uv_size = uv_b - uv_a;
    const ImVec2 scale = ImVec2(
        size.x != 0.0f ? (uv_size.x / size.x) : 0.0f,
        size.y != 0.0f ? (uv_size.y / size.y) : 0.0f);

    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    if (clamp)
    {
        const ImVec2 min = ImMin(uv_a, uv_b);
        const ImVec2 max = ImMax(uv_a, uv_b);
        for (ImDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = ImClamp(uv_a + ImMul(ImVec2(vertex->pos.x, vertex->pos.y) - a, scale), min, max);
    }
    else
    {
        for (ImDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
            vertex->uv = uv_a + ImMul(ImVec2(vertex->pos.x, vertex->pos.y) - a, scale);
    }
}

//-----------------------------------------------------------------------------
// [SECTION] ImFontConfig
//-----------------------------------------------------------------------------

ImFontConfig::ImFontConfig()
{
    memset(this, 0, sizeof(*this));
    FontDataOwnedByAtlas = true;
    OversampleH = 3; // FIXME: 2 may be a better default?
    OversampleV = 1;
    GlyphMaxAdvanceX = FLT_MAX;
    RasterizerMultiply = 1.0f;
    EllipsisChar = (ImWchar)-1;
}

//-----------------------------------------------------------------------------
// [SECTION] ImFontAtlas
//-----------------------------------------------------------------------------

// A work of art lies ahead! (. = white layer, X = black layer, others are blank)
// The 2x2 white texels on the top left are the ones we'll use everywhere in Dear ImGui to render filled shapes.
const int FONT_ATLAS_DEFAULT_TEX_DATA_W = 108; // Actual texture will be 2 times that + 1 spacing.
const int FONT_ATLAS_DEFAULT_TEX_DATA_H = 27;
static const char FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS[FONT_ATLAS_DEFAULT_TEX_DATA_W * FONT_ATLAS_DEFAULT_TEX_DATA_H + 1] =
{
    "..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX-     XX          "
    "..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X-    X..X         "
    "---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X-    X..X         "
    "X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X-    X..X         "
    "XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X-    X..X         "
    "X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X-    X..XXX       "
    "X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX-    X..X..XXX    "
    "X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      -    X..X..X..XX  "
    "X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       -    X..X..X..X.X "
    "X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        -XXX X..X..X..X..X"
    "X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         -X..XX........X..X"
    "X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          -X...X...........X"
    "X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           - X..............X"
    "X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            -  X.............X"
    "X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           -  X.............X"
    "X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          -   X............X"
    "X...X..X    ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          -   X...........X "
    "X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       -------------------------------------    X..........X "
    "X.X  X..X   -       -X.......X-       X.......X       -    XX           XX    -           -    X..........X "
    "XX    X..X  -       - X.....X -        X.....X        -   X.X           X.X   -           -     X........X  "
    "      X..X          -  X...X  -         X...X         -  X..X           X..X  -           -     X........X  "
    "       XX           -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           -     XXXXXXXXXX  "
    "------------        -    X    -           X           -X.....................X-           ------------------"
    "                    ----------------------------------- X...XXXXXXXXXXXXX...X -                             "
    "                                                      -  X..X           X..X  -                             "
    "                                                      -   X.X           X.X   -                             "
    "                                                      -    XX           XX    -                             "
};

static const ImVec2 FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[ImGuiMouseCursor_COUNT][3] =
{
    // Pos ........ Size ......... Offset ......
    { ImVec2( 0,3), ImVec2(12,19), ImVec2( 0, 0) }, // ImGuiMouseCursor_Arrow
    { ImVec2(13,0), ImVec2( 7,16), ImVec2( 1, 8) }, // ImGuiMouseCursor_TextInput
    { ImVec2(31,0), ImVec2(23,23), ImVec2(11,11) }, // ImGuiMouseCursor_ResizeAll
    { ImVec2(21,0), ImVec2( 9,23), ImVec2( 4,11) }, // ImGuiMouseCursor_ResizeNS
    { ImVec2(55,18),ImVec2(23, 9), ImVec2(11, 4) }, // ImGuiMouseCursor_ResizeEW
    { ImVec2(73,0), ImVec2(17,17), ImVec2( 8, 8) }, // ImGuiMouseCursor_ResizeNESW
    { ImVec2(55,0), ImVec2(17,17), ImVec2( 8, 8) }, // ImGuiMouseCursor_ResizeNWSE
    { ImVec2(91,0), ImVec2(17,22), ImVec2( 5, 0) }, // ImGuiMouseCursor_Hand
};

ImFontAtlas::ImFontAtlas()
{
    memset(this, 0, sizeof(*this));
    TexGlyphPadding = 1;
    PackIdMouseCursors = PackIdLines = -1;
}

ImFontAtlas::~ImFontAtlas()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    Clear();
}

void    ImFontAtlas::ClearInputData()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    for (int i = 0; i < ConfigData.Size; i++)
        if (ConfigData[i].FontData && ConfigData[i].FontDataOwnedByAtlas)
        {
            IM_FREE(ConfigData[i].FontData);
            ConfigData[i].FontData = NULL;
        }

    // When clearing this we lose access to the font name and other information used to build the font.
    for (int i = 0; i < Fonts.Size; i++)
        if (Fonts[i]->ConfigData >= ConfigData.Data && Fonts[i]->ConfigData < ConfigData.Data + ConfigData.Size)
        {
            Fonts[i]->ConfigData = NULL;
            Fonts[i]->ConfigDataCount = 0;
        }
    ConfigData.clear();
    CustomRects.clear();
    PackIdMouseCursors = PackIdLines = -1;
    // Important: we leave TexReady untouched
}

void    ImFontAtlas::ClearTexData()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    if (TexPixelsAlpha8)
        IM_FREE(TexPixelsAlpha8);
    if (TexPixelsRGBA32)
        IM_FREE(TexPixelsRGBA32);
    TexPixelsAlpha8 = NULL;
    TexPixelsRGBA32 = NULL;
    TexPixelsUseColors = false;
    // Important: we leave TexReady untouched
}

void    ImFontAtlas::ClearFonts()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    Fonts.clear_delete();
    TexReady = false;
}

void    ImFontAtlas::Clear()
{
    ClearInputData();
    ClearTexData();
    ClearFonts();
}

void    ImFontAtlas::GetTexDataAsAlpha8(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
    // Build atlas on demand
    if (TexPixelsAlpha8 == NULL)
        Build();

    *out_pixels = TexPixelsAlpha8;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
}

void    ImFontAtlas::GetTexDataAsRGBA32(unsigned char** out_pixels, int* out_width, int* out_height, int* out_bytes_per_pixel)
{
    // Convert to RGBA32 format on demand
    // Although it is likely to be the most commonly used format, our font rendering is 1 channel / 8 bpp
    if (!TexPixelsRGBA32)
    {
        unsigned char* pixels = NULL;
        GetTexDataAsAlpha8(&pixels, NULL, NULL);
        if (pixels)
        {
            TexPixelsRGBA32 = (unsigned int*)IM_ALLOC((size_t)TexWidth * (size_t)TexHeight * 4);
            const unsigned char* src = pixels;
            unsigned int* dst = TexPixelsRGBA32;
            for (int n = TexWidth * TexHeight; n > 0; n--)
                *dst++ = IM_COL32(255, 255, 255, (unsigned int)(*src++));
        }
    }

    *out_pixels = (unsigned char*)TexPixelsRGBA32;
    if (out_width) *out_width = TexWidth;
    if (out_height) *out_height = TexHeight;
    if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
}

ImFont* ImFontAtlas::AddFont(const ImFontConfig* font_cfg)
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    IM_ASSERT(font_cfg->FontData != NULL && font_cfg->FontDataSize > 0);
    IM_ASSERT(font_cfg->SizePixels > 0.0f);

    // Create new font
    if (!font_cfg->MergeMode)
        Fonts.push_back(IM_NEW(ImFont));
    else
        IM_ASSERT(!Fonts.empty() && "Cannot use MergeMode for the first font"); // When using MergeMode make sure that a font has already been added before. You can use ImGui::GetIO().Fonts->AddFontDefault() to add the default imgui font.

    ConfigData.push_back(*font_cfg);
    ImFontConfig& new_font_cfg = ConfigData.back();
    if (new_font_cfg.DstFont == NULL)
        new_font_cfg.DstFont = Fonts.back();
    if (!new_font_cfg.FontDataOwnedByAtlas)
    {
        new_font_cfg.FontData = IM_ALLOC(new_font_cfg.FontDataSize);
        new_font_cfg.FontDataOwnedByAtlas = true;
        memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
    }

    if (new_font_cfg.DstFont->EllipsisChar == (ImWchar)-1)
        new_font_cfg.DstFont->EllipsisChar = font_cfg->EllipsisChar;

    // Invalidate texture
    TexReady = false;
    ClearTexData();
    return new_font_cfg.DstFont;
}

// Default font TTF is compressed with stb_compress then base85 encoded (see misc/fonts/binary_to_compressed_c.cpp for encoder)
static unsigned int stb_decompress_length(const unsigned char* input);
static unsigned int stb_decompress(unsigned char* output, const unsigned char* input, unsigned int length);
static const char*  GetDefaultCompressedFontDataTTFBase85();
static unsigned int Decode85Byte(char c)                                    { return c >= '\\' ? c-36 : c-35; }
static void         Decode85(const unsigned char* src, unsigned char* dst)
{
    while (*src)
    {
        unsigned int tmp = Decode85Byte(src[0]) + 85 * (Decode85Byte(src[1]) + 85 * (Decode85Byte(src[2]) + 85 * (Decode85Byte(src[3]) + 85 * Decode85Byte(src[4]))));
        dst[0] = ((tmp >> 0) & 0xFF); dst[1] = ((tmp >> 8) & 0xFF); dst[2] = ((tmp >> 16) & 0xFF); dst[3] = ((tmp >> 24) & 0xFF);   // We can't assume little-endianness.
        src += 5;
        dst += 4;
    }
}

// Load embedded ProggyClean.ttf at size 13, disable oversampling
ImFont* ImFontAtlas::AddFontDefault(const ImFontConfig* font_cfg_template)
{
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    if (!font_cfg_template)
    {
        font_cfg.OversampleH = font_cfg.OversampleV = 1;
        font_cfg.PixelSnapH = true;
    }
    if (font_cfg.SizePixels <= 0.0f)
        font_cfg.SizePixels = 13.0f * 1.0f;
    if (font_cfg.Name[0] == '\0')
        ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "ProggyClean.ttf, %dpx", (int)font_cfg.SizePixels);
    font_cfg.EllipsisChar = (ImWchar)0x0085;
    font_cfg.GlyphOffset.y = 1.0f * IM_FLOOR(font_cfg.SizePixels / 13.0f);  // Add +1 offset per 13 units

    const char* ttf_compressed_base85 = GetDefaultCompressedFontDataTTFBase85();
    const ImWchar* glyph_ranges = font_cfg.GlyphRanges != NULL ? font_cfg.GlyphRanges : GetGlyphRangesDefault();
    ImFont* font = AddFontFromMemoryCompressedBase85TTF(ttf_compressed_base85, font_cfg.SizePixels, &font_cfg, glyph_ranges);
    return font;
}

ImFont* ImFontAtlas::AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    size_t data_size = 0;
    void* data = ImFileLoadToMemory(filename, "rb", &data_size, 0);
    if (!data)
    {
        IM_ASSERT_USER_ERROR(0, "Could not load font file!");
        return NULL;
    }
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    if (font_cfg.Name[0] == '\0')
    {
        // Store a short copy of filename into into the font name for convenience
        const char* p;
        for (p = filename + strlen(filename); p > filename && p[-1] != '/' && p[-1] != '\\'; p--) {}
        ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "%s, %.0fpx", p, size_pixels);
    }
    return AddFontFromMemoryTTF(data, (int)data_size, size_pixels, &font_cfg, glyph_ranges);
}

// NB: Transfer ownership of 'ttf_data' to ImFontAtlas, unless font_cfg_template->FontDataOwnedByAtlas == false. Owned TTF buffer will be deleted after Build().
ImFont* ImFontAtlas::AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    IM_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontData = ttf_data;
    font_cfg.FontDataSize = ttf_size;
    font_cfg.SizePixels = size_pixels > 0.0f ? size_pixels : font_cfg.SizePixels;
    if (glyph_ranges)
        font_cfg.GlyphRanges = glyph_ranges;
    return AddFont(&font_cfg);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedTTF(const void* compressed_ttf_data, int compressed_ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges)
{
    const unsigned int buf_decompressed_size = stb_decompress_length((const unsigned char*)compressed_ttf_data);
    unsigned char* buf_decompressed_data = (unsigned char*)IM_ALLOC(buf_decompressed_size);
    stb_decompress(buf_decompressed_data, (const unsigned char*)compressed_ttf_data, (unsigned int)compressed_ttf_size);

    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
    IM_ASSERT(font_cfg.FontData == NULL);
    font_cfg.FontDataOwnedByAtlas = true;
    return AddFontFromMemoryTTF(buf_decompressed_data, (int)buf_decompressed_size, size_pixels, &font_cfg, glyph_ranges);
}

ImFont* ImFontAtlas::AddFontFromMemoryCompressedBase85TTF(const char* compressed_ttf_data_base85, float size_pixels, const ImFontConfig* font_cfg, const ImWchar* glyph_ranges)
{
    int compressed_ttf_size = (((int)strlen(compressed_ttf_data_base85) + 4) / 5) * 4;
    void* compressed_ttf = IM_ALLOC((size_t)compressed_ttf_size);
    Decode85((const unsigned char*)compressed_ttf_data_base85, (unsigned char*)compressed_ttf);
    ImFont* font = AddFontFromMemoryCompressedTTF(compressed_ttf, compressed_ttf_size, size_pixels, font_cfg, glyph_ranges);
    IM_FREE(compressed_ttf);
    return font;
}

int ImFontAtlas::AddCustomRectRegular(int width, int height)
{
    IM_ASSERT(width > 0 && width <= 0xFFFF);
    IM_ASSERT(height > 0 && height <= 0xFFFF);
    ImFontAtlasCustomRect r;
    r.Width = (unsigned short)width;
    r.Height = (unsigned short)height;
    CustomRects.push_back(r);
    return CustomRects.Size - 1; // Return index
}

int ImFontAtlas::AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset)
{
#ifdef IMGUI_USE_WCHAR32
    IM_ASSERT(id <= IM_UNICODE_CODEPOINT_MAX);
#endif
    IM_ASSERT(font != NULL);
    IM_ASSERT(width > 0 && width <= 0xFFFF);
    IM_ASSERT(height > 0 && height <= 0xFFFF);
    ImFontAtlasCustomRect r;
    r.Width = (unsigned short)width;
    r.Height = (unsigned short)height;
    r.GlyphID = id;
    r.GlyphAdvanceX = advance_x;
    r.GlyphOffset = offset;
    r.Font = font;
    CustomRects.push_back(r);
    return CustomRects.Size - 1; // Return index
}

void ImFontAtlas::CalcCustomRectUV(const ImFontAtlasCustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max) const
{
    IM_ASSERT(TexWidth > 0 && TexHeight > 0);   // Font atlas needs to be built before we can calculate UV coordinates
    IM_ASSERT(rect->IsPacked());                // Make sure the rectangle has been packed
    *out_uv_min = ImVec2((float)rect->X * TexUvScale.x, (float)rect->Y * TexUvScale.y);
    *out_uv_max = ImVec2((float)(rect->X + rect->Width) * TexUvScale.x, (float)(rect->Y + rect->Height) * TexUvScale.y);
}

bool ImFontAtlas::GetMouseCursorTexData(ImGuiMouseCursor cursor_type, ImVec2* out_offset, ImVec2* out_size, ImVec2 out_uv_border[2], ImVec2 out_uv_fill[2])
{
    if (cursor_type <= ImGuiMouseCursor_None || cursor_type >= ImGuiMouseCursor_COUNT)
        return false;
    if (Flags & ImFontAtlasFlags_NoMouseCursors)
        return false;

    IM_ASSERT(PackIdMouseCursors != -1);
    ImFontAtlasCustomRect* r = GetCustomRectByIndex(PackIdMouseCursors);
    ImVec2 pos = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][0] + ImVec2((float)r->X, (float)r->Y);
    ImVec2 size = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][1];
    *out_size = size;
    *out_offset = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[cursor_type][2];
    out_uv_border[0] = (pos) * TexUvScale;
    out_uv_border[1] = (pos + size) * TexUvScale;
    pos.x += FONT_ATLAS_DEFAULT_TEX_DATA_W + 1;
    out_uv_fill[0] = (pos) * TexUvScale;
    out_uv_fill[1] = (pos + size) * TexUvScale;
    return true;
}

bool    ImFontAtlas::Build()
{
    IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");

    // Default font is none are specified
    if (ConfigData.Size == 0)
        AddFontDefault();

    // Select builder
    // - Note that we do not reassign to atlas->FontBuilderIO, since it is likely to point to static data which
    //   may mess with some hot-reloading schemes. If you need to assign to this (for dynamic selection) AND are
    //   using a hot-reloading scheme that messes up static data, store your own instance of ImFontBuilderIO somewhere
    //   and point to it instead of pointing directly to return value of the GetBuilderXXX functions.
    const ImFontBuilderIO* builder_io = FontBuilderIO;
    if (builder_io == NULL)
    {
#ifdef IMGUI_ENABLE_FREETYPE
        builder_io = ImGuiFreeType::GetBuilderForFreeType();
#elif defined(IMGUI_ENABLE_STB_TRUETYPE)
        builder_io = ImFontAtlasGetBuilderForStbTruetype();
#else
        IM_ASSERT(0); // Invalid Build function
#endif
    }

    // Build
    return builder_io->FontBuilder_Build(this);
}

void    ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_brighten_factor)
{
    for (unsigned int i = 0; i < 256; i++)
    {
        unsigned int value = (unsigned int)(i * in_brighten_factor);
        out_table[i] = value > 255 ? 255 : (value & 0xFF);
    }
}

void    ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride)
{
    unsigned char* data = pixels + x + y * stride;
    for (int j = h; j > 0; j--, data += stride)
        for (int i = 0; i < w; i++)
            data[i] = table[data[i]];
}

#ifdef IMGUI_ENABLE_STB_TRUETYPE
// Temporary data for one source font (multiple source fonts can be merged into one destination ImFont)
// (C++03 doesn't allow instancing ImVector<> with function-local types so we declare the type here.)
struct ImFontBuildSrcData
{
    stbtt_fontinfo      FontInfo;
    stbtt_pack_range    PackRange;          // Hold the list of codepoints to pack (essentially points to Codepoints.Data)
    stbrp_rect*         Rects;              // Rectangle to pack. We first fill in their size and the packer will give us their position.
    stbtt_packedchar*   PackedChars;        // Output glyphs
    const ImWchar*      SrcRanges;          // Ranges as requested by user (user is allowed to request too much, e.g. 0x0020..0xFFFF)
    int                 DstIndex;           // Index into atlas->Fonts[] and dst_tmp_array[]
    int                 GlyphsHighest;      // Highest requested codepoint
    int                 GlyphsCount;        // Glyph count (excluding missing glyphs and glyphs already set by an earlier source font)
    ImBitVector         GlyphsSet;          // Glyph bit map (random access, 1-bit per codepoint. This will be a maximum of 8KB)
    ImVector<int>       GlyphsList;         // Glyph codepoints list (flattened version of GlyphsMap)
};

// Temporary data for one destination ImFont* (multiple source fonts can be merged into one destination ImFont)
struct ImFontBuildDstData
{
    int                 SrcCount;           // Number of source fonts targeting this destination font.
    int                 GlyphsHighest;
    int                 GlyphsCount;
    ImBitVector         GlyphsSet;          // This is used to resolve collision when multiple sources are merged into a same destination font.
};

static void UnpackBitVectorToFlatIndexList(const ImBitVector* in, ImVector<int>* out)
{
    IM_ASSERT(sizeof(in->Storage.Data[0]) == sizeof(int));
    const ImU32* it_begin = in->Storage.begin();
    const ImU32* it_end = in->Storage.end();
    for (const ImU32* it = it_begin; it < it_end; it++)
        if (ImU32 entries_32 = *it)
            for (ImU32 bit_n = 0; bit_n < 32; bit_n++)
                if (entries_32 & ((ImU32)1 << bit_n))
                    out->push_back((int)(((it - it_begin) << 5) + bit_n));
}

static bool ImFontAtlasBuildWithStbTruetype(ImFontAtlas* atlas)
{
    IM_ASSERT(atlas->ConfigData.Size > 0);

    ImFontAtlasBuildInit(atlas);

    // Clear atlas
    atlas->TexID = (ImTextureID)NULL;
    atlas->TexWidth = atlas->TexHeight = 0;
    atlas->TexUvScale = ImVec2(0.0f, 0.0f);
    atlas->TexUvWhitePixel = ImVec2(0.0f, 0.0f);
    atlas->ClearTexData();

    // Temporary storage for building
    ImVector<ImFontBuildSrcData> src_tmp_array;
    ImVector<ImFontBuildDstData> dst_tmp_array;
    src_tmp_array.resize(atlas->ConfigData.Size);
    dst_tmp_array.resize(atlas->Fonts.Size);
    memset(src_tmp_array.Data, 0, (size_t)src_tmp_array.size_in_bytes());
    memset(dst_tmp_array.Data, 0, (size_t)dst_tmp_array.size_in_bytes());

    // 1. Initialize font loading structure, check font data validity
    for (int src_i = 0; src_i < atlas->ConfigData.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        IM_ASSERT(cfg.DstFont && (!cfg.DstFont->IsLoaded() || cfg.DstFont->ContainerAtlas == atlas));

        // Find index from cfg.DstFont (we allow the user to set cfg.DstFont. Also it makes casual debugging nicer than when storing indices)
        src_tmp.DstIndex = -1;
        for (int output_i = 0; output_i < atlas->Fonts.Size && src_tmp.DstIndex == -1; output_i++)
            if (cfg.DstFont == atlas->Fonts[output_i])
                src_tmp.DstIndex = output_i;
        if (src_tmp.DstIndex == -1)
        {
            IM_ASSERT(src_tmp.DstIndex != -1); // cfg.DstFont not pointing within atlas->Fonts[] array?
            return false;
        }
        // Initialize helper structure for font loading and verify that the TTF/OTF data is correct
        const int font_offset = stbtt_GetFontOffsetForIndex((unsigned char*)cfg.FontData, cfg.FontNo);
        IM_ASSERT(font_offset >= 0 && "FontData is incorrect, or FontNo cannot be found.");
        if (!stbtt_InitFont(&src_tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset))
            return false;

        // Measure highest codepoints
        ImFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.SrcRanges = cfg.GlyphRanges ? cfg.GlyphRanges : atlas->GetGlyphRangesDefault();
        for (const ImWchar* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
            src_tmp.GlyphsHighest = ImMax(src_tmp.GlyphsHighest, (int)src_range[1]);
        dst_tmp.SrcCount++;
        dst_tmp.GlyphsHighest = ImMax(dst_tmp.GlyphsHighest, src_tmp.GlyphsHighest);
    }

    // 2. For every requested codepoint, check for their presence in the font data, and handle redundancy or overlaps between source fonts to avoid unused glyphs.
    int total_glyphs_count = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        ImFontBuildDstData& dst_tmp = dst_tmp_array[src_tmp.DstIndex];
        src_tmp.GlyphsSet.Create(src_tmp.GlyphsHighest + 1);
        if (dst_tmp.GlyphsSet.Storage.empty())
            dst_tmp.GlyphsSet.Create(dst_tmp.GlyphsHighest + 1);

        for (const ImWchar* src_range = src_tmp.SrcRanges; src_range[0] && src_range[1]; src_range += 2)
            for (unsigned int codepoint = src_range[0]; codepoint <= src_range[1]; codepoint++)
            {
                if (dst_tmp.GlyphsSet.TestBit(codepoint))    // Don't overwrite existing glyphs. We could make this an option for MergeMode (e.g. MergeOverwrite==true)
                    continue;
                if (!stbtt_FindGlyphIndex(&src_tmp.FontInfo, codepoint))    // It is actually in the font?
                    continue;

                // Add to avail set/counters
                src_tmp.GlyphsCount++;
                dst_tmp.GlyphsCount++;
                src_tmp.GlyphsSet.SetBit(codepoint);
                dst_tmp.GlyphsSet.SetBit(codepoint);
                total_glyphs_count++;
            }
    }

    // 3. Unpack our bit map into a flat list (we now have all the Unicode points that we know are requested _and_ available _and_ not overlapping another)
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        src_tmp.GlyphsList.reserve(src_tmp.GlyphsCount);
        UnpackBitVectorToFlatIndexList(&src_tmp.GlyphsSet, &src_tmp.GlyphsList);
        src_tmp.GlyphsSet.Clear();
        IM_ASSERT(src_tmp.GlyphsList.Size == src_tmp.GlyphsCount);
    }
    for (int dst_i = 0; dst_i < dst_tmp_array.Size; dst_i++)
        dst_tmp_array[dst_i].GlyphsSet.Clear();
    dst_tmp_array.clear();

    // Allocate packing character data and flag packed characters buffer as non-packed (x0=y0=x1=y1=0)
    // (We technically don't need to zero-clear buf_rects, but let's do it for the sake of sanity)
    ImVector<stbrp_rect> buf_rects;
    ImVector<stbtt_packedchar> buf_packedchars;
    buf_rects.resize(total_glyphs_count);
    buf_packedchars.resize(total_glyphs_count);
    memset(buf_rects.Data, 0, (size_t)buf_rects.size_in_bytes());
    memset(buf_packedchars.Data, 0, (size_t)buf_packedchars.size_in_bytes());

    // 4. Gather glyphs sizes so we can pack them in our virtual canvas.
    int total_surface = 0;
    int buf_rects_out_n = 0;
    int buf_packedchars_out_n = 0;
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        src_tmp.Rects = &buf_rects[buf_rects_out_n];
        src_tmp.PackedChars = &buf_packedchars[buf_packedchars_out_n];
        buf_rects_out_n += src_tmp.GlyphsCount;
        buf_packedchars_out_n += src_tmp.GlyphsCount;

        // Convert our ranges in the format stb_truetype wants
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        src_tmp.PackRange.font_size = cfg.SizePixels;
        src_tmp.PackRange.first_unicode_codepoint_in_range = 0;
        src_tmp.PackRange.array_of_unicode_codepoints = src_tmp.GlyphsList.Data;
        src_tmp.PackRange.num_chars = src_tmp.GlyphsList.Size;
        src_tmp.PackRange.chardata_for_range = src_tmp.PackedChars;
        src_tmp.PackRange.h_oversample = (unsigned char)cfg.OversampleH;
        src_tmp.PackRange.v_oversample = (unsigned char)cfg.OversampleV;

        // Gather the sizes of all rectangles we will need to pack (this loop is based on stbtt_PackFontRangesGatherRects)
        const float scale = (cfg.SizePixels > 0) ? stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels) : stbtt_ScaleForMappingEmToPixels(&src_tmp.FontInfo, -cfg.SizePixels);
        const int padding = atlas->TexGlyphPadding;
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsList.Size; glyph_i++)
        {
            int x0, y0, x1, y1;
            const int glyph_index_in_font = stbtt_FindGlyphIndex(&src_tmp.FontInfo, src_tmp.GlyphsList[glyph_i]);
            IM_ASSERT(glyph_index_in_font != 0);
            stbtt_GetGlyphBitmapBoxSubpixel(&src_tmp.FontInfo, glyph_index_in_font, scale * cfg.OversampleH, scale * cfg.OversampleV, 0, 0, &x0, &y0, &x1, &y1);
            src_tmp.Rects[glyph_i].w = (stbrp_coord)(x1 - x0 + padding + cfg.OversampleH - 1);
            src_tmp.Rects[glyph_i].h = (stbrp_coord)(y1 - y0 + padding + cfg.OversampleV - 1);
            total_surface += src_tmp.Rects[glyph_i].w * src_tmp.Rects[glyph_i].h;
        }
    }

    // We need a width for the skyline algorithm, any width!
    // The exact width doesn't really matter much, but some API/GPU have texture size limitations and increasing width can decrease height.
    // User can override TexDesiredWidth and TexGlyphPadding if they wish, otherwise we use a simple heuristic to select the width based on expected surface.
    const int surface_sqrt = (int)ImSqrt((float)total_surface) + 1;
    atlas->TexHeight = 0;
    if (atlas->TexDesiredWidth > 0)
        atlas->TexWidth = atlas->TexDesiredWidth;
    else
        atlas->TexWidth = (surface_sqrt >= 4096 * 0.7f) ? 4096 : (surface_sqrt >= 2048 * 0.7f) ? 2048 : (surface_sqrt >= 1024 * 0.7f) ? 1024 : 512;

    // 5. Start packing
    // Pack our extra data rectangles first, so it will be on the upper-left corner of our texture (UV will have small values).
    const int TEX_HEIGHT_MAX = 1024 * 32;
    stbtt_pack_context spc = {};
    stbtt_PackBegin(&spc, NULL, atlas->TexWidth, TEX_HEIGHT_MAX, 0, atlas->TexGlyphPadding, NULL);
    ImFontAtlasBuildPackCustomRects(atlas, spc.pack_info);

    // 6. Pack each source font. No rendering yet, we are working with rectangles in an infinitely tall texture at this point.
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbrp_pack_rects((stbrp_context*)spc.pack_info, src_tmp.Rects, src_tmp.GlyphsCount);

        // Extend texture height and mark missing glyphs as non-packed so we won't render them.
        // FIXME: We are not handling packing failure here (would happen if we got off TEX_HEIGHT_MAX or if a single if larger than TexWidth?)
        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
            if (src_tmp.Rects[glyph_i].was_packed)
                atlas->TexHeight = ImMax(atlas->TexHeight, src_tmp.Rects[glyph_i].y + src_tmp.Rects[glyph_i].h);
    }

    // 7. Allocate texture
    atlas->TexHeight = (atlas->Flags & ImFontAtlasFlags_NoPowerOfTwoHeight) ? (atlas->TexHeight + 1) : ImUpperPowerOfTwo(atlas->TexHeight);
    atlas->TexUvScale = ImVec2(1.0f / atlas->TexWidth, 1.0f / atlas->TexHeight);
    atlas->TexPixelsAlpha8 = (unsigned char*)IM_ALLOC(atlas->TexWidth * atlas->TexHeight);
    memset(atlas->TexPixelsAlpha8, 0, atlas->TexWidth * atlas->TexHeight);
    spc.pixels = atlas->TexPixelsAlpha8;
    spc.height = atlas->TexHeight;

    // 8. Render/rasterize font characters into the texture
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        stbtt_PackFontRangesRenderIntoRects(&spc, &src_tmp.FontInfo, &src_tmp.PackRange, 1, src_tmp.Rects);

        // Apply multiply operator
        if (cfg.RasterizerMultiply != 1.0f)
        {
            unsigned char multiply_table[256];
            ImFontAtlasBuildMultiplyCalcLookupTable(multiply_table, cfg.RasterizerMultiply);
            stbrp_rect* r = &src_tmp.Rects[0];
            for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++, r++)
                if (r->was_packed)
                    ImFontAtlasBuildMultiplyRectAlpha8(multiply_table, atlas->TexPixelsAlpha8, r->x, r->y, r->w, r->h, atlas->TexWidth * 1);
        }
        src_tmp.Rects = NULL;
    }

    // End packing
    stbtt_PackEnd(&spc);
    buf_rects.clear();

    // 9. Setup ImFont and glyphs for runtime
    for (int src_i = 0; src_i < src_tmp_array.Size; src_i++)
    {
        ImFontBuildSrcData& src_tmp = src_tmp_array[src_i];
        if (src_tmp.GlyphsCount == 0)
            continue;

        // When merging fonts with MergeMode=true:
        // - We can have multiple input fonts writing into a same destination font.
        // - dst_font->ConfigData is != from cfg which is our source configuration.
        ImFontConfig& cfg = atlas->ConfigData[src_i];
        ImFont* dst_font = cfg.DstFont;

        const float font_scale = stbtt_ScaleForPixelHeight(&src_tmp.FontInfo, cfg.SizePixels);
        int unscaled_ascent, unscaled_descent, unscaled_line_gap;
        stbtt_GetFontVMetrics(&src_tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

        const float ascent = ImFloor(unscaled_ascent * font_scale + ((unscaled_ascent > 0.0f) ? +1 : -1));
        const float descent = ImFloor(unscaled_descent * font_scale + ((unscaled_descent > 0.0f) ? +1 : -1));
        ImFontAtlasBuildSetupFont(atlas, dst_font, &cfg, ascent, descent);
        const float font_off_x = cfg.GlyphOffset.x;
        const float font_off_y = cfg.GlyphOffset.y + IM_ROUND(dst_font->Ascent);

        for (int glyph_i = 0; glyph_i < src_tmp.GlyphsCount; glyph_i++)
        {
            // Register glyph
            const int codepoint = src_tmp.GlyphsList[glyph_i];
            const stbtt_packedchar& pc = src_tmp.PackedChars[glyph_i];
            stbtt_aligned_quad q;
            float unused_x = 0.0f, unused_y = 0.0f;
            stbtt_GetPackedQuad(src_tmp.PackedChars, atlas->TexWidth, atlas->TexHeight, glyph_i, &unused_x, &unused_y, &q, 0);
            dst_font->AddGlyph(&cfg, (ImWchar)codepoint, q.x0 + font_off_x, q.y0 + font_off_y, q.x1 + font_off_x, q.y1 + font_off_y, q.s0, q.t0, q.s1, q.t1, pc.xadvance);
        }
    }

    // Cleanup
    src_tmp_array.clear_destruct();

    ImFontAtlasBuildFinish(atlas);
    return true;
}

const ImFontBuilderIO* ImFontAtlasGetBuilderForStbTruetype()
{
    static ImFontBuilderIO io;
    io.FontBuilder_Build = ImFontAtlasBuildWithStbTruetype;
    return &io;
}

#endif // IMGUI_ENABLE_STB_TRUETYPE

void ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent)
{
    if (!font_config->MergeMode)
    {
        font->ClearOutputData();
        font->FontSize = font_config->SizePixels;
        font->ConfigData = font_config;
        font->ConfigDataCount = 0;
        font->ContainerAtlas = atlas;
        font->Ascent = ascent;
        font->Descent = descent;
    }
    font->ConfigDataCount++;
}

void ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* stbrp_context_opaque)
{
    stbrp_context* pack_context = (stbrp_context*)stbrp_context_opaque;
    IM_ASSERT(pack_context != NULL);

    ImVector<ImFontAtlasCustomRect>& user_rects = atlas->CustomRects;
    IM_ASSERT(user_rects.Size >= 1); // We expect at least the default custom rects to be registered, else something went wrong.

    ImVector<stbrp_rect> pack_rects;
    pack_rects.resize(user_rects.Size);
    memset(pack_rects.Data, 0, (size_t)pack_rects.size_in_bytes());
    for (int i = 0; i < user_rects.Size; i++)
    {
        pack_rects[i].w = user_rects[i].Width;
        pack_rects[i].h = user_rects[i].Height;
    }
    stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.Size);
    for (int i = 0; i < pack_rects.Size; i++)
        if (pack_rects[i].was_packed)
        {
            user_rects[i].X = pack_rects[i].x;
            user_rects[i].Y = pack_rects[i].y;
            IM_ASSERT(pack_rects[i].w == user_rects[i].Width && pack_rects[i].h == user_rects[i].Height);
            atlas->TexHeight = ImMax(atlas->TexHeight, pack_rects[i].y + pack_rects[i].h);
        }
}

void ImFontAtlasBuildRender8bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value)
{
    IM_ASSERT(x >= 0 && x + w <= atlas->TexWidth);
    IM_ASSERT(y >= 0 && y + h <= atlas->TexHeight);
    unsigned char* out_pixel = atlas->TexPixelsAlpha8 + x + (y * atlas->TexWidth);
    for (int off_y = 0; off_y < h; off_y++, out_pixel += atlas->TexWidth, in_str += w)
        for (int off_x = 0; off_x < w; off_x++)
            out_pixel[off_x] = (in_str[off_x] == in_marker_char) ? in_marker_pixel_value : 0x00;
}

void ImFontAtlasBuildRender32bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned int in_marker_pixel_value)
{
    IM_ASSERT(x >= 0 && x + w <= atlas->TexWidth);
    IM_ASSERT(y >= 0 && y + h <= atlas->TexHeight);
    unsigned int* out_pixel = atlas->TexPixelsRGBA32 + x + (y * atlas->TexWidth);
    for (int off_y = 0; off_y < h; off_y++, out_pixel += atlas->TexWidth, in_str += w)
        for (int off_x = 0; off_x < w; off_x++)
            out_pixel[off_x] = (in_str[off_x] == in_marker_char) ? in_marker_pixel_value : IM_COL32_BLACK_TRANS;
}

static void ImFontAtlasBuildRenderDefaultTexData(ImFontAtlas* atlas)
{
    ImFontAtlasCustomRect* r = atlas->GetCustomRectByIndex(atlas->PackIdMouseCursors);
    IM_ASSERT(r->IsPacked());

    const int w = atlas->TexWidth;
    if (!(atlas->Flags & ImFontAtlasFlags_NoMouseCursors))
    {
        // Render/copy pixels
        IM_ASSERT(r->Width == FONT_ATLAS_DEFAULT_TEX_DATA_W * 2 + 1 && r->Height == FONT_ATLAS_DEFAULT_TEX_DATA_H);
        const int x_for_white = r->X;
        const int x_for_black = r->X + FONT_ATLAS_DEFAULT_TEX_DATA_W + 1;
        if (atlas->TexPixelsAlpha8 != NULL)
        {
            ImFontAtlasBuildRender8bppRectFromString(atlas, x_for_white, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, '.', 0xFF);
            ImFontAtlasBuildRender8bppRectFromString(atlas, x_for_black, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, 'X', 0xFF);
        }
        else
        {
            ImFontAtlasBuildRender32bppRectFromString(atlas, x_for_white, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, '.', IM_COL32_WHITE);
            ImFontAtlasBuildRender32bppRectFromString(atlas, x_for_black, r->Y, FONT_ATLAS_DEFAULT_TEX_DATA_W, FONT_ATLAS_DEFAULT_TEX_DATA_H, FONT_ATLAS_DEFAULT_TEX_DATA_PIXELS, 'X', IM_COL32_WHITE);
        }
    }
    else
    {
        // Render 4 white pixels
        IM_ASSERT(r->Width == 2 && r->Height == 2);
        const int offset = (int)r->X + (int)r->Y * w;
        if (atlas->TexPixelsAlpha8 != NULL)
        {
            atlas->TexPixelsAlpha8[offset] = atlas->TexPixelsAlpha8[offset + 1] = atlas->TexPixelsAlpha8[offset + w] = atlas->TexPixelsAlpha8[offset + w + 1] = 0xFF;
        }
        else
        {
            atlas->TexPixelsRGBA32[offset] = atlas->TexPixelsRGBA32[offset + 1] = atlas->TexPixelsRGBA32[offset + w] = atlas->TexPixelsRGBA32[offset + w + 1] = IM_COL32_WHITE;
        }
    }
    atlas->TexUvWhitePixel = ImVec2((r->X + 0.5f) * atlas->TexUvScale.x, (r->Y + 0.5f) * atlas->TexUvScale.y);
}

static void ImFontAtlasBuildRenderLinesTexData(ImFontAtlas* atlas)
{
    if (atlas->Flags & ImFontAtlasFlags_NoBakedLines)
        return;

    // This generates a triangular shape in the texture, with the various line widths stacked on top of each other to allow interpolation between them
    ImFontAtlasCustomRect* r = atlas->GetCustomRectByIndex(atlas->PackIdLines);
    IM_ASSERT(r->IsPacked());
    for (unsigned int n = 0; n < IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1; n++) // +1 because of the zero-width row
    {
        // Each line consists of at least two empty pixels at the ends, with a line of solid pixels in the middle
        unsigned int y = n;
        unsigned int line_width = n;
        unsigned int pad_left = (r->Width - line_width) / 2;
        unsigned int pad_right = r->Width - (pad_left + line_width);

        // Write each slice
        IM_ASSERT(pad_left + line_width + pad_right == r->Width && y < r->Height); // Make sure we're inside the texture bounds before we start writing pixels
        if (atlas->TexPixelsAlpha8 != NULL)
        {
            unsigned char* write_ptr = &atlas->TexPixelsAlpha8[r->X + ((r->Y + y) * atlas->TexWidth)];
            for (unsigned int i = 0; i < pad_left; i++)
                *(write_ptr + i) = 0x00;

            for (unsigned int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = 0xFF;

            for (unsigned int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width + i) = 0x00;
        }
        else
        {
            unsigned int* write_ptr = &atlas->TexPixelsRGBA32[r->X + ((r->Y + y) * atlas->TexWidth)];
            for (unsigned int i = 0; i < pad_left; i++)
                *(write_ptr + i) = IM_COL32_BLACK_TRANS;

            for (unsigned int i = 0; i < line_width; i++)
                *(write_ptr + pad_left + i) = IM_COL32_WHITE;

            for (unsigned int i = 0; i < pad_right; i++)
                *(write_ptr + pad_left + line_width + i) = IM_COL32_BLACK_TRANS;
        }

        // Calculate UVs for this line
        ImVec2 uv0 = ImVec2((float)(r->X + pad_left - 1), (float)(r->Y + y)) * atlas->TexUvScale;
        ImVec2 uv1 = ImVec2((float)(r->X + pad_left + line_width + 1), (float)(r->Y + y + 1)) * atlas->TexUvScale;
        float half_v = (uv0.y + uv1.y) * 0.5f; // Calculate a constant V in the middle of the row to avoid sampling artifacts
        atlas->TexUvLines[n] = ImVec4(uv0.x, half_v, uv1.x, half_v);
    }
}

// Note: this is called / shared by both the stb_truetype and the FreeType builder
void ImFontAtlasBuildInit(ImFontAtlas* atlas)
{
    // Register texture region for mouse cursors or standard white pixels
    if (atlas->PackIdMouseCursors < 0)
    {
        if (!(atlas->Flags & ImFontAtlasFlags_NoMouseCursors))
            atlas->PackIdMouseCursors = atlas->AddCustomRectRegular(FONT_ATLAS_DEFAULT_TEX_DATA_W * 2 + 1, FONT_ATLAS_DEFAULT_TEX_DATA_H);
        else
            atlas->PackIdMouseCursors = atlas->AddCustomRectRegular(2, 2);
    }

    // Register texture region for thick lines
    // The +2 here is to give space for the end caps, whilst height +1 is to accommodate the fact we have a zero-width row
    if (atlas->PackIdLines < 0)
    {
        if (!(atlas->Flags & ImFontAtlasFlags_NoBakedLines))
            atlas->PackIdLines = atlas->AddCustomRectRegular(IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 2, IM_DRAWLIST_TEX_LINES_WIDTH_MAX + 1);
    }
}

// This is called/shared by both the stb_truetype and the FreeType builder.
void ImFontAtlasBuildFinish(ImFontAtlas* atlas)
{
    // Render into our custom data blocks
    IM_ASSERT(atlas->TexPixelsAlpha8 != NULL || atlas->TexPixelsRGBA32 != NULL);
    ImFontAtlasBuildRenderDefaultTexData(atlas);
    ImFontAtlasBuildRenderLinesTexData(atlas);

    // Register custom rectangle glyphs
    for (int i = 0; i < atlas->CustomRects.Size; i++)
    {
        const ImFontAtlasCustomRect* r = &atlas->CustomRects[i];
        if (r->Font == NULL || r->GlyphID == 0)
            continue;

        // Will ignore ImFontConfig settings: GlyphMinAdvanceX, GlyphMinAdvanceY, GlyphExtraSpacing, PixelSnapH
        IM_ASSERT(r->Font->ContainerAtlas == atlas);
        ImVec2 uv0, uv1;
        atlas->CalcCustomRectUV(r, &uv0, &uv1);
        r->Font->AddGlyph(NULL, (ImWchar)r->GlyphID, r->GlyphOffset.x, r->GlyphOffset.y, r->GlyphOffset.x + r->Width, r->GlyphOffset.y + r->Height, uv0.x, uv0.y, uv1.x, uv1.y, r->GlyphAdvanceX);
    }

    // Build all fonts lookup tables
    for (int i = 0; i < atlas->Fonts.Size; i++)
        if (atlas->Fonts[i]->DirtyLookupTables)
            atlas->Fonts[i]->BuildLookupTable();

    atlas->TexReady = true;
}

// Retrieve list of range (2 int per range, values are inclusive)
const ImWchar*   ImFontAtlas::GetGlyphRangesDefault()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesKorean()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x3131, 0x3163, // Korean alphabets
        0xAC00, 0xD7A3, // Korean characters
        0xFFFD, 0xFFFD, // Invalid
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesChineseFull()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0xFFFD, 0xFFFD, // Invalid
        0x4e00, 0x9FAF, // CJK Ideograms
        0,
    };
    return &ranges[0];
}

static void UnpackAccumulativeOffsetsIntoRanges(int base_codepoint, const short* accumulative_offsets, int accumulative_offsets_count, ImWchar* out_ranges)
{
    for (int n = 0; n < accumulative_offsets_count; n++, out_ranges += 2)
    {
        out_ranges[0] = out_ranges[1] = (ImWchar)(base_codepoint + accumulative_offsets[n]);
        base_codepoint += accumulative_offsets[n];
    }
    out_ranges[0] = 0;
}

//-------------------------------------------------------------------------
// [SECTION] ImFontAtlas glyph ranges helpers
//-------------------------------------------------------------------------

const ImWchar*  ImFontAtlas::GetGlyphRangesChineseSimplifiedCommon()
{
    // Store 2500 regularly used characters for Simplified Chinese.
    // Sourced from https://zh.wiktionary.org/wiki/%E9%99%84%E5%BD%95:%E7%8E%B0%E4%BB%A3%E6%B1%89%E8%AF%AD%E5%B8%B8%E7%94%A8%E5%AD%97%E8%A1%A8
    // This table covers 97.97% of all characters used during the month in July, 1987.
    // You can use ImFontGlyphRangesBuilder to create your own ranges derived from this, by merging existing ranges or adding new characters.
    // (Stored as accumulative offsets from the initial unicode codepoint 0x4E00. This encoding is designed to helps us compact the source code size.)
    static const short accumulative_offsets_from_0x4E00[] =
    {
        0,1,2,4,1,1,1,1,2,1,3,2,1,2,2,1,1,1,1,1,5,2,1,2,3,3,3,2,2,4,1,1,1,2,1,5,2,3,1,2,1,2,1,1,2,1,1,2,2,1,4,1,1,1,1,5,10,1,2,19,2,1,2,1,2,1,2,1,2,
        1,5,1,6,3,2,1,2,2,1,1,1,4,8,5,1,1,4,1,1,3,1,2,1,5,1,2,1,1,1,10,1,1,5,2,4,6,1,4,2,2,2,12,2,1,1,6,1,1,1,4,1,1,4,6,5,1,4,2,2,4,10,7,1,1,4,2,4,
        2,1,4,3,6,10,12,5,7,2,14,2,9,1,1,6,7,10,4,7,13,1,5,4,8,4,1,1,2,28,5,6,1,1,5,2,5,20,2,2,9,8,11,2,9,17,1,8,6,8,27,4,6,9,20,11,27,6,68,2,2,1,1,
        1,2,1,2,2,7,6,11,3,3,1,1,3,1,2,1,1,1,1,1,3,1,1,8,3,4,1,5,7,2,1,4,4,8,4,2,1,2,1,1,4,5,6,3,6,2,12,3,1,3,9,2,4,3,4,1,5,3,3,1,3,7,1,5,1,1,1,1,2,
        3,4,5,2,3,2,6,1,1,2,1,7,1,7,3,4,5,15,2,2,1,5,3,22,19,2,1,1,1,1,2,5,1,1,1,6,1,1,12,8,2,9,18,22,4,1,1,5,1,16,1,2,7,10,15,1,1,6,2,4,1,2,4,1,6,
        1,1,3,2,4,1,6,4,5,1,2,1,1,2,1,10,3,1,3,2,1,9,3,2,5,7,2,19,4,3,6,1,1,1,1,1,4,3,2,1,1,1,2,5,3,1,1,1,2,2,1,1,2,1,1,2,1,3,1,1,1,3,7,1,4,1,1,2,1,
        1,2,1,2,4,4,3,8,1,1,1,2,1,3,5,1,3,1,3,4,6,2,2,14,4,6,6,11,9,1,15,3,1,28,5,2,5,5,3,1,3,4,5,4,6,14,3,2,3,5,21,2,7,20,10,1,2,19,2,4,28,28,2,3,
        2,1,14,4,1,26,28,42,12,40,3,52,79,5,14,17,3,2,2,11,3,4,6,3,1,8,2,23,4,5,8,10,4,2,7,3,5,1,1,6,3,1,2,2,2,5,28,1,1,7,7,20,5,3,29,3,17,26,1,8,4,
        27,3,6,11,23,5,3,4,6,13,24,16,6,5,10,25,35,7,3,2,3,3,14,3,6,2,6,1,4,2,3,8,2,1,1,3,3,3,4,1,1,13,2,2,4,5,2,1,14,14,1,2,2,1,4,5,2,3,1,14,3,12,
        3,17,2,16,5,1,2,1,8,9,3,19,4,2,2,4,17,25,21,20,28,75,1,10,29,103,4,1,2,1,1,4,2,4,1,2,3,24,2,2,2,1,1,2,1,3,8,1,1,1,2,1,1,3,1,1,1,6,1,5,3,1,1,
        1,3,4,1,1,5,2,1,5,6,13,9,16,1,1,1,1,3,2,3,2,4,5,2,5,2,2,3,7,13,7,2,2,1,1,1,1,2,3,3,2,1,6,4,9,2,1,14,2,14,2,1,18,3,4,14,4,11,41,15,23,15,23,
        176,1,3,4,1,1,1,1,5,3,1,2,3,7,3,1,1,2,1,2,4,4,6,2,4,1,9,7,1,10,5,8,16,29,1,1,2,2,3,1,3,5,2,4,5,4,1,1,2,2,3,3,7,1,6,10,1,17,1,44,4,6,2,1,1,6,
        5,4,2,10,1,6,9,2,8,1,24,1,2,13,7,8,8,2,1,4,1,3,1,3,3,5,2,5,10,9,4,9,12,2,1,6,1,10,1,1,7,7,4,10,8,3,1,13,4,3,1,6,1,3,5,2,1,2,17,16,5,2,16,6,
        1,4,2,1,3,3,6,8,5,11,11,1,3,3,2,4,6,10,9,5,7,4,7,4,7,1,1,4,2,1,3,6,8,7,1,6,11,5,5,3,24,9,4,2,7,13,5,1,8,82,16,61,1,1,1,4,2,2,16,10,3,8,1,1,
        6,4,2,1,3,1,1,1,4,3,8,4,2,2,1,1,1,1,1,6,3,5,1,1,4,6,9,2,1,1,1,2,1,7,2,1,6,1,5,4,4,3,1,8,1,3,3,1,3,2,2,2,2,3,1,6,1,2,1,2,1,3,7,1,8,2,1,2,1,5,
        2,5,3,5,10,1,2,1,1,3,2,5,11,3,9,3,5,1,1,5,9,1,2,1,5,7,9,9,8,1,3,3,3,6,8,2,3,2,1,1,32,6,1,2,15,9,3,7,13,1,3,10,13,2,14,1,13,10,2,1,3,10,4,15,
        2,15,15,10,1,3,9,6,9,32,25,26,47,7,3,2,3,1,6,3,4,3,2,8,5,4,1,9,4,2,2,19,10,6,2,3,8,1,2,2,4,2,1,9,4,4,4,6,4,8,9,2,3,1,1,1,1,3,5,5,1,3,8,4,6,
        2,1,4,12,1,5,3,7,13,2,5,8,1,6,1,2,5,14,6,1,5,2,4,8,15,5,1,23,6,62,2,10,1,1,8,1,2,2,10,4,2,2,9,2,1,1,3,2,3,1,5,3,3,2,1,3,8,1,1,1,11,3,1,1,4,
        3,7,1,14,1,2,3,12,5,2,5,1,6,7,5,7,14,11,1,3,1,8,9,12,2,1,11,8,4,4,2,6,10,9,13,1,1,3,1,5,1,3,2,4,4,1,18,2,3,14,11,4,29,4,2,7,1,3,13,9,2,2,5,
        3,5,20,7,16,8,5,72,34,6,4,22,12,12,28,45,36,9,7,39,9,191,1,1,1,4,11,8,4,9,2,3,22,1,1,1,1,4,17,1,7,7,1,11,31,10,2,4,8,2,3,2,1,4,2,16,4,32,2,
        3,19,13,4,9,1,5,2,14,8,1,1,3,6,19,6,5,1,16,6,2,10,8,5,1,2,3,1,5,5,1,11,6,6,1,3,3,2,6,3,8,1,1,4,10,7,5,7,7,5,8,9,2,1,3,4,1,1,3,1,3,3,2,6,16,
        1,4,6,3,1,10,6,1,3,15,2,9,2,10,25,13,9,16,6,2,2,10,11,4,3,9,1,2,6,6,5,4,30,40,1,10,7,12,14,33,6,3,6,7,3,1,3,1,11,14,4,9,5,12,11,49,18,51,31,
        140,31,2,2,1,5,1,8,1,10,1,4,4,3,24,1,10,1,3,6,6,16,3,4,5,2,1,4,2,57,10,6,22,2,22,3,7,22,6,10,11,36,18,16,33,36,2,5,5,1,1,1,4,10,1,4,13,2,7,
        5,2,9,3,4,1,7,43,3,7,3,9,14,7,9,1,11,1,1,3,7,4,18,13,1,14,1,3,6,10,73,2,2,30,6,1,11,18,19,13,22,3,46,42,37,89,7,3,16,34,2,2,3,9,1,7,1,1,1,2,
        2,4,10,7,3,10,3,9,5,28,9,2,6,13,7,3,1,3,10,2,7,2,11,3,6,21,54,85,2,1,4,2,2,1,39,3,21,2,2,5,1,1,1,4,1,1,3,4,15,1,3,2,4,4,2,3,8,2,20,1,8,7,13,
        4,1,26,6,2,9,34,4,21,52,10,4,4,1,5,12,2,11,1,7,2,30,12,44,2,30,1,1,3,6,16,9,17,39,82,2,2,24,7,1,7,3,16,9,14,44,2,1,2,1,2,3,5,2,4,1,6,7,5,3,
        2,6,1,11,5,11,2,1,18,19,8,1,3,24,29,2,1,3,5,2,2,1,13,6,5,1,46,11,3,5,1,1,5,8,2,10,6,12,6,3,7,11,2,4,16,13,2,5,1,1,2,2,5,2,28,5,2,23,10,8,4,
        4,22,39,95,38,8,14,9,5,1,13,5,4,3,13,12,11,1,9,1,27,37,2,5,4,4,63,211,95,2,2,2,1,3,5,2,1,1,2,2,1,1,1,3,2,4,1,2,1,1,5,2,2,1,1,2,3,1,3,1,1,1,
        3,1,4,2,1,3,6,1,1,3,7,15,5,3,2,5,3,9,11,4,2,22,1,6,3,8,7,1,4,28,4,16,3,3,25,4,4,27,27,1,4,1,2,2,7,1,3,5,2,28,8,2,14,1,8,6,16,25,3,3,3,14,3,
        3,1,1,2,1,4,6,3,8,4,1,1,1,2,3,6,10,6,2,3,18,3,2,5,5,4,3,1,5,2,5,4,23,7,6,12,6,4,17,11,9,5,1,1,10,5,12,1,1,11,26,33,7,3,6,1,17,7,1,5,12,1,11,
        2,4,1,8,14,17,23,1,2,1,7,8,16,11,9,6,5,2,6,4,16,2,8,14,1,11,8,9,1,1,1,9,25,4,11,19,7,2,15,2,12,8,52,7,5,19,2,16,4,36,8,1,16,8,24,26,4,6,2,9,
        5,4,36,3,28,12,25,15,37,27,17,12,59,38,5,32,127,1,2,9,17,14,4,1,2,1,1,8,11,50,4,14,2,19,16,4,17,5,4,5,26,12,45,2,23,45,104,30,12,8,3,10,2,2,
        3,3,1,4,20,7,2,9,6,15,2,20,1,3,16,4,11,15,6,134,2,5,59,1,2,2,2,1,9,17,3,26,137,10,211,59,1,2,4,1,4,1,1,1,2,6,2,3,1,1,2,3,2,3,1,3,4,4,2,3,3,
        1,4,3,1,7,2,2,3,1,2,1,3,3,3,2,2,3,2,1,3,14,6,1,3,2,9,6,15,27,9,34,145,1,1,2,1,1,1,1,2,1,1,1,1,2,2,2,3,1,2,1,1,1,2,3,5,8,3,5,2,4,1,3,2,2,2,12,
        4,1,1,1,10,4,5,1,20,4,16,1,15,9,5,12,2,9,2,5,4,2,26,19,7,1,26,4,30,12,15,42,1,6,8,172,1,1,4,2,1,1,11,2,2,4,2,1,2,1,10,8,1,2,1,4,5,1,2,5,1,8,
        4,1,3,4,2,1,6,2,1,3,4,1,2,1,1,1,1,12,5,7,2,4,3,1,1,1,3,3,6,1,2,2,3,3,3,2,1,2,12,14,11,6,6,4,12,2,8,1,7,10,1,35,7,4,13,15,4,3,23,21,28,52,5,
        26,5,6,1,7,10,2,7,53,3,2,1,1,1,2,163,532,1,10,11,1,3,3,4,8,2,8,6,2,2,23,22,4,2,2,4,2,1,3,1,3,3,5,9,8,2,1,2,8,1,10,2,12,21,20,15,105,2,3,1,1,
        3,2,3,1,1,2,5,1,4,15,11,19,1,1,1,1,5,4,5,1,1,2,5,3,5,12,1,2,5,1,11,1,1,15,9,1,4,5,3,26,8,2,1,3,1,1,15,19,2,12,1,2,5,2,7,2,19,2,20,6,26,7,5,
        2,2,7,34,21,13,70,2,128,1,1,2,1,1,2,1,1,3,2,2,2,15,1,4,1,3,4,42,10,6,1,49,85,8,1,2,1,1,4,4,2,3,6,1,5,7,4,3,211,4,1,2,1,2,5,1,2,4,2,2,6,5,6,
        10,3,4,48,100,6,2,16,296,5,27,387,2,2,3,7,16,8,5,38,15,39,21,9,10,3,7,59,13,27,21,47,5,21,6
    };
    static ImWchar base_ranges[] = // not zero-terminated
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x2000, 0x206F, // General Punctuation
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0xFFFD, 0xFFFD  // Invalid
    };
    static ImWchar full_ranges[IM_ARRAYSIZE(base_ranges) + IM_ARRAYSIZE(accumulative_offsets_from_0x4E00) * 2 + 1] = { 0 };
    if (!full_ranges[0])
    {
        memcpy(full_ranges, base_ranges, sizeof(base_ranges));
        UnpackAccumulativeOffsetsIntoRanges(0x4E00, accumulative_offsets_from_0x4E00, IM_ARRAYSIZE(accumulative_offsets_from_0x4E00), full_ranges + IM_ARRAYSIZE(base_ranges));
    }
    return &full_ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesJapanese()
{
    // 2999 ideograms code points for Japanese
    // - 2136 Joyo (meaning "for regular use" or "for common use") Kanji code points
    // - 863 Jinmeiyo (meaning "for personal name") Kanji code points
    // - Sourced from the character information database of the Information-technology Promotion Agency, Japan
    //   - https://mojikiban.ipa.go.jp/mji/
    //   - Available under the terms of the Creative Commons Attribution-ShareAlike 2.1 Japan (CC BY-SA 2.1 JP).
    //     - https://creativecommons.org/licenses/by-sa/2.1/jp/deed.en
    //     - https://creativecommons.org/licenses/by-sa/2.1/jp/legalcode
    //   - You can generate this code by the script at:
    //     - https://github.com/vaiorabbit/everyday_use_kanji
    // - References:
    //   - List of Joyo Kanji
    //     - (Official list by the Agency for Cultural Affairs) https://www.bunka.go.jp/kokugo_nihongo/sisaku/joho/joho/kakuki/14/tosin02/index.html
    //     - (Wikipedia) https://en.wikipedia.org/wiki/List_of_j%C5%8Dy%C5%8D_kanji
    //   - List of Jinmeiyo Kanji
    //     - (Official list by the Ministry of Justice) http://www.moj.go.jp/MINJI/minji86.html
    //     - (Wikipedia) https://en.wikipedia.org/wiki/Jinmeiy%C5%8D_kanji
    // - Missing 1 Joyo Kanji: U+20B9F (Kun'yomi: Shikaru, On'yomi: Shitsu,shichi), see https://github.com/ocornut/imgui/pull/3627 for details.
    // You can use ImFontGlyphRangesBuilder to create your own ranges derived from this, by merging existing ranges or adding new characters.
    // (Stored as accumulative offsets from the initial unicode codepoint 0x4E00. This encoding is designed to helps us compact the source code size.)
    static const short accumulative_offsets_from_0x4E00[] =
    {
        0,1,2,4,1,1,1,1,2,1,3,3,2,2,1,5,3,5,7,5,6,1,2,1,7,2,6,3,1,8,1,1,4,1,1,18,2,11,2,6,2,1,2,1,5,1,2,1,3,1,2,1,2,3,3,1,1,2,3,1,1,1,12,7,9,1,4,5,1,
        1,2,1,10,1,1,9,2,2,4,5,6,9,3,1,1,1,1,9,3,18,5,2,2,2,2,1,6,3,7,1,1,1,1,2,2,4,2,1,23,2,10,4,3,5,2,4,10,2,4,13,1,6,1,9,3,1,1,6,6,7,6,3,1,2,11,3,
        2,2,3,2,15,2,2,5,4,3,6,4,1,2,5,2,12,16,6,13,9,13,2,1,1,7,16,4,7,1,19,1,5,1,2,2,7,7,8,2,6,5,4,9,18,7,4,5,9,13,11,8,15,2,1,1,1,2,1,2,2,1,2,2,8,
        2,9,3,3,1,1,4,4,1,1,1,4,9,1,4,3,5,5,2,7,5,3,4,8,2,1,13,2,3,3,1,14,1,1,4,5,1,3,6,1,5,2,1,1,3,3,3,3,1,1,2,7,6,6,7,1,4,7,6,1,1,1,1,1,12,3,3,9,5,
        2,6,1,5,6,1,2,3,18,2,4,14,4,1,3,6,1,1,6,3,5,5,3,2,2,2,2,12,3,1,4,2,3,2,3,11,1,7,4,1,2,1,3,17,1,9,1,24,1,1,4,2,2,4,1,2,7,1,1,1,3,1,2,2,4,15,1,
        1,2,1,1,2,1,5,2,5,20,2,5,9,1,10,8,7,6,1,1,1,1,1,1,6,2,1,2,8,1,1,1,1,5,1,1,3,1,1,1,1,3,1,1,12,4,1,3,1,1,1,1,1,10,3,1,7,5,13,1,2,3,4,6,1,1,30,
        2,9,9,1,15,38,11,3,1,8,24,7,1,9,8,10,2,1,9,31,2,13,6,2,9,4,49,5,2,15,2,1,10,2,1,1,1,2,2,6,15,30,35,3,14,18,8,1,16,10,28,12,19,45,38,1,3,2,3,
        13,2,1,7,3,6,5,3,4,3,1,5,7,8,1,5,3,18,5,3,6,1,21,4,24,9,24,40,3,14,3,21,3,2,1,2,4,2,3,1,15,15,6,5,1,1,3,1,5,6,1,9,7,3,3,2,1,4,3,8,21,5,16,4,
        5,2,10,11,11,3,6,3,2,9,3,6,13,1,2,1,1,1,1,11,12,6,6,1,4,2,6,5,2,1,1,3,3,6,13,3,1,1,5,1,2,3,3,14,2,1,2,2,2,5,1,9,5,1,1,6,12,3,12,3,4,13,2,14,
        2,8,1,17,5,1,16,4,2,2,21,8,9,6,23,20,12,25,19,9,38,8,3,21,40,25,33,13,4,3,1,4,1,2,4,1,2,5,26,2,1,1,2,1,3,6,2,1,1,1,1,1,1,2,3,1,1,1,9,2,3,1,1,
        1,3,6,3,2,1,1,6,6,1,8,2,2,2,1,4,1,2,3,2,7,3,2,4,1,2,1,2,2,1,1,1,1,1,3,1,2,5,4,10,9,4,9,1,1,1,1,1,1,5,3,2,1,6,4,9,6,1,10,2,31,17,8,3,7,5,40,1,
        7,7,1,6,5,2,10,7,8,4,15,39,25,6,28,47,18,10,7,1,3,1,1,2,1,1,1,3,3,3,1,1,1,3,4,2,1,4,1,3,6,10,7,8,6,2,2,1,3,3,2,5,8,7,9,12,2,15,1,1,4,1,2,1,1,
        1,3,2,1,3,3,5,6,2,3,2,10,1,4,2,8,1,1,1,11,6,1,21,4,16,3,1,3,1,4,2,3,6,5,1,3,1,1,3,3,4,6,1,1,10,4,2,7,10,4,7,4,2,9,4,3,1,1,1,4,1,8,3,4,1,3,1,
        6,1,4,2,1,4,7,2,1,8,1,4,5,1,1,2,2,4,6,2,7,1,10,1,1,3,4,11,10,8,21,4,6,1,3,5,2,1,2,28,5,5,2,3,13,1,2,3,1,4,2,1,5,20,3,8,11,1,3,3,3,1,8,10,9,2,
        10,9,2,3,1,1,2,4,1,8,3,6,1,7,8,6,11,1,4,29,8,4,3,1,2,7,13,1,4,1,6,2,6,12,12,2,20,3,2,3,6,4,8,9,2,7,34,5,1,18,6,1,1,4,4,5,7,9,1,2,2,4,3,4,1,7,
        2,2,2,6,2,3,25,5,3,6,1,4,6,7,4,2,1,4,2,13,6,4,4,3,1,5,3,4,4,3,2,1,1,4,1,2,1,1,3,1,11,1,6,3,1,7,3,6,2,8,8,6,9,3,4,11,3,2,10,12,2,5,11,1,6,4,5,
        3,1,8,5,4,6,6,3,5,1,1,3,2,1,2,2,6,17,12,1,10,1,6,12,1,6,6,19,9,6,16,1,13,4,4,15,7,17,6,11,9,15,12,6,7,2,1,2,2,15,9,3,21,4,6,49,18,7,3,2,3,1,
        6,8,2,2,6,2,9,1,3,6,4,4,1,2,16,2,5,2,1,6,2,3,5,3,1,2,5,1,2,1,9,3,1,8,6,4,8,11,3,1,1,1,1,3,1,13,8,4,1,3,2,2,1,4,1,11,1,5,2,1,5,2,5,8,6,1,1,7,
        4,3,8,3,2,7,2,1,5,1,5,2,4,7,6,2,8,5,1,11,4,5,3,6,18,1,2,13,3,3,1,21,1,1,4,1,4,1,1,1,8,1,2,2,7,1,2,4,2,2,9,2,1,1,1,4,3,6,3,12,5,1,1,1,5,6,3,2,
        4,8,2,2,4,2,7,1,8,9,5,2,3,2,1,3,2,13,7,14,6,5,1,1,2,1,4,2,23,2,1,1,6,3,1,4,1,15,3,1,7,3,9,14,1,3,1,4,1,1,5,8,1,3,8,3,8,15,11,4,14,4,4,2,5,5,
        1,7,1,6,14,7,7,8,5,15,4,8,6,5,6,2,1,13,1,20,15,11,9,2,5,6,2,11,2,6,2,5,1,5,8,4,13,19,25,4,1,1,11,1,34,2,5,9,14,6,2,2,6,1,1,14,1,3,14,13,1,6,
        12,21,14,14,6,32,17,8,32,9,28,1,2,4,11,8,3,1,14,2,5,15,1,1,1,1,3,6,4,1,3,4,11,3,1,1,11,30,1,5,1,4,1,5,8,1,1,3,2,4,3,17,35,2,6,12,17,3,1,6,2,
        1,1,12,2,7,3,3,2,1,16,2,8,3,6,5,4,7,3,3,8,1,9,8,5,1,2,1,3,2,8,1,2,9,12,1,1,2,3,8,3,24,12,4,3,7,5,8,3,3,3,3,3,3,1,23,10,3,1,2,2,6,3,1,16,1,16,
        22,3,10,4,11,6,9,7,7,3,6,2,2,2,4,10,2,1,1,2,8,7,1,6,4,1,3,3,3,5,10,12,12,2,3,12,8,15,1,1,16,6,6,1,5,9,11,4,11,4,2,6,12,1,17,5,13,1,4,9,5,1,11,
        2,1,8,1,5,7,28,8,3,5,10,2,17,3,38,22,1,2,18,12,10,4,38,18,1,4,44,19,4,1,8,4,1,12,1,4,31,12,1,14,7,75,7,5,10,6,6,13,3,2,11,11,3,2,5,28,15,6,18,
        18,5,6,4,3,16,1,7,18,7,36,3,5,3,1,7,1,9,1,10,7,2,4,2,6,2,9,7,4,3,32,12,3,7,10,2,23,16,3,1,12,3,31,4,11,1,3,8,9,5,1,30,15,6,12,3,2,2,11,19,9,
        14,2,6,2,3,19,13,17,5,3,3,25,3,14,1,1,1,36,1,3,2,19,3,13,36,9,13,31,6,4,16,34,2,5,4,2,3,3,5,1,1,1,4,3,1,17,3,2,3,5,3,1,3,2,3,5,6,3,12,11,1,3,
        1,2,26,7,12,7,2,14,3,3,7,7,11,25,25,28,16,4,36,1,2,1,6,2,1,9,3,27,17,4,3,4,13,4,1,3,2,2,1,10,4,2,4,6,3,8,2,1,18,1,1,24,2,2,4,33,2,3,63,7,1,6,
        40,7,3,4,4,2,4,15,18,1,16,1,1,11,2,41,14,1,3,18,13,3,2,4,16,2,17,7,15,24,7,18,13,44,2,2,3,6,1,1,7,5,1,7,1,4,3,3,5,10,8,2,3,1,8,1,1,27,4,2,1,
        12,1,2,1,10,6,1,6,7,5,2,3,7,11,5,11,3,6,6,2,3,15,4,9,1,1,2,1,2,11,2,8,12,8,5,4,2,3,1,5,2,2,1,14,1,12,11,4,1,11,17,17,4,3,2,5,5,7,3,1,5,9,9,8,
        2,5,6,6,13,13,2,1,2,6,1,2,2,49,4,9,1,2,10,16,7,8,4,3,2,23,4,58,3,29,1,14,19,19,11,11,2,7,5,1,3,4,6,2,18,5,12,12,17,17,3,3,2,4,1,6,2,3,4,3,1,
        1,1,1,5,1,1,9,1,3,1,3,6,1,8,1,1,2,6,4,14,3,1,4,11,4,1,3,32,1,2,4,13,4,1,2,4,2,1,3,1,11,1,4,2,1,4,4,6,3,5,1,6,5,7,6,3,23,3,5,3,5,3,3,13,3,9,10,
        1,12,10,2,3,18,13,7,160,52,4,2,2,3,2,14,5,4,12,4,6,4,1,20,4,11,6,2,12,27,1,4,1,2,2,7,4,5,2,28,3,7,25,8,3,19,3,6,10,2,2,1,10,2,5,4,1,3,4,1,5,
        3,2,6,9,3,6,2,16,3,3,16,4,5,5,3,2,1,2,16,15,8,2,6,21,2,4,1,22,5,8,1,1,21,11,2,1,11,11,19,13,12,4,2,3,2,3,6,1,8,11,1,4,2,9,5,2,1,11,2,9,1,1,2,
        14,31,9,3,4,21,14,4,8,1,7,2,2,2,5,1,4,20,3,3,4,10,1,11,9,8,2,1,4,5,14,12,14,2,17,9,6,31,4,14,1,20,13,26,5,2,7,3,6,13,2,4,2,19,6,2,2,18,9,3,5,
        12,12,14,4,6,2,3,6,9,5,22,4,5,25,6,4,8,5,2,6,27,2,35,2,16,3,7,8,8,6,6,5,9,17,2,20,6,19,2,13,3,1,1,1,4,17,12,2,14,7,1,4,18,12,38,33,2,10,1,1,
        2,13,14,17,11,50,6,33,20,26,74,16,23,45,50,13,38,33,6,6,7,4,4,2,1,3,2,5,8,7,8,9,3,11,21,9,13,1,3,10,6,7,1,2,2,18,5,5,1,9,9,2,68,9,19,13,2,5,
        1,4,4,7,4,13,3,9,10,21,17,3,26,2,1,5,2,4,5,4,1,7,4,7,3,4,2,1,6,1,1,20,4,1,9,2,2,1,3,3,2,3,2,1,1,1,20,2,3,1,6,2,3,6,2,4,8,1,3,2,10,3,5,3,4,4,
        3,4,16,1,6,1,10,2,4,2,1,1,2,10,11,2,2,3,1,24,31,4,10,10,2,5,12,16,164,15,4,16,7,9,15,19,17,1,2,1,1,5,1,1,1,1,1,3,1,4,3,1,3,1,3,1,2,1,1,3,3,7,
        2,8,1,2,2,2,1,3,4,3,7,8,12,92,2,10,3,1,3,14,5,25,16,42,4,7,7,4,2,21,5,27,26,27,21,25,30,31,2,1,5,13,3,22,5,6,6,11,9,12,1,5,9,7,5,5,22,60,3,5,
        13,1,1,8,1,1,3,3,2,1,9,3,3,18,4,1,2,3,7,6,3,1,2,3,9,1,3,1,3,2,1,3,1,1,1,2,1,11,3,1,6,9,1,3,2,3,1,2,1,5,1,1,4,3,4,1,2,2,4,4,1,7,2,1,2,2,3,5,13,
        18,3,4,14,9,9,4,16,3,7,5,8,2,6,48,28,3,1,1,4,2,14,8,2,9,2,1,15,2,4,3,2,10,16,12,8,7,1,1,3,1,1,1,2,7,4,1,6,4,38,39,16,23,7,15,15,3,2,12,7,21,
        37,27,6,5,4,8,2,10,8,8,6,5,1,2,1,3,24,1,16,17,9,23,10,17,6,1,51,55,44,13,294,9,3,6,2,4,2,2,15,1,1,1,13,21,17,68,14,8,9,4,1,4,9,3,11,7,1,1,1,
        5,6,3,2,1,1,1,2,3,8,1,2,2,4,1,5,5,2,1,4,3,7,13,4,1,4,1,3,1,1,1,5,5,10,1,6,1,5,2,1,5,2,4,1,4,5,7,3,18,2,9,11,32,4,3,3,2,4,7,11,16,9,11,8,13,38,
        32,8,4,2,1,1,2,1,2,4,4,1,1,1,4,1,21,3,11,1,16,1,1,6,1,3,2,4,9,8,57,7,44,1,3,3,13,3,10,1,1,7,5,2,7,21,47,63,3,15,4,7,1,16,1,1,2,8,2,3,42,15,4,
        1,29,7,22,10,3,78,16,12,20,18,4,67,11,5,1,3,15,6,21,31,32,27,18,13,71,35,5,142,4,10,1,2,50,19,33,16,35,37,16,19,27,7,1,133,19,1,4,8,7,20,1,4,
        4,1,10,3,1,6,1,2,51,5,40,15,24,43,22928,11,1,13,154,70,3,1,1,7,4,10,1,2,1,1,2,1,2,1,2,2,1,1,2,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,
        3,2,1,1,1,1,2,1,1,
    };
    static ImWchar base_ranges[] = // not zero-terminated
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
        0x31F0, 0x31FF, // Katakana Phonetic Extensions
        0xFF00, 0xFFEF, // Half-width characters
        0xFFFD, 0xFFFD  // Invalid
    };
    static ImWchar full_ranges[IM_ARRAYSIZE(base_ranges) + IM_ARRAYSIZE(accumulative_offsets_from_0x4E00)*2 + 1] = { 0 };
    if (!full_ranges[0])
    {
        memcpy(full_ranges, base_ranges, sizeof(base_ranges));
        UnpackAccumulativeOffsetsIntoRanges(0x4E00, accumulative_offsets_from_0x4E00, IM_ARRAYSIZE(accumulative_offsets_from_0x4E00), full_ranges + IM_ARRAYSIZE(base_ranges));
    }
    return &full_ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesCyrillic()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesThai()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin
        0x2010, 0x205E, // Punctuations
        0x0E00, 0x0E7F, // Thai
        0,
    };
    return &ranges[0];
}

const ImWchar*  ImFontAtlas::GetGlyphRangesVietnamese()
{
    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin
        0x0102, 0x0103,
        0x0110, 0x0111,
        0x0128, 0x0129,
        0x0168, 0x0169,
        0x01A0, 0x01A1,
        0x01AF, 0x01B0,
        0x1EA0, 0x1EF9,
        0,
    };
    return &ranges[0];
}

//-----------------------------------------------------------------------------
// [SECTION] ImFontGlyphRangesBuilder
//-----------------------------------------------------------------------------

void ImFontGlyphRangesBuilder::AddText(const char* text, const char* text_end)
{
    while (text_end ? (text < text_end) : *text)
    {
        unsigned int c = 0;
        int c_len = ImTextCharFromUtf8(&c, text, text_end);
        text += c_len;
        if (c_len == 0)
            break;
        AddChar((ImWchar)c);
    }
}

void ImFontGlyphRangesBuilder::AddRanges(const ImWchar* ranges)
{
    for (; ranges[0]; ranges += 2)
        for (ImWchar c = ranges[0]; c <= ranges[1]; c++)
            AddChar(c);
}

void ImFontGlyphRangesBuilder::BuildRanges(ImVector<ImWchar>* out_ranges)
{
    const int max_codepoint = IM_UNICODE_CODEPOINT_MAX;
    for (int n = 0; n <= max_codepoint; n++)
        if (GetBit(n))
        {
            out_ranges->push_back((ImWchar)n);
            while (n < max_codepoint && GetBit(n + 1))
                n++;
            out_ranges->push_back((ImWchar)n);
        }
    out_ranges->push_back(0);
}

//-----------------------------------------------------------------------------
// [SECTION] ImFont
//-----------------------------------------------------------------------------

ImFont::ImFont()
{
    FontSize = 0.0f;
    FallbackAdvanceX = 0.0f;
    FallbackChar = (ImWchar)-1;
    EllipsisChar = (ImWchar)-1;
    DotChar = (ImWchar)-1;
    FallbackGlyph = NULL;
    ContainerAtlas = NULL;
    ConfigData = NULL;
    ConfigDataCount = 0;
    DirtyLookupTables = false;
    Scale = 1.0f;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
    memset(Used4kPagesMap, 0, sizeof(Used4kPagesMap));
}

ImFont::~ImFont()
{
    ClearOutputData();
}

void    ImFont::ClearOutputData()
{
    FontSize = 0.0f;
    FallbackAdvanceX = 0.0f;
    Glyphs.clear();
    IndexAdvanceX.clear();
    IndexLookup.clear();
    FallbackGlyph = NULL;
    ContainerAtlas = NULL;
    DirtyLookupTables = true;
    Ascent = Descent = 0.0f;
    MetricsTotalSurface = 0;
}

static ImWchar FindFirstExistingGlyph(ImFont* font, const ImWchar* candidate_chars, int candidate_chars_count)
{
    for (int n = 0; n < candidate_chars_count; n++)
        if (font->FindGlyphNoFallback(candidate_chars[n]) != NULL)
            return candidate_chars[n];
    return (ImWchar)-1;
}

void ImFont::BuildLookupTable()
{
    int max_codepoint = 0;
    for (int i = 0; i != Glyphs.Size; i++)
        max_codepoint = ImMax(max_codepoint, (int)Glyphs[i].Codepoint);

    // Build lookup table
    IM_ASSERT(Glyphs.Size < 0xFFFF); // -1 is reserved
    IndexAdvanceX.clear();
    IndexLookup.clear();
    DirtyLookupTables = false;
    memset(Used4kPagesMap, 0, sizeof(Used4kPagesMap));
    GrowIndex(max_codepoint + 1);
    for (int i = 0; i < Glyphs.Size; i++)
    {
        int codepoint = (int)Glyphs[i].Codepoint;
        IndexAdvanceX[codepoint] = Glyphs[i].AdvanceX;
        IndexLookup[codepoint] = (ImWchar)i;

        // Mark 4K page as used
        const int page_n = codepoint / 4096;
        Used4kPagesMap[page_n >> 3] |= 1 << (page_n & 7);
    }

    // Create a glyph to handle TAB
    // FIXME: Needs proper TAB handling but it needs to be contextualized (or we could arbitrary say that each string starts at "column 0" ?)
    if (FindGlyph((ImWchar)' '))
    {
        if (Glyphs.back().Codepoint != '\t')   // So we can call this function multiple times (FIXME: Flaky)
            Glyphs.resize(Glyphs.Size + 1);
        ImFontGlyph& tab_glyph = Glyphs.back();
        tab_glyph = *FindGlyph((ImWchar)' ');
        tab_glyph.Codepoint = '\t';
        tab_glyph.AdvanceX *= IM_TABSIZE;
        IndexAdvanceX[(int)tab_glyph.Codepoint] = (float)tab_glyph.AdvanceX;
        IndexLookup[(int)tab_glyph.Codepoint] = (ImWchar)(Glyphs.Size - 1);
    }

    // Mark special glyphs as not visible (note that AddGlyph already mark as non-visible glyphs with zero-size polygons)
    SetGlyphVisible((ImWchar)' ', false);
    SetGlyphVisible((ImWchar)'\t', false);

    // Ellipsis character is required for rendering elided text. We prefer using U+2026 (horizontal ellipsis).
    // However some old fonts may contain ellipsis at U+0085. Here we auto-detect most suitable ellipsis character.
    // FIXME: Note that 0x2026 is rarely included in our font ranges. Because of this we are more likely to use three individual dots.
    const ImWchar ellipsis_chars[] = { (ImWchar)0x2026, (ImWchar)0x0085 };
    const ImWchar dots_chars[] = { (ImWchar)'.', (ImWchar)0xFF0E };
    if (EllipsisChar == (ImWchar)-1)
        EllipsisChar = FindFirstExistingGlyph(this, ellipsis_chars, IM_ARRAYSIZE(ellipsis_chars));
    if (DotChar == (ImWchar)-1)
        DotChar = FindFirstExistingGlyph(this, dots_chars, IM_ARRAYSIZE(dots_chars));

    // Setup fallback character
    const ImWchar fallback_chars[] = { (ImWchar)IM_UNICODE_CODEPOINT_INVALID, (ImWchar)'?', (ImWchar)' ' };
    FallbackGlyph = FindGlyphNoFallback(FallbackChar);
    if (FallbackGlyph == NULL)
    {
        FallbackChar = FindFirstExistingGlyph(this, fallback_chars, IM_ARRAYSIZE(fallback_chars));
        FallbackGlyph = FindGlyphNoFallback(FallbackChar);
        if (FallbackGlyph == NULL)
        {
            FallbackGlyph = &Glyphs.back();
            FallbackChar = (ImWchar)FallbackGlyph->Codepoint;
        }
    }

    FallbackAdvanceX = FallbackGlyph->AdvanceX;
    for (int i = 0; i < max_codepoint + 1; i++)
        if (IndexAdvanceX[i] < 0.0f)
            IndexAdvanceX[i] = FallbackAdvanceX;
}

// API is designed this way to avoid exposing the 4K page size
// e.g. use with IsGlyphRangeUnused(0, 255)
bool ImFont::IsGlyphRangeUnused(unsigned int c_begin, unsigned int c_last)
{
    unsigned int page_begin = (c_begin / 4096);
    unsigned int page_last = (c_last / 4096);
    for (unsigned int page_n = page_begin; page_n <= page_last; page_n++)
        if ((page_n >> 3) < sizeof(Used4kPagesMap))
            if (Used4kPagesMap[page_n >> 3] & (1 << (page_n & 7)))
                return false;
    return true;
}

void ImFont::SetGlyphVisible(ImWchar c, bool visible)
{
    if (ImFontGlyph* glyph = (ImFontGlyph*)(void*)FindGlyph((ImWchar)c))
        glyph->Visible = visible ? 1 : 0;
}

void ImFont::GrowIndex(int new_size)
{
    IM_ASSERT(IndexAdvanceX.Size == IndexLookup.Size);
    if (new_size <= IndexLookup.Size)
        return;
    IndexAdvanceX.resize(new_size, -1.0f);
    IndexLookup.resize(new_size, (ImWchar)-1);
}

// x0/y0/x1/y1 are offset from the character upper-left layout position, in pixels. Therefore x0/y0 are often fairly close to zero.
// Not to be mistaken with texture coordinates, which are held by u0/v0/u1/v1 in normalized format (0.0..1.0 on each texture axis).
// 'cfg' is not necessarily == 'this->ConfigData' because multiple source fonts+configs can be used to build one target font.
void ImFont::AddGlyph(const ImFontConfig* cfg, ImWchar codepoint, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x)
{
    if (cfg != NULL)
    {
        // Clamp & recenter if needed
        const float advance_x_original = advance_x;
        advance_x = ImClamp(advance_x, cfg->GlyphMinAdvanceX, cfg->GlyphMaxAdvanceX);
        if (advance_x != advance_x_original)
        {
            float char_off_x = cfg->PixelSnapH ? ImFloor((advance_x - advance_x_original) * 0.5f) : (advance_x - advance_x_original) * 0.5f;
            x0 += char_off_x;
            x1 += char_off_x;
        }

        // Snap to pixel
        if (cfg->PixelSnapH)
            advance_x = IM_ROUND(advance_x);

        // Bake spacing
        advance_x += cfg->GlyphExtraSpacing.x;
    }

    Glyphs.resize(Glyphs.Size + 1);
    ImFontGlyph& glyph = Glyphs.back();
    glyph.Codepoint = (unsigned int)codepoint;
    glyph.Visible = (x0 != x1) && (y0 != y1);
    glyph.Colored = false;
    glyph.X0 = x0;
    glyph.Y0 = y0;
    glyph.X1 = x1;
    glyph.Y1 = y1;
    glyph.U0 = u0;
    glyph.V0 = v0;
    glyph.U1 = u1;
    glyph.V1 = v1;
    glyph.AdvanceX = advance_x;

    // Compute rough surface usage metrics (+1 to account for average padding, +0.99 to round)
    // We use (U1-U0)*TexWidth instead of X1-X0 to account for oversampling.
    float pad = ContainerAtlas->TexGlyphPadding + 0.99f;
    DirtyLookupTables = true;
    MetricsTotalSurface += (int)((glyph.U1 - glyph.U0) * ContainerAtlas->TexWidth + pad) * (int)((glyph.V1 - glyph.V0) * ContainerAtlas->TexHeight + pad);
}

void ImFont::AddRemapChar(ImWchar dst, ImWchar src, bool overwrite_dst)
{
    IM_ASSERT(IndexLookup.Size > 0);    // Currently this can only be called AFTER the font has been built, aka after calling ImFontAtlas::GetTexDataAs*() function.
    unsigned int index_size = (unsigned int)IndexLookup.Size;

    if (dst < index_size && IndexLookup.Data[dst] == (ImWchar)-1 && !overwrite_dst) // 'dst' already exists
        return;
    if (src >= index_size && dst >= index_size) // both 'dst' and 'src' don't exist -> no-op
        return;

    GrowIndex(dst + 1);
    IndexLookup[dst] = (src < index_size) ? IndexLookup.Data[src] : (ImWchar)-1;
    IndexAdvanceX[dst] = (src < index_size) ? IndexAdvanceX.Data[src] : 1.0f;
}

const ImFontGlyph* ImFont::FindGlyph(ImWchar c) const
{
    if (c >= (size_t)IndexLookup.Size)
        return FallbackGlyph;
    const ImWchar i = IndexLookup.Data[c];
    if (i == (ImWchar)-1)
        return FallbackGlyph;
    return &Glyphs.Data[i];
}

const ImFontGlyph* ImFont::FindGlyphNoFallback(ImWchar c) const
{
    if (c >= (size_t)IndexLookup.Size)
        return NULL;
    const ImWchar i = IndexLookup.Data[c];
    if (i == (ImWchar)-1)
        return NULL;
    return &Glyphs.Data[i];
}

const char* ImFont::CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const
{
    // Simple word-wrapping for English, not full-featured. Please submit failing cases!
    // FIXME: Much possible improvements (don't cut things like "word !", "word!!!" but cut within "word,,,,", more sensible support for punctuations, support for Unicode punctuations, etc.)

    // For references, possible wrap point marked with ^
    //  "aaa bbb, ccc,ddd. eee   fff. ggg!"
    //      ^    ^    ^   ^   ^__    ^    ^

    // List of hardcoded separators: .,;!?'"

    // Skip extra blanks after a line returns (that includes not counting them in width computation)
    // e.g. "Hello    world" --> "Hello" "World"

    // Cut words that cannot possibly fit within one line.
    // e.g.: "The tropical fish" with ~5 characters worth of width --> "The tr" "opical" "fish"

    float line_width = 0.0f;
    float word_width = 0.0f;
    float blank_width = 0.0f;
    wrap_width /= scale; // We work with unscaled widths to avoid scaling every characters

    const char* word_end = text;
    const char* prev_word_end = NULL;
    bool inside_word = true;

    const char* s = text;
    while (s < text_end)
    {
        unsigned int c = (unsigned int)*s;
        const char* next_s;
        if (c < 0x80)
            next_s = s + 1;
        else
            next_s = s + ImTextCharFromUtf8(&c, s, text_end);
        if (c == 0)
            break;

        if (c < 32)
        {
            if (c == '\n')
            {
                line_width = word_width = blank_width = 0.0f;
                inside_word = true;
                s = next_s;
                continue;
            }
            if (c == '\r')
            {
                s = next_s;
                continue;
            }
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX);
        if (ImCharIsBlankW(c))
        {
            if (inside_word)
            {
                line_width += blank_width;
                blank_width = 0.0f;
                word_end = s;
            }
            blank_width += char_width;
            inside_word = false;
        }
        else
        {
            word_width += char_width;
            if (inside_word)
            {
                word_end = next_s;
            }
            else
            {
                prev_word_end = word_end;
                line_width += word_width + blank_width;
                word_width = blank_width = 0.0f;
            }

            // Allow wrapping after punctuation.
            inside_word = (c != '.' && c != ',' && c != ';' && c != '!' && c != '?' && c != '\"');
        }

        // We ignore blank width at the end of the line (they can be skipped)
        if (line_width + word_width > wrap_width)
        {
            // Words that cannot possibly fit within an entire line will be cut anywhere.
            if (word_width < wrap_width)
                s = prev_word_end ? prev_word_end : word_end;
            break;
        }

        s = next_s;
    }

    return s;
}

ImVec2 ImFont::CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) const
{
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // FIXME-OPT: Need to avoid this.

    const float line_height = size;
    const float scale = size / FontSize;

    ImVec2 text_size = ImVec2(0, 0);
    float line_width = 0.0f;

    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    const char* s = text_begin;
    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            if (!word_wrap_eol)
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - line_width);
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
            }

            if (s >= word_wrap_eol)
            {
                if (text_size.x < line_width)
                    text_size.x = line_width;
                text_size.y += line_height;
                line_width = 0.0f;
                word_wrap_eol = NULL;

                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (ImCharIsBlankA(c)) { s++; } else if (c == '\n') { s++; break; } else { break; }
                }
                continue;
            }
        }

        // Decode and advance source
        const char* prev_s = s;
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += ImTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                text_size.x = ImMax(text_size.x, line_width);
                text_size.y += line_height;
                line_width = 0.0f;
                continue;
            }
            if (c == '\r')
                continue;
        }

        const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX.Data[c] : FallbackAdvanceX) * scale;
        if (line_width + char_width >= max_width)
        {
            s = prev_s;
            break;
        }

        line_width += char_width;
    }

    if (text_size.x < line_width)
        text_size.x = line_width;

    if (line_width > 0 || text_size.y == 0.0f)
        text_size.y += line_height;

    if (remaining)
        *remaining = s;

    return text_size;
}

// Note: as with every ImDrawList drawing function, this expects that the font atlas texture is bound.
void ImFont::RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, ImWchar c) const
{
    const ImFontGlyph* glyph = FindGlyph(c);
    if (!glyph || !glyph->Visible)
        return;
    if (glyph->Colored)
        col |= ~IM_COL32_A_MASK;
    float scale = (size >= 0.0f) ? (size / FontSize) : 1.0f;
    pos.x = IM_FLOOR(pos.x);
    pos.y = IM_FLOOR(pos.y);
    draw_list->PrimReserve(6, 4);
    draw_list->PrimRectUV(ImVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale), ImVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale), ImVec2(glyph->U0, glyph->V0), ImVec2(glyph->U1, glyph->V1), col);
}

// Note: as with every ImDrawList drawing function, this expects that the font atlas texture is bound.
void ImFont::RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) const
{
    if (!text_end)
        text_end = text_begin + strlen(text_begin); // ImGui:: functions generally already provides a valid text_end, so this is merely to handle direct calls.

    // Align to be pixel perfect
    pos.x = IM_FLOOR(pos.x);
    pos.y = IM_FLOOR(pos.y);
    float x = pos.x;
    float y = pos.y;
    if (y > clip_rect.w)
        return;

    const float scale = size / FontSize;
    const float line_height = FontSize * scale;
    const bool word_wrap_enabled = (wrap_width > 0.0f);
    const char* word_wrap_eol = NULL;

    // Fast-forward to first visible line
    const char* s = text_begin;
    if (y + line_height < clip_rect.y && !word_wrap_enabled)
        while (y + line_height < clip_rect.y && s < text_end)
        {
            s = (const char*)memchr(s, '\n', text_end - s);
            s = s ? s + 1 : text_end;
            y += line_height;
        }

    // For large text, scan for the last visible line in order to avoid over-reserving in the call to PrimReserve()
    // Note that very large horizontal line will still be affected by the issue (e.g. a one megabyte string buffer without a newline will likely crash atm)
    if (text_end - s > 10000 && !word_wrap_enabled)
    {
        const char* s_end = s;
        float y_end = y;
        while (y_end < clip_rect.w && s_end < text_end)
        {
            s_end = (const char*)memchr(s_end, '\n', text_end - s_end);
            s_end = s_end ? s_end + 1 : text_end;
            y_end += line_height;
        }
        text_end = s_end;
    }
    if (s == text_end)
        return;

    // Reserve vertices for remaining worse case (over-reserving is useful and easily amortized)
    const int vtx_count_max = (int)(text_end - s) * 4;
    const int idx_count_max = (int)(text_end - s) * 6;
    const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;
    draw_list->PrimReserve(idx_count_max, vtx_count_max);

    ImDrawVert* vtx_write = draw_list->_VtxWritePtr;
    ImDrawIdx* idx_write = draw_list->_IdxWritePtr;
    unsigned int vtx_current_idx = draw_list->_VtxCurrentIdx;

    const ImU32 col_untinted = col | ~IM_COL32_A_MASK;

    while (s < text_end)
    {
        if (word_wrap_enabled)
        {
            // Calculate how far we can render. Requires two passes on the string data but keeps the code simple and not intrusive for what's essentially an uncommon feature.
            if (!word_wrap_eol)
            {
                word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - pos.x));
                if (word_wrap_eol == s) // Wrap_width is too small to fit anything. Force displaying 1 character to minimize the height discontinuity.
                    word_wrap_eol++;    // +1 may not be a character start point in UTF-8 but it's ok because we use s >= word_wrap_eol below
            }

            if (s >= word_wrap_eol)
            {
                x = pos.x;
                y += line_height;
                word_wrap_eol = NULL;

                // Wrapping skips upcoming blanks
                while (s < text_end)
                {
                    const char c = *s;
                    if (ImCharIsBlankA(c)) { s++; } else if (c == '\n') { s++; break; } else { break; }
                }
                continue;
            }
        }

        // Decode and advance source
        unsigned int c = (unsigned int)*s;
        if (c < 0x80)
        {
            s += 1;
        }
        else
        {
            s += ImTextCharFromUtf8(&c, s, text_end);
            if (c == 0) // Malformed UTF-8?
                break;
        }

        if (c < 32)
        {
            if (c == '\n')
            {
                x = pos.x;
                y += line_height;
                if (y > clip_rect.w)
                    break; // break out of main loop
                continue;
            }
            if (c == '\r')
                continue;
        }

        const ImFontGlyph* glyph = FindGlyph((ImWchar)c);
        if (glyph == NULL)
            continue;

        float char_width = glyph->AdvanceX * scale;
        if (glyph->Visible)
        {
            // We don't do a second finer clipping test on the Y axis as we've already skipped anything before clip_rect.y and exit once we pass clip_rect.w
            float x1 = x + glyph->X0 * scale;
            float x2 = x + glyph->X1 * scale;
            float y1 = y + glyph->Y0 * scale;
            float y2 = y + glyph->Y1 * scale;
            if (x1 <= clip_rect.z && x2 >= clip_rect.x)
            {
                // Render a character
                float u1 = glyph->U0;
                float v1 = glyph->V0;
                float u2 = glyph->U1;
                float v2 = glyph->V1;

                // CPU side clipping used to fit text in their frame when the frame is too small. Only does clipping for axis aligned quads.
                if (cpu_fine_clip)
                {
                    if (x1 < clip_rect.x)
                    {
                        u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
                        x1 = clip_rect.x;
                    }
                    if (y1 < clip_rect.y)
                    {
                        v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
                        y1 = clip_rect.y;
                    }
                    if (x2 > clip_rect.z)
                    {
                        u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
                        x2 = clip_rect.z;
                    }
                    if (y2 > clip_rect.w)
                    {
                        v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
                        y2 = clip_rect.w;
                    }
                    if (y1 >= y2)
                    {
                        x += char_width;
                        continue;
                    }
                }

                // Support for untinted glyphs
                ImU32 glyph_col = glyph->Colored ? col_untinted : col;

                // We are NOT calling PrimRectUV() here because non-inlined causes too much overhead in a debug builds. Inlined here:
                {
                    idx_write[0] = (ImDrawIdx)(vtx_current_idx); idx_write[1] = (ImDrawIdx)(vtx_current_idx+1); idx_write[2] = (ImDrawIdx)(vtx_current_idx+2);
                    idx_write[3] = (ImDrawIdx)(vtx_current_idx); idx_write[4] = (ImDrawIdx)(vtx_current_idx+2); idx_write[5] = (ImDrawIdx)(vtx_current_idx+3);
                    vtx_write[0].pos.x = x1; vtx_write[0].pos.y = y1; vtx_write[0].col = glyph_col; vtx_write[0].uv.x = u1; vtx_write[0].uv.y = v1;
                    vtx_write[1].pos.x = x2; vtx_write[1].pos.y = y1; vtx_write[1].col = glyph_col; vtx_write[1].uv.x = u2; vtx_write[1].uv.y = v1;
                    vtx_write[2].pos.x = x2; vtx_write[2].pos.y = y2; vtx_write[2].col = glyph_col; vtx_write[2].uv.x = u2; vtx_write[2].uv.y = v2;
                    vtx_write[3].pos.x = x1; vtx_write[3].pos.y = y2; vtx_write[3].col = glyph_col; vtx_write[3].uv.x = u1; vtx_write[3].uv.y = v2;
                    vtx_write += 4;
                    vtx_current_idx += 4;
                    idx_write += 6;
                }
            }
        }
        x += char_width;
    }

    // Give back unused vertices (clipped ones, blanks) ~ this is essentially a PrimUnreserve() action.
    draw_list->VtxBuffer.Size = (int)(vtx_write - draw_list->VtxBuffer.Data); // Same as calling shrink()
    draw_list->IdxBuffer.Size = (int)(idx_write - draw_list->IdxBuffer.Data);
    draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ElemCount -= (idx_expected_size - draw_list->IdxBuffer.Size);
    draw_list->_VtxWritePtr = vtx_write;
    draw_list->_IdxWritePtr = idx_write;
    draw_list->_VtxCurrentIdx = vtx_current_idx;
}

//-----------------------------------------------------------------------------
// [SECTION] ImGui Internal Render Helpers
//-----------------------------------------------------------------------------
// Vaguely redesigned to stop accessing ImGui global state:
// - RenderArrow()
// - RenderBullet()
// - RenderCheckMark()
// - RenderMouseCursor()
// - RenderArrowPointingAt()
// - RenderRectFilledRangeH()
// - RenderRectFilledWithHole()
//-----------------------------------------------------------------------------
// Function in need of a redesign (legacy mess)
// - RenderColorRectWithAlphaCheckerboard()
//-----------------------------------------------------------------------------

// Render an arrow aimed to be aligned with text (p_min is a position in the same space text would be positioned). To e.g. denote expanded/collapsed state
void ImGui::RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImU32 col, ImGuiDir dir, float scale)
{
    const float h = draw_list->_Data->FontSize * 1.00f;
    float r = h * 0.40f * scale;
    ImVec2 center = pos + ImVec2(h * 0.50f, h * 0.50f * scale);

    ImVec2 a, b, c;
    switch (dir)
    {
    case ImGuiDir_Up:
    case ImGuiDir_Down:
        if (dir == ImGuiDir_Up) r = -r;
        a = ImVec2(+0.000f, +0.750f) * r;
        b = ImVec2(-0.866f, -0.750f) * r;
        c = ImVec2(+0.866f, -0.750f) * r;
        break;
    case ImGuiDir_Left:
    case ImGuiDir_Right:
        if (dir == ImGuiDir_Left) r = -r;
        a = ImVec2(+0.750f, +0.000f) * r;
        b = ImVec2(-0.750f, +0.866f) * r;
        c = ImVec2(-0.750f, -0.866f) * r;
        break;
    case ImGuiDir_None:
    case ImGuiDir_COUNT:
        IM_ASSERT(0);
        break;
    }
    draw_list->AddTriangleFilled(center + a, center + b, center + c, col);
}

void ImGui::RenderBullet(ImDrawList* draw_list, ImVec2 pos, ImU32 col)
{
    draw_list->AddCircleFilled(pos, draw_list->_Data->FontSize * 0.20f, col, 8);
}

void ImGui::RenderCheckMark(ImDrawList* draw_list, ImVec2 pos, ImU32 col, float sz)
{
    float thickness = ImMax(sz / 5.0f, 1.0f);
    sz -= thickness * 0.5f;
    pos += ImVec2(thickness * 0.25f, thickness * 0.25f);

    float third = sz / 3.0f;
    float bx = pos.x + third;
    float by = pos.y + sz - third * 0.5f;
    draw_list->PathLineTo(ImVec2(bx - third, by - third));
    draw_list->PathLineTo(ImVec2(bx, by));
    draw_list->PathLineTo(ImVec2(bx + third * 2.0f, by - third * 2.0f));
    draw_list->PathStroke(col, 0, thickness);
}

void ImGui::RenderMouseCursor(ImDrawList* draw_list, ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor, ImU32 col_fill, ImU32 col_border, ImU32 col_shadow)
{
    if (mouse_cursor == ImGuiMouseCursor_None)
        return;
    IM_ASSERT(mouse_cursor > ImGuiMouseCursor_None && mouse_cursor < ImGuiMouseCursor_COUNT);

    ImFontAtlas* font_atlas = draw_list->_Data->Font->ContainerAtlas;
    ImVec2 offset, size, uv[4];
    if (font_atlas->GetMouseCursorTexData(mouse_cursor, &offset, &size, &uv[0], &uv[2]))
    {
        pos -= offset;
        ImTextureID tex_id = font_atlas->TexID;
        draw_list->PushTextureID(tex_id);
        draw_list->AddImage(tex_id, pos + ImVec2(1, 0) * scale, pos + (ImVec2(1, 0) + size) * scale,    uv[2], uv[3], col_shadow);
        draw_list->AddImage(tex_id, pos + ImVec2(2, 0) * scale, pos + (ImVec2(2, 0) + size) * scale,    uv[2], uv[3], col_shadow);
        draw_list->AddImage(tex_id, pos,                        pos + size * scale,                     uv[2], uv[3], col_border);
        draw_list->AddImage(tex_id, pos,                        pos + size * scale,                     uv[0], uv[1], col_fill);
        draw_list->PopTextureID();
    }
}

// Render an arrow. 'pos' is position of the arrow tip. half_sz.x is length from base to tip. half_sz.y is length on each side.
void ImGui::RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col)
{
    switch (direction)
    {
    case ImGuiDir_Left:  draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), pos, col); return;
    case ImGuiDir_Right: draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), pos, col); return;
    case ImGuiDir_Up:    draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), pos, col); return;
    case ImGuiDir_Down:  draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), pos, col); return;
    case ImGuiDir_None: case ImGuiDir_COUNT: break; // Fix warnings
    }
}

static inline float ImAcos01(float x)
{
    if (x <= 0.0f) return IM_PI * 0.5f;
    if (x >= 1.0f) return 0.0f;
    return ImAcos(x);
    //return (-0.69813170079773212f * x * x - 0.87266462599716477f) * x + 1.5707963267948966f; // Cheap approximation, may be enough for what we do.
}

// FIXME: Cleanup and move code to ImDrawList.
void ImGui::RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect& rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding)
{
    if (x_end_norm == x_start_norm)
        return;
    if (x_start_norm > x_end_norm)
        ImSwap(x_start_norm, x_end_norm);

    ImVec2 p0 = ImVec2(ImLerp(rect.Min.x, rect.Max.x, x_start_norm), rect.Min.y);
    ImVec2 p1 = ImVec2(ImLerp(rect.Min.x, rect.Max.x, x_end_norm), rect.Max.y);
    if (rounding == 0.0f)
    {
        draw_list->AddRectFilled(p0, p1, col, 0.0f);
        return;
    }

    rounding = ImClamp(ImMin((rect.Max.x - rect.Min.x) * 0.5f, (rect.Max.y - rect.Min.y) * 0.5f) - 1.0f, 0.0f, rounding);
    const float inv_rounding = 1.0f / rounding;
    const float arc0_b = ImAcos01(1.0f - (p0.x - rect.Min.x) * inv_rounding);
    const float arc0_e = ImAcos01(1.0f - (p1.x - rect.Min.x) * inv_rounding);
    const float half_pi = IM_PI * 0.5f; // We will == compare to this because we know this is the exact value ImAcos01 can return.
    const float x0 = ImMax(p0.x, rect.Min.x + rounding);
    if (arc0_b == arc0_e)
    {
        draw_list->PathLineTo(ImVec2(x0, p1.y));
        draw_list->PathLineTo(ImVec2(x0, p0.y));
    }
    else if (arc0_b == 0.0f && arc0_e == half_pi)
    {
        draw_list->PathArcToFast(ImVec2(x0, p1.y - rounding), rounding, 3, 6); // BL
        draw_list->PathArcToFast(ImVec2(x0, p0.y + rounding), rounding, 6, 9); // TR
    }
    else
    {
        draw_list->PathArcTo(ImVec2(x0, p1.y - rounding), rounding, IM_PI - arc0_e, IM_PI - arc0_b, 3); // BL
        draw_list->PathArcTo(ImVec2(x0, p0.y + rounding), rounding, IM_PI + arc0_b, IM_PI + arc0_e, 3); // TR
    }
    if (p1.x > rect.Min.x + rounding)
    {
        const float arc1_b = ImAcos01(1.0f - (rect.Max.x - p1.x) * inv_rounding);
        const float arc1_e = ImAcos01(1.0f - (rect.Max.x - p0.x) * inv_rounding);
        const float x1 = ImMin(p1.x, rect.Max.x - rounding);
        if (arc1_b == arc1_e)
        {
            draw_list->PathLineTo(ImVec2(x1, p0.y));
            draw_list->PathLineTo(ImVec2(x1, p1.y));
        }
        else if (arc1_b == 0.0f && arc1_e == half_pi)
        {
            draw_list->PathArcToFast(ImVec2(x1, p0.y + rounding), rounding, 9, 12); // TR
            draw_list->PathArcToFast(ImVec2(x1, p1.y - rounding), rounding, 0, 3);  // BR
        }
        else
        {
            draw_list->PathArcTo(ImVec2(x1, p0.y + rounding), rounding, -arc1_e, -arc1_b, 3); // TR
            draw_list->PathArcTo(ImVec2(x1, p1.y - rounding), rounding, +arc1_b, +arc1_e, 3); // BR
        }
    }
    draw_list->PathFillConvex(col);
}

void ImGui::RenderRectFilledWithHole(ImDrawList* draw_list, ImRect outer, ImRect inner, ImU32 col, float rounding)
{
    const bool fill_L = (inner.Min.x > outer.Min.x);
    const bool fill_R = (inner.Max.x < outer.Max.x);
    const bool fill_U = (inner.Min.y > outer.Min.y);
    const bool fill_D = (inner.Max.y < outer.Max.y);
    if (fill_L) draw_list->AddRectFilled(ImVec2(outer.Min.x, inner.Min.y), ImVec2(inner.Min.x, inner.Max.y), col, rounding, (fill_U ? 0 : ImDrawFlags_RoundCornersTopLeft)  | (fill_D ? 0 : ImDrawFlags_RoundCornersBottomLeft));
    if (fill_R) draw_list->AddRectFilled(ImVec2(inner.Max.x, inner.Min.y), ImVec2(outer.Max.x, inner.Max.y), col, rounding, (fill_U ? 0 : ImDrawFlags_RoundCornersTopRight) | (fill_D ? 0 : ImDrawFlags_RoundCornersBottomRight));
    if (fill_U) draw_list->AddRectFilled(ImVec2(inner.Min.x, outer.Min.y), ImVec2(inner.Max.x, inner.Min.y), col, rounding, (fill_L ? 0 : ImDrawFlags_RoundCornersTopLeft)  | (fill_R ? 0 : ImDrawFlags_RoundCornersTopRight));
    if (fill_D) draw_list->AddRectFilled(ImVec2(inner.Min.x, inner.Max.y), ImVec2(inner.Max.x, outer.Max.y), col, rounding, (fill_L ? 0 : ImDrawFlags_RoundCornersBottomLeft)  | (fill_R ? 0 : ImDrawFlags_RoundCornersBottomRight));
    if (fill_L && fill_U) draw_list->AddRectFilled(ImVec2(outer.Min.x, outer.Min.y), ImVec2(inner.Min.x, inner.Min.y), col, rounding, ImDrawFlags_RoundCornersTopLeft);
    if (fill_R && fill_U) draw_list->AddRectFilled(ImVec2(inner.Max.x, outer.Min.y), ImVec2(outer.Max.x, inner.Min.y), col, rounding, ImDrawFlags_RoundCornersTopRight);
    if (fill_L && fill_D) draw_list->AddRectFilled(ImVec2(outer.Min.x, inner.Max.y), ImVec2(inner.Min.x, outer.Max.y), col, rounding, ImDrawFlags_RoundCornersBottomLeft);
    if (fill_R && fill_D) draw_list->AddRectFilled(ImVec2(inner.Max.x, inner.Max.y), ImVec2(outer.Max.x, outer.Max.y), col, rounding, ImDrawFlags_RoundCornersBottomRight);
}

// Helper for ColorPicker4()
// NB: This is rather brittle and will show artifact when rounding this enabled if rounded corners overlap multiple cells. Caller currently responsible for avoiding that.
// Spent a non reasonable amount of time trying to getting this right for ColorButton with rounding+anti-aliasing+ImGuiColorEditFlags_HalfAlphaPreview flag + various grid sizes and offsets, and eventually gave up... probably more reasonable to disable rounding altogether.
// FIXME: uses ImGui::GetColorU32
void ImGui::RenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list, ImVec2 p_min, ImVec2 p_max, ImU32 col, float grid_step, ImVec2 grid_off, float rounding, ImDrawFlags flags)
{
    if ((flags & ImDrawFlags_RoundCornersMask_) == 0)
        flags = ImDrawFlags_RoundCornersDefault_;
    if (((col & IM_COL32_A_MASK) >> IM_COL32_A_SHIFT) < 0xFF)
    {
        ImU32 col_bg1 = GetColorU32(ImAlphaBlendColors(IM_COL32(204, 204, 204, 255), col));
        ImU32 col_bg2 = GetColorU32(ImAlphaBlendColors(IM_COL32(128, 128, 128, 255), col));
        draw_list->AddRectFilled(p_min, p_max, col_bg1, rounding, flags);

        int yi = 0;
        for (float y = p_min.y + grid_off.y; y < p_max.y; y += grid_step, yi++)
        {
            float y1 = ImClamp(y, p_min.y, p_max.y), y2 = ImMin(y + grid_step, p_max.y);
            if (y2 <= y1)
                continue;
            for (float x = p_min.x + grid_off.x + (yi & 1) * grid_step; x < p_max.x; x += grid_step * 2.0f)
            {
                float x1 = ImClamp(x, p_min.x, p_max.x), x2 = ImMin(x + grid_step, p_max.x);
                if (x2 <= x1)
                    continue;
                ImDrawFlags cell_flags = ImDrawFlags_RoundCornersNone;
                if (y1 <= p_min.y) { if (x1 <= p_min.x) cell_flags |= ImDrawFlags_RoundCornersTopLeft; if (x2 >= p_max.x) cell_flags |= ImDrawFlags_RoundCornersTopRight; }
                if (y2 >= p_max.y) { if (x1 <= p_min.x) cell_flags |= ImDrawFlags_RoundCornersBottomLeft; if (x2 >= p_max.x) cell_flags |= ImDrawFlags_RoundCornersBottomRight; }

                // Combine flags
                cell_flags = (flags == ImDrawFlags_RoundCornersNone || cell_flags == ImDrawFlags_RoundCornersNone) ? ImDrawFlags_RoundCornersNone : (cell_flags & flags);
                draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col_bg2, rounding, cell_flags);
            }
        }
    }
    else
    {
        draw_list->AddRectFilled(p_min, p_max, col, rounding, flags);
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Decompression code
//-----------------------------------------------------------------------------
// Compressed with stb_compress() then converted to a C array and encoded as base85.
// Use the program in misc/fonts/binary_to_compressed_c.cpp to create the array from a TTF file.
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
// Decompression from stb.h (public domain) by Sean Barrett https://github.com/nothings/stb/blob/master/stb.h
//-----------------------------------------------------------------------------

static unsigned int stb_decompress_length(const unsigned char *input)
{
    return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
}

static unsigned char *stb__barrier_out_e, *stb__barrier_out_b;
static const unsigned char *stb__barrier_in_b;
static unsigned char *stb__dout;
static void stb__match(const unsigned char *data, unsigned int length)
{
    // INVERSE of memmove... write each byte before copying the next...
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_out_b) { stb__dout = stb__barrier_out_e+1; return; }
    while (length--) *stb__dout++ = *data++;
}

static void stb__lit(const unsigned char *data, unsigned int length)
{
    IM_ASSERT(stb__dout + length <= stb__barrier_out_e);
    if (stb__dout + length > stb__barrier_out_e) { stb__dout += length; return; }
    if (data < stb__barrier_in_b) { stb__dout = stb__barrier_out_e+1; return; }
    memcpy(stb__dout, data, length);
    stb__dout += length;
}

#define stb__in2(x)   ((i[x] << 8) + i[(x)+1])
#define stb__in3(x)   ((i[x] << 16) + stb__in2((x)+1))
#define stb__in4(x)   ((i[x] << 24) + stb__in3((x)+1))

static const unsigned char *stb_decompress_token(const unsigned char *i)
{
    if (*i >= 0x20) { // use fewer if's for cases that expand small
        if (*i >= 0x80)       stb__match(stb__dout-i[1]-1, i[0] - 0x80 + 1), i += 2;
        else if (*i >= 0x40)  stb__match(stb__dout-(stb__in2(0) - 0x4000 + 1), i[2]+1), i += 3;
        else /* *i >= 0x20 */ stb__lit(i+1, i[0] - 0x20 + 1), i += 1 + (i[0] - 0x20 + 1);
    } else { // more ifs for cases that expand large, since overhead is amortized
        if (*i >= 0x18)       stb__match(stb__dout-(stb__in3(0) - 0x180000 + 1), i[3]+1), i += 4;
        else if (*i >= 0x10)  stb__match(stb__dout-(stb__in3(0) - 0x100000 + 1), stb__in2(3)+1), i += 5;
        else if (*i >= 0x08)  stb__lit(i+2, stb__in2(0) - 0x0800 + 1), i += 2 + (stb__in2(0) - 0x0800 + 1);
        else if (*i == 0x07)  stb__lit(i+3, stb__in2(1) + 1), i += 3 + (stb__in2(1) + 1);
        else if (*i == 0x06)  stb__match(stb__dout-(stb__in3(1)+1), i[4]+1), i += 5;
        else if (*i == 0x04)  stb__match(stb__dout-(stb__in3(1)+1), stb__in2(4)+1), i += 6;
    }
    return i;
}

static unsigned int stb_adler32(unsigned int adler32, unsigned char *buffer, unsigned int buflen)
{
    const unsigned long ADLER_MOD = 65521;
    unsigned long s1 = adler32 & 0xffff, s2 = adler32 >> 16;
    unsigned long blocklen = buflen % 5552;

    unsigned long i;
    while (buflen) {
        for (i=0; i + 7 < blocklen; i += 8) {
            s1 += buffer[0], s2 += s1;
            s1 += buffer[1], s2 += s1;
            s1 += buffer[2], s2 += s1;
            s1 += buffer[3], s2 += s1;
            s1 += buffer[4], s2 += s1;
            s1 += buffer[5], s2 += s1;
            s1 += buffer[6], s2 += s1;
            s1 += buffer[7], s2 += s1;

            buffer += 8;
        }

        for (; i < blocklen; ++i)
            s1 += *buffer++, s2 += s1;

        s1 %= ADLER_MOD, s2 %= ADLER_MOD;
        buflen -= blocklen;
        blocklen = 5552;
    }
    return (unsigned int)(s2 << 16) + (unsigned int)s1;
}

static unsigned int stb_decompress(unsigned char *output, const unsigned char *i, unsigned int /*length*/)
{
    if (stb__in4(0) != 0x57bC0000) return 0;
    if (stb__in4(4) != 0)          return 0; // error! stream is > 4GB
    const unsigned int olen = stb_decompress_length(i);
    stb__barrier_in_b = i;
    stb__barrier_out_e = output + olen;
    stb__barrier_out_b = output;
    i += 16;

    stb__dout = output;
    for (;;) {
        const unsigned char *old_i = i;
        i = stb_decompress_token(i);
        if (i == old_i) {
            if (*i == 0x05 && i[1] == 0xfa) {
                IM_ASSERT(stb__dout == output + olen);
                if (stb__dout != output + olen) return 0;
                if (stb_adler32(1, output, olen) != (unsigned int) stb__in4(2))
                    return 0;
                return olen;
            } else {
                IM_ASSERT(0); /* NOTREACHED */
                return 0;
            }
        }
        IM_ASSERT(stb__dout <= output + olen);
        if (stb__dout > output + olen)
            return 0;
    }
}

//-----------------------------------------------------------------------------
// [SECTION] Default font data (ProggyClean.ttf)
//-----------------------------------------------------------------------------
// ProggyClean.ttf
// Copyright (c) 2004, 2005 Tristan Grimmer
// MIT license (see License.txt in http://www.upperbounds.net/download/ProggyClean.ttf.zip)
// Download and more information at http://upperbounds.net
//-----------------------------------------------------------------------------
// File: 'ProggyClean.ttf' (41208 bytes)
// Exported using misc/fonts/binary_to_compressed_c.cpp (with compression + base85 string encoding).
// The purpose of encoding as base85 instead of "0x00,0x01,..." style is only save on _source code_ size.
//-----------------------------------------------------------------------------
static const char proggy_clean_ttf_compressed_data_base85[11980 + 1] =
    "7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
    "2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
    "`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
    "i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
    "kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
    "*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
    "tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
    "ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
    "x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
    "CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
    "U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
    "'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
    "_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
    "Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
    "/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
    "%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
    "OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
    "h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
    "o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
    "j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
    "sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
    "eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
    "M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
    "LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
    "%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
    "Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
    "a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
    "$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
    "nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
    "7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
    ")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
    "D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
    "P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
    "bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
    "h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
    "V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
    "sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
    ".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
    "$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
    "hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
    "@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
    "w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
    "u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
    "d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
    "6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
    "b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
    ":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
    "tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
    "$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
    ":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
    "7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
    "u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
    "LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
    ":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
    "_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
    "hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
    "^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
    "+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
    "9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
    "CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
    "hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
    "8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
    "S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
    "0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
    "+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
    "M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
    "?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
    "Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
    ">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
    "[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
    "wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
    "Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
    "MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
    "i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
    "1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
    "iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
    "URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
    ";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
    "w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
    "d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
    "A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
    "/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
    "m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
    "TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
    "GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
    "O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#";
// File: 'DroidSans.ttf' (190044 bytes)
// Exported using binary_to_compressed_c.cpp
static const char DroidSans_compressed_data_base85[167935+1] =
        "7])#######i*ol@'/###I),##d-LhLYqH##$Z;99=$$$$?X[V6X?'o/(RdL<lgo<W+guH2biSxXdGxF>-DNJHFQRrm(tgS/7HJf=5`WZ_h_@4(B>uu#&]n42Wj7/SZ->>#e2M>6aNV=B"
        "HxM0hr.GY>Md68%f1XGH9N7*7-&1GDVci21_Lk7DPaaN.2xv4AZ&-p/e_(*HB<G##<l+G-Vq>Z%+1HkEYsWLR;p%##P^>>#b&#:C]Z$h'u*)##0i(##^TDUCF19sc0%i9;1=o92f'TqL"
        "`ld^=@$JY-*p_w'H3n0Ffq$Bi#D$##4+QT1gQrhFv:@PtC_%##29:kFv4S=B<m)N0mT$F78Sr92A_NDFc4Q<^d37,#o:.(#<Q#4GS3=SiDd6'#o*Y9#EAI)GC<IrL4Q&%#SkI&#hd2<#"
        "_.1lLx7<H,_1#t'o?)&+Y9w0#*/fT3.@_'#?4q.$'BwehF//NB3XQv$,8m92_^.R3ooXeuIumo%bOahuX__PM:XI+Nvt//L9k?$$F?)4#_X$0#_Rsu.c>$(#5knNO+)*)#?KXV.p(ZN#"
        "s+o:.%V%h$6AoA#H.Vp.^2WfClpE_/kIRs$4f(Z#`wp:#Uxr)#&vV*@Fsk-$fAut7<1LfLaOqXu[2Mc)raR&F(=N&#cdZk+DqL_$40r5/ISL;$7]^[MlZ/%#VZs-$rvo7R8'HcM1s:D3"
        "aa9PJ*:UfC40j%F$HMcM5C3A=[lmY6/gSD=5XTjCe]*87NFx9)Wpo7R6'LG);@]PB&928])S%pAYudG*PF3/M:BIAG#s,v?3X5,EwU'^#s#Is.EUE_&4LE.$oJ+.#='E<#sfT:#v]J=#"
        "n*O9#LPBsL6I+)#2vD<#MDZtLH=[3#&dn#M7Br#vB`_tMX]+)sbi@Mhh&:2q'MsfLWdHMp0&+2_4MTU/b+qP/(vB5SlPj]O<ELg(q*+pAX?x`4omrc<#AF)FSw$_]wNw%F[$q:8DNsf:"
        "nxqfUFiS876:US[4Jgl]ts/F%qrG/:9>r(Wj67p.XoKM^;5%g(-a$g(14L`s0E?vZYvMrZ8Uh>$@F:@-Te$]-oVwE7<)er?=k#Z-6'BdWQQ*###IKwKo`08.p=;5&ZHGwK_$QV6%JIon"
        "R4JwKjMSfC`Su8.LWxF`cU+p8DGYSJ;J[A,E'`#$XBBsL#gBx$2==e#g/5'$u*HpK3V_2#2%47/xPs[tmci+M[.AqLlC_kL?>FJM*uV7#A<vlL.xu&#]ZM4#Y'xU.2Tn8#Ko8Z.cCH;#"
        "^5T;-R(.m/XJ+.#IfT:#8eI20OB<uu#bt1K-A;dO9(9gL5?*mLq`5oL/-;hL;BH^$&nV[$krl8.Adu1KQF?-derrw'@jb._,VpV-T?S-Het]-?ulSrZp3#5A>[;w$/xw)$MH?G$#AiP/"
        "l4$]k;*MrZ5S^f`gpbPp5BJ]Fjc,<-PEvaO[R,.#K4S>-X4S>-(<iP/p=;5&1^RrZ2S<-vlNQG%P`^wK;ZYQs&A>_8>t/F%R]6F%YuPlLlN^xU1kucMuX:p.=hJH%T8Tw9bpRrZpO(##"
        "EAif('JF(sU%rGWoqOV6dHASoRWaJ2V(5Dk#NxD=&l^l8.9?vZ[[7A=mVpJVC33J:Rb$K`<kOuc0'B-mfdOPpCh[w0#ChcM7RX3N(H]YM3.AqLW&F<#ni?x-&LJ?NW2Zs-GM^%O'p)*M"
        "OHn3#/wCG%nH=&#+S<-vK?pEI&3NrZ)V#<-9MRm/lq0'#+0Z3#?A$<-PArP-sY#&PBYC5SA+QM^wA-J%lOr$#5aJ=#s%n(#fTm;#4:7Q/uO2A=UK4A=2i0^#h.&J.[gsP$8J2Q/OB<uu"
        "2x&5Ao62#Ho<3jUX74,MjTiJ#Ar:T.X@r$#rs:T.cc-0#Uq@u-$T8(MqIs*MqVG=$BdVs#5juJ$0&]5%%_`8$.t:T.Rq@p$'/r5/WUDs#+2P`MEp019qCH?$-(6?$Rv#AM,*</U^3/W-"
        "`eK-Z&ux9)&s'##$E#9.dr1A=e62Z$r%vhL_OL58>iV9VfIuv$^fBF$_H+kLEKx>-SJwA-f7ab2H?$,$xhnD$5Q4m#k&fL$9XjfL.Z*2ggWEr$SBJ]FAq_#$hpj7MJkLDN?EJ_/HZJ=#"
        "Bjewu_V'%Mp]c&#C#hS/8QZc$J3t=$+Jpw'+6vE@E<hf(vi/Z$N>Ps/F9op$f9F.%ghEB-U%sqLe%T,MnW)2/2gHG)>Cl&m165O-+j)M-%;eM0;0q#vf@^*#x<trL>Nx>-tx]7.pfdIM"
        "Q#;.9pgr22/#iD3U`p9;/X31#2Zi,#F8b9#Le/,M-,8s?C0Y[$HkDp7L(rlg=D>kF4fofL?+l<%glovLRI_*#wt]=#Ah1p.$;EG)]R$?@erV7#[-RiL4s+$M%9N;7ke(vP&u0&O^fZD3"
        "6X,mACBE/:^%e]b.hhrZUDdon>4k-$ReXvZ@%//(&b'x'r$%kt=4J_&8x#e=99DDN9BYonq;F8RC;K##r]*2gHVw?ZdcNGr2*+<$D)B&FPbQYuNnj9D1/W]40ZG_&,S?_/8`'?$p2_-M"
        "(_bl]4d,/UOnjdODP't#Mv)1%hfWt$:SF0MAm.38P'jEIsHsxc,0up'Y[1Q8ndb&#Y]W)NEHsf:2lbfClDt-$H)###fr:T.&FZ;#?4I20<8/870*>PJO<s-$Gwj4S,Or7RB#SfC$HMcM"
        "8'HcMqI2A=1ev$$YP#N#$Pik#5uTF$oT_pL?a2$%<DJvLFZZ#%sE>J$L0sD%Uo^%MfbRZ$gvUC$/og'%]Z=oLQOqkLnMpnLqp@G$0CbE$2IvZ$C_0-3([1C$KZ0F$]0QZ$FdZ,$&.hHM"
        "P<.FNLB05#Io46#G@eM0>nap#KdHO$.BmtL8e=rLu>xg#rFEjL*ecgL^N&/Npw[n9DwsY-YXp&#+####2C$_#&7YY#$,>>#.A;=-9gG<-KfG<-q/v<-R',)NPhm##SRv-$F[6D#O(b=$"
        "T0@<$&Zqr$1sjt$>lJM%AO8)9DQu98M]NYYx^XD3ji6Z>rVt-$O_N7KC#,>G+0f._Q'>`WZwL:QjOuI:itbP9&u^>$Qa8g24jsjtTm2Vd6P(>Gk_TrZ.$u1KCs&##bGDG)otJG)Hme--"
        "UpRrZe5N'#@V65&qQKucxF[w'ewh--q;dw'a``w'E8xQN>C5>5AVED*`92GDecD`WVKOpTMOvu#33ho-q+>F%mD]-#VOD,#/^a)#ju[%#&YI#M0U#oLHRuG-'d#9%:s8GDb@)_S2KnFr"
        "SOr=l/eV<%=]V<%MIvZ$pZ`=-H):53He;@$n`3=$6ONP$0ZO2$o.KkL]&PX$0_e6/5tK/%>+ofL4#a;$Jo.nL*Sm^$<X`=-Gp-A-pN#<-U4r5/NZJ=#Ve`mLdae&Mmfo8#5Y.+M4sj,#"
        "fBA5#Kp`mLZu+$Mx(4<#B:j<#6&A'0bb*x#-@Z?$&seQ-4HK[NKkw&M>bO)#+qW1#Jg;uL7L_*#5J(7#M45F%&2DF%=-vfLuGV8I-Uh>,:+[F%ZtF6#VX31#sc9%Mi/BnL9-:kLNGfqL"
        "Y:)/#Wx+wLCF?)MK:.&M(,&_NMO]s$mR92g;^m-6a%)N9a6_>$a12W-K*S9`IBRfCO0PRE]=lGM`DZ6#o7b9#5f+REu>hWJP;bK&@2,5]c]srZ)hXono4MG%XJsd+=ML5Sq9L]u.fQW-"
        "I>gQjDC28.g?/F%xs+F%v'd%OI5,>GR1g%O]s4F%VIq=#i1K*#=*%8#0;`$#ulM4##iM4#vHH;#wU$(#Exh7#1)$;#q=e0#NIr$#`/RI/DGS5#b?(pLHm&-#=uEk'Ha0j(?fD8ARd$XJ"
        "P`W>5A<E/:*s>'Zpi9M^4uwf(i^SF%dd.'mELm<-=ksK:dcX]Y1,*jT^X2+(l1?m85]@H%ujo/:4vB#$o)Cm/;X&]tQcAxt7N#<-Za>W-dN/F%D0b.$S#I`WeG-/()7h'#UfK>#)<sd*"
        "0lC?#Oi:p.YvLG)g#O1#DqUhL9SH),5n$K)LKGR&&Vte)9BOu-0Lv_+%)S1(ma8@#JEbA#]@,$,1BJ*+t?K6&5?Wh)v,>O0+S3S&)`p'+9Gp;-@<wm1]HS@#nX]T&9q4/((U:hLYwJ@#"
        "g5LB#qA,8/uIkA#eg3f-k)o0#6eNu-)U.T&2tn0#@%(+*/2TV-:S%@'=CcgL2Prh1=&Hg)IfBkLixmR&IHV-MvQ)%,C[+RE7k;FNjU0'+`Z.R3C]No&>,vg)qiwd)W&vn&FF>W.5)@i1"
        "9/Md*L9@g)Jw-Q0Lt_-M5P;P0,R%T&ESs+Mlr1p.%F)-*5w*1#[N=6/n33u-)&iB#:52kbc?n8@JvUL1go*e)EEW)+cPWe)7%6(+6_[t-1h%a+6]^'+:cST&:+,i(f4'f)?K8I*-IQk("
        "+opb*hZo@#u7+.)#;Xe)]ETM'/:m(+=68*+6Bjh)BbV)+nY<i(<WX2(r9Gf%7uc_+<8Fe)++$G*19kU.x[,(+&r5(+$am7&%]Bf)v$v(+-(h+*cOTD4)cg'+6-<L),W-n&8G7i1mL9F*"
        "n6u,M,3*-*9rMg)>0`hL7^.iL>`mk0$sMo&Sj^Q&ASG>#4EuA#dx5q&TMrl0S+gF*(5pr-<qUhLhCH>#?iJcM@O?hMU[H>#l:k**@a^7/>@Vt-Ep*jL;2d112Ur@,,sCB#q66c*x7M&0"
        "lOc(+07qb*<.RM0gk5(+nktI)Hg<jLQf7C#kDID*+IChL:bZ(+fNS@#p&o8&a'&_SWgo*+#()%,UAOjL46[(+6G'+*%u>c*di***8$o=-^@j636c'J)OJ7m-`T?tL3YSfLFtte)$xs^-"
        "P-&nj)7C)lsUfL(C0=L#w?+L)r7=I)erxB+n70f)A<g,*QDXjLr)CF*L'e68&)[sAP:Y58q^((&/D9'+JCg`Q;]rh1FBW%,HFY8/3,B;-1,QX$#JF&#Df`1)U4FGM4PG##,GlY#3oh;$"
        "+P1v#61^gL'R()N-7)=-?T9S/2cCv#0Y(Z#:b$Z$C3@@'&DFGMxUvY#8RL#$3l/gL>:>GMk_2v#mX]q)Fw(C&vY&Z$%f'^#A%d05PY(Z#fUq1MAt>`-oRSq)axc$BN.aUDmPbk4s9n0#"
        "A@NF7_IvTMOxSq`L;Pr)iduh#:?$FRiwUqV:wGt_Zn:Z$XH<L#J,n$0W6he$Oe&.-P`U;$=OBh5/qc6<v%ffL_t];/4oh;$]74,MJ/gfLhlV;$pOmH.6uq;$-qBb.C&(XqG/[$0Ri:Z#"
        "5`Tv$gl?['+?Nv$6uAgL$%[2MC.,dMOvV;$4i_;$>t?v$$o^>$SggWh0/<X(a<rB8hwrQN&$g*%W(r;$vC6hG=VmHHlJGq;Tuh;$w2QD-T5+KMtJSs$)>cY#;b>e6Qo-s$,fcs-VuRfL"
        "[tMv#_=+gLb:#gLTNLTMDD3#.q@B58usB#$E&J^#V%ffL45$##m<9C-c7L1M)PmY#0Kf05r9w0#;`b>$^9&I6`tG_&'4Gb@$nQ#$;h.Y-`g/I$VS;$^(G+gLA.gfLQnM;$3lLv#NN[w'"
        "u?n0#e%Qn*Ws%gLW_Y-M>>iP-GI]rL[O`t-_==GM*o?2M@#$u7ZMHT05VF$/3VuY#M3KtUU+.W$G1o?^`#dEnv]&D&bb9f$Gsu5SxHieM&JH##/C6C#5LWp%-M(Z#1oh;$N#k<:$5ofL"
        "uD-EMS1#,MQ9(p766?Z$p1to7LNI?pW%.W$5l_;$p/Fp75e#I6Rr-s$ux8;-MI'O.5r_;$#`'^#S,0:)Rf:Z#L`%j10]:v#2`1Z#,S1v#aF=,M3%gGMW=5,M#UucMHA.$8jhfq)eIrBS"
        "1#(3;5joC%Qi1?%Ra@fMs'q`$&x#Z$TG`pK6+#dMD_:)NU-As$0IPh%C7(gLe^)Z#HBGe-Um'1,Po$W$QT.Z$&eva$MJc>#v+TV-nQ]6aTohV$r'1O+%#t>$F3;b7-=$?$tLK58)$oNO"
        "u?n0#OcU;$;:,F%l`h--$/]fLg$s;$^;Gs-etK)Nb.gfLAb55Mp_:)NK@/W$>:Is$6(%<$j+V'8fkr2M;MSF-iG$i$Tuq;$OPtg3X?Ss$[1ofL.RcGM73O7/5SY>#j.058tS)w$2MuY#"
        "VuRfLU]fF-4;8F-.Mj+MCY1)NY5pfLs'^fLQMC@-Ngfv0/>P>#+8G>#17w8%+7$?$cF+gL6[2v#(xg>$s.TV-:&f3OU+7W$%G5s-fkK)NRmbZ-jf_$'S^@v$w*e;-xiM^NXd%u-o7'p7"
        "#4Z;%vW#C/=RQh,$5ofL9dGPM>2`e$_N@dMt8H&NMI/w71lJUD9RC#$3aW@'3GFk=5ejDYT9h1OqUGdMPZg;-=kB5'-8::)R*H&#'iBB#7[&6&DXr2M]N]s$-YL;$bR56UN,c?-<_Y-M"
        ";3/W$^4ofLw&jV$;@Rs$&rpwL0L>gLkd^t7mvT#$28Qt1&G=gLxkMv#8eQv$6e@3k#-9pp-x(t-`7ffL4XlGM>v@jLJ.gfLhIG,MU<Ss$Q<ojM>=pgLX67r79n>e6%ej-$k;g9;6>TP8"
        "O@-Z$nwHmL>>]k83dqk+SG)_]&S0)Nw9#gLh:#gL+Av<-LN`iL,x1T.;:@W$AVoo7^nB#$(wH#$R'6F%Y.r;$#YQhG,nd<CYC[s$_C`8.0Pl>#ML:Z$W(%4=4`4R*U%%W$2YjfL/:Ss$"
        "GdF?-1+m<-]=eA-Gvb?-Z.R]-a;GF%+SD*[a,/Z$5xS,Mbb)Z#B=Fk4b5]v$a@ofL[K4T%@[YHMJC=;.<C*T%_/JZ$4(@s$?.CgLP`hQ8qcQh,6>TP8x.$?$cUF,M:2il-Oqr2M4Xwm/"
        "7(VZ#.YL;$v^EnLS^MqLG%N#Mq@,gL44pfL6@j'%Z0Yh#sFwS%-SC;$(/,##5(MZ#44[8%=.r;$6(R8%[(R]-V9w0#k5kX%::e8%:7[8%<L3T%8(@s$EdB8/;RW5&eI=,MS3p7.cO=gL"
        ".:#gLOqDv#:@es$k]JjDh#*6.#]^58=Ba9D]nX$0(J4gLj]pT80+?v$pP+O4$csH?W1%<$I@hKcu?n0#qH*68*6$?$v[^58o[vZ$>@Rs$:I*9%3KToLoLU58ov(g$1Oe'OU7HdM`,QX$"
        "q3n0#^GlY#=fP##0YL;$<Ces$1u6s$cR56UX=U5%vx[fLVUbR-[[R'MBsrV$&0O<qe=Gv$)6<Z$i4Wt(D'%Z$LD+Z$L32^#0rY<-5Pfb-7]i63QGJ#$S,0:)Z+%<$7.R8%2T8c$USlY#"
        "77e8%6B:KE(RcGMCDLlL2OP,MQ<f8%3lf(N0D+T%/Wk;-ILRb%=[$Z$7TfE#0w$aN/hd'85*i?%X's;$r'Kh8&rT#$E_r2MXkTO.@[NT%kgrgLW>R+'>f5m8T`c'$.xc<-RQ:=%$>=,M"
        "ZFJs$77@W$hU=gLqxIb/9.%<$<Fes$I>4Z$j2E6.x[#Q8?-w<((VtcM@30T8(1Hv$J1Ve$cAfZ$7=3p%e@xfL4C>,MHN>gL^Ef8%>@[s$w%,w$AOw8%x_^p7^-6Z$d8Pb%0s6@'b9Mb7"
        "O/q<1U4[8%,])T.1(eS%:+TgL+2Ve$OjNs%-=wWAi^8s$7]Cv#/YC;$::IW$@[NT%fR=gLI3.e-d3Gb@V(r;$^qm2&[@RW$_%]fLF[Q>#5Ujp%T;*hYcLes$6oU;$jU4gLI&g+M*@Rm/"
        "=F[s$<@Rs$5o8gLR$j;$&[Qh,]kAm&L^<W%gR4gLp,HD-vSm&NA;7I-V;4KM`?Ss$dUbGMXJHa$&;ofLIFx8%aHE*e(M=gL/*tnLQ>5d$ZCIW$;sa;%LmW*@&A+gL4^Oo$>F<T.7uh;$"
        "Rh+sL):#gL<EAW$BhWp%Z>ojL4@Q&%c/p68sd:C&P`L;$@Cg`Q$7%mL6J(DNV$nY#Ak3t$-VuY#;L3T%H_r2M,t-&OTPmY#fM,W-Kkr2MceDv#0E`KluOM=-6[rX-Su_n<[4.<$87nS%"
        "1Ch>$0JYa8+Cm;%]Q*1#p`9H4<q.>-#':R3<(.<-g7l*.YBlERw(q;-JC7F%W47W$C2w=.@=@W$?6Fs-^@=GMlNRb%`(,Z$p7Dt-'o5Q8KNn;%Jkr2M(S,<-.DNfLW_3C&l_(W%;3j8&"
        "h)g*%jwr2MpNx8%xh5Q8@<a8&S_3C&G:ZQ81_;W%&r,Q8-Lv;%x[pP8*4?v$x3TK1?4%w#1iUv#&&###Swr2MAh6Q86qV8&hV=s%*I;W%jhk,M/E7F%2k`8&cZ388a`r8&f@xfLj4/W$"
        ":F*T%vtaN0BeWT%:(r;$`IF,MRB+T%(1J>-_v$7*/]Hs-bC+gLbL+9%*<v2M`^bp%5bd;%W[1UDt-b9%/c:v#>XNp%4Y:v#o0dqT*eC)NAAIb3?X<9%?Ies$;1iv#<[a5&C_Np%3@rT."
        "@Ln8%^YFkLAKG,MX?As$2chV$cR+gL9t)j92BPb%R^;-m^:7<$;I<p%,%rP/:Cn8%0f_;$#55W%_T*1#[I[s$tUB#$%:2W%cTw0#]_NT%?Tfq);qQv$fvSe$s9n0#)eL+%+]bGMWJ[#%"
        "[R3T%@h?v$9Ft8pS0/d;YlD<%4xH8%`=xfL$I5s-_Ot(NwBfs$];r-$bh<9%Erkl8h($Z$=p3Z$'h<nLJL>gL:BEj$FX'[Kcwsp%O>Uw-#c#Q82nV<%:=7<$A@7<$sn@'0>:R8%AIIW$"
        "l(-h-l&e<Cgam92Rgf;-x1ei$eDxV%Y7*UDW4@s$BJp<%2Y:v#F[*9%hekGMJLos$-cPW-W>Kt:(DxfL^oFT%Jqap%DkN9%PH`>$F0%Z$lS];%=%i;-kG$i$bLes$=CRs$Mx&tqvx[fL"
        "w;#gLuE=p%hV=s%'8^v$0:2j9bl;[$C3,3'1`:v#4u?s$1rus%Fwjp%aNn0#*VO,M75m)/<=Rs$[Tj-$xQ31#_X*t$Ner2MoS_Q&5=w8%Fh<9%:%`;$=f=(.a.f+MQgwI-:KuK.BeWT%"
        "3vdER-`FgLlKfs$shFgLK(A>-w,Z`--J4$g4k2[$=XNp%==e8%cCxfLas=T%Dw8Q&DRw8%:@n8%X-e8&%R<nLWYvh-fJC_&KiR,X[Z=9%AK+Z-,Z$Lc%siTpO]Oo$nZ2gL4+s;$uJj9D"
        "wEn0#hwsp%B_j5&=JQt1-fkGMQB8F-WIY58sd:C&NS1v#@Cg`Q^8XJ-.FvXMnNT_-xdh?%jV)k'pR<j0DhWT%51[8%4`Cv#'4Z;%#]K#$I::Q8+@Zv$e@ofL1s'q%jOc;%w:uS&B6R?$"
        "6+7W$>C[s$`;ov$6u6s$<B'o/8xq;$>Ins$&Gp;-C*DIMORo8%>=n8%8Pm92X1[8%7`uY#cPXs%j[XGMugt5&^En0#oMa>$qc&gL3M>gLGKMiLZT+t$x<g9;-&:r)5o^GMKe4w$^C<K3"
        "Y,IY%]F/qi`Ar-$)J+gL0,/rLU)Wv#6oUv#<($<%lq9dM]W49%U$s2MCrha%/#ew'5nQ,M`p'q%nohh,FRb?K4/[w'>Xc'&QiCZ#[hc#$gO+gL$ORb%S4&F..fX,Mlr<[8[N8U)l]fv$"
        ":1%<$&.Hv$qUdL-?XP-M':;iL(@,gLghDv#nL_lLnEx8%[&&gLVibT%&DuW%I$bT%G$Tm&#,TV-'rM*Iq2tae1UZv$dUFgL'F4v-gI+gLt6AW$pPaU-`-FCMnb'Q&3MWfLq;-n&$F5-2"
        ">oh;$3fL;$B_<9%J3T6&jrCp.A[ET%u)gAZZHx8%L_3C&o-Km&3xH8%)/,##i'cgL^WXp%4i,<%@[&Q&4pSs%Y9w0#]U39%>OPHM^#:6&bdj-$ghjp%@@IW$KQY3';X`=-xe2i-^Hn0#"
        "0Tc'&(7Z;%v)&<%kU4gLF?]s$-X;<%CbNT%$JEX(U#**GZ^k5&qq?mLspXT%BO<p%nY)+&_I7<$&GPS.ERn8%&,B;-v6*NMXk$R8%,-Z$2,/b-vGa9D&?<tU/c(T&^E(tLUJ4T%d=;ZP"
        "iGV#(PfU;$8?A&O7j5<-Vj9*9mNN9B3BGO-.[af16UA2'B%78%7@*T%,.?Q8Z-PN)dwHq$)=Zv$lX=gLIsOp%<7nS%G*06&T3tWUaw8Q&=n$?$aF4gL`L>gLYD]8%7u6s$U4/R0:%r;$"
        "B[<9%b@xfL[$S3%itAQ&9.%<$BFn8%UlCn<VlqV$Bw86&i9P'A*[.T&`Hn0#vBn0#.cFgL:XPgL4SGgLREx8%<qd;%OUn'&5xE6j+:Hv$U$PsLCb-Q8^Q7$p5:`p7hP2W%BsL4D(VGs-"
        "b4ffLp8Ss$v(KgLt1nx-nw0HM`A-n&`Q<L#8J1q7P95F%Xq%njCkdh,lE5qVj6B6&@O3T%ROY#$G7iBS`@nS%Fnsp%i_tGMt116&C$BQ&Fqsp%IfI6j[Ie8%%td>$i'-na^XET%Jan?9"
        "2,[['Bed--+JofLcaF9%@-F7/<[/m&o_=gLKVFT%6*/T&6*_<L0iFgL((tN0Fkjp%Q9KQ&nwkgLhQAW$@Ons$@In8%bKn0#X4Is$p9.KW_m0Q&'r5Q82n`W%=Fes$lwk,M92.M.D%2Z#"
        "XaFW-X3s2M#ba3M3:U6&meOgL#L>gLHrj$'V4h879:e8%HU[<$2o_v#'&###BouY#H3^Q&Gks5&B-^Q&cJFs%(=*/1B[<T%JeET%GX*9%>jq-/FL[s$u7w<(]Fes$xwOgL[ppo/@X<T%"
        "LK5n&EWrm$j,0X:-`O,MR`ld$ctsp%Fx.R3W7@W$1Zjv$Ew&q%#PDp.K''q%UI^Trv,iNXI?@['n(CT&Fhs5&'LrS&hJ];%9;C)lJEUt%_e&Q&s>a9D^Lw8%p$cgL_.q2'bslQW_X*9%"
        "Q]Fo$'jb&Of2)^'VrLv#`tSe?wN9hP4=$EN-6G0.dU'DNbN<v#MQBU%2u_;$F0^m&8i_;$<+%W$h06aN3nw6/F$0q%leOgL'^]W$A'Tm&3Ui/:SjR5'a-)?$wb,Q8]liW%:7.<$?qS2'"
        "_#/Z$N.+R-2Im=M'4pfLUUFT%ftmp&<@IW$ehbk1=UET%8oqV$C$96&lk'HM=*[Q80ei8&gZw0#/lXgLrW=9%Uml<:/obgLn1-L--TYhNKS=T%/3Wb$W.;W-771gaE*)iLhW=9%BKSq)"
        "^#19.:rU;$FRE*[Z:[8%uF0p77QV8&6EQ3kKIdQ8Y1(KE?,`l-$cq;Ag/U6&cGRhGa[ap%MHgQ&XW5UVX@nS%-n7t%@_*t$FOI<$un7b/E[eW$J6K6&KS'W%]w.[B]Le8%]m`>$XE-F%"
        "N4H9i)3,F%GR.K:Iir2MtYQ3'Gq&6&<ot$/FnaT%[Sp9;8MWt(`wAQ&Dm46'N-0q%b+)R<V?,F.,Hc'&Jkg*%>f,Q8,RMs%E=Fk4bCIW$9u_;$m'cgL/V+9%o'l,MXTx8%L-Ft$4?:A4"
        "<Cn8%Mm_K(4r_;$9:*T%J0BQ&pwOgL#8_Q&m_FgL+i'6&?2X-?+;5b[+6f$nk#g8%Ke*X$5(.<$'&###F+Dv#J?#n&*:8J.HNP3'Xv$3Ml8YT%L3'q%K-tp%I*tp%s*cgLdYPgLZeXp%"
        "Uj,R&Edf5'a@)'6ePZ3'?;^W%@[NT%&iJU)('Te$2s$@'8:(-Mc6AW$0Mj+MO1'C-^?nR8+LDs%flw^$g0^m&BR39%=IVp&ltt,MVZOT%x$dGM1UY,Mq8Uh$anAm&jUXGMwP_m&N'bT%"
        "QgcN'Kw&6&']Kp7d/]jifQ49%TdwGM:[=T%5rh;$Cu+g$ofxV%g;A,MlU9`$&UYH2M:v##:1[8%L<gQ&9C*T%@ELU9?@vV%veg>$2nJ,2J696&FeET%B7r;$K?pQ&vt>h>kwNT%;.7W$"
        "b]W-ZMs9[0s%p?$Jnjp%>7@W$+.QQ83b;s%rf&gLhP/W$BF@<$7:e8%Jh*t$')9;-5.Kk$X:Rs$FhYhL'_uGMoHKC-%)9;-TLRb%=.?s%#.vV%Uge;%7Hl0.4k[g:JN*hY#a]n3ql^d4"
        "ambT%=4B:^YE=p%mhCs%`4x2MUm_]$+3g*%X$J3kUEF-;qo7N2WEx8%>D>p7g5xP'dMq.8xn^>$=X76'Ke39%Ob[?TU+rv#uGFt%F'gm&LEpm&6<-XLD;bv$q'lgLhhu,MxpT<0Ebap%"
        ">FwS%#n+nadeap%=U<T%CL'L53e2W%FFb05[F?p&79At%:(r;$T9'q%>U)qiakWT%xgY[.OKpQ&:Lit-mnXgL&jvN'M-tp%V3ea.AXET%(XN3M`pia-mo-@'rHT6&I'Tm&ALes$eDg;-"
        "Q1Ve$61u,M7vLl-^0s2M*L>gL-q4=$NLRb%jTYj'P1Ve$e3TQ&+N)E-t9nmLbikp%J::Q8=^xX%Nn3X$6+.<$()###H.Dv#nqbgL%1_m&J^uN'Tht2M^sBQ&(mjn1NEpQ&L9T6&PT>n&"
        "khXgL^q49%U5%1(PN5n&wfC_&4:Im'$6[wB`ej5&u?3t_Ds;*efns5&K6^m&THvE7@`Kp7wKDs%a^tg$AKss%J$Km&Avm)G(L>gL2`YgL^P5W-$Pjt(2>wNF0X;s%(-u&?bmXT%I<pQ&"
        "R3Op.VvuN'`p1nWqt_8&G5FQhg?3O9gVlb%V.@s$0Wo$&8IvV%FHR,ME6#O-&)v5&t&?7&oIwM0H[I<$?ejp%wncM9@I;s%veg>$(i:p.NH^Q&25sae_7r;$NQ>n&puX8&9<A9&Kkap%"
        "a@xfLITGgLbK8<$<%`;$kqtgL5[os$@[NT%F?XKl+P+gLP_GhL$b=t$kXM7'AOns$E$06&61Rs$OQ,n&ea*1#f'XT%wq>v$'O%T&94v<--YGs-n$1HMD?HN'cDtf-k.ww'-C)W%0-W'%"
        "RsFW-u?D3VJ.uP8O`Sq)jfPW-Rgn?9DOE*[_FwS%Ak8m&'5^V-7Q]'/[@hs%Pnws$m;6tJYB+T%B3Y#@jG?n&uj&@B)GxfL?ZvN'A._;%e=x2Ml&K<$M<p2'Kw&Q&HkWT%Hh*t$r-ugL"
        "qH(q%I6#3'Jn*t$DeWp%hbFgLtTx8%BO*T%;_i=-dX_p$l6^m&Fejp%onX,M>LSs$Pd,R&oqXgL=Pp$.ttbgLO?$R&Aj+:&K69q%X2i0(O696&FcMq2ahjp%l[FgLEr'q%<AJ6aeqap%"
        "v9ugL2/$3'8w)w$:u_;$rB:-MjIfs$mg<L#a_ET%<UDj9c%&X%R5[H)8..W$O,Kt:oK,3'SZpQ&gWn0#e3TQ&9p>pL%@g?.CI@<$Xm@v$4-JX%Pt<X$717<$()###$ac'&1ubgL#4-3'"
        "gS5W-[9w0#h*0q%TZpQ&<&nE0QNgQ&RKBq%lD,F%?59W%675R*pvl3'<PY'A;F(hLU=@Q/DkaT%Le39%TBNX%I-B6&N?gQ&XHT6&exEsSh,PT%P6B6&J''q%Q?T6&Gj:#8KOV8&1MBv$"
        "69+Q'.R-L,ufq?9P3$XL6/7@'F.?Q8b#&t%JE5n&i_OgLdnkp%&@(hL<n2O'Tt^-6>'x0WtHrU'ED]t1#dr2MlY_@-wa*r$6=3t_#Ln0##)h;-)Laf1=-cJ(J=no%=h8Q&4IZQ8Ca4m'"
        "sR?q&MH,n&@bWT%^+F<q3v$@'qBKq%A@Is$Bns5&N%+n-Nkr2MkMLq%G_ws$B_NT%Z-K['#xEt$G0#N'BOns$G0KQ&61Rs$RdP3'_`OkL`8_6&$#9;-wx8;-HhYhL]2-3'SA#Q8RHuNF"
        "%Rn0#(;h;-V'PsL)7d]%hEpm&V5e6<c$kT%+FMs%Ttr2MdpbT%$uGQ85-]5'&,B;-*;^;-]'-l.Vd,R&FiOP8u:Hv$HfOP8[.ge$87lgLN')m$Sg7nNsmY3'Ht&6&SXu>$/D+_fh653'"
        "PW>3'kqXgL'<qm&Jw86&[JT5'$jgQ'M'=t$Gnap%QHB_8jw/6&Bnjp%H0TQ&W%V&M^FH3'?6Es%9Lem/BCRs$SsPn&_O@'MV3(q%UZP3'SaGn&'o$:&M<Bq%]D7L(Q?B6&BM]b-Q(bWq"
        "Vnv05`eET%03ot%Ud,R&NB>N'DUns$Oqr2Me;Qj'7MWfL.48L(jfPW-T0s2Mi2Cq%6T_KcEc6hGgBgQ&);^;%ePGs-'P0p7S3(<->UNp%T0bt$8:RW$()###L4Dv#Sm1k'RB,3'P)i0("
        "5tfN-)GNfL5uDK(U^lj'QTcj'2hSN-jVT,.u'l,Mcboh(Vgc3'%mC_&g*96&]52O'*sUNtn-Rp&VGFK3e5C6&PEpQ&`j7_8VuQ-+i/PT%R?K6&L-0q%SE^6&[d5qV6-Jp&qmh?Bdnsp%"
        "RguN'HXU).u*ugLGeXT%5rcW-id3L#B:7<-Zi$A-[=]31H'96&_&m3'Us1O'[AL[-7@&FI8gLd;5Sl'&SlbgL]*s;$SRO0j4og;-s@Qh-m#bL#VK8m80jr2M,=u**<1RW$D0K6&H:%W$"
        "jkOgLxG)u$%Rn0#$5ofLs(w/(PH#n&OQG3'Aksp%XQpQ&C_D6j6:1hL5Af8%9Pm92d-BQ&C;CV&;@RW$S9TQ&DOns$N69q%Eej5&gU=,MjXPgL4$Pt$LBGj'kb(w$JE#n&_vf*%mdP3'"
        "_/c8.RBB6&mYr2M/XPgL&QQ3'(1GR*kZlj'cQn0#vBn0#:YaX(1rXgL%'j^%/Z^s-#R?EN&8-INvaAN-V4d`%eX[W$Dtjp%5_)#MWTOp%LQ`iLw7gV-id3L#rm5R&EVoo7h9Hv$6oo(N"
        "?,p7.#=1hL@H37%M0?tLqvc3'p,/UVm7@m/KEYN'SjcN'6Qlk-0v;*e,`OgLUM#<-R;$t.VaY3'f1#b[b4i0,v$BQ&Ehjp%It&6&L$'6&MBT6&M'0Q&4@Rm/N*'q%D9Y/(4S,<-o#36."
        "u-ugLL9qm&%1lgLhvKQ&;Nxp&c))k'$OLhLIX=9%=7QW%L*06&+&lD5Hq&6&O9^Q&^pcN'NEg6&Gb<T%;(.W$:0*I-ce39%pj<L##xsp%QI`v#3x6s$?XET%RHO=$?UNp%dbOgLv<Cq%"
        "]TpQ&6DNfL.b<<9.WrS&vh#Z$Q4]T%q2`g(;IN5&.;5##LHOX$J^lj'XEB6&ON1K(XQ53'^9w0#=-cNbxp>n&VZpQ&rM,F%kCdERnaG3'.ohl/dZT6&c8DO'1TtV$?X:hLI_YgLwx9Q&"
        "QKT6&=6as%M?gQ&dv/q&_a5n&GeNT%e^w9)pjP3'Ij5l0YdGn&P^5n&PN53'_9+XCf*0Q&XZwe-61jEIpZPN'Lw&6&bT3S87K/UDEnd--k9B6&PKBq%Dq%:&Rm:0(_g,R&a`[-)Ss0<."
        "M3^m&Yu:ZPXjC]$0=Zv$w[pP8IpaX(eja5'PuOP8)`%T&)..20BHL,)OI35&sxtNFiq[<$Kkr2M2&.n&O-tp%II7W$W&;O'VZ>n&PN#R&_^Nb.@bj5&FM.Q-PLRb%92%@'lqap%LTPN'"
        "F[*9%Pnr2MxlUQ&N$96&K-XT%Dh/Q&B.`;$HHPN'uk=gL&9gV-['s2M/klgL?Y&,M'^vN'PC7F%K=?Q8iL#3D`)hm&+_R5'K0v),$BTs$.Pu^]k$s2Mi#(q%k_XGMV^k5&AB+v-w*lgL"
        "1?1-/^A7L(gZ<L#8#LW-bZ3L#W6he$<L:-M1M>gLxJJW$Sj10(;12X-eTn0#YCI<$ai@3kq?Bq%T0Ot$P<TQ&GMI2.n$cgL>ZvN'B+7[Bdwj5&m+MT.Gbws$RPmp/MBgQ&J0KQ&^iQX."
        "PH^6&8M0W0EBuJ(XE^Q&w*u,Mk)2hL%E?3'ePKt:@6kR'Y8%h(^jPn&YWK6&I$9Q&W-s2M`dkp%tX?U&^2v3'TTcj'InE9%QUn'&ja:K(8MWfLY(,.)5cU;$;Lw8%IBPj'4i5Q89Tkm'"
        "]/vN'k^n0#A/w2MhA-n&S/]N2[ESs$Pd57&ZLRb%(RXX$Kg10([NK6&R^Ug($A?L-)GNfL^18-)Z#VK(2lL5/aGI-)t_=gL['UQ&7#v,*c2rg(I9xu-qwXgLkIfh(7=[20Ie39%Wv1O'"
        "JvPEno''q%TQ#R&cP71(R60q%(cMp.WmP3'+4GR*j$4t$&4lgLosugL82SB.M-K6&j4Z2(GFuG-r't^-)@:O+t)AX(NqRg:x:+q&OQl/(Mw8Q&[wrU&bV7L(Y&%-)gD;O'PBK6&<Icj`"
        "+qD'%,2s2MRL,&R+_YgLht4-;P%xP'1lMV8Q=&M)QL35&B-#3'R-+X$E0^Q&:=e8%->TV-eG(C&-qu3'TT#R&O0X9%OpLK(^,D0(RQ>3'4_Lk+3>Wt(q<gQ&N6K6&FenW$JaU,)<T+q&"
        "G*Tm&P<96&Vrk;%2c-'6N<gm&P-TQ&M9tp%H$Tm&E7i;$JQlj'WQ,3'.*kM($DuS&?3*<%V2V0((O%T&De%Y-+4GR*mvUK(hV,<-Hb>hLX3wm8Rx[5'M%De$lQGN'57$q7gwSe$%I%T&"
        "1-+6'O'O9%,o:5/E=7W$01HQ8k]+?.fM?n&nd<L#w2dn&QeDG;`Ad;%Kokl8x/rjt2UP'AS20x$W&VK(ZN^Q&OHgQ&DIR<$0u-20Q^1k'[>iK(w[s9)qQ>3'Q<#n&^v:0(86AT&Wpuj'"
        "5h2W%6dt?0q$bT%Ik*t$M9K6&PT5n&oB#[.X#Dk'Xo-A-E>W'1HXw8%^MMO'TaP3'Gi`6ErmuN'Y&v3'GEpK1V^>3'dD24'bS*e)]GKt:nGC6&RE,n&RZ>n&O60q%OKg6&^G7L(U<'q%"
        "UBpm&BLRW$O27L(O?pm&U/`k'.qRT&`oNI)]?n0#u_ET%NNYj'MCes$==R8%SW,R&aArk'&:(hL^>-n&5pia-v)e<C2uXgLb]b5&<%O=]]]`8&]NB:%;FeW$))###TLi;$]D7L(_s:0("
        "VJ[h(Zjc3'vIPS.RKpm&Uh`j6[TpQ&avc3'Z6bT%]j5R&O9K6&m/Qn&iV.1(sn$[^%m3.)l^w0#i3^m&Xj,R&TvOs1OK53'Wp(k'gPVk'Ad,B=%WrS&Fd]5'GEtR/^vl3'W&d3'[5cs-"
        "xnO,Mv:8L(u3(hLe-9$.s*ugLd_Oo$H4?Q8Z2+6'RZu/(Clk$/PK,n&Fv=_/#KIL(cm,R&?P;cY#QGl&Z8[sA3(29.;4@W$Cu+g$85c5'?rScMerDY$q3n0#hl_;$`kD$#AI3T%^5Vk'"
        "?qJm&B@[s$k&O1#;<Jp&TGmans?B6&Nbes$Z5`k'XY.$pv^Gn&XEgQ&Bn/Q&qwXgLd&RQ8ou>e6c[39%U^5R&.YPs-iRxfL6+OR/QB':%Qa1K(V&4<%PdcN'R$0@'papQ&#.vV%*kwP'"
        ",)9;-/Jg;-?##)%8.PgLsKfW$t0g9;1[.T&@c6H+D9EN9L++Q'J1sq2G=t2M+[AN-g.5o$2-x5'EkAm&#>#-M)QHn&?r/gL@%<4'HiOP8fj7T&lbO,MLar.Mp7/1(KR1H-L&qS/MNpQ&"
        "cvGn&U7Hp&SLRb%pjL0(]ArK(q0lgL6dvj'SNG3'#K,F%k9B6&Z&;0(;Hfp&J-Km&JZ's.MH,n&1TxB/kTG3''oo6*I$e--AK8p&44'njcLes$f&Hn&#7lgL5B$R&$UcW/b;Dk'UET6&"
        "'(*n'hVVO'dV*e)t=CT&Qguj't.c8&]u2:/N*tp%6FQQ89Kx9&WH#n&CORW$P;Rh(B%lC.X>.1(.tep&d+te)PLRb%%8p'+H1Ve$vvC0(aVnd)a/M0('R7p&4L'[^4(cgLTUoW$SBc[$"
        "[Hx8%U&dR&H%i;$;`(Z#M`se)E?5n&gA24'Mq<T%e#Hn&U:r*%vL[L)a>@-)]/rg(bPeH)Xa(0(0[X(0`uJB+jMI-)K9xu-t*cgLohXI)=c&gL9WY,MAW(q%Q<K6&TaY3'^,;O'i/m3'"
        "M$tp%X?<T&xB:hLrqugLUw;O'xk=gL'iu,MpYHn&_;`k'Ke@e6oT^Q&0Vuo.X9'q%:=1gag5U6&jd<L#oNgQ&Vgp6&QvsS&a?4k(d&Qn&i4Xe)t*TZ.SNGN'_aLq):_IKW3()9.=:IW$"
        "OOC)&CvE,Mi6-^8EP2-*UXNP&f[X,M[8]8%[m_q)=L1-Mfivj'M*g2'*_L-MXv1:5[g,n&ER*9%QN5n&SQ5n&Ht<t$Ns$H)6Laf1J6p2'TN^Q&IwjT%iF+gLH2.n&TK0:%SjLg(JqN9%"
        "RmlN'=O<T%_Di0(a`OkLgo24'IHwV%*n3m'>.Oh#q/Mk'(X@p&D$]:.Wd,7&A$@v$ZE2:2P7Tb%WM>Q80-x5'X&2k'8#x92:Oq)NuTmkB09Hv$iVT,.Bd#'=$k7T&'`Bp7%&4m'9Q'3("
        "gSD4'Jrkl8A>Z;%F<[;%hQV,Mr=/1(6R;39NgDw$[>@-)^ZpQ&P7dER]R[<$x8Jw$],MK(WKp2'N]0hPoN9U%H(/R3r#d3'h:)3(WQ9U%-I&N-w4lk/O696&O-9Q&ESp8&3aqKcd.;O+"
        "%$)k'O*bT%Zlje)-$,nawR-Q'U3$I6vP[-)wHg$.v*lgLK=Y**i;DO'a#6n&Q?gm&VZ53'WsC0(@Qqx4Rj:0(]MeH)cvYn&Xg,n&M*96&GQCg(,v]7.1+?Q8,9(<-]k7<$8=wS%I6^Q&"
        "[p'>$Dq8Q&FE,n&^mGR&i52O',#9;-S*.e-W's2M&K2a-Qkr2MsxmR&4f#Q8L7S.*PnS2'i+FI)TB':%ei71(NRM$#/'xp&c]E**'4lgL)DA-)eVnH)[j10(sxaHZ)#KB+oi3e)4kChL"
        "%hcgLwu'f)@Pl5'SceZplKK6&A/c;-^vS$0n1O.)XKT6&=YmX$r8`k'&l@m/d8DO']AMk'()B;-l)Uh$kK5n&b]eh(E]>U)s/o?9ARV5'$6<gLp?\?3'I%Ep'Wv_g(UE,3'ST>3'l1FI)"
        "cS3**oi.1(ZmGn&Om&Fc,pcP&3t7p&1:2W%b'FW88&P%&qZ9hP?nQENE1Ve$#)h;-1?:A4IvmD*Yejl&FBYj'W6x<$KQP3'$f#Q8klR5'xwGv$=j?#5^2vN'XTgQ&Qees$aVRL(a/;O'"
        "Xsl3'?3>k-Otr2Ml]?3'QWcN'@X*t$Oxi63dnWT%;_2R8.kR5'o(U5'(mh1)J'X9%YN53'N^lN'EF*T%XD7L()bwq$>_UhL[?-n&##9;-_kSi$tAr0((X@p&D$]:.YsPR&hV,<-CA26M"
        "+qugL_D?3'$Bm6:D?NN9Ew_NtZ=MW-r#XL#r,2O'i]5<-Yqj$'a_*t$rm<L#/30I$[*A3k(,ke)j^<L#I;CW-fa3L#.q/9.g`RL(hV,<-r_DI.i8v3'WFm5'Y*s2MpDGt$a>rg(Z^>N'"
        "VW5n&XWBU%^c$]-(&`$'5*)4'ZH^m&P3'U%WW#R&RQPN'O?#n&TQ>3'%/@['spPn&$r=gLQAW'1MkET%exI1([)2O'=R7m'p_VN(eYwH),O1hL&A/h(fuW**jAMO'4)'UDkK,3'5sG'Z"
        "dQ;b4P9TQ&YdY3'ncIh(Z#mn&Q<KQ&?:Rs$`_ZT&P6'q%xv<L#PksG*^C7F%x-96&]*xS%AC.w#V2iK(frWE*fDrK(TiTwKNUd68v4=Q'P0Km&C7(gLQ1YE*T$fs$?F7W$<H?D*RwoM'"
        "j1OI)/kiW%h%]L(P[V$#<vBr&ecN**fY*e)bJ[H)ioN**a/rg(Wn77'c4,$,rr<e)mVMO'w3lgLs0L+*<=7Q/R^Y3'^ZpQ&&aHb.*0M0(l=Bb*e)vN'PEpm&%Ys9)#EMk'G<X7/hJik'"
        ":ed68NRkm'^dBU%(Z&UD$ns2($=(-ML9qQ&HouP'fss5''7ugL,D7F%ZB8O9Tbk6't4=.)h+bI)n>v3't71T&[j@_8*ZVmLjvkT%hh'dM8GDp.`,2O'w2bL#r=2=-]1Ve$6:1hL_Df8%"
        "r0lgLF@,=(p4&l'^#m3'Zd^q%YSwd)hP7h(]2iK(a&Q3'GX39%v-ugL<oZ3'L*Ft$Q2[)*41ei0NH>N'Yd,n&M39q%*63Z5_PnH)L3tT%ZN53'OduN'FF*T%[V[h((F_HMg(WO'xL_5'"
        "%,TV-uv*1#vP@L()b[5'E$]:.]&ZR&LS(W-gK1_A`Pun_ePdN'.^^W$dG,W-^=0tq=x0gL/GMU%s2;O'cQGQ/KR[s$UQ,n&/_p-$`e3t$XeIj(xV,+%&m3e)=_6a$uuGW-gd3L#pp7_8"
        "<pPg)[^G3'LaW<%aSeH)e#Z3'*I]f.IXe<$<$>D3X,7h(eie-)Vvc3']2iK([p:k'b(;j(>LI6/dVI-)#%lgL)N>gL>sdN'UmuN'QdcN'Wv:k'K1r2(-h_-M<P*4']jY3'R/e)*h)vN'"
        "a,;k'bVeH)Y1%S'llRh(^mY3'cDiK(r.S1(n1K'+>dO6'W)M0(Vd>n&Ljgo/T?B6&YpcN'R1Ve$x#Q3'Q<^Q&soA,M'0fd)mR`S%lT,n&bw@<$9F<p%K9^Q&a/UY$I-^m&HTP3'b)mn&"
        "pP`k':DNfLWCc0.x$lgL@<wQ8DJ;d*dKs2M7w6c*HNY3'ro.l'U<'6&pPDO'sl/gLt7lE*ic3e)f]*e)l%kE*c87-)w(bHZ6>5$,u(X**oYMO'XslN'oXpF*Eb,F%oalj'b5m3'MCAj0"
        "a2;O'tRBf)^^#R&gkHp&,D>8.g52O'OcH0)^^Kq%e)m3''jJ7.*F1hLnHdj's_$mLYiv3'u't^-[ObGkOMw;9+v25(];IH)YWPN'WgcN'qIte)gcWE*v.]L(%f2i-Qp&Fc,pcP&T8[sA"
        "2uls-(l#68YE3xB,Lg9;;odj91&wH+vqJfLO*/W$%Ag;-'S5s-nlRfLRLZY#-.+,M8RNYl;51G`[aV/U4)[0#9.`$##/nK#YQMO#r.sW#@*=]#VCno#$t5r#tO(v#5(rv#[p^#$@@p.$"
        "D$t5$)_,R$20`C$g'TI$@aU[,J_<a,,=<t,TvGC-.$J=-Nsk>-er?@-&YEA-9@KB-2/aL-dioi-($SX-H)1Z-erH[-su(s-'nIt->ZXu-[lHw-fV+'.u5Wrt&)Y:vw+>>#C9F&#OLB,."
        "L5.#M)-?u#p8bJ-t:bJ-0;bJ-^JbJ-5_#d.[v@1$=E,p39Re<$1BOE$a[/j,HX3a,+TZk,PrYD/wm7=-TDOJM)0R@-?EbJ-P:bJ-cXbJ-bEbJ-qPbJ-g9bJ-@`)d%Z#Y:vu]1]b>OwCa"
        "7U#2BHlm(3>FT(Nr2g7ID8j.C8Eq4A$ps19k7+lfguI4f#)[.Lg5,iT.7s1'[_u>>NK3Dm)liGm/*Dul7PoDlq^7WlkEiVlT_cUlH:,UlT]1NlE3K/#TOE@lOAL#l5K==lMmm7l,+0&l"
        "nI3%l[c-$l#oluk)d-vj:?uu#stl+#)C]5#V^5U2Aen-$^do-$jo02UNN%H%&bQs)SHmcE0r8^F+<p.UbCRfUf[3GVjtj(Wn6K`WrN,AX73ecD&Ye4oSFJloW_+Mp[wb.q`9CfqdQ$Gr"
        "21HDEjvvCsO&KSRS>,5SWVclS[oCMTn8W%t(8P>#_$*#Gc<aYGgTA;He83#,;V.&48E65A5FE5&>VoA#&Mn/(H8YY#F;q0,9a(JUUGR^$@nD##,AP##0Mc##4Yu##8f1$#<rC$#@(V$#"
        "D4i$#H@%%#LL7%#PXI%#Te[%#Xqn%#]'+&#a3=&#e?O&#iKb&#mWt&#qd0'#upB'##'U'#'3h'#+?$(#/K6(#3WH(#7dZ(#;pm(#?&*)#C2<)#G>N)#KJa)#OVs)#Sc/*#WoA*#[%T*#"
        "`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#hZhpLr5G@N4=c%OBNs7RZ.IfUjta(WoE#AXqQ>]X#-VuY%9r:Z+^n7[-j3S[380P]8MKl]=uc._BF%G`"
        "G[@c`F8H2'OYs.CG`OcDW8:SI5TfxF[>J;-YFl(E3-D`NBBXJ1uEoc2j^m+DoL_P/%w<207$tf1bvTlJDwTxX`95JLJ1-AF?X158)'xl/d,q1KBu8MB+`JcM%QQG)=+mY#4=E5&81.s$"
        ";mxI_p9S%bxXSD*W8VG)qDh`3kM_M'Q:iS%DBYJ(4O_xO4U$>P='[uPEK<VQGjSoRtA^Kl;gh.U]xLiTdX-,;=o(>Gn4?D<Des4JuUv%=/8$=$Z7Wh%Ulj89)'p+>k-3i<Z:LO;JGf6:"
        ":T)t8*bBZ7pn[A6`%v(5Lm<i23_:O1p4<9/]AUv-:rtI)b,)O'N'tp%=47W$-AP>#c/Q?.E?TV[_:+N$RvZ6:Y42X-:?$=(?pm=(w7%N-t?cP9P8[V[tr-A-X<K%P/I<mLbE2eP2e8jM"
        "5*5gNhZ6/(gL0;67/H59:>1,fd$,2_(8cC?e&%BPYs`D-Qr^_&v6X_&Hj/KM4klgL=#(Z-W%&9._1dID9_,Q#D^MZ[(20LO:xYQ1caDd&,u-T8A5k31?/ZQ&0hrS&2jDW[;sE%]6qtD#"
        "/@q^#'>[u[:tx2'fRg79A(u59?L?s<V5*t8/tqY?wCsZ[v11X:rAR&#b<;].#Y:Z[#*Z%.5u:#MK`@]O7cxdaX^gB(gixs0qW[2C?0R][Eoa-62GC29q,Ub[9>nD0e4oD0sR#M1U8s^["
        "QB:99-j+L>kn/iN*w_-Oq38S[+0o9.o54=#eGn;.H^G-d@1A5B:hdv$Np/=#DD@T.3C+tALS^?.uFB%BsMx2vIAw>-=e2(]6(I1GD?TV['aQ][?pP115O(V[Ur]][RmII#T>q)2cn0[["
        ";YhD?A.:Q9:?lpLxhC5/4OD&5H0;hL)S&7.j&tmLAvvl0tj2Q8(/[W&jD(Z-O*:aO)^:MB0TmV[(5klM)jG&#SR#6#m/xZ?ee$6MO%RXNs[A@>ab72L5+<+71+GB.>[X0;`Qob[O)_6C"
        "PPA_[VIP^[,?%N-X_;a'iTm2(Ifmw+YqKu(h/*22LQZw0EF;$^cg2nDuWa5/=-o34]feV[Xqs63TN4I$.kamLI1K?.@/qv+Q(,9/JtUA5=I<3MKkP?.Q+wh#<*hk*hhB@-vYdA:s+r?-"
        "n/mx=[wTs8eFsV78Q;u$n'6Z2,,E:/Av.Q#_>dmLRPaC?A&K&GM4g88oqDs-Fx35M6:mjD0e(73*k1691.;Z[nV?3NZYf>2K`/v.FjG59EV]b%h@r*%laU)=iv?D3?#Ss3E%CW3[:EV?"
        "eN_V%YpfR8ULN5/Za?8CeWxoAtC%N-c5O>?PE4g7:V,2B^2WSC$mV<.K&lT[tg&OFT#(Z-BpJ(1kY'D?0C^/;`A;,54JhY?]`Yn*qVs>$cqM]91?$g+%R:X[[V@g%tnK>?GV`V?cK'3("
        "L=b>-lfcs-GBolLXG%K%+hj-$cZ_/%etchL_/+V.c'd]+Zc=kLS-nD02hLw8d8fw'=Jb$']j=T8e'd[.W)d2$6I,[.@vGY-@nx?MK'l31;-J>-K,&b-8>AtLF3&F.YldKN2@.p.[?+v-"
        "a0cfL]hXU%;%a69oR%h*DaT`<$=wMC+[sJD/8Fg<8F4g7^2WSCVpwv8uGfs&ZcG594nIvR?Fh*>Uc1q7U]3N1wbUV-r:;&5Sl<W[kY'D?1H@t7W5nw'tHpw'Mg=W-tgci,vNpw'eNh?-"
        "$bZ/;Hgo-=*iM,NlrJfLP-sZ[QtnoL4uEP(MdAO1e_X?.3=+Q9-5P2(G13WfDE3(]IqJ1GlqXe5'JHDO)<`KO.PRD6F8x/;gjwj+w(1kXhds$#e8g*>qK0t[oGLv[w7@4=IfM`N8&GS_"
        "SD4;-a*3>]l:TP8&Fvx[`Hbr.UjL&#Rjhq)3S]q)aUj*.H]i$'cRUD=RgwV%m0Ta+s;%##s'c?-@hlHMrc89_g@/:)cMt9)VPSiBwN8:)W8l_/0;s9)5S8:)qW1:)u@q0#O%a69h,_/;"
        "fxv>-Hnh^[35vuJ%YC%B_MaKcd5KU[$aOh#cU+Q#5m/S[8D)v[8eJm&gRtMCDbE9.v[`=-s[`=-ub`=-JJ.f2@S2Z[iWJJ1efn=(D-i$+YF`QjK@Wq%>J+;-B4kTsDg<X#01o._x?Yp0"
        "r#&O9jK`U>4Y^U>f?]%kYH6ijq.aJ2m9)la(_ZxtA0XJ2I8Vs^l/v._]<e)5C>AO92[(v,FcfJ7*:7@[QWaC[CG0#M'X&AZv?4#-iX>k#Nq-v#_i'v,?SK]T)ituG3e-k#wAqOovb=*&"
        "gmlcS*k6v#Z]'v,F4hES=XilJ]J?L,8,OvL&SL52,ugS<6.D#R_JJ32<]4hQJh#o*OOem<`xF/(]H_S2gR4;H7ogJ#H&KuuWeGoF,=6J#R-YkF6W3J#G#KuuU[,SF+7-J#9W(s$Vc[@>"
        "ox+;-MJ+;-Z4Do%CX?I#@pp0D1W+GDtuFk<AR)_[2>sPCH6UB[0ERGV$7Ee#AWsl%+OW'%4F]ZA-vQZA9=[j<c5DR@np-H#^5/F@v-(H#`_Q%ttL46@@9vG#ZOuu5BL_,)n2]`$+Q@D$"
        "#Fu(-9FRw#nhX2$Vv1)-Y/9)$.w_T]<RSO9qn*i]diXgLldTe]iU4U%op[,F$GMN9f1N$-Rqvh<&S)AtOTJH:Jw'F#(dRY>Df/5:hwxE#8s2DNCcso9b'V`+6+XF3J>1@#vQ#F359S>#"
        "lVfi9p9Q[t.Z<x+scDk'x+XQfRaCk':@.s$hxQ0#nrM<#8]>>#kD`V$ol+;QRGVr0YFpK%rp0[[0sm][-3Y,/]l.T%1QB%@iNYb[.%[A@/JM>#tO3LPfd;E,OTnb=T,SH,2a_ACd@jV7"
        "O.72L@2q3(^jF%#PLw+H4_bcHLApr-*r]lNeG^uG>Yx6*pM=/#@1Kb%^jew'R.s5MiSu+H'U$@'h,fh,%%q*7lMQq2J#%=(MJ-F%:eVJ#f1P#H4&m)H%PF&#1DOB#B79$H:)'8MKu'kL"
        "?pxu0I8gV-e2@e6EU7e*FY.%#,'r_$FZI%#YU$k$(ss1#ECp*#r,92#>W(EM?iGs-dUoFMvgrV-Z?*1#UD,M^lhi'&I(r-$*5hB>>Z4PS@or1KcRTrZ:H[AG.V&7*V0l6#Gb/,MuNvwL"
        "qra:Mj9KucDS:5K`)s$'-BvV[h2*AOq7(mTj1E-dJaY_&d>88S8+^e$$D-AFi2Jl]m/.DNvY$##Nmk.#2X#3#f3'U#8-LW.#)GY#$S1H-GaYs-cYwBNAop;-rBVt-pFvtLEH%3#ep:$#"
        "(dR%#C0+&#jIV,#Lg;eHrt0GM,bioI]eLl].A###k3i%FCJ)GVT<H&#W$gcN>O,J$wEdk+Q.c%OD0wZ$lFp2#$tV<#/aeBMtqY.#Y;G)N^]DR#T((PMgY)7#_B9U#wLPH2f'###7#Gmu"
        "Mhteun)Lf#2KVX-)ONL#UHr.#E(_B#'F$,$Jn1($RV=2$sZn<$v#xD$wFhj$s0>^$+/kt$cD#N%.DQ_%r>Gb%MWCv%kNj-&E'j[&ewp''rp4#'2O?K'dO9B's.2O'v[T^'$3Cn'?=kE("
        "F+t=(SZ=U()b2))pLJE)5nr<)j;cb)H#^M)kd?S)@RRd)gDU;*t]H4*nbXU*Kkgb*cf?s*WU(,+97gU+Rt:O+]n_c+dT%9,X(oT,=gRp,EO(g,E_&:-Ol[P&/32Jr%4oA#)kmcWge#gV"
        "i21p]SIc)4a'hp&e5/QK3@>9&>TEdW5dvm0-Kb#Z*<dQ0<H<HW4,-HsimjgMrNkp]&:1?mHCk^GE?VNUNwVWe[q8Bud==n0g93'YgV60io)D[$$nrE44Zg',Rwb0;^kCqJDj?*k2]R$Q"
        "6sV0`;B<tn4.Jt[xLsHj&^I=%/?]b47nAOCgFcR':#(%6?BGLDH*dqSiYgkL&O4=[,(5Fk,_r[?//2+O5QQR^:qq$mM-oI*VeoR9`Fp[Hk46+XuuQOhf@24qx&O4L'Fo[Z,f8.j0pI]$"
        "HY6]6M#V.ERBvUSYnZCc_+D4qa;H`,YjP(P#$v_u*C$511o_x?4v,MM1qk=[NVqOq,I-51/PP`>]Xo:]H^j=nJIdS'Q%wx6^:5AHiMkS'Oc-&6KBTSBNb9AQaSP2)6v52M;?UY[D'r(l"
        "T7+jDPbMp'RYL)5P^P5CZBQ>R`bqfabf>;o^Q4N)_F3^6[ArMDCtF8g2G>d=9s#QL?\?C#[FqC,kSeMT'Zx-E5UgPpB]<6^Qdn6gal@V8pr]Zd+x)%6:X]4dkXR1<Svm/Ec&:OmqvV$h*"
        "+ZLNMx4x2iQ6I0<CoCHb>NO03hbUN`p]De+x21nLOsF?nBtQt/th[*5,6nT^;p#bcn#H_6JlKk;Hb<kVX=F0s6[2:0enVwIpEc-OIX>C-[2C7()q:=8/7?IF4]$7U>NR@.kmeC$WK/]%"
        "6$)%[DkX=Ae=dblkU-cGK:0J+Vw]Cd>1qCQo`*,,BGqo1$o=/+Oo55;U5:AIloj%[;?MA71.6>]c3drg#0tcG-e]52&a;#A-`0&[9HT#&p&iPM+.l/+uZrftLqtA@fIJ)d0aS5iPw]An"
        "q7gMsb)KgFY,sDZfU'*6[AmAnW_UgO9$[TC_^(?o%>(6iNdnTC:k]dlFn#kWH^nmr)rF0X#8B6r0&C_.2.%OETcJ9qrC&43xB=n`(@f*H?:2+-&wW-ubgJOEo]s9qE27.>w7ebHnwUnr"
        "T)'([F`-4jraG.>k6k[SseOIc'V14s8VMx/F`fC@^@%cQK[kbmW]Px8CcT@fn,1`RknJcHsIp%Ags653]d'2Fbvbx]Ho0p)uB3PsP0@p)l6N&84W*ZK/u0&oj+Z&8xvCvT:f:ZB0#rPj"
        "HOKW1T[VN+1M$k+#Z]mWu$?w'iD20G8$Wdd#I`9)np;9V&ZfE@,19_/&*`N#d$eQj(U`*@CxX_&/(bb.g4BO=ev.CJv4)UrA=>IH<J1r)52<Rj+?,xB7ET_fs)<u100XCA8_=1PFEIIm"
        "%QD7jDNrU`RXAxgoAYS*XKfc@a@gu_,iXGR43bSWxrN2GVmk25cqBpEj2)E8a4O^9(nWT38-o,n:QaaSNP,$UIBxB'fJ.O,)2wBB<hl'922be.+Uf[(Dm?bfpN(Cg:q<@UF%hF/Umw0v"
        "g_YXiE<.lP-a^x1xb=(gtRb7Ox=FiZ=QEVEtIxFfj.1,/TEi7tLVD&(2JwS4aE6>r5Sl`^]@c>Dea'?2@NFZ;a(+apXp.6,g&]8tkGm5ct;CKIh.tW*m#lv`Bh49b?q/6lFFX'(BKc?2"
        "r>Y<<q(40eXUqH8i_nNH%%L6Y3g(C11bmNZ<UN9kLXt'(JjC[2&pq9=4sn?ME)lE^Ng1km]^;=*jQj':OA/UX&nwTt.(*./(pht<DSwKnx00OmLf,%D$x$rOitB=W9(J@`Y<%]M>[=Y<"
        "L8?J/>Fol$/VA`U-T*Pd4*f=s>eff/Ow(2@a3ASPrEYua7^,Sub.#s4q4vxD*;s(U9Ap.fQ=4j%xQwi7)(]VF1]]`U:D#/fKPd8#]]sY3no5&D31_YW[QTYjd*(K&n_uS5w@v]D*)<,T"
        ";;TMeLMmou^SN#3xb[;FNKN8YV*OAi`[x2%kC5W4t%6aC(a6jR1B7sb9w7&r>4w5-AA`&;FgDjIK0e;XVt*ah]:sp#d`N^2fdr2@YC`>sx2Z&`sK?KJgC]H0sBLZangM*(%5YY#:k)##"
        "B#TM^-l68%D5AW&51*p%qc2/L[@(p$c_Np%v`Cv#S.OG3LN<I31S9t@u3H12v/41)Xe(a4Ew[*b$i:._ZJUJubS/TtlAirm[*d;RDOHVQ3W3H2.a2P_kNcIq?NIH)kqL1L1kcIVQkfM'"
        "qNlY#KqN%b)rs5&$2wo%,02v#2P1hr3@u^%2k.4(qSSm2M=;8.;Pr`3twC.3m]WI)#0Tv-OXW`<K&AFqM>Pb+M3%K)M;>F+M0%K)4G/a*3p%*4.+.X-.Um)42=Rt-3+-l.T)Al#xuH+7"
        "L]:Y7?YIH%cw(0&>K4,2$&NjKs5KlS9R[w#%p(*M4t;Z#]7ffL)QqkLdf,,2sF]#A*4gBl3+*v#B6wiLKTaf:@3(_#km8>0#gVIqg/t-$bK/H3vSN`s,vV#G^3=&#XjB)$s<Hc*&m?;%"
        "[:gF*Cx>>#79wo[4F6g)kw.<$g%oh(O_I<$BJZa<Tmh0(?QKa<T`x#,%Tb?#HmpT%RW=&#b$m'+Dm/Q(u26c(OpYn&diN.)8`P##M]#K3,&+22Iq,F3%gu@3W`Ps-h+ikL4+N^1OVI<m"
        "bAfO(AZNP(ix2C&^Ij_&*%i0M@d6g)I:R/)X5Y)4PgwiL*1F%$23k-$2VSfLn(3%$Snf`*+^`@RQ?kX=U].uu,Rt`u;hbE=/rso.^0@G#iZMW#d[V@k[pjI_V(DG#($iP/*]B-O$^^%="
        "29]/Lf.(R#o5>##+(^.$kNr$#1jd(#WOWg78O,70H:R[-gJB#GwR9;6:>^Y,_b)?#m&(h1p@W$#D2Puu$&###m.bo78$uFF7Y*&+2G#i7Y_.0:B@PI)-DL2)#x>W6Z?Bs.],=T'f%6#-"
        "_:nA#>mkY--.,d3k-BN'<17W$k^mI36s@0:0oan0usIO(%gu@3leW#AQ6.m0Mo)l(6@-@5V5oO(]ran0pSO12UuOh,#E.Clu-MkLS<4N3GC*u@,mVm/DpMG)OgHd)w0>j1J=7`,CoMh1"
        "#.@x6p+tI3&f^F*H1NT/]$S8%TN_V%$2xF4X&l]#W[Rs$:**u@e'`73_c4D-bTLTC0QHh>[lmSLaoWHYn/Z(PS>JF@A5rk0U&vA@n,Tn/Ckwf=FepaGL3Zsm6C5C+ha<X-WQn4]G$2JF"
        "t%@i=r`?g[X'SYQkhZYu2TOd=/b*a42RsZuwZ+e#S=*22xZ/>G70&##984*vNC&]$M[u##hd0'#HJa)#*9T5(a#jIqgX3#7/0]K)3,.7^e+)t7$qaV$B2>6#rD@=-)H,x#f6Hs6N$wp4"
        "5kf^1ZS4'(]gD9+K=KDl,[..$a;-,20#)t-G(0(M+xSh(I?%12JQRL2>]WF3DIn9.e^qG)x(h;-HdZ.%LN<6+0X'B#Rc=7UXwrE#xaT'H#T5S;'DkEG&jYo;)F*;NGo<M.+07j:)h<4+"
        "qoRX/KHiehJ[v?$rWxO#ZF[KWXO%/IpiJGZr9MiHkMafYa`sqW`Jen*5n_n*90.o*mbqrQ$=VM^&5>##-:op$t^n8#U?O&#7l(b7rOqF*<wi61[gbj9$?Z<8h.AA4P[=E,<m(p9b)]i9"
        "6Ad010(b31Y?$F#&`*2:B):v-u^]v7]ckL<+0</3F$8&6Q@NO:tC1l2t8-P7^XQlLrjLDlHIAC#Lq/G((Kf'&9e`GlNg5l0I?%12-3-G(Z&nO(3IWi21Y_[,(OT&$h=S_#,Z`,)x1f]4"
        "Js$d)V$?s.'D:68tZGmqFu^B%G7o]4oIR8%'LYQ'DO=j12Ki@(-Z]#6<Xw(+kGs^>V;MSB4dH]u]@+f4Z*]sAkIng;5*K>6RM>:A2p[4M50/(>.DclAQg8a+trc30m1NB#I'Zu2fn/6<"
        ":Pgc6g3.qiO:xv.a/2T9/9R51AJXR[cO[%-rA];7/`6G4*=E2qQO/e>ltv'?-uv?IvB)b=>dW,FH;n]uMM+s8R,JdFXlE^uvjNn9bXN1De*,##Sv%P#)(G6#;doW7+5tlfgVTv#wRHc$"
        "78(]&QP1v#CmwJ3Z?ab$JWb8.a`)$H?'o;-gH,[.3Ohn#B*%U0;X2V77m[gq4c:mLgbq&taJ387%DKY%Ni'i2.s>x6@3Cx$m?`[,81Fj1<<Kj:rf%5P6UD0E(PGFGpWlc2N3j+ht,.D3"
        "`p<sl*S?>f,[b'Aa5R'A<$d'AZ#=T'0o1T&O>V'Ad%UasY'(CA=.A1A');B7Hq.[#9tT&$a-]P/$MvY#qR'FGw/70EXF@MP0b#8;^xhqJu,f2@&PJ.p`Sp4fQOTmm2uo$2`WlG)K3KM'"
        "G%Qj)+GSfL=W?0L(FNw#_>6v$0YY##?d[/35A.,2xA.2A;IM^4x2V,)WIdP/BbK]$ACs#1vdj[uXt5g[mTpsuRG1DNhR.[u=SS7,>OYH#.*0'BHrR(bsbW1#>*7H2?pq;$wU;4#B^fW7"
        "YU`?#5[+2$4M=;-$B(GV/(no%/n[v#IowJ3hl,,2G_-ClQFR`%fF3]-DNAX-8*x9.5K(MK?ACN#ed:BSDMCN#eW(BS)7Fe?5M5%t7OtLphAmx4UkRd'#6G3bPvQ_'#B#k0>%V?#+7G/_"
        "1c'9'DnwWABqs9%#5D/ULIU6p]e_m0)h*u@ti__%:GsO'9<Nv6t8s(EA7002_7Ox'$BqU%/V]:J;hK]63-<343-8E=:Q@58%sE*[Q6`v%BL;mL.u#J_52eBl0[&4(g141)6AEC==bl1g"
        "gr6O+;i$/vD>K2#1C%%#4da1L)R>IVbPNe)0>7paKTaY#Fi8*#Q5C/#Ht8tUh#$6&bguiTr(?H.L#Q;p=A$a4EHb<-`oIr'G*vN;[L-##95YY#Imn#Mf8[r[4-u2vdB]+/6.`$#i.Wt("
        "0BJO(m(t.3Dg*i2#j98.b6?Vu0$iV-SAcjMRR6##w0ST$+J?&M9t2'#hsj[,wVK3(&6P_FXL<5&%on9%#*R&+.j-P7Go^AlQ.IUl:^LM)T@h8.>8`^#*l0C4*^B.*e,rG)SwC.3R[FU%"
        "uLD8.Y(M;$-]k@]<eD2H[G=%]>wiMH7M_[,lEik;is5#-pHik;F^m]#Em0onvFgKbJ.@,]e=.%D2F1on8wK1S/uSMlrm0lR-l82l$),##Zs9S-WDWl%$4q%4,WTm(B*TM'`%ZOa'>lY#"
        "N-,##U%>;-jdE;$FQ1k'A_R%#qk5T#leW#ApSO12KTSm2SI1N($2f]4B@w8%+m)`#JZ.v#Rx+C+[7IUREsCBSESpQBhKEj(-]Xi(D#ni0J/Wj01?uu#:S[8#n%iY73T9^F9:(3(05%aF"
        "W&QJ(V=H>#_Cb2(lBX:%hN`J#wb1BlTR<]3I)-,2GfQ]/:fHT(Ex;]3b14Z-FWNH='n3Q/>]I?7@ohk)5hv;u>Zt5;;]Sw-MnwUA;r[w/L#B&8rDE+6-Ca9;i-do[:L13=Mb'U;n&vA-"
        "HB4a.7:-D4vUnG>;Mp5=U/t##wa/A=Wk=MpBA?T7OB?^7K([[-D82`Fn6m.#2G#i7jvF1:hoev5$Q>D<Ntjt$.]ii-kaoA#X*]S%TY7.2TSs=%wjGO'YmwJ378C.8UZdHd7:H6f<SABl"
        "`5*@5XXx3:1sP,*$.7q$ua)*4+a0ppO9Z)4N=Bv-Jp/m%hZ9%29q(w[m)P*e2QcR>UP@^@`W=*4GEvlM#iTK>h_DZ@`1&x:rU$KFLFPOW.J(mAwOWG5Vu(4^*&,LH^F%_YKIoj:#L59B"
        "@xn&0EI.g3f8US8,ATD4Qu_7BU05##>G:;$&X:7#E_R%#<@Ut%%*,##OZ0V(b:u;--S:v#iN+A#Ru(o*L+ca$8Ins$a`cV$B$':%CeR%#<d4l(K.5Cl:c-W-meb$'W^skL[Lv@3eWjkL"
        ".wX?.4t@X-HImO(hoEb3i]%>.<`(?#Bunj[nKEPS0TFlJ0Vl>#bC5(+W(>fC$%iI_e,u0$)JQC#[+JdHFjee2;UE?u-U,eQg_O1p.B]M^tumQWBA_7/3uV/12eRP/Q.IKDo&[A.[0;2q"
        "PV:0_6:Zm(8)Kq'_)'##MEn`.mSC;(c^0-%J-wAl1S9t@f%EpL1w*F3Q.IUlt3^X;Lw1T/f1(p7JRBpRG.,Q'g$60)(EL#$^RdV-O^T<:49pCLhYpJ=wldVB'7xtJVpB5YKbrD5w;Q`#"
        "=iaj/1WMJL&;VtDdKo^8sqTJ)#sMd*p?v$IK;eg$rYV>5&qfku?5M$#A'&>Gtb`5&ucgi0D(S]4#:>FFMM'6(cr9x5M`+/(FIvu#A9dJ,<RB$H<5DwGqr*877wIc(#-hQabNx@3d[;-3"
        "$241)xo`D3FO&H*:u0eZ.'$E3W@IT.C]R_#fRp<&2G]I)u;do)`<?icorp020Fh>?ALQ=CR6,x#tg>M;c/:WT(gXAHB9OB[vrvCj2c0X/,@Y?8O$*r0WTEf#9_/Za3E'b#%&viT`bJ-<"
        "MVAp<#2-F+6uI:EfZ=$9Yaosu.iBrK)Wc4<(0YtA1%%7Eh),##,H:;$:'G6#[h%t[7'u2v=;<hLP-aV$D72?#KW4;H,6Wb$`WcC#9:H6fl<Nl%@h*i2AcYV-:eS@#sGLD34w1;ZbXa/("
        "2(T_j;@h(Ej]O##sDOP/RpWf:7m@d=ZHGFFWi63:.Zpig:Eqj$=->j'x'HF#g7Lu66//B#$(Hf_86p;.0hD)+geYZ,?^+35c5?D#Iol(54/4#5wL85/W#ja#,0CJ1TcLx5Blb/EciU#A"
        "RdW#A^Q#[T)rLkL@i-G(YwpwLG1e()`;Hc(6?wu%n^0#$^<Js-'SsO:4sB#$f&UQ/(ISs$bl@d)C@=T%8..&4l:1A8*5.-5Y`6/4E(WxCEi#d6?4Xe5eGRv.6^E/d8E*::A`4[.9L#C7"
        "AX%L#D=QE.rffL2Z/'QK%`k?\?Z>XgEPapx,9JqV:aT$.5X[uc@<pop:WhM$@XD*5MJrb[Xj>r>.rc_,54m-[7(2koTJ2RUCxk-K3sIV:8(=V3E%/5##)1ST$MP[8#4>N)#EHEA5<W.87"
        "M;-,2#vq<-0]G;-XrP`+BQ*&/bTaD%h't_&qT^1A&*'A3DpMG)%6Tv-eN=ZePe&>.5WlG;O<Kv-[bC3t<3b8.[f_;$3pSs6]Kx9.2Pq*[r9_2'rPctAp`qpEbmq1XtJGAL)X+981a*s8"
        "0cFt.$Q^M9n@w9X=.kV$-UgB'[EJwh+R<m1nC[20BV20<U6li9gY1NMRbK@8Wc9>RDOHVQKX+,)3ufr6dKg301#JA0es#Z,vA01:C<gW72U%0:PlO6+++_`$(,YW#8vU#AwN*u@%.eB?"
        "Vvf0:lruZIcMTVQY_#Q'Y',V/Ytc2(4oR(#dCa`uXs:g%%j4;-Itl22MmN*.C/s-.@f7pafM,[#l'P>'.mb?#[gru5^VI5'owA2'9R1E'PYR<]]ZGlTE%kT^H.KG0<doH3)g4N34Y-1#"
        "3Y,haqDv=(jWS<-,%g`:3v0^#onXi%wlOfCnr0Z#aJ#s$7C1N2tZCZ%gHI>#,]_V$K<&^FO`^Y#T),##JaB#$=r(#G%h_lfu7$W$g2,##0fUv#3+NpL4U?P3T+;-30>O22MGf`*[6h)<"
        "dlWL2*pvC#>PC;$4>b*$6P9^#F@fxXcr%`VuCdCa6oOfC:*/Z#iS'v#1#@S@TnEZ#w#=?#D-E>#4$#>YUN<1#KJB>#$*0585lPlL:[WVZ&&W[$Uup>#=k<X$5+.W$n4#X#NBN]Fr?L&#"
        "q7>Al2*b+.dtro75e`s@w*'9(wp]<]/&%&4n:l*$JUG%0P=I)Sa0oDS1D[+PtNYGMT3w>#3Sc>#AnD]F[.?Z#Z+)X-t(6ntVJ5lt2c4GMWbPe#P/</MUeauP)mS`EH%tiCNutjt,D;#v"
        "4;QZ$lf0'#22%Z79q4'0d?L*#3qMN9>uL[6ZrNE*9`$s$AW6nT$EQ'=(j*875JYYm/uKo-&m#kko-IUlu-MkLqOZ%3i<7f39`/P:OGcD4v`(a%ms$d)c5s3(tLJl'Dv[L2q3j[,=fKc="
        ";QT,=4kK#SkX-D4vUW:8Ua#I8K.w5/w83B5GcYU0$YTK<g?XF5*XoYQrn)rA^v[jD.*EO1jS+61Qb>B;U+p>G=ct*vOnAe$12;,#;Z8`72s+oA,kf@#7qID*SSsW/hitk2D),##9oZI#"
        "Cp[F#Z(>9@^-i0C*+iw,`Auw#F*=A#kObi(_o8c=Jo[Q:Gb#+bp02@5ou#:ALW(9(9sQ4(JH]C#$i;.AE>_1b<[l3+S%EpL.e9'bL8lm8QT;H*Fd$H)mevi&9)r>$X<#N0tB:a#&iRs$"
        "-`e/;`lcW.MDJ)*af/+*9A^>cuc6<.[:.s$l@.B58>TP1siIv$bwEo9wt2@IKK`cFV5Qw?>OiF*]-*u-/M7R9Y'[uSo9*lu&wmM<rr6g6DTGt03k&_]4jG%DX]b]439*s$`jX(*d2f.<"
        ",_pM'VQ,k:;HZ]#,'EKGjYv^,4`Rs.@[m%5(kHSC=#mZ%I+mYupDx1(R(@r8Un]MDk8[4kBe?+dLvPbSew:V'N8g?PJI*hbXK(Z`6_g#O76&fqJV6FI]D=>9qCoZ5O9'##R3n0#62u%k"
        "AE=8%g_+GM=78W$^SGM9N<UG)#94?#wuZS%@KuN'6_+/(^joE%=aGN%;<u2vBXMu$7cn/(VD.l'HY@.2)L,.V5+Au$?p+/_fm@Q(A:VW'NwI0:_`NI)/DG##Qd[/329fk(kQ@V#rRH:A"
        "I4H12U*wGG^r+@5swnU.)g_p(@c%T%RX*i2ColV-(aCD3V-xf1_^Jw#P]=v,DsR@WPon[YPhuFD6@Rh(V^Lg(3EoVQ$4#;6$RH199YaxX<w/v767N^6jp1V?&5>##DI:;$g(G6#]EX&#"
        "Jam^7:(qu?kR.Vm'>CZ#]t$s$(pN<@6K*X($*Xn2?(qr2h.RC#&*uM*;4A9.l+s@#Oe[S%x[LS.$`et-ItdR:5C:;AN96V5ci#XlvS(9(l/g()F,V#A/a'i2'm-j:5=d;%14$&%k&^k9"
        "Xv:9/688L(XGN/<m/1WHQc)>L)KPW/f(F99me=u7i%reuBdp<>@$IPT`'D5&]f&C-i2Va@^Gw#?nM+gEkZ16&$q`[I>5R6FtOJb,l'VYIuKpMu?8qQC:cFZ8I,>>#8LO1p;9?8e=<8G;"
        "5T/a'28Pj)J%xN'muhA#4J1G-:`%W]MYY>-tr?B#Wb-iCs7=PA[-T6&S%&%#OBxkLPh#L/I4H126[FpLQ49t@#F*/:0R(E4+(Jw#$>sT%'An5/&6gFEwB$'6p%'d?2i(-<e#YfWU3r'?"
        ",gjqEp$I@75Qx)=XC]LP]SQ@.X7#F*ln[ec5?wRXnB5VF6rmK-S/5##DI:;$1*G6#FXI%#V.F4'Ttu0+b)MG)Uk>k*K5cv#ZbFg(P[];7IFZ-.9FS(MXLg[#r,9Z-H&B.*1Z?a3D9p;."
        "`loiLWmbqA$=&=YM;>PSo2vcAcf/<_[_^O#:+rZ#Z8L4oN#WM^)9xbrxogWOB_&M#H,>>#H+P#$?>ta%DD'58PW_U%5u1?#F[.='9jrG#Q/9QA3ooi'6k@Z#A_j5&7j4v#iPRcMvY,@5"
        "*pxGGBML-3^LlO(pHSm24lH*R7Q9.$nBuQu$B&XuSQ.@uQt+^$e0=V6G0T[Omrv##h9<hLxGXm'*D1;$8$<;QRpAv#JqN%bWQu5&'MjP&xnOU&#u2d&4]Y##aQw8&t=F-M*uZ#%Ip-8%"
        "vL'T7SdV$%&_41:^+f+MbsJfLTMLDl5<<m0<,_K-BOr2%MPs6*5el;.%Hq9V-o)t_e('[T;2<%XY`*$%L+Hq$p(U'#9usl03n)w?#.@U1+u#v,s=r91XB&##'IST(]bCB%W=KDljo4Al"
        "(%f20,6*@5`7JC#g*PZ.WB-H340_:%eTf88xmu>>Ve;P(V4i?#&FV(I,Huvq6*5W^J8ZoF`'4[.ZTXp1rZ]B8np'=Ndh2d4kCg6$kK;l(qNRdDX.ttZU;P_b0a0-Wx1sO'r?$=Ori0k>"
        "@UeW$nJ^9`v[88#Wb>n$:h1$#x_Xs%9p7JCTjf?#gt=;-=%.<$HPMw&G:3U%<_ET%;uqV$v7)T.s=$:ACK*f/nbW#A>s'A3riAv5Fl*F3Df*F3aOWQuVvRn#%fMYY3bVM^$UHGD3'&E="
        "4c+/CVX`v5A.UT&kZ&[#Ti^B':WsL'4#c?#N$I8'Hv4v,;%eS%B84a$HHa>#-lB%t;3M0(p>Y>#917<$9CN5&gbQ)b33S=3T8#%0J8T=3iK`UA#$9;-Ongb%-d4c4]X3]-ho/F*w[shp"
        ";1%g(b]%=0O+vjCm7vbDM,Guu<iQK#Qngb%4[x+2uxM%*`dx$G)X`h)D1Hv&5Cjl&H=I8%FFR8%]xw+2+%TwBdj.4(_x7B.TO2s-h-MgLMw5K).PAG3Q]N,<S*K_6TsliD>+C?'8KPQ&"
        "Z4+v88+bOAihJn2DEx;-FIZi4)m)_7pbhgC<o:$#S^5R&/&###<oH8%((C'#F?o?#r95/(B,,d$n<>/(9N7[%ci-n7AKl)3<_a5&1YUV$RmwJ36.-@5MoV69Mkis@$)p'5?_skL9WnO("
        "i[nQ/tc_;.`0w9.hv>d4]<E;$+sYr6HUN-JdQ(TuK@4nG@4ZM^FHTl#q?I_81eA]$Dfp2):BqJ%<1)?#:gr.C0S:v#v)U0G>>U2M:a%QMSPqkL)o6g):6cV6B_/2qRJOg$Q>KkLTr^%#"
        "8A:C4w(*)#QZ'U%k[mE5QjqK(Otu2vP(KiLls]%#B9@(#R90q%Xm,G<%1%a+OQ4A#/'3I-UN*I-:7N?#=2Rs?nrJc*>-Tq%7,VM'NTYn&.?dg^4'Ap&ccl@0:-R-3Iw1o0?vfKueqZs0"
        "DcW#Auu*@5we0o0;6L(MwSwh2/CLa-u6_F*v(]L($'lA#P0wiLLvnf3SV>c4nuK8.v]gs$2bC;Z+>m.Uu=2O#N3Jv#1VlYu_%Vf:q,&M5sxXM^$8&#Y8L<8IcU4p9E/bcM?&$N'nP<K1"
        "wN']7.HUB#?(tt?R#kU[;h65+MJe1(r+xp&<U3p%Yw>k*QkWt$;mcP&ud8q`3*]5'<NGO-gLD:NHM/$M'N8j-JC-+[d;DD3Lun$$Lc&]$8``(3]6HOhQ&gv#.S:v#<1if_7A8aPo'5)M"
        "=6BO;jwOW8IPP;-%/5##=:op$*f4,%lXvX7hMa,#HDTF0deeB#BO,,2,L'_#XcXgC2Rss'O.ca$7,>p7+E2s@AD]V%=WrV%3eo1=F[WTWI%FO;GLr$UR&det:s'1Xo(fUEA[XkWq74rE"
        "6XKUY@f[uEoPOD*9@nc)/:;IcCw%8O;eP.=W+aOO8_P.=veN']4C98#`:&]$=$M$#nbnZ7=GKv/NWw.:8%l+D*A;A#9Xpi'T'Gj'FAf/C2FRQ2rp<Z#OAa&+A;R8%_.i>7)Z)225/c58"
        "gMot@h9JC#k@C8.6o0N(A,m]#e:tZ%s9bD3A6W,T5rw(E'1[H6NfI'A2o7eW+&^5&K8Dq;ObZM^j,-kud/XEG7b]1#Le?guV`Zc$d)+&#xcCY7tla,#]heX3nKkC#Ln,,2@3-`#mI_hC"
        "Pv-0+YLca$N=v##Q_5Clr)Fp7i@OkOZ^T#A;ICp7PRHh2D2_s0=^g8D>Cq30MOB*-mioI^g1Z)QS3xac=-to7B*Md2*)G6#Nww%#E>)#&]08[#<6pi'8*S0:S?gm&84V?#NP-9%K.I8%"
        "BFi?##X&i')k6L,gMZ3'H9<;79'v.VTPD`k/Xw],:2ir?^muo&AA8-#YC=gL*sWoA84i?#RmwJ396*@5Ee`Gls'[m%.S#/3OU&53b_7:.#r,F3j+mh23uv9.6eH,*]q.[#OW'B#:gZi$"
        "wGU_#Ax8JLD7xH#Y'1+=R*FYP0_=hmG`=3=DgBrK$TWS@,MYM^HBjoL&Ik?u<@4pU8'%=9-0-E,7(1GD2rNVd'V@D3xx;W/o%;8.K,o+DMR'%vEL/04D*#D#w^V^+NN6O0(rX^.a=,nA"
        "r<LhLmi,P7_[2>M[T+ClDF.W-[aV2`:;Uv-kOWY$?29f3S`jj1Z%5o$'0_:%:OpD=gIn8@gXFVdkcA3Hc'm#.trm33o6HM:-M=5MV;kAH]$?5aBKT,XExqh,7]%A@9`BC#fN19EROE8]"
        "$DAl'=;>f<CS.<.csq0>Br.33-AqN,YkJH5l'l]-skIWK6.7QA;G:;$F'G6#tGH^7o:;?#Mg<D<]2^Z#fP;s%e*.8[w+CZ#`s$s$7j7d#J:m7$8lY>#3Q(v>[E48I8do._/PFW&)[>b#"
        ";8FE$8:r$#aIw@31S9t@A2`#AOgu@3#_j;-JMpk$kH7g)to^7`-NAYuC[=g(DWOfL;l93_G,#&kMuED*Y0f),Ov@JC],(A#urZ8+OHx@#PS+0C]u''%Ecuu#,eE:8iD.0<dE-RARJ'N("
        "YIX-tm'qN(xUQjMf]50cVx/kZTrkN=`p4/_NS_.c7GPbPm#mrZ.>sra@^C4=2d1_JG####3tVKu%oH`E##vY#9VG##K):J#r.sW#2OiQjnP:u$1>7/(c#L-2)cIEa-opT7*t:_$&#jIq"
        ":th;$=Bu2v(oRK(m`Ys-6gQgLn0WZ#9c>>#[X.I3Iw1o0mtfC#>02o0&/L`$#.-@5K^aI3d2DD3xck-$Gf^F*u-l%$qHO`*h]U@c=oMP0;)h$-UMvP/1jYGYnmxCB.Y-W-P^U3X[..]k"
        "]CRd)M:Ok'5g;L^36=m'unM..FAEhLO7-##7=Y&#5sp*%UgIiL)^j22AVWm/*KC:%tkGm8>3fv#hn>K)+,Zp8r-TZ$5>ds.bYoY,?+i?#0'.['q#4CMlIi:%>7X0(0C+bFRjC_#wW141"
        "bc$C#@#]iL8lp2)OjniLXIJ1(xA:-*QH0#$ai^s7_/,F%o0w#M=U(3(,Y+GM/i*F3V&pG3s'IF><V[#8$[9CGd=Y@5Cw$12XpxGGq8nH3mD<v5d2DD3o`[D*Y$s1)(k^#$h.<9/^@[s$"
        "V1L8.pXFd;P[(lm&ib.)I^C0('Yh4dpD-nucv#n&N6B6&OQfa`F)3rkcD;O'PAwH)X,TM^rid(G7(59/$#[n23S>+;q)ueEF&kI30>3M4teg--gPPnB,los.=oNo2/o3T%.(WM^Z(/JL"
        ">b06&G()$#rXWP&jHn>#->P>#?tixFeo:`a4W]._8L;N)'I^*#@C/2'l%J5'Aj_?'<F[s$+$hj]BJNIFh)#=$v:QP/6%eS%pFWt(f:r$#]Y#K3Zqdu@>=n'5H@LFG%xQ]6lg4N3Mq,F3"
        "O0=r.tc_;.THNh#,xH>#gM&mAmwFwLe#u=cD?LS#(`q0aPHKPJPVXxtZYaLgIW8c;abK&#veg>$-I]k64r]`77;L?#-2im&&.LJ%pw-?#13a]+Akj9%NEpTV,@:=%[ir.Lxj(c#B4Q>#"
        "9rlY#N-RAIL/cPS#Mpe=gnV<%`$@>#:61g(CZrm$hIQlLdk_#AN96V5=K2F%jF+F3jr-F%7;6N3clSL2EqfP(6*CD3@spx=>Of2`2t>p#/nT:uo9wo#(W>:;DoK#$Nnfe%09mx4@bkp%"
        "J_M[&2$7W&(iKb&;:.<$C=)@&=Dba$(hEd;];eB5)-MD#&]F12>_FpL[m?12rP(Z--*Em$&>?Yu>8`ucJm-,M9I)Dj4cLNXarY9MPiNkuHqPlLvUe##O>d=$eTUJq5:%@#AchR##B[cr"
        "p2ncraN=H5m68B3bl8]-8BF:.MxE>5$_[oRIj/c``paPtGeP[.<5YY#Umlw.Tbnl^DZ3[9SMuu#,--0^KMP,c8=AC#$EoO((V+F3C0B@#iJ*p#*uM=l_3D,MdiI_u/:Q]MI9:W7.nL+r"
        "'v8'%L#.s$))CR%poFU$M)bF.I$eGlll6H*k_Y)4Pn&K3%q%5S@X4j$3RQ7%$),##x,6?$XjU7#GSuN^_I=gLdkZ##a>2,2p$i0MrPqkLsbkA#*53&D/*9kuxd.l#:m#3uh]KUuE,Guu"
        "4Z-4$e.g*#c-G80)Jl>#6b;##K`4.6QxF)4=@^*u&mcbuE<Ah>R,j%Olh/g(WQ02'.B%<&]@]/L<a.-#Tww+2iJ+T#,jWL2#in$$&]%`,Rs(g(_5>l;_dKF(m8kh)qJQ`=R$Wf)4[qO;"
        "JMi63NWD)v_UrK$OLb&#?bB]7ow220me[i3`4Il3N7*4:Wvu2v`gB5'Q*q1(E:wH+(rHM3kNgF,kbk0(pUAd<Bh@%#b.i>7[KxD5&vQ?lMR'tm0:>n0&Lf#A>s'A3-Y[[-)RJw#C]R_#"
        "OM>c4j@pE-xg3Q/3B6T%CPp,3#IR+4Ps$d)jY<&+_WS88V2e89Zqi<nsG``46oK(QQhuT^:InABjgS[6=q5$Qbo&C-6KUt7/(#*=H8[e[f6bW8RcoI2/X2'5==$]B,W]B/C7'rB2IUA5"
        "ZH9d>s+5E3n1ST$'Y.%#r>$(#17k3:7coh2*m/h7McLk)]7?n(f''%#p3#e<s<R=-I[,/(wW,Cl1Q_;79#;P-gHDm$@Emg1,P0N(,['u$k,$E3v,ZF<>r*G4`c`^#(h66FN1eXTk'jm;"
        "QU*q.9DcfLwm_Z%x]ltA9<2%8]#DgEP7,wK^T>'Q=Q%YhB#Ol0eVd+%7?fF.8)E9/so^*+Ham9UP&rTE(53,gH+,##/(89$xTLpL4kqW71IrW-D[*e-W#lA#2%xY>]Jk=&HWZ+#)Cn2&"
        "3:H6fg[b[$iE3/:(*Qm8D3Tv-iL0+*KG>c4Fl:Z#di:#MGwwB]JWn83:/-A6P<^KQtnK4MubOT)XjWKSe&L+7XkXE&nK(Wd`5_./pva],Jad1%bW$H)Wj,<-sJuX7,5X]+fC%23?agf1"
        "S]hl2tax%#p*L1(OpmF#]2XJ-=;Kw%HMu;-M>;U%rF4g1fXNT/ap*P(Z#Qs.-@;58%K*hYO>-#>u,mi;c&(WT#'NGFb[trA$v3o&/]L;$33=uc9Wm&-b1k$%:*/Z6O^Mj;nx4_5I0WD+"
        "vaNl]gPct1A#1<0)I*W-^t2Ra@e*>lNNVY,8LCP8LP`2:x(B;0H>O]F+8'N(DNsu/m@du-,t#j'o(Ls-tCjc<=nmq7g`Tj(G2`#A@#-9.>,3ClM/_T.nfse).;gn$1]Yi)Yr0s-0+,d3"
        "aDjlAjA$=:@f)]C$ZW+6b<A2TAS./=bCkg;IvXdGLMBlo9BgP)V4OZNgEm8<f&ITFmf^H&^KU*Wm,LU%4U8*SU?W71A,8>,lTc.qduW]+xdc/<cp'V,#%u1L9-2JV+FRA,A_^F,1?DlK"
        "Z,G[#`WF]#xr_v#[ppY$b*:j$1PP##Y1P;-:$<;Q%O<s$2`_V$>$<A+T]Rl'@mb?#$G+877Eg;As%w@3C(T)3*rQ4(C>@12F5_t@?mSL206;hL]c4c4C8U<-VA,n%?`R]OVhYUbq'/6<"
        "n83021['q/7]1er$@(S7x@et8&^n1>NdJ?P-&@I2v[-.#K%ho.6]^xtp#.87)n#5A$+n,+fr$^=85s-2c`Ni4k#o]45FKU/)m/s9LqN%bm;v5&wYl>#Irg=ueR%Q,vvJA#XMCC,K>T>,"
        "veuoA$4Hgk1T^'BwhUW7Xd-X/j*L7'bp@J=aNvV@)OU-<QX@8%HdN5(J):a+3(V$#9PWEnw=$:A6T95()Z)22h?Y@5@10*ba+OV(T@Os5_[[C5p..G(i_9D5Dt&9(>Fd()LTRL2MX#NB"
        "uNOQ'n?`[,wiU/+QS4o$n1f]4.W8f3V/W[.&+p+M4pSF4h=S_#D.,Q'cF0+*EL+U%bIUv-Di`XcIc>UAOPF11)h.a+X^7'5&Di8La*^/N3LMRV,Z698`BaE4q$9n9YoURB6I2H<B;6H+"
        "n8&n_1=Tx?/f&K;TF:jEgvNnDYHXF$sX*;.NN:RLf:*v#9D2p/44:j(ln=.FQ=q@BC.UFGDSf>8iG2C=oqqV0&/;uAII6]BhY#^td`Nm1LU7r1EaD0(U-vlACIkCI6Wl/#(=S5#?'kt$"
        "Pe[%#^>-&#Ie^f+uVmg+&f,j'8Q*X&PLx<$O0Qn&CW_c))II=7Vr%N0J''9(^RXL2b($lL?W/[#(o0N(lY$3WB@[1*SS[S@hBeIFF4Ka^>H]49e(On^%]9%M*e+_RO^82%NhA@Qw>oK)"
        "<e@`3%/5##p->>#.DH;#74i$#WC91(l^][#%IlB$<x2:%Ai7[#N`cd$aveV#>,3Cll4oi$ZPr`3?U9T%><Tv-fE`W^_'tP197PM(N5:[6F&520jh?$,55CD5^2PuuxG?IMolp/)s&^f1"
        "Zbn9./A0#,DL^H)4m($>$wc(-f9k9%9rn%#bk)K3oU6vmrpI4(E'O/)Ziq<-jDl82AV)K*]8Ba+>o)O^d:a7)d9V_u:vBTRoT&R/;v8GrUBIXJf7(QAQ^TvIn_/&$.[.%#%HH^7=u_;$"
        "1u6s$sf_V$2G#i7>U%0:BI.W$,Z_-2Ito2''.o#>=D###kCwn7A-S0:dp/I$Wi_v#K04x#[.i>7@4nM9oGD8A/;Eg1#G+F3'F)i27&3D#W$px=,cZp.ZG6MNr0PpuhB#DgJ(,x]wRd]4"
        "7F4eu.?pM#sHk7@r>6]45_x9V,p-qLM#@##FuW0(O_2Z#kLrx=))###O=:D.ci#Xln%m8.K`Ma^uhJg:(=S5#`2^I$X-_'#+Iq>.9e:e&m^1BXpr-`&d&19IF$^Q&W^V61qL<*2#_=C#"
        "E16o1do6C#Kpok93nx<.kg5j'v=G2(L*E78=dA,3,pJAl3sWc#[]_#AM=Is-u-MkL7Y^Gl#YW?'tR76/YNU&$%Q++NudVh)pR3EC(`Y$8qW-nD3$Cd+=1%8&&Bk_?:P.F/;=Q'?mr.,]"
        "CoEtL8,NI?Y8^)YUj&g:LI.T0sc'62kE:FaiL:u7asg>$,xNo6V2PX7IS6h*hi#C+8cuN+0b`j9U?Ll'Ft91(Xs261]>Ak(`_VT0025a8faHXL3PsH>M+:kL)S&%#)iOH6M$_*#R4B^-"
        "mPHo-WvkA#:mn<&]]Wc<T]vb<FqOs%<dcRq>vBK'0:7KW5WgJ1[gHpFU^9w9q'8ZMM4$>#j]QTN(3CW-VZRXh4JC_Q4nlwg2A:_Q%#Z)hk0EgfJ7YY#:8Rc)%h.eQ^&-<-cWFa,b^Zx>"
        "C5[C#l9Km&RVUl2fBm**]jGn&j`/`$^lG)/a,q>7;4vl;`c3?%GJ<j1dlX.bkX2u73E0_,]YVO'Xx*Q'1'849#_c8/tSCnCZ*<v#u_WgRA(u[5VDaP:L1RMLh[%t-pUR3:iv9^uuauXR"
        "w4Hb+u3Sp.9wv9A(Nk</?apJfQ1PAG6E.^#F%i2(EY2kbD>%Z$_)wA>UkRR'_(il2>uFm'eq/f+7@KfLW4Cs$sg(E#T9)G:fqS,3FYD<f]McL:R+O^,Q8xb4F0(d%YruY#W43M-wLN3&"
        "GCeJ*;Zb]^/V:v##-mi;RN0H'x/CRa/W9[6J:'7eG&W[5M/Lv-@Kk785lK#$x+4i%X]bo7$=u2$&Q.;Qw*%Z#JqN%b;X9Q&4nv`*)(4i*%ocJ(/F=$>fvc3'Q.d##4r9,bq>`P-d#1s$"
        "S7JC#Lbn6%Jd:T%-#6x6$kmL,eUM%,vEpMD8%YgR>+l@5%CH5A0UOi$BXSmM'l0N$^Z2e<>85kO98N1pt06kO9C](#SCXt.d_D2#q05##(8;d4;&Pn4oR3D#?$_,VeJh]#Hpq:/1l*11"
        "d;Uq%*DNj$(/Lm(T7w@33jq3A%fWd5$Qrb=NIr)-4hdq*Wp8a5Ni(0FgASJ;>1Jt7;x1J)Ex]6:Ia0b4w?1;B8rKv7h.1XBm'CW9W-wU0pvi[I7NUv-g>vc*Ws3$6ujvh2%[I>6XVNT9"
        "+fKQ0wiml0Q3@d3gp]],aI:-#)q9N93trs#S/K*#BGww[O6u2v[JUX%+H0W$EhOG<wR(`+Zi'j(#_uf(SDi0(j`oA(*#=E(Y+#C+YKdI)D$O`<.J+87lVcO:<R[]c/#DT.I4H123nM2/"
        "fB:a#VEqt8+K&Q(?VGc4L=6g)*A0+*_pBL119-3't%Q++En;71h0nkD$_B1pCMDs$jMY+%^(c;-_)Ke#'g_$n+]jFMiUhKYGkj0vv0.<$2le%#[A?A#FBq@>waVoA4kxx#/:+$>sA2k'"
        "f+Lh&5Y`8gudgh;q*R=fX[-lLriYVHXQMm$3:bJO%g7<E@e.]8aEK#1#dih14&qe#Y*)mL)_x^Rgg6ct4c9s$Ju2'M:oj$#[BMJ(ls`p'?D,R(^#Ke$bUjp%Q1D0_TEr#)aR7F%Vx7;%"
        "_J3hL_RK/).A0B)$Ek`<Y,[-)s:M4)0MP>#ci-n7)$Iw%=Lr?#P%*Q/?A.,2.Ae#At)h20[02@5Ck1o0nmK6a=Mh;-lhgk$su(8I0;T3d.k2)+K/3a*2tYvcm.#J$*TCh-r:90+r(bRD"
        ",m$qLPH-##<PUV$_sO,/%Fh__/F_E[&QeKY.OSS%-U^*#Ps+$M=3DhL8a=#-M:O#-2sH-d<OC-M:RGgL=C:b*rURP/R^Fl7<U%0:ZKe`*KIn20.S5c*hDdJ(F%9_oeCsV7*>Ha2i_Tk1"
        "+_QL2A?6e2N>@L(fGZl.A)_$-#onr5.t.*+mcm0+$e<&+H'8%#&;-u@/]#K3j4rkLCRqkL,u*F36U>n0bt[s-Y-*kLf)+c'G9#g1E4NT/]*;*'Jc_8.o`[D*i?7W?lo2<YLZG3'L3Ot$"
        "co/'+XtTpuX-8C&['f<$TVNe)ggV5c[<K+*BoUv#c;dR&KcoUmxa=L12e2]5cY0/3MddIU(%`I_'['C@iA6;Bg[@DE5K%W@k_'@'1Wn0FCfadt0+,##I>uu#MEp*#Twkl7XW?\?#_j$s$"
        "$sXU$eMk>#cYC;$nc'?#A6,)M<sL(&(H3`sigVs#a9R>#A.$c$RRuu>i[f]$.s+/CFDgY#^b#&4maeY#3f:?#?RjP&*QnCsp)xl%dT3?#3,l/*wj2T7K&iKEY(HZ#hKVY5(?FZ#Tvl3'"
        "Hn.;QEB+[#F1%s$&[o:dH8&I&8.D?#ji,-;d(fT&I2nd)BUE9%H+V$#'<-,2h[0Z$cha#A6^QBkUH@m0BSQe$:a<F*6&_e*xWiiL;9iHMJr*F3D;-u@hfGh2dHT#AlZM1)lwH>#)YiS%"
        "&5nFVdNgkf6)LVH;5/tuqqRD3LUY+DQ'B4oOIUl]FNLfLVH*Zut<)S$:FX&#qRh2(AeJM'VH1x$/Z?T.=_9Q&D9Mk':Vnh.+%#n.FL3B#TFNX.K7S8%JQ]._a?ds&WIs:/sn7l)$;JfL"
        ",eH=7'Z`I3j<n'5*DZZ.f)*i21;M:8a7+Q'#ZWBmZrgd4OOb?i$`Fi(HpQ)*.qYvcINA+,*crl<)kVf)R(JR9pFg?5atI.UN&TJ=m8-;B^igiBoigRLk0,t$-.*E*Y.GKVD[)E=[j5&4"
        "Z_vt%0OCW&s[_>#D<,3'MD4gL,::Q(.3r?%*$7W&nQ(_&s'7?#2lB%tFA+v-_(]fL37;$)m5Da#)jTw0=t[m(09r?%5c@j'l`c'&P$'E<#G+87;bC0M(H,,2hQaI332I<8')9t@h6s@R"
        ">CACuY_R[u2@JPuaK(5K1T4+Rerl?TBtO`u4*oh$Vf0'#8Z9E+E&SU-s%p/)E-Yn&cLOm'Yu,@#]w79(o&g30Eclbs#$P#>Wggj(F,V#A4HS-HT1hI3Ye-D2g[b[$oYRD*IJ7&4]77<."
        "CC7lL9kPl$Gt^L(YsHd)fBS@#IuPt7&)=_A-c8?KkPs5M]g<Q1DQx+HnHT[6@Ht4SPl[HF(L14D(N*5D.qGIFx&YoR<E3Z6qmgvD(_[H)>tx^]Y/Honc@aiK@%vu#ZEb&-Y?FG23M:#G"
        "]=m;$(@%v#$uf&$k$I4$4]uY#>####'<-,2t)`1AL8JC#63(B#@Uw,%vOXn*$I9D3Z;xoS@OU50#3-9.q#XGs6h#[+m58;QcNB>$<7T?+n(GcipIY(+o=>R..=Wh']Jc;-02^7.L;<AN"
        "77Xb3Abg:/9[fNJ[&'tqFWLD38F,gLcT/[#KTU&$PbbEnD%BZ-D^Cv7Qp'B#q1OsLc7XpD3RV[6uFB9rQQ4m9=a5l]6m4L)&;%eu/+Fu92d>N#lYjM25lOfCv,+Z#Np38.W%#9'EBOq."
        "?CD?#Q,[G&gqN%bGse-)YjqY$_Id3'F<n8'Q_x='(;g2'8Z4N38#in0h%3,2h:H6f_Zgf1DPF/M'sUE4S,m]#9^UD3x1f]4.<Tv-h-uf(+Lp60xL6&6nZn^RBe8V833n12WFVt-cs`;M"
        "EQR]O[6xW$Gs]*+DTa2'?x*A'X/5##Bu8M#mh%-#=XI%#tR)##74t]$<H`IVrAvbaF`L;$'';v#K:6Q56U%0:+)DjTHk=/(0fCZ#d1`#AnAET#>HNB8@%=F3s6(:@%VYLO_w?0J9jZE)"
        "T<3D+GfOI)T?E`+4h][#[f@p$Q03'8^x9(%at?>#x`Q1p]J@8eMl`r?Hqow#+8`;$9s^R/TY^Y,PhBH2H%GmfAXj5&('Dh(wjnc$h-C-MhK*k'</6X$J*G/(Fn[%#BR-@5;Rcn0YgK?l"
        "F?c#@/jmZc$X)22F`'i2/Io8%c.5rJnE(PM4YvrHjWD.?$gL+4EQr.OW_m*4?L7&6s*8+%>=4q,u%5H'E>w:OPfN:;/$P1$UeXE&oTCsd_2_./`LOfLgf:*#O/?Z$kl9'#Lmw3'+#Sm'"
        "cB8[#&jKe$#Q[D*0ioS][B/nL+op?dM*Pp%oplO0*(ZV-@P[N'r#d40,jY>'lgE*.Q4P/.]VZ4.o?Vs-%m?=H$QGvG1)PgL(a/@#9-nH3o?Ke$'.RLMK`_#A@Rgk)7t6g)&ZVO'a2,F%"
        "$&Lx6_?k=.&PFb3UQ>MTlk0<8eDBO2)TmA#:)uR*EsP]#ZuZ211#B]cw$g'RB$'=.mt4b+Ss)auCC@37M$a3(CV=307e,QhI*#'?>rB(#LhHK#46JT$'Mb&#w9`b7Ot:8*)5%i1T+Hn&"
        "+kaY-I0axFZ5[h(Q`ED#l_(^YO*,##Hsu29O6l5&Ntlx40k]A#mD<a*ZCr;$YdT5#h>%5'/(Vcf'>_Z#gVt*&Tk4v,9YC^F8cu##kbW#A-SE126_@F*h4U_c70G-v;+Ns@Ix^t@CoSj0"
        "c$dt@u/41)At>V/3BxF4ae_F*9Ze)*ig'u$pH3Q/u;Tv-I$4Q/3nVa4b+XT%d.,Q'x@_#-aIT92taQJEdA)qLV;:Z-^FKt1v&NGFZdgsKkrP(/w%YI3a(49/$)G.38/W^#x[G72S#$I-"
        "c5SML^>3)FqZ$W.o]-x-&mplLao29/W0H&#[b+T%E'`_$ET@%#/i&Q2_,[-)Z%lu)ZM)o&1LYuYGHt]#l>^lSYnPg1=Gns?;rLG)OLV?#fN,##64_T7EnSm&Un%t'QmCg(Dp>##Ta8wP"
        "xK21)5MShLs_C/)VJik'Taqg(f7dD%=?%12W0d%AU,S4(woV-M$9,,2=(Xn/t7_Glt[du@*8kc3/mOP(lZ4N3Zqdu@1oH>#Jn.i)BPYS.4t@X-`MWfLB2MD3=XFnufiL.h$sE.h<8#H2"
        "SgOna4^^ulIK#uURh+Y#+l1T.&####sD5P&Xkd##mov&#gCxt%;4j%&^j(Z#,7D##,l0T7sC%aa=Fw8%1`Cv#$B0W$Ye_E[GMuG33rxW&Ld/O9M$59S&BM$u-O.%uV4c^%PV6,v,Gt&%"
        "2oA*#7A>a7'-b]%Wl[8/80dS90X&wu]lUh-njcV-HG5H#1fJM9r'fO9ec6(#$&###dD-(#`b39#x<3T%Fv>K;Agq[>T-U2'C>Yh)@mb?#.j-P7R.kD3#T6/(mqxGGCufBl%F]#Ahb[#A"
        "DA?k(2$L?l-8bi2EB[C>:4Ev6-P)1Nr4Ad)oYRD*n`WI):Tl>>ZoBGd+fR_#^]n]4Z73H*#C7f39>Wm/-RqO+J48/;Kn?-Pl2d?:qbVx9>Bf@5-ScEG2QZ40ZsVN;rv3'B^rSZ$b?]69"
        "5DpL(JIGR:U)OQT,$e9:O,qZ-UvCgE98/:/DFYH3H'Wn'MIJ`6m:bp/h&LS/6iSi1c^&&,R40'7x$rj2k>O4<OX$u/hpZ$()A0b*ag/A,#A.%-3>77Cqr6a5?Nd3'%4kx-SLMD5@8[M1"
        "`5YY#iGCm&p,fciG&w%+epB^-$8,Z)TrE1%e`+/(_cP`<-SY##3hhS*Osu/&`H<?#d2]t@w=EA3e1`#AYB/1Ah9JC#HjE.3ig'u$_ims.B+kT%0$dr/;<ZV/Q])8IeK4:.X_=;/QP6vG"
        "Eg/+4Gs8+4Y4:'+E,d'&b>uu#uUN1p>;<8e44co788k(ErWC>c(4It-tO`S%0E=ebo9A`a9t;%?Y[>9&w),Vd0=.x,QWdkK'F#.VFfl##'troARb12L6i4R*9iIn9A'ICd>j*87nje7?"
        "`9]9A-3-G(m7eBl6G'HGEv0N(o^M-M:?L[%q^Kkrm_;u7F^bA#jj0W-/:A>K;X-m8s*l,2=YP;A4_RY6ib_C4q<@c*>kT>.ft+O3l/9?CaG-#7uEOCaBq$lXvFBkaBMXK4Og%*FSmI&N"
        "SDw]C0?CW-os1b=XqW[I_<Um&wcL_&7xg58k%:UCRNa^-K=d%,_$r%9jEF04rt,uL(VnmL.*8-#j:^l8%`958d6ZY,Imo(<lu.C#0$@>,p7K>,Z/q$-PUQnK6(^[$J0VE*Q;P?#owO?m"
        "]K+A#5a#s$KTXY+#@KDllNdRl+X)2223lZcGh.4(I/xghriB.3WqRP/h&L9.?;Rv$DZ-T%g)]L(Ls$d)^^D.3#IR+4:)ji(*@0>.==jC,6R)b6fO$W08IYG6kldY3*&o*3*nqo0Yl7D="
        "-Dj4'F[ML3TWl;$6[AD,14<V.AIZu5s.*g(hwBG3(dGt7,=2wg&cf+#0kY/$FIY##&TZ^7GbS2'nJiC$8aL;$ALws$EBu2vNTFs%:fej'EBd$(@E7s$^w_>#/Puu#K$96&/GY>#=l1?#"
        "`8I124/Q]6YKNP(d5Ph#9aQ.2XlK4.4*E:815X2($*Lk$w&D,)Qd9B#?CnG=iCEYpR[oReH/DA4=KCTHn]B20f&DbueoK:<erX(Om1n(#6>4I#2V1v#EAP##n%N^#;8l/(q(;K$`=h>#"
        "M@PY>xd:C$bBe>#C1wo%0c_;$4m+6#=5dc;9F1v@xvPBkh9JC#4t@X-I*boR:/]>,8&@JLe*P:v5'w4%RMY##3####T,IY%<po##&,3)#uN,i7fJin]miCx#v.9Y*SK]@#Z2kE*xsax)"
        "pFte)H.v##%far%[)O=$>x,##>_2of,e@vPF<'>$PZ=59k=-C#?JKJ1<2a4-i(J8#k$q+*$huVdJS<X%<[[x,[v;lK1eP.VKfl##uZMoAJIc1L#oC`+jGKW7((Zh:lZH##Eo4d2nl^t@"
        "ax3;('Z`I3:B35(KTDWJCt.>-NnchLb$$@5K>_3(rq)f$$Q7S#?#ht@8L'0%>arG)Ppd)*rYRD*.1M8.Fl*F3>^d&Qjgp7njWse=V&_rQoqw%-2vmO0b&0Q].4+*oR;gkoSt.$6_E^99"
        "Z=+U227H6'U7Xm9)r.@#&Z.1$;9@xb:a)q/%A_7%G4Rs$N8g0+]<As$$7Am&uVt1#K,6PJrkFfq]4/,)8LCP8YicxYu&(C0iiIY2XP/L,+p]A(dE8[#i-E-(O+v?#o:I['b-Gj'_)2j$"
        ".WrJ1DcW#AQ&3,2%9-9(?'%'dn%xU&]/)O0Dxd(HpRaf>bcv4M3n%2372oR9DxC-4n_H1MJ+.`,^m.i,Q8cl2^vr%87=/`,7miW/<RS%-A^'nLS*$kC)M(Z#*qSP&m]MY5P,/L(CtET%"
        ">Wv('nTPipD<F&#$?)4#69RWZ>-sR8iSE/2)[+t@#G+F3%/fO(QHdm:WO3]-;(O#+6-09.?5-/U/.Zx%hhl[tQHC>#Kajf:bO]Vd,lFS7c1W>lTIWG<:w+^#>dl5&l'@V#P,g._2lBT'"
        "D'$F#9*5Wmm35YmAN2878*f()7vO4%@h&9(GqZ%3UxF)4&&8&4nN7/&+#3T%68:W-UPQ=cICJt/3S'J32lRI4CIB8B5+?D4;D.N#m3W*G4@JY8WUAZ7`*AQ2v:d>@:n<G>+l6wL^_u]#"
        "I5RqL+E/(#YkC&$d./P')w$;#v;s4%w(w$QcM,1MDEWT/UGp=-@B<&+ZZda$]lr_keM/B+C&6<.&4I()Xw;^knWrkLCo3l(G2`#Atf]?MkW;]3suAhLNL,@5F:wJ)g2?a&_HaR8m.>)4"
        "kaNp@C0Tv-@Tsv6TOr_,r10.=AU2v/<wQV9FLkMM)'RMEO1k^@32b/2dAYN3]R4s8JTmjD#%,8napsF*'X>5CvKSx,D`Pa=l@5EGWOG/2H>2t./48E+98oc=P2rH,e'[##omKn&Gl&m&"
        "ux6K+x/bY$TmU&$[faO'Qj'B#w+$12IdKF($2k@.^L;X,45O%.R$aC?=#$##'7SluQk3=$5(+&#u'h5'(74?><0O>--ob&4c$O*,veD`+HBm**i]mr%OmwJ3=a^u00Y#u@xK7'/D`cd$"
        "H$nO(2rwJ;Es]@SDhfC#_=&M=R['P^beCH2##P0EKDlY#=GiQ#_gPP2?g1m=KYW9/C`KV0uwsA4(&BBG4((gCPa(,)oR9K17v>3#HkuS7,Wk;$@q]2'(Zdc)Ii,##O_%[#ECr$#Www+2"
        "WxsR#sS(9($BKv[]S&N%f+tPAdZ?T.7:vfCxBId66p?6Fj>[MXgWH_uD4>2tHY2l(pV27P'gj53E3*B/W;#F#CRLpL;smX78X(1L)R>IVD0S49A*4Zm1S9t@e*DZ8;L<;R<J.X-GwEFY"
        "Air=.%sTlS<Zv%+gjb&-s7be)V]#v(OQkt$%Q[]+7&T$P:DIN'S8=.)i4C##G@;C5aQ@V#k?P?poW)22M6oO(>qdT-rdm5.=.a:8A=Ha3][_a4)_0#$69tCS/8Ek'Q)+h<%$Qv.Rsvx-"
        "K6^[u.cK(%CA>v#f/c:.`v*T[7Ll40>Pio%@6kf:E7ZM^f#U`39JQ7(pF>/(gR%0:#Mv=l5nvuPKZB#$[S1v#o_@q%;UK:#Q0(N7d+V#AESO12q2/cG?u:S($2f]4=Iw8%[uLINgT:v#"
        "oN)-*2;VM^fkQ;$B=;)5.7_N(xE&##kwx&$qGGf$SB%%##.]9],Ie._8L;N).X^*#vvkA#B&B<-:gl`a(ovK:g@#qr_Ad1APMD'GH.0jEi@(h&BaR]OKZ@x8Fk)r0JW@x8GwD71SNIb>"
        "ew.F>6N5&F,pP52,0Ck=)^,p1)':k=ZKqDI[,+8M+7w+##Bv>:8e'^>Vi%tf@^_PoaROgLj>lP&&FtOo,p4/(4.cS79_J2':fY##_Z3L#(QG9.MK%12&E59.9,tZc+/wJ1QT^1A1oH>#"
        "Bam]4`5,+%:815SOO&>phsBA4U4oReA^hpHjM_M',7^F#+j^2<)tV(Ouba)#cc387_*m]OZK35/IA'##duRD*pW%##jc@;6-6J._[<+E,V1#c*SqD=*vaa@#:3.)*wS0@#FKPJ(;'LfL"
        "5Hk/4ot).$g[U+4xUT+4CiK21c,KJV^:%5pOe@)4qAe&#PNpkL`5_W7j%gb*:OI@#O;4^(((<N(?)B>#nfDX:pxVt:.0?#-5l@8ffcCwT0(fwTdu>X:2YA&$S_Yw.u@/$$&W>wTJ%j.5"
        "Xp>D#D2KW3ROjC#@M$/C0,hL33FfC#U;DD32Xgo@rq.s@-UsHZ,KE(#ib8au;APB$Se0'#8V[Z7:Z],vowkU&l^)X6(LMN9dqN%bHM5_+T5OE*:#8nT8W.1(fhx+2^Se]m%x,@5aRABl"
        "i:H6fW-Yt(9fZ%3(ZWI)H(>jBf*g,3X5Du$jq@T.&&8&47='w'TGY7(ec(e$?Y](><^#H=4kK#SiOh(4wUW:8fI>q=m*Zs.V$rO;:Im&46%x]0&7:KO->j'@mu2U88H]0)R1C6(S:Q:v"
        "XIVS%2OiO%je''#H,>>#neJ&v4mc+#Ycwc)_xp._:0op+iRQp./:](#jDS;2I.$>Pb7r/)<&cV-eS*dEr^pZ1=@#+#M,j&.3'';R^4aIN)(l?--a2E-Bgi&.VZ9wQTVvdNla>.#vIo31"
        "5NBw#oH[5/>Bhc)/sk0M6t[(#8Ee>$].AT.DwF(-h9YX78UGip8S*Y+:f-A#ID>;-irxB+?t*A#B85##5SZh-p-HT0EH7+4%2Puu(]qr$5(G6#E@%%#noFw$So3t?FB5/(<Er?YoUp/)"
        "'Y%5)8cG>#WN1,)b&0I$ojsV$RN^n):6u2v$+&Z$O%('%Mno3';$X=&Bgn8[G?u/(Mqws$B4gQA;=r$#Q`$D2Y&+22*pxGGD@DL#Xj,,20?)tm.6O<:aiU#A]HK7cS?:`-Bd/q$m^'B#"
        "T66T%S'lA#)aZv$)KuYu&SSn#(1.SuRK%@uI%RctDp-pE$4#;6-wSkuNs/%t@vmxX#)>>#clN=$9e<p#p^''#+Pc##Fno=#:)5N^mTxW.,m:B#>l%N0C@uu#_Wqf#KjAKMkWL&v.mc+#"
        "Oa#m&^rg._WQ%$)s%'58qI8;2Pm.87]9/W-D.3U2XC'h(f3)B#Aj/wOU3F1^H23E-1Et+.qB<O8T;$_#e-[5/3Csl&kG7?89EWv64c+/C_`O213'V]u;4WE-;xYO-G?:@-4kqw-G1rMO"
        "Q`VePA/;hLNWb7O^NYO-[>(@-kdQT-<gGO-4<C[-8S8q`lTL,E%H5r`9$>r`nK[Y#R[$##5T]M^Rlou,PvAa5w`rK(e)Ob-(ge`*cnX0(QXC0C#1$RAKY>>#rIa)#BD]5#X(5>#r(FS7"
        "%q/nL-]R&,f59F*3<>##f;SDlspcn0.`AP3HS6/(H>@12OB*u@o=+7*f/41)Uctm$1Cf[#r,9Z-[BQd3D9p;.60fX-2)H&#@%v8i+@wsA$=&=YQCRK$Od/<__^_`<wj]O#;47w#8*ic@"
        "(t9n[,?VD`hOU$BYG2`8'r;kufW0#v@,78#=qa9%+e''#U,>>#n>EU-s`+w$@O[5/)]N/2i7^W&_LO1pj^JvG%2C,3>4.W-'t8X1q^lm##*_^#<7d(^EHZ<2jJgY#)m3^#,W$x-n,AnT"
        "k9&,;0FaX1+50Z$qPQ5Mp=@uLp)epM.Q6LMx=%0^.G9)A^v0:`Zf8YYwc_20_(j[$OAP>#<O4:2jq/T%E),##7Rd$$+n-0#p0EX_p^cIqJxF1:n<a=lGj@h(Sr0hLrR5;--R;s%.R;s%"
        ")xK#$^(AjLnj>l7$vEc3#;@8%;RjP&#$M($9b&6&5MlY#IAtV0>WO`<jPm.Lo]1>>R8St%2[+6#7c`#AaD+F3xb0B#I`pO(*JMs@Z9bi0rCbA#`k:Z>^-I>#Cl?8%MpKB#pn;xDO6258"
        "KXhR#1bbCaOH`e#KZ4jD$Rur6HILnu32VV6O<).#NAm1KNcY>G?^DA+%QYc2OsAa5_8(_,pA3<@V/ZN'`V_SI1J_c+(.k33_%WP0+)tSAV3#R&aLg+*kTwv&Sgr%%V=Y@5g<Y@55nIgL"
        "J)`*37OA8%u7XI)T@h8.E$4Q/E&o`*9f<T%Hj3D#fNf@#`GUv-H-Y/(xrA+*tB:a#ImkA@b>^BpPD/7<o`EB@MmZ^WamWg<6m;L2alM_GE74rEssBF?,2I_GA(fUEI;*,Sr,0+_(S5sc"
        "OfI6<D*QM0M@nc)NiY9/ZcoCMMego[InhXS5+L>#(mx+,^q3Tj1Tf._`:?w@o*&;8Q1^[66.bc)K;0Y$X*u2v$)sc*fwDj&%V:vHbCIdOo'VS.@dV2:xgpZ1=@#+#R-j&.49B;R&N7_#"
        "]A0LP-$q1KuX[20Lp,Z$M8G>#L&N_As-/t-W1brL5CNMM&TV/#59OJ-oPgc.P.k9%^h?Q%G/5##aQo87Nqn%#-'oG*XB7I,Fi5/(^E>R&FAf/Cb%1'%Zi-t$#(Z;%=ZHm/,&+22T2-G("
        ">94Y$v,^0bjcMG)$9p;.Fl*F3oCTUJ0cki_%pJ>'fcN9@Dv9buJgf5uS=[D%4K%uL;or,#n1ST$iF=&#rM&`7<*/n1Y,ii'Nl[.2C[)V7@kwWAFEU_#Q3=&#^(nS%kX$##Yx'p7LtTw>"
        "CFGC-,T8($@%d)>%E[&4rA`f<mmXj)n#j+*VmwJ3fSMDl^b[+%$m%FM^^$12leW#AR<D'f(rUkLv_.d)Lt3<-jMYR%hp>6/jG7l1'SL/D(uxc3^n0T%BgP-1n]Jp0-C[E5hs(^-NS3M>"
        "`H#G+tlsw.N+7f*5RR*5X+`.*gmK$7R5oc=xuI>8]ojDGgp>vShH`lo;^uB6o9rf)nnA:/CPDuA3U=3=/cd)%j9'M1@6]#66fNp/qfoT0twvG4Lt,R0[i+.)90$dt9XtYDQ]+9/93T:v"
        "oCN1p?I6W-:R_l8*`$s$Pki2#D^lm#:N_^#w?t.^i9/t-KbEsQ$DN24w-HU7G=2mL,%KR3E/Mw--cR%NMN2a-&Z@/?`Q:]Ntw9A=]1l?-ZB2&.eb0_Q%QcciMrx-EoJ?D*vouY#DW$$$"
        "$h+o9G;9ji^$:S-Y[vD-JN:@-/KU[-K1$4OO*Z&#-2m92,)Bm8i9px4fxP`<O]1H+%4Tb,Ii*<83ij</99wL<r%M4VOf&5:?Qu2v`r)7'C%xN've]+#eI^a#)bQ+#=()Z#K-p'-nO[,;"
        "2=[>6X<<e3CPV6&Z&SL(PmwJ3GiB.3Ii2P(G2`#AsNx@3#S'tmkE`3(jP3*sF6ja$F0qk$lA6X-otCfu&`V[GLQp;.?\?r#?HH1a4TL+A[b86N'gOw]G@FD8Ah=tT%JtK%1qIx<Bb_hCu"
        "Z1SGRBCun:`(5L<Tr0+e/[FXAO_jWJ&N-FBwJjX%E2:OF1:]+iux<G#b.Y7'NV(H>wk4v'lpPM4OGeS/Ir.]@b^bS;sThiBa*AEGJV?uu>u'fuXC-G$Te''#1Md9KA6(*#:)5N^Dc:?-"
        "OrQj:>p*BGJJ1>lG;^'/f66WS=0`N(Y-u2v=pbq/g44WSacC[-n91?7TU$lLhgA#0:/Mw-)J.%NMN2a-@8@/?VtOwNegk3jSA-Q/9W$$$43E?6*+p&6J:Q:vgBK_-q1l?^B1%Rj2A7Y>"
        "n,Lh)C/lJ(X*u2v+PF<$NldT.)7YY#Ajqw-L6^kLuEIU7mt0hLCk[G$]VC[-;_<X1OK#Y1<:#bulkX0MYIkl&,Wnxu75(@-d#GwMiYFRM)wnU7@O82'^UQ>#5gK.;t7g2B?kD]FAInJ1"
        "tBIP/&>$;H9#`n/Yns>-s.B#,FHku-'$Px#kE+)6U(&66Qr-j'BeT)3[U]c5@_d,*+Z'Z-xk?r'XMJ&($gOG2Y87l'h^FA#qOT)*.j-P7I?%12.E;WpIZj39Lbi8A*j*tm09E,Md(3_="
        "0TS1=-C_F*#2NT/-2Ue$j;jhLsY_:%;rc29Vx>K)SwC.34aXb$v^uB[JMC#I6;&XTC4TVIso_f:TH)52a3m,6XQ4f8vl]K`E5^7;E>&_-E;pR;/FaV6wsMT<l4cKIi?8'8kr)sHAh`jG"
        "jZ1G+3qa61+39&8`sfk[Kpb13-E-L>##;m2@+P:v'1)##x'sulGsk%=*`$s$qNV=#,B@W&'Et6/#SsJ2ol;^Q/<gJ1MrUw0BjwJa0t(:#D^lm#'N_^#i@I-^DXa'6n6D+v@]C[-sWMe?"
        "Jv/kL7-QC-2/Mw-K&j[ONw*1^gJZPMZl<+.3o7`MsuBKM+7F1^qlNG@VtOwNiHb0tH(`O^cegN(1`6w^ktNN-r^[+'NAB##1wbU$+=A5#=XI%#DTZ^7Ax.C#v9;c*5G4)$bMvl/dEwY#"
        "4'u2v0*1t%'w<;$%@%@,Q^FA#h'xo%I(.1(uZZnAfs>bawI@:%P$&%#0a+6#QSO12'%Kc(T6wGGig:I3<8V#AI$gBlJ041):eS@#4.ZT'EB]r'^B,/UhAow,PslO02=.=-PmY40H@'#("
        "$P7CSvR[Q.5BQ3'`Y:Z-8TvN'O%)Dju0v>>&5>##O4UN$=EK2#bQk&#B<6^7xi)p/0w]r.[lT+*Qn&6&A4D$#o2k223OqC2fu?C#DGk9'CZl^,Ct/j0QDv5&YR%W$1?Gk0AwQ)*a*qRl"
        "PT.hjcf_#A'jl*;>n#K)Tx.nhPKv)4v5:-*:>`:%Iw.g:,>Qv$*wgNiA*(A$q*.B]bT3w8'b)T9[jQpF29bkL:NY89J`&+*px(/;u5,i#UHKuGmen'&([RXhN$I+FUaeEGxfv5/hBiU:"
        "FU(?N0K.`#HksQfKL=Hu>-#kg#)>>#d1ST$Td0^#96H>8dt4IWBSYn]w2t6/#x`.UvBao&sn)N&)uqvHA%(,)AVQP/M:D#?&_9$1D3R[%]@_*@Ou;vQ%Qccir#I-E>Zv%+VwTS.4:^fL"
        ")+41^Na6F@+ZGT.Cb3x#`&gN-2.,k-5fl'A)DT3OOKl50MghX%b#@T%hX.%#uJ6(#?I>/2UrQM39$SP/=Im_+c7r13weL`+oq'l'UjQ+#x7+87U,?<-xIZf:skQMNwqXI)#0Tv-8T1t-"
        "(u<^GhqkA#baBZ&VQLm//]L;$D/8?#,p8f&q(fmGx2H`IZnm##S57Z(2E:t-R(4GMr[cci&IGcMfxe$]%FmU7`h./1^UQ>#d6<iM]Ha/'@a:&bpJcs-LFqb=hP`H*_xp._B%$w)jqw#'"
        "?DN1p7Zx]F*=EW-iF@'dU[-lLO+$H3ij8c$,(N<-jmV&.4EdJ;;b0^#pq?g=AQZX1g[@PpueG,<LGdD-W<dD-0$CH%P6)'#F*s%kr`ve?UKVMB7-CG)Hn+5LMSRU/soUT/Ia9B#>Mm/4"
        ",&*5Av*%/_f7ae?Kqs%=/09X1$#m/*]LfE#Eh';Q/F[?9E^_E#4P-0N]i@mMU6],v_[IH%c`JV%r=@5&=aRB#*wBB#=)va=kg$A.kBD+v3Xd(%_QV2%+bXrH/I?D*I7//1l,6/(b7%M-"
        "+%3q.:/C)4[po(A=@#+#%/BO&;.5b=:i:(AeW?(A2>.krFvZ>-O%Pp/>NTN:*=)s@fEx`456.B#P;b(NR8LkLP+0b.imPk-PF%:`J_<D<2qGcMS;$hMq(HC-tP)1.I+[:NNjkjL>bF72"
        "3tNJV.,kJ;,^2E-Ls.B.4]Ct0^MU#d$#gxtZGO-QsC$###C@G;G<6WSu(`Y$Y-u2vvFJd*pRKW&R(O1p?I'H)<K)m9PBD^#e4W'MxN6N^U;G#7xLRj:aWMr9T$/s$UH2L,tlIfLQC>B$"
        "1Z.%#-vv(#_%/%-^`Hn998WS&PNTc5Ol@A4[OAG5LH4B,&Of],1Q:D3DL,/(tOO2(osJn/:eZ)*x^5ClEim-$nZ)225rb[HF[svm$X)22$iB.35M>RKBg8hX>.OF31D=HZNIP(NHe5BU"
        "/$:B]S>:Q93&HRN3XV-+viuY-NNw4(d&Oa^WkJ0M3,s*@RgJvC]-&oclWDe$d231%%i@>%>L5+#est5/^vBB#33xg:X,+BGJJ1>l)agQqso5U8b?IQ0n;ZJ(_M<R:&U@q$f>re?:F;xt"
        "EAl;-1E)a-V8X.b/EdD-kY)a-;/]e?L6:e?&%91;1>Te?\?wpe?2R687mo),)1qHD*(>p;%4$]`]_%fe?J_<D<*XGcMTPqkL*='n/ga1-/&bs(an`VF@eNeV-o'JS'3.GuuiTbh(Ia9B#"
        ".uQ:vJ8*MBk5M5A,='9.]8<)#TQSW7e<G96A5`D#;I';Q@u@(6a:WD#[G(,^#_*D+^3+s@SvF)4:V1E%=[/9.k-Z@tY=VW-I_j&-'_W3&xTiBmX7MW-k^k&-'0],v6;raM$EM'#,Y(?#"
        "xF]?N+)=[$A95l3N%di9nkYs.6]^xtl:'j:Qa[v$fZ8]M]`XPAan;v#g/=B>9/9e?QH%qrskP,<x%ee?PB:e?m[AxQ<eHMp]IUe?J;r5#Eg13$1[r].+'$0^F'wD-amV&.].$*NYrZ1M"
        "Uh;e3;<3Z$f.S#/%XG+#M6c'A:uf'AG1VdAL$V-/T(Xfq.V/(A-5ED#glmt.7,$&=aTCI?E)>>#Jx?t-#.Rt7j=cB]S*dHPlH88#YtuN%6l5`-J=/QCvHXH?>w0#vR,oW$v9E<-PRqw-"
        "j'VxLDZc0Nc1sB#bx`V7axc?#LWG>#tdEA+f(+&+-8ka*/+&Q/q>V#A%/5##3]0H2+2YM^M,@D*>Rl22LPws?_2d0+HPr7[gi]'+MW,j'GcU0CkZKN*ahPnASKC,)7MYS.Q8rg(8OG_$"
        "r--@5'IKeEnoS,3l]+9i=+$u@:O3*sFiB.3x[t'4emwiL?20i)6o0N(Wd6#5]S%&41QR%`B`B3d`>V+Dp5-^+OVZ]bLLVS%.I..h$&Mu>?.%juo^o:#6:r$#?'kt$X?O&#UvX1)AO+_-"
        "$+o9.J$kE.f:+i(9c.R&g)E4'RYlb'g/^M']Wb=$Kd32(+N(#$a_`$#I^s5%[xM(M&L^t@Fa0.NCKhkL.'e()_6p>'&EsI3=t>Z-C-dkKLsq2UHTEPJslRa*xRET%GHDtL^9I&BAEIJh"
        "Nm29O,lb<7ILAbg(?AuuoD8=#F]/M%+e''#P,>>#'ecbu&##)%3+[5/^6E:.0haW-we;X1KJvul<Yr.rCtefV+LEs-oE:hLDu#_+_DP<-2E_w-E<vtNC_Q'vGTvD-UDe;9Skkxu6G_w-"
        "JI5lNBU6buKVvD-)T;a-jauF@#]7X1m15T7wkG,<7Q3B%2IpE@9Dc^?0c$]k-uM&5P_XF@PeE(a.J>s%$g3^#Ob=o&Fbcbu&^9b$5oV&.T/$b8qNO&#AGRI%L)>>#TfJE#j1p(Mo&.+#"
        "+Su>#D>$##*3bZ#8?J0YOnYs.7ak/4?ATD=RCS2C7b*r;:;Q`<:#l;-1u;M-sRSc&v9'T.,](g:]-'/('Rn,vP<&g:+#+^u&UvZ$2*L,b$$BJ1bw,eZw0EY7u3(lKN1*i*.-)?%Cd91_"
        ";h7K*7RUKa]lUh-+NpA#Okpi'?9</M>)9t@Gk-m02<kM(us/KVl9CDN7pg3=Vx0<%/fwf(e5Gm13SvY#0SVA#.kj+Ve`R34e5$D#e@?;-9[p%4r2=mL#P*nL+#b'4f&*#vBJSa@3jWN1"
        "+i?8%9&R7e-6pJ)5;e&,BeM390Fq#vJZn[#Ne0^#_1V.N<TZI%=ts6/4cDY1k#7ou`.oh$Wf''#/]Ok1-<d%H@VLK_+kZ4]<&an8p-_#v&:SluXaO6%TW4?-<b4?-k;hs.l^Be)4et?K"
        "d$.##*@uu#cEp*#it3[7Nmh0(>h;n8TcL0_@?k),88,R(iin-)2$*1)6(`v#*#`o(CR*X$*Q:Z[g%YC5e80*s;D=r7t[9T7]YOP(.Y[[-`0w9.7.5I)=Fn8%:ddC#INb]uQ_Z)^3i_;$"
        "SDite?:#;Z_qbbu.>pau)E%@/%k,Q#cD.@u/QX4'W98w^pdXrH%a/'?k>PX$X*u2vZ*Qm&Nxc&6TNQuYYZg`3AfVEnH9>:2]$I]OAc5N)^(jxFBwixFFC@[#Y'Ah=0[Ds@Bn'QM=Es:6"
        "5ZN,Mco;Z#A_K^-Z2vwNZPQ:voju:6UB(H)0/O^#SKJ=#(Lv%lP:[r9v.-Z$_C=KMIt*#5s=3`W1^w8'3`Y>#EJalAfIx+Mf-3WB_tpuZ2loo]A_rx$C0>)#729Q(w/<T'kWmS7.C[<A"
        "1]:v#27Hm0lSj_A6)f_AKbcF#k.o-#<Z`uY3g&6([kie$N14.%^^''#t,>>#/)RW%RGn=5OCm/*'-?ip?<%=)sNE@#ljEPAj7B/)40uQ#R<J/:f@)s@bs*O4(u9>,:Aq(aAj52'L9i/3"
        "EkAm&/YU;$=u$W$BeET%GBx?#Onsl&F4Vv#:v4$#O(.X$DbR<$'.hB#Pwkg(gf*87QqUEGeL'N(_80*sFxv@3@Q<I3xq,F3YZ)22laDQNlM+D(f$Xx2Qos9)IY5J*p06g).73Q/-t1A<"
        "1wcV6*uXT:4s-g(<q0AKlZ-GX,;B8Ke*?#vr7YY#%04c1-5W_7Q]ra<A@ns$TBBtU%:TQ&nxp%k?gQk$Zsxi'&HP,26(7W$$[t_0&2RS%C:B6&dXOgLP*A8%giIfLuY]b/@2`#Ax`pO("
        "xK;:V*svMNTO2K(IT7R4B=B4<L794<=,xY,9pwvI0ZJa#6$rAA5e*#HxEx8.7k5fqZ$xs-Wf]`=OSQvZ931g(wn>S1%2ZrHP%(,)?Dqo.c1JS@2i>S1r@8ouw_*X$3i`A.8%6cieS>W-"
        "C26.?+Wg8.FoE5AxS<^#=)?dX3u+.^5ldlX>F,+#jVxF/mvBB#OB#lT@SAG;D6_c)$vZiLRCv2vfVo)+8(V'Ad9<;$+)G6#FbY+#_->>#q0bSn7^fW7X`[@>c?TB#U?j+V'pSZ/&PMB#"
        "E^GT%.Rd#%7qW69@;b2(XBk63c#7oun9pm%7Xt&#Eg0e*tliD-a$I=-2w6d/1sPB#,Z0R/^YMO'a:11C6AF**fRTb*R4'QA42eBljtj>7ET6/(akB.3nbW#A459@IWdQL2p9n%&=Mf`*"
        "vIWT%T]R:/wZv)47.5I)m$Z]O68du-e1o/;7+So%9r_;$WBHq#SqU;$g=,p45GY:@vMtX?IRKZH[Om7K=r(d*0+j)E=dk,2Tk9'#nxV#$;3sj9,^W3(Xt?3L+;g+*T,';0iWcC-r@tE*"
        ":qP+#`=KDlnQMDl;-_^I3k/N-U/VKEnv4x#%k6^#*Ak9'la`cDAe.]8P]0Q+=,hs-ih=l*UrUW-=3@X&nbOP$Q'e$%K`G4SsM>52K>v1K1b[>Gi#F5/Ue$##$L[c;5@23)&wWM0PL6##"
        "u.-3)rBD+vFoD[$L<$9%ZVUO)w-HU7d:'N(h9c>-+#cKM:J1T7vkP,<Xsce?L6:e?.m97Be=Te?jLxe?Ce%##D^lm#GZr].;2a1^og@F'vd:t'M,U]=C4xE@]aw8'b:/]$<ATP8hZngL"
        "ed;U'6?2V9LkaY$YQ;hLd.ha>j(1^#e=u^%m@+i$`g[%#>6-^7/C]@#dpCG)9*YT/PRl+DF>%C#c]OgCjq?N<[5wH)FaeS%wCFi([TWA+dVqS.^/#(+?;r$#vLVL#R5cf*R@AQ/T/MA3"
        ")4gBl-YVs@rP(Z-`r#,cwJ]LClkFY^o1>c41]wv$P8oj0vkuX?6wdSBm0aWuRQ.@uJ4w(uT74rEGq'1XJ8c>/buMQ&'-4^.sJ]LCHbJA$m][8jWT5ITD.o<$Gfi$#B*&>GM]L>l=1%kk"
        "M-g(#]m)_7_SF875Jl#3<BdU&9/q&,b9Aq7gbAE#&tki0c9U]7@G#;7Iv$j'S/b87VD#g3Wj.1('nGe<BdZ)*gn*T.U:5Cl7G[tf`YU#AYlK0%jNx@3LTRL2B%NT/YsTW-EKR;^nAMG)"
        "dM=0&sb5l1q:Rv$hc_pL7fo`c9H,&L3.@^]eetL7QC5m*bvI_43Ng[Tjl/`S:P$/qdi2&7:XVH)4JD.G476A%4SknDC^/$(cEt7gp8QvjiM6w8XMK3U'8H90HUJGVq7C5/2PVh$8%-hF"
        ".uwZ$Y-u2v]1?;.Y)C-OTQT5#Aq8/+OD*,#*JY##,Sbpg/_1N(L#1K*1@?AO;A8ou^x[h$<eKs01a%>I@VLK_1E8i^Zp&XL9Xu:6;PE5AJ2j3=5HD>5CS3GMo@HuudN=4=4*nB#),i?#"
        "?TYn]vIxKYxu%*0A=<M-92s..1mSGN@CAG;nF[-?gTPQ#<Rx+/u3b.^A(-fZIcN1pkhMvG+]ZD4KB+W-h`xDc<:PS7k;kxF=pc)<=6-v?2RO;7&_n]>Qa[v$cX818<HJ88,Q>s@MU]x0"
        "/d*B#8$.BNSV$lLRwe>6J4/_P@7&,;iFdD-WND&.@mG(P1Y,eu_l&m$M,#0.k^Cu7-1ADtEf(.?F)>>#Iw'w-GTLpLJ_xf.OX$0#75_W.<XM>#e6'oLLLa..2OCANQvQlLFfw:7+?jJ;"
        "^s)M-TrDi-]w>lXBsr:6J;TM^.,da4*`$s$DO>48_djJiFWIw#EDg(%ODs:6UkqM9Bi1#?KW*<%gqo4A?$_Gs,QUj(b4IK)931g(r`pq;FC@G;p<YkX%B.kXd<TU%mt7GGq`lxuV/]5%"
        "?G081(hBcF:)5N^#hAW.jXVe$t)D9&A>Y$#Bt$(-'^>c(u4v^kh.kM(as8Q(H>wk$/ohV$=>@s?/*X`<pltY#GAcY#Uw$K#DkGN#YqH>#5:%@#NxQ8%FI#nA->uu#H:;Z#3?>##a3O=7"
        "&d<I3,WEJ:aF%J3uf_p(+gu@3q;-H5WmxGGATQJ(v%5t&ED4jLxL1M^-NAYuTVTP/bn&nu2ldC#KBSku:S=iBr]Wp*X6YT%IRi(Ed%YA#+NUY$2G#i7a_.0:.r/02^4j^#suCm9%C7x,"
        "j1#c*Wjh0(?JpT[_JCX$nB[t-p34#-[7EG<3=Qc*>Nx&OT=aI3OYjkL_rAT(9-nH3p[]o@;/mL:&PY)41:rkLcuFc4QIR<_bRkL:?V7bO4cK9<KD.[uh.5Yun_mL:Z3catFhW1<4thns"
        "n0`WBE7EW-peTB[J#:A=_qm-$pTPQ#pvBB#;2a1^>K^t'Owj0vw9IW$5+NU/'->>#qU74rp%ms-0JB=@&5+2_p1brpXnHlL%o)?#06]rM?3O+N&PQ(+K:l`Fvb,g;K2o>/#ECB#F=7YQ"
        "<J1T7)2T,<TgGkD,A1jCV)r?^jLxe?$aadM?3oiLrC7p.doTHM0_ucix'2d<;-+o2ro_Y$L/,##8E45i.dMa-A96(AtoR'AVaw8']3lBAM]_V$mwP:2=;`9.*ig4]PPsV._H]D4A:M-M"
        "fx0q@iv0B#rk%##QrxS/i8bJ-h)E>(CL6P8n^</_@LD*=>o###;BcQ#F.hKkM=Q:vprb'/Tq%g:7g/<%0Or._VGGQ'De)D+oj22%^`###M^OvG,7]^5+i?8%a&/29rTnm0OF-##E.bc<"
        "fXF&#NK.Z$U'<M-QEC[-R0Fk=QB@W&=^;hL1<<M2g21%oIJ$P&=C>'vVpu1%*1pE@b6^q%A<ET@]Q.TCE6&,;>$%'&@jc<.r#JgLiOUD.;#Lj0c'X>6Q:g&#HNi/8rrpj(QUQ>#%KNfM"
        "^_Kd%Cq%##_HqS-8d-x-Y8i.NhLihL9s6?&ev$q&)M?)4[(.s@lPg;-+v*J-m3_S-djLw-8u>lLZ_`A.H5520C3^W-FxH7V/K)a-i)>X1+J,<%APEoew-n;-6HvD-,xmEOIDAG;;+FF@"
        "5rU?^n3ikBm`F&#5t$B#1Av?^.19e?`uQx0`q?>#'1)##_HVv,[Uho.5@kp%^2@1(qs4x#'Gix=EdI%%9L7%#/9AV#jLe1As>[h2J'O<'WOgbG;ls]1)23(6Pvq$-KEf^5hWRS[WI]`?"
        "D3]C<EJk)3Zd3x#3S[8#.%=[7Vx.C#6EYA#O,+i(h&-7&Ov$1(cd@U7RQLg(W-93*P*S9.o:5gL1+p%#k>U29_$gG31gq92YgK?lj($u@ATQJ(k@C8.4c;9.oi4c4U@+K(O'juPpw%6<"
        "bp<D+43+$6x_X=?xXu']0/,m8BWE$^NvG2T#g?I2)4P]btt;1P*-25O';G##wfX;db@'ZY]L18.==gr6oOh;AUo320gShK20bv%57d:w,OTl3'`wA2'f?<jCs;v;%ZqveDYsvD*PI-1M"
        "Q3=&#@6>##ZDK?#aHe<&VMGJ&d$-7$Rj</:JP.;dJGS>,_BY/(WNsH$7<KQ$83u2vBXMu$4PR/(%9Wp'GWj22(JU2V4)k#%IGY/_09Rj2a]<B.#?QN'@(&w#YcwC#]s)1(W(#v(2xjlA"
        "xA?e2DUD8.)=mh2C%&?$^N8<$RmwJ3`=Y@5J#9;-+mhl/.AHT(=w1o0(:aLbM[,@5v.-u@.&%m/T+_Gld`9D5A$nY5T6wGG#6H12Q9W6p88JC#Ei(?#ndK#$%-=QBrN9_,Y3xf1_^Jw#"
        ")%ht6%[CT%bC]:/.m@d)BiaF34Wl`<;VLxtNc@.hw&:`sAT8#6-0&(>%gLTRfCe5&g;VI2)2io%Bt3L7NV#K3'ZG=7lZ/tA]K0B#'#<H$j-TW9Pk8D,o8i*ugTH<(4(8),5jIl1d^AI$"
        "E)8v7PJb**YO*0*4gf@6W`:%6hZWV$oCN1pu&%ZYbZXc2c1ix=vsf.U`/m4M)D(s7-F)p79Is9;YQJnLA_jI+$uNo9KXTB7c(T5:tJv2v&fA8'`)E3(SdHt/s;-;BS,,i<d3/$GMEP3'"
        "'1Q5A1a#h<MIgT9?M9r0?5h],6Ule<lfiO'Mxl>#`2]t@1B:D&_x^t@)Z)2229X2(f=;h$ZL=j0%/fO(I?%12H8Q?lr?MAlr[lo79[bA#s'gD0rxfb*5CW<-EoUE%8)TF40w)B?qir@."
        "jeQx.EiXh/`6JF.nc4.3Hs(Z-iP]L2N*)ZHZuH=7Z.@=7;]e,FPSrn@6q+*$-[+T%Rq8D,Pw`L3W?4T.ZLRA6aUaJ1x%>uu*Q(Z>q7C5/Gjt&Zq6%p.1r[20q)_^#/7*t]wYV)-_%B)v"
        "a^1$%l>:@-LRU[-2jnd=]Oxo@$QB>G=(vx++Vvs-sm[mGDhOPTN*)^#>Q>m/:T]5/>U6(5d^:T&p3[1^6Z-F@MMFw%+fS^$YxYlLpMgn1onCk`cL,euAZ*;&1ED>5SZ=W-<:l?nM=Q:v"
        "%=`'/0=HgLp=o?;7G:AldKUl%CvJ>#[h,7$:SO&#GEAW7Ute<$EG&i%aQ[0#Ll^^%]9x+2E;lUeJ-=F+@@O22@BRL2>u29/^Cn8%hTU&$Y82#$Qj$*J-7iF4T.6N+K8G5(sm[V8`2Yci"
        "k0Ic444/A,45O%.4[qO;:D:SEo9Y/(Tcxe&)5e<(HS''%qo4RE_N/SEoIR8%IweP/c_C+*3S>c4cZE6+(>?t0.p2Q3e'$fE1AVs$(O;d<<^#oL@nbu-V7s7BSS]F-v,t?3G&l.#&:ZU7"
        ".&###$kWPAX(ffL^F4fK'a,[-v+b]+$US@b[>NvLl8xiLUjbd$'O7%#oK`f)R0G>#N&;1Mb&cB&g]0F&l+e<(&wjPA^==,Mof0re'S=c/YB/1Ah9JC#Wh0T%x1%`,uq-x604SPJdRx-F"
        "k&tU:i1hF$4,4613O=x-XKlp6#]kT/=YKp0)@Xp%p@cjLhw^h$Ik)d$ltC$#B8;U%?VRVQS5Vg&QaXG2hUXgLH,v`m88/[B`goO9gYHJVEkHx%]]EG6X#`l0B%v(5&8P>#49Fi%iK`DN"
        "Uuou,&S4Q1u_Z$#]UG+#1<G##P+B:&'j+%Pp=+m'%a5r'PNJ&(?_[s$fIY4OUEWKA=&3,2gvh^M:/HY+g]UU8jua>-?rk%(?nAd8o,J?ph(1P8-?&##M&%)*H>J/LrM2h(jb-$#;*SkL"
        "e+CG+iUKI*^^D.3V)2u$?]d8/xH:a#H>(m]kc%M1SO*E,9[6/2-g+kOp34[$SFIT%[-H0;0<JeMp9H&%*bqm$xsn%##@Qm'-N,]#d%1<->OWp%1fa]'$Rx0:.s:Q&ENR-)M2(hLfk#,2"
        "s+<]35GABl:?x9A1$Ym2s%J)=.2T;.&s=,M'4sR'O:m10%'`B-La]HDtd9l:o:pO;6+Lk1,B1@'+Y:DjuWZ)4ECL<9drZ<.3bA8B9A$##_3)S$p431%)`kgLMT6X7m2vN'frR+#^V5D<"
        "NSq/)V*?k+paTd%7fB4p86]t@LK(T5,jWL2cB8<-,qTd%#IG.2Ta2wcg9).=nx9DEM:&@'Fb=@'U%$V7>P:KEBkC,bFP4`sOFc>$+w2U/9kY/(86;_$v,=$#Rl,XLhYmD+6Bwa.qaj>3"
        "UJrm&Qk[O&_6x<$?@nM15rhF$*.(.=m4qb*?SFD-;<LG)l0$&OBA:kk3:QfCsm*JMD;hV.9d$^4>?iS.lh70:9PF**oRu2vT#(t%?%xN'rx1s&)fE&'F](XQeq*A#gxd8+u`,j'w)>]#"
        "<94D<=tdYm@.MkL7QqkL;oH12ouAhLmavGG=1R<-PJrQ&$=jJ'*H8'+)<nPf0.I]6dDm;5Gq%E+h*060*Dvs*Jb,*3dd4L<x*A=-7MKx.FTvq&@hQc*Mr)q.chW4o8X0bZbY[FuWc53#"
        "d@TA+%)vw-r8U4ov'r>$uYOa*&+crLS:-)MK_j_$Xf0'#0-92#,Y(?#=[(*M+00k1BTew#D^u2vXL`j'm4AfUAa[h(L^t&#fhjFaju4_+5KT#AhfvC#j[%#ovt]+M8$(a<8,o8/Scj2O"
        "F78FOt-;hL-[=g7`Qkt$rv_90ju7A#)b5/(qZ/I$`bW#A&.it:_4`'S,p?QM]?`'#c(h,.da/.MnF8%)-'=@#1,>;-X)Ih(7C7@#mN-f7m<TD6;0Hlf:WL7#*=D>#J9alAu^P+#fB@>#"
        "d),##ZL?x/eC`/:9aYp%u9Gk-7P/bIUv=)4'asS@cC$$vR;op$e_n8##7LF=Q[FT.2of_#K57GVVv>t1o>:C#n+?;-;'Ch1SA6C#Xt,:&SC-##=(EY0,xuq4lO3D#Cu0hL1`U#ArDW_A"
        "sua-mx,(PMgTbD#3s9D22tYM'DV:0_[&O<*2Jo$(4mZp.82f0:Hw@k7e?k9%f[7`a4$Cj$Tup>#Nf_V$Uq+,2dY8b*rPvdd+k8*4W[Zs0x)X,48YKv$=J@2.))@/OFT*Y7*,W%$&(+D#"
        "kw-##Q>SC4Otjq.<xHA#BxNB,5@-E#]w587Qe^9`_u5?m4098I[QE5/::m?^(&K1g;QZ`*xuuY#`I7B#Sc>;-Vf(+E_>Y)40me+M<QTW$q%LS.Gf>D%7HXM^P.ho.Zpx;6A)78#?)nk$"
        "4CP##vdU#cplbY#,=cY#GqN%bH^r,)sc68%p=8=%El)D%*T8Q(6='s?>2>Z#8a:v#-$bR%LqAT(ffpTA?mSL2lntM(ouq(aOXOk$m%;IquJR&#3Sfb-xN_wBej(;?(Di;$<OaP&PAmxF"
        "]_%`Ffnv5/MR%0:e/_-2h9/_FcLJ-V_P_t$-ZPp%($Ad#eYMK(#jCv#g;`fL4u,P7]T,,2xWC).hov0:8^Og1__G)4es]s$TukV-wI@?$FCUR#g+PT(l4du#r$*j1SCke$k=R*Y3phZ#"
        "`7_+8_*%ktOMY##3mdw'Fq%##6Z]M^h:nY6cL)<-X92X-@u0O+inIs-g.hxFiA_s-@uS(#3RSM'bG=,DO[4C#L9t.Li9)Q1/<o`*uCr91i`M3Km]m?$H)8>,l.@=$lQZa*e*>Q/m>BV#"
        "jLe1A.lf*%(a9D586RL2&0fX-$BJa$=FFd?%7$c*O>Jk0)k>)*?YjcMInf,U7a`I,'5GiVQegOoJ,.6-[*nGM;_)eNiWDXMX1CkLp$TfLeK<;$k(G6#$NQ^7mxajpB@Rw#CeYe)rEw8'"
        "mb%T%rLr/:D:.W$DEu2v+ek8&M&5D<x5c+VM/*Pfp4?8%4),##Xcb;-3-;_$;i($#Qd[/3murB#EGV#AE&9J3Qp<I3-.>n0XY[[-6[3hW5w;8.<je]O9hA7-F35O047*Ucva#4$i;o=;"
        "p,/,ueZG`/@1SucXEX,2J;gJ23B3Z$:.m<-[<:H/#hc+#Bal)M)l?##5f1$#of$N)@'4A#=Mdc%YI*t$2]1?#,(487Awx2'Q_x='daa?#W?J]&UrED<=..w#)xfQaZYl##<xJQAP4'QA"
        "SJ2MKXcqKE=Z4N3sS(9(tnxGG$r#@5q7&@'i/41)(EL#$;KaD#]Z&ue$>e7@k(9*uMwX.u&iw0W&SQntTGS4#k+Rp.S&5s$,ofKY<W)ed7T'%MQ=Z;%^HFa,9p7JC<w@?#DPk*&D8G/("
        "6m-s?BGlv#;vqV$=LV58IS'3(^RXL2Y'M`$5rw%biEQs_8-Cg(-ji38o)$?$?kn#MacQLM$IgnLO5DhL+S_V&v*$9'W&o;-C-?b#2Vu>#QqN%bO#Ad)7I53'&117'<vEA'=[Wp%]Q?C#"
        "g;P>#81*p%;vFv#9+OwKSP2W8sk0B#PZ4N3T1j5/(a9D5/M(kLg8,H3:b#&4&3lA#Sr-+<068]FOZ,V?fU#GDc`uj#D;j;R^Y2Wu9sb;-$WeuLr,o(#uRj/%YkqE[]n_E[4HA##(:op$"
        "YR[8#s8q'#vq1b7T:),DKfNt?,w2w(j0<@#nXlf()H<<@TtP22INIV63gqD#c0mk6IOoD#F?HG2XS'X62t/s$5uHiCsRrrA2g_h1+u3^#0w3L#nt%%#P.i>7=i?N(%gu@3&i0B#kU*MA"
        ");-W->CG_&jW)22d`PH3slu23c7JC#YKI&4_-f<$LHF:.wO=j1$.@x6IGO1)#f.HH(cR$$kmqc)@:t9)>LZl]rZ3Ob*A1c3J56[CM[<OCpjq.4eY?NbtW:U+@8Hp1w/?-?Sop%?M_vj1"
        ",::K+]o9HA%ZFb66lsL=GwE`26;=WL%`cFTc(f(6sc>2<:=TA6ifHC?Hh(v7nCc/CA:(n#e1<)#$),##;QUV$D)G6#N`A`7O@n$&NZP,,Pd0DN^a4B&K).-)_/o+DLPMB#rMIlfxQ,d/"
        "Gi/;Q'pSZ/'SMB#J`dS/GFB:@7Uw#-^.EQ&#t*B,UZ_g(*+$K3T#r-%jvuk0q'#i2wsUO')O1N(I_Gw-F_)P(D4`[,d3rkL],E.3a+)X-'Nv&$Bf_[MQj^/`qojsafD+pK,1&o1$Ua(a"
        ")c)CIo=Oul)CBGPmV&s?\?^Y.]hOqQjE2cQjvl?i$v@$(#?M.V']IZ;@N#MH2?OsP&cu'<7=m8mp'lvp.[tvp.<5dk0i%d,*3XwA,iZk#3&qXC#kM587LWq_#`Vi4'?bwW$=<xK2ILn=$"
        "-<63'6ucgLBe4Cld5/(>Hn]G3=c`20e,+u@3`9D5kUow'N%]x*>G[N0P_G)4UJ7&4Zoo`*:_+<-O@;=-SbOU%XxmY#Jq@:EMmG4M2P4Q#0WCl<05B'p;+1aVbuXIFqg.Hu^7l3=2[XkW"
        "msEQLt(A6`;Rn>R(*#1Y;HmVr97rDZ;<B+d(UKeW;.]/ZtRkr_>J@uu)krR8v-9)$FPIAGP'eVQs?@5/MJ'##*3px=O$(,)fCif:si7)*@uA.2skRT.n'89$O-l?-w0l?-r*a60L`u6/"
        "bcYY7tqIx0]8Rc)?#-W-YYuw08Csw0l(Vw0FrtaQOqj0vdNgSM]SLp$hfjOf;xd;-pcm9/9&N1^m:*(=3Tm928+G#%.^AN-q5)Q23T###Bb%Z#wf2ipKfZ^$n#tb.:+[Y#803l%Yg%-#"
        "q=N)#G?*&5h2S32/'j97;r_0*>4Hm'cq/f+qv(41c7pO(fIe;8;o]G3(w0T.>$w@3IGo*-?L_F*OS>c4't$d)D`M6&Jq[P/p8w`*$XLU%N?c4WeA&s2t?M)+3.24'8[,Q'K7?q72,^/3"
        "vV<**:F'6Ma8GWDK%rTE)>NGgQo5L4dswF>.VmY-f&&k4Dw_J3gJ((&V,TW76E3m1mDo&%7mSv,O5:-m^Is]73Z<_$-fSG3fAMD3vC8d)OU'w>G^&N2F`>g)i<c%78jh)3b8eC#<FJ(#"
        "#lvR(PZQ+#E=KDl2>lO(G2`#Ak+K0uKK7a3nl^t@a#L?lc)))3m]WI)[(9+*7qNe$YqUN(ul>x6m_+Q'GPZ'='Gl)4ULD8.oIRA0F`Oe#kI(.I?1Ic[B*#dZ'C]L6AkN/Yo$W6;f@LI="
        "Y`teFt%;rAU3Kn;a[1x.He)'%kA]^R5vlV*ocn%`,<7i4J_=4o6FqA].mq/-L$[,uP#qf)iXXeF(;)p;RuVN:eUa;AP3jOB>IHa37gv^7SJTm(C;iF,*S*/2Z1*#$JCm>#/L@/_j;Xj)"
        ":x1$#p:TB+$*&a+4%oS%wl%5'[R5]5`PVo&PkNP&X*o%#^8bI3qm,,2K)p'5]d)l:UxCI3/LcClDfW#A*(tt.m<E:.Mv'a4U_Tc&p9llA*2Eo&1D^r.UjWm`CEMD+F+7W$wuDS&MPet6"
        ":3*n33sXLPMA<DuiRY&@PMsC=Sc3^#M$l^&AC/'-Dw_l88:Pm1gFox49k1I3K=U#?l:jsf>5O'$*3's$-sA_>$Ro88c8k(5>@Y:0Y+Z7(n`*87Y;SDlY8fk(0*AJ3a?Ss-5/51MxC:D5"
        "h9JC#I_ns-2'Se*%,IW-Ur1xPl)1B#Yq[P/0lT[%Y.]G)wuwv.%uvs:5D-0=OXaM:-<nR0_OQ_+_NaE>'(:79*uQ7D1ZxW8:USl]WH].43&kw8?-V[8N]UYYG.JVBTERX9J&dS/WT=7/"
        ":39r.)0bv6QE%h2O4hJ?+6G_8Dk9SFT5Rj=(RgdFJLH2(dZA;DSH&S1.+,##n'89$7h%-#K*q]7FW(B#$d(L1<]q06at>46UKWVAVJOw5mMZ?.Uc6h*)Em]#nqKD#CeiL5L,<;&2BE3("
        "QN)+*])mw$Bb`Gle80*sI$gBlKU1TC84Hk(j7JC#.;Rv$m&l]#%r%p%ZD.&4$2f]4)hQU&lL<GY./[W%MgfZLM'dQ9J>hX@lv/#6GK&MX*91<7i%sQ9l6^iDDADx?*8D<7N')hEk*U6C"
        ".l3@TU9L79189b*kLr-.I6N`H;#3&7ugYN'e0N13G.(=/peBJ)L)Xg;Z9;W/crun%.cQxFU<(H):l3>5_]n$$PheH+mpLG)_]oeaWF%0:v@)20JMAi1xdlF#S*$P.c4AC#x-702-8-9("
        "bYjkL+rxqJb$xP'RBG?.#Cr8.2Ph;-WXHk%^3xF4n@[s$Rd5m#<smaK'-dK1-+_q9@(Df)1,T[YG'%K)`<;m;92Qp1P.uuE[]ppAm+r>TS]#g]8]4S2L)&?.W&b$6q#o?\?U8Q>-jBGE,"
        "H#P#-GaLgEc40N#S$_wE/]L;$*3:d%mn^`*]?+E,Fn[CF+/1R'Ft91(_Pnd)(Y+87kb<t-H>f`*`I=30YNU&$_R(f)lfU@Gg4wBRc>Mc)qdp3NCLH.:nj8f$hoeD*hP),)i3IP/NpAa5"
        "SqT*#1n^30uBfB#?((C0::aB#1QCW-YYGl9qXUE+el7B#7ph%O]2Q^#`3]S%Iw%x,BbLG)Z_Ym:^]_#AS4:Al;u%i$AWIAG'pR>%_FL,D>-?mSc08@#x1f]4#<D,)]CI8%LYL;$=c']]"
        "<n`MHZG=%]>'/jH&gvN:L8a#$fmiV-bITq9KGAZ$;H)HMCZ(nuOUK1F.p>K`.h(9fs6rouQoH$;l&nGR>/[_$%n$(fkA.@#7,>>#`mP1p>.H/:nAq%4kU#u%MD<E*bC'F*''UN*RI`S*"
        "8xPZ*&oci9PuZ=$C+A%#ME2M%j`CT(mdOj3K^E78?_<2*RS+P([:a5:[CwW.=I^K2nKG/E>LE/:65`WB[tg('ZGcuu'@S5#qMoT$vMY##,v3kOL389v1HBu$s8q'#nl@w-$MOi(jxR9&"
        "4`0H29f-A#%MRH+OPn1(%<cx#=McqAl`i_#jE&s$`'u2vdAX$#;d_B#DVvb<Yd9a<`w[f$lW)22n1D+E3rv5/Mg_p(W>O22Y:+F3^r+@5tARL2G]jj1r:Rv$wF#G4S,m]#)ChhL6&P,%"
        "jvT_#)^m$$$=Ss$<4i?#=,<w,.iIw-qQj78IGGO<vh)/:BhPQ'Jcx_4/Q<22?8r_&w&uX$5MQ%-6cJLV1`mb+s1`G%8+SMN?L*G4ASbcMwsSn/5of(W[F.*?..BV<^8aQsmh@B&Cgv;/"
        "^VWa*0%Bk0Jh3e+veD`+B0k`<s<F$'$WqQCcA<.3jj#Xl>?RL2QOx[>8PCZ5&>uu#AEp*#^u_Y7hg9:%)I%k9Y?L1(Vig;*J3]@#8Q]._vEO6#aSD4'r4+87ttfC#qL?:A'*RrLOMwq$"
        "t]RL2C?W[,Yx5x6JgrI3Al-s$)l1kd0n2)+P[ucGZ^6-/P+flav10o[lQB_@RDTMne.+TWMq>(s:cbl8$@[R909d/<2)cP(+(9T9/V<X.m77*d.HR[%2H=P9gKPJ(RE#XA#ttppsq[0:"
        "B01[./)C/3e>NS07#EY?s$xx#*i@8%'Q@)*tg%##3A&)*G1i>7*4gBl7`cu@0@35(fr2+=p`B3(`aw#bu`CT(s4HH3bMNP(SUJ79eJg*YoaN7%<Dk5A,B(a4eUn20<=Rs$kL3]-,U,Oi"
        "w9M8.C+?-?)QoK)iK^A_>iIh4xV&I<lc*T/[k`w6'44P::NjB-0+Z($U@DC>/$vtAQiUH4?M;x-n@gqK<m[n&.[M;:m)>1)/]L;$jvw,<n,O+=w%?uu?(C+vEf)@$4NY##v####kQ-iL"
        ")5'Zd41^`*ITa)5pu<8*_iiO'N-/%#;xuY#srd.2<i'lp^Wl3'%V#h%r^41:_vDf)L.;o(Uw7%#;i@%#n,q>7wGiD3lW=]3I4H12ItIO(P4_t@6*tt.e)VT%MctM(Z_Y)4_BF:.c^X=$"
        "#rhn'tx2CGc$fX^a<rP/`3s?'+QHK(@ql(a$cKS[VR/NuDCp(#uI<`WrE@w^mke%#oiLY7]E_*#GI2H09;>)%5?dj'Ldau/`*L1(kk/d<ElG*R8[nERM80*sgUI8Ap:)W-I2^5`AxMT/"
        "K8Id;$u9B#dN1K()AK3%LMhJ1=x%YD:Pbx$)>P>#=baJY_UCII*`*,Oo+B%QIr&?KVZRXh[[2O1Z],6B2(vR$$-imQu;gxguq_Od3s.i2k0Egfu?9T%<3I5AP>l%==iWT/9Wb$#h[:<-"
        "n5?E3(tFN3N.g^3l:eC#6md]>Q3Z*=8]ml/#,61:ZZG7&Un4j'b.?Bt_ULm(.JV#A10b(&8+<]3cE0v*5JVj9xOWv6K]B.*0fVp.b=;M:.PpaP@P?OM4$U3M%+owR0L?%5YC=c<X5<^#"
        "Q6j5<=dQEPda@q7lq'[T])_$.I23-):6h(G&1-t$28LI$.S@%#nV1Y7'8M2:Ggcu>pnN?QvBfW[`N:[$8COZ>*cJd<Ns=P9n]?hc*:4C3=;lK39n6g)khrV$gT5T%bU^:/H.,Q'?pSw6"
        "3$^PE7Gmj;YK(L#(A04/YN^@uOl[HFrs<u9pAl'6>mBs0Mx1la'HNHOAo59jZIPB0J?MN4Xg)+6<657<gUW81(a(##6=7wuJ.@qLton&,mD?Z.`b:w,Enf2'rC;[,OWVx5]6bx#t$#%l"
        "Ww3]#cEhl'qT<R%]j,,21ln.A+iOMAZJ.JXa_-m%aPDT%;d4c4nFg=Pwg,Z.V.eW.=+kj1)p/^H2W^Fi=a#UTQYNG=w[7s$g>>+%^oo>,b>ge#`2@4M*sj0v):vlLW(=B,OOMg*BZZx>"
        "'troA:'>#$we@#>ldGj'A1')*.j-P7_80*s=km-$Y[b[$rA(&=Y87W?J29f3X7GH3j;@1(Z8uSLhdIQ^2w6X9]@-0(ex4'(H#]Gr'f7r`>.)GurJQxB,6+.:sjnc6w.oVL.U_nuN15##"
        "WujX$Ug%-#(dZ(#/a'j2<w47SeWUn*1Jd5/o*kf45Wqi't`Hu4J;FoK4lo%$1vJw5,b/s$-(+Z>5A(Y&?B8.*Y^.)*(Y+87T%w@3Xb*A%6/-T%gMBg2[3b-;8@(a4Q.NY$MPEb3^aTS%"
        "o5e`*@/IT%_YwY#5%=<NkBm.<B<+<7JjN@?e>nhQX6Lh41TM[DsYID49YOf2xqqWJnpMc)G2xP8n&]m=PTsj4<9KeGFV[=TiFEZ#Ph2u<8SW&S2pQ_$rT7p.^h+O3^r%#8YIXv5.Z(##"
        "fZ;sue26?$Dk9'#10vID1[x]#*qt8IU##w#pOO/,#&?A#+mH;%OqN%bE2'+*SO3?#KwET%_Ylf(S^=W6@X.0:+4:n'[T_H<Kbk?$k48da?Wcs.m,v2v<]p],NHj`+nV)w$,)do/g#mr%"
        "LMG##_8x_+WL8d<X.UH41U:F*VR'B7BXG12FEI12MaCT(dS9t@KNuu@x--@5J,V#Apg4N3J041)7ZRD*lcdC#b@XPAX@K&X(0-$$KX)v#`GUv-wF#G4;s<b5;RIK#hJl:H'V58/p%Ji1"
        "U'lr/@hnK3tp3i^N%0]V4*1Y.BUpA$+8vY@`+]l#cEStu4G>K3+@;>PKMCQ96Xs(E;J/s66do_?#Xx5&$),##5vjX$WMr$#,1O[7]3+l9Bb.],.tTw>;DhW7b[RW[;7'[$NxD50>)1m9"
        "tvhT&#Y65/do^+*Uu###e-ej)x'h[%s5*@5T&Qt$8Rn;-=^7^$.1w9.RkC58XZIg:OscY>3oej3cr[1(hw[1(0962cp+nR`;=]T4>S#$%Wt7&&%gP<DB@:9Mb23[KalspjPoeYu)oPR$"
        "_;#eTrx4(jk4*q1?)auLCD6DGVZJ5SWnku5r-k5:6iIA#x?'X+LG1A#-@6MT[#WA+ZGq2:AjiA$_70a#WqNq6$,xoT5);^6.pEA%ii^0:+0'd+,gmj'7IaF5oR@%#S1,ClVoYC5*YXR-"
        "GI;2%]AqSA%6pr-4JLV82<Gd3#C.J3-ZKs-`9#W8)H.kV3_cL%u:WO';]d8/$=Ss$JJ@,P9.Nw,M^(O-H7m?,UaMk1]x-7]P$d0;gb>R*D?DR*Z6wqAWNfhOL;Y`O=d@GeZ=@SNnsQ0N"
        "C7(VB'X[70^WHD#O86aL4qX?SC:7SNn#eKNZY;xI+6R(?1D>,#8<hc)4098Ir4v5)%?j(EZ&+##2aw8'V%bM()I')3xp/%#T_L*44Us-$J^0dWQ/wV6<rXvQ*`$s$?sp92ov(n#RO[t1"
        "5+*]k&Gib=gu%r2OmHn/0N###.9kZ#$Th[-:phI;1[c'AC?wiLbni&.c+kHMKqNM0#MB>,D-Ue?fPi^d;GC5S4`98SZ0.<%<Z5]bwwFZ-a%)/:d2dqAUg)5_->fv%ht._6,gmG2co=m0"
        "LB7-FmC88#A;NL%NZ9&0@jA>#$eK1p?mSM^#FGcMqpb;$VCDs'F=>o-hkcV-+U'R/aN#_/;rTIrJpaq*c=5;Hm:5c*MP(%#iv[U7_]Mbahose)O^u+Dne`[#*H487EA_[#Jd1,)su+D+"
        "cHQRAbAeh(GC+,)P.wP/%[)22=8]t@T&5H3L;M?laR[Q/wV(9(v1lIlpugg2KWc8/-HF78nLu)4m>:Z-Sp;iOVVO03nfse)Ju1v#P;J-+wRD?-w:%gDk(_?`.,ko[1wkr?Xtb04PjMU)"
        ":[t*&xM3%?+0>GH[P;g(0SSxFBV8f?l?^ZC]sv4A4S2<%knBB#a*[l$3GvkCn4_B#bvBB#xLR<$=*j(?)X5F#8LO1p;9?8eSF=PAs[6U7H^r?%h=G_+-5kxFA6G^#kqN%bCkt5&/vMp/"
        "g&aV$^tHiClUrs'4e7Z#Bq]2'r,@E-Nn79(tvI1Ci*1B#>s'A3JDc;-V(0Z$Y7JC#]CI8%kDP59a14Q'sDwV@jGfI*$A%$$`ECeIIq4;HBrxS7$Wfl*gcS40SK-pFGKNVO?('oSp;[c?"
        "x8;8ebs5A4j4-#of&4P2<(gaun`mc$'22].P####ZJ]]%^?l]k_pJM0;jw=P.r<w?LPP/_Z/Ed=Id7[%X8qj,qB;[,2_R;Qh7)#luKBK.XvgN.Du7d<EQiJVW:[s$/rGg)k/o+Dc?:D#"
        "TZPp%wuJC4AAHq.W>fS%K40X%m,=&+)Rck0FMr$#`sup.J,CD3t7T>g/M0A?U.J9AR0=(5H'b+1ML2nU/OuM(]IR8%X*Jx&r1,G4`cJ_4fND.3]^d`*s)DT%T>e]k'EL=NN?']b?&Cv-"
        "wjlI<M+Bj:[=EA-+sgE+-jlS/ON+8RxD?dKG#dDIUbx<@fYcG#@NW3G>Hu?G+wQ<=-ma+69[TB'JSgaH]:^S#e0E`Yn(/EG2-uWM:aNE6@/5##Ge68%@'G6#Z?O&#OWfO(SiHt$&'@iL"
        ")MD=.fppi'^&8mKns,?$Hcg>&(E*W%Z'd3'?KeS%W^,R&R[H?&%>+87Uk)K3G2`#AP.U`$/#vA@RIDBQn.WO'j3C9r8o0N(aawiLeWqo-K#qHmf_'$`8qxm_[D+_#'Vn`$stuj#JV,OH"
        "Q(-Z$rieh%Qt]uGikTs?P6:l'@%#lK8EIGrBKIX)_15;HaSNI)EIDs'vgQO(+)vR(afuJ(JEx?#vQLv'>Mu?#%a5r'51r?#c$-RAL2/-#?knw#>46th(R-@5ro8hL6_`q8crA,31]D8A"
        "$7Ap.@oq_,At)qint`W-GcB6(GMQ*9&T?$$k.CgOo38X#(oi:%KuvP/DGLvGVMOl]E;l;-Yw>t$64[5/cfVK(UoFgL:0#xuu5hJ%]3(@-`R$=.)X28.t#nS/XJp5'tdan8C+<Q/nGsH$"
        "K)G6#'gv^7YlPfh.dqr?pN^6$':`>#lN(v#E?f0:S?VY552Wm/9A[s?F0RT[X9x='77jD'DU`?#D/J/CB+Vv#<s4Z#+DP##=EI12csU=%qr/al:o^t@gvU#AFdYm2Q=uM(ej8s7@=j@$"
        "E00#]<_On#8)^.Lb5XM^$Ub/((vc68ptg>$-lfe%*HW]+2(Pm1>5[S%Wb9:@b;rl'<oXlK1iI>$J.Ot?pCY`+Lnw8[ChAQ&SCBQA,F2R8*TBj(Osas^0]>:Avv`w'R=gA#Pdo;-.L.r'"
        "u,T@6?<Chu$Bi?ut%<af%.pC<@V1]$<jfg3RMbsWim+*,oJ?.%HjSa*0[0016p@-#%/5##`N8e$@s:P-/DXi%nXDW&DZoa+);<^4Zuh0_hNHW&U%)O)L=k9%@8G/_ed%6(0Sub'Awa9%"
        "MxWt?9EV,#2G#i7a^41:CkqKC)<GA#Xt[S%6)1W$lqVW'3p3]%_(tPA0o-W$x=+87tvR4(C>@128wIc(iwfBlLT(9(?0jl,>6kx@'W(9(IsAT(klSL2$t@X-Fl*F3jSD]-DtuM(_Ue8%"
        "5O/5u.6-828G:N2VxFJCh5*hup7wi1q(^X/$;<JLvh_P/WjT3nT#%Lt$bQR;B11Qd%-%tT[L?>#'/###%o9qr1t+j7WeXsHj_vv#_<Iw%:M3p%.Wa9'7$B2'aRXoIZ1$v#N+^*#)6>##"
        "5KD`'0o#2Trxd['FxB-DeOMw#Zd#7&E$BQ&I<jxF#V@aa2w^2su?c>%:u30(;/S0q`RH@)NLmC)0-F@#]QOS7Y,[-)`[eS%7h4/(3#jIq?Q7L(Ac:v#PgPN'AcC;$CmwJ3SJG8.^X+F3"
        "@jk;-+;J#%Cq,F3io9t@(5rkLVR,@52Mq0#;LSt@Gmu23x7V#A1oH>#h+=O4QIH>#&uv9.ZaB^#WJe2f:6cm#<;[Lpnq4Tu^MvhpCGJt#TQRJNW^Ki'$n_x+Uua)<$),##['89$6S[8#"
        "Z/N_7:ja,#$9>G2fMOV-0Gu$,,th*6p'lxFw*a[#-;M4)VqC,)@hd;-PKqA#xkpi'o+CW-dn'41+jF&+DCAk9e7iNX&@?$g5AU2(k@mD%S?hiso.WO':qTIVgrGw-o=7`,m(TF4d2+<@"
        "ZG^;.FXv[/trP/$VOU;KYibCADbq'f1NYR>vp1tB/`mcfUx3g@kq1u.p.=GFE5v>/pq[w97s<J5I;+[IrN5t$8sJAI6%,LH^F%_Y.Aa4'H%VXL:4qQC$tBc58IZ7KeA'e*@l61#C@+Vd"
        "8'Y/(KoED*?2Ld+Ur3t?*E&of[(Z8+8xwI2^T5r%PFC1CjuY/_)&TZ)cL.F%C2*?.9EV$%rH*6/E-,3'93%iL:w@=-LDPu%8Z540Ua-H)15>##%;?:Al8_<7R;^;-]S-Nudt1*sEPwh2"
        "U*nO(NCI8%eudC#2$uq%+O;8.IKiY#/f_;$$*_`h3t_;$`BX(3_gf5uJHxV8sG/O;wp@+;]vB`POd;etj)P:v=)9<%anBB#5P/(%2X=A=CTPQ#mvBB#'eT/^j-kl&BIWT.DI:;$gKZa2"
        "wGH^7_njp%=D,##5fYY#db%T%^NET[lXkq&AOV?#rRPY>F8c/_qJ=N)4Tn<&9#%s?H(I9%67IW$)P]<9;d3l(F'Athae0o05l]c2$)p'5E>cI3&F)i2Cd(B#;wDjV[E,)Wsm9$`:g[Uf"
        "*Z>VmMYTM^$*cxbA6%b[H&>uu'A/vLLO>&#B1ov#WR)?5'wF1:#PX4+g[^'+-8Ol]NQo1hc;SY,0=7h,jQe],9EV,#/c.X-YZ*e-N,i0(o#`d,V<]T(^kiBShke8.:+$:A2Wt;-nA2*/"
        "#G_Gl%[=?oUI%?*D9#v_UR'n_wK1E:G0T^H?Njq'[VFI#Q^`&(R^'$d>cv%b<64s$o+f(N<,?%&XmI?$9e<p#)^/KMIHt/'@k9T%BjYM^_sE]F1u7@#8hRX-Ist.#pxV<#5'u2v:(Zt$"
        "08./(U(Om'@3G^#b1`0_a?ds&SS<;nQhN]#/fEA+R^Fl7>wI0:S3bx#D7?\?&O$&%#Bd[/33PIc(Qr9t@`Ob,%#ce20^KNP(@BRL2(dAU'63)G##L0#$YNU&$deqrZEMRa=(h1P3wB@Z?"
        "w]i=8e-qhpvS/e$CC7<$>'^Q&j#Z]4+`T[t>00<:eb-[)4^*`>M>*)u7klC+l7lv,lk`v#U.UwI_7mf_tLtA#>HGS.'G:;$ebaT.oP8e$Iu4D3Tu_Y7N^7d#3^V,#D9f?#)3)/:mPh'#"
        "<s4Z#<Ir$#*k.4(<)O_2ci#XlKT(9(<UO126*tt.`M`Q&VE55ulj_cM^h9(jKVq8.vXs@Xl7VMLIVk)3ln*[7Y;ir?f)5%GIN0]#%U5/(MM*t?^@/A&3ovG&?Eu2v9%d9%=:0-)G81W$"
        "2VB58ox?B5i^lC#`+&QM@H$Xlklc8.u;Tv-L?:T%s^D.3*S+F3,rS3H9RO&R72AwD#1M6Gck481m*1KOor8EuZQ2#0AmT%MGB88#7lPLMK[]^7pa4?#L@*Y$F#G5S,RRv#b.<d$0Yl>#"
        "Zwxe%pk/2'cxF;-SX.$G)i[aaD&q#lS^c3'4).s?IIN6&6.nS%D@of(wHw@3uOfC#kU*MALrHs-u-MkLM'5w$KlV20:u]=%J$5v_uSXh#eI%T.$Ub/(YJKG.7+3vl/2Rj99fJ`7'$Y?#"
        "TeAr%J;l5S4kwv#j_S&&<LRW$8Nxu#,65##UFGJ(g.G;-[qR$G1++baL>?$l[,%L(8ARs?U05O'B_/m&[,5$PQd;A3&vQ?lL>:j`9?rHMJ<rf[X:N)c/kFG/9F.%#Ft#b:mvbE=^DWK("
        "kY$?#+O.K:wU6C#FG:C,QjDH,P&Js$Gm4$eck0QAOJB)(7*CD39);C6JnLm;_:>)4,=Z$5wX?Nb=L#juwt,qRB12k4jRU-[0dkJOHe%N%G<cX;0K$$$@'*##&[L-ZkaN$#Y'+&#@?]0)"
        "*->>#C2sA+uJ]*-fXm0-CHcj'[K7e,_CNq%;?eS%Wv,;%Z)QK+ij76A'8,LO^:Z%3&PeG;25(B#nlGh:#8bi_dSCt-k:W$;%[R4XLTn`$k(r3;$dN-ZTuOm1O3muL+qWh+sPdg+1q;V7"
        "_h<e+wHx0:;4fD+7g5eZV<@Al'Pfr%RPT9AQ:8UH@B'2Y@B4sVxG:Q8O0ST$8Q[8#.O']7@tH;@$76,*dq--DTE/TfRq+$$osN^,M?_A#vepi'kobv,n0)SAb9xw#8;(<$xQ<]33PIc("
        "&bXt@u;6@#Ht&9(8`9D5Gdv_-#JRv$irxF4bn$c*7bJW-<igXS1$)w(XF3]-fpFf_NwZ/<Fe94W`9vI=JO+l5P`gX/Y+/;pUn>xO<1Zj9a(&Q/8X'_0(IYnuk21muCYtC%O':*WvcID*"
        "<H^nF9nn)#B,R4vfwIh$ame%#EHH^75Jq>&,MPJ(mav;@uXtn'oT%N2sfnT2tlPc2T%:Q(FuRn&tO[62r@[=$W-c0(L,i0(N*K2'eCca$Y+4n-&S+FPuTf;8<#.m0WBFT.DPXL24RlZ%"
        "POuM(F$nO(`EHGMY1R*&1xR_##F/[#_WrqTonrMlg4]YuHn7R#sOQuSbwwfmY)Q[tq47qEpn8R2BO/F%3*^n2V`-luP;M:C*7@GVgI[5/ObW.@lMk%+WC_U%ho###*6]M^NMsx+^vdv%"
        "gIIrmrrbY#*lB%tLZvj'EP4t?.lQS%HC?;7DxSwU^tv>#X<Z(sU%o+MPklgLX^iG#tAcY#j_.0:$#jIq8`mY#'lCl%#4eo%(EWp'8p>T%L:eS%vX(&,OeCk'KRdZ&GKlf(*OR=7*^)22"
        "Ic</(VM]s0pSO12mN4N3%0d%ACf]e(-)4CljQYH21oH>##Wm$$K)'J34o0N(&(Vl%?].`#HisVj<iLoB?)(xth<uZ`Y$R:0beR2=4GMGun>YV)Sr4(AN]A+;?6o+`$:(w8A4_t/#)>>#"
        "r'89$$%]d#nh8*#%/5##41ST$n^b&#02<)#[0)O1?]tx>pgqD#w%QS7XS'X6uCS_4SI=D6'k%(+hUfd<n9OT%:WRw%(B,##Y_e<7KKjMBw(_3(`qZ%3W9<l(IU@lLn@sO']ZNh#BKR7A"
        "^jbA#O8K,MnXaL%xVPVH&.BOGq<Nb-oV_dH:qZ9:pv-h(b?m_7`ef33QbfYDOk7B8OVcSB9@;]5gm?/E+VC'+me.8Rj@E:e]#Of2KSs)*l&QN'C[^OG#xI@-gj_sEKDi@9'v(uurf#)Q"
        "rPX_Aj8RQ:GGwKR*6>##+I:;$5Cp*#X9F&#@<6^7GOEY-#Vfi'6Cbv>IG.C#NRbY1T+Sa1US?j'(Md5/>LF$>hG;c<.:`oIPdc^#Z2_r.XM5_+B%8%#Z_2nAh/902,p`i2o*'u$/uw20"
        "S7K,3%hp`3NJm'&gX3(&,.ikL&%x0sN'u7B(M7OCm%?Y5wp5cP.)luLO0G>#U[=Z8A6*lECw'676d;#$Hn9s(=9<Z6v:1BHMk&g:>[-Iu9=aauMtL69jZX[7-P1U.a,>>#Y`S#$(@BP7"
        "B,GX7O_+/(%)###(:.?#RiW`<u_li9,Nx>#3G7[#Thbs%j]CT(f%EpL7juM(&9L#$=[cV^5.LS[8Cq?9uF)206g$^4gHRs?IXI%#Yl(F*9m<t&JIu>%RcA^+A)G/_8,,E5;=j)'I,[H)"
        "DkSm&KKY/(kxGM9QcnP'K0'/+*4)<+pS#A#qCD;$6*3a*J7+k'IE0a<@+Q##;g[/3N<okLYK/@#Ud9W-8*tdOhP*22Eii,5%<M?l+`Q_dc5vf(JB1T%3RJw#jKAxZu5%m>-m%$.*`VRS"
        "G[gL%]<8i3$#%2Kj8VD*W4@X#EhI%=4AS6iltQuAOF?$$j%]d#r*^*#9w5l$5Eb$M:O'N(u?f&-9CN5&#Q###k+,J_Z,pZ#nL9PSEw]Q&A(%W$F)man#Cr#)aR7F%iv>Z.7)Kq'84n8%"
        "Tdh,)Yie-)o@H@#?W$8).M,j'O?1K(_vlN'ci-n7aD7h(91_[/W5ed)Hhoi'J9:o/^4H12BK]s0CvV^'2NOLMb>HA'AOBD3ZenO#6<8V?lg(puNxl19nX*pu#d3&4$RtkoxRXAur+LBu"
        "V_jfL*DmA3W'89$-h%-#QNQ^7jJL0*fUQlLj$(r&m)<#?aeUfanm?&$DS?;-?54q&]qd%5(LLD#dpON5o[ED#YlOP(4m+-*Gbt0(]=h_%<b`GlTR'tm54]+%=&+220(J+%fB:a#Wq9H<"
        "#m,g)PqMeM)mNB.^MNc6kNm#HNV`9;*SCI=D7-DQlI:G3i?mE=*R,fEX;LC[v_m'?OoMrAMM5OO@Q/u(ZWCh;L@O;Rp;qq%GdwX9]8E80w&P01A^$W8C7G=.q<[w,19>O0U,)B@l-HX$"
        "F:9v5'gPP&%9Kd+WcWa*fuuM95sF$#nnU<-It]aaTb#0qR](0_PwYa'>_[/_fm@Q(VJ^t%s*o#>E[jP&DFE5&:+g0%R80*s8E>n0iu7T%W7JC#paBD3&Jo-)YQ/@#:#M;$nar>u[I-g:"
        "0ao:$qx0n#cju#uH4isCL)#B$((+]#soE9#,]:Z#)>$##T8VJ(jrg._C.?<*7$XD<OA?vZbL@VmfoK9&sP$?.)PC;$R8+^4EU/m&IR<5&$wh?%4v1Z#>lc##ER[8%PUS;7CtZGM0-$u@"
        "$)p'5M6oO(d7JC#RL<D<6K6_#C6-MBdN(puZpb#M47FWul+LBuUNem/f+,##)Ko9$'Wn`%ce7G;3C[M90WrFrv+a9)E;R@)`o5#>^D*.)w'T*#_2X**)07D*B0^Q&IfU4)Bsw[%>q#XL"
        "dtMu=<GD8Ai@i8.WmxGGq,l;-f'wd$c)**4*)TF4o%E]-7&Mmu+@.WAI7Zf2GRG`4]v4l(-lKS[_ZEoR*iUs$j'@Vq%NI48d(-Z$SxMd%9,x4AnPZA#6Xq%.V*PS7eI<^#[q%s$H>pQ("
        "Tq#j'sGgq'D`3p%.<^30dILw,<mO++CLt.+[=^+*)HYx#W3PJ(Li:H3D8V#AaG#W-n#EKaSG:e;J`tQ'Q%:8.'weP/MWIh,;a[D*G,3D#cC'eVarnh(TlW_NM%F,M_xx-Wp5>3'`0o<$"
        "a1'+*o%_diK+K@5O$X?u3qrD'=7sG+C[)g:#(%T8^`/L(G3L^-Yoh,O,c:?@-V.s$'T5;'^+S5'MEiT7_hcmf3u)X$TqN%bP&sK(h4Vj9@]mT%bNO>#38(R#Toxu,W'*v#51<5&@u>##"
        "Nd[/3gg+W-?%U-Fc)wh21c?X-[4)e$;R/@#b`RP^o'te^TX=Y5I+qh^$[P`*3P8$]jRdvZ931g(#B98%W*f),thR>>fU1<-[q`#Gf_VW$kLrx=/-2c#LE_c)kT1LGQlQcmn<$/CMM`W^"
        "#I%g:4AOS[2%-t0>mN=$r#]d#'nl+#K)oAG-Ci]F'thQ9$,>>#LI$##%Z0LGv##B6-&6]kscFnEac#$&4Y>>#xcJe$1RPfLJC[Y#(Hka$oGgm$KRm>#0Muu#?nb2$CbCv#*KI%%jY%H2"
        "86]t@@,V#AwLF12XmxGGL8_M9n(4,4O^%du^sa@becJS[$cKS[i*>>#Cb3x#)3AnL,H;pL5g?##WujX$aX.%#63*jLjx,@H+G3'#SvIG*CQ=L:js:,'qpU^XI3,n&Pm;_$FA/m0`]qB#"
        "_#7-1'^uB#TDPS7cdd%-q8sA+S2TV0*nW^,E?R1(mcb=&_tGNB?'T,3n>P7(k56hHCL[m0H%w@3;%.)BhpX,M$2f]4'#NG)pM;O'0o[:/s/xF4w3@o`qs?0Nw@QlGUfhq99H4gY$t.-["
        "o$Dx.P#fjYs93s?0-Hic#vXlQR2Uo.bWTZ>$=D?K0:YY&DG%G)s-)x#cRA2DE.tgee'W])J+P:vF7$##Po,g:1m$8@$,>>#T$uFM(H+g:sU^l8K[(o99`$s$r$Dj9sM<5&nnU<-/Dn-t"
        "hjSs$#?gu&H9g6&<:'d)`S387pAQT%8G;0Aq-$:Aeawm_n6.q$Cv45]@k.a^uWQZV$#%2KjrG%=$QK%=)^SPTHlJ]uYJvd*[6f),4W*X&'JHn%YMqK*Ffiv#M*HR&%)M/s^5*22(<$h%"
        "X/N$BQGEF3x1f]4Fax9.0GJ>%R)Y`<P?gO;^[Ka^Si)>A#+4N=F=paud4V$,BZ&Jhl8LonMelM(_>xx8Rk#9T@$wM9(2t,2c$2*#F*2Zc6wp+&O98&&Nd[)&BD>?#@LRW$phR>>_0a?#"
        "l+s.L0Y-[#6?OS7il,12@@3T%%Egu&+K.`$Jx:Z#$*0W-gOP8pEUkJ%`hW#AWFLAlYm[v?-;kM(4qv$$mW4'_>_fQ#3^J0$s]8-8`<X,O5n]Vmx(Zn')$d/<2.^K%G?,>YuHp83JmxSR"
        "F.6C%8Iw8%1p&U'Q;wgL(kk&#U)TM'gH/@#/JOS7QL<$l:.3=$.bHO(#1;j9ACSn&L^(O'<:I<-R>EF&:r8$c?v^t@_%q`NEeir?)DR0Y>lAx$:X8.L.sC']=q2dbcDJS[s)]Y#LI$##"
        "XoF']qsN$#0<6^7gwlB$ZegF,N<XM,e$P87WB$)*+Yjl&G6@S[.S$[#EL35&B)b=&8c9F&O5ed)'snKch,ha<PSmb<:WJ$&b%3,2ApJ129#N?l.,lDH0OZo$In@X-Yd_`3P.@[@C*3/:"
        "/?^jew&-T%:vuj#h`bV-Zv2&v`/v]4B>a^#0]0t;*f#%$eU&%t%w`N(ZPu30x7ae+u9>^$mCp*#?.`$#7Zd^7nnT*#[7Y;-m<hlf6%r;$1n.k9_g-m'xj94-g2_7-<CJGVrCLw,vxI8%"
        "1ae<&,C7eMwR-R&Mf#V')1PT(h8h-%rg/0b5JvX$1[EINC9fC#6>eC#-r-x6Q@fHdB.svoPG.RB+-6g^rT)Fjx(=</lx?YNXP&g:$W+g:_>c6ak1fHdG5YY#@'as6He[%#6<fK)61pf+"
        "?%-##?rV%GE](V[0rnu$getgLnEs4'-^4T&?-,ND^2*22B`9BG=jb%3Tp4)O^68L(6D6h-^mBXU7#eQ#3br2Pb`>F#G#M1pvTG5AR=x4Ai]wN'5-S0:T)q#Q%8w^#O&f`*YdT>$^fU;$"
        "t9f0:_8@6'sSq?#M*5b'bhCw,rvJw#L6b9%_W62(AccxFnr_AlFD9,1>h#X$n$$@5&q`*'1qH)4_c6<.]CI8%$re'&fd258ft%G4bCiBHSrHYu0qNI'A#Qx597C(4hXT0<=:7ZM_M;u_"
        "k;e+W3')'>EeEQX;F[-)?YA**Y6=,OYD8>hFD%##)@o1vKU=gL]f7iL<[e]Gh4n1:`H/@#@LPJ(UGL2:]M*n'O`r^#daw10@IWB#7&w5/>ja9'iFBd<Oj^a<#U#>YTpF[#lhWP&a<A[#"
        "V)m3'c]*e)q2[GMwRKm([V(W-N%c:9V2eu%enr1=M;FQ'CO=j1rqpW&'dn`*OrfT%?S9T7juXXB3mc?%*]Ra^4$&0:U)oWB1twDu@ch.Q@aUiP)t'c$+WC6ae(k=$p?2AH,WT2;A0]IV"
        "AQXs6I>uu#XDp*#Pww%#xpg[#eYEa*2OX?>3GV`FGN5#$8Tf._n`p,+(b7O9<eYe);FGG,He3i*O82c<A6S0:N8SBfpWcC#7WHBlF&9J3;O3*s,'$@5tARL2;Muo$Xs:a#V*x9.oS`G)"
        "T*`I3*;Rv$Mj@8.dias/H,7l=7E=ZrmKZ#@J)>`Egi_,'Sm3u9gK*V025=S[DgKR9$]YfC/o2B%e?jci>wv.:*`$s$G(Q]SlX?>#HaLc)S2%H)^/bQjSC7%-f5sQ1DAxE3k:tQ34j4^,"
        "^m7G*9eN)+p[)D+nl6b<0sg30C$F20gP[`*m0+t-hb)rJY#T,3#an%/@O,@5DBk>J#oKb*s6SW-N;gLC296K)&[c.qsx40&w6j1=Sb0P^/ujLC#tpU@lx+p8Bac%*RPsw;%G'(#d%8g1"
        "W:1W-p/Moe*`$s$*x3'DSnpj(Kb-29Dr82B4StWSeoH(#*Ck[7CfK+*X1I#QsuQ^#I^.)*5%xY>JodqpO>';*YP#j'HY@.2XmZ)*erm'=GZAS8;DN,sG7V#A-Og#R&YR_#Iu[AF9Edv$"
        ",0fX-j2hs-JUJk*KpK$5P;n?#*J`s%kOK4MubOT)XjWKSe&L+70k9Z$%Go_Tj+x)+.u[iL/]s(vhH3XAJ83,#[lPJV,HSZHlo5s$GKLfLBH-ipw]^d1I,Y:vIQR@#Xi+g;?_#Z%u1%wu"
        "2t3=$5(+&#NAj_7,AE*,DDf<&PA&;QSm^]#e(-##g)28&&u_[,Gv=81eQ(41pt291fcGC1Z4/H)wCm590$2t.UEhl'[?m**TZA(%L;SDlP4_t@IA2Q/F8*@5nl^t@2K93()gK?lw3IM9"
        "5<T;.DltB43=S_#Xl*F3C[3v#8pINesh&OGV$d$<(/=n0aq?s7x_hZjBJNT9%h3=A2vuP&OOLrZIjtVHsM@N0X?O&#A4c[$B8%j'L^ok9Apwlp@hiE.2i)B#XFH>#9ek$>mY?b<lWYfL"
        "d7tx+7hkH;c-sae6u/TCG-,c,p]tEEZ40Z-NWoW_Nbes-H)kBO&w#/CFXja9SMq^u%&=d)&lJB7nSWu@E*Aha0VSr6&3lG;8),r`^tbf(931g(;v-I/(5M@#ZsZ`*W=&Y-l>'fDCKOm'"
        "V>ja*>Exa*J]`9.QgSm2vUw$'agOX-*W2Q_tin`*h5o+3'1)##3&9W-woa.h*`$s$:l9Q8eJVH*h^tl&IF`v#-1xG;]Omv$cd''#P<Lw-=P>vMocbjLSaGK1'p6xG]>3ebap4=$%]Qk%"
        "t/>^6d`Ji^N^AZ#-(TK%x)vN%*oO)N('.@%8/I8%&rci9J,V3ig3R##)Qgu&,WEx%D%wO&t4+87vY[2;*v^N(F,V#AsF]#Ag>Vg14ctM(h5xH#0G9l]p2fw:O33I+sk;i-O+`KlR6,vl"
        "oI^l8?,ar?NAJ9$$kj`<-:W%$brAmfF*Kq%8$iV$E/6_%X8x;-*Laf1EHCg(*+$K3L;M?lXc`*ME.;hL>f^GlATQJ($[Oo2i<gM'PM6%`c;PJ1;d6IqCG9ebXNO5&H2FqD94f),pN%##"
        "5/5##vwli9=7x/(wA4-vF[<D<P,=gL&e55Ms'?D-U.4j$bc(Zu1.-AXFMu6u]L5F#c)>>#]c:X'4c/*##MCW$Oq[5/4g.VZRw1_,x%jF&^f1='A>BP8/7_c)p8QO(Bw@jUO.SO($bNe*"
        "nEF9.;52GV:ns/)Q<Z]45AR2Cv:KH@O56X1_i###;Y6X1a;nBAkkCF4iqHn00MNY5+i?8%sm>>5AZ1*+g]^`3(:^fLbCX1^s`N_AOA#HM6RcciSX.v?R0Q&#7C26Mm#OP(.8lJ20;4kN"
        "o9]>-o_5s0>@CP8M1N^-)RGgLn@v2vN#?b*:q+>-Q>:@-Vx8?.g0@8.s4':2%Zg>eZU+E=R(prZLV#v#O,d6#&E#i73_70:,GP##F[Ic`E8AV#q3H12#hM`$Sn^S@G+U4YQiIfL(I?#Y"
        ";JD,`-VQuuSi;%$sc)'MUAe##TDm=$11RW$)]('q1]CZ#_t,hI;n0B#<+_GlvUtA#d[$b[2BRL2TqdLMEg4)<$,QA=#]t]uMS>lOlieh%'Mn%b0LVM^T@ho.=%WW'@$k9%Ul)D%lZ>N%"
        ")5dmS(+S-%Kbc]m&@En<W1EpLpfuS/$F.J3aOQp.?.kj1_1Fx'#?qU%4-ZC4#'&05:+T$Ko#UoLUes?#I#1J#`hET#wxC/UA11L>&50L>4IqhL:E4jL68158_ZJ12](+]'$BqU%,ku_4"
        "x#s/5@hUtL$hI>/[ZoX#dXl)MAt[O_GoH%bDvaa*@;YsML-[TMY4&s$NqN%bH`41:2/&F-5:*F09EI12j5=h2ilB:D#=,O'UDSb-Rbj9D'CVFNWo-9M#JYx#=D@`$Qr>e5`>=SAG'Gj'"
        "QLv]Fm+MDa2?b]k9..W$OTVaNklK(M#<Fi%,-1#$9in$$#Yvm%*Sol'(J6l1l$+X$f[@TMA&q:JhOs.*<N?D*g.t1gCcc#54am^7Gg:A#4Ypi'('>`Fd&ik'pQS_F51+FaPD?_kgfNI)"
        "DZCn&cSW`<K@mxFA[E`aR2G8%l4SC'h:&0U?G>##X=EA357&QM6UHhM?bU#Ar[lo7p%tY-*ckA#$N1e&<nJ-8kXU8g[4XB1FLrO)rd?S)hGn[)[.wa0X_VW'oMlb'3[xfL$D?I%-g3Pf"
        "Kh&Obi6tt$O9JfL[T-1M7#^N-Yb=-8j%tY-`neX-,ANW/f&nU&^k6##]d4u#cmD<#He[%#Y(#uf+<N1qVS,o+(`Pr+(-9A#;.4J2OAOoAJIc1L4n)FNd<#)1ke,(+;mw#l3iQw#Bg.^#"
        "lA4hc8;058MO8W7HpO1:CS;&YbN-_fQj+W-^sw?gahW#AI2IB&H9QjM(rWK8MjGmUJ`4.#J_-0#?+$(%Bf1$#n@_CQj?o#l2ld<$[75/(XEiT7@7[s$MTET%aa)HrhB@h&R+Q?#CCpx="
        "**nW&V6N?#WM66&m]2B4<>h(a9R[w#xZ:v#-SUV$Tm<9%IHs##?#,=7[Y<.3`<eM0=+_GlLD+F3%X#)M;b(p.uP<I3<--_f=]#0)>ZH&4b%E%$^D+euw/at4$(b]43hQC#o%K7-$,^xt"
        "lVUA#Arhd,*tSN-6:rg%7F.S[AICD*7uYU[EINx#0<>;-7Cxb<R'#3'bouj'<]C;$=4f*)tO?@#8[$5JW$Fq(7D:@#oM5n(^qUC(iGi0(?P:;$Pv86&7cUV$7(bp.^v#Z$t%]h$<K3T%"
        "Z5QR&hkFL(feL((-D,##IN]v#IM<l$XT`wgfP69AX%dI3o^M-MKT_S-HArT.Jhhwm9;P4%[,=FN`S-*M?vlc2$cP1^lT(`uTF+42RVGs-EKb1MA.AC#[,,3-$acoIlVUA#2:h-MnWifu"
        "KrbU%CE+##g`>N#&.U+$)C%%#Rhl/2rW]_FGs<Rf=xZwueVEe)RkM^F1ps8[]tBjpdvDmfiQ`3_0Ybs&'I^*#b3pi'g%SfL;hdGc1san0(>)s@3pNx(gtMd.rfxh2Yies.si4.34#N^#"
        "P<gB7T'X5CT7gB7X-X5Ce^n7R=ZJDj&0DqM2Z5'#E]?f7c`fY.B*V:6EA`D#waM56[*^D#Of$/CfMw`#4^``3P)xv5?LQvCohi[#ttdl/gs[C#3XRxk^?S?3i9bC#1VI]OPE,53i8_C#"
        "'tGG2#TF;-BEQ*3ghO:&6EX.q$$v+#405##Su4;--Vc##`]viT7=W$#QQ&7pcj,,2DcW#AeFg;-7XJ(%Cp*BG5Z)22]a`=(pRTL2FD2i:=U#<.E_%p.lqY8/l1A)]`-4ZQ794:.ig'Vc"
        "2?(P:eV3]-BLG)4q[Nx-EUP)4PE5U%j*98%w[N1pFOB8eID8>,0Yju5Vv<J:`ia%O`n9s]@kPb,8=MK.4?m-7&bjY7OhG&$93Vs/s]2s/tjU&$&%k#;q?AN<lR]sKAJha$G:m2^9:xq%"
        "Ljjj?$ZYW<QXXu+0RLDlIRE@l1.-k('?%12kbW#AP/D.2;db%3;G0+*T]s0(t08W$hK:P'LN<6+kW'B#eZrFtIFG?U._UucT.MP<OC:=/Olu4<TU_X/q5@4+YSD@lK^_^#D_+rTx6Eh5"
        "E,=k:coJ?$rWxO#ZF[KWO/9p1BSs%XA5oNMeep.Io`/,Zr9MiHkMafYS1OcMngef:<64s$-A###0+1^#rV4U%i2###wa/A=?mNxtI^xr$k(3N$IqN%bB$hm&`>t87-da8[Xh0jp97ns$"
        "co$1$-PY##/`_V$-=#120>O22@9vhUq$Bg2*<e`*q@.b*.0q#P,P/Z7.^=a;OQ'3(KRvY#@4c_.jKR>#FLu$/@L`t@gTL7Sj*CB#ag)mHX%M7SD@xjkr=[:mD-WM^Nh;;$Y@D0lOY:;$"
        "t^<u%:9SC#n[fo@,->>#n_(##3U)F.f28>,dIGs&d=r$#O.i>7irs.3x3A4CrA-_#QZpaUc1UM^r&Pq2e'.##ZCb]#.:78#dC@Z7qkp:#cf/+*q_N=*w,YB*M3]@#'BFi&2G#i7='S0:"
        "N`6`a+)dc(ngs'Mgr;]3=nbC#<cv@3p-xghw[u[[<UYH3,&'J3(1J[#8J+Q1#IdkK;#CT7VF[>,aiDu8Zo%#8i_@JU04atu/nL1.[2MfLjUmc%*-YrHiD@/(:rP##jbC<-G'^Q&%0B@#"
        ":j-s?l=Z]k9:w8%=k<X$H0R8%FZ6sADB:K(X')O'Ck=Y''(qT7C^7[%K_e<$GD(w#a2vQW'a9D5a]T9A.<Hc(M^p9%Zx^t@ZvA,3PKE/2:tY/1Q:ZV-8*x9.kDsI3C(KgL)t=]b$S:%k"
        ":rLm#-#Ro##XDW#6@o[tc>pI$?GG=uwlSN-'a=<3kDI8#q?ib7gc*e)^#@<.@s3jL@eSb,O#v2vlXNv%Z>O`+C5[C#%6XV7Me8b,MWWA/>jg30OngN05LIlf2RxN0+E.T0w>*a09ve5/"
        "H>M'#B4YJ,aC:/#K=KDls+9+=G_L$^&i0B#9QaI3[V=m02(_4.*e1U@2%^,3XG2Z>ud5g)YJ(]MJ23D'_[-lLC]8f3UQ>MTSgO@Yf)1B#:)uR*[c.o&a4=l'=8*uUWN#^?E9[BX6O(q8"
        "`ZahL<35)k@o.33h#k4Sw&]M^O=^]+-,r%4JAPg*Gjei(tlDl*DWIiL'52X..i_V$(e41:LqN%bQ^HN'WB)^4Ll[]4r0]0:Yk6`.RP:b**9Xe)N`Bg)P9VX.L?qv$*9>VH/S+87fSMDl"
        "F#M&d[_1*sk_9t@m^cn0WR>l$QqQ4($&J4(6*tt.x1f]4$)0N%<j4c4+`OF3+S4I)]sUO''O1N(h-)^81@l$%a6Cv5Bs=^,&0Y@7kJ@Z._L-KEO&Y_u-9=aeDo'ptuX_g5VGNr@2o7eW"
        ";U(I$j-?AOVPuP9Ao&DaTKL'hN0OP;YIYF@CmtEu64kx`dk-##S0ST$>Q[8#BS_$^EhLE4o7Do%+^.3'.rLx5Fj0/1,-YX$Eggw--cnl/g,5:.174U.:-WT1..Y^.%6dc.[0`o..#_$-"
        "br,UABenTfRP3N1mwp_4'Ja=%$5u@,YmwJ3i1*M8V<)s@s(sG2YgK?lE*CD3l=cI3GcWI3T9Ct%#iJQ/Xg$H)xRKF*[SRK1C]R_#+EsI3<,9f3orHb%eiVD3m#-T%G$m29Mxrm(k*q48"
        "(;P>#XtO,.Er,LN%07F@-@QI?tLMj#mJnw(.p&nGpc#i<O`$Q/P:>G@+i>n&-$,40+cEA_r@64,*Vw3b[])],7oJX%0#PPQaKhwQnO$##`#B.vW6vn$=7>##f&U'#2:`b7j5D>5D)^a'"
        "6Zo._IqOm:[+a]9wXQd9lxp?#&.QM9?LJM'KR'd2cmt<$nfkuux3%g:IT_c2f0?4(1A]'l&Nr[$F-G)u2#w(u/bO&uNbR%uNb7s$&P+87#%l_r_#*b$voxGGw)0(Msn0F*dmo0#@GV#A"
        "ZqQ4(Z&nO(jOm;-/gbW'2^1X-G=EcYW&e=/?;Rv$]WD.3V)2u$5VG#upA+j0fRAe<0QjGFv96e<0KaGF,'D`3m/.p0kQDN(k#iS0mWMN(Vs%wICA$Y@+fA4C.CGA>$t&KbLCs;.$nQi$"
        "4Igm=T'3O1*_uLN9V`Q3SDr;.u1(f3M#)Z-IS_rQwMD*@*Or=/lKS='^b[cmm&1r(85YY#2:O1ps6>8e5cNY5@kh/3%73W7^Nl?.cUIs$+CBEFrY.L(@hQO(VVgY(gO9f)JEVL(NN>G2"
        "jSd##ZJRs$1S+87xZK?l:<3@5E:5Clcj,,2%s2F%C1D<bWH.m0eU+H25GL8.UD.&4lCn8%QS>c4MT7J&E?1K(k5;,)U.,Q'4OUt6c='G-V)2#$M9CQ9xj$/E48m6Bk?NT13T&7;ep(^-"
        "#Sqv9H[IoB?LaH*9kVL3P)@+32mZ4M'7d&>^[,d((KCZ'@k#MpOpOl2Qvn=9rDPUDL$s^2*mi>$Nnp+3Q_rCEoO)$JD7Juc_$H&OKoED*0AR]4oh?f_:/m?#w65>57*n4.:b6S[o:v3#"
        "?LGS7G]kr-,5%*3g*,[5;PP_55MGopgZE.DxMJJaqvQ&$XNul&<;[s?ReG22A*_[mm%<`kPk/#$<m1Q&lT'$-@pUaF6VGmpt`o>,O-3-*TP<a*'5.w#3SPS'.?dg^A#(R'@Xr$#Iw1o0"
        "N2:mLmL:D5=2`#A)wDb*Yt#30vS(9(tkA12)Pl4iYBAH;KnAg2Z3Ed*V]%U.E[)P(G84[(lXep%1)7&4[k##Y)D(c%6u$W$h#srH1Mc>##PE`W(&%h;*6nJE'T6F%W&>4<.=,6:TG8q9"
        "rJ;qB)Nkf_7A8aPFh@p$j[xnf'W4&ubU;e?(Op&'9Zf+H=Yv1FAd;;Rd05##5h%p$/(G6#J9F&#emt,#[J57&uM-ba6Q0#lLUd(%Eq(T7-TXv#M`C;$[+kM(^nP>#QHbt$HMoY,.6)%c"
        "KZ$W*nhFm'+db;7U<&W7fQP$.xHo0:>AJ9$P82s%qOta*)/`v#rwsI)R^Fl7CK5h1Q:2?#OH4=$CmwJ3;rLkLe>nH3%r^I3>>eC#[p?n0pSM0AFY<.3@8mG;`:qW/-X;8.XL3]-`X)v#"
        "'N%d)Fl*F3T(qkL,@@x6-c`#$&I+0cNYno#&4/?#M/?PJZf5I]&8n0#YYm]bsuNJC@1K.U''St-_4ZYu>^wSu$^kAu_06,&>xSp&RmW:vrd%##LvvoeMgbrH%5YY#h)b@kTkH/:u2#,2"
        "k,C^-kQZM(6mYh:>f_V$2Y=i(],l$6^C#12j71u&L?==AacMs@)Z)22wPSAl]i@x*ZKnj0UOr_,mntM(T$D.3mCi/C_aC,)3RM8.]CI8%2FBsL7>I61xPI;L%g%oPM4/>*7k*p18sXm;"
        "Zp?a=VOoA)>sl/43WuB6ZP,TUcoXQVWU^D,[o.BMXrZ11bBR>6jpSB5Y(B_#.[WI)DiF.)/'K[unQ?k%P#N1pc-[M^pC7PJ_.HP/Gr35A+i?8%vmUMBZJCG24`Dv#g8#A#eni:mmQWa("
        "<Q4@#[fFG2v5h-2;QwW?E]CT(iuCp.x)X,4b[a3=Lbs3=6qf3=;524=U:W4=?pVs/x0'9.xEZ.q[0JZ3sZkC#Wm/GVuq'T3d,hC#V_VS.eXG12*0jQJ?TO$B#uYG<7(aE[rMwrSa7wlL"
        "=,NB#69Xxb'qYd/BPQB#@ZblAMSRU/q.MB#?X2J#?\?V)AEL5(A]ggv79LUA5kR;d3o6O9`Dwx@#d?'PoW@P/&8-A?#HnViKb%Jt%X6l*=eoZIOg6`g%Y/5##w0ST$)-%8#<c/*#H$iG3"
        "0ceu>D9`E#)0's$-1Cp&Ed3J<A;6[-f$K?$h[g)*VG%F#cXfBl^94C3tgW#A`d*Z5J-f<$C]R_#$[qA$ae_F*L,9f3G.,Q'm1:rA*#CB#$>x]4>5Wt(ms1d)[XM;$%I0QC77N4E<hxG*"
        "_6+>TLE)Z1YpR.-]kSd+o'6L<%]?%Cudm64K7*-40F/30f.;w68k$v.i@.R9o6.Il?A:P=Z)QV;Smn:MZ)J:9*Ff)a.<7x,^I^E*.R?>S/dE;1.GKpM]`YK3+JjODcP#.33jm&#wY@N0"
        "PG:;$A(G6#6xL$#$_K[#;LRW$FHRE$L%(N$vn_;$2G#i7MR%0:SiAb*x'D_4Hu`2'MFgq%Q3f0:08>##UP_<7m7eBl^u(W-8mf-kGZri2jGDD3[2lA#Gc*w$TukV-s<q?`SZ>:d?8iM1"
        "5ZbB,8jcOGxb_e_<<BBBagV*Y+9P##@#7ou;aWjLG/e##VKk$#HAZ$&f[xt%Yl5o$hU._S8Mgb*xpnD$FNNPA/DG##@h.m%<h&9(^RXL2hbtM(Qq]G2QnFc)Ao7du4H<@tuxBqs0$J^."
        "/lN=$l3v52v;6^7WUG,G2i-s$5:35&19Rm^7Zn<&ob4gL(1?l7FmO1:/,###2f-LGg#P1:Gdm`*w*;gLYV]x*&We)3Ud+f*9w1o0t3H12(a9D5b%EpLF&_b*5%VW63jXIucv&*up$nCu"
        "r$Qju@errQivKlWpiueU3hcERc_Pe#_W1v#C5>##b]v]#kFC>#)r^oI%-2,#qd@>#q3LG)ZIFgLrW2VZEoa`*U@qg1dlWL2LH7g)+B;k#[jk;MoA;#v;cs1%wTl##NaB@#]I=,MJD>T%"
        "Ai:v#63[8%,=#12o^@V#B&9J33f].25fes-wV*i2'e'B#Z6V/)x.quGHaT/^5m*s#rKNw#$+Q(EWnOfLaKmDIA^5d2X418.DqCP8)*)K<xO)/:h>WK9MW]WAw=XV/&%e[/3L4D#NX<Q/"
        "cX`82qh5,20O7I2m(3UINX8&Z09xvZG'AvZNJKE+C1Q##O_%[#d[:w,P<>;-R[I%#@;6N3A2`#Ad=Y@5)gK?lh5eBlD,_Gl2)m<-.2ei0?/V#AHq.[#a%NT/;8m5/vcMD3qNjR8mL2^O"
        "sF>c4`GUv-]]WI)gctr-].B+*tB:a#h=S_#'t0h;wACsTl8A0<Qg%HFoqK-<u/LoUh5JK<Zsi,FjD.lJ4`5i2<)`W7S#;11xO>E7f$qeEQ^3Q1nr>X7Oh0aG6Kp*HNtS`,`iK;:`All2"
        "(Z&.<OwuE=K:>;B,[DG3Ipp884Cr(E@f<^@T8Qm8:;gj2@),##Ib3x#CX.%#i]:Y7L7;pfcqE=$/Vc>#QLvo+WpZd)h';[,,DgY,oEr$,.1)##.j-P7=8]t@Hb`GlFKNU-TwBS-dLFR-"
        "RM'DRTvnf3>,x+Md--i(OoM:8;Qa[?Y.g/;Zhf>.-?8k();m)%2Qk8RP8ps7x)9Fi23=;0'P7d%BcX)>WaWjL.>;'#7>4I#SaL;$ukfe%NKt+;E=4E,rk,F#6]:<-$SKT700,=[5-[G&"
        "i`ZY$X2_jp@:Iw#<7Bh(lY4w5KB8W]x^5CldS9t@aJ,C3BfpWlgm$I/f%'i2uiQn9G4P)4`6ph_[0C#$Wth^#jv6P1A=Zc*9H%>-<]8j1l`+,3[C*G`[6xW$FjAe*WJ8:.MOKe)`6L?-"
        "$t'7FdC'J)XNeM1OBm;H5lOfC#=.Z#?0(,)q4>Q(Y8rK(cM[h(1.[8%O3bp%5#jIqw.+0([Wpq%YSnd)%sG>#W.c+`/Xt.%]:$?#d>:GDs?u#%eQw>#BbET%>-_J%l`@L(SEIw,Z*wY#"
        "Q3,##;xuY#9CN5&+toi'MYGZ#s)To%T_AP3%/fO(%gu@3F>eC#Z:Tk$3;6N3^[]20Hso'5sJ=7KG0u0#4@Q4(]5gg2W.Y)4PgwiLk`_:%:EL#$']rS%)G+5A*;4vIjEMxtBkwKta]OK#"
        "__7JLT,^HSjf5N#EbH1S`M,juI9[Y#jQKW--`L;$]IY##kT0]7kUjl&#A[S%g5G>#0r-W$F(l.#M46:nO8N$^4(tt.p1jc)/>2]-;#Txt%w>NuTbSCM;,bW#I2xi%Npx@S+n95OW,o3O"
        "Zr$s$.>G>#FLd(%P85.4h5Y/)Sp<f-FSoNO?a>Yl4l)/U[$)C#LjAE#N#(;-%KDVprE*?5?H:;$+wh7#1Yu##N=G`#6+i;$=.`v#69TP&p:OS7`2%0LUBKaaME,R&hCeS%7qY>#1]l##"
        "`T3L#QqqVZZ==[Z62eBl:<3@5MK%12VF+F37Hg8.xvPBkBmtM(HbOU%>BRL2IW'B#ddsv#LR9V6XM&E#?(vl&l:4lf9bw=lxq?]k&0[u,$v+>#v(GuunmQiL]r.I*o-.5/mT%##x<7G;"
        "sb(##J@Mg*HLrP,=xHA#p^pi'dYp:[nQi22du?C#%:08%5kE.Ng3[AOTaPOM:E'W7+=T;/Q(A^-3D5)OJ@@>#&X`cig-(E*xF+87Np@D*bN%8[#22]#G2<P*+$e--V(35&f..s$9-$tL"
        "V)e#%uQ%##rCU##ASO125$hC#A>6N3hP$t$LR.W-hl<R:`Pf]%-:qGP)p&J?Vbbi$Ge%.4F$Yx-?,?+<;rkT/urdF5dVf-)w<^(#591nuKPtE$+A%%#T=01(pjo[#3^9w#EL@P)pjTM["
        "biB/bXdQL2hgc_-F++F3Xq.[#AV)K*krk`UFHu?Kfiv?$$g5f;%G'(#WF0DNq6ooIJ*02'3^[s&Hh^#%6(;$#rOU##IPfV#l<91AFA7&'DQ:(&X$f<$&fC50wxOE$s-(.=m7-(+>PFD-"
        "1.LaF*]QVHuik9;U9`v%4PUV$&79X:=Ml<:6MF12%^ge*VU(*3%gGc*HBq/%mI%D,Ic8/+DY@oL3rv'?_7N'SOa]ci%S5W-(IxG*<NKX:_;&vLs7K8Nb<Kw#lRHx6=e+`#:jGc*F9Uj$"
        "nIi(,lYbGMg4qb*m@Xj0F/5##0>a`#p978#K'+&#W]Vu$*%NG<8qx]#=dl5&d8%A*bReCkRu$_ku12T.Hj&9(C8Eq--sXwnAiV2'^>;':Evp0#HhBk0Yv-Z#]];nXXDZ6XM<g5/p*)MH"
        "$0RfLH4tZ#Yalf$?b($#jbnZ7:>v=l;f:v#@0+xP,U6.MD/Zg)e_C;$,T,R&2oOU&D6^Q&wJ]P%5bT:#@(f()G2`#AlWcC#o<Y6=bl/QASL;8.]L3]-HImO(6H8f3L9Kw#nQiuPdSl@O"
        "SNu@Ks4Cv#kh#c**eS5&loRf#K/1sbB/fU&Tq(n'k.>>#B(f]4G9WM^<-Wf:idBv-H3AA,DEp?louR=/FP@5/%/6V&2G#i7QvO1:+$I;#;5RYmk:sWmEbxZ$7O2BlT9>n0Q.IUl:T9g-"
        "(UeERF,m]#vkh8.w0W/'RT-Q'g$60),jd;%(x158H0jR1$$CvI8Ym]6+tBYAd#6q.lEnQ8T-lmuA->@5QQJp8Im%YUNhgu(RH=H;x1CD$^tri0vHd8in+bo7]SKOhngAJ1gp'B-@Q:Al"
        "&7H42Xh5,2Jq]h)]MYT.O3Z(+?F9r,AxS*[7j*22SF/j-l^qE[hIQlLLHrA&R(tO'AT[F4x&1q`^HRF4E?:T%5A`[,&NtZ0fP`Z7XR'79En0V1=*`^7ggIlB>)&^7g_0.93p&X/m$[(+"
        "L3nfCQBsR1'w0k=B%+5FiR?V79/'p0+Q2kDv(BJ=LXra4A@`-4A),##KOPb#C(G6#qSvO_jKcIq-,$##cKVo$WjCjKdOX,M=Zlp#N;8t#fL7Vm66S%k$K4mL5b%QM]/$@5AQRL28xF)4"
        "=e)>PqWCiBo*&`a$C^p.G2*-uu7>##<>a`#b978#'^Q(#u4,a7+ERo-VDi5&^2;>$xJ4L#sfIBldLfU91(SM9Q:d#5P,61_KM@W/ZC%L*dVJ/3.O]239'N938IM)3a5pj6;g=n67N+20"
        "2.`=4=b^:#f;SDl1EBT(6`lS.cran0M$x#1,&+22`=Y@5]uQ?l?>Us-Ymb;HB2R=Hakaj1#.@x6p:P^?2+S_#(ISs$tl@d)oYRD*]BLV%G.:A=&cpT0^7.`,%S)(,Ck>C73R-P0ooRi1"
        "x@R3:x@p6:KAiR/w-@q7ro->7G),F+EkL/)L-lr+bmhu.xE`N(5ho79&Vx>8i6aY02k%O(+lHx6a^Ff46T$+6_ko`,T75m'DdQF#?[Dc+F`85'E.f21Who(#HQ0^#4=ad$hYt&#)v_Y7"
        "i^9$l*jQ]4ae,k4M,e#%+=0BlbLec('4[5/k1P.'9$4W-][,B?,59t@],*@5EY`U%t(QW-3hrKaqPl]#]MKE;R$5?,r6:v?K/.V7G5c++@H-E,a'nT.H55?6k=t50`Jrv7o=0_u)Bho:"
        "WT*Q1d;9w60up5`wQ-9%Bhv&4<:<m(C*Xq.T5%<8@'j02#ADGA9TD/+iJ:V.vYm&#8^gx=mR@&bru^V$?3GJ(U]W]+fh18.vsai00);D3@4ku5P?DP8i=M]=G.H]Fsr2JLC:e:msMW41"
        "ZMYf*bCe/G($s9CInWQEL'A:=D_2&=./mhF_5W/O@Pk,P-)^%9:A=E*'?ZtA]?r3;(Y8(#7TCT9@+*.N+eL%J;kp@Jm*^.>>8CiFwB32BgEO:_I;`v.C;*eFF'O2<>]c,-R/AB7W0SE$"
        "g/2/1vDdn&eue;%;l?8%];x9A%oQ(H<d3l(q4j$'j)oO(dS9t@?sQb%bVs&HP7LkL-ApV-XX7X1-4rkL:Ao5A2=kM(x>i*%@>JX1)041)8BF:._R3]-8+xC#Z_OF3m>sI3iNAX-Z>d'&"
        "'eS@#'r0B#F$nO(;kq_&Vs]G3l`>)4DFt-$wNv)4L,k.3ig'u$#*%#%xiWI)GYPLMtrId)N-o:.Zx<F3`-h;.222(&A9].*I.TgLqo,K)nQG&>oe+l$;$o*FM7)lahkS:F'MRJ(fR[M'"
        "$1]M'$w0rLi'][bbK4c2pHL`aW*%K#GNGoF*R;w$M,ELtKCNRN1arS%(_k>75--<83A28F&b]'5GJ*Zf3GFX.Waql:)OZd3c$r<-a%,7K,-jA$-6ObFkYreu]^;u#.tK:F$$kLgTZ.+F"
        "]W@iF+QYM'7boi'4DF1FxPHwK$uCqD;?7.u2#<qIDI1cDi0-]O)cuxT6kiiPFu#@AWhp?A*Qe4o/Xtq8).mm8_:lG#?*:v-?M7$1:jl&ua;$T.=<TiuXF`-#$nubuGs'/%v*ST%_6#,2"
        "1VWs/<P6W.s?PR0f0bM0mI3B#4D0#,Pdqc)+9p;.YHKJ1LJ.$$`$]w#IW;I3Y)oO(=9I1298AC#F0.@':qZw').j*%DBRL2j)H&4PP,G4FH7l1#.,Q'$V]C4.rw+%hm?d)VxUE%<H9<."
        "MA>Yu$jc=>f4<PoAm/n;/*l3=R&KJ=Z)KTKUd)W/vQPe+<jrp9<p.q9;Z>u>$88>YdCLw61EDe?E1CX/0R-K)aXF_5_H#w7^4E(6BRxP:4K.Q:m)P:vHfRc)Ls3W-/>e]4*c68%+LeF`"
        "3b%Y-8r<C7S$/s$qn4R*.38##t/ST$]:U'#)UG+#nJ0k;m)Nt6X2Oj9TKWx5uM./(@;Q'=UY687E2*o9hh#C+%pWA+R=2w6f>A+#UVK3(;DhW7v9#[$u=F:0Tj(a<JLLf<D.bj$Z)&d#"
        "@(+F3W4P4(=8]t@hC3T.-6*@5N>pV-g<Ee-U7JC#hpb59-LdZ$#DXI)/H,^=VxpW1jrxF4S,m]#_iwhE_8E.3K=]:/a?;m$Fo8N00W5x#w[Yi;Q4Rb>iuVP#N#XS7]xIkF8Pl8&wj3#-"
        "QvOq.k[c.3*_MhLZ<tm)+9OfW%^_rZm5ZW/aj`PDBCgn(IAcvE-fWA?`,CL=1Vp[5%k(ipL(5VAl/us@c.tL12`SjKG4t+3_$2^>9*7R0o5ZA#HnII*nh.,;v<)Y8i)b'>`:W4O$),##"
        "+k`2K8Tc]b*BWuPR01g.^9`S.lh70:iMqA#t&PS7,IEY-UvkA#C#jIq?8)Z-kC&5'AR`?##ct**s,99[h;L?'Hic?#0r<D<jIb6'%xA2'v&SAI+xNSS_w=(-SoE2(klXv,+_jY-i1AL("
        "qi*87vWLm(q)-,2DcW#AL=LbcBEYBGh'AJ3v3u9);;6N3clSL2G7o]4tI/a<m77W?DUe8%jQ/@#9f()+I%*g*34*Y7V*%O(i5qQ&@Xh/)l]SF4Zt]-`2t>p#>PwXV>nbZ6p3OT%o5e8%"
        "JNEd*B(-qMr+F:m43DDN7wbf(wx@mA8Uux+3ssjtOSU-+?b,,2ne+C#]S$>YmUJ?1*oa31Q17'ZZ,Y[#D/#N)JRb%73-o*7NO#07RduD#?3uD#>nvr-)rF/(D_cS7)TtV$:0u2vK^#i("
        "HOYA+Oq8U/QQBa<A4fU/J-%)*wW,ClSIw@3%[)22=ef(3Ae`GlEoj#A6X-F3M$Kc(o%E_&6Nv9)ETRL2PNv)4$2f]4':F]-'^1#$fFi8.UuUv#pgu8&o`0i)L:JV%5pl^#.0X%)2<g[u"
        "d?B;dqG]a+mw3<EM2)w,RE2o/RF>D4d(Wq8&kAaB$Y7I47-0V.+%KDr)Da^J;W]UT(PA?K=Di8?4`:)$d,4&Jio86MY`@3:v.sf[JNI,<iO.;JI$i(#@1SucXEX,2s=C5/S_$##LdXrH"
        "H9tx=,B@W&wd[5/To#>-o%j1+)6&#MUD?+%j2(@-Ta=kL<,Wq/@ECB#jQK.*,,w9/PI,R$E;O$MnSX/M2J88@?u###@aL3$ZI#g)0m'@0*&f9M&iueh%fJ5A1n,p8FbP&#Y%cZ-9+H=Z"
        "Fv9#vKxs5v<f.vpZ.SS@bX?_/QIfS^XSd$$l>?4_JCt,&3Vw2v)b_(4<xmp.*If5#PYB>#ISW`<eRj)#D4?>#l1%QM`cS+49pS+4=cl-$W/5##m8Kq#3=*1#U3=&#/6]K)rrg7(J%d%P"
        "=k<X$H7G^$t%]h$&@[s$0=bx'_'YFhm>BV#Td_x*lfxV%J]%>.nc4.3FdY>-iP]L2Mwc>H[uH=7[7[X7UD@+=iN%iLc)]?TKOZGDw.*vcLn[?T/jav%bKrT7RmqK(`/gm%e4or%Z/+D+"
        "auC;$Sk%[#GkAm&/^/Q(I.ST(TZ+&#al2H5g+AC#0`[#A8-nH3vILV8gr:I3PIR8%+afF4&iRs$boHJ;rZjD#fa>c*#kj&)>[O&-&gUlI2Jx%-tW?1209/&OwPts:+c_5<ni.1*9,fh("
        "9jgr.;O@uu@*+&#3f8t<1w@Z5>-<J:sb(##x<7G;mE%##mPv;/F6HM5$,ID#x)-,2P/f?5?kRxkie[2->GV>;6CBF#j>ri's+Hj<B*>)4^W,a<q7Hv$-odD-sTD&.X,e5T8okD#h%QS7"
        "XS'X6^QLg:M>m1K;cPp.^+5;-Mgs+;[,iu$f6H;@VaV<@5j%faP:VU4wo-D#tACK4`Wf5)52:^+6iAE-pp@TA.A+87J^dSlS0GD#9FS(M*ODpLXgH12MnT%%o-Jw#oXT[%C8`^#*l0C4"
        "rn^k$<)NG)C))GYH`Uv-PsvD*OsA<7k'vN1QCtSVKILmpOp)?=IA7V%]XeIESWj1X^R0.ESa/MX3E&H)G?q=Be'DPi8r,lokK_c&0Gdw&4aPj3O(QRjC1q,LuBD+vcl=U$5vJm%rEWf:"
        "F&)i:0rti(GK_*#5fYY#Xn[f1v^'v1&wJ#2E>o<&,(N[,eToL*6'd8JaMR]cvWAN(A2`#A#_.$pHN/*N0nmhMEl_k&*<gJ1[gHpFU^9w9[jQpFI?Jm1==gYcYGp<7L,rV.E'b[TwCx-$"
        "fo^P/angV;abK<O]_K;;H1u^4l@iI*2$IZ8vHmEAoT8Hg@,;9#2Q]._e*W)<bOLS.<_h/3?YHT%A-l+VFv7a'8<o?#+(f&G6]Y2q'm8?/ZC03(lOTu?q9=B#hYdv$h<MSA8(Tk$1EI12"
        "Y1p0%PpTs@]K7Cl_0Og)tlcLMib$F<(J'N(A2SZRTfTd4J;1x3Rk5Zct)IC>r#MjMf]50cUojNZUrkN=^^SM^o&5.<s)$l9iRaq'?$b.c7GPbPm#mrZ.;aVa@^C4=s]5q`m/l0v=33H$"
        "=g0'#`1A%-[WlA#nL7S/Wsg30oDIKVY0G$$hP-##q-_K2f3Pj'HWau/S>u#-jG@h(=qd)*xF+87bE-P7elVnaF?;W9B#pG3'M?na/+bg;0;nw^jL+F3eRU%$,?''+GN0Q2?Dm52Z5a1="
        "Sb0P^;_h;Rv8fK)J%2*(pIhTijw@J*S1Z::-Co5#[$M:Qo<1A+WQ02'.B%<&)E6:ni#4`u*AW)=GXSEjhm6L23J*:,;a;f+RLC.=Xv0X:ck7nL;7BZNBcZ##omKn&3GZY*N;Lk)/J(kL"
        "#0<j;k34H$(%u-=2k@.MY,('uGPK6vv66;#bcCY7%NuI;oqps/%$M'-La]HDtd9l:o@5l;6+Lk1C-0k0p@PO.:SHx6,AO</8Nq7&NA:V$X=e4Jt4pKPofTm(E6#3'kJKe$<AT'+w]3&+"
        ".5x+2D,u23L>eC#S.w*%3$Bp.&m1T/k.>n_9/=&%wF#G4H9B[uwNt?6$kOY'qPkH*VoUC,>]j0(4vWqM^Cc^5360^FWqAb*?te=-Cf:v#H-R8@xof=u@^>juQnH0#J8>3^itRP&<,x+2"
        "q7eBliS.a35`*T%w@C8.EHY(3N>_<78$ng)muEU6@:]D,Zi&_4@1SucNbje`?lUZd8'Y/(q7C5/-m-[KLABP8d+;A=CTPQ#hvBB#4]p'^unX=?NV1_AV(O1p/p8W->wv.:Tmev$Pki2#"
        "J*)^#8;P.*K1)##i-AX1Hxi#Nj=@AM*Rtl&6Z;a-MmkL:8*0QA?LOQ%PEI8#pfJ`7h&W>5Iw8V4BbdFXqLkO4Xh%D,3#1-Dma]6(_VYi9x_4r9Hem):X`p8%0+[iCa80J3YL,*$CIVs-"
        "er;s%u><=64-Nb*SmwJ3?p(o0/6oO(;pgj(GH6/(iVH%>dN2s@;iI:.j7JC#k&wM(ei?<.u;Tv-;:w)EU:dK)SwC.3*S+F3YdMI&Gs:F63.t'63`*v^LFoPL7a,fYc3T*YcN>`L-Oav^"
        "b8,/64Q;E6U6<r0?f+c@h9U0``,^Y@f3w31e;Cp.8X'_0kI]mumm4lu*3b53-m<=InFb]Y6.P/_$*83UB5MHOD+.fmlcx8/$eQl8luhiWDUT_f&.u'6]:BGrH;(/1T[gv%du4VddHB^#"
        "C#jIq9/]0:PY>W-1&?F%l+o7&58./(b)i</,_%X-1m_'#Jr0hCQW(O'rX/K:.7hP'wC&kKj9]^kGruV'-pk?#o17DEshPg)=`_S.RK@m886oSRBhEt$bBFt$4KOl9l*Z$,e3r%,Z5-@-"
        "+$9qffaU[$KA5U;=175/3H(2:4,]H)='cs-`M@P'NZ0u$6]]x$MWX&#Td[/3Lq,F35mgI*_]rB#@CrkL=f#:A6_@F*MK%12rB+,%B;3SMPdnO(WrSh(>7ZE4^<Sm2(cnA#@SB+*YNU&$"
        "N<UD301,,MXZ-64o(mY#V&_#$^T.S'1SY>#5Y]U`O[ss%aLPC+j-B5W*;8w,fte8%J<u,+L1tVT2NV#'@FNp%#>MK$o:%)?DPm[5vidA#SMeSuk,G6D&7?wfB+=TWLn>(sJNtx='4[>#"
        "#)<8&x')##9F.%#?)w0%MR@%#vctd7W,@D*_/te)Sru/_1c'9'xmpB-nH.;Q3bwZ#9)jS]qsfNMtn[Z7;mNt&_'q:@_-f&#?[9d<JIU2LOb(P.=5-B#PhTM'e_')*^Nh-):@D>#RJ<D<"
        "Cg_0(;t+/(Dei$#d0(h(.j-P7fxc;-xo>4%9U?V#m5e^4(k#Xlspcn0'#5HGx--@5wfu@3c7JC#T1/N0Dn@X-if*F3BH`hLv/me<(_`#5>^p5SA+)k4'EL=N'+]=crC3R#oOPUVUbx<@"
        ";dd-$iIRxS@MW3Gj>QjW$wYIq4K%NM:aNE61$'s$DA$Q/X4PV-C'dv%@0xk9@uU?#WhGn9I;Ot&=qJM'91Fv>3Dr5(1`,D.fQ[L.9&qM'6*S>#Nwg-)du+v,jxWS7ns/nLZXa0([II8%"
        "GE>/(:fFG2]Sj**m__hLJ6k$N'gfu0EA8r%rT8j(ag@m0w`nEn<T0.3qYVO'%CuM(9,B+483lA#Gdx$&:(6ru8rOkg%_M6Vg:*SpJ,pi0;-w?$x+=</&o]^7XxbFiU^7j%n#?x*Ix*U%"
        "e,R1p<7B8eSr);?JeJW$n,n;%o_Us-CLoQ0@ehu?TA[n0XQK?$m?;SA%c-X-o,'Z-v.ti(-Cdw/rbqD5Ah/%J&*MDl_80*s&F+F3ND:F*(1$12VSntEOX9_,m(TF4+(Jw#`r29/w5d&'"
        ")0_:%79=&F5R9O3x0'ZH;;SPV=%eb>I0jaom4SYuJn7R#=5jgX)b*.c@%:vRSOVg*1t(U%6]xQ#pYLu>vIQ1TMq8f@6rmK-R,>>#j5Q1pQ8I5A['q.C/B^A,w&9u&[o.p&01QK)u&G9%"
        "F^AV0`0_L(_-Js$Fh2?YJQoZ#KGi20H-'q%sKE:.EUJ@@bTjMMNvdp-XWm$po--@5hP1s@h@>Z$4v,Sj;CcWqj9N&F>)^A$Y)5]LkeueqOR7,)VXF3XBWn83:/-A6@13fQ^A=O%jj-O4"
        "#v][5<?Lhu2=pauZGamG-=gOTX^Kq%I)lGkN7$##R3n0#SrQ,2?WTP&?<MrZfggs$A]`Sn9=w8%@VM8n/RtI%m[L>#Bji4#E-tp%=.[8%WmTU%w*,##3@Ep%,A,##dO(<-$,a/LO#ls$"
        "^@'E<Em[P+OkNA+2G#i7R)P1:dgOS7hloB+5+%W$ci-n7I:.w#Znp6&C1on&9MG##GCVv#*+$K3&[)22Fcv@3j%9J3DcW#AEq,F3YQwGGll5H3^,Q]6qdk;.6a9D5L'Kt@?Mao2SI1N("
        "lW'B#9]rS%$O_Z-MweP/=iCZ#5jWgu#4K>59Bd%d/9,an9pEl(na':%lb5(+4riOu0S/TtjLHrWs3MX8Ej^s72fj'#/.35&,j-s?'gPP&`d$Pf,7c`#OlQg##7&^+TnmY#v1t4S+I7Z#"
        "e5wo%D=vY#O^pU%Y7p'+N<(K(SLI8%K'YJ(@3G6#2'u2v,:+8&?h:e&eb=gLl)PP&Fgsl&?1Is$D-5/(4*u2vjm^e)Sx@T%e`i:%#bI>#L>2%l]+Z8+9`$A#.b;)+FPc/_dZ`p'43Ba<"
        "]D7L(HNg6&?O7%#^+R=7Z8fk(q=AZ$#Mq0#h3H12:)>(5rF2I30>O22hw1o0q$^p(pGPh#Cig--X2#h2ew:H#n0M8.kO`S%v^'B#RIR8%YweP/,&Tl]IWw2WR)SR#j73^+j]s/q?ZAa+"
        "rwsdtoJmruUPJ87Yjq2+Y[q&.Y()->>5YY#@'*##e+UM^V(5;->0Od+YX<Q/Q)7d)^?+x#eG]Z$AT(O'5-1kblWf8Lb8:s+p^8A#UOcj+X1Gv,1L'jK2?b]k4.n8%L,a]+G`Lv#T_`$#"
        "oSG$G:i=0qX#i0(=U0d<N9&pfMn=Z$EVK#&m0ut$X/&_+O*]0:uVVO'B$PJ(P[Iw#*OR=7+nhx$K%w@3^80*sP2BT(q2h,59>Q]6t0$12<>$T.(X`I31G>8.dlOP(0rf*%An#?8W30Z-"
        "KEi0,fN*78T5<@$1sjguDpbS%mE5$`pnkN8ow(tIg0Y)4#;]L(n,Vj;nU9`$4suj#6C05)lGU5#54.<$+A%%#oELm7U=r>#J?Vg$M]p>#Qj5r%jB<20/macDPulV$KpDA+I^5V%^dGR&"
        "_su3',N2J8bLVH*Nsf^dR6TU%+1C'#fJP@#UU.W$@LSZ$L(5:&&hmG&@kj5&7s:$>Vk4/(12nT%I6:Q&9WXV-+O%Dar>ea-gmpf-B7nA#M*]S%53Ba<v_#(+a/[-)Oens$$G+87Q+R=7"
        "&[)22.L5Clublo7SIZO(1:>n00?)tm+&+22q[:J3D51]&lfu@31/b)>GH3R8DPDs@$/3:8'ZK/)A<x;-QPl)%4*$>8;hIab$JRa^IZ(6G=(I78(u9^uIti.JPkSZAS:ST%[`###NEWM^"
        "m9.5/5.?Q(SMl(NT77p(;a$p.;h70:H#jIqpVHb%WT19&'T>W...&8/DE10(Z0&w#V3x8%KS>##w0%=-nTWa(F#g<QUfL0_`Wa&688,R(Nl)p/>H^B##JS.03Thd<<-:$$m,Ms/Oab&#"
        "3]k^.pt_8.eZ45&cq_^%S6s$#Qd[/3=6Lj0.`AP3lWcC#A>`#A79IAG6'0W-9Z1/bhOt2(U@O22#G+F34vU#Ak^4N3Y7o]4:H7g)]`[D*5?#g1(O1N(^'7W?]CI8%IVXV-VfjC4R'3du"
        "U8gau6X-h>VXW5ae.;E4._5.`SK]I4/OIK#X-TEuYx-S>N6/#%#d8lO$;N#5en6cuj>RC:&L0@u=PvW*XDaaulTrG#f1KppHMbS$;Lb&#QmYi7G6R+NFKgx#l?Tm&rqd;-WP&<6E@%s$"
        "XI*t$wP,P9>'46/Cs9A#QqN%bMf>(+R89[%#I/@#U2f3%ln]W7h[.0:(.C3(aZ5T%kM6?Q?N(p/k:D8.Znm5/[%V%,+Bv8/H(hDX%m@T&^&6v$3T''%:=Rs$h@Xi(Amb;-sf2Y$iNx@3"
        "$iB.3s03u7i).m0-tSp.Eq,F3H8Rm:`fU#A^<Sm2OKuE,^fTX-@TG11uIpbuo_*puAGM[8G7Is_]A0K2GeCAX(rgN2-I@K#3wDP&qxcPCVL<?#Y*8euEjJL;tBwUZH;d4:TTICsl,Pk0"
        "QQ8o[,LNJ1$^fQ(3AP)O]^jl/osGrdtN3.-t<2(OWB5R&D3%v?1TUq%[+<3_2K3K3jxgS0jB<$#r$i<-K3Pj'&]8p&tp+o/?VL7#Siou,GNd)<AnI%#b?x[#@2^6*i@@)*2G#i7[U%0:"
        "^`7'lt'>[1RQYS7X.BO19>@s?u7On'rW<<'tgI5Bwg,R&B6^Q&BFR8%^@?##6p'K*_b3u7oB:Al4Te^$/j#Xl,qh=%<q*W-_Ru>Rbo_#AVF+F3^;Nj;^pQL2o5xF4^+^0%Q.C`4<O49)"
        "8#@lJ?/U`W[H*g:4269LC_hRJ.fm8.Lan=63+3t-R)7O:ZXkA#_M9..`1)##fN*g:wvai0@7(9'_n3.-[faM0$ZEd=M%1kLe(/t%V?5R&0)ZP9R,?k0M;hA#k8-n&l)@)*r>$(#&/G>#"
        "As)E*q2.)*<(L,MW1VY7h[.0:LqiTf/9.^1Gr2C#V'>D<)i@J17hF5&wqR#>@txj0N-hY$gxiC0m1Nk'Z*jZ#H$bT%c]g9;^?_c)DQF<-1_'t'r]CT(1:>n0i.-@5eDJ]8f5pAZOh=n0"
        "H.D?/=-lA#3U#@,/(Hg^;h1ZuuXwouAGM[8I@e8`JtNT.'F7K)IsJ0%036C--?0W-+j`2`1NTM#LEIi9DB[Jh9&d+Vo93d*1%)_?((7l;DNL(6NoD1=l*P1DY4$.DH6n^$C0*dGc0;s?"
        "lqN%bqnhb*?R*K=K$t4V-W4)$ws:Q&`[lr9t$JN<HaM'4hofS%OodT8pKeS79v&d)R1D`tkF+,).j-P7Y/MDld--@5*pxGG-[+F3dN4N3atan0Z9.C<E*2d*Rlhl/:B35('Z`I3Sf1p."
        "Z9bi0V9YA[<w9a#S,m]#AO04N86_;Imn<r1;Dq;.G-1)&-l2Q/J^Rl1aPsI3mu'c$x'L:@Za3Be1s:E6aS-C#0G5j3QN<Z6Hvkg=jiZ986tkT%Pw..4BN9[@T^cR<#VvTUfhO3=&^8bQ"
        "gACR`IUK-6+4/`4o$cqI^$+G+m.GU05j$R&G8[I2=c<C=UKoH3Ivg'?,xbU8<;uh(ELU+*t?f<$<_s5&LmZK%Tb7w#DV?3:YU]SBS]%3:(0iQC>lRa/D[nG4h;US8Rf*1;(@&?7]?bu-"
        "'v2Q97FSW$i-*E*^_^5'%sX>9wLWG55//:&WYB>#Wvf:H%u`A=KBEi^-*Pg*@WlND^HO+$L)UF?RWgbEvdQH#oO,F3@K:bEsjsP8+bdx>DsSxfA?5,$F5JG2n3uZGBACQ9oraK(0:D+*"
        "K6hO=vZ%j'&=#B=3VoNDh^HS7:h^*#jb70:CvR.#30Eu&Ji9o0KUeI:AX*r7xV7J3)HH129l;]3YZ)22tb`GluUAC#xRABl/V2s@d.j/'#iqVo6`o:&ABFp7j7T;.AGL<-?l/d)=Zc(&"
        "5OpD=,*^a#/g7Pf5j$R&Cvqh1teT_4L9Fd*CfaT%JtN52G'.^5BY7$AwB_p9.vCp/-uOA>3=3A?dTGu-Za?i40r.hM#rla6kNm#HNV`9;HL1%8sg141D;cRBF.SV8$tWx%V=i8%[7@%-"
        "UtHB?Z69K4t%5>@pbjf>MBp7/6hDd*:`Cv#CF=E&E+Vv#IoQn9+6F[?'Edk0U,)B@64TW7g4B=/mgou-exkc4Ql[Gtd*R:v6^&##I7ZM^L^FVH#)P:v0pQc)]?Gg(YYMj9d$###NnZ,v"
        "6bZc$YB%%#kpB'#`b0u.T/o+DKOxB#J3t.Le'Hp09M`S._cXgCm*IN<&r)U.Po;l.wY>W.pEss'Juba$1Vh;-t5_h$=nEs-Z2uDHra'^#L>h<-`^]3k1ln$$D1)`?V+bSTU8DFuu;%UE"
        "OB#iTnBjqUX5.qE%,5/CltZtTR+alrDE9Icix>S%.F[IJ.p$x90e%6L*u4;_D[4qiR(O1pbkF5Agm#E+KB*qiXU@%-/feu>%h*pfMbf#$uL^Y,9COZ>Hldqp`1Vl,bi#j'X44/2i`4Z,"
        "l+Ec<ZFOX?*B1AlVs9s@VJDe$aO)Q'i[pw%t=Q>#9=F]-/bnt-xVY8/Og5L'BG,&LpGhAPsEUCa$D^4OO6xr6YOerp2x8_Q'mLK]'7?$$SbmqWr->>#XIVS%QlhV$X#x%#CrX<%BF`?#"
        ":t82'7-@5+(`3&+#8TY,IJUA#2Ses-dv`d,#8o8%B?u2vixlT%HC[=$DnJ2'BOr$#^+R=7rO6]<B=bBGIx^t@0>O22X%dI3+q>g1&aCD3he^F*/jt1)__KCGE-x;6mKh)4-LsZu6q0A+"
        ".pEB,E'pN0'[iY6E]4@.#2B(4nkr2:^h,4o=RpLBw(C1<CV8##j%7t0.rw)&T[_X36KTA=ZZe%FP2Oxu'rR7&KUoV%3;Sj0voR5'5g#9%qUd@#(=a=l9Lt5&dknX-J?Xm-2pk`<vU-X-"
        "99v_-.>,##Sf4(#']#g)Ybkk'3(V$#a*MDl'(a$gIQ7$gp$d?g1QaI30*RL2WgMM%phw58vBo8%%l1kd2*WD+FWhG)_YDZ>ct6B?W.(N(@Vr-2JJECrh^^CW)j`=Bw'UjEB&PS7QBcg*"
        "c&B.$#ff+uDx&589R2<%lw^^#=9V:5RnmG$/mc+#K_V>#n)_^#`?c+M.[[V%ui/n$1pS+4T%[=.]F1W-%eSMLMStc<N4:R3O?rHMpm4&^8K%)*-`n29NAm1KDr[s?_:/,)?6Wf:e;*<&"
        "*;+&5XjSb4-Kv2vJ$Op/w'HS7gle.qo$aV7hN0Z7E6OA#52YV7UMx4(CZJu4Wij$%keOU&&q-M;9Ari'`J,425ZYqK[g<ek__d[%N4q>&<Wx12$]wp8Wh-D42*&TA=$&S8ZY/<73uIc("
        "Hm>(F*^DbPHeP+b,&+22N=$&%oeaZ6t,S4(h9JC#nG<v>G3d,*VIgq@]]5s.%]MT.i@Iw#/'Z8&ZvL]$XbN5kmjkOR'fJMllQ01SjDj/2?Pp6DwQWk1=S5RDO4?@$_;1I'qZ==$`X(`+"
        "%E3]`#YPV7s;1i357[U7F1)h3*fer8u+uxYP@'U%7DO.UO/pf=l8-;B^igiBoigRLk0,t$-.*E*Y.GKV#)>>#Z:gU%)h%-#Fhc+#+Pc##H#l.#;v8q[9NqN$;XK/2Sa9B#dMr%4%/5##"
        ")l]e$t_6(#,Ja)#oveL2'3a$8u)].8Mh=E#mme5L7xG;@Hgk,+`Aja*Dj6H)mMV#5d]eg+&4*m/`JLW$*>+87H%w@3_MP-3DcW#Aci#Xl`SqAIA-QK)@4[s$)AlMCT:@b4JjE.3r81#$"
        "]&l]#L6[]%U=S_#S8[W%$9rV$*3KLc/THe450RKLR#G99VSx?'(kLl:Qid:OlZ#X1_2?S(h[879_2H-?U>-8LRa+k2UC=_=)MK_#TL2%b-$uG4,'74KfPtDSrsPQ*Yiq[6tQKqt,3+1R"
        "F28:/FNNOB_5.,HCJP3*-I_K2AtFSa%/5##*blJ$1&G6#$pm(#L1b3:B0<D5PW]e]g^J3Mcg>Z.KqoB-`U&,5V61-2`p2f<e/%l';=9d<Q;3**dc9p7:VRm07j#Xl=MCp7'CH,*E]-T."
        "N29f3Fa,t$*WTs$u>:JEbC.<6.*nY%q]8c6q,b^B-%dc4^n2vSO(?(NR3E?5$*sr&FC'`I$)te3RmOjF-wNPaWO.@5tJbl(D7MLIiPTuLJ&kJ0Jwu9@:3FGZ]Xq?-Y=CVebQWP-g1fM1"
        "a**##NnZ,vTn4Y%PoA*#AaX.#^I/m4t]iG)cSh#$=$Sv&j`H]FO)9##sKBl<i[iFQtT7xf@ZL`%.%RiCOEft9+5v0=X9aI=+jMA==E4GD$wWJDZqSVD#AP-l)c-LaLYpc#W7mg9N%8q9"
        "n];A=W?^kaV_s-lnLihFgfxA[+XNE$HNHPAWZTT((RO%Q)d:Y7_:6e4@&o-)wR)m/UHg;%DmwJ3&5oO(Q8bI3dN4N3wLF12^UJ/A))@P3oP,9(4^S/Ae1`#AL$Bt@VDj>3W?35(vR'HG"
        "hPYW-Nx0,lrwVf'@pjfk<h9a#$EPr$NH7l1a<E:.CLD8.vrse);;gF4S,m]#v6'u$wF#G4[>kt(JiX87`x;q.e0)q93;2H#8Qr^>7FiF*4=VV0r&+W6sq+DQZE3k1x&'3FvL0pM.YnmI"
        "0Do%-rN$l1wO^&.WnO31MV]&.BK4:.euw.2eak]@#K6_#DU;Q0e&f4(Da:qL-P^%<ni.1*)VU'%7/uC/m5>##1:op$hkl##k<M,#-DEg7F$&=9$m5O<Tw)i<#A6x@Vjl<DHu=I#$tdR&"
        "He<v#7?g?7O[42:Gd[.#Ex,eksJKi<g^il8nfCl<pLS+#/.`@E@1()Em.8'Adhq52/oU.*>oj0(j2[+#EMt5(:o:W7M?1[.dQ$)*r(=.31wmH3OB*u@$ks/2_UJ/AhdXo$VS6'klBJJ#"
        "9FS(MFk,@50x,@5e1-f*8IjPq^gQL2vEW^6&`R_#h6++5,WGg1n4K=^ci=c4%Qnh%1nQ7A(eK/)#)i:.jO4@$Ij1T%qmP'=QiE-EQgS%LXPPCRixvA6gv/Q(`K@k(i-JoNu=H-*N&/90"
        "e:w-G`q]60i<((4iE6&>X2Bm:rxwu7V+^,Ncik(Gcq@n$?E@e)>,R*3:%[N)T,U;.3N1H%6ga**<23-)/wk(he#k@-x7&P'Yk$3egF^b*DEZg1XsgF7%&Gb.*'Ne3Ze9%87DHa3F>3k'"
        "`lsE*sIDQ9%/5##?LOQ%7QG##.nl+#/sYB.`;pM3pEv4STqua#6`il8=eJ@@:N_V%h9oE+5&/>>508##>YUi<e&Iuf`U)&%B-Is0i2E99_J1VAL7Gu%V`i.LunAa#wsap#L5/t#tv7i)"
        "p*(@.MtI0:[='*-m>kb$GcH=7lx^t@rC1re%*oO(%gu@3Uh5?>`M'3()Z)22bfMa%1uq8.L&:.$`q4NNq-#P=bw:w2c'V<3`$;#,f4lQAdj*<%V&5>DvaT>ZiErSAr?H&#&,wM0mhM4#"
        "l4ND#lvmgL5jgN0s'OC#[e,,2N]q;-`cYs-ssJ3MWXi,+.QTEYPaU0(:ExP'?_*n/jITc3p2B>,D)RW%pb[r'ed*;7X[A+,JuL0,fL>7,?LTc36F]>Q4uPD._B+B#mXa=.,1pH)ccVS."
        "oJkn8T5CUR>1>d3M+a=.Pu$n92)-O0+]nQ0w4WA-$:C&$s$b-?uj@q7Gp0H2a93h#VQVk#(a4N)d4j`33H(2:B,]H).j-`,$B=',#.-K)RZ/M)xTUR'UmwJ3-wml$V=Y@5)Fcn03j'<-"
        "fh6*(S%EpLFQrb=%)MI3YLS.2^C;J34beta-G9t@R1I202s+@5A3[h2Ta+3)]FXPAW%i>$sJNR%@_9d<R2KL0/]6+8+-8Dti`[G2vZ.##clN=$7R[8#0bB]7`RDs'K6l/(xHo0:lwnh_"
        "p5IE-+^G;-2G#i7@R%0:8=e;[T&R8@<[EZ-NkE>$Y*46)bKo%#bR<]3=4oG;kgis@tpcn0c:;`;C_,g))>m8.Jo0N(,%>U%:6gFEwB$'63]qJ,^9[g)/Y&@qdf4GYG3+.c?rtYR96.uS"
        "F)tRjk$?p%DV8iB0;dec5B*SXnB5VF58NT%2+Pc)M2I5A+]fr6Bpcl9I(tK(RE:>$&D]a-+%?/(M041:d1(a+mDA,*iid'=II`$#PWhCMh46/(bH]#A@;AhL#9aM-EuLE%baJ49<5[<_"
        "@JFm[m,RkM[]N/b-j^KQE]kMD$oBI)vANp@5)b)<Y+gsbU/`7@RZ*##7DF*vK9Nd$AI@@#Rs0='eHJ[#O3FA#Ddqg(9uH8%E.I8%dJ@-)AM>>#7xH8%G3T2']&ig(@7[S%7+@W$qnkgL"
        "(H$7&ND:j(GN4Z-&6Ab.vvN;$AFiZ#;sgE<qa?W&5(M?#MeWP&IUPnAQWD0qViL0_rOJj;88,R(X]Ne)HBjh).PY##8c(?#CK57&OqsP&/[ChLpFA=%H6^Q&FH#n&W<>+'f*iw,?rQS%"
        "?YlY#UE,n&C4M;$/=qT78&0U'[-x+29-nH3jrc;-.Ham$QW-F3[YjfLE%KgLR+fo@[>,-3dN4N3%.MkL#MDpLBf*F306k2(e@PRPf*+aa0*X2(]/Vp.MAWL2]lvH?[pbV$$[eP/BbK]$"
        "8plG*kN4U%PRGF#jQO:mc89mupm$v,k]),:xAs&LD+qi:en>S[>qQkurvtu5>*nO#D'`mWP,iauZON?5C@+VdksMS[Kcec)*b-'(GU%0:k<a=lb,Hb%r1&h(Li###`Cl;-aLaGV;=[s$"
        "^GDk'4P:v#/Xu-3cBj>3B*CD3;_FpLq4<I3Ud6h2a+Rs$P36T%2ndD4*^B.*vkh8.7$b/1he0t%(KGA#GK,L2?TfB5vt'I3%phr.K_Gh2C<280U>gC/Z^D8$fHsBM:K:Q(D?rc%L;3E*"
        "[u*I)b[R8%[qN%b=9M0(lqUhLt5%r%6=#l$^2P'&Js9(k$7=a(,(]AM[6T9A-a><-YqN+.o6$,M=Avg)+U^:/#F/[#J+xC#qf1FIcr3xJ;f>R0>SXM>n4B_.Dn6kEe06l1O7RML]Yw20"
        "7'*,i4OX&4Tx4;-9qYN'iH@%%ko*87-*ZPA;q+j''uH/A)%R/Au`JwBK[KT%1M0+*FJ29/0&1'6to-s.24?;J;,VQ5&q]a+sin-)w1MI;IeP(A7hY'A=`a)#u/5##7WhGFdgB#$,Kr='"
        "M7ia%;1Np.ApgAJ8@I'?e(UM^O?UJ;s%Z1:$MJX%cn[@b-SlVQ^hHP/@A3_A$.x7INfd+V34HSC&[r8.JjE.3^$Fx#7rbA#W=tY-J@:4-$>h+$Q2p*+P-rJ)0WI/C?rII4&2x$RO2sFr"
        "=8Y$0erQ[-5$`_$W'lZ-J/wV)b*gwQlSY?-vL]b-?o@k=_3CKDKh]%-rbs0RgvIPf&6ut$&<=^,5*<G3.hd*r:cQZ?$2+o9C$abNJ12X-1o4A'[Q?UPCFvW-(.=x'%qs9;B2<Q/+Sl##"
        "'boE%$-O9#He[%#'WH(#[I5+#:<x-#M;rO'9<Nv62<fP/Rj]G32OhB#=a[D*;weP/am/I$]@a$'T`>lL_pbZ-xad--G'M#$]k%>-nn%>-Df8,M_cw]4ua3>-oRQ?&`<=X$)3VE3'9X6/"
        "NCdY/^uL)?fb[X'M;cc>?<<%$>w.d<;4=69/(YR:+_`s#+h^@'Ql4I)QOYm':t%f3/:.)EB]`T/Q@rT.)aB.*4(X^-Qp-c@G]rnSv=?YY'h>%.u6&3MRLa$usAvW-d?h6N_9XQ99bern"
        "m^V@''$]Y#lQ^.LlxxGDp/Yc2`Mm5:]F8:/r0JB#D&6K/FqBK2HQ($$;&[7&3=Hc*Z7d##-C'O3-Js7[U`]G3Fv;,)EcK#&*?f/VGhDV7H*Z*7,,_f-Q^lF)#Iti]pa/nL*K?l7Y?x0:"
        "f7F.)ik3p%WP/-#+_$iL/xl;-QYcu%rZa2097[s$FS>##'DH:Al8_<7D?71M6wJg1dN4N3ci#XlB5V#A^WM*MdGaI3%*oO(v-MkL_.@P3*Y[[-+M>c4`P)B#F$nO(ZoD)&RbM8.JFIw#"
        "C]R_#P^gAZ:$lM$?p_;$aBX(3XT.v#.YCv#$*_`hM,9XXURot@_^]-PThZYYmTTL;YO@e4h)<&+#b6N;cC1_$'R<Y+^K1'-s(ml]<H<(jk*vGDb6[:0b_7m<30N3(34M-4%/5##X`AI$"
        "3nI%#WWt&#D*>5(0MAc34kfx#jr;S&QsuN'Wd(0(@pRw%w,wW&l-ub/aP)h/o99/LM25##LaB?-xehpA=B-w$%#jIq4mu?0uF*9%=-*>/#c)@#-ZPp%V+r<-;I;O'/%iV$P%h^%am,[$"
        "Fk)K3YV=Q1/,9t@N:uw$559N2^]_#Al?X20l=0N(p4)?#wq+?.M&tuPlbjOo7$M;$LN7Z^?fs,+3dA2Ff-qICdVT2u]R;W#b-?+<;*qPqKlv01M+[Y#R[$##lIUM^Z:PV-7+p51]7RT%"
        "Dws;-vF#k%7xPcDrrB#$%9cHD6b@echO6x)(35dDglkA#.CaI&5mUv_$ttXc+[bX-Y^6R`7Z-*09G:;$VX.%#x8rg%=6^&6OxF$,B0IA.CKRL.I$DS.@H$)*F?;O'T2eb<4Z,b<K_3=$"
        ",Hh2:T<pi'iTS[#Dk3=$_#%)*V6'+EPZRAGnlNf/xw:Y3r3$u@Uknj(B6RL2&@v^]nIr6VvJ=eDE(:B#IVI[uutn*&6mc^^1P.e%c&5)OIv1$UuVG52=k1:rLPe=Y%5BN2%w`N(UGuxc"
        ",<,J3GJ.,)#XC`N(8NY$vUGc*R-x8%q24L#JZnZ,4Bnx4omwu#)/pSfWC=Z.X21B#N+xs?&]c)+v*wS+mn0J)XE$K%>$Tm&OncnAT*@W&GH#n&b`Lw#+>+8796*@5v86V5r)MP-]==g$"
        "O0KV8WSV8AA8@0lF-g()I?%12I'JY-O&YS;wFWO'JW^:%AEp;.li6*nE.n]#k(TF4%a5J*N29f3>/%?$<(t0;_)MULNbBv_-h4Ob*=pC<.J0'-;49P3K*[YV06UcFDdnlTA3Y@u5-VQ5"
        "BS$GDTYGU;^Z%,?a8_Qs$pUlu)%#B$OLb&#elS`7TpW.3)2`?-b-m`4'$,b+wcu$6koQn&[RJc5BE@12eQqc3v#_v3AH.f3,YZ7(g/;k'-EG&#4K+^,%fE1NY'LS-BIQP.^+$K3prU8A"
        "arE$N6LhkL&[#@5QF5C5C60I$Dj*i2SJ<j103e,*&S'f)86XH3YNU&$S,m]#Tu5_%%#+Q'5R(O`(vm*M'e+m#U(jm;/u7w#8ZJM^Ekn)*HoEE6qSsF6RmJ4E?.:T99Wm&-C#DgEaP=m'"
        "iRS.>Q0`11jjO#-.$bZuuauXRw7Hb+]q>9PObJTT,_PQ&9wv9AHuWNCg$n%$VNRnUrxwJf$),##U>uu#H)G6#OK%c76HP,24u-W$Yiou,a8B2#Omp:#4'u2vI0J;%vQ%##Jk_97KtSEa"
        "@+R8%>M35&`Lo/LKTSZ#X@X,3LR%0:Dr8QAqA:-2uIME<pw'hLk@O)'qd&2'HE,n&C1;<'W(Tb*3J@N01&w@31S9t@(aFpLNp`m$a>/@5glSL2lntM(][qM<$=s(a6M_`<&QD*Tt[ATX"
        "-(wNutOY6WMH=q`@n/q`HL%g:fdeGFAw^Q&$ZuG27.Rs$Rx[HVZ2Yw#/qlB$L5/t#q5hKcwk^e)Sx@T%W7Jv$-jll&wh2E<7m+Z#EmE%,@Q&^+4x$<$*'Ba<Ld_c)G^3m0m?nNXik3YN"
        "tNDpLMoQL2k_Y)48>>,MSx?AUsOicu?VI[u$N_=OIdOju0.';-A.JucS+ofLkwQ(#E#<_7.rR@#=:@W$0c`n/7wWiKZ,v^#o2Vs/h%38&@/J/C<BaSAb1XlA&117',:,j'bwqakACe<$"
        "JqwW$?,Yv#YPN;&%4H%%vg]B3#.V#AQ.IUlf0H'#Wwwr-e0w9.AuHv:&o:Z>]q.[#C]R_#VeC+*glEb3QmJxtY^,i_4>;xbO%tQTdaMJ='^<p2h$G&-+`'F#^PY)[^O2W$eJXM^.?nW$"
        "w#JxqVd3CWB*@-)Vq1c*GY#W_e(LfLUxC$[JL/>c?\?38.Nj&E54TOu-0Q`G)Fw%0:F<Fq.tOOPAB0>#vQY:v#OP>n(JSH##QEMB+bhI8%*Pt**RZ@h(;wY+#`=KDl*De8LDI<d(&Fa8."
        "Z6wGG(iV?gTNb9`;5^+44FX;RG_s`#0&$r1rs`#I6w+R+93)n#KIuG,B<wZ]O#9dF7$(w,u>Gr.Yn-3ejm#A4aHJju/.=s6if]ESD0@mL*BIYu$-9m$DN7%#Y8bS(-'P1:qf%-)K>,C+"
        "eue+M+.,GM2o=A+<9l/(D/rr?<nY#Y9t%Z#N/)>P,N7)$2>0sLs:x)$>(Z##`8Mk'%eO1:ho<e)?P7@#@pjt(ef(g(2G#i7NU%0:c^b=$CIt^*whm@#3-dY#LZJI*x2<P*4f>j'LRap%"
        ">Ces$ci-n7XvZ)*In]6&Dq/U%LC,RA7bv3<)K-P7*.-@5c(1Q(j(p'5#G+F3Fu8ZAhj$H2T/lIl9_cn0'0fO(RF?UA0ENfL$Dv@3*M1o0Wp?n0Q/mA#jQ/@#&OBD3@xpkLuuAgL#s&sZ"
        "mtnPu_/jV-4/Wp#BQK5&3m`W#BUMm#Qp)JM?VKt#TQRJNZAUBu[HTU#P->>#TxnFM&:%g:JWPJ(];=3(@(<20%60[#<+Yu%8rW^4Z8H?$>'TI$*&3D#7%=H,/nP3''&v5&TTa>#5-#>Y"
        "luu/$Z+_>#vWM#&>MY#@S:6n(PW]e]laJ3Met2K*:DGn(tC'F*<be@#+QC_*af%%c+LqhLt4q2)efxN2^SF;-bPsE*x$RsA'/&L(*/VJ:]+RsA859t@uhOV(6%@p7fX)s@B<o;-)i;'%"
        "WHT#AZR$1#vVF=6ZSSl80fBWfKX8V?._9qW4P*ou<O8FcT+LBuMblS.nCr?uLnrS%ZvRHu6tZ,vk'^auZ?fp$noBB#+.>>#X3nK#l####=IM>#;2a1^wlRR&gR1gL5bamLrG&D$O'l?-"
        "r:1[-uc>x0?Q^w0kx9,N%kk&#@#7ouCCfp$?0`$#i,-a$-'P1:d6Km$dtqV$O*]0:r])[$AYvP/YOn8#Y7>##N#F)#Uf-s$YW>N'&/@w')_5##]Q%U7J-Gj'@#.s?cl?5(.](?#NxO#)"
        "=PRs?X$^n&ci-n7G0]0:k20I$#iNp%?S>##@MEp%3jf=#YjI124f4l(AX>n0s6>N0Ie6Cll+$:AC'KlLNl*F32'PL#((tt.Fo0N(&>4gLVYoO(:ddC#qTx,]W(tcOi:=K#[W1%tMHTU#"
        ")?4nGv*+M1%d?C#T$uFMKZ&g:v`g1TZ>xGM]]7pej`pv#P:kj.`9NZ#;3F7/*qn%k[=FGM-(^&$Z+eS%rT@>#xXWL#?&K>#P]ii0V,',;<XI@#1SG>#-2,##=()Z#Id1O',Q*X&<DVZ#"
        "J[NP&>XTd);rxmAS5W;7;YH+9]-p,3Ix^t@#g:W$g]CT(Es67j>WiA#=N?njsP2=c%M(,;N?udu1QRN$]tisulR;W#<J_LL_Oi/:DI:;$,)G6#1<,Z_i=Z=l=hZe)+*8nAf(Fm'agjp'"
        "rQ_v%5)H9%wn;l(H_uf(8E]>#SpB>$4fc>#3xQS%gW.L(Eb+/(ZUiS%LSc##Q:1Z)i2K@#StWP&Y:(<-FelS7;eS2'MQTq%l2*XCWII=7M&+22(FUh(Iufo@>4FT.Fli,5#G+F3eD*@5"
        "@;6N3Q0>.3Ogu@3Z9bi0*l/[-*7E:.)2E/%IH/i)#x)<6GUTpu4n(s_1dRL#-V1%t=8<T#k1VFi8ww7%5&ZZ].L:fC(LLrZJxT8./Y*,)1.h^(s6f0:QqN%biv@T%<Ru2v_<m]%;V`6&"
        "oWlf(aFn3TKZ'x#B$*1).(kP&^D*.),oli9DR]R&Z/Mk'=;rH2RaCk'^2@1(O<_c)Cetm)o<ft)DkV%*LWY3'Jgq,))=6_#Qp_0($G+87KXrkL6s$12^X+F3iQ^#A^4H12PN1,sC@NOD"
        "^9_S-2nr.02VTN(F$nO(2rpk.*,jc)JD,n%37l;FxAS=K$J_%=v22puMEr4]u>pQ$(YEjJ%]*R#9<bL:3MYP/j-2e$Nk+)WHSIG))%<Z7_&:R)OK==$%Z#V'ALM$#H8.s?G4G/Cn].1("
        "2w3dX/=MU[*_%.*K6]@#N4vv&7PYY#A0]0:2#jIqPQPb%b?:g(w@9p7*18'fs@(kLh^4ClT(Pd&8&9J3tM^%Mck>-02Pcu@tARL2<:6a$*.Es-'?,T8W?/-Wj)tcOunUv_wq-6ClUQf_"
        "PF$O$8;jG]@Ph/Y/UgFMFCfs-8>uu#.[.%#S-Mb7`)vN'G[ap%?iuN)0r$s$PW]e]jaJ3MW1VY7ctI0:TVW**N*=M*Yvl3'1qHo'Cd_0(eNSw#b.9b*Tnwo%_VNI)H,HF<+>cY#c2)k'"
        "e]*87BP&Lc)xd;-E3F)9GrfG3L>%m-f2o0,iQYBG_pL+3g_F*e#q@+4L=6g)66UD3:ddC#7tdk%`d''^$GS(NkgaL#55UxFLhBpu=0#BUIMMa^v5Ufu&9[cD,W8rQoNlbuW5b9#$)wM)"
        "mR5m8BsPM'C$.K3NmY3'VphD-HkuS7I?-/q$GPS.CKG3'pP)F&(Ei0(5=[W$Hi^^%k%;T..0-@5bkSj-ne?']N=C*[`Ff@N57G)%iF3]-7-U`0?9QpuQ*$nux&?q7pHHZ$RXm4/4/@T%"
        "7J$##?8,g:BCl+D^gC2'3@;j9+>,##bIu;-R8$aa>L*9%6cu##FU>cr+SC;$9ouu#;S1v#/ZWp'$<#j'>Y.R3UBKa<C0k9%c-[q$x;3@5qbfo@ci?6%(YP6W$pxGG%/fO(N%nt7HD'N("
        "*<M3XW$5K(t^,our(8ff#J@mu4Mt]#b(oeqlrYJ1SCPS[V,TLp$),##7Q8e$.)G6#+gv^7psnD$E@_J#>X,N#9qOJ(:QV,#u/aHr^Iq_(G#.L(Ps8Q(p19m&HGo/CSRrs'Dn]m&L6^6&"
        "?\?pSAD_]Z$E9X=77?bJ-wa]Z$dhW#A$$'A3>PXAGkti:%d@eRVE4+F3(d#g[U+b6`6cu;`LGc;-SbXb$15RqL=VTlu5)ZR$qa*k57s>]%$vuP&Iduj'f:v/hI3,n&n0<U.9uDBMkC8#M"
        "?\?UkL^U#oL+9w>QANgt$#RlB$;,H^ti3@Q%9+iJ_Bnj9%*>8=%:Iw8%@9>j'U[*lM[8d@NY_U#A(Q[b[;W3eZ:SO12q`'i2U+LIZR$nO(5rbcMo+Ra^H#JZV;5A2&In$gLF.J;#wj:_$"
        "<tC$#ZE3v%)3&##u/O?#`),k-)Is#7,`jJ27N7nA=DgT[*l7U%:,.s?Rq]3'G`ws$G:3U%<_ET%U'=G%d=$:AphvNO1<Y20ax3;(u,OV(PhW'S0eACSI0fX-K@EW8_#$j'h0$p72$jcW"
        "eAhh$I4fL$`:gS3JJ+W%'jGp%lN%##@VGV'HIV$#_k5^#u<Hc%Kr#nf7+<X$VqN%b@nTm&U^KXo3kTf%K&#j',Zl,2<C3T%BwS6&=oP##Td[/3mT]%)r$Ss@9Wqi0;/(H3#kY`M*e:tQ"
        "([@S^ih`8.0):lSkh2<-9H/e%n$3>#mE[M^TI=PA]mLQ&OG[h(jx.a-2[U:@BZs=0sRF#/-[aoAhcv$$xq+j'q&Ock-]_;$V7-p.$C'$$9Vx-)m8A$,xQ<]3b-MDl*)KBGc:0*sQxPBk"
        "A%m`/B6RL25$;H#i%[:.+N2K(3BY20(w0o,&ww>q-mBLE3*X^6IocW:[+csJgO=L3iEQs_?s%^+nCN7/g2>f$LKu)MPY`G#03BP&B:)g:jou7I59B9.g_2v#BWJk9P:w@bD^Ix(fF18I"
        "&:W[#h8Q7&lFlm';#`^#t+:?/uG@P/_ACB#?w0%>]A0Y&w)P]#w3q+*X':l'e^O=$3fC&OGZ?p-c6mBoabW#A-OOLMX%or)HroI;?'h>$8j0)&r[N&-BoxM^Y>>>7=h'o/$QK%=BaSl/"
        "lj'd%>d8##Rc'j$7_n8#_hc+#3K0`$cPW@$s$c,#v&(^##;n@-+]b,#BuF$>:K8+>UnB0>q:M5>csV<@9_Tu8,.S##s(EA-iCt(Gt:0#%pTo5/pZC>$J7HUAUIVW'W$&%#QxfBlYR#-0"
        "4Ye()x;-H5EqZT-wJgk5K@_Glb&ai2wsUO'v4WF3-&Ui)u;Tv-oRS/(9NFAFWM(a4Yt$]%Zh5l1x>1V%kQT&$UTss7-$W+=vcTA,vj'oOOQ;;SHL9l;H?mF8U.m:&oP0i)8'mO3*p#@C"
        "$UJ@@5b8U;:_^H5g7tZ6/-V80ZKVE42&bm1IOr[CLgC`.U<DZYZ0oT(aHlR#8d_Q#4i<4*3tf5&s6dUTTVB8jk-9dD[kusM)M4)5OKe2>(2@c4P*R7:VK9[/Q=,mN%/5##_EEP$*i%-#"
        "(I5+#kZMt8pRW@$T#K*>w*4^#%G3]-w'f?7N=M&?t^heO+QJ($;&)P<;ZdF#4WB8%[6]-+mdom89^lo9.G6^,P-?12u5+h<j70+*K]IK12^)22XfsR#l_EiAbMq0#r.MA3h4;lIOuQsA"
        "lMl]#09p8.ootr-L]Z<-g(m<'3k#;/F.@x6bM`]$^CI8%qmTH%Hcc%kA?3h)pwI]6EHBG;R+pr/KfU]]<5_YI;]7H=oYYh:U#A]8eJ7HF#2=%$-,BC>1W8dFK:hc30Y^_4Zr$),u:tFr"
        "9W5F5`B/TE(gcJuArCN(KD4de5h&hS#qK5S7HUcFoZb]R*rNZ#EecAAW@(U/X>tW9hjme+Hb3T%rm(vuJh?gu]Gig$mf''#gP,d$,E$(#>Dlj^xVG53*;wf;DO=_/Uv3P8M2I5A<e?M9"
        "GklA#g?_W.&#oZ$:>eW%&*,##eEsH$F'G6#3Y8`74.a?#rXEPAu%-[#87:7'x+V;$Wpxu,.](?##8AjpCk86&$E*T7jYKZ#B1[S%9&4&%d:W)%:9u2v:+m9%1/2Q&?8lv#i[T[%RIw@3"
        "1S9t@A2`#AOgu@3l,TlL:'^1AAjbu97ke+4'<1^#]OL>`-NAYu&vGOu/aAe$V3%xL*d*20]l@5A>V2XUR*gK'76f?##N^:'c^a?#-c8g%ngYj%FU;mL4dxX%gAt>#VG`uP3r-W$'_YlA"
        "i0UlfINt]#(55##96.S[=6A;$.`_7%2Y.j':(bc)rrL(M70Dg*<RRgL?vQlLk^V*/XY[[-rQJ$T[$nO(l_:GDr_<&4:F*_^bIW^THrD4oOs3%S8hK]$7OY##;W/eZZ*Lc),*.eZ>jQlJ"
        "5I96&hA(P#Dm=PS(8G>#YS%1(+KmT%Bh9Q&O/Xe)`L6]k?1w[#.1)s)+AOD<)[ih)I-S@#80u2vhc^w#K_3X$N[`Z#IU`Z#Sqn%#^8bI3Q1%K3j%9J30[Th#k3H12:p8J3fGD_Gjh8g2"
        "3#>']UYO1)9/aF3Y$s1)JVPM9^t,S`6a%a+G+%<$.rO2(nX5YGvE+SnCV&lfdS9N<XHF=A>wlc2(ID>#;KV&#2O?k%b.`$#bDqkp3+?U7+O^*#48Rs?.#h5SXES&&g,.s$2G#i7P)P1:"
        "CCRs$<iuY#HR0T%R]'&(p9w?#V3pi'Fph0(n//t#MWW>#gbr.Lxj(c#E@UG)(8YY#/VlY#P%BQAA<P3'M/`k'gf*870#eNbDL5ClE*CD3hu(RjM6u.5%/+R-f3F(2'?%128vU#AYgK?l"
        "N-xlLgavP.n+jc)H*1T%nF3]-9m)`#]2cCd7TSP/bn&nu$^'^u]`Tig4ddFMWUc=YT-hU3A*8dX#N^%#b8bO';Fdi9(.V>#Fwcj#%7v;%+P[9IwkD^4H[.0:[.G1q:]Eu++Wb`<5wuIV"
        "VR&f+q`5A#Vn[S%)#jIq=EId)<luu#EE3e)H'5N'VhR@#9+@W-'lH#[,*_)MtoQL2nn0N(23iA#NWYN0L=QM9'P]X#4%,&8nU_B#SJ6fUkWsI$&7eb'Ar-9O$&###Zpnl]C&mi'H04gN"
        "n7)/:?k+j'9@7W$Iwa9%Cb&q%G<#3'wOWt(%GFgLw@s[$JhRw#E''U%3:>YY^A%sdi/AAtcWkG24-;_$:Y;2'ZjYN'Dn3-`L+OQo@F3X$c,n3+k4)$#4cCf_.v_4o#D_V$1]EX%tr:;$"
        ";xcY#=qOJ(TRdc(B4R`aWSF;-2^K[#XqN%b=Z*I)7l.@#Ih<9%*#jIqROWm/^(xp&;C7W$D=R8%#ebm/0d4l(PJV#AO:md$-4;t@_uw0N_>aI3L:*82ZV_&5%gu@3CJQO(0>O22Z8fEN"
        "bHb`a4l#c')F2W%x=pe$S(Fk4G]a#>Xm5Q8[3*v#VQhJIQvErQPr$ou<0GG;0.=nuRg9(j#dLPJPVXxtZYaLgIW8c;$),##x^AI$;Cp*#WlLY7Z2c?#2:Nl]^iJA-s0/_6f44q&V+p^+"
        "H*gQ&uVuu#FEP3'1OM6jL[fo)=fWN$lWC>YBxTF$0@#J$pbR>>3rq;$_`BDje)A]tIP*p#8nV>#e&Al#W-SVHZ16v#wZ7d#^f5]ksltuuq7tS7)o&&6%64Z#Z=<5&IL2Z#HuSE#OUH>#"
        "?XR@#^#g*%CIP>%9/xNO9>?T%2G#i7sb70:O[GnAV#g<QrL5nfmTH0q%6ke'R5p?#R<>JC:7udM7jAU'AV*p%&;=w59s9[#a.`V$X$8[#C.`?#?2Bu&AM(V$s4+87#vW/<s`j/2VefGc"
        "%r*N0m^cn0F$^p(H7i0MH_.u-BYK*NBY#<-&.=,%2NOLMw&RL24w:H#(1,.)am'W$&/)B#6GUlS7inVt5w8fU7#Y.[&4/tuqqRD3LUY+DQ'B4or:@4oWTRl]`/AM9VHAx$?(G6#]84d7"
        ":jnD$D@`8('PZ&MIS>n%piNg(1@uJ(a[.GVUp$L(oxlR($Z[8%p7oP'%Z#V'v&.['ELM$#KR,R&2oOU&^FA]&A^3T%sbxt%qsEx%FK7)&R5t<:i0t5&B84K2>87w#:]#$&7vf=#aIw@3"
        "V5S#%[+$:ANOi;-_[eB2m^cn0]?>]3@PXL2Xf*F3@+^;.ZHuw$l:_f#p`wf[9jOZe`5Z/(dUe8..XhqWn=560nT@&FmQ:'f8Twd&3`.[#$xb?Kvn5b*vU3N0SK:Q&EE%L(<gEJMPIl$M"
        "7jo8%uwIl%'CnS%*_P'$Oi`u$UMuu#D54*$HaCv#S0KL):a5p%5>6t$E^Ta<vpVuPSxo?MY<$hM1ZL`NBYJ$M(5=&F&vFa98)LB#sa/kul=[Z9=)h#$wVY+#FXXGX-*5d2QO>>,.tVm("
        "dc%5'Hc.s?9H?O,cD(#$3U%RN4Ct<%&R%##:$4#-57OG<@%llK&Z;Gad^Lg(eg7e,g9#8&]cw1(Q1GE$^Qc6Wxc4l(`6C1;b$b<UPUw8./7x9.06OQ'DO=j1mteF<[Du`4>W8]$OQFn5"
        "-T$n2Xtb04E5OW1%Mf-5:Bpk$=56u-*m4:AQkoi90n@GHttTJ)-/<Iq9x*9.D`AI$fDp*#6Zd^7joS^+dKsv#1:Zi9bAYl9f[^'+#d<&+>(fs-_W41:-Vxl'>ublKDk;SfF_.k9efFD+"
        "?D6b<EQ$)*ImO1:O4<t_I9X=7ci#Xl44,O-;wHe/CARL2gHwM(gC)H3o6$,MZQqkLr`</:Si)>A#+4N=@7(_17VYU0J28t*=(#n/FQD&(3^GA#5gkc%*mc4#XgAe$L)+&#c_HU&HI)Z#"
        "p#C>$Uq.[#L-Ft$iNX2*7>l+-j75,2sIUw,=m5vA@A`GV9*x;$MqN%b5@t5&:qU0('7Z_+Y%TQAPfjI)VY3i(&^kMMREbF3;<@12jT37%t33-3P=$:AM8)%Mkn8H#exqp.V$nO(0;J-d"
        "1*ct$e0w9.-(;s76WOq&FR<#9Rp(?I5NOxtFlRT%G3DcurDV)+1QA+;fhVr.L5R8#joG3#)MCV$4xw%#@t0NM#Pv;%duMc<B_S2'vCx5'HTP3'Vb'u?gj?2(@+(2LR;[s?:nai:?HF6'"
        "n@bi(hBkP&Rw&-*cYW**V<2PHjV$1cEHg8Al[j*O&/_[QI1M-2U7/T%R&?92uwKa^_t67Khwk'>-+)AX8(sP#C^#g'Mp&Jhl8Lon4w&wundi4?&v@Z5vHt[7#e-H)?kSm&]3d0+99-0+"
        "T[75+OsZ8+kJL?+U8)0(UN6)*gH?k*R;/-#?.2Z#OvuR&g++,Mi:*22iTm%[q,8aHIit&#%(?F3sk_5%=ax9.]77<.]CI8%/&S%`p`3$Ab1?1`9+Rw?0:?l_2B;uYc%6:AiS`I_=4VcH"
        "ttTJ).R$S1X8-?$.Cp*#4)aP_nLZ=lAet5&vS6h*GUhZ+YTDoA0_fx#5Ee<&K3]D%Yjp&=<R_A5:T'G.`oEb3[^+N(jILQ;`afw#*sJ0%[G5^dJubI#B5V$,@Oj.11r/D*MblM(^;xx8"
        ")H]Y#M5M1pW<qoe/87A4[YY#7%i]R&`xK$5PK=203our-37>9@3b12L%>@#$OF0p&/p0r0d-E<['EZ$$a6]S%o:Et.WHOQ)1mxTAli*87p%%i;C4(;A-`AP3HS6/(l`-Clkx^t@IP3*s"
        "CPXL2%'h@1w:Xe)%W1#$KG>c4_O.f$a^+w->;gF4DnSs$+GeC#]CI8%S=S_#Ev-9@p&s@Tit%)uX#aAH8x`^fn(5mDqB3aoh)VvPxvr/UL%Ye#bH$QF+C(g(-4j.$BlX9ZY[(ou%q?<J"
        "5J[61mkuB+T3HZ]WbIAJaR9l'<5^H#8mJW_JcMBLi5YY#CmL1p@_K5A/2r%4idUp07Md^,>jex4GZ'Y[^+l`4b3QG2IM@j'j6[S%C4VV$^v#Z$g+)a<=s4/_LV[s/><;_$#mic<VgYk0"
        "X&@h(v;pj0J<,R&GZGR&:SwM1iURt-]a%)*wW,Cl:<3@5h6k'6p65Cl-1lZcOU&53N`L/bOB*u@@G]v$HJ<j1_ugs7swP,*>-Z<%@jfX-`^D.3Z3Si)Yr0s-0+,d36q5&4fMm9;M[tu("
        "+31N'Y.BP',Fv,@$ZW+6b<A2T_X:/+c?,U%Y#MO9LELh#lL<<7M4&#.D<HIYJqbQL%CZ1bjg:@,]'%%,r`0Dja*AEGJS$##b7N`u_27W$u:q'#elS`7P<Q,,GV^=@:sh*6sP/#6Ac6s."
        "MHQd3P+dxFS<qcaO[-^5;bjj3[<qi'o&c<[6FAn/-f[q/CPkA+qJMH,&_fF2dRMs-&%r[?NXDQ1QO,@51/:.*.1w9.RkC58%D<t.i@4e5T'wUODKXg3F%lE*@J5Yu9oB;(grGsPf8VU%"
        "#M9D3a8OM^^+DJ1ADTrQ4O*j9k8-*3jBV#?\?,ED#*D[55QOLoK_J%u.lOA_kNNuN'WK:0(]6/w#u5'Y&>Hu2v`%a3(l0c^4*12l2DH8e*M?o@#6I$O(Fk.@#kUKb*00He<vw+*=pmZ)*"
        "cS387aFN:8Zr-E5vInv$Ce4w$D/?>-a.b$%oQ7C8PL76>Khv%$ZH7D4HkXoM7Ugv+a<p@-4qW4ST8Z$>b1v@8]H1^#TI0%%f-R:vvp%##E3WM^Wn%/1#)P:v'/###q3;dDR)8S[+i?8%"
        "PfjFr4r*,)1qHD*OF-##mp#7&T_L*4-l]cM7c.)aEf6X1sWbX1Ed``*BYLgL1qb9%MnG&#_A.T%YX[M^Z6m+D.$O`+aE/T(PQp6&%eO1:LHlJ(m,TW7`_aT%9&4a$,XWSA$WQV%ae662"
        "5RPjp?=[<$?XGe)3inW$_:6)%v--@5hu^Glh#%Q:7u],36BwGGT&7'5Y;Hc(B6RL2#^cg$7-0@#Kq*lVf0]>'.r3w7s<2$`(@gSuZ:'#&/SP<:[+csJTc9V(:tKj$lH/a#>6VG>8p;'I"
        "(Cc/Iotvk%/4BP&_C-g:d>BYGi/dT%k%(s-CKK;.2gxA,aRuZ,TjAp.cIFPA]$MmKxMrGaMRA#vqcOV-r60b,.UJ&,CZ4$#GoH8%da&v-/opf)Hr`6&D+dTi^&,W-Ho8*nN80*smbCV>"
        "X=kM(?pNBGEBRL2JQ`Y,4[(`$C>QW-J[HBR[7Do%-*S9AojM?7xPTP;Qr3hDv1_D.NuSC-a:X)^.JWo#W`rMMJJxYhovplBRWV-EX<t3(.*rC$xiL?#K<dL/QN8e$ckn#MbVm%HAro._"
        "Me8T9C-?b#v[s@#D$#%likIQ,L-oT,>PVc<gYx>,:[.X-mc`r?U8-s.jj<./K/-j'9_;3tq9Yj'L,WPAbXYnAlsVSAu=+87`6RM0e1`#ABnS?32JWfECm?['qk?(5k>X-3bkuM(]IR8%"
        "#&t*'rYOl(NX[GEE0^gi,4m<-4G+I)#$:1_<h%qfKsj<XLV3orG3T4DT#D+)XZf;->Z&m%LGoFM1c(g:>2);?9RRM9V_)))^Q]$GCV,U[*b;l(Rv,7&hHJ%#VJXI)xt`D+T]#j'P[&6&"
        "2Oj`<KjTE<I&ch:jn9wg:R=Jr/*5B5kbW#AuCiP-,J1qs`T/S5tc4uuYeWpL,1@4(;>9]?Z<T[#aLIv^[opeVTm=&u]R;W#1o:g%C*4>#-<]M^4qr+;CF/W$X+xs?(xAcatWXR,c;GA#"
        "ZU`8(g(hQANU[S%o%Fe)Y/Ih(<g]##[d[/3QBm]%M80*s'99W-XtGK3%YR_#9-Yp.4*x9.DPLe$L.5I)]Bm1gA/Zs-TT+S@`HxD=),3>#5dl8.-54v,uQXp.J2>rspxNK*5f8v5B+oQW"
        "a#1H+OTlQWi<sK*^,9>,=9PG2ChXc<um`1qqbe1_LV[s/H`c3)3RO$>4)LXoI#+>)VG#j'BG%.2oObGM'+a0(?ErC$:Z[RaN4A;%?*wGGc&s3b@;Ls-'os_PQo>X;Qr3hD9#3a6C,ih$"
        "Ka>mChDBO10mTI#E)k=$Q/ND#ArR*Dx>g[m^2XM^C&mi'=bh/38_J2'2Tf>#Vs5K+[3$u<.0.E-c:?0hFxv@3EK*i2Y7F%$8$VPi4Cb@$D%Qri]nSG*M),##X`AI$<3AnLCo$a7FtuN'"
        ",WAq'L[n,`LZUK(mj7&.CNCjNZSlo&09I#$r4r8.23CHJav+68E@SDX)/n'QBSr]7+0qJ+X>`r?n3b$'o65t?`LJ]&5%<d&@Hu2v?ID:%A[5**JGUs$aHs_tw,Ri(IY>##b-MDljr+@5"
        "`c*i;Y>p,3@PXL26D9W&'8d8.Fax9.v_K#$D4%f(YYgSXqs0LsOHqaup,&-<<(P_H)h8Z7nO7h*^5Yl9GN&_%`Qe<&TFa_tFa.`$IfDc<qDdR&/l#XMH*Go$/j#Xl?v^K-_$s')=)cG3"
        "+TYX-OG6LjeAkh^j8VD*Bf%&dEWmfqSbuu,U'`T4')*)#Q/v2v?^Qq&Dc?&#1=[20IZ5n&bVw-)*/.@'J-k?#i'ut?$77X-F8%l-VvkA#?c;U2U5:[0?7N?#=2Rs?lMR2(GLu2vth7r%"
        "1Npo%YMwh(P-XA#gB#[./+^6&WEVe0q.+F3A:cF3a&<GMO?Zs0@@w8.fqZs0J3>j0we0o08*RL2UcnA#f9'9.$ddC#p$0T%puEb3&_*G4,Y)v#=b$[%6(@s$;TI@#f'MfU7ebOQ`17R#"
        ",P1v#p.k48gv1R:$^M(ur]Jptn@sC9JLH$$2,8v7jjmQ:jBc^$PbQc$9Ep*#e?ib7O?Ea*8H9v-2B5&#_1@;[%8.^#q5&M/q-JB#5qu;/a_4F/^>CB#;-sN9$4jv-jadS';G2W'Z7,_+"
        "njJw#oFXM(,=.i3A$C%>[#3a*8$Wd*VuAb*;L7%#_5#u@*l)K3@,V#AQ:i;-Ha/r-1c;6(q:Sh(`1@J3bkuM(<`(?##Wm$$:o0N(#L0#$^7o]4#]Rs-`p?a*:$/.2J*,N'Zh`?#>]kdJ"
        ".7NJL2Y2%-_)lJ:l_U'#Z`p%kgoB5/;$<J:uusY-O$Bm/k<[5/YF#g)Uwwb%hj'9A`]dcVEf6X17C8X1>*HA4a&(,)SK)#5,F%p.m9/t-TX:gLMEdD-Q^D&.gYi+MUwnU7SGfY,SevY#"
        "=cggM*6UMu-aMa-G*'J)IZ.$v(]qr$7:(n#]WN1#jOvH.%[G5A(Xx%YM3=#dDF+;dw:9p/P/sxuTTMI0awZ5/L&2o&_=PF[iFdQj0P$RjB2(RjNhx:%G3Y&#(dTw0+J<g(jHZY,@3<J:"
        "&w/E+u@`t0n3xx0>W,(1H5AW&Pix;@%R;X0E@%++4PP3+bVQr%sa-.)9]#$&H=KDl'tI&%U80*skn?t@>s'A3[jf;-ZXHA%-m)+&)05]-4X9)3gJD]-YFmY#Hk0N@9n+=KUGa?XOTedn"
        "^u'[NG@VO%o.Ic8oxb@8;aNN/3][20)ds%(rjNaE)YM8Z*5'Zbhe:S#OtH<JYE]dti*;]$_==*#M;%jCBv;/i)'ZEe*A2'#Gf7w-..=v>'8eB#Z7;8.@//[>_d84Vn:gQf'J)=$clou,"
        "WS>(+CS./(K-0s@p_Z,36,Be=.xR_#*8[8/G.,Q'17H9ilsRF4WVGZ519nX^Ux?ZI3DxpU/Hu1^8QHT3>ATX/@ZF(?2[c_&5_(A5(ZXp:?GmnN%A%r9:3lIGc34-)vhYwLNR?#uwqn'&"
        "rND+v.60:%#w^^#[.>>#?M=.#hkc+#Mn%Z#/3$$$]rS.hXH;g*-tji0^?A_%i@5Z$(Y_@-5(wX.X:i-cOYh[-q.aq2pW(*0Ub2#P'SH##.6Q9M_Tfn$va9JrN/)I*t`P>#l)_^#]<&7/"
        ".Qx`N[Th[-#MXwBmc39#H)jYQ9S;qL]-;hL1Bv2vf`,7/4dx`NnE$'v/j:aM5e=vM7J.`aBj@^#O7p<CMG+crtVE4)u1UK-=W_@-,B8F-$f_@-1H8F-tD8F-u^seE1N&:DF),##&kf/C"
        "Tk:;-X?6Q1a+l%,T3kV7[$tE,D9&a+gAE**c)k'.V1kxFk[)[#wGT:'J(K2'3s`d,=#jIq:bp2'Jdo[YGOh/#4Y>>#I2SY,Rt`W'*A+87[KxD5leW#Ahw1o0HWcC#W%;9.tPl()M'Bp."
        "]uQ?l@lOeFx[FWS`jZ)4FF3]-LL`9HBjxW8rXfOFGSO<&tvIK#r&f1$Nio]$f&^vC.]l`74(MV*VJUTRKsv^S.O&iQcjGa+GIpiB^5=W6[[Y)#L5%j:jip2)o1H#-VvkY-=^4l9kPLaF"
        "gPd^#4$qi'_qo2't9f0:q*'d<,0,&#Kh70:TxEm'dGcb5b;*229FRLMa*C0Mr.cu*Dx>dM/N3)YGQ^W:s%T3HPi/&-j1/L4iDPP2H8[1>x39MuI[u>#b:8^H]%/u^7d`v-g>vc*5&1;K"
        "BUON=bXb1poJoe`I4G#%doBB#,.>>##L5+#'mc+#^4KA+^rg._D=vs*TA,tTYZT5#hko]$e?:@-weQX.[ODF5J>C[-HY?X1_&/*'-l,m8]C0Zd8'Y/(:s)(A[M=.#rF/(%fvPjD*aDE-"
        "wG7?8rc,eQYaZfi.m[&#ps(WfJW<78vnHwBL&fwB)Ra5K1b[>G-sR&#8k0h$2.Nc86QQ$$,-]5/QCml/VA'58>'xAGAInJ1/,/b-Rmv8T0ltk8ahL50(Md8.&3)d3vT]wBxoNp#TwH0#"
        "#)>>#d)j>$3)m<-j1%bNk@oY-B/2'oZw_?$/3]5/UXMM0u2)iNK)^fL7o%s$TT;'#2aw8'gt#O(L*Z&#9t0;2`9w^]Fu%em:p^%M1xra-Gsq2;d:Zt7?EoDX:gBA$s,3m.J'89$GL6p-"
        "q([R3uqo8^fFmt7f5Bvd=p9&$%v_@-Lr0@/F+7o#6<t?MsK]c;HlKQ(s7IZ$HSCs-m#BO90I]9&oUH]kk+C5/(bMQ(giv6#,B@W&+Qt6/m>5W.0$Nt&K8=kb$4E9rJgLkbcVDkbq<00."
        "(3`#85+b.)p+PaX%FmU7`h./17E%B#eY/@BoTfwBTtJRaTdRfCDGCs--2^0M9]1xRx+2W-7x(a=+#WE-WpVE-=^SB.#I:;$3Vh;-+n^`$v8$MNG6+/MwVX**X$Ib'9J(##YAkci;@6Al"
        "Um[v$pCF)NJHF/MuMk&+^Fhp7>cBX8)qu'$H)G6#&m)_7Ci>>#/0Hlf&25##2G#i7g9x0:^d0-2M@V^Fx9N<$B$Mb*7nNMaRj/Z#gioi'LU)Z#5cCv#IGm?&;>uv#>W-T@,3N;7mg-a3"
        "leW#A@o?t@m^cn0YmxGGE#jc)x3xF%'=9xbDtNk$iR^OS(K?JLRUYB#T$uFM)L%g:Rvw,<c8)^#-:n0#wWFaa1U^b&KlP?#9[>1%*C=GM?bNt$Q+I8%&+2j915_W$D[gU%uslF#SFAi$"
        "O80*s7/-CJn/Cp.kQEI3UKp'JX(XS@)k/_^^&vjNV[Q:v@'*##*DQj0h<t=cEEo;-w#Eb$'MdP/UtDv#$79x$G_U*'PRSq2u3eci2<JYmV]RV[a9)gLaH7eM7963':`L02Q8u(a]WE5/"
        "vt[q2u0[0#RQ]R0V_(v,Z$5/((/-lL`4DhLI;N+M21dWM?\?f0#x[x>-Z`2Q0>.$G$$)G6#_1DMMATX-$w_^)4`FG>,ONAI]L@B6_>HOQ,frgo.99MxRCD4jL6k+=$AKL@-Vd-x-odovL"
        "B]$R]=xHA#NbxvL)Fx8%Pe3s@(Cq0%.JS'Ap#k$#.KNQ9D>:[#-TB#&]$@>#jr$s$?u:;$6GY>#OA487C?1K(A(m>#FEu2v1/?6&a,,F%[5dX$],2O'Dk4/(@wixFguC`adpd<CtJ@W/"
        "(R#F#H^ehaJ'Y9)5rqr$nFam/Z(xp&=$K6&Ce82'6Sl>#/o#4%sq#@5(^CT(lWcC#l>X2B/$pG3fJYh#5QBN(,&+22rv^8/Qp<I3M1EpLSjQL2wDN/MK9H)4$ddC#D:TZ-V,@S#H:?HY"
        "=1IjumE.[uv>F/%PFMM#IXoPu@VhJuY4NsWSC^LgJb]CWt*,##I>uu#MEp*#7ImU7QDj;Q5Yl##EBuj''HnCsjd%p$aBn>#cL4gLj6'B'Q)m3')K3`skpr8$b?[>#D:?(%SUuu>ke+#%"
        "/v+/CHJpY#_e#&4ognY#:4%[#H$,j'.^nCsxM9/'hmW?#;8l/*%-WT7K&iKEb@mZ#lWVY50WkZ#]G.1(&9^[#50l>)Hw@@#4m3`s+)Hg'm,k?#'pmQJFo1s'X8X39PNc3';T`C$rYO>#"
        "K'4A#VT%H#+pI'06exi'J=2Z#rFB@%ceW#AH%w@3Rn*T.RdW#A;B,s-_ERkLNe=n0C)_j(5>O228Z(H%@r/:)b+61M1+5)N?pfc$f/41)9$gf$G0fX-887T%Ae/##hNpVtf(S4fu;n_s"
        "=3-pu/+tjfj32xtrB1Pf$'9MT1DYhu;_4mud0j<#YU^%Os;e(sBsPM'']FS7VPX&+]?Dh+Dc5/(Z19:@^&Vl'm&mD<>5D,V$kj]k)lhl/VR$$&?J>##qZJ$&cik6U@.Xb3oJpkLx%AX-"
        "wnQ>#o^1T%#h`I33pT^fbI$nu+lsbK@@.^L?5Hp1mSAO#1k;NGvsiKtekArU0Y'>6-,YUMU;6Q/Ake[#q$T*#o<-#v)4dlLi@^;.d9UiBooIr%9g+D+]]sG#UG4b3hJ<1V)KV%$r$/i3"
        "iH+U%7kM<@G&vX.5$q;.5leL*+faX*6YnD*s.T>,k]qs$.A+87H%w@3(5_t@xZK?lci#Xl;8;elwW4V5c7JC#Xq.[#a%NT/>?4l9.#U<hS]WF3vu.&4DX;Uiek>^=Brk<@pXx=TM#oG="
        "1X]Qm^#kKSx4#=B<.E(,fVM'Aaf$RK+eU,57_Pbu)ODuC2rB=/']ZOgx;gkGq&/ht#Vie,5L^C#j.YS84T/tLRx=:0,nQAGlSGv?]IbWqVp1*#_#55(uL,h<Y1Tk19eL$%sWn=9x$#C9"
        "n^PK9Y8`E#A*'79`Hp5:2;SB$>]/g<Osc01Q*ue<k:B+*NMi;7ItO+<-L2s@m33U-6+[i0L%bi26TSF4x=mhMibR_#+Cf1**P/#%dIS:/prk/M1Z1#$jA,n%P59f3C0Tv-iL0+*x`2YB"
        "Fs0gO#3l0=C,9^+8Cf2=qVsF6qZ.*AG,)(6Lu:v#6-cQ#X,@Fu$8'b%=7,wKWr7L#-g2xDkrN_,N*h60clD2:LCxLPB'/I+BG/T%.70@#OiI,77kMk1A&:<0tv$i4ked/f$),##eB46%"
        "8Q[8#(4Vb7W<rD-$3Gt.@).m0#u`v#RpkJ453='8*Ja*8@9v484x.B-.Xq&$@2^6*u0SF<huA+*Dr9mA`fmC#sA'P%,X.?#e]6DElmna+VRD2)q;mg+2FNW-bMq0#`x^t@`hUP(O0&H;"
        "'dTN('?%12?b>M;LMcd3D9p;._<:1;h1Bv-fPfidnuId)n#IKY.Ho?&<WF20Xe:)A)j1dPvVZp8*eZIYS@@QB%7D-FRJDJ3Ivg'?VG%f*KJ<cF1:TlCO$O]RTO2DNE>&_-[%mShte9,-"
        "&ogC4N0*o:8Htr&rlJ]?@j*OD&1lVLplPf1a43q1ST^3#uNN`<cr;)WlTlu5^h&?.Jq6;-WdEi21-4++q'wu#OK*X&W_)X63gqD#Q@HG2A=k2_M`w80GqwWADsUT/T14c</V)^kGWWQ+"
        "I8bP&fFBg1m<9d<`E'a<3J+87DB`7HeODs@$&et7J).m0:?Tp.'?%12'w;Q1+k`[,a@f)GfJ8f3&Q3Z%$aKs7PD3%$b_ce<4i>W^L[q%BThX04Y_c)60qkg;[(-`>th0),OjR-3+ZvA#"
        "nL[*5F08$&AN4YJM.&9(-_;@,(R`d*VR*?5;PfG=$),##IFsH$;R[8#n'Db7_,0Z/'N4L#9Gq@#Ghe3LZ4Tk11M&125adC#qtKZ3N@jC#TK_G3Op'#G+I8]#7x*i*2lN+3)NiW%nZB?#"
        "P.CSIE'Kq%>njp%ZEdnA'K4UA(=(`$`Rr8.0_9>7H(#B46X-F3@MF12X<3@5xpek(JPXL2fHn68-=*h)8E_#$lPp1Ddh_'-Wg1Q#WAG$85ex`,.e<(juNN`<FQQfq^+DJ1a:2H+`.vu-"
        "uN3L#bT&%#?.cN1:4hK2nF-qA4X03_2lBT'c5*U0m6M@23OqC2$lat(?TRk(H;66)#p_h1rj>Z$ZH'?#sit.%/<vPA<L*t$M+I8%A-cf(U6_1(@9lG%K](<-iC)u$:od8.oMF12M(Rp."
        "wOXL2kNhs/>.ue)Fo0N(0ZnB8[IF31&p+KVf(S9Avv@E-5fe3'pVc98Rv3,@rIh8Ii/o5%w)G6#'-xc7NQD8n87n8%S@9U#u=P>#l>S1(u]$eOcAR1(B%%L*>;3d,)qQxkgb.6,JwR9,"
        "=JMc<7')JVgV]#,4V]P'.[ENCH2nbFSv31VoIXbkZo]G3h7eC#8k_j(M[wx#8+;91+A+87T3O=7B$N/2'$^s-_HASBq6-4(ug0o0HOK?l,F&u@7PJS@@8hJ)cK2[%(AC8.UD.&4)(-i,"
        "ZMJx8G/3vQ_t)aQ.0ILgj#;<AXD8714)U.#4b+/(lib##Dmfl/Nj&E5NrUn/1$/=)h(6@#5v%;Q=*O[#jB5/(Y8n-)l1T^+^PKV0rUMd0I^4p%itV?#V59b*Rq]v$72K7.;X4`M7Lxi$"
        "LNo;-RCP[(x^2U@lGxv7H:$vC*'oB(p1@v7+g-p8[]Ej1_Y&7MBRS4vIxD[$$mQiLt'Mc+_;SwLJf6(#NPK#&2wC4-^Y;Ga.X<#-SjXA#pTtM*pXQRAZ?Pj'aA/B+3@Zc*REow#Nl%%#"
        "Dvxj%Vgx;-L=P*:Vp8g2F<(Z[`3pr%AVwdBq4ZW^V7gf?ZZZe5RO4$`&a6R#.P>x'@t(XIohD^4LR(^O;qqu,mnNp96K2_$EN&k9T@R&#2^1^#K?)`-wmpf-][/q-ZvkA#Q60b,YVNc<"
        ",EHQov&fC(7S5v,pVH7&)tACOs?:p$AKX5V?6LoMwEv@3ZhV)Q`'/G(q$vM(h:#/Ch9.w)J8./EslCUAV?=&RHJ.xkeb/n$pLrt-*G$iL&:w(#LnZ,vmP@`$oxK'#SXgQ03wD<@,3`v%"
        "L*QI)x#ZHaBXw:1UhQv&>TgTAb<.E+WC+=$lnk,)Y7;H3*^)22X^Fc/*j*tmkE`3(7UI0Y=4qb*V_fQ/n]B.*sim$$Qj1T%vWLD3vu.&4at*]#drvOkL<jZX;AfQ#lPv_e;q?a=,Ml9J"
        "-r=H4wgxlE>/F^uPV]t>$Q$S[WX1S#AHcP#=#>:1u-I[,2@-uS8whTi%A%?I#mB-d^(O1pZVI5Ax*0JLd9(q/v2'nL)l`K(mTl,;8BQO0Xhs:moj5_#wgdY#^VtU0C:L_00tg>,Lm8;0"
        "1XHS7fMG?e@]%Po%(&5#o>iuPpk_Y#Iqn%#Wl6s$^t:d*TZr0(^[g)*$,V8A#$9;-KD>S.YmxGGd=;n8E(,d3V<$*MshF_$=lh;-@G`o%=xdg#:6/s_+T*vN%JeSjs@(v;88kG@_tFk3"
        "3SFCAV(,@.'Qof:<;*;OwS##h=7ejRM%/BPg,v+FrMDsTOn?e<Z4LfL'BVhLxBd[dM+'&+M*m22GP4KC(2FlKS+d:[^#W]+17lkp/ubn*TQS%#;HPj)I-S@#WIxw>(W$:I*oK`$q-t5&"
        "W]Bf)Q%x8%'Q<H=_2*221pQ;HO2mQU$-'l)dH7g)pI=YBq7ms^2-#Q/$#,PA.&NlA7<BX-_Xhhki8rg%s()g:VNe%FW)(k9`Zql'.d?T,M1KY,_POV7le*#$6<SA,8lY>#Xgoi'@3&Z#"
        "4lqV$:a8-#ea39#m^;3t%e;A+jiNA+JX<5&MbX=$.Mri'wHw@396*@5Z/KkLMsP_$`eW#AhLlw$e/41)-,qM(6$E[5@;7&4*Rte2xUO[ANSd)Q.F/vCRoo`T-Eb-9j11^##lUH-4-4i%"
        "C=:8een]o@%Z<j3K'u2vjw9I*J=>A+w<5:&/F$2V0uG;@'7if3bvjr*a's@#Se[S%&r_8.<@R0)L`hW$DmwJ3bRafL.c.r3G-MD#jLe1AdiE12,&+22tk,@5`2(H/vcMD3Bj18IX2gG3"
        "]-GW%VuDZ'%]R_#a;nW%wT@l1ro%&HS$uWJjYGW@])7I5oe,31nHOfb8AjYCj)$rEW3-ial6Y@M*xk/O<%X/>Y=HN<XiIf*uA35KJZa]5OqO5CKSZg-Cln,Odii^Yp>icF9RtWJ%U,t$"
        "8mAAI'I/c2p'89$Eh%-#'h%x]f`QA#;g&V-<5Do%N%;8.wKCn'G0V(l4m.&$-pPS7Tr1j9<*9D,GS)w4jLsO_Mv._45,Tb4EsEc4.xT71adTa<G-*r7fC0j(Fs=p7a^Vs@<Is>34&)q%"
        "+7E:.'(OW85G3FMT,E.3];+dM+eWh#SI3rM*,ig4]dnl;K_B2H*tC14ndjP2@O8`,X%ki>Z*_@7JL0nCZ7SWA*l3@TU9L79.vN**g+QO,JNJ]ICD/^7saYN']k213G.(=/%DQ,.gtUR1"
        "G[s$##),##o6S,2]WE5//F)##Y%//L4O)##;<PQ#148(#,WjP(gYG1M*FaZu:uCC$7eF?-qubZ-0%,F%ldZ8/x`?9/j1VK$vAa-A/YK/Dx&0R<VB@W&#ETR/U@l_+)exW-W,aIM'fK/D"
        "J2_X1i]qR/I,XC5/*5=-aoV&.hgu/AJ*_^#l)_^#M_ds0NnS-%-4(@-kn+K.Z<fVHdp0B5=5IA4`Pp0)MVNF@ptvG$Ok,T1Mn%Z#g2$$$3UV:v0VV_APQ#6/s0ST$xNVQNZW_@-`i-a$"
        ")[KD3bf-]-&)pwB(.Jh,.OiO%UTaVINs_A,VlAs.BAE^'F_K1p^xxXCIuk`Ni=EMM(2w&6rWN>PQUJg:g<*sIi8Xv$wr@FeV^nXC_D0W%>B&F-,NsP-GN7m-(paXCnosFel2Sj2FDU1T"
        "*ONvG8SO'#A@#+#At(G#quf&?j.CW7xE2L)g*kX$wRDM0.SQp.>[jT.YHbi0bk-a$?FW1T?I6W-Grx&#kiV6hn,.q0=$###I/?x#Ygh^#oBE6'=60t-L##HN@sW^-xDF_/)ap%kbXbwp"
        "3&AwpWaw8'&QNjLrD6##iCtP_psmp-bT1@p@^_PJ^b3Q/LW$$$ud]?7gMp'O)TBa[uN2NX2geiNhO&3OEwnIM(abD)BonH%D;MP-X9jE->Z$/8c+c&#GeXL8<wUPK3l'-+Z4(-QVhO=A"
        "oNFq.G.v%k6Q'-+Hg?+#Jc]@88Z#w-Unxf19F#,MGsai0/SQp.1__[%h/16/r);0$K%IN8Yi;,<VI[9.Zgh^#dhm5'g8e?KrTIPBWk@(8$/^'8kCZM^0p<p/gU9`$BQPQC`&qi'ce)v#"
        "1Sc)mWD_w-?VLpL?HG0N`]_wMInsi0dB3K28T?j'-.46/C@uu#SVCxNal1F$Cm-q$bkNX$#JqW-Mw8=-dnV&.W,u8N9F@O(aa8[6Y#wD-_srA.KFlIh9tJg)p8UhL[Bv2vbVK:.:6@#v"
        "OTVo$khBW0[BQIkw-HU788TkLEPkf2q5sA.]qG2g,&b#$k,>wLs@aO^8P<9@.[gJ)^GFW]WH)>98Kh2:+i?8%9B#q.0W$$$wLS(4J*/r2oC$##*4$YCTTeXC<mn^#0CvXu8PReMnk_@-"
        "J)Vl-Jm1Lc>f>29odZHdemF&5+AnuNBG88#JpGV%7TrP-TSbG9p,sEeT:s5SOLxB.O^2NBVgA8J#JPAGE;k;%xLaG91SlGe8);0$7N^oM*JIP/b7+fqI71Z5#%###EWB[#.TLB#])_^#"
        "p%MB#Wog._mW&-8e+;3;AqOrLuds4%h*Ia.@B&,;GsJ4&n(m/*O%rR/bvV2/pGe^$SX9m_X)Ru%Fq%##soP>l4>T21YF3#.*SjG8:[K/)n)g<-M_Xv-i.auL@P]BNQSMpLPYIU7JQLK("
        "w%#(+uRo('3xV1TE3WM^;SN/D?_<X1gTPQ#`n^O.;2a1^.7]6/ZnYN#[1p(MCN5OMciu*$fNU[-.s$hcWL;uu[fk=$hv,E&_NX1Tm*uoe'8C,3SQ.Z$fsl%06)UR/&`sj1I%<Q&R*X1T"
        "bkF5AISC9rRcCZ#rkEvMHWkJMXBX1^%7t:2/Md;-c%j&.tU'x7k/h^#)*_^#*ks&P=CD+vGx`w$f$j&.du6:NblLN(>&s'4-8:hMM7BZNr);8Iote)6UkvY#$3$$$6fl-Nf3d&'3T2jK"
        "02b#$wuUxLxCaO^Y.kM(^eP2_Pm(*0;#@BI@D&F-UE&F-_>Ab-G1DB%_bQLMMF&F-TB&F-t3dEQ.R7m-ud:Fe-F;Ier=c`sg<*sIkD0W%>B&F-*HsP-ZN7m-(paXC-C2Ie:;rg$?M^oM"
        "*JIP/t+OrZ%>Hs-xox.:315X73ewl/QR?##X*+$#;/j+&<Q,6/03VK$RZsfM:>.5/YN2l89?28]@%$r/RK2#5.rG;-l($/_dL>94h@&w7gX9x9]LO1p^=>kOQg.-#`gd&#8K^58htxs7"
        "R_mY#RQB@#newW-k=DeH)Z_pL5:[##?l%##)]fY5kxp._FmZ^1Yd]CO^,W&.=bFoLP?:@-ZpI#/7n/u]8f>*A:Z1_MSD$LM0QA-^O_P,k;<Y0.^oJgMBEUP8DoGg)E?MMMjUK*^)33E-"
        "=kk0M_pDZ#)7$gL+iZLMaf3)^8vmr``Ma_AR*A5KoHtY6ldV8SIvB_Q@Wsul2M^v$3o7`M^?qKMV6F1^>G@r`;D7-%@'NbNq)Vb$3?):2iPUR/09QxXLKav'92VK$B`3$O/Hm]%e58.M"
        "Ox:*81>+2_6BE*k_^E5/[w$##?=(,Dpi@90m@b]+`I-##S'nm8S(acWRm3W%wT8X1N<TX100bZMPg^Y,qug._GXrp+l%/jicDv2vNQiO%c%j&.C7/880'LB#nvBB#]7Q@PRE_w-IcXoN"
        "cl1F$WX#D/'ECB#beMxPKtO)<`v5'o#L>^Qe'k=c9tJg)7=^kL^Hv2vdSbm0@[8QCMwj0v?Ej)%l>:@-`2jq/UO4#B;v8q[-E:-/#,$k1TuL[0*r?uu1*u2vnoD&.)GriMFd'KM]p9m]"
        "jf&h?],0:N-53ulklOZQ[B)]N:3uo7B^VqM8&cm5aMPqM^Y*#Q,<j;%#d#X$L()^>Tb@/?WZ1.?W2]w%Q_Q']q-q#2o0Kj-C1FF@Jw8A=t,Gg.Wamv2qEc,/e,T)<x*v']^hW20l^sl/"
        "nug._ho1QCEDU1TC6Zfq>W?/DY1;W-n(w924;e/ES72e$:*T1TpUUM^2`/;62_n;-;W9`$3kC_A[a2-&';to7#.89&oUH]kT$]<KVwDZ#Hh;uL>G:q[40uQ#V4o50h`.a<_sOn*3f+>%"
        "aoV&.A/,A81Nkm'dPMG)5$mf?ccXoN_l1F$3j:*%VH;hLxk/>.bIddM[4aINJ$:A=dMDF@2IpE@#`-=-6,0Z$S+&Y%*iv1#,B@W&+0:e?W<3_5Kk-BG,bofU@JUw0uK*##n7'58r$DnE"
        "IZOK_MB2igV:WV&Btji0Vd8]t1I0quZSO&#Kqn%#i0)J)1f0T7_d387u#]Y#'a=sQn`r?#1<P>#8Z4N3_&dC#'Y8K+',FteSL;8.[L(*49a[D*Adu)3eEqB.gol5(SQK02SWtOS*k5oL"
        "G4@]M;oYci<d8/=r_8d<NWb`+Xvp587._CEkmoCad30/(<^0A=Xs6n&O<f@#qRv',?RB+,J_`;$#c=PARrd+V=M6`axQM`+BNsvPiL+0LD10QAeXtGMcNH/AEi[^0:.lUe0PR]6ruKkL"
        "_YAT(c+5hYv1NT/YFq8.QS>c4Df^F*n7o]4UiEX(@+?(j[v-l3])_OChpDeaIxA.<=<AA,*h]-EHn[ciOJJL2EC`x5b?LNMhF+M1gE7g)G_X'#Y##J$amF@N9`>[7aX/m&p2.)*B--n&"
        ".@I*R8MW,VM(*aa^u[h(AnvuPVl./L3=EM-LWai-;[:LY+8;g)I[/N-dI[m/W82#$/35P3GUiq3[&RL4[#C4Cxci.E2U%@,$wF=$aUQL-HNqD0a3/F6ga6l1[.v>@$h:5K2L4/(TCx4A"
        "v3@(+q&-N']]FM(5kk;7gUa#$r.U8.I,-q.E,H8/Lk3TB_#b>37,2u7A8^,3/XRq7<Oj1)NCI8%UaDE4>U^:/vBo8%M/'J3w]sAHuOpX)=&Vu$Q/Vu7p5hR/)Bt#-TEk8K1DdrHInqf)"
        "]*o'&PTs)+)6`($w(C3kCec]uQ31hLX[`'#YnUA$>:;$#7hEmL'F8`a)(IP/bu%6-1>E9-9T`v%b%6#-UV/M($D99*5N<I3=acn0t7A-mwo>F+:,u23/,Q]6?jG0l5Cj9.w[R_#$P'=."
        "u$D.3SwC.3''sR'x$:A(;<dh.btf),6@1q@b=mt7Wmq99aM%@'pTM#,=kqM`b+jr?RYDP0h^K(6E]GW-alQb%Zp1'oZB]cic2:&bM]ou,Yti,+kKbQ&)XW?#oE_I)@&@B46vVl'QfL[#"
        "l2]t@qaj>3R4,T#xG8iLpC*22miWL2b<En%,XJw#a%NT/T.ZB]m1f]4_1C=-&%[6%m2O]FeX=IF>)I,%403J2N#Us7,(/W$:qBk1v$D=6NAg;6)xa&Pow]49K%UxtP.<'5]xA/LBt(+*"
        "^D?-.)SQYQ4)9vN83_KMC?ql%/<UxT;/O&m2QfRRGhXF1R-gX-wD^*+Mi5*89,s<%<@%%#%JIZ7@kr%,([-t.5#%(#5LNT%YBdw-BTS`+xi-['FGba$Oi,E5_5*22iu'5%JnE^614;t-"
        "ia*>PHq598hT,H38LL4fRa3m0wud/1W#/51*Dod)**4b$w_];3wBL$-lZ)w?A]ds.>$KA=[H)58KTm0)E<w.:oH8A,Hi3n0o@l[2uA8N0dCj`kZ3^#$GLCJ)b=bf/@^aI3^0>.3X#/gL"
        "Cl*F341u.d&xEUR'L):8d,V%gFDpDcs0.##AfJE#?$###:LvU7gT[0#G?uf(`'x+2YVjNX#&dF+iUKI*%2%`,Ug'u$S]Dd%x*XIj.K#A5:i]9CSU9P'kh5d3j1<:)c%GU0K,IT%t(8j0"
        "4b+/(^T+8I0^LJ(JJ<-&G-=t$M$x+2]-HD-qW.9%U;<T%mJ+P(b&IA+qYn;/m%>w5Le;FXO4[mCRH_q%UI-e2<;E)#aERPT9RePT8A-S<<@G<q:A7a+H&Z-?2`Nh#RAl-$uQj-$<4m8&"
        "JdZR*Pi68%Hkj_&`?_c)O*k_&']^%OpXQ##fS?2Ne_Z##oFW3NYed##h(T4NZkm##&Tp<N]w)$#8#&?N`3E$#g4`.N`6N$#gk5.69'D_AK5>>#F+dER@%);Q-=`T.9OI@#'C`T.54.[#"
        ":@`T.R2-_#DrG<-N2:W.V>?_#)4xU.=LR[#IA`T.t7;k#[A`T.V/6$$L@`T.Tos5$u@`T.kC8A$B@`T.:s*E$wrG<-3fG<-fDfT%/Fgx=/*cjD')qoD2(rE-js@>B17V.G5E+7Du<AAY"
        "+9IL2CA)m9+^KkE[ZRdF=r7fG'>ZhFwPXaG+4rE-eFI+Hc:@bHOEs%8#?pKF*'FG-k^7l+u:1eGkW?X.(*7L2q').MgHPd2@8OG-6HcfGg8T0F-$]F-;oZXC4j[:C6Rw#J+$%F-w#OnD"
        "8A2eG%?`gD%mfTCCelpL1;YbH,K4;CP+)RNoc>eNLPM6O_dF_.%0W=B);B_->jaE[QwLVC0@.F-<s4VC_0iTC+oL*H:67rL*:<hFX4urL@VqfDaY+8MH_LG-s#;OCxoUEH2m9oD$rvgF"
        "KswrLu&tV$'q^qL,*crLwEUSMvYRSDZW[)Gq@j*%fpR216'98%7-7L2:4//G3Ww<ILeo6BviXj1F>%72,i:90[gER:Meie=A/v?07i8*#w)IqL=-lrLrv%0#B*]-#BYlS.N5C/#8Mem/"
        "Kmk.#cSq/#`_4?-E`.u-pbLxL9c@(#N;#s-_p/kLns&nLXe=rLvRxqLMKG&#lrY<-lW_@-w,:1Mu@]qLnwc.#[;8F-AZ5<-;hDE-#n7^.Z/:/#O,E:/U;L/#tZhpL4OEmL:r%qLE_4rL"
        "N(8qLuJG&#PT2E-iY5<-l@19MD*.A-AX4?-vp9hMlKG&#e5v5MJLG&#vIKC-)V^C-d&@A-dX`=-5XhPN^'qC-uoWB-'R-T-AJsM-.=dD-n'=J-uij#.4MhEQ0(B-#2Cg>-AZ4?-](A>-"
        "vEEU-hg:AM4@LSMLn3_$ZKXw0tXTiBM2F>H:%$x')7pQW`DS?gs]s.C0+4mBujNe$_Kx?0T0cKce4Qq73TYj)PDOJM?W.Q-JND6M-skRMM-lrL6)crLarkRM)<CSMlS=RMBND/#:aErL"
        ";.AqLH.]QMI$/qL>'crLc4JqL$LRi.Nmk.#$W3B-Dj;v7aRH-d,e.@9vF,#HuAr,+]@g-#[F`.Mf#YrL,1%I-n/b$%vFh%F)T]X(X@%t--#krLd3v.#e,KkLhT`/#K3*j1w]8#H*S.R&"
        "^02RN9l`f1Jgi`F?rP/2D^xIN^&@T-nf'S-It,D-LnL1.`Ys3Nb(crLO6fQM^/6*&bHA_/>gv9)L2cxF&_'mBxl=S3g&f-->In<-aBGO--m:9OAD$LMCO#.#?&#lOl(crLUm-+#Zc/*#"
        "bO>+#uJ,W-)5[k=H5YY#vC58.EJSfCBeGJ$#=n0#Su[fL6Ss%$,]UV$0u68%47no%8ONP&<h/2'@*gi'DBGJ(HZ(,)Ls_c)P5@D*TMw%+XfW]+](9>,a@pu,r7b[-iq18.m3io.qKIP/"
        "ud*20*d/-GM+E5#KA*1#gC17#MGd;#8(02#L-d3#saiO#]m&M#U9w0#T.j<#O#'2#DcjL#]_^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#"
        "qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#TmD<#;Uu>#4`1?#8lC?#,px=#0%V?#@.i?#"
        "3/PY#5:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#;QJ=#?HbA#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#.H-(#.YQC#5ddC#9pvC#=&3D#A2ED#E>WD#IJjD#LMa)#@3QV#"
        "Qc8E#UoJE#Y%^E#^1pE#b=,F#fI>F#jUPF#xIf5#&dcF#rnuF#v$2G#$1DG#(=VG#xdf=#Z0?;#eJiG#0U%H#4b7H#8nIH#<$]H#@0oH#D<+I#HH=I#LTOI#PabI#TmtI#N@:R#Z):J#"
        "^,1/#HKvV#>m%P#eGhJ#iS$K#m`6K#pc-0#4'?V#&NB:#3[T:#3C0:#-$[K##/nK#';*L#+G<L#/SNL#3`aL#7lsL#;x/M#?.BM#C:TM#GFgM#KR#N#O_5N#SkGN#WwYN#[-mN#`9)O#"
        "dE;O#hQMO#l^`O#pjrO#tv.P#x,AP#&9SP#*EfP#.QxP#2^4Q#6jFQ#9m=6#x%cQ#@2uQ#D>1R#HJCR#LVUR#PchR#To$S#X%7S#]1IS#a=[S#eInS#iU*T#mb<T#qnNT#u$bT##1tT#"
        "'=0U#+IBU#/UTU#3bgU#7n#V#;$6V#?0HV#C<ZV#GHmV#KT)W#Oa;W#SmMW#W#aW#[/sW#`;/X#dGAX#hSSX#l`fX#plxX#tx4Y#x.GY#'>YY#*GlY#.S(Z#2`:Z#6lLZ#:x_Z#>.rZ#"
        "B:.[#FF@[#JRR[#N_e[#Rkw[#Vw3]#Z-F]#_9X]#cEk]#gQ'^#k^9^#ojK^#sv^^#w,q^#TUx5#Z:-_#ZnF6#L+Ro#Yb46#Iu$8#ib_7#V,85#qLH_#fH:7#jF$(#hL-(#Pfm_#7p)`#"
        ";&<`#;qs1#S3N`#C>a`#E8<)#O?E)#Wk>3#d_V4#R'Z3#Z.92#MRp2#tW/a#OcAa#SoSa#W%ga#[1#b#`=5b#dIGb#hUYb#lblb#pn(c#t$;c#x0Mc#&=`c#*Irc#.U.d#2b@d#6nRd#"
        ":$fd#>0xd#B<4e#FHFe#JTXe#Nake#Rm'f#V#:f#Z/Lf#_;_f#cGqf#gS-g#k`?g#olQg#sxdg#w.wg#%;3h#)GEh#-SWh#1`jh#5l&i#9x8i#=.Ki#A:^i#EFpi#IR,j#M_>j#QkPj#"
        "Uwcj#Y-vj#^92k#bEDk#fQVk#j^ik#nj%l#rv7l#v,Jl#$9]l#(Eol#,Q+m#0^=m#4jOm#8vbm#<,um#@81n#DDCn#HPUn#L]hn#Pi$o#Tu6o#X+Io#]7[o#aCno#eO*p#i[<p#mhNp#"
        "qtap#u*tp##70q#'CBq#+OTq#/[gq#3h#r#7t5r#;*Hr#?6Zr#CBmr#GN)s#KZ;s#OgMs#Ss`s#W)ss#[5/t#`AAt#dMSt#hYft#lfxt#pr4u#t(Gu#x4Yu#'J1;$*M(v#.Y:v#2fLv#"
        "6r_v#:(rv#>4.w#B@@w#FLRw#JXew#Neww#Rq3x#V'Fx#Z3Xx#_?kx#cK'#$gW9#$kdK#$op^#$s&q#$w2-$$%?\?$$)KQ$$-Wd$$1dv$$5p2%$9&E%$=2W%$A>j%$EJ&&$IV8&$McJ&$"
        "Qo]&$U%p&$Y1,'$^=>'$bIP'$fUc'$jbu'$nn1($r$D($v0V($$=i($(I%)$,U7)$0bI)$4n[)$8$o)$<0+*$@<=*$DHO*$HTb*$Lat*$Pm0+$T#C+$X/U+$];h+$aG$,$eS6,$i`H,$"
        "mlZ,$qxm,$u.*-$#;<-$'GN-$+Sa-$/`s-$3l/.$7xA.$;.T.$O,xEF#6hC%KGRR27)0w%6G2eG1J7fGm.1VC(3f:C$]eFHpg`9C:<U=B,SvLFnTJF%J43TB,ZkVCkE&+%YW?dF#HxUC"
        "'6emMPKRQ8q;fuB.QBkEpPgjB7O?\?$Y_nFHg-3I$IOjpBwXp_-oT/+%cMtdG@HnrL0S5W-X@gwB6M3nMAqZ293PKv$4W5W-35]*77?C6MLV;KC3Id;%3fLd-raJF%k3fjM0;oQMGKjj9"
        "]0*T&@/dW-1M=b725IqLKasj9&5H1F-Cm`--^U%'w#C-O<w@HD7O?\?$LGcdGl<3I$e]fBHnWrTCs^&+%pN4kMki7dD,7Hv$t'#k-s^&+%pN4kMTi7dD0[`8&4^3f/f%Sb#LDig:;x(T&"
        "C/dW-C#YFRHk(.P<2BeGti5lEl,4nD:o&:B+)YVCvg&+%(QHeObEQLOTpoG;u:ViF3mWRMEN]G;PLl>$7E#W-<l=F7ww>F7Nj_73IkcLOOmO)<]vIv$QbU&4oX,-GBCffG&'AUCA/W7D"
        "b:X7DT7Q-v@49oMR5lD<)`I'I-)tRM`uUE<,mX7D/o;2F0%OGH8/%A'w#lOM9_#ZG0k@p&Bc1d-8)V%'w#lOM9_#ZGx;o`=RWIv$BW5W-R^Ja5w.mvG:$Ap&drE^-:/V%'$9ClM-@LSM"
        "RP6#>^sKG$F/7m/]@poD`?b'%DW5W-(0V'fFXpoMQVd>>H;Ov$J8.Q/Y4^oDtFoVH/q[5']::S-RAvW-jGZDuq()WH1e%T&rcdh-;/;`&2*:`&Q/2`&)T_PMTRR5J.X`8&asaZ%QEDmN"
        "q2x`E1D`*H(=?kOPqA5B4Om;%@_lt(iiRTC5-ST&_#N=B::c_&Gg1`&:3Ux7%v@p&M/Q<-1Rj+2%XvLFxV>0CuAFVCMmESMKqhp7uW)=B>Tft%,(oFHo,=nDF3**Gj<0eQ='3W%-o6.-"
        ".ZV4NYVLE<%judGOP$.-0)*JOxA:E<8wHnMBN=eDwo)SE8m4REWO8QCE?5j9lmV=Bx':oDIA^f3A+Gc-t.4O+8):kF]/.PD(%_>$R?E]$[7`.G8o&:BdkY<B?50g#AX>;?FFHXLfm$lD"
        "P%PhFL;*7DIS/#>2mO)N)%l`<;'bP8q#/d<AudtCwVsr&eb]eQV8(%0:t`8&+bGT%@4gKOJN>gL8GqS-Y:7^M,xIXH2F5ojb#j>$WBL[-E72njnunTCmaXB%pGj>$X`i=-x=pV-QRZ6j"
        "oS/Z$dMOR-u4b$Nw7GW8aC1g#&#3^FEaBp.q0QhFR[2njT*ZGMm]>;H:#pwK:iErL_t%qL^]>D/k@oUC93'q7&94m'/H=p&Ev<kbGn61,3eV8&sP<C&sxnpCW;TL2@_NIN_,<i2]pA,3"
        "[+nL,iKU:)`IE(&aLE(&bOE(&cRE(&cO<(&3bNeN85jE-f;/b-akaR<B]P@9B]P@9B]P@9C`P@9C`P@9R=GLYH87:VMeaR<H87:VWjqe?Wjqe?Wjqe?Xmqe?Xmqe?Xmqe?faq&$]0jeN"
        "c1jeNc1jeNc1jeNc1jeNc1jeNd7seNd7seNd7seNd7seNd7seN=p@r%du+xBdu+xBdu+xBdu+xBB?'(S3b0r23b0r23b0r23b0r2CB'(S9=mkO9=mkO9=mkO]#re?]#re?]#re?]#re?"
        "]#re?^&re?Itcwph+,xBh+,xBh+,xBh+,xBh+,xBh+,xBh+,xBh+,xBi.,xBi.,xBi.,xBi.,xBi.,xBi.,xBi.,xBi.,xBIuY@9a'3g2`;/b-)?]L,=.3:87[aJ2a'3g2`;/b-pcxwB"
        "-I3IN_bQL2a'3g2oAil-qi+xBal+xBa7D)4?W>'5ZICeGgvb'%Jv1T.`lqaHVPPHMU%av>-S?W-4a>(&)WqlMj^$TMlNq2B78RR2C,#(5#P]:C<QCj0dM`='C=oFH,`I'IBw0NCe(-oD"
        "A?081$B#hFep4KCxHM=BbT)60.HXcH*g^oD'xnw9MP^f30l0VC'*A>BThmfGNf.u-T:E:8DCc'&GP0W6HvA+47[I*H%bV=BkWT$&wd%UC&>.FHu[QSD77l_&,]cgC@qu_&F@brLn%1oM"
        "C<oc$XORFHp3SMFE8CN2b_3N0?<0$J-j#lE+@:6Mufd/C.KfUC/;J:(u:ViFJJC<->x@7&?P7`&F3oFHcgap&:GViF]Fdp&6=ofGu^0hP:*8T&UPpPN5'MHM.dPHMR6e=.$6xUC9XnKY"
        ".9IL2f7pGMcoZL2qk+x'Y%RF%Z(RF%[+RF%].RF%^1RF%_4RF%`7RF%hORF%f@7F%nf`T9mfYs-$Hq/Nhj&3MYjdh2%g?F%=H0NMl&EI3,]ow'@WBNMd]cHM$&87960@R*#.bw9@agjM"
        "6-pGMA+SR9Wm3r0-'@u%q`goD+S?pD&GpGMCMP<81TcD<1];MFwgae$xn`e$AtWe$F$)4=>43u7n.,F%F-WGDvA$`%#Q]YB1=kCIw+$d3$2-d3j(Gc-'Q-Ra4hE.Ne0N.NctmLMA.ae3"
        "s;il-BAOqM_>#d3GUeD4k7pGMhZ`=-f4m<-_Y#<-`Y#<-hY#<-4/Pp7O%,d3q%,G-&f`=-m`>W-nrQF%WuQF%+W#a4+W#a4+W#a4+W#a4+W#a4+W#a4+W#a4+W#a4+W#a4+W#a4+W#a4"
        "+W#a4+W#a4-gG&5rX:d-puQF%,a>&5,a>&5,a>&5,a>&5,a>&5$H>&5$H>&5$H>&5$H>&5$H>&5$H>&5$H>&5%QYA5kvXA5kvXA5kvXA5kvXA5kvXA5@N^A5s8YA5s8YA5s8YA5s8YA5"
        "s8YA5s8YA5s8YA5&2<s7/xBo-#vQF%5]3s75]3s75]3s75]3s75]3s75]3s75]3s75]3s75]3s75]3s75]3s75]3s75]3s77lW88rX:d-$vQF%6fN886fN886fN886fN886fN886fN88"
        "6fN886fN886fN88QFh&QlkV:86fN886fN888usS8rX:d-%vQF%7ojS87ojS87ojS87ojS87ojS87ojS87ojS87ojS87ojS87ojS8cFoS87ojS87ojS89(9p8?@H#9Wxxc3iU2W-LAjwK"
        "8AjwK8AjwK8AjwK8AjwK8AjwK8AjwK8AjwK8AjwK8AjwK8AjwK8AjwK8AjwK9DjwK@$]hMA7,0Ne8,0N97,0N97,0N97,0N97,0N97,0N97,0N97,0N97,0N97,0N97,0N97,0N97,0N"
        "%xhx$Fx,k294gP9:4gP9:4gP9:4gP9:4gP9:4gP9:4gP9:4gP9:4gP9:4gP9l+]dXY,ae3:.iH-`'W?9AXjJ2UT^s$@PbQ1p<xR9Fi4R*)KK)4$4.e-8T`wK/CRhM)%wL2)KK)4$4.e-"
        "8T`wK/CRhM)%wL2)KK)4O8gr-8T`wK/CRhM)%wL2U)Y)4V6nQs2Qnh26`N-v/@@LM:v7798f'gLPZiiLPZiiLUvIJM.^cHM&-pGM&-pGM&-pGM&-pGM&-pGM&-pGM&-pGM&-pGM&-pGM"
        "&-pGMZv7798f'gLm$SR9BJJ88b,q2VHH7+N51Uh$fLhwK8f'gL&kv^8YwZw'b,q2V@nC*N51Uh$fLhwK,M-W-''o2V8=P)N7+p/N51Uh$nehwK8f'gL&kv^8YwZw'b,q2V@nC*N51Uh$"
        "fLhwK8f'gL&kv^8pa/g2P0Uh$^4hwK8f'gLNrL*%8f'gLNrL*%8f'gLNrL*%8f'gLNrL*%8f'gLNrL*%8f'gLNrL*%8f'gLNrL*%,M-W-''o2V8=P)Na2Uh$;(X-v8f'gLQoXl8YwZw'"
        "7Zu2V@nC*Na2Uh$;(X-v8f'gLQoXl8pa/g2P0Uh$Sa&g2WM#Q8IwZw'mCl&Q@nC*N@'of$)6=wpBUJR91+#H3SFo'=fI46B+)YVCq&sXBK*U/DGgaSMGsW'OEBNhF^n=D/;x^kEW<qs."
        "l7FVC&ujX:.@4%9Gn?L,6X/1,l%Q2(oTV,M8XcUCY1Jh:=f#gDBMVX-]<`Y(*H2*#'4-_#*DYY#&D(v#_V$@'*tj-$H@gq),.L#$ZA;=-9=pgL=w8g1K-4&#hOQ`Ehpf.#](ofL0rJfL"
        "WxSfLeZ7DE<?*:)tx*GMCq.>->?f>-w@pV-=Z&@'e*@>#Z#s8$iD@@%b%mN%nrO:&s*Jp&7?*T'pjlj'aI3t($$9U)5*]=+ShZW,.as=.<?-F%-SSqL5_4rLKcc?0k3570B+ST%-6C3:"
        "9QCEF56*9ob9T'#C/,3q]Isj_oiqjhs@EX.ggbkC/L*t-qrJ@m4WZ_u1gOL#wJ9r4iS>8.42FlUYG_A55I26M3d1p.+'CEFK([L#k/S;JMnh50pnQ8N)=;e7U++r/9#iP/64JKD<_AEF"
        "2(5F%5E($N$Q4H=464Q'fI35M,pVE4RnDL#33GBMEQ+Z-N?,kblLie$T8eT8@FHx8A^mw7E(KV-;(#d2:i'9o_e&-j-G_jLRx-+5d6^-HVaBEF,T8o:O(F1N[^n=/v8?%e9]wUphmw1j"
        "+*p>&-X1pqPLGK+seIWT#H/3s7ZvWK^PH3a9scX0NSIKDbS.F%Z4&4WNxq3j'0HKD1kFR`K4:l*we6x8p`m-$^+TLOB;lqh.s^:1KdwQW^Q?.P)Bf:CI,sl3_l_x]Y(H;C0iXSVaLdSr"
        "PLc/cH&umNfYoH?\?]Rb.@THbRh&obn+YEJ-dD+>UA;Y>(OqkJ-1C#2u756W)HRhJH$8#?1pVj&0Y#33#'MJK-,vnd.7cr>qI.CEF,jI-.i3DEFiN;?_PDDEFf'8q3PZ^35Mwpd[;u2W)"
        ":i'9oKI,<.2xW$W?Q`S%xbXwUD`r)3^tJ_p)5Nb.(,U>(QPEbR)IaSrO3>*R>X#.$1+b8S'L:9Mk[jbncbt-$`u[>->:0.$S/+)XV],ITP2G80:?i&0>90EAXSFX(1&,0M%_E3MBh(eM"
        "/`-2M@;V9.VdLK-JJ6L,KDgvH#*]/M34a3M^K>LNq/or:OVfVRC]Jj-W6Yw9Z>]q2.8x?T5JCHN:95dMNj+EA@gp?gN5((fZbg?TM^Es%.A558jG5^Z*-xT.$#D3:.licW(huo.l@>oA"
        "?,jL#Q'NX0c#B,2vJSS.Sm+/1.;R]4Yt4>5]i?^5pmlu5g1R>6d.nY6m61Q9.Kai9g$C2:mNHj:M@7A=wG4&>vV0#?:]hY?'5Qv?>J@S@O=niL;^5/MMCNJMNLjfMOU/,NP_JGNU*5dN"
        "d@>YPuP%#QYZ?>QZdZYQi7-/Vu=%&Y^S?AYtEX]YuNtxYvW9>ZwaTYZ(Kio])T.5^*^IP^+gel^,p*2_.,bi_3Y>Gaj^w(b6u:Db3#u%c:CR]c=_NYdSW0;eA-greC?GSfIv?MhK2w.i"
        "MDWfiOV8GjQio(kaE]oo?Pv1qoVHjq],qi'NiVJ(1EK/)3W,g)FL>W.G#95/]]LE4mN^]4q`vx4``UA5'ir]5^f6#6/M3-v]:2mL<'L&v`RUpL2@]qL:qOrLZKCsL*kqsLIv-tLd,@tL"
        "W]3uLYiEuL[uWuLY+kuLX0ho-A/1F%rn9R*d81F%cso-$e#p-$I$d-612DiTTi1F%-GOV6KU.R<&=_(NcUI_&LsHwK1fI_&HaRk+=Lbw'crsA#M3^gLK1#Gi2U:GjP`ScjHNr(kRr4Dk"
        "gK&8obNx4poo.Dt(OP`tS*h%uv@+AuwIF]u2_H$v9l5###0S>#wjwl/,]@;?-lIpAS>+)E6t1)FK&f]GOVKvH7ebiKVLRMLX_3/MNLjfMXwJGNRq+)OT-c`OcdCAPmC-/V=H>AYciZ]Y"
        "2^28]4pio])T.5^6,JP^75fl^0&+2_1/FM_28bi_3A'/`Yk_f`F*kudJZP8f/pJciZ#<GjQio(kT.l%l8NJYm#Qu1qk2Uiqi86JrlS2Gs#,NcsLXk(ts%/Dtt.J`t'iX&uT3sV$k,T8%"
        ",b%9&KG6m&,nnP'>ZO2(3K9N(m;D)*EP2H*;[#a*DS`D+AP%a+O=+B,I_:v,?osY-A+T;.C=5s.EOlS/GbL50It-m0K0eM1MBE/2OT&g2Qg]G3T,YD451<&5XPq]5ZcQ>6]u2v6_1jV7"
        "aCJ88cU+p8d_F59i%+m9i6$j:saZJ;mZ;,<omrc<q)SD=s;4&>uMk]>w`K>?#s,v?%/dV@'AD8A)S%pA80i`F#N3&Go4KAG?We]G@a*#HOip?HOevu#wX7PJ@d>PJQ`1F%#B05/gDKM0"
        "P^A,3Rpxc3^fha4KGrx44_9>5au?^5dR4;6V',)OrfWVR_2srR`;88SaDSSSbMooSd`OPTeiklT8@22Ug%LMUh.hiUi7-/Vi/5>dVs,8fRr4DkT.l%lV@L]l]kQ>mKMZM'0<0j(FL>W."
        ":U;5/N?n21,iMG2P^A,3Rpxc3XD(E4a4&#5m7r]5/4tiCZ53)F:BIAG42+2_3A'/`0>BJ`w1/Dt;7g%u-I-_u)@;##u_SPAJB62Bj7c`EKoUPKMCNJMOU/,NQhfcNS$GDOU6(&Ps<=AY"
        "IGnr[+NMS])T.5^3)fl^-#FM_1G^f`U71Alh$ixldaXlpfs9MqlG?/rTC+d)9,2H*/AA&+l9_M:j?\?/;lQvf;ndVG<pv7)=r2o`=tDOA>vV0#?xigY?$&H;@&8)s@b'4kOW#])v9r$qL"
        ".(8qL04JqL2@]qL4LoqL6X+rL8e=rL5rOrLkNV+v*ZI%#-pC&MtJKYGC]=S[$H75^d7bKle/0qidPS?gi/p-$H1A(]?k#:)9*]?^B+3F%Biq-$V>)L>#_j-$%ej-$/-k-$L91b+_/[iB"
        ",[_7[^mj-$ZEL1p(#l-$P;l-$RAl-$Y*Z(N:r-F%DbFxb_Uv?0lmJs-';8vLoIBvL`OKvLaUTvLb[^vLdhpvLen#wLft,wLg$6wLh*?wLLVFo$V4g4fZ45DkT.l%lV@L]l]kQ>mut[M'"
        "0<0j(2NgJ)cMEW.%l:5/J'I21ZpaJ2P^A,3Rpxc3T,YD4;Y?&m<#OW#FA;=-_eF?-0O#<-/7T;-8hG<-9Nx>-q7T;-Qt9S-c2jm.0R=m#-YlS.rtrs#*6T;-56T;-G6T;-M6T;-O6T;-"
        "Q6T;-S6T;-YZlS.T]sh#)O#<-'7T;-1hG<-+7T;--7T;-17T;-fB;=-W7T;-d7T;-j[lS.])89$7YlS.nd8.$;M#<-h5T;-j5T;-l5T;-n5T;-p5T;-r5T;-t5T;-v5T;-x5T;-$6T;-"
        "&6T;-(6T;-*6T;-,6T;-.6T;-06T;-26T;-46T;-Wa+p7XZ`d*<WL1p5Jl-$WPl-$YVl-$ZYl-$SuN?pBgk-$Dmk-$VMl-$XSl-$:On-$r)c?T<Z2ed?_%2^/sel^-#FM_1G^f`aGuoo"
        "JC.F%q.'@BbAnfL?T#vGRWp#6QO)20^mj-$*tj-$E2gFi(#l-$P;l-$RAl-$oKu9))rm-$Z5H_&^do-$_go-$`jo-$amo-$bpo-$dvo-$<Vq-$Biq-$RCr-$TIr-$VOr-$YqmFic#k-$"
        "00k-$/.n-$6Cn-$,&q-$4JlFiUU)20apj-$0bF_&5@n-$Gwn-$M3o-$O9o-$Q?o-$SEo-$UKo-$%gp-$'mp-$ULr-$WRr-$dwr-$f's-$iHnFil8k-$7Ek-$h+m-$j1m-$l7m-$n=m-$"
        "pCm-$rIm-$tOm-$vUm-$x[m-$$cm-$&im-$<`4'#(,rvu%$:hLBJ=jLl<?a$Bu8>5aiq]5CQbs-f-vlLS,@tLL2ItLM8RtLN>[tLODetLPJntLQPwtL:vWuL]%buLY+kuLZ1tuLi0HwL"
        "%]1%M<fC%M:rU%MaVZ)Meo)*MAS09v5Z0#v;PNjL/:+%vI(<%vdtYlLY*elLd6wlL^B3mLbZWmL(YVpL2@]qL:qOrLBtVxL45P$MsU/+MubA+MI:H:v?oIfL9fbjL%/moL;wXrLI9B1v"
        "w]e#M+mx#M-#5$M1;Y$MM9X'MdEk'M,*r6v*hewu4vQiLA,fiL?8xiLAD4jLCPFjLE]XjLGikjLIu'kLK+:kLM7LkLOC_kLQOqkL74fr%@iH>Gr'*vG.6>B5fYIV6^mj-$+f)20xo>iT"
        "e,k-$HSD_&_^@L,BmMY5t(/;6hrKW/O,@tLa2ItLM8RtLN>[tLODetLPJntL(sk-v_'GwLZY1%M@fC%MkIJ4v2od(#HfGlLX$[lLbZWmL4YVpL2@]qLtsVxLoU/+M+cA+MI7->#glIfL"
        "Qed##EYXgLs.V#v-,loL;wXrLM9X'MOEk'MQQ'(M;xRiL=,fiL?8xiLAD4jLCPFjLE]XjLGikjLIu'kLK+:kLM7LkLOC_kLQOqkLe-m.#.oNrL<'crLduli8`P,Qq2pt-v)#v=-#dWo8"
        "pQ#po$ms19_)VjVp#gO&x`xl8<o]jMP-lP*+tT@--6mi83IxJj+[NR8q7^mBnP>iT^mj-$0^#O9h?\?vZwdFxbPoGp7vpcj`hvNW#amt=-0%EF&h]V40:r4:v<(G:vB*cj`?F*k#v.2k#"
        "$vVE9Ku*/i0^8c$Zw5;6JKQ+-bs*29)N'*4fvK`3e,k-$._[9Kh0eQ%[xgFivtp5+DPb%OZ8h+VH60.=n;q-%VFS'-4LX%vg0vlL^B3mL/;)Q&ONe7[x]Q1pUNj-$PQXs-;FqY;G#C#$"
        "Y8V[-:5*XA`Z`P8Lp)'$9$3$#4/UL&iKAW-;1qWf*l>;-%n/A=^mj-$Fp>iTpH;3Vx_dq;5GrE4G6Y]4&r^l8D1CM9fq'm9g$C2:i6$j:QxBd<#gp%=q)SD=(Wo`=wG4&>xPOA>#Zk]>"
        "%mK>?*86Z?ALfr?$&H;@%/dV@&8)s@K1niLe-4/MMCNJMNLjfMOU/,NP_JGNT$,dN74Vc;NG*,W7hEGWne`cWon%)Xpw@DXq*]`Xr3x%Ys<=AYtEX]YuNtxYvW9>ZwaTYZ'BMS]Ommo]"
        "-a.5^*^IP^+gel^,p*2_.,bi_/5'/`H1CJ`1G^f`2P#,a3Y>Gau*%)b6u:DbOqV`b81r%c9:7Ac:CR]c;Lnxc<U3>dUQOYdC'0;eA-greB6,8fOdGSfM,@MhK2w.iMDWfiOV8GjQio(k"
        "%P+4M]bvuu^E=gL(XPgL]HjvuL0rvu=&:hL03DhL99MhL6?VhL7E`hLk;>xu[pQiLA,fiL>2oiLG8xiLD>+jLED4jLsil&#6NNjLUh?lL*uQlL_H<mLbZWmLwaamLi/BnL(YVpL/.AqL"
        "B@]qL:X+rL>qOrLkKCsLLWUsLckqsLMv-tL[,@tLQ8RtL`]3uL^iEuL`uWuLY+kuLZ1tuLi=0vLbC9vLdOKvLgbgvLen#wLj6QwLAuVxL7GdT%9Ymud/sYQsK+N'MSEk'MPKt'M,R'(M"
        "RW0(MgJH)Mb]d)Mccm)Mp[8+M,iJ+MJ@Q:vmtRfLTbQuu,@YuurTc##ZYXgLs.V#v0+loLx4voL(YVpLE``pL7FfqL5RxqL<Wuk-%EG_&OjG_&LH0F%NN0F%(0?4DQ>[tL[DetLXJntL"
        "UPwtLVV*uLHc[m->;H_&UKo-$cf`w'Z,IG)^CI_&%gp-$2+bw'+;2F%,>2F%=';R*2iI_&/G2F%0J2F%1M2F%2P2F%5Y2F%j1l--N7r-$G&8^#TO#<-Q7T;-.6SD<@-u1qrA:Mq=VViq"
        "l;q.rmD6JrqiMcss%/Dt#]X&uI5U5&c64m&,nnP'6BO2(3W,g)4aG,*EP2H*RJw`*?>D)+8/`D+Go[A,<Sw],=]<#-?osY-@x8v-A+T;.B4pV.C=5s.DFP8/EOlS/FX1p/GbL50HkhP0"
        "It-m0J'I21K0eM1L9*j1MBE/2NKaJ2OT&g2P^A,3Qg]G3Rpxc3T,YD4V>:&5XPq]5ZcQ>6]u2v6_1jV7aCJ88cU+p8h-_M:>e(j:nK?/;oTZJ;p^vf;qg;,<rpVG<s#sc<t,8)=u5SD="
        "v>o`=wG4&>xPOA>#Zk]>$d0#?%mK>?&vgY?')-v?(2H;@);dV@*D)s@+MD8A,V`SA-`%pA34+QB9$;/C/4tiC1FTJD3X5,E5klcE7'MDF99.&G[Vf]GGDK?H`Z78.`AfFie,k-$=.ML3"
        "8Cu9#e4;<-p&Kk&3*Q1pX@qh&9pLH;6xP&#mhIn&E4RW-c?o3ioaamLQNf3'YDU`<2l>)=bYl;-vM#<-=Lx>-xM#<-u5T;-w5T;-W)m<-L6T;-M6T;-N6T;-O6T;-SQP8.1J<1#B^,>-"
        "6Mx>-m6T;-n6T;-o6T;-p6T;-QugY>E(NS];0mo]@'T=-Gs.>-07T;-17T;-27T;-ZfF?-77T;-87T;-97T;-:7T;-;7T;-5[A]>qM,8fclr(?P?@'(%'fFi]gj-$(nj-$,$k-$b^x;-"
        "<4`T.Ps6S#6M#<-7M#<-?M#<-=5T;-?5T;-A5T;-r@;=-c5T;-qfG<-,N#<-3N#<-6N#<-:N#<-FN#<-HN#<-KN#<-K6T;-6OZZ>I8<;RokKYA&tVxLcguw$=,$&MvCkV&/4&t-.Lb-9"
        "Mb7abAjGxbY[m-$<#8R*s:v;-=gG<-KA;=-KN#<-Yp8gLGGYcMSb/,N]-KGNUtfcNV',)OuPL@IZoNuL-HA#M6NJ#M+TS#M,Z]#MQ9X'MN?b'MOEk'MPKt'MQQ'(MTdB(Msu)b&q;dw'"
        "<au92kN4F%lQ4F%UAMxbamj-$*tj-$4TfFiq`,F%;j,F%<m,F%;Qk-$<Tk-$=Wk-$?^k-$@ak-$Adk-$Bgk-$Cjk-$Dmk-$Epk-$Fsk-$Gvk-$H#l-$I&l-$J)l-$K,l-$L/l-$M2l-$"
        "N5l-$O8l-$P;l-$Q>l-$RAl-$%FO>-=:7I-7eF?-nM#<-oM#<-pM#<-qM#<-rM#<-sM#<-tM#<-uM#<-vM#<-wM#<-xM#<-#N#<-$N#<-%N#<-&N#<-'N#<-(N#<-)N#<-*N#<-+N#<-"
        ",N#<--N#<-/N#<--6T;-/6T;-16T;-36T;-?;eM0j8':#hCP##lMOgLYBsvuf#:hLBJ=jL@F(&(sWu9)exl-$f%m-$g(m-$)^M1pN@m-$pCm-$'_^w'vn.F%wq.F%xt.F%#x.F%%(/F%"
        "&im-$+`7Y-B.1@e.CdwLWImwLnNvwLoT)xLpZ2xLqa;xLrgDxLsmMxLtsVxLu#axLv)jxLw/sxL'TS#M;[]#M-af#M*go#M+mx#M,s+$M.)>$M//G$MH5P$M1;Y$M2Ac$M6Y1%MG`:%M"
        "8fC%M9lL%M:rU%M;x_%M<(i%MU.r%MC:.&MAF@&MBLI&MORR&MMw3'MK-F'MM9X'MOEk'MQQ'(McfWn'::K1p]gj-$1F8G5]Bsvu:':hL03DhL=9MhL6?VhL7E`hLt@x$(sp6R*v#7R*"
        "crl-$3iHs-:(@qLB@]qLBX+rLFKCsLHWUsLKjqsLO,@tLM8RtL^iEuL[uWuLY+kuL]=0vL`OKvLcbgvL3^C6=T,CLjlZPrdOdGSfL;<JiOV8GjRr4Dk_3&8obNx4p<Ae,XfqPe#AGOe#"
        "%J^q#w5T;-&ZlS.rtrs#-N#<-36T;-=gG<-?N#<-KN#<-LN#<-QN#<-RN#<-SN#<-TN#<-UN#<-6.q>>0LCAP-Hmr[6j28]+NMS],Wio]QPWfiNMs+jUfBwKPFs'MUQ'(MTdB(Mu'-X&"
        "q;dw'<au92kN4F%lQ4F%jKnFiamj-$*tj-$IhK1pq`,F%;j,F%<m,F%;Qk-$c</`?G>+jLID4jLBJ=jLCPFjLDVOjLE]XjLFcbjLGikjLHotjLIu'kLJ%1kLK+:kLL1CkLM7LkLN=UkL"
        "OC_kLPIhkLQOqkLRU$lLTb6lLVnHlLX$[lLZ0nlL]<*mL_H<mLaTNmLcaamLh)9nL>1BnLn5KnLo;TnLpA^nLqGgnLrMpnLsS#oLtY,oLu`5oLvf>oLwlGoLxrPoL##ZoL$)doL%/moL"
        "&5voL';)pL(A2pL)G;pL*MDpL+SMpL,YVpL-``pL/lrpL1x.qL/.AqL1:SqL3FfqL5RxqLd*)+vH'Bi>h.-e4qmsmL*t&nLg#0nLq`5oLslGoL.+Gx+8Hj--sMp-$tPp-$uSp-$vVp-$"
        "wYp-$(pp-$)sp-$*vp-$+#q-$,&q-$.,q-$HIIs-_1-&MIF@&M)u/k$^t$JhSJw.iMDWfiOV8GjQio(kQKjp^),ni$G7XJ(WoOM96L]oo<x6W-@&u4MFKCsL`jqsLK,@tLUiEuLWuWuL"
        "Y+kuL]=0vL`OKvLcbgvLen#wLDrCE%K_J_&L1r-$O:r-$RCr-$_hr-$vIS1pj0S<-5)m<-jtgY>@#rlKZl&qB8)ks#HB+W-gQ&qB8)ks#LZOW-gQ&qB8)ks#IKFs-vaA(MRIV:'*(1Jq"
        "EIKfqR0]K<>A09$s&89$P7_M90QfoolU'`$l<YA4go:&5XPq]5ZcQ>6]u2v6_1jV7aCJ88cU+p87el2r9209$-K0W-Lfl2r9209$-K0W-Lfl2r9209$-K0W-Lfl2r9209$-K0W-Lfl2r"
        "9209$-K0W-Lfl2r9209$.TKs-a6rJ:nCPPTt[Nn/Fb;<#WsP3#e5T;-f5T;-?+8)<>R7ab8TGxb]^^w'vn.F%wq.F%xt.F%LPb7[Z'/F%%(iFi=`a-6h8Gs-o(HtLj8RtLN>[tLODetL"
        "PJntLHlPd$ut/;?Q:*,W7hEGWne`cWon%)Xpw@DXq*]`Xr3x%Ys<=AYtEX]YuNtxYvW9>ZwaTYZ'BMS]GTmo]-a.5^*^IP^+gel^,p*2_.,bi_/5'/`H1CJ`1G^f`2P#,a3Y>Ga#7%)b"
        "6u:DbOqV`b81r%c9:7Ac:CR]c;Lnxcpg7ji:+I3#GhG<-dkF58jsbPT1qJ_&K.r-$M4r-$O:r-$Q@r-$sS1U.Cs6S#&5T;-JiF58x)uPT%v>iTrss;-A3C5%.h&)<v8HvZGT`7[FlPd$"
        "BN')<v8HvZJ)M1p:#9c$%cll8K5&j:HUaSAFrE-X:1eN#9iHs-hPTsLRkqsL^v-tLx,@tLQ8RtL`]3uL^iEuL`uWuLY+kuL]=0vL`OKvLcbgvLen#wL00wa$):IJ:E)cPT8v;R*L1r-$"
        "O:r-$8`'F7Vh3F%_hr-$bqr-$<rg7[i*K1pV<96/n@`(vF1uoL.4Bk$Lk))EAE;dEk@(&F`cf]GcV(k`<=*k#V)m<-S+s^%-L/9B'.ia8GubPTfL3R3qEi--Yp0F%5%Kk4F?f;-m1a'%"
        "E,u._Fubi_MDWfig@t+jv1T9VPFs'MUQ'(MTdB(MMns?'Mn@QL<:*k#ePGs-;Fr*MQV/+MK:6:vsnCvuOdbgL,qugLLB*R8&pOPT$>Qs-auQiLT&]iL=,fiL?8xiL@>+jLAD4jLBJ=jL"
        "CPFjLDVOjLE]XjLFcbjLGikjLHotjLIu'kLJ%1kLK+:kLL1CkLM7LkLN=UkLOC_kLPIhkLQOqkLRU$lLTb6lLVnHlLX$[lLZ0nlL]<*mL_H<mLaTNmLcaamLat?q$ic(Qq'.ia8+kOPT"
        ")Bt;-N)[6%ic(Qq'.ia8+kOPT)Bt;-N)[6%ic(Qq'.ia8+kOPT/#m5/#+f)v&x-qL/.AqL1:SqL3FfqL5RxqL7_4rLf6;+v^wWrLhH`+vAkR#''K_K:1.1#AK1Bk$KrN1p_Wb5MT+35&"
        "T)B;$*Sv;-54X/%`PZj:B6-Z?F=p.Ck`#K)q-J_&suIG)6Jl-$WPl-$I'o-$SEo-$ZZo-$^do-$P=r-$Q@r-$pEs-$rKs-$[0LG)VQj-$?x:J:N#2)FR@RMLL:3/MT-c`OtEX]Y*^IP^"
        ",p*2_.,bi_m]Mcsoo.DtuC4&uHEgM'.*O2(6s(d*99.&G;Ke]G=^E>HJ=(Z$w#0<-qp@#/Yl1$#BBH-dS@X-d%O45&%p/?nPRvu#6(%kk@AN1pQ>M1pK<IXhmw&@'i.m-$l8p-$m;p-$"
        "n>p-$oAp-$pDp-$qGp-$7Gq-$9Mq-$r;.L5J-)B#^Pc)M7';P-*gG<-R[lS.E)89$<5T;-@5T;-B5T;-D5T;-F5T;-H5T;-J5T;-L5T;-N5T;-P5T;-Shj3M'c5oL5%e+#.1#O-'+#O-"
        "4oXM>I8_M:ocYD4Y*[D=.m(&YVjAq^P0&,;Oip/)gtoc)@Q[Ks,9[';Uj:_$@0)M%I?jFi06o-$P<o-$RBo-$VNo-$&jp-$ur$LNho)*Mk%<*MDrh]&5WfFiqGk-$i.m-$k4m-$m:m-$"
        "o@m-$$nQ#$wM#<-u5T;-w5T;-#6T;-%6T;-'6T;-C*MP-),.e%W(8[6nlIV6RJ1A=d4]xXSSa9BuviM/x<.wuQ3LhLOAPL:?6=Jir-QDkZ1:/:V#rlKVejfMP_JGNRq+)OV?CAP&928]"
        "(Kio]T.l%lejt1qg&Uiqj>?JrtLs+&</D_&8Hk-$i.m-$k4m-$m:m-$o@m-$Ze'kk6_kY>)#L>?#s,v?%/dV@'AD8A)S%pA9g*$$njiFixdn-$VL.20-CrT.10L+vGH`t-2is]BYNF6'"
        "c?ki'0UUY,D:^v-cv35/)djP0NB*N19a;MBWx<Z6Rj<J:Do1#?&,6Z?U3gr?);dV@O=niL/tEWSnd?.)dB5R3$It9gUHUh(=E8=#AMU.0p#M1pL@@iT-E[Qs]x-F%h@ku5t3ji-B1.F%"
        "UGXm;j<D+vWo,tLp]3uLZ1tuL^C9vLmHmwLQ2<Q&a#%:)Q@r-$oBs-$NrNk4um4F%vp4F%ws4F%;+T1pkA>iTSmeFif`E/).F]kMM&7tLP2ItLo.Fn'e.x9)2Sv;-jpgjB8uIP^;M4m^"
        "dDt._1/FM_28bi_OtZB7UV=`s1c/DtuC4&ub@nM'.*O2(;2vg)*'dD*oM%a*`Y9jBZt0&G=XqAG3gbYG@a*#HE,k>H6r#;QkxL88'h(99pVto7s0aYQ&F<<-+VKD&DUt&Q$WW(vcAi(v"
        "+2(pLSc/)vuFl5,&o.u-CJ'%MJ+@/*=H1@-0X@+%f&ol8utpGj/N3fh;Zo-$^do-$%r[q.-bp6vw2=KN0KeV#kY,W-+3=KNsP./9W`8Gj/hgO0*'e,v>9*-v]/HtLXdlX(g,xl8T2@Mh"
        "v3O9.]?Kq#kY,W-x5=KN'n$r#*1Su-)S.+MN/6:v=qtgL.'2hLPq<(&aK3hk5lFrLWM%f$h2+m8_7,HjheZV$>`65&,k[5'%(6292@4w$=PL8%3EK5&-_rS&BrW>-FU&X-$=w,=tck3&"
        "fGMSC@H`M%dbO1p2Bo-$UR8bdp$3Y*1eWs-HejS9'`bKNqHmwL->s8MT>F'm$0:DMnq^:D'3>,kVLG&#n@jJ+^k)_.0pUvuXE$5(l`]Csv>^?TB[FxbSMf--4no<-o@;=-u1MDEn&xmi"
        "pPJ(s7g.#,1mE;-@x8v-8%T@-DX_@-M`>KEq3O88EkfT.@b;<#[5T;-^viZ+QZjFiJhfO*p6qeF^B-Y<.LR]c]+.aE,i1_AxZRIGN?K1v;GlfA()hY?Cde]GMDWfiNMs+jOV8GjP`Scj"
        "U+>)k0^[>,@cq;-AM#<-?5T;-@5T;-A5T;-B5T;-UCK0%I>-F%Fsk-$&<ALaoQgH<::eM1anr;-tgV2%S]-F%[7.%JUU$lLTdr'#%hc+#Ejq7#1%Vp.m=N)#dWRIGpwqH'K9KT.eKC7#"
        ")dQX.-8':#R5T;-C3jm.,e0'#:>EY.rF.%#*KXa>[d$N:`9OY>gSdR(T@i]&vv+R<sd^u>Vrs&#/W_R#]r]Z.pmKB#JlL5/oOI@#Hl`s>ePJAGpk-sIvXXS*thWY>t>*ZQ:=)8R0MXVR"
        "EA?YS#ImwL4vVxL7dhH%/*_-?Jtbw'tgGE<dPfoo,(p_'nM.+Mr^8+MsdA+MvhJ+MwnS+ML:Q:v5?0[Dm-NP/03wa$(Y(5]Fj`.&=';R*+;lFigF2F%dnQ-Q1M2F%2P2F%5Y2F%$]Y&."
        "[.AH<iM</iPZ>J(uMgV$TVUV$qK85&Sv&v#,o0^#3rY<-.rY<-P(xU.15,'vHXBs&b0%,aJI6,aFw)fb_Bsvuv#:hLqluxut#g&v9k%nLg#0nL>Hh*#Y$AnLu`5oLslGoLLG<,#T[CxL"
        "wmMxLtsVxLu#axLv)jxLw/sxL(Z]#M)af#M*go#M+mx#M,s+$M.)>$M=.r%M?:.&MAF@&MCRR&MIw3'MK-F'MM9X'MOEk'M(tq6vWq'hL/-;hL19MhL3E`hLBKCsLGjqsLK,@tLUiEuL"
        "WuWuLY+kuL]=0vL`OKvLcbgvLen#wLo^#`-1U3F%O:r-$RCr-$_hr-$4OdCsj`IV6f92YPe>,F%.B,F%2;AfLF$kl&#JOk+3:n-$H$o-$N6o-$P<o-$RBo-$VNo-$&jp-$(pp-$TIr-$"
        "e$s-$g*s-$3edCsamj-$*tj-$4<k-$8Hk-$TGl-$VMl-$XSl-$ZYl-$]`l-$_fl-$all-$crl-$i.m-$k4m-$m:m-$o@m-$qFm-$sLm-$uRm-$wXm-$#`m-$%fm-$'lm-$&Y6-vgt@jM"
        ":q2Y&(A=Lj9BW#&*S)A@eJUF24A8(=sne8)Gt7'&<UVuP?U=`W>fl-$4/2A=I1m-$&1LV6K7m-$u-F_&n=m-$iq`._t7O(sLvHoeZfp-$lD%:)4Hu92>JD5&M2dw'&PTk+co120$;7R3"
        "wVl=lj<m--qHs-$C_8A=TNj-$vTj-$Lk/A=gEVuPx@:kFxh@J1odPY5-Zt58_:SD=p=q58o]O8p(unCPM[k'o4jDG)1Bt;-XeJ:&,++T.jb;<#)ZlS.E9':#PRUW$P6,n/4kVW#v@BU#"
        "D5T;-]fG<-ZM#<-[M#<-]M#<-:6T;-QgG<-S6T;-Z6T;-^6T;-j6T;-DZ`=-TO#<-SJ>8I0ZLa6&P%_#Y,$_#1`9NAA;c`O)nbAYRiNYY-a.5^.jIP^/sel^0&+2_1/FM_28bi_5S^f`"
        "%bcGcPEHL%:ZfFi+qK1pum/F%=q/F%>t/F%?w/F%@$0F%i3_9MOoY)m7%NhDe-b-2Wpk;-85,d$leI=-?QMD&gI.X-#:E.+#9[w5L:;?#3:8G.[L<>-m1vl&&eu*<?YT<TwEsvu%vT#v"
        "LKE&.^qYgD]0h-kw<m6HnW>J(gS(p%H-#krhQZ3Bn'l`627+%v3%<%v4']='uYr]$$CGS@i<CeERs]aHE.r%MU9/u%W=,PfRC0o2Fm3<#Y:<v'CUK1pf,k-$wZX6))46o$SE.#,r,E;-"
        "@x8v-Z%8o8iYu`4hxUA5aBB#&HT@A%Ycw9):?Sw9Wj0F%JfAB=+Rfooq]Uh$IoY`EPLHJVP1,2_[e(/`)p#oDkQof$4/Y`ElhkA#B)l?-tI5s-]RwHB=#sR8t/:6VlHhjEjZfooOjmL%"
        "X`w9)v0J<-TI'_F0Qfoo>')`$j.>AOA`#<gZoNuLJ;Kc$<C)5]xKU+?9,ks#E0PG-,7T;-%<i`E1Rfoo:,O9'io)*Md')`$M:`]F/K]oo9BlT.s&89$l+=2BF$H,*M9,Hl<d7iL$_bk-"
        "*G$g:GFB]&9209$pACs-r>>7CD7A]&9209$o8(W-80A]&9209$pACs-D5uoLG;)pLP7iW$N@i]FAPfooP7iW$L-)a*5bFV.oS#7$?ZlS.p8':#n(d,>eNCG)-Kp`.<vI-#nSlr$J5o*F"
        "-I7>GrpNYG2dkuGr-NVHA,^VIY*gcNwo*ZQ?9&v#S^Ij%T.O1p9PEPAYQ>2C0=9/D5klcE99.&Gv2X`b4I5/iVRb-6L#`w'E30F%&g:X1$w,/(w:F)FIdl34CrFMB4);<-Z@f>-?6T;-"
        "SHJF-<G76%cqh--OjG_&x'd7[0Q0F%i-i--^ow9)RZ0F%S^0F%P<o-$RBo-$#wibQ[%buL^+kuL[7'vLm=0vL8D9vL-JBvLhOKvLj[^vLkbgvL&ipvLKFn/v+:Puuq<FGMZsOrLIwXrL"
        "AE:sLm)#A#geF?-F6T;-G6T;-I6T;-J6T;-qB(xM@]:U#+%_B#dA;=-eA;=-YN#<-ZN#<-X6T;-bgG<-_N#<-`N#<-]6T;-fgG<-cN#<-t55dMDP:U#tM.U.A9':#ter6(JLGc<6Z&Hu"
        "TLk`-v=_78&#x.i-GtW)MC&30aT-,vP@g.vIMkL:qZC#$LF0i*:vgW-rJkL:m`$EP'[:U#,lVW#:6T;-;6T;-`jF58nK]oo(f`poP<P1ph'K1p$nI,X-]al*&xBjK0dc]S'eXW8t'v*v"
        ";pZ,vW>mr7eh9o8q5TTAb4FMB%C_fCkOxfD:*2)FCsE#HlBQP8%_&gMZ_td6U&7tLaUT^$L*RP8*TlDOuljxO)dDAP[T_]P#^%#QDkJQ8)_&gMV<Qk+@Q^5/Vb,3#iS]vLobgvLELQa$"
        "1&p@tgI8o8j1L^#nt6v#%.Z;%Z%8o8Zd4?.J>k8=&3Pw5c[p(.ejHXH1WRAGMHCk$G,DN0bZ6,vq]N1#V':`J#YC#$39TU-@D1]&1Ep.vjCAvL7&pc$h$=MTA)FM92S_3BQBMTLBQ5_d"
        "PlrpLXW>*vn-1::@e^VI?)+Yf)/V#vvM#vAFwgI;='&Z$w?N<-&lab.*rB'#]a5w*97Wc*/N'd*GYdT.FJxP#KO6i$-2%9.^S#7$]:F=2-S4X/E>+jLmmEg;tsD(=n1wU7TQN.;o)xU7"
        "7PjoobZm]m>m3<#%-H>+<B/b*xP#X-jQvU7v_foocZm]m,h-ek';'w%;,MQ&GtOK:&]w.iJ#-Y-NO)V7akooom2_w'>MN1psH8'/^E5'vjo9LNY*Cx;?1V`bu`5W-jo9LNY*Cx;?1V`b"
        "tVp;-ms@+%1]@LNg:f4$sPg;-ms@+%1]@LNg:f4$cl2QBbAwiBh(E2C27XMC3@tiC4I9/D5RTJD6[pfD7e5,E8nPGE9wlcE:*2)F;3MDFgQUP'B1ns-p6C69Ikt^-;D*j$c#429O5]D4"
        "f:C.G]ub7[vUN1pmZ/F%A'0F%Aen-$KEjFi2?o-$VmjFijw<_&5'6G6F&^vn5&>KIlg'^#L-c@-e$,)'.hiT@-$n9^wBu9#CR0S#_O+g$wOk`*fZbT.[t6S#@?pV-*<kwR7O#n*Ki[a<"
        "k`&gM>ow9)O9o-$P<o-$9tGG)gMP.;Y3$:TVTpP#,U3R%-WB_/c0uE@riNV6JcH_&1DHs-1S3q*BYee;&oK.;'MC#$_M/V#u@i'#Wi?p.M9.wu';<.bJ=eN#jABU#eQ/W-jcSFcVmpP#"
        "dt6S#8lVW#CkEs-li#tLhc>d$Fs>A=+Tv]md(/S#N0+W-8,sqK>=eN#O'mN#M'f;-7?Dl%m8g;-UmeB&26>A=?7/9&[3iFiU0=(&pC@A=WkE>H,>G;IYu^VIY65dNh*GD*L=_,MXMCsL"
        "6;G0.hdpsLR&7tLN>[tLODetLnRZL-U+bn%JS-_SpC@A=,Sfoov>pK'u`_Rq@24<#>+rvu89.wuY.OJ-q4OJ-QGuG-TE=-=fVZ)M^xNW#UlqA&#G')NsP:U#1-$L-(Ar9(W&7^=cfbx%"
        "h>9$&p#8kO4aZ'o:XcYQg.6.'*tOFYcEqLp$eNr@4Xv)Fe3t1Ka@3/(OO:87N1u[>Qc&q'*//S#JI/q':/,d$lDxFluGHA'.e.q'05C.va9OO'qL[]=xlXPTrd33&Y4rm8MkOPTOf#q@"
        "Ysd68OgiM0XZN1#SPu>#dJb-9-`L&PLb-;Qa8a;R$OKG)w2^oofK8W-f_4j`7vNW#<#&<?QYVq2:rv29_]ooo,gs?9amo-$bpo-$cso-$]7e3=5h<SRqRVP&3E,<-s#jE-g@FV.0%_B#"
        "[N#<-g)m<-^6T;-.F/)vPFZY#]N0Z$1/8l4jCUk4berW_6,$<-g#)t-Wh_w*kc,<-_jkv-P$*H?J8V`bf?_/ij:Ixb<iw9)xug;-nbe2.Z>dtL]JntLI.X3&UPkxO%WDAPXQ$#QYZ?>Q"
        "ZdZYQk8QpK9`:%M9./S#l)m<-b6T;-39_M9D:i`b8RH9iNv55&D+j9V?5d3=Gwn-$NUT'AJ_e;-ppH]%g4x9)ZZo-$hXYSSJr.r%=uu29CNi'8f0Ie-J*o-$8-s?9f?&-XZQ02'3w)-M"
        "Xw-tLOJ+_.?HjD#<3_hMj,/S#hHi6&Z)QT.,PI@#V6T;-]h[u/nl1$#d5>##u1RA-B40W$`)Z78?IHpKQRd>#3U.L5T7.20u8HG)(7[bmhce`*pmV,M4QL2#PtE-;eLqKGDXgsLqBf>-"
        "dND6M=sa:Md>9C-]N#<-KVEr$Q=A;%*.)#P3g-/v[8S_O`)`oIt$@8JGcUPK,?[k=KKZfLVejfMB5.3)#O:U#f)m<-r+)m$];8vLGTNj$cqw1TlU)?Px:B_-jNpE@t>so719w.ii[2W-"
        "X=u>@u4`B#LV^C-o?l`%aW)9@[jwVo@]QiT_X@&5u[Wt-N[XJEi/'dQ^lF?%]grj`a(asBTuO)#Ajq7#Gs6S#GSWL#tZlS.WBBU#4O#<-9t:T.&mVW#Ms.>-rg^fF;#OW#dB;=-n5kn/"
        "dT=m#E&Ro#)5T;-RBRi.tJ^q#MgG<-F6T;-I6T;-ZA;=-ON#<-PN#<-T6T;-3YkV.T]sh#.B;=--O#<-.O#<-$BV)Ec`&gM0M,F.OP,F.Aq#:)KjRk+4>q-$5Aq-$6Dq-$Rg;R*LI3F%"
        "I(r-$bKK_&l9s-$Cvu92rd4F%sg4F%tj4F%um4F%8Im--d`MxbX#fFi^dj-$3jC_&BRK1pjJ,F%W0<r7dCi`bs-+EP^B&wuMB>n%/ORb*R$l<1[lVW#d'mN#TSWL#x6T;-(/B;HRo=po"
        "QHKxb+tbw'@%mFi)UJ_&Afq-$s*Yo@4Q$:)LbJ_&Erq-$Fuq-$Gxq-$H%r-$bVSk+NO3F%K.r-$mR-F.QX3F%N7r-$F3&r'd,%:)TIr-$ULr-$VOr-$WRr-$XUr-$YXr-$Z[r-$[_r-$"
        "]br-$GJ-_SM$Ww9iaK_&f?4F%>rg7[RvQV6V^i9Dxi%:)j3s-$l9s-$n?s-$k,l<-C<v1%=GN1p>MCW$?4DDEUPb`Ek?u`*)W#<-ewDG#)gkh#'hO(&Sm`cM1+#)NuvKGNUtfcNV',)O"
        "W0GDOX9c`O,g)&PZKCAPl`88SlxOPTj@HJVne`cWpw@DXtEX]YvW9>ZxjpuZ$'QV[%0mr[N]38]OfNS],Wio]wC2T.L(Ro#.7T;-T-`*%:i2F%8Jq-$kJf7[^%?YGww2F%Kwbw'oKt92"
        "t-Yo@/eJ_&HBr8TN-F'M<%jw*/TLu-ds](Mj3$)M+WZ)Mccm)Mdiv)Mqo)*M<v2*Mk%<*Ml+E*Mv/`liWC6Yu?0E##$%?v$&7vV%/q.t%7SQ5&b-oP&tDtX?QLkM(@m0j(B)hJ)Rt>h%"
        ";K%a*8-Fb-(s0.$U3S>-?e?8AP[?#-6g0<-Pc,W%6kf(aE##Akb[1AlQ`b;%Y56T.)urs#J6=b*9[3c*+D$<-TMx>-67T;-r-$;'qu2*MN,E*MSXH:viZRwu,twwulLl>#%IJF-wrX?-"
        "t6T;-FGk7/i7':#dCCtM8]U%k^@Z9MWtf(Mo'C4NH,cwR:Sb=CUL<n29,ks#L#`EN]mL%M4Gw=NJxfB&DC4_JK&T,M8&$0&_b4e;HxQ3bknIp.Yb)20]<E+E&.E^6+K(A7QE1x$<BY)<"
        "tK$d3sSm20P$_(NOfkFiZcp-$=KQ1pc=2W-)0qW/NTpP#b7)W-S#(KEe1Jo#kB+N9rUvvZ%9jFiT$d7[--o-$M]LW-RY8q'(eUq#c]sh#S9_M9Y1-vZFE5R3b$t92:i2F%>]q-$XAR1p"
        "6<cw'QqJ_&FACs-9M%+M*]8+MrhJ+M[4H:vJSGP&`v1j';5iJ(=M)6/o)4xuuqUeN8TER%-:/@gJKZfL1d8d9)YoooR8#LG8lvoo</^3Fd%7eH7fmooRrXo$AfmooJAfn$FOU?gdVacM"
        "[.ks#gte2.%*p(MtCj*M#Q&+Mp[8+MDV.Q--ce2.GF4gL?mlgLm&V&2nd8.$i7':#42)0v=T1xL;JuD%#Hps-wI<O8Hp4)FPP5v/FBBU#G%_B#O9_M9g2B0i<5f7[28R1p]FJG)?.j(Z"
        "eLd%&f>0/:ZD3/i&1(F74@kjDpDhj*L+es-BU*rLx$?x%1gJGD4P3/iHePk+4[*W-=G[K<$XUq#Vx7ID0D3/iN%('?N?Y[$e2XM9O7w.iqdO<-11w.%?Z0203qJ_&S*+cEG4u1q7r%?n"
        "#UUq#vsj;-7$j]&G)G:v*ETrg^<jvu;'1hL[Kj$#<C_hLOp&o8qfB#$[0Js-Fj?iLi[4=2(8xKjut,x%$:n1^%Dh+`dZ/G`b[`(aH04'cW*5Vd1oWrdJp6SeMQgreBhN-=OWpP#MhG<-"
        "F7T;-G7T;-qK?/:pC3/i4wJ_&#kt92QX3F%N7r-$W-K_&RCr-$b#6Ks-43Yl[UhxlY[HYmZedum]wDVn^*arn_3&8oaE]oobNx4p5&?PpqPh2qjX=fq4nY+rrYQfrlS2Gsnfi(tt>UT."
        ">S=m#DM.U.s[sh#JN#<-T)m<-N6T;-P6T;-R6T;-V6T;-tY`=-d6T;-n6T;-tZlS.M.$_#$O#<-x6T;-$7T;-FMx>-(7T;-=Z`=-+7T;--7T;-V0C29T>r%cfSH8f]0ri9P?*/i`aZp-"
        ";d$:)K.r-$lZJs-2Bj'MxLt'M<R'(MXdB(M;g(P-3Wuk-NmK_&g*s-$U@.3MQI?uu^nCvuddbgLZK/wuPgYQU%bq6$DL.U.g5?$$<M#<-Q'xU.FQr,#v?\?kW6sCiT[^P1p[:OV6P$_(N"
        "m%d&Z:vNW#+50N%H*<M9xwbPTF%l--]Tcw'-VHs-R2,)Ms%<*M;tmi,`ck*vbQZ&%4FKMK?Xda*6@;b*4X3<-Pf)R:uU^w%Z,5<-P75)%SgSJ:7l[abFPf7[sY020%lq-$6b*W-ht,e4"
        "GU629?4``bG1iE%,u)20i/k-$TPu%&=LC&+.K%^+Jr@&,UfAqBwGiB#DBBU#^#sc;q]3/i%uG?nXjK(MD?[7v%>r5/(3w,vC)?tL`1AeI2G*/i[f4b$,1[i^Q:GM_0>BJ`Nv6d;?>*/i"
        "@Dpc$@;&8e(Kp'H$XUq#O%J?(dw;YuYM>s$oh(W%/'x5'Nc2j'5jcG*8uHs-]`[`*-r&?-95j#M+<Dc`7f>Ga'aI&#?p&vHi7-/V$$Dh()Jf3X;R3j972`jrDRoV%:cbb-.Fjr;f_&Q_"
        "&hRn(`,?d;Zwa@@dT%)EjmJ30cm'(#Y2)0vJUD4#k[M4#wR1xLP605#,N_*)%nj;-Q5S>-87T;-k(l?-I7T;-^hG<-rGc)3'A4I#A/$_#)urs#VYe[#o=;_;u/72LSIniLM'7#)33=m8"
        "HM>^Z7;Y$M`Gl$M6]mX$7?1AbB[R]c`%a&d(XUq#U*m<-aU]F-O7T;-v'(m8`8X/iQjnFidvj-$[vhKEJ;F0&e^oW-X6g@[3'2hLRN?u)+Nr7eME6Qq*H]YMDhd##EYXgL7eXRMAd5m#"
        "gv%j4r27l$@qe;-]akR-XGVe0D(@4vZ.I4v`t^%MbE6P.TT=m#xerp(C(+vZd56*&wF$S8QxnvRq3.5/?+ET.nlVW#pKB[%GTg(a?(?GaA:v(b<=r%c;Lnxc4>?<-MgG<-I6T;-K6T;-"
        "k6T;-pN9wL>W*Z%pr,W-%R^-6BGH&Maq*'M&9-)M^D?)Mj7W*MlCj*MocZ*&FuR2'x34j'3K9N(k<cD*=PIa+G`>G;b)(X-473uHQWsZoX5<j$wK.F>/L:U#6uHs-:vf(MA]*hBps'^#"
        "O*&d*X?BW-&gS&Z8Pw<23w[KuphDW-8IN:I-ql]>:o<d.DBBU#n6T;-tZlS.glVW#WlK>>[9i`br-J_&cdD=9+QTYG3'trHq_@T%@Yf6'QGCs-61$&Mj9X'MtEk'MQQ'(Mt.rv'RtgD7"
        "i-cW8KxU@G1d2dW(<DYY$w,vZHI'@R9#OW#JhG<-^gRU.rT=m#Vqx#+k'ks-Y+loLKwXrLx:ni$_xdL'Th_7[Eo]'HSgP2$L9f;-i4/L6-%Qx%)Ql+?,:]5/c>:3vfNCR>uC.lB].-EP"
        "Q7jF9MQ-EPrLJo#V(Ro#ET=m#sHh29E^V`b%.Yl4SZ:oC-IRq.E)<Y/2u6S#]lVW#fM,W-q]r2M/o;]J:Ac$M15s7#XAn4vuWG@&wArlDKgEb$eE5;?qMJYmj=hum_4'#Ae+/S##4(D."
        "9&_B#]TOW-wkvX/%^OL#Y'<b*I0e<?ksU@GEX0/Fe8oO?.O``bQ%kOs/la7)Y,IG)UqNV(mr'5]_m@P]8l(],L/k--nS&F7*lk;?::i`bTJe-6S,Sk+?`q-$3Zk;-C`3[0D-^ulbmQSL"
        "m1Jo##H,W-tnq[Go^PSL3n9I?5$r[G-4s'miK%,$#H,W-uqq[G1=_dOui1;(LFI;$Gj/F>^g`7[jCDiTkF4^=R+d,a+8`xbK>U]=I4``bihlo$2%1PfX#1R8`(YPTe[:1&>Ow@k(LdYf"
        "EVZU(ZAHQCf+/S#KafA@CZAQC9;%I%dVX*v+_b*v3tWrLJdhsLLp$tLR>[tLPJntLRV*uLVoNuL0VM:%n`:sKa*]^&Bgj--(pp-$s/K<-a1oj%qMut-s;a'MlEk'M,>vd$n@`xki^]oo"
        "ELn;-mhG<-g7T;-NMD8@9&/t%a$S5&wmoP&.h7p&;#DjV<KihLDd7iLU6F,%EC[@@4Lgk$fgdxXgLFYYU1^rZ&3dV[a*QR1f+/S#?*m<-D*m<-?O#<-FO#<-I7T;->T<b.pJ^q#CN#<-"
        "i6T;-sgG<-S'8?p:/G$M=;Y$M>4%&MBLI&M`RR&M#)2_&6U8Vmh<*;nrgarnrrDgrXD#W$FpX8%/3kM(8T0j(9vcG*98%a+Fre&,kicf1l21t-SVC/<iY*/iUbVe$k;u=lwm'aH#'S,v"
        "mS7)$8)ks#Hf<^=[`mr[KYNS]3Y>Ga8tQmM9,ks#fN`*%Xa$:)=ke;-<v%]%Y<4W-l$Uk+o,nKYYFF^=ftJf+%9>G2m])E<U[KJ`MO`*%[OI@#mfFE<HSgJ)]k0/ikRKW&g:pS@Se]q`"
        "-CSq(+]q?^66l%bp-eDbn]->c:CR]c;LnxclCDW-/B$Lcn.MD<AkJq``.ZkV;g_h.Lb*x#p.ck%D;fr6;j:qV(Yi=l[:OV6MB/20li],MI=Y$M>Ac$MSl-=NThC%MCx_%MJLI&Mn_e&M"
        "FCaY$9`Cqiq.Tk+.P^50a)Uh$MZIpTEX34#I3bKl:P&ed)sp-$B8'fMu@](oJ/1<-j`9s$Pi(Dj,%`%k[n);nPYcrnjAQfrlS2Gs$5j(tpxI`t4Y%sr<e6qr3nr3t7YTY,3+]9.>v71v"
        "j.hMBCCw.i^xpMB/b1pJf@QrK^/WF*<G-W-YD7_8Hl)'Mt1?:v4%o4;rq(p$k_8)ZlPD3vYBRE&98,QTvc>iTo/3H-ZT1H-^lOPS?e(B#.B&-M(i`:%7@e;-+)Y,%L^,Da'=-Jhg&Uiq"
        "&bTa<^NeW&#UUq#DACs-;OQ&M,-q(M$iU9&=A$t-LoGb<:Bi`b>Ellr=#p$&0$Q1p4f6W-eF0LPk#AM<$O:U#.St+.u-GwLq6X-Ai@w.irG./iN0i).2F4gLWNPd$d&+m8DPE/iFt35&"
        "E?Vcji-DsB8#OW#ahG<-I0SXA30H+<:=RmMh+PU_foi3vWV0%MIw3'M/ed9'>Z`PAb0tY-L]iofes6/i<9K_&67A-mj)7/iT?<o8QAc`O=iLJV[2OgL;YUq#:*m<-B0GDN];.&MKU1H-"
        "V]Ia*$Ltp.g'T1pE>i^o&hj-$sD:T.i7':#/*UH%O'+<-C4l-Q<YOG%bE@,MuZUq#fTimMp*ks#M($ENp*ks#LNkooTN2p*t@(*Nr609$Zq-9Kc;bJ-7A?t$0[c1pY-Cfq:T;Pf(W-g("
        "D(F',#31/(X:JG)e/@/F.h:U#xu6S#=tdg#Z/o7&XE^T.jZe[#P7T;-x.EFM].E$#(Fg18]j4poFuR1pj_K`$@h@M^8VBJ`,82;eZedumC3t22g#pwuw`bP9V+a;T03+n%>Pv<-*,e2M"
        "Q@AIG8lZL-#Nae*5Q><-,OdxLN]0m&b>L,*<G.E+na?>,:CET.8mVW#P7T;-;r[PA'E3/iKxP.4Dxn'*b^22_$7Mmr<cDw*P)B3B.uooopv;RsHE:uH#e1AlU$;w%ff5jgv_-gD.dcof"
        "cT,5gBdHMhLZB<-dB;=-s*m<-4`qs$qGYQ8gBUmrXCqP%pNt92cC)W-E]:%Gw-UW/BA09$#'89$91I20lR#7$,b;<#B9q@Ki)>]kvU1>mSSb?KI5p*NM)ra%X%`oo,IfG*njDgLC/#Gi"
        "v>eh%=@e;-o-8q*i*h]-ENCONw_o4vv[<5vVIJqD49Lxbc)7/i@v$:)9uJG)7h3F%bqr-$-(m--kvF(FH1U^#hJ^q#B)m<-LN#<-NN#<-DC%a*RLE<-a)m<-&pY=93s+$M:)>$MptF5v"
        "F<a'MO7/x*qd'=-TIGIu[.LP%b%i8%r@Exb_nH6%f-=xu'MZ*&ft7nLaQo4vvf1l-+NJq`fdA`jqxuoo]>b3FQ+Qp78,Y]Yh`[8]rW`l]*^IP^]/uoo2'F(Fl:1LPS-)a*T]WA+&,GeQ"
        "+3c,O?ob;&>5_j`H@n^&AIn<-gp+w$YwmFiXRD684onA,Rb7i:Z=5Dkb[1Al_3&8otwG,ML#EB8HixvRkg8]&OX1H-Ze'S-*5T;-AVjfLFJ`c)e,1h$Ll:`jr=1>m7B5T.xQI@#^3Mp$"
        "?H8=#61S'J>u<w[:c=_.t.$_#YB;=-lpc[.b5?$$>r:T.ipB'#2-Z`-r.Ne+;vNW#Yi(W-5&&Feo*G)+_U_PT+7Tk$dJ:T.7LS9v^*@t$h(am/3K^q#k02k#xC%a*E_e296'L-4[+Tht"
        "i#pwus+AS8.v]cjp,S4<r6Cw.*B@G-T$PW%O8Pm8>PE/iYXS1p*-;/iem(F#B:.&MBxM7)P'L1pgX6B9+`o&v6x*O'On--MmpCa6$xSfL50tx+0k/T.'1'6vQ6b$%6GG/CjWsp^5FfqL"
        "nJc]+[_EiTtS(i'tx_s-j.6'9SmqQTh''PoV+tFr,[QV-,:Tk$nuY%krQw@kB5:<-`41Y:_:i`bK[/_J7;Ixb.0o-$THo-$2v=p7-v)Os%UUq#Y(.K:YC3/iq1Nk47In@&8](J:gC=Q'"
        "OlM/(:5Md*2ftu,qZvJsxK:U#J.5^#avghkoAH>#YPW&#,QdE&seVn2X00`&[=Rp.Yl1$#w&2^Q(aES*pr(9.rwlFilu1t-q'N'M3*ks#jT+49Oo*/i0MmX$K'v`*(c*49O(t+j:>k1."
        ").COF'YlsKMIw90,c=4HH[5D7#_]r+ud9t-%v3$MJusTF.Q^f`Z;Zf=&b50][ak;-d<@t$BuYE-=Gm;`ik3]b&Hl)9RL_(+#9cD9+8YY&)2Ao=N6pb/=TGnFF[&b/^Ut=%l,Hxbk9n-$"
        "x-9a&ZM`w'P<o-$RBo-$.jXF.S86A=Av_LUDIk(tLk^`tlMZLU]rXLU+=HCGA=%vG0V./(PqKV6(ERfCMU.F%oM//&'Q5D<,&1#?6PhY?#s,v?(>m;@S4AS@2urs@kP'#GTq`YG=^E>H"
        "H8R+8vGa%O%Eg$PB+3v#`d8.$_H<1#gV#3#3w+)3-`($#/),##C4i$#JQk&#(5+gLmjkjL1>`'##>T%5+?$(#bP?(#gxL$#CDW)#$i8*#.le%#+0r1.g<fnLhPP&#W-=(2E$),#JXt&#"
        "0[%-#;M*rLuIr'#a'<;6*4n0#0F-(#?@T2#CKg2#QQj)#^9v3#aJ;4#dv&$.hi*$MAam6#@Vr,#Y7I8#*0-&M`qP.#CIQ&M_.>9#ftt.#I=#s-D#E'MjK2/#.OB:#LG&(MhQV/#a=#s-"
        "de(*Mj^i/#g=#s-n?r*Mr&&0#pff=#u^I+MYl;2K%5P>#Pl,v#Sf.JL1r$s$YO`S%Z1f+M9L<5&b6XM'dXFcMI^1,)l)mc)oB$AO`1B>,$;'v,%tZxOk'VS.-+;5/2d8VQ(<0/1=BKG2"
        ">D5SR4/))3Cmc`3Dcl4SA7tu5NoWV6Th*JUO*QS7[ePP8_B'GVePbf:srUY>t>$DW7ZDMB-L(/C6o4VZJiXcD?^W`EDbh4]g/_lJWhAMKl7r@b47P`N#j3AO*[,SeGQEVQ4()8R:a@ig"
        "X`u1TC0XiTKc9cis^cxX[(FYYeQFrm2)Xo[pKVl]rMZ1pA+m._#6Of_*/<ipXd^xb7f@Yc;C1`smCk1gJHi.hweT#$+u:L#7?X(jaPgV$Iri4otNBlo(XTG)Ch;L#j'j@#eSSX#7g+u#"
        "F<b]#2Vu>#Zx_Z#Z5-_#w#UB#UN%d#UB?_#<wRH#b5+e#bNQ_#P^XI#nr0f#tZd_#eD_J#*Y6g#06W`#1+9M#QjPj#hfub#gNDO#wcrk#/S.d#CfhR#R*Io#b^?g#].@S#hBno#hjQg#"
        "hX*T#qmWp#u2*h#x-kT#)HKq#-KNh#3_^U#7#?r#7j&i#F^2W#J4/t#aV5j#'8>>#xZ:v#'i71$O9FA#FY9#$WTq3$jQkA#_r^#$^a-4$rj9B#e4-$$dm?4$$-_B#kLQ$$j#R4$,E-C#"
        "qev$$p/e4$4^QC#w'E%$v;w4$>&*D#(Fs%$'H35$F>ND#._A&$-TE5$NVsD#4wf&$3aW5$VoAE#:95'$:mj5$a7pE#BWc'$N)06$-I`G#ciR)$b.d7$5b.H#i+x)$h:v7$>*]H#pIO*$"
        "pL;8$IH4I#xh'+$wXM8$QaXI#(+L+$'f`8$`G_J#uBwntj2#&#S@wntW:$##(PUV$$t#p@4####";


static const char* GetDefaultCompressedFontDataTTFBase85()
{
    return DroidSans_compressed_data_base85;//proggy_clean_ttf_compressed_data_base85;
}

#endif // #ifndef IMGUI_DISABLE
