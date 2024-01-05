																	 // dear imgui, v1.68
// (demo code)

// Message to the person tempted to delete this file when integrating Dear ImGui into their code base:
// Do NOT remove this file from your project! Think again! It is the most useful reference code that you and other coders
// will want to refer to and call. Have the ImGui::ShowDemoWindow() function wired in an always-available debug menu of
// your game/app! Removing this file from your project is hindering access to documentation for everyone in your team,
// likely leading you to poorer usage of the library.
// Everything in this file will be stripped out by the linker if you don't call ImGui::ShowDemoWindow().
// If you want to link core Dear ImGui in your shipped builds but want an easy guarantee that the demo will not be linked,
// you can setup your imconfig.h with #define IMGUI_DISABLE_DEMO_WINDOWS and those functions will be empty.
// In other situation, whenever you have Dear ImGui available you probably want this to be available for reference.
// Thank you,
// -Your beloved friend, imgui_demo.cpp (that you won't delete)

// Message to beginner C/C++ programmers about the meaning of the 'static' keyword:
// In this demo code, we frequently we use 'static' variables inside functions. A static variable persist across calls, so it is
// essentially like a global variable but declared inside the scope of the function. We do this as a way to gather code and data
// in the same place, to make the demo source code faster to read, faster to write, and smaller in size.
// It also happens to be a convenient way of storing simple UI related information as long as your function doesn't need to be reentrant
// or used in threads. This might be a pattern you will want to use in your code, but most of the real data you would be editing is
// likely going to be stored outside your functions.

/*

Index of this file:

// [SECTION] Forward Declarations, Helpers
// [SECTION] Demo Window / ShowDemoWindow()
// [SECTION] About Window / ShowAboutWindow()
// [SECTION] Style Editor / ShowStyleEditor()
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
// [SECTION] Example App: Simple Overlay / ShowExampleAppSimpleOverlay()
// [SECTION] Example App: Manipulating Window Titles / ShowExampleAppWindowTitles()
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()

*/

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include <ctype.h>          // toupper, isprint
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif
#include "imgui_internal.h"

#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif
#ifdef __clang__
#pragma clang diagnostic ignored "-Wold-style-cast"             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored "-Wdeprecated-declarations"    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored "-Wint-to-void-pointer-cast"   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored "-Wformat-security"            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored "-Wexit-time-destructors"      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#pragma clang diagnostic ignored "-Wunused-macros"              // warning : warning: macro is not used                         // we define snprintf/vsnprintf on Windows so they are available, but not always used.
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning : zero as null pointer constant                  // some standard header variations use #define NULL 0
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"           // warning: implicit conversion from 'float' to 'double' when passing argument to function  // using printf() is a misery with this as C++ va_arg ellipsis changes float to double.
#endif
#if __has_warning("-Wreserved-id-macro")
#pragma clang diagnostic ignored "-Wreserved-id-macro"          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored "-Wformat-security"              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored "-Wdouble-promotion"             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored "-Wconversion"                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#if (__GNUC__ >= 6)
#pragma GCC diagnostic ignored "-Wmisleading-indentation"       // warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on GitHub.
#endif
#endif

// Play it nice with Windows users. Notepad in 2017 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE  "\r\n"
#define snprintf    _snprintf
#define vsnprintf   _vsnprintf
#else
#define IM_NEWLINE  "\n"
#endif

#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

//-----------------------------------------------------------------------------
// [SECTION] Forward Declarations, Helpers
//-----------------------------------------------------------------------------

#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && defined(IMGUI_DISABLE_TEST_WINDOWS) && !defined(IMGUI_DISABLE_DEMO_WINDOWS)   // Obsolete name since 1.53, TEST->DEMO
#define IMGUI_DISABLE_DEMO_WINDOWS
#endif

#if !defined(IMGUI_DISABLE_DEMO_WINDOWS)

// Forward Declarations
static void ShowExampleAppDocuments(bool* p_open);
static void ShowExampleAppMainMenuBar();
static void ShowExampleAppConsole(bool* p_open);
static void ShowExampleAppLog(bool* p_open);
static void ShowExampleAppLayout(bool* p_open);
static void ShowExampleAppPropertyEditor(bool* p_open);
static void ShowExampleAppLongText(bool* p_open);
static void ShowExampleAppAutoResize(bool* p_open);
static void ShowExampleAppConstrainedResize(bool* p_open);
static void ShowExampleAppSimpleOverlay(bool* p_open);
static void ShowExampleAppWindowTitles(bool* p_open);
static void ShowExampleAppCustomRendering(bool* p_open);
static void ShowExampleMenuFile();

// Helper to display a little (?) mark which shows a tooltip when hovered.
static void ShowHelpMarker(const char* desc)
{
    ui::TextDisabled("(?)");
    if (ui::IsItemHovered())
    {
        ui::BeginTooltip();
        ui::PushTextWrapPos(ui::GetFontSize() * 35.0f);
        ui::TextUnformatted(desc);
        ui::PopTextWrapPos();
        ui::EndTooltip();
    }
}

// Helper to display basic user controls.
void ui::ShowUserGuide()
{
    ui::BulletText("Double-click on title bar to collapse window.");
    ui::BulletText("Click and drag on lower right corner to resize window\n(double-click to auto fit window to its contents).");
    ui::BulletText("Click and drag on any empty space to move window.");
    ui::BulletText("TAB/SHIFT+TAB to cycle through keyboard editable fields.");
    ui::BulletText("CTRL+Click on a slider or drag box to input value as text.");
    if (ui::GetIO().FontAllowUserScaling)
        ui::BulletText("CTRL+Mouse Wheel to zoom window contents.");
    ui::BulletText("Mouse Wheel to scroll.");
    ui::BulletText("While editing text:\n");
    ui::Indent();
    ui::BulletText("Hold SHIFT or use mouse to select text.");
    ui::BulletText("CTRL+Left/Right to word jump.");
    ui::BulletText("CTRL+A or double-click to select all.");
    ui::BulletText("CTRL+X,CTRL+C,CTRL+V to use clipboard.");
    ui::BulletText("CTRL+Z,CTRL+Y to undo/redo.");
    ui::BulletText("ESCAPE to revert.");
    ui::BulletText("You can apply arithmetic operators +,*,/ on numerical values.\nUse +- to subtract.");
    ui::Unindent();
}

//-----------------------------------------------------------------------------
// [SECTION] Demo Window / ShowDemoWindow()
//-----------------------------------------------------------------------------

// We split the contents of the big ShowDemoWindow() function into smaller functions (because the link time of very large functions grow non-linearly)
static void ShowDemoWindowWidgets();
static void ShowDemoWindowLayout();
static void ShowDemoWindowPopups();
static void ShowDemoWindowColumns();
static void ShowDemoWindowMisc();

// Demonstrate most Dear ImGui features (this is big function!)
// You may execute this function to experiment with the UI and understand what it does. You may then search for keywords in the code when you are interested by a specific feature.
void ui::ShowDemoWindow(bool* p_open)
{
    // Examples Apps (accessible from the "Examples" menu)
    static bool show_app_documents = false;
    static bool show_app_main_menu_bar = false;
    static bool show_app_console = false;
    static bool show_app_log = false;
    static bool show_app_layout = false;
    static bool show_app_property_editor = false;
    static bool show_app_long_text = false;
    static bool show_app_auto_resize = false;
    static bool show_app_constrained_resize = false;
    static bool show_app_simple_overlay = false;
    static bool show_app_window_titles = false;
    static bool show_app_custom_rendering = false;

    if (show_app_documents)           ShowExampleAppDocuments(&show_app_documents);     // Process the Document app next, as it may also use a DockSpace()
    if (show_app_main_menu_bar)       ShowExampleAppMainMenuBar();
    if (show_app_console)             ShowExampleAppConsole(&show_app_console);
    if (show_app_log)                 ShowExampleAppLog(&show_app_log);
    if (show_app_layout)              ShowExampleAppLayout(&show_app_layout);
    if (show_app_property_editor)     ShowExampleAppPropertyEditor(&show_app_property_editor);
    if (show_app_long_text)           ShowExampleAppLongText(&show_app_long_text);
    if (show_app_auto_resize)         ShowExampleAppAutoResize(&show_app_auto_resize);
    if (show_app_constrained_resize)  ShowExampleAppConstrainedResize(&show_app_constrained_resize);
    if (show_app_simple_overlay)      ShowExampleAppSimpleOverlay(&show_app_simple_overlay);
    if (show_app_window_titles)       ShowExampleAppWindowTitles(&show_app_window_titles);
    if (show_app_custom_rendering)    ShowExampleAppCustomRendering(&show_app_custom_rendering);

    // Dear ImGui Apps (accessible from the "Help" menu)
    static bool show_app_metrics = false;
    static bool show_app_style_editor = false;
    static bool show_app_about = false;

    if (show_app_metrics)             { ui::ShowMetricsWindow(&show_app_metrics); }
    if (show_app_style_editor)        { ui::Begin("Style Editor", &show_app_style_editor); ui::ShowStyleEditor(); ui::End(); }
    if (show_app_about)               { ui::ShowAboutWindow(&show_app_about); }

    // Demonstrate the various window flags. Typically you would just use the default!
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
    ui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ui::Begin("ImGui Demo", p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ui::End();
        return;
    }
    ui::Text("dear imgui says hello. (%s)", IMGUI_VERSION);

    // Most "big" widgets share a common width settings by default.
    //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // Use 2/3 of the space for widgets and 1/3 for labels (default)
    ui::PushItemWidth(ui::GetFontSize() * -12);           // Use fixed width for labels (by passing a negative value), the rest goes to widgets. We choose a width proportional to our font size.

    // Menu
    if (ui::BeginMenuBar())
    {
        if (ui::BeginMenu("Menu"))
        {
            ShowExampleMenuFile();
            ui::EndMenu();
        }
        if (ui::BeginMenu("Examples"))
        {
            ui::MenuItem("Main menu bar", NULL, &show_app_main_menu_bar);
            ui::MenuItem("Console", NULL, &show_app_console);
            ui::MenuItem("Log", NULL, &show_app_log);
            ui::MenuItem("Simple layout", NULL, &show_app_layout);
            ui::MenuItem("Property editor", NULL, &show_app_property_editor);
            ui::MenuItem("Long text display", NULL, &show_app_long_text);
            ui::MenuItem("Auto-resizing window", NULL, &show_app_auto_resize);
            ui::MenuItem("Constrained-resizing window", NULL, &show_app_constrained_resize);
            ui::MenuItem("Simple overlay", NULL, &show_app_simple_overlay);
            ui::MenuItem("Manipulating window titles", NULL, &show_app_window_titles);
            ui::MenuItem("Custom rendering", NULL, &show_app_custom_rendering);
            ui::MenuItem("Documents", NULL, &show_app_documents);
            ui::EndMenu();
        }
        if (ui::BeginMenu("Help"))
        {
            ui::MenuItem("Metrics", NULL, &show_app_metrics);
            ui::MenuItem("Style Editor", NULL, &show_app_style_editor);
            ui::MenuItem("About Dear ImGui", NULL, &show_app_about);
            ui::EndMenu();
        }
        ui::EndMenuBar();
    }

    ui::Spacing();
    if (ui::CollapsingHeader("Help"))
    {
        ui::Text("PROGRAMMER GUIDE:");
        ui::BulletText("Please see the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
        ui::BulletText("Please see the comments in imgui.cpp.");
        ui::BulletText("Please see the examples/ in application.");
        ui::BulletText("Enable 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        ui::BulletText("Enable 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
        ui::Separator();

        ui::Text("USER GUIDE:");
        ui::ShowUserGuide();
    }

    if (ui::CollapsingHeader("Configuration"))
    {
        ImGuiIO& io = ui::GetIO();

        if (ui::TreeNode("Configuration##2"))
        {
            ui::CheckboxFlags("io.ConfigFlags: NavEnableKeyboard", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
            ui::CheckboxFlags("io.ConfigFlags: NavEnableGamepad", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
            ui::SameLine(); ShowHelpMarker("Required back-end to feed in gamepad inputs in io.NavInputs[] and set io.BackendFlags |= ImGuiBackendFlags_HasGamepad.\n\nRead instructions in imgui.cpp for details.");
            ui::CheckboxFlags("io.ConfigFlags: NavEnableSetMousePos", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NavEnableSetMousePos);
            ui::SameLine(); ShowHelpMarker("Instruct navigation to move the mouse cursor. See comment for ImGuiConfigFlags_NavEnableSetMousePos.");
            ui::CheckboxFlags("io.ConfigFlags: NoMouse", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NoMouse);
            if (io.ConfigFlags & ImGuiConfigFlags_NoMouse) // Create a way to restore this flag otherwise we could be stuck completely!
            {
                if (fmodf((float)ui::GetTime(), 0.40f) < 0.20f)
                {
                    ui::SameLine();
                    ui::Text("<<PRESS SPACE TO DISABLE>>");
                }
                if (ui::IsKeyPressed(ui::GetKeyIndex(ImGuiKey_Space)))
                    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            }
            ui::CheckboxFlags("io.ConfigFlags: NoMouseCursorChange", (unsigned int *)&io.ConfigFlags, ImGuiConfigFlags_NoMouseCursorChange);
            ui::SameLine(); ShowHelpMarker("Instruct back-end to not alter mouse cursor shape and visibility.");
            ui::Checkbox("io.ConfigInputTextCursorBlink", &io.ConfigInputTextCursorBlink);
            ui::SameLine(); ShowHelpMarker("Set to false to disable blinking cursor, for users who consider it distracting");
            ui::Checkbox("io.ConfigWindowsResizeFromEdges", &io.ConfigWindowsResizeFromEdges);
            ui::SameLine(); ShowHelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors) because it needs mouse cursor feedback.");
            ui::Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", &io.ConfigWindowsMoveFromTitleBarOnly);
            ui::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
            ui::SameLine(); ShowHelpMarker("Instruct Dear ImGui to render a mouse cursor for you. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something).");
            ui::TreePop();
            ui::Separator();
        }

        if (ui::TreeNode("Backend Flags"))
        {
            ShowHelpMarker("Those flags are set by the back-ends (imgui_impl_xxx files) to specify their capabilities.");
            ImGuiBackendFlags backend_flags = io.BackendFlags; // Make a local copy to avoid modifying the back-end flags.
            ui::CheckboxFlags("io.BackendFlags: HasGamepad", (unsigned int *)&backend_flags, ImGuiBackendFlags_HasGamepad);
            ui::CheckboxFlags("io.BackendFlags: HasMouseCursors", (unsigned int *)&backend_flags, ImGuiBackendFlags_HasMouseCursors);
            ui::CheckboxFlags("io.BackendFlags: HasSetMousePos", (unsigned int *)&backend_flags, ImGuiBackendFlags_HasSetMousePos);
            ui::TreePop();
            ui::Separator();
        }

        if (ui::TreeNode("Style"))
        {
            ui::ShowStyleEditor();
            ui::TreePop();
            ui::Separator();
        }

        if (ui::TreeNode("Capture/Logging"))
        {
            ui::TextWrapped("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded.");
            ShowHelpMarker("Try opening any of the contents below in this window and then click one of the \"Log To\" button.");
            ui::LogButtons();
            ui::TextWrapped("You can also call ImGui::LogText() to output directly to the log without a visual output.");
            if (ui::Button("Copy \"Hello, world!\" to clipboard"))
            {
                ui::LogToClipboard();
                ui::LogText("Hello, world!");
                ui::LogFinish();
            }
            ui::TreePop();
        }
    }

    if (ui::CollapsingHeader("Window options"))
    {
        ui::Checkbox("No titlebar", &no_titlebar); ui::SameLine(150);
        ui::Checkbox("No scrollbar", &no_scrollbar); ui::SameLine(300);
        ui::Checkbox("No menu", &no_menu);
        ui::Checkbox("No move", &no_move); ui::SameLine(150);
        ui::Checkbox("No resize", &no_resize); ui::SameLine(300);
        ui::Checkbox("No collapse", &no_collapse);
        ui::Checkbox("No close", &no_close); ui::SameLine(150);
        ui::Checkbox("No nav", &no_nav); ui::SameLine(300);
        ui::Checkbox("No background", &no_background);
        ui::Checkbox("No bring to front", &no_bring_to_front);
    }

    // All demo contents
    ShowDemoWindowWidgets();
    ShowDemoWindowLayout();
    ShowDemoWindowPopups();
    ShowDemoWindowColumns();
    ShowDemoWindowMisc();

    // End of ShowDemoWindow()
    ui::End();
}

static void ShowDemoWindowWidgets()
{
    if (!ui::CollapsingHeader("Widgets"))
        return;

    if (ui::TreeNode("Basic"))
    {
        static int clicked = 0;
        if (ui::Button("Button"))
            clicked++;
        if (clicked & 1)
        {
            ui::SameLine();
            ui::Text("Thanks for clicking me!");
        }

        static bool check = true;
        ui::Checkbox("checkbox", &check);

        static int e = 0;
        ui::RadioButton("radio a", &e, 0); ui::SameLine();
        ui::RadioButton("radio b", &e, 1); ui::SameLine();
        ui::RadioButton("radio c", &e, 2);

        // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
        for (int i = 0; i < 7; i++)
        {
            if (i > 0)
                ui::SameLine();
            ui::PushID(i);
            ui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i/7.0f, 0.6f, 0.6f));
            ui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.7f));
            ui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i/7.0f, 0.8f, 0.8f));
            ui::Button("Click");
            ui::PopStyleColor(3);
            ui::PopID();
        }

        // Use AlignTextToFramePadding() to align text baseline to the baseline of framed elements (otherwise a Text+SameLine+Button sequence will have the text a little too high by default)
        ui::AlignTextToFramePadding();
        ui::Text("Hold to repeat:");
        ui::SameLine();

        // Arrow buttons with Repeater
        static int counter = 0;
        float spacing = ui::GetStyle().ItemInnerSpacing.x;
        ui::PushButtonRepeat(true);
        if (ui::ArrowButton("##left", ImGuiDir_Left)) { counter--; }
        ui::SameLine(0.0f, spacing);
        if (ui::ArrowButton("##right", ImGuiDir_Right)) { counter++; }
        ui::PopButtonRepeat();
        ui::SameLine();
        ui::Text("%d", counter);

        ui::Text("Hover over me");
        if (ui::IsItemHovered())
            ui::SetTooltip("I am a tooltip");

        ui::SameLine();
        ui::Text("- or me");
        if (ui::IsItemHovered())
        {
            ui::BeginTooltip();
            ui::Text("I am a fancy tooltip");
            static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
            ui::PlotLines("Curve", arr, IM_ARRAYSIZE(arr));
            ui::EndTooltip();
        }

        ui::Separator();

        ui::LabelText("label", "Value");

        {
            // Using the _simplified_ one-liner Combo() api here
            // See "Combo" section for examples of how to use the more complete BeginCombo()/EndCombo() api.
            const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
            static int item_current = 0;
            ui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
            ui::SameLine(); ShowHelpMarker("Refer to the \"Combo\" section below for an explanation of the full BeginCombo/EndCombo API, and demonstration of various flags.\n");
        }

        {
            static char str0[128] = "Hello, world!";
            static int i0 = 123;
            ui::InputText("input text", str0, IM_ARRAYSIZE(str0));
            ui::SameLine(); ShowHelpMarker("USER:\nHold SHIFT or use mouse to select text.\n" "CTRL+Left/Right to word jump.\n" "CTRL+A or double-click to select all.\n" "CTRL+X,CTRL+C,CTRL+V clipboard.\n" "CTRL+Z,CTRL+Y undo/redo.\n" "ESCAPE to revert.\n\nPROGRAMMER:\nYou can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputText() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example (this is not demonstrated in imgui_demo.cpp).");

            ui::InputInt("input int", &i0);
            ui::SameLine(); ShowHelpMarker("You can apply arithmetic operators +,*,/ on numerical values.\n  e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\nUse +- to subtract.\n");

            static float f0 = 0.001f;
            ui::InputFloat("input float", &f0, 0.01f, 1.0f, "%.3f");

            static double d0 = 999999.00000001;
            ui::InputDouble("input double", &d0, 0.01f, 1.0f, "%.8f");

            static float f1 = 1.e10f;
            ui::InputFloat("input scientific", &f1, 0.0f, 0.0f, "%e");
            ui::SameLine(); ShowHelpMarker("You can input value using the scientific notation,\n  e.g. \"1e+8\" becomes \"100000000\".\n");

            static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
            ui::InputFloat3("input float3", vec4a);
        }

        {
            static int i1 = 50, i2 = 42;
            ui::DragInt("drag int", &i1, 1);
            ui::SameLine(); ShowHelpMarker("Click and drag to edit value.\nHold SHIFT/ALT for faster/slower edit.\nDouble-click or CTRL+click to input value.");

            ui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%d%%");

            static float f1=1.00f, f2=0.0067f;
            ui::DragFloat("drag float", &f1, 0.005f);
            ui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");
        }

        {
            static int i1=0;
            ui::SliderInt("slider int", &i1, -1, 3);
            ui::SameLine(); ShowHelpMarker("CTRL+click to input value.");

            static float f1=0.123f, f2=0.0f;
            ui::SliderFloat("slider float", &f1, 0.0f, 1.0f, "ratio = %.3f");
            ui::SliderFloat("slider float (curve)", &f2, -10.0f, 10.0f, "%.4f", 2.0f);
            static float angle = 0.0f;
            ui::SliderAngle("slider angle", &angle);
        }

        {
            static float col1[3] = { 1.0f,0.0f,0.2f };
            static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
            ui::ColorEdit3("color 1", col1);
            ui::SameLine(); ShowHelpMarker("Click on the colored square to open a color picker.\nClick and hold to use drag and drop.\nRight-click on the colored square to show options.\nCTRL+click on individual component to input value.\n");

            ui::ColorEdit4("color 2", col2);
        }

        {
            // List box
            const char* listbox_items[] = { "Apple", "Banana", "Cherry", "Kiwi", "Mango", "Orange", "Pineapple", "Strawberry", "Watermelon" };
            static int listbox_item_current = 1;
            ui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

            //static int listbox_item_current2 = 2;
            //ImGui::PushItemWidth(-1);
            //ImGui::ListBox("##listbox2", &listbox_item_current2, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
            //ImGui::PopItemWidth();
        }

        ui::TreePop();
    }

    // Testing ImGuiOnceUponAFrame helper.
    //static ImGuiOnceUponAFrame once;
    //for (int i = 0; i < 5; i++)
    //    if (once)
    //        ImGui::Text("This will be displayed only once.");

    if (ui::TreeNode("Trees"))
    {
        if (ui::TreeNode("Basic trees"))
        {
            for (int i = 0; i < 5; i++)
                if (ui::TreeNode((void*)(intptr_t)i, "Child %d", i))
                {
                    ui::Text("blah blah");
                    ui::SameLine();
                    if (ui::SmallButton("button")) { };
                    ui::TreePop();
                }
            ui::TreePop();
        }

        if (ui::TreeNode("Advanced, with Selectable nodes"))
        {
            ShowHelpMarker("This is a more standard looking tree with selectable nodes.\nClick to select, CTRL+Click to toggle, click on arrows or double-click to open.");
            static bool align_label_with_current_x_position = false;
            ui::Checkbox("Align label with current X position)", &align_label_with_current_x_position);
            ui::Text("Hello!");
            if (align_label_with_current_x_position)
                ui::Unindent(ui::GetTreeNodeToLabelSpacing());

            static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
            int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
            ui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ui::GetFontSize()*3); // Increase spacing to differentiate leaves from expanded contents.
            for (int i = 0; i < 6; i++)
            {
                // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
                if (i < 3)
                {
                    // Node
                    bool node_open = ui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
                    if (ui::IsItemClicked())
                        node_clicked = i;
                    if (node_open)
                    {
                        ui::Text("Blah blah\nBlah Blah");
                        ui::TreePop();
                    }
                }
                else
                {
                    // Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
                    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                    ui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
                    if (ui::IsItemClicked())
                        node_clicked = i;
                }
            }
            if (node_clicked != -1)
            {
                // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
                if (ui::GetIO().KeyCtrl)
                    selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
                else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
                    selection_mask = (1 << node_clicked);           // Click to single-select
            }
            ui::PopStyleVar();
            if (align_label_with_current_x_position)
                ui::Indent(ui::GetTreeNodeToLabelSpacing());
            ui::TreePop();
        }
        ui::TreePop();
    }

    if (ui::TreeNode("Collapsing Headers"))
    {
        static bool closable_group = true;
        ui::Checkbox("Enable extra group", &closable_group);
        if (ui::CollapsingHeader("Header"))
        {
            ui::Text("IsItemHovered: %d", ui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ui::Text("Some content %d", i);
        }
        if (ui::CollapsingHeader("Header with a close button", &closable_group))
        {
            ui::Text("IsItemHovered: %d", ui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ui::Text("More content %d", i);
        }
        ui::TreePop();
    }

    if (ui::TreeNode("Bullets"))
    {
        ui::BulletText("Bullet point 1");
        ui::BulletText("Bullet point 2\nOn multiple lines");
        ui::Bullet(); ui::Text("Bullet point 3 (two calls)");
        ui::Bullet(); ui::SmallButton("Button");
        ui::TreePop();
    }

    if (ui::TreeNode("Text"))
    {
        if (ui::TreeNode("Colored Text"))
        {
            // Using shortcut. You can use PushStyleColor()/PopStyleColor() for more flexibility.
            ui::TextColored(ImVec4(1.0f,0.0f,1.0f,1.0f), "Pink");
            ui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Yellow");
            ui::TextDisabled("Disabled");
            ui::SameLine(); ShowHelpMarker("The TextDisabled color is stored in ImGuiStyle.");
            ui::TreePop();
        }

        if (ui::TreeNode("Word Wrapping"))
        {
            // Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
            ui::TextWrapped("This text should automatically wrap on the edge of the window. The current implementation for text wrapping follows simple rules suitable for English and possibly other languages.");
            ui::Spacing();

            static float wrap_width = 200.0f;
            ui::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");

            ui::Text("Test paragraph 1:");
            ImVec2 pos = ui::GetCursorScreenPos();
            ui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ui::GetTextLineHeight()), IM_COL32(255,0,255,255));
            ui::PushTextWrapPos(ui::GetCursorPos().x + wrap_width);
            ui::Text("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog.", wrap_width);
            ui::GetWindowDrawList()->AddRect(ui::GetItemRectMin(), ui::GetItemRectMax(), IM_COL32(255,255,0,255));
            ui::PopTextWrapPos();

            ui::Text("Test paragraph 2:");
            pos = ui::GetCursorScreenPos();
            ui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ui::GetTextLineHeight()), IM_COL32(255,0,255,255));
            ui::PushTextWrapPos(ui::GetCursorPos().x + wrap_width);
            ui::Text("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh");
            ui::GetWindowDrawList()->AddRect(ui::GetItemRectMin(), ui::GetItemRectMax(), IM_COL32(255,255,0,255));
            ui::PopTextWrapPos();

            ui::TreePop();
        }

        if (ui::TreeNode("UTF-8 Text"))
        {
            // UTF-8 test with Japanese characters
            // (Needs a suitable font, try Noto, or Arial Unicode, or M+ fonts. Read misc/fonts/README.txt for details.)
            // - From C++11 you can use the u8"my text" syntax to encode literal strings as UTF-8
            // - For earlier compiler, you may be able to encode your sources as UTF-8 (e.g. Visual Studio save your file as 'UTF-8 without signature')
            // - FOR THIS DEMO FILE ONLY, BECAUSE WE WANT TO SUPPORT OLD COMPILERS, WE ARE *NOT* INCLUDING RAW UTF-8 CHARACTERS IN THIS SOURCE FILE.
            //   Instead we are encoding a few strings with hexadecimal constants. Don't do this in your application!
            //   Please use u8"text in any language" in your application!
            // Note that characters values are preserved even by InputText() if the font cannot be displayed, so you can safely copy & paste garbled characters into another application.
            ui::TextWrapped("CJK text will only appears if the font was loaded with the appropriate CJK character ranges. Call io.Font->AddFontFromFileTTF() manually to load extra character ranges. Read misc/fonts/README.txt for details.");
            ui::Text("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)"); // Normally we would use u8"blah blah" with the proper characters directly in the string.
            ui::Text("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)");
            static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e";
            //static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
            ui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
            ui::TreePop();
        }
        ui::TreePop();
    }

    if (ui::TreeNode("Images"))
    {
        ImGuiIO& io = ui::GetIO();
        ui::TextWrapped("Below we are displaying the font texture (which is the only texture we have access to in this demo). Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. Hover the texture for a zoomed view!");

        // Here we are grabbing the font texture because that's the only one we have access to inside the demo code.
        // Remember that ImTextureID is just storage for whatever you want it to be, it is essentially a value that will be passed to the render function inside the ImDrawCmd structure.
        // If you use one of the default imgui_impl_XXXX.cpp renderer, they all have comments at the top of their file to specify what they expect to be stored in ImTextureID.
        // (for example, the imgui_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*' pointer. The imgui_impl_glfw_gl3.cpp renderer expect a GLuint OpenGL texture identifier etc.)
        // If you decided that ImTextureID = MyEngineTexture*, then you can pass your MyEngineTexture* pointers to ImGui::Image(), and gather width/height through your own functions, etc.
        // Using ShowMetricsWindow() as a "debugger" to inspect the draw data that are being passed to your render will help you debug issues if you are confused about this.
        // Consider using the lower-level ImDrawList::AddImage() API, via ImGui::GetWindowDrawList()->AddImage().
        ImTextureID my_tex_id = io.Fonts->TexID;
        float my_tex_w = (float)io.Fonts->TexWidth;
        float my_tex_h = (float)io.Fonts->TexHeight;

        ui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
        ImVec2 pos = ui::GetCursorScreenPos();
        ui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
        if (ui::IsItemHovered())
        {
            ui::BeginTooltip();
            float region_sz = 32.0f;
            float region_x = io.MousePos.x - pos.x - region_sz * 0.5f; if (region_x < 0.0f) region_x = 0.0f; else if (region_x > my_tex_w - region_sz) region_x = my_tex_w - region_sz;
            float region_y = io.MousePos.y - pos.y - region_sz * 0.5f; if (region_y < 0.0f) region_y = 0.0f; else if (region_y > my_tex_h - region_sz) region_y = my_tex_h - region_sz;
            float zoom = 4.0f;
            ui::Text("Min: (%.2f, %.2f)", region_x, region_y);
            ui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
            ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
            ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
            ui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImColor(255,255,255,255), ImColor(255,255,255,128));
            ui::EndTooltip();
        }
        ui::TextWrapped("And now some textured buttons..");
        static int pressed_count = 0;
        for (int i = 0; i < 8; i++)
        {
            ui::PushID(i);
            int frame_padding = -1 + i;     // -1 = uses default padding
            if (ui::ImageButton(my_tex_id, ImVec2(32,32), ImVec2(0,0), ImVec2(32.0f/my_tex_w,32/my_tex_h), frame_padding, ImColor(0,0,0,255)))
                pressed_count += 1;
            ui::PopID();
            ui::SameLine();
        }
        ui::NewLine();
        ui::Text("Pressed %d times.", pressed_count);
        ui::TreePop();
    }

    if (ui::TreeNode("Combo"))
    {
        // Expose flags as checkbox for the demo
        static ImGuiComboFlags flags = 0;
        ui::CheckboxFlags("ImGuiComboFlags_PopupAlignLeft", (unsigned int*)&flags, ImGuiComboFlags_PopupAlignLeft);
        if (ui::CheckboxFlags("ImGuiComboFlags_NoArrowButton", (unsigned int*)&flags, ImGuiComboFlags_NoArrowButton))
            flags &= ~ImGuiComboFlags_NoPreview;     // Clear the other flag, as we cannot combine both
        if (ui::CheckboxFlags("ImGuiComboFlags_NoPreview", (unsigned int*)&flags, ImGuiComboFlags_NoPreview))
            flags &= ~ImGuiComboFlags_NoArrowButton; // Clear the other flag, as we cannot combine both

        // General BeginCombo() API, you have full control over your selection data and display type.
        // (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static const char* item_current = items[0];            // Here our selection is a single pointer stored outside the object.
        if (ui::BeginCombo("combo 1", item_current, flags)) // The second parameter is the label previewed before opening the combo.
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                bool is_selected = (item_current == items[n]);
                if (ui::Selectable(items[n], is_selected))
                    item_current = items[n];
                if (is_selected)
                    ui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ui::EndCombo();
        }

        // Simplified one-liner Combo() API, using values packed in a single constant string
        static int item_current_2 = 0;
        ui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

        // Simplified one-liner Combo() using an array of const char*
        static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
        ui::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

        // Simplified one-liner Combo() using an accessor function
        struct FuncHolder { static bool ItemGetter(void* data, int idx, const char** out_str) { *out_str = ((const char**)data)[idx]; return true; } };
        static int item_current_4 = 0;
        ui::Combo("combo 4 (function)", &item_current_4, &FuncHolder::ItemGetter, items, IM_ARRAYSIZE(items));

        ui::TreePop();
    }

    if (ui::TreeNode("Selectables"))
    {
        // Selectable() has 2 overloads:
        // - The one taking "bool selected" as a read-only selection information. When Selectable() has been clicked is returns true and you can alter selection state accordingly.
        // - The one taking "bool* p_selected" as a read-write selection information (convenient in some cases)
        // The earlier is more flexible, as in real application your selection may be stored in a different manner (in flags within objects, as an external list, etc).
        if (ui::TreeNode("Basic"))
        {
            static bool selection[5] = { false, true, false, false, false };
            ui::Selectable("1. I am selectable", &selection[0]);
            ui::Selectable("2. I am selectable", &selection[1]);
            ui::Text("3. I am not selectable");
            ui::Selectable("4. I am selectable", &selection[3]);
            if (ui::Selectable("5. I am double clickable", selection[4], ImGuiSelectableFlags_AllowDoubleClick))
                if (ui::IsMouseDoubleClicked(0))
                    selection[4] = !selection[4];
            ui::TreePop();
        }
        if (ui::TreeNode("Selection State: Single Selection"))
        {
            static int selected = -1;
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (ui::Selectable(buf, selected == n))
                    selected = n;
            }
            ui::TreePop();
        }
        if (ui::TreeNode("Selection State: Multiple Selection"))
        {
            ShowHelpMarker("Hold CTRL and click to select multiple items.");
            static bool selection[5] = { false, false, false, false, false };
            for (int n = 0; n < 5; n++)
            {
                char buf[32];
                sprintf(buf, "Object %d", n);
                if (ui::Selectable(buf, selection[n]))
                {
                    if (!ui::GetIO().KeyCtrl)    // Clear selection when CTRL is not held
                        memset(selection, 0, sizeof(selection));
                    selection[n] ^= 1;
                }
            }
            ui::TreePop();
        }
        if (ui::TreeNode("Rendering more text into the same line"))
        {
            // Using the Selectable() override that takes "bool* p_selected" parameter and toggle your booleans automatically.
            static bool selected[3] = { false, false, false };
            ui::Selectable("main.c",    &selected[0]); ui::SameLine(300); ui::Text(" 2,345 bytes");
            ui::Selectable("Hello.cpp", &selected[1]); ui::SameLine(300); ui::Text("12,345 bytes");
            ui::Selectable("Hello.h",   &selected[2]); ui::SameLine(300); ui::Text(" 2,345 bytes");
            ui::TreePop();
        }
        if (ui::TreeNode("In columns"))
        {
            ui::Columns(3, NULL, false);
            static bool selected[16] = { 0 };
            for (int i = 0; i < 16; i++)
            {
                char label[32]; sprintf(label, "Item %d", i);
                if (ui::Selectable(label, &selected[i])) {}
                ui::NextColumn();
            }
            ui::Columns(1);
            ui::TreePop();
        }
        if (ui::TreeNode("Grid"))
        {
            static bool selected[4*4] = { true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true };
            for (int i = 0; i < 4*4; i++)
            {
                ui::PushID(i);
                if (ui::Selectable("Sailor", &selected[i], 0, ImVec2(50,50)))
                {
                    // Note: We _unnecessarily_ test for both x/y and i here only to silence some static analyzer. The second part of each test is unnecessary.
                    int x = i % 4;
                    int y = i / 4;
                    if (x > 0)           { selected[i - 1] ^= 1; }
                    if (x < 3 && i < 15) { selected[i + 1] ^= 1; }
                    if (y > 0 && i > 3)  { selected[i - 4] ^= 1; }
                    if (y < 3 && i < 12) { selected[i + 4] ^= 1; }
                }
                if ((i % 4) < 3) ui::SameLine();
                ui::PopID();
            }
            ui::TreePop();
        }
        if (ui::TreeNode("Alignment"))
        {
            ShowHelpMarker("Alignment applies when a selectable is larger than its text content.\nBy default, Selectables uses style.SelectableTextAlign but it can be overriden on a per-item basis using PushStyleVar().");
            static bool selected[3*3] = { true, false, true, false, true, false, true, false, true };
            for (int y = 0; y < 3; y++)
            {
                for (int x = 0; x < 3; x++)
                {
                    ImVec2 alignment = ImVec2((float)x / 2.0f, (float)y / 2.0f);
                    char name[32];
                    sprintf(name, "(%.1f,%.1f)", alignment.x, alignment.y);
                    if (x > 0) ui::SameLine();
                    ui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, alignment);
                    ui::Selectable(name, &selected[3*y+x], ImGuiSelectableFlags_None, ImVec2(80,80));
                    ui::PopStyleVar();
                }
            }
            ui::TreePop();
        }
        ui::TreePop();
    }

    if (ui::TreeNode("Filtered Text Input"))
    {
        static char buf1[64] = ""; ui::InputText("default", buf1, 64);
        static char buf2[64] = ""; ui::InputText("decimal", buf2, 64, ImGuiInputTextFlags_CharsDecimal);
        static char buf3[64] = ""; ui::InputText("hexadecimal", buf3, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
        static char buf4[64] = ""; ui::InputText("uppercase", buf4, 64, ImGuiInputTextFlags_CharsUppercase);
        static char buf5[64] = ""; ui::InputText("no blank", buf5, 64, ImGuiInputTextFlags_CharsNoBlank);
        struct TextFilters { static int FilterImGuiLetters(ImGuiInputTextCallbackData* data) { if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar)) return 0; return 1; } };
        static char buf6[64] = ""; ui::InputText("\"imgui\" letters", buf6, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);

        ui::Text("Password input");
        static char bufpass[64] = "password123";
        ui::InputText("password", bufpass, 64, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
        ui::SameLine(); ShowHelpMarker("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging::get().\n");
        ui::InputText("password (clear)", bufpass, 64, ImGuiInputTextFlags_CharsNoBlank);

        ui::TreePop();
    }

    if (ui::TreeNode("Multi-line Text Input"))
    {
        // Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
        // and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
        static bool read_only = false;
        static char text[1024*16] =
            "/*\n"
            " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
            " the hexadecimal encoding of one offending instruction,\n"
            " more formally, the invalid operand with locked CMPXCHG8B\n"
            " instruction bug, is a design flaw in the majority of\n"
            " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
            " processors (all in the P5 microarchitecture).\n"
            "*/\n\n"
            "label:\n"
            "\tlock cmpxchg8b eax\n";

        ShowHelpMarker("You can use the ImGuiInputTextFlags_CallbackResize facility if you need to wire InputTextMultiline() to a dynamic string type. See misc/cpp/imgui_stdlib.h for an example. (This is not demonstrated in imgui_demo.cpp)");
        ui::Checkbox("Read-only", &read_only);
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | (read_only ? ImGuiInputTextFlags_ReadOnly : 0);
        ui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-1.0f, ui::GetTextLineHeight() * 16), flags);
        ui::TreePop();
    }

    if (ui::TreeNode("Plots Widgets"))
    {
        static bool animate = true;
        ui::Checkbox("Animate", &animate);

        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        ui::PlotLines("Frame Times", arr, IM_ARRAYSIZE(arr));

        // Create a dummy array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float and the sizeof() of your structure in the Stride parameter.
        static float values[90] = { 0 };
        static int values_offset = 0;
        static double refresh_time = 0.0;
        if (!animate || refresh_time == 0.0)
            refresh_time = ui::GetTime();
        while (refresh_time < ui::GetTime()) // Create dummy data at fixed 60 hz rate for the demo
        {
            static float phase = 0.0f;
            values[values_offset] = cosf(phase);
            values_offset = (values_offset+1) % IM_ARRAYSIZE(values);
            phase += 0.10f*values_offset;
            refresh_time += 1.0f/60.0f;
        }
        ui::PlotLines("Lines", values, IM_ARRAYSIZE(values), values_offset, "avg 0.0", -1.0f, 1.0f, ImVec2(0,80));
        ui::PlotHistogram("Histogram", arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));

        // Use functions to generate output
        // FIXME: This is rather awkward because current plot API only pass in indices. We probably want an API passing floats and user provide sample rate/count.
        struct Funcs
        {
            static float Sin(void*, int i) { return sinf(i * 0.1f); }
            static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
        };
        static int func_type = 0, display_count = 70;
        ui::Separator();
        ui::PushItemWidth(100); ui::Combo("func", &func_type, "Sin\0Saw\0"); ui::PopItemWidth();
        ui::SameLine();
        ui::SliderInt("Sample count", &display_count, 1, 400);
        float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
        ui::PlotLines("Lines", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
        ui::PlotHistogram("Histogram", func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
        ui::Separator();

        // Animate a simple progress bar
        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.4f * ui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }

        // Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
        ui::ProgressBar(progress, ImVec2(0.0f,0.0f));
        ui::SameLine(0.0f, ui::GetStyle().ItemInnerSpacing.x);
        ui::Text("Progress Bar");

        float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
        char buf[32];
        sprintf(buf, "%d/%d", (int)(progress_saturated*1753), 1753);
        ui::ProgressBar(progress, ImVec2(0.f,0.f), buf);
        ui::TreePop();
    }

    if (ui::TreeNode("Color/Picker Widgets"))
    {
        static ImVec4 color = ImVec4(114.0f/255.0f, 144.0f/255.0f, 154.0f/255.0f, 200.0f/255.0f);

        static bool alpha_preview = true;
        static bool alpha_half_preview = false;
        static bool drag_and_drop = true;
        static bool options_menu = true;
        static bool hdr = false;
        ui::Checkbox("With Alpha Preview", &alpha_preview);
        ui::Checkbox("With Half Alpha Preview", &alpha_half_preview);
        ui::Checkbox("With Drag and Drop", &drag_and_drop);
        ui::Checkbox("With Options Menu", &options_menu); ui::SameLine(); ShowHelpMarker("Right-click on the individual color widget to show options.");
        ui::Checkbox("With HDR", &hdr); ui::SameLine(); ShowHelpMarker("Currently all this does is to lift the 0..1 limits on dragging widgets.");
        int misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

        ui::Text("Color widget:");
        ui::SameLine(); ShowHelpMarker("Click on the colored square to open a color picker.\nCTRL+click on individual component to input value.\n");
        ui::ColorEdit3("MyColor##1", (float*)&color, misc_flags);

        ui::Text("Color widget HSV with Alpha:");
        ui::ColorEdit4("MyColor##2", (float*)&color, ImGuiColorEditFlags_HSV | misc_flags);

        ui::Text("Color widget with Float Display:");
        ui::ColorEdit4("MyColor##2f", (float*)&color, ImGuiColorEditFlags_Float | misc_flags);

        ui::Text("Color button with Picker:");
        ui::SameLine(); ShowHelpMarker("With the ImGuiColorEditFlags_NoInputs flag you can hide all the slider/text inputs.\nWith the ImGuiColorEditFlags_NoLabel flag you can pass a non-empty label which will only be used for the tooltip and picker popup.");
        ui::ColorEdit4("MyColor##3", (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | misc_flags);

        ui::Text("Color button with Custom Picker Popup:");

        // Generate a dummy default palette. The palette will persist and can be edited.
        static bool saved_palette_init = true;
        static ImVec4 saved_palette[32] = { };
        if (saved_palette_init)
        {
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f, saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
                saved_palette[n].w = 1.0f; // Alpha
            }
            saved_palette_init = false;
        }

        static ImVec4 backup_color;
        bool open_popup = ui::ColorButton("MyColor##3b", color, misc_flags);
        ui::SameLine();
        open_popup |= ui::Button("Palette");
        if (open_popup)
        {
            ui::OpenPopup("mypicker");
            backup_color = color;
        }
        if (ui::BeginPopup("mypicker"))
        {
            ui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
            ui::Separator();
            ui::ColorPicker4("##picker", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
            ui::SameLine();

            ui::BeginGroup(); // Lock X position
            ui::Text("Current");
            ui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60,40));
            ui::Text("Previous");
            if (ui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60,40)))
                color = backup_color;
            ui::Separator();
            ui::Text("Palette");
            for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
            {
                ui::PushID(n);
                if ((n % 8) != 0)
                    ui::SameLine(0.0f, ui::GetStyle().ItemSpacing.y);
                if (ui::ColorButton("##palette", saved_palette[n], ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip, ImVec2(20,20)))
                    color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

                // Allow user to drop colors into each palette entry
                // (Note that ColorButton is already a drag source by default, unless using ImGuiColorEditFlags_NoDragDrop)
                if (ui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                    if (const ImGuiPayload* payload = ui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                        memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                    ui::EndDragDropTarget();
                }

                ui::PopID();
            }
            ui::EndGroup();
            ui::EndPopup();
        }

        ui::Text("Color button only:");
        ui::ColorButton("MyColor##3c", *(ImVec4*)&color, misc_flags, ImVec2(80,80));

        ui::Text("Color picker:");
        static bool alpha = true;
        static bool alpha_bar = true;
        static bool side_preview = true;
        static bool ref_color = false;
        static ImVec4 ref_color_v(1.0f,0.0f,1.0f,0.5f);
        static int inputs_mode = 2;
        static int picker_mode = 0;
        ui::Checkbox("With Alpha", &alpha);
        ui::Checkbox("With Alpha Bar", &alpha_bar);
        ui::Checkbox("With Side Preview", &side_preview);
        if (side_preview)
        {
            ui::SameLine();
            ui::Checkbox("With Ref Color", &ref_color);
            if (ref_color)
            {
                ui::SameLine();
                ui::ColorEdit4("##RefColor", &ref_color_v.x, ImGuiColorEditFlags_NoInputs | misc_flags);
            }
        }
        ui::Combo("Inputs Mode", &inputs_mode, "All Inputs\0No Inputs\0RGB Input\0HSV Input\0HEX Input\0");
        ui::Combo("Picker Mode", &picker_mode, "Auto/Current\0Hue bar + SV rect\0Hue wheel + SV triangle\0");
        ui::SameLine(); ShowHelpMarker("User can right-click the picker to change mode.");
        ImGuiColorEditFlags flags = misc_flags;
        if (!alpha) flags |= ImGuiColorEditFlags_NoAlpha; // This is by default if you call ColorPicker3() instead of ColorPicker4()
        if (alpha_bar) flags |= ImGuiColorEditFlags_AlphaBar;
        if (!side_preview) flags |= ImGuiColorEditFlags_NoSidePreview;
        if (picker_mode == 1) flags |= ImGuiColorEditFlags_PickerHueBar;
        if (picker_mode == 2) flags |= ImGuiColorEditFlags_PickerHueWheel;
        if (inputs_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;
        if (inputs_mode == 2) flags |= ImGuiColorEditFlags_RGB;
        if (inputs_mode == 3) flags |= ImGuiColorEditFlags_HSV;
        if (inputs_mode == 4) flags |= ImGuiColorEditFlags_HEX;
        ui::ColorPicker4("MyColor##4", (float*)&color, flags, ref_color ? &ref_color_v.x : NULL);

        ui::Text("Programmatically set defaults:");
        ui::SameLine(); ShowHelpMarker("SetColorEditOptions() is designed to allow you to set boot-time default.\nWe don't have Push/Pop functions because you can force options on a per-widget basis if needed, and the user can change non-forced ones with the options menu.\nWe don't have a getter to avoid encouraging you to persistently save values that aren't forward-compatible.");
        if (ui::Button("Default: Uint8 + HSV + Hue Bar"))
            ui::SetColorEditOptions(ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_PickerHueBar);
        if (ui::Button("Default: Float + HDR + Hue Wheel"))
            ui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);

        ui::TreePop();
    }

    if (ui::TreeNode("Range Widgets"))
    {
        static float begin = 10, end = 90;
        static int begin_i = 100, end_i = 1000;
        ui::DragFloatRange2("range", &begin, &end, 0.25f, 0.0f, 100.0f, "Min: %.1f %%", "Max: %.1f %%");
        ui::DragIntRange2("range int (no bounds)", &begin_i, &end_i, 5, 0, 0, "Min: %d units", "Max: %d units");
        ui::TreePop();
    }

    if (ui::TreeNode("Data Types"))
    {
        // The DragScalar/InputScalar/SliderScalar functions allow various data types: signed/unsigned int/long long and float/double
        // To avoid polluting the public API with all possible combinations, we use the ImGuiDataType enum to pass the type,
        // and passing all arguments by address.
        // This is the reason the test code below creates local variables to hold "zero" "one" etc. for each types.
        // In practice, if you frequently use a given type that is not covered by the normal API entry points, you can wrap it
        // yourself inside a 1 line function which can take typed argument as value instead of void*, and then pass their address
        // to the generic function. For example:
        //   bool MySliderU64(const char *label, u64* value, u64 min = 0, u64 max = 0, const char* format = "%lld")
        //   {
        //      return SliderScalar(label, ImGuiDataType_U64, value, &min, &max, format);
        //   }

        // Limits (as helper variables that we can take the address of)
        // Note that the SliderScalar function has a maximum usable range of half the natural type maximum, hence the /2 below.
        #ifndef LLONG_MIN
        ImS64 LLONG_MIN = -9223372036854775807LL - 1;
        ImS64 LLONG_MAX = 9223372036854775807LL;
        ImU64 ULLONG_MAX = (2ULL * 9223372036854775807LL + 1);
        #endif
        const ImS32   s32_zero = 0,   s32_one = 1,   s32_fifty = 50, s32_min = INT_MIN/2,   s32_max = INT_MAX/2,    s32_hi_a = INT_MAX/2 - 100,    s32_hi_b = INT_MAX/2;
        const ImU32   u32_zero = 0,   u32_one = 1,   u32_fifty = 50, u32_min = 0,           u32_max = UINT_MAX/2,   u32_hi_a = UINT_MAX/2 - 100,   u32_hi_b = UINT_MAX/2;
        const ImS64   s64_zero = 0,   s64_one = 1,   s64_fifty = 50, s64_min = LLONG_MIN/2, s64_max = LLONG_MAX/2,  s64_hi_a = LLONG_MAX/2 - 100,  s64_hi_b = LLONG_MAX/2;
        const ImU64   u64_zero = 0,   u64_one = 1,   u64_fifty = 50, u64_min = 0,           u64_max = ULLONG_MAX/2, u64_hi_a = ULLONG_MAX/2 - 100, u64_hi_b = ULLONG_MAX/2;
        const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
        const double  f64_zero = 0.,  f64_one = 1.,  f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

        // State
        static ImS32  s32_v = -1;
        static ImU32  u32_v = (ImU32)-1;
        static ImS64  s64_v = -1;
        static ImU64  u64_v = (ImU64)-1;
        static float  f32_v = 0.123f;
        static double f64_v = 90000.01234567890123456789;

        const float drag_speed = 0.2f;
        static bool drag_clamp = false;
        ui::Text("Drags:");
        ui::Checkbox("Clamp integers to 0..50", &drag_clamp); ui::SameLine(); ShowHelpMarker("As with every widgets in dear imgui, we never modify values unless there is a user interaction.\nYou can override the clamping limits by using CTRL+Click to input a value.");
        ui::DragScalar("drag s32",       ImGuiDataType_S32,    &s32_v, drag_speed, drag_clamp ? &s32_zero : NULL, drag_clamp ? &s32_fifty : NULL);
        ui::DragScalar("drag u32",       ImGuiDataType_U32,    &u32_v, drag_speed, drag_clamp ? &u32_zero : NULL, drag_clamp ? &u32_fifty : NULL, "%u ms");
        ui::DragScalar("drag s64",       ImGuiDataType_S64,    &s64_v, drag_speed, drag_clamp ? &s64_zero : NULL, drag_clamp ? &s64_fifty : NULL);
        ui::DragScalar("drag u64",       ImGuiDataType_U64,    &u64_v, drag_speed, drag_clamp ? &u64_zero : NULL, drag_clamp ? &u64_fifty : NULL);
        ui::DragScalar("drag float",     ImGuiDataType_Float,  &f32_v, 0.005f,  &f32_zero, &f32_one, "%f", 1.0f);
        ui::DragScalar("drag float ^2",  ImGuiDataType_Float,  &f32_v, 0.005f,  &f32_zero, &f32_one, "%f", 2.0f); ui::SameLine(); ShowHelpMarker("You can use the 'power' parameter to increase tweaking precision on one side of the range.");
        ui::DragScalar("drag double",    ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, NULL,     "%.10f grams", 1.0f);
        ui::DragScalar("drag double ^2", ImGuiDataType_Double, &f64_v, 0.0005f, &f64_zero, &f64_one, "0 < %.10f < 1", 2.0f);

        ui::Text("Sliders");
        ui::SliderScalar("slider s32 low",     ImGuiDataType_S32,    &s32_v, &s32_zero, &s32_fifty,"%d");
        ui::SliderScalar("slider s32 high",    ImGuiDataType_S32,    &s32_v, &s32_hi_a, &s32_hi_b, "%d");
        ui::SliderScalar("slider s32 full",    ImGuiDataType_S32,    &s32_v, &s32_min,  &s32_max,  "%d");
        ui::SliderScalar("slider u32 low",     ImGuiDataType_U32,    &u32_v, &u32_zero, &u32_fifty,"%u");
        ui::SliderScalar("slider u32 high",    ImGuiDataType_U32,    &u32_v, &u32_hi_a, &u32_hi_b, "%u");
        ui::SliderScalar("slider u32 full",    ImGuiDataType_U32,    &u32_v, &u32_min,  &u32_max,  "%u");
        ui::SliderScalar("slider s64 low",     ImGuiDataType_S64,    &s64_v, &s64_zero, &s64_fifty,"%I64d");
        ui::SliderScalar("slider s64 high",    ImGuiDataType_S64,    &s64_v, &s64_hi_a, &s64_hi_b, "%I64d");
        ui::SliderScalar("slider s64 full",    ImGuiDataType_S64,    &s64_v, &s64_min,  &s64_max,  "%I64d");
        ui::SliderScalar("slider u64 low",     ImGuiDataType_U64,    &u64_v, &u64_zero, &u64_fifty,"%I64u ms");
        ui::SliderScalar("slider u64 high",    ImGuiDataType_U64,    &u64_v, &u64_hi_a, &u64_hi_b, "%I64u ms");
        ui::SliderScalar("slider u64 full",    ImGuiDataType_U64,    &u64_v, &u64_min,  &u64_max,  "%I64u ms");
        ui::SliderScalar("slider float low",   ImGuiDataType_Float,  &f32_v, &f32_zero, &f32_one);
        ui::SliderScalar("slider float low^2", ImGuiDataType_Float,  &f32_v, &f32_zero, &f32_one,  "%.10f", 2.0f);
        ui::SliderScalar("slider float high",  ImGuiDataType_Float,  &f32_v, &f32_lo_a, &f32_hi_a, "%e");
        ui::SliderScalar("slider double low",  ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one,  "%.10f grams", 1.0f);
        ui::SliderScalar("slider double low^2",ImGuiDataType_Double, &f64_v, &f64_zero, &f64_one,  "%.10f", 2.0f);
        ui::SliderScalar("slider double high", ImGuiDataType_Double, &f64_v, &f64_lo_a, &f64_hi_a, "%e grams", 1.0f);

        static bool inputs_step = true;
        ui::Text("Inputs");
        ui::Checkbox("Show step buttons", &inputs_step);
        ui::InputScalar("input s32",     ImGuiDataType_S32,    &s32_v, inputs_step ? &s32_one : NULL, NULL, "%d");
        ui::InputScalar("input s32 hex", ImGuiDataType_S32,    &s32_v, inputs_step ? &s32_one : NULL, NULL, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
        ui::InputScalar("input u32",     ImGuiDataType_U32,    &u32_v, inputs_step ? &u32_one : NULL, NULL, "%u");
        ui::InputScalar("input u32 hex", ImGuiDataType_U32,    &u32_v, inputs_step ? &u32_one : NULL, NULL, "%08X", ImGuiInputTextFlags_CharsHexadecimal);
        ui::InputScalar("input s64",     ImGuiDataType_S64,    &s64_v, inputs_step ? &s64_one : NULL);
        ui::InputScalar("input u64",     ImGuiDataType_U64,    &u64_v, inputs_step ? &u64_one : NULL);
        ui::InputScalar("input float",   ImGuiDataType_Float,  &f32_v, inputs_step ? &f32_one : NULL);
        ui::InputScalar("input double",  ImGuiDataType_Double, &f64_v, inputs_step ? &f64_one : NULL);

        ui::TreePop();
    }

    if (ui::TreeNode("Multi-component Widgets"))
    {
        static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
        static int vec4i[4] = { 1, 5, 100, 255 };

        ui::InputFloat2("input float2", vec4f);
        ui::DragFloat2("drag float2", vec4f, 0.01f, 0.0f, 1.0f);
        ui::SliderFloat2("slider float2", vec4f, 0.0f, 1.0f);
        ui::InputInt2("input int2", vec4i);
        ui::DragInt2("drag int2", vec4i, 1, 0, 255);
        ui::SliderInt2("slider int2", vec4i, 0, 255);
        ui::Spacing();

        ui::InputFloat3("input float3", vec4f);
        ui::DragFloat3("drag float3", vec4f, 0.01f, 0.0f, 1.0f);
        ui::SliderFloat3("slider float3", vec4f, 0.0f, 1.0f);
        ui::InputInt3("input int3", vec4i);
        ui::DragInt3("drag int3", vec4i, 1, 0, 255);
        ui::SliderInt3("slider int3", vec4i, 0, 255);
        ui::Spacing();

        ui::InputFloat4("input float4", vec4f);
        ui::DragFloat4("drag float4", vec4f, 0.01f, 0.0f, 1.0f);
        ui::SliderFloat4("slider float4", vec4f, 0.0f, 1.0f);
        ui::InputInt4("input int4", vec4i);
        ui::DragInt4("drag int4", vec4i, 1, 0, 255);
        ui::SliderInt4("slider int4", vec4i, 0, 255);

        ui::TreePop();
    }

    if (ui::TreeNode("Vertical Sliders"))
    {
        const float spacing = 4;
        ui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

        static int int_value = 0;
        ui::VSliderInt("##int", ImVec2(18,160), &int_value, 0, 5);
        ui::SameLine();

        static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
        ui::PushID("set1");
        for (int i = 0; i < 7; i++)
        {
            if (i > 0) ui::SameLine();
            ui::PushID(i);
            ui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i/7.0f, 0.5f, 0.5f));
            ui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i/7.0f, 0.6f, 0.5f));
            ui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.5f));
            ui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i/7.0f, 0.9f, 0.9f));
            ui::VSliderFloat("##v", ImVec2(18,160), &values[i], 0.0f, 1.0f, "");
            if (ui::IsItemActive() || ui::IsItemHovered())
                ui::SetTooltip("%.3f", values[i]);
            ui::PopStyleColor(4);
            ui::PopID();
        }
        ui::PopID();

        ui::SameLine();
        ui::PushID("set2");
        static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
        const int rows = 3;
        const ImVec2 small_slider_size(18, (160.0f-(rows-1)*spacing)/rows);
        for (int nx = 0; nx < 4; nx++)
        {
            if (nx > 0) ui::SameLine();
            ui::BeginGroup();
            for (int ny = 0; ny < rows; ny++)
            {
                ui::PushID(nx*rows+ny);
                ui::VSliderFloat("##v", small_slider_size, &values2[nx], 0.0f, 1.0f, "");
                if (ui::IsItemActive() || ui::IsItemHovered())
                    ui::SetTooltip("%.3f", values2[nx]);
                ui::PopID();
            }
            ui::EndGroup();
        }
        ui::PopID();

        ui::SameLine();
        ui::PushID("set3");
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) ui::SameLine();
            ui::PushID(i);
            ui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
            ui::VSliderFloat("##v", ImVec2(40,160), &values[i], 0.0f, 1.0f, "%.2f\nsec");
            ui::PopStyleVar();
            ui::PopID();
        }
        ui::PopID();
        ui::PopStyleVar();
        ui::TreePop();
    }

    if (ui::TreeNode("Drag and Drop"))
    {
        {
            // ColorEdit widgets automatically act as drag source and drag target.
            // They are using standardized payload strings IMGUI_PAYLOAD_TYPE_COLOR_3F and IMGUI_PAYLOAD_TYPE_COLOR_4F to allow your own widgets
            // to use colors in their drag and drop interaction. Also see the demo in Color Picker -> Palette demo.
            ui::BulletText("Drag and drop in standard widgets");
            ui::Indent();
            static float col1[3] = { 1.0f,0.0f,0.2f };
            static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
            ui::ColorEdit3("color 1", col1);
            ui::ColorEdit4("color 2", col2);
            ui::Unindent();
        }

        {
            ui::BulletText("Drag and drop to copy/swap items");
            ui::Indent();
            enum Mode
            {
                Mode_Copy,
                Mode_Move,
                Mode_Swap
            };
            static int mode = 0;
            if (ui::RadioButton("Copy", mode == Mode_Copy)) { mode = Mode_Copy; } ui::SameLine();
            if (ui::RadioButton("Move", mode == Mode_Move)) { mode = Mode_Move; } ui::SameLine();
            if (ui::RadioButton("Swap", mode == Mode_Swap)) { mode = Mode_Swap; }
            static const char* names[9] = { "Bobby", "Beatrice", "Betty", "Brianna", "Barry", "Bernard", "Bibi", "Blaine", "Bryn" };
            for (int n = 0; n < IM_ARRAYSIZE(names); n++)
            {
                ui::PushID(n);
                if ((n % 3) != 0)
                    ui::SameLine();
                ui::Button(names[n], ImVec2(60,60));

                // Our buttons are both drag sources and drag targets here!
                if (ui::BeginDragDropSource(ImGuiDragDropFlags_None))
                {
                    ui::SetDragDropPayload("DND_DEMO_CELL", &n, sizeof(int));        // Set payload to carry the index of our item (could be anything)
                    if (mode == Mode_Copy) { ui::Text("Copy %s", names[n]); }        // Display preview (could be anything, e.g. when dragging an image we could decide to display the filename and a small preview of the image, etc.)
                    if (mode == Mode_Move) { ui::Text("Move %s", names[n]); }
                    if (mode == Mode_Swap) { ui::Text("Swap %s", names[n]); }
                    ui::EndDragDropSource();
                }
                if (ui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ui::AcceptDragDropPayload("DND_DEMO_CELL"))
                    {
                        IM_ASSERT(payload->DataSize == sizeof(int));
                        int payload_n = *(const int*)payload->Data;
                        if (mode == Mode_Copy)
                        {
                            names[n] = names[payload_n];
                        }
                        if (mode == Mode_Move)
                        {
                            names[n] = names[payload_n];
                            names[payload_n] = "";
                        }
                        if (mode == Mode_Swap)
                        {
                            const char* tmp = names[n];
                            names[n] = names[payload_n];
                            names[payload_n] = tmp;
                        }
                    }
                    ui::EndDragDropTarget();
                }
                ui::PopID();
            }
            ui::Unindent();
        }

        ui::TreePop();
    }

    if (ui::TreeNode("Querying Status (Active/Focused/Hovered etc.)"))
    {
        // Display the value of IsItemHovered() and other common item state functions. Note that the flags can be combined.
        // (because BulletText is an item itself and that would affect the output of IsItemHovered() we pass all state in a single call to simplify the code).
        static int item_type = 1;
        static bool b = false;
        static float col4f[4] = { 1.0f, 0.5, 0.0f, 1.0f };
        ui::RadioButton("Text", &item_type, 0);
        ui::RadioButton("Button", &item_type, 1);
        ui::RadioButton("Checkbox", &item_type, 2);
        ui::RadioButton("SliderFloat", &item_type, 3);
        ui::RadioButton("ColorEdit4", &item_type, 4);
        ui::RadioButton("ListBox", &item_type, 5);
        ui::Separator();
        bool ret = false;
        if (item_type == 0) { ui::Text("ITEM: Text"); }                                              // Testing text items with no identifier/interaction
        if (item_type == 1) { ret = ui::Button("ITEM: Button"); }                                    // Testing button
        if (item_type == 2) { ret = ui::Checkbox("ITEM: Checkbox", &b); }                            // Testing checkbox
        if (item_type == 3) { ret = ui::SliderFloat("ITEM: SliderFloat", &col4f[0], 0.0f, 1.0f); }   // Testing basic item
        if (item_type == 4) { ret = ui::ColorEdit4("ITEM: ColorEdit4", col4f); }                     // Testing multi-component items (IsItemXXX flags are reported merged)
        if (item_type == 5) { const char* items[] = { "Apple", "Banana", "Cherry", "Kiwi" }; static int current = 1; ret = ui::ListBox("ITEM: ListBox", &current, items, IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)); }
        ui::BulletText(
            "Return value = %d\n"
            "IsItemFocused() = %d\n"
            "IsItemHovered() = %d\n"
            "IsItemHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsItemHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsItemHovered(_AllowWhenOverlapped) = %d\n"
            "IsItemHovered(_RectOnly) = %d\n"
            "IsItemActive() = %d\n"
            "IsItemEdited() = %d\n"
            "IsItemActivated() = %d\n"
            "IsItemDeactivated() = %d\n"
            "IsItemDeactivatedEdit() = %d\n"
            "IsItemVisible() = %d\n"
            "GetItemRectMin() = (%.1f, %.1f)\n"
            "GetItemRectMax() = (%.1f, %.1f)\n"
            "GetItemRectSize() = (%.1f, %.1f)",
            ret,
            ui::IsItemFocused(),
            ui::IsItemHovered(),
            ui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            ui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
            ui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped),
            ui::IsItemHovered(ImGuiHoveredFlags_RectOnly),
            ui::IsItemActive(),
            ui::IsItemEdited(),
            ui::IsItemActivated(),
            ui::IsItemDeactivated(),
            ui::IsItemDeactivatedAfterEdit(),
            ui::IsItemVisible(),
            ui::GetItemRectMin().x, ui::GetItemRectMin().y,
            ui::GetItemRectMax().x, ui::GetItemRectMax().y,
            ui::GetItemRectSize().x, ui::GetItemRectSize().y
        );

        static bool embed_all_inside_a_child_window = false;
        ui::Checkbox("Embed everything inside a child window (for additional testing)", &embed_all_inside_a_child_window);
        if (embed_all_inside_a_child_window)
            ui::BeginChild("outer_child", ImVec2(0, ui::GetFontSize() * 20), true);

        // Testing IsWindowFocused() function with its various flags. Note that the flags can be combined.
        ui::BulletText(
            "IsWindowFocused() = %d\n"
            "IsWindowFocused(_ChildWindows) = %d\n"
            "IsWindowFocused(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowFocused(_RootWindow) = %d\n"
            "IsWindowFocused(_AnyWindow) = %d\n",
            ui::IsWindowFocused(),
            ui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows),
            ui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_RootWindow),
            ui::IsWindowFocused(ImGuiFocusedFlags_RootWindow),
            ui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow));

        // Testing IsWindowHovered() function with its various flags. Note that the flags can be combined.
        ui::BulletText(
            "IsWindowHovered() = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByPopup) = %d\n"
            "IsWindowHovered(_AllowWhenBlockedByActiveItem) = %d\n"
            "IsWindowHovered(_ChildWindows) = %d\n"
            "IsWindowHovered(_ChildWindows|_RootWindow) = %d\n"
            "IsWindowHovered(_RootWindow) = %d\n"
            "IsWindowHovered(_AnyWindow) = %d\n",
            ui::IsWindowHovered(),
            ui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup),
            ui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem),
            ui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows),
            ui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_RootWindow),
            ui::IsWindowHovered(ImGuiHoveredFlags_RootWindow),
            ui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow));

        ui::BeginChild("child", ImVec2(0, 50), true);
        ui::Text("This is another child window for testing the _ChildWindows flag.");
        ui::EndChild();
        if (embed_all_inside_a_child_window)
            ui::EndChild();

        // Calling IsItemHovered() after begin returns the hovered status of the title bar.
        // This is useful in particular if you want to create a context menu (with BeginPopupContextItem) associated to the title bar of a window.
        static bool test_window = false;
        ui::Checkbox("Hovered/Active tests after Begin() for title bar testing", &test_window);
        if (test_window)
        {
            ui::Begin("Title bar Hovered/Active tests", &test_window);
            if (ui::BeginPopupContextItem()) // <-- This is using IsItemHovered()
            {
                if (ui::MenuItem("Close")) { test_window = false; }
                ui::EndPopup();
            }
            ui::Text(
                "IsItemHovered() after begin = %d (== is title bar hovered)\n"
                "IsItemActive() after begin = %d (== is window being clicked/moved)\n",
                ui::IsItemHovered(), ui::IsItemActive());
            ui::End();
        }

        ui::TreePop();
    }
}

static void ShowDemoWindowLayout()
{
    if (!ui::CollapsingHeader("Layout"))
        return;

    if (ui::TreeNode("Child windows"))
    {
        ShowHelpMarker("Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window.");
        static bool disable_mouse_wheel = false;
        static bool disable_menu = false;
        ui::Checkbox("Disable Mouse Wheel", &disable_mouse_wheel);
        ui::Checkbox("Disable Menu", &disable_menu);

        static int line = 50;
        bool goto_line = ui::Button("Goto");
        ui::SameLine();
        ui::PushItemWidth(100);
        goto_line |= ui::InputInt("##Line", &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
        ui::PopItemWidth();

        // Child 1: no border, enable horizontal scrollbar
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar | (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0);
            ui::BeginChild("Child1", ImVec2(ui::GetWindowContentRegionWidth() * 0.5f, 260), false, window_flags);
            for (int i = 0; i < 100; i++)
            {
                ui::Text("%04d: scrollable region", i);
                if (goto_line && line == i)
                    ui::SetScrollHereY();
            }
            if (goto_line && line >= 100)
                ui::SetScrollHereY();
            ui::EndChild();
        }

        ui::SameLine();

        // Child 2: rounded border
        {
            ImGuiWindowFlags window_flags = (disable_mouse_wheel ? ImGuiWindowFlags_NoScrollWithMouse : 0) | (disable_menu ? 0 : ImGuiWindowFlags_MenuBar);
            ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ui::BeginChild("Child2", ImVec2(0, 260), true, window_flags);
            if (!disable_menu && ui::BeginMenuBar())
            {
                if (ui::BeginMenu("Menu"))
                {
                    ShowExampleMenuFile();
                    ui::EndMenu();
                }
                ui::EndMenuBar();
            }
            ui::Columns(2);
            for (int i = 0; i < 100; i++)
            {
                char buf[32];
                sprintf(buf, "%03d", i);
                ui::Button(buf, ImVec2(-1.0f, 0.0f));
                ui::NextColumn();
            }
            ui::EndChild();
            ui::PopStyleVar();
        }

        ui::Separator();

        // Demonstrate a few extra things
        // - Changing ImGuiCol_ChildBg (which is transparent black in default styles)
        // - Using SetCursorPos() to position the child window (because the child window is an item from the POV of the parent window)
        //   You can also call SetNextWindowPos() to position the child window. The parent window will effectively layout from this position.
        // - Using ImGui::GetItemRectMin/Max() to query the "item" state (because the child window is an item from the POV of the parent window)
        //   See "Widgets" -> "Querying Status (Active/Focused/Hovered etc.)" section for more details about this.
        {
            ui::SetCursorPosX(50);
            ui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));
            ui::BeginChild("blah", ImVec2(200, 100), true, ImGuiWindowFlags_None);
            for (int n = 0; n < 50; n++)
                ui::Text("Some test %d", n);
            ui::EndChild();
            ImVec2 child_rect_min = ui::GetItemRectMin();
            ImVec2 child_rect_max = ui::GetItemRectMax();
            ui::PopStyleColor();
            ui::Text("Rect of child window is: (%.0f,%.0f) (%.0f,%.0f)", child_rect_min.x, child_rect_min.y, child_rect_max.x, child_rect_max.y);
        }

        ui::TreePop();
    }

    if (ui::TreeNode("Widgets Width"))
    {
        static float f = 0.0f;
        ui::Text("PushItemWidth(100)");
        ui::SameLine(); ShowHelpMarker("Fixed width.");
        ui::PushItemWidth(100);
        ui::DragFloat("float##1", &f);
        ui::PopItemWidth();

        ui::Text("PushItemWidth(GetWindowWidth() * 0.5f)");
        ui::SameLine(); ShowHelpMarker("Half of window width.");
        ui::PushItemWidth(ui::GetWindowWidth() * 0.5f);
        ui::DragFloat("float##2", &f);
        ui::PopItemWidth();

        ui::Text("PushItemWidth(GetContentRegionAvailWidth() * 0.5f)");
        ui::SameLine(); ShowHelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
        ui::PushItemWidth(ui::GetContentRegionAvailWidth() * 0.5f);
        ui::DragFloat("float##3", &f);
        ui::PopItemWidth();

        ui::Text("PushItemWidth(-100)");
        ui::SameLine(); ShowHelpMarker("Align to right edge minus 100");
        ui::PushItemWidth(-100);
        ui::DragFloat("float##4", &f);
        ui::PopItemWidth();

        ui::Text("PushItemWidth(-1)");
        ui::SameLine(); ShowHelpMarker("Align to right edge");
        ui::PushItemWidth(-1);
        ui::DragFloat("float##5", &f);
        ui::PopItemWidth();

        ui::TreePop();
    }

    if (ui::TreeNode("Basic Horizontal Layout"))
    {
        ui::TextWrapped("(Use ImGui::SameLine() to keep adding items to the right of the preceding item)");

        // Text
        ui::Text("Two items: Hello"); ui::SameLine();
        ui::TextColored(ImVec4(1,1,0,1), "Sailor");

        // Adjust spacing
        ui::Text("More spacing: Hello"); ui::SameLine(0, 20);
        ui::TextColored(ImVec4(1,1,0,1), "Sailor");

        // Button
        ui::AlignTextToFramePadding();
        ui::Text("Normal buttons"); ui::SameLine();
        ui::Button("Banana"); ui::SameLine();
        ui::Button("Apple"); ui::SameLine();
        ui::Button("Corniflower");

        // Button
        ui::Text("Small buttons"); ui::SameLine();
        ui::SmallButton("Like this one"); ui::SameLine();
        ui::Text("can fit within a text block.");

        // Aligned to arbitrary position. Easy/cheap column.
        ui::Text("Aligned");
        ui::SameLine(150); ui::Text("x=150");
        ui::SameLine(300); ui::Text("x=300");
        ui::Text("Aligned");
        ui::SameLine(150); ui::SmallButton("x=150");
        ui::SameLine(300); ui::SmallButton("x=300");

        // Checkbox
        static bool c1 = false, c2 = false, c3 = false, c4 = false;
        ui::Checkbox("My", &c1); ui::SameLine();
        ui::Checkbox("Tailor", &c2); ui::SameLine();
        ui::Checkbox("Is", &c3); ui::SameLine();
        ui::Checkbox("Rich", &c4);

        // Various
        static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
        ui::PushItemWidth(80);
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
        static int item = -1;
        ui::Combo("Combo", &item, items, IM_ARRAYSIZE(items)); ui::SameLine();
        ui::SliderFloat("X", &f0, 0.0f, 5.0f); ui::SameLine();
        ui::SliderFloat("Y", &f1, 0.0f, 5.0f); ui::SameLine();
        ui::SliderFloat("Z", &f2, 0.0f, 5.0f);
        ui::PopItemWidth();

        ui::PushItemWidth(80);
        ui::Text("Lists:");
        static int selection[4] = { 0, 1, 2, 3 };
        for (int i = 0; i < 4; i++)
        {
            if (i > 0) ui::SameLine();
            ui::PushID(i);
            ui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
            ui::PopID();
            //if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
        }
        ui::PopItemWidth();

        // Dummy
        ImVec2 button_sz(40, 40);
        ui::Button("A", button_sz); ui::SameLine();
        ui::Dummy(button_sz); ui::SameLine();
        ui::Button("B", button_sz);

        // Manually wrapping (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
        ui::Text("Manually wrapping:");
        ImGuiStyle& style = ui::GetStyle();
        int buttons_count = 20;
        float window_visible_x2 = ui::GetWindowPos().x + ui::GetWindowContentRegionMax().x;
        for (int n = 0; n < buttons_count; n++)
        {
            ui::PushID(n);
            ui::Button("Box", button_sz);
            float last_button_x2 = ui::GetItemRectMax().x;
            float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
            if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
                ui::SameLine();
            ui::PopID();
        }

        ui::TreePop();
    }

    if (ui::TreeNode("Tabs"))
    {
        if (ui::TreeNode("Basic"))
        {
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
            if (ui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                if (ui::BeginTabItem("Avocado"))
                {
                    ui::Text("This is the Avocado tab!\nblah blah blah blah blah");
                    ui::EndTabItem();
                }
                if (ui::BeginTabItem("Broccoli"))
                {
                    ui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                    ui::EndTabItem();
                }
                if (ui::BeginTabItem("Cucumber"))
                {
                    ui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
                    ui::EndTabItem();
                }
                ui::EndTabBar();
            }
            ui::Separator();
            ui::TreePop();
        }

        if (ui::TreeNode("Advanced & Close Button"))
        {
            // Expose a couple of the available flags. In most cases you may just call BeginTabBar() with no flags (0).
            static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
            ui::CheckboxFlags("ImGuiTabBarFlags_Reorderable", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_Reorderable);
            ui::CheckboxFlags("ImGuiTabBarFlags_AutoSelectNewTabs", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_AutoSelectNewTabs);
            ui::CheckboxFlags("ImGuiTabBarFlags_TabListPopupButton", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_TabListPopupButton);
            ui::CheckboxFlags("ImGuiTabBarFlags_NoCloseWithMiddleMouseButton", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_NoCloseWithMiddleMouseButton);
            if ((tab_bar_flags & ImGuiTabBarFlags_FittingPolicyMask_) == 0)
                tab_bar_flags |= ImGuiTabBarFlags_FittingPolicyDefault_;
            if (ui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyResizeDown", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_FittingPolicyResizeDown))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyResizeDown);
            if (ui::CheckboxFlags("ImGuiTabBarFlags_FittingPolicyScroll", (unsigned int*)&tab_bar_flags, ImGuiTabBarFlags_FittingPolicyScroll))
                tab_bar_flags &= ~(ImGuiTabBarFlags_FittingPolicyMask_ ^ ImGuiTabBarFlags_FittingPolicyScroll);

            // Tab Bar
            const char* names[4] = { "Artichoke", "Beetroot", "Celery", "Daikon" };
            static bool opened[4] = { true, true, true, true }; // Persistent user state
            for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
            {
                if (n > 0) { ui::SameLine(); }
                ui::Checkbox(names[n], &opened[n]);
            }

            // Passing a bool* to BeginTabItem() is similar to passing one to Begin(): the underlying bool will be set to false when the tab is closed.
            if (ui::BeginTabBar("MyTabBar", tab_bar_flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(opened); n++)
                    if (opened[n] && ui::BeginTabItem(names[n], &opened[n]))
                    {
                        ui::Text("This is the %s tab!", names[n]);
                        if (n & 1)
                            ui::Text("I am an odd tab.");
                        ui::EndTabItem();
                    }
                ui::EndTabBar();
            }
            ui::Separator();
            ui::TreePop();
        }
        ui::TreePop();
    }

    if (ui::TreeNode("Groups"))
    {
        ShowHelpMarker("Using ImGui::BeginGroup()/EndGroup() to layout items. BeginGroup() basically locks the horizontal position. EndGroup() bundles the whole group so that you can use functions such as IsItemHovered() on it.");
        ui::BeginGroup();
        {
            ui::BeginGroup();
            ui::Button("AAA");
            ui::SameLine();
            ui::Button("BBB");
            ui::SameLine();
            ui::BeginGroup();
            ui::Button("CCC");
            ui::Button("DDD");
            ui::EndGroup();
            ui::SameLine();
            ui::Button("EEE");
            ui::EndGroup();
            if (ui::IsItemHovered())
                ui::SetTooltip("First group hovered");
        }
        // Capture the group size and create widgets using the same size
        ImVec2 size = ui::GetItemRectSize();
        const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
        ui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

        ui::Button("ACTION", ImVec2((size.x - ui::GetStyle().ItemSpacing.x)*0.5f, size.y));
        ui::SameLine();
        ui::Button("REACTION", ImVec2((size.x - ui::GetStyle().ItemSpacing.x)*0.5f, size.y));
        ui::EndGroup();
        ui::SameLine();

        ui::Button("LEVERAGE\nBUZZWORD", size);
        ui::SameLine();

        if (ui::ListBoxHeader("List", size))
        {
            ui::Selectable("Selected", true);
            ui::Selectable("Not Selected", false);
            ui::ListBoxFooter();
        }

        ui::TreePop();
    }

    if (ui::TreeNode("Text Baseline Alignment"))
    {
        ShowHelpMarker("This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. Lines only composed of text or \"small\" widgets fit in less vertical spaces than lines with normal widgets.");

        ui::Text("One\nTwo\nThree"); ui::SameLine();
        ui::Text("Hello\nWorld"); ui::SameLine();
        ui::Text("Banana");

        ui::Text("Banana"); ui::SameLine();
        ui::Text("Hello\nWorld"); ui::SameLine();
        ui::Text("One\nTwo\nThree");

        ui::Button("HOP##1"); ui::SameLine();
        ui::Text("Banana"); ui::SameLine();
        ui::Text("Hello\nWorld"); ui::SameLine();
        ui::Text("Banana");

        ui::Button("HOP##2"); ui::SameLine();
        ui::Text("Hello\nWorld"); ui::SameLine();
        ui::Text("Banana");

        ui::Button("TEST##1"); ui::SameLine();
        ui::Text("TEST"); ui::SameLine();
        ui::SmallButton("TEST##2");

        ui::AlignTextToFramePadding(); // If your line starts with text, call this to align it to upcoming widgets.
        ui::Text("Text aligned to Widget"); ui::SameLine();
        ui::Button("Widget##1"); ui::SameLine();
        ui::Text("Widget"); ui::SameLine();
        ui::SmallButton("Widget##2"); ui::SameLine();
        ui::Button("Widget##3");

        // Tree
        const float spacing = ui::GetStyle().ItemInnerSpacing.x;
        ui::Button("Button##1");
        ui::SameLine(0.0f, spacing);
        if (ui::TreeNode("Node##1")) { for (int i = 0; i < 6; i++) ui::BulletText("Item %d..", i); ui::TreePop(); }    // Dummy tree data

        ui::AlignTextToFramePadding();         // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget. Otherwise you can use SmallButton (smaller fit).
        bool node_open = ui::TreeNode("Node##2");  // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add child content.
        ui::SameLine(0.0f, spacing); ui::Button("Button##2");
        if (node_open) { for (int i = 0; i < 6; i++) ui::BulletText("Item %d..", i); ui::TreePop(); }   // Dummy tree data

        // Bullet
        ui::Button("Button##3");
        ui::SameLine(0.0f, spacing);
        ui::BulletText("Bullet text");

        ui::AlignTextToFramePadding();
        ui::BulletText("Node");
        ui::SameLine(0.0f, spacing); ui::Button("Button##4");

        ui::TreePop();
    }

    if (ui::TreeNode("Scrolling"))
    {
        ShowHelpMarker("Use SetScrollHereY() or SetScrollFromPosY() to scroll to a given position.");

        static bool track = true;
        static int track_line = 50, scroll_to_px = 200;
        ui::Checkbox("Track", &track);
        ui::PushItemWidth(100);
        ui::SameLine(130); track |= ui::DragInt("##line", &track_line, 0.25f, 0, 99, "Line = %d");
        bool scroll_to = ui::Button("Scroll To Pos");
        ui::SameLine(130); scroll_to |= ui::DragInt("##pos_y", &scroll_to_px, 1.00f, 0, 9999, "Y = %d px");
        ui::PopItemWidth();
        if (scroll_to) track = false;

        for (int i = 0; i < 5; i++)
        {
            if (i > 0) ui::SameLine();
            ui::BeginGroup();
            ui::Text("%s", i == 0 ? "Top" : i == 1 ? "25%" : i == 2 ? "Center" : i == 3 ? "75%" : "Bottom");
            ui::BeginChild(ui::GetID((void*)(intptr_t)i), ImVec2(ui::GetWindowWidth() * 0.17f, 200.0f), true);
            if (scroll_to)
                ui::SetScrollFromPosY(ui::GetCursorStartPos().y + scroll_to_px, i * 0.25f);
            for (int line = 0; line < 100; line++)
            {
                if (track && line == track_line)
                {
                    ui::TextColored(ImVec4(1,1,0,1), "Line %d", line);
                    ui::SetScrollHereY(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
                }
                else
                {
                    ui::Text("Line %d", line);
                }
            }
            float scroll_y = ui::GetScrollY(), scroll_max_y = ui::GetScrollMaxY();
            ui::EndChild();
            ui::Text("%.0f/%0.f", scroll_y, scroll_max_y);
            ui::EndGroup();
        }
        ui::TreePop();
    }

    if (ui::TreeNode("Horizontal Scrolling"))
    {
        ShowHelpMarker("Horizontal scrolling for a window has to be enabled explicitly via the ImGuiWindowFlags_HorizontalScrollbar flag.\n\nYou may want to explicitly specify content width by calling SetNextWindowContentWidth() before Begin().");
        static int lines = 7;
        ui::SliderInt("Lines", &lines, 1, 15);
        ui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
        ui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
        ui::BeginChild("scrolling", ImVec2(0, ui::GetFrameHeightWithSpacing() * 7 + 30), true, ImGuiWindowFlags_HorizontalScrollbar);
        for (int line = 0; line < lines; line++)
        {
            // Display random stuff (for the sake of this trivial demo we are using basic Button+SameLine. If you want to create your own time line for a real application you may be better off
            // manipulating the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets yourself. You may also want to use the lower-level ImDrawList API)
            int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
            for (int n = 0; n < num_buttons; n++)
            {
                if (n > 0) ui::SameLine();
                ui::PushID(n + line * 1000);
                char num_buf[16];
                sprintf(num_buf, "%d", n);
                const char* label = (!(n%15)) ? "FizzBuzz" : (!(n%3)) ? "Fizz" : (!(n%5)) ? "Buzz" : num_buf;
                float hue = n*0.05f;
                ui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
                ui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
                ui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
                ui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
                ui::PopStyleColor(3);
                ui::PopID();
            }
        }
        float scroll_x = ui::GetScrollX(), scroll_max_x = ui::GetScrollMaxX();
        ui::EndChild();
        ui::PopStyleVar(2);
        float scroll_x_delta = 0.0f;
        ui::SmallButton("<<"); if (ui::IsItemActive()) { scroll_x_delta = -ui::GetIO().DeltaTime * 1000.0f; } ui::SameLine();
        ui::Text("Scroll from code"); ui::SameLine();
        ui::SmallButton(">>"); if (ui::IsItemActive()) { scroll_x_delta = +ui::GetIO().DeltaTime * 1000.0f; } ui::SameLine();
        ui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
        if (scroll_x_delta != 0.0f)
        {
            ui::BeginChild("scrolling"); // Demonstrate a trick: you can use Begin to set yourself in the context of another window (here we are already out of your child window)
            ui::SetScrollX(ui::GetScrollX() + scroll_x_delta);
            ui::EndChild();
        }
        ui::TreePop();
    }

    if (ui::TreeNode("Clipping"))
    {
        static ImVec2 size(100, 100), offset(50, 20);
        ui::TextWrapped("On a per-widget basis we are occasionally clipping text CPU-side if it won't fit in its frame. Otherwise we are doing coarser clipping + passing a scissor rectangle to the renderer. The system is designed to try minimizing both execution and CPU/GPU rendering cost.");
        ui::DragFloat2("size", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
        ui::TextWrapped("(Click and drag)");
        ImVec2 pos = ui::GetCursorScreenPos();
        ImVec4 clip_rect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
        ui::InvisibleButton("##dummy", size);
        if (ui::IsItemActive() && ui::IsMouseDragging()) { offset.x += ui::GetIO().MouseDelta.x; offset.y += ui::GetIO().MouseDelta.y; }
        ui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(90, 90, 120, 255));
        ui::GetWindowDrawList()->AddText(ui::GetFont(), ui::GetFontSize()*2.0f, ImVec2(pos.x + offset.x, pos.y + offset.y), IM_COL32(255, 255, 255, 255), "Line 1 hello\nLine 2 clip me!", NULL, 0.0f, &clip_rect);
        ui::TreePop();
    }
}

static void ShowDemoWindowPopups()
{
    if (!ui::CollapsingHeader("Popups & Modal windows"))
        return;

    // The properties of popups windows are:
    // - They block normal mouse hovering detection outside them. (*)
    // - Unless modal, they can be closed by clicking anywhere outside them, or by pressing ESCAPE.
    // - Their visibility state (~bool) is held internally by imgui instead of being held by the programmer as we are used to with regular Begin() calls.
    //   User can manipulate the visibility state by calling OpenPopup().
    // (*) One can use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) to bypass it and detect hovering even when normally blocked by a popup.
    // Those three properties are connected. The library needs to hold their visibility state because it can close popups at any time.

    // Typical use for regular windows:
    //   bool my_tool_is_active = false; if (ImGui::Button("Open")) my_tool_is_active = true; [...] if (my_tool_is_active) Begin("My Tool", &my_tool_is_active) { [...] } End();
    // Typical use for popups:
    //   if (ImGui::Button("Open")) ImGui::OpenPopup("MyPopup"); if (ImGui::BeginPopup("MyPopup") { [...] EndPopup(); }

    // With popups we have to go through a library call (here OpenPopup) to manipulate the visibility state.
    // This may be a bit confusing at first but it should quickly make sense. Follow on the examples below.

    if (ui::TreeNode("Popups"))
    {
        ui::TextWrapped("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it.");

        static int selected_fish = -1;
        const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
        static bool toggles[] = { true, false, false, false, false };

        // Simple selection popup
        // (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
        if (ui::Button("Select.."))
            ui::OpenPopup("my_select_popup");
        ui::SameLine();
        ui::TextUnformatted(selected_fish == -1 ? "<None>" : names[selected_fish]);
        if (ui::BeginPopup("my_select_popup"))
        {
            ui::Text("Aquarium");
            ui::Separator();
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                if (ui::Selectable(names[i]))
                    selected_fish = i;
            ui::EndPopup();
        }

        // Showing a menu with toggles
        if (ui::Button("Toggle.."))
            ui::OpenPopup("my_toggle_popup");
        if (ui::BeginPopup("my_toggle_popup"))
        {
            for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                ui::MenuItem(names[i], "", &toggles[i]);
            if (ui::BeginMenu("Sub-menu"))
            {
                ui::MenuItem("Click me");
                ui::EndMenu();
            }

            ui::Separator();
            ui::Text("Tooltip here");
            if (ui::IsItemHovered())
                ui::SetTooltip("I am a tooltip over a popup");

            if (ui::Button("Stacked Popup"))
                ui::OpenPopup("another popup");
            if (ui::BeginPopup("another popup"))
            {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ui::MenuItem(names[i], "", &toggles[i]);
                if (ui::BeginMenu("Sub-menu"))
                {
                    ui::MenuItem("Click me");
                    ui::EndMenu();
                }
                ui::EndPopup();
            }
            ui::EndPopup();
        }

        // Call the more complete ShowExampleMenuFile which we use in various places of this demo
        if (ui::Button("File Menu.."))
            ui::OpenPopup("my_file_popup");
        if (ui::BeginPopup("my_file_popup"))
        {
            ShowExampleMenuFile();
            ui::EndPopup();
        }

        ui::TreePop();
    }

    if (ui::TreeNode("Context menus"))
    {
        // BeginPopupContextItem() is a helper to provide common/simple popup behavior of essentially doing:
        //    if (IsItemHovered() && IsMouseReleased(0))
        //       OpenPopup(id);
        //    return BeginPopup(id);
        // For more advanced uses you may want to replicate and cuztomize this code. This the comments inside BeginPopupContextItem() implementation.
        static float value = 0.5f;
        ui::Text("Value = %.3f (<-- right-click here)", value);
        if (ui::BeginPopupContextItem("item context menu"))
        {
            if (ui::Selectable("Set to zero")) value = 0.0f;
            if (ui::Selectable("Set to PI")) value = 3.1415f;
            ui::PushItemWidth(-1);
            ui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
            ui::PopItemWidth();
            ui::EndPopup();
        }

        // We can also use OpenPopupOnItemClick() which is the same as BeginPopupContextItem() but without the Begin call.
        // So here we will make it that clicking on the text field with the right mouse button (1) will toggle the visibility of the popup above.
        ui::Text("(You can also right-click me to the same popup as above.)");
        ui::OpenPopupOnItemClick("item context menu", 1);

        // When used after an item that has an ID (here the Button), we can skip providing an ID to BeginPopupContextItem().
        // BeginPopupContextItem() will use the last item ID as the popup ID.
        // In addition here, we want to include your editable label inside the button label. We use the ### operator to override the ID (read FAQ about ID for details)
        static char name[32] = "Label1";
        char buf[64]; sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
        ui::Button(buf);
        if (ui::BeginPopupContextItem())
        {
            ui::Text("Edit name:");
            ui::InputText("##edit", name, IM_ARRAYSIZE(name));
            if (ui::Button("Close"))
                ui::CloseCurrentPopup();
            ui::EndPopup();
        }
        ui::SameLine(); ui::Text("(<-- right-click here)");

        ui::TreePop();
    }

    if (ui::TreeNode("Modals"))
    {
        ui::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside the window.");

        if (ui::Button("Delete.."))
            ui::OpenPopup("Delete?");

        if (ui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
            ui::Separator();

            //static int dummy_i = 0;
            //ImGui::Combo("Combo", &dummy_i, "Delete\0Delete harder\0");

            static bool dont_ask_me_next_time = false;
            ui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            ui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
            ui::PopStyleVar();

            if (ui::Button("OK", ImVec2(120, 0))) { ui::CloseCurrentPopup(); }
            ui::SetItemDefaultFocus();
            ui::SameLine();
            if (ui::Button("Cancel", ImVec2(120, 0))) { ui::CloseCurrentPopup(); }
            ui::EndPopup();
        }

        if (ui::Button("Stacked modals.."))
            ui::OpenPopup("Stacked 1");
        if (ui::BeginPopupModal("Stacked 1", NULL, ImGuiWindowFlags_MenuBar))
        {
            if (ui::BeginMenuBar())
            {
                if (ui::BeginMenu("File"))
                {
                    if (ui::MenuItem("Dummy menu item")) {}
                    ui::EndMenu();
                }
                ui::EndMenuBar();
            }
            ui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");

            // Testing behavior of widgets stacking their own regular popups over the modal.
            static int item = 1;
            static float color[4] = { 0.4f,0.7f,0.0f,0.5f };
            ui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
            ui::ColorEdit4("color", color);

            if (ui::Button("Add another modal.."))
                ui::OpenPopup("Stacked 2");

            // Also demonstrate passing a bool* to BeginPopupModal(), this will create a regular close button which will close the popup.
            // Note that the visibility state of popups is owned by imgui, so the input value of the bool actually doesn't matter here.
            bool dummy_open = true;
            if (ui::BeginPopupModal("Stacked 2", &dummy_open))
            {
                ui::Text("Hello from Stacked The Second!");
                if (ui::Button("Close"))
                    ui::CloseCurrentPopup();
                ui::EndPopup();
            }

            if (ui::Button("Close"))
                ui::CloseCurrentPopup();
            ui::EndPopup();
        }

        ui::TreePop();
    }

    if (ui::TreeNode("Menus inside a regular window"))
    {
        ui::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
        ui::Separator();
        // NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
        // To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
        // would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
        ui::PushID("foo");
        ui::MenuItem("Menu item", "CTRL+M");
        if (ui::BeginMenu("Menu inside a regular window"))
        {
            ShowExampleMenuFile();
            ui::EndMenu();
        }
        ui::PopID();
        ui::Separator();
        ui::TreePop();
    }
}

static void ShowDemoWindowColumns()
{
    if (!ui::CollapsingHeader("Columns"))
        return;

    ui::PushID("Columns");

    // Basic columns
    if (ui::TreeNode("Basic"))
    {
        ui::Text("Without border:");
        ui::Columns(3, "mycolumns3", false);  // 3-ways, no border
        ui::Separator();
        for (int n = 0; n < 14; n++)
        {
            char label[32];
            sprintf(label, "Item %d", n);
            if (ui::Selectable(label)) {}
            //if (ImGui::Button(label, ImVec2(-1,0))) {}
            ui::NextColumn();
        }
        ui::Columns(1);
        ui::Separator();

        ui::Text("With border:");
        ui::Columns(4, "mycolumns"); // 4-ways, with border
        ui::Separator();
        ui::Text("ID"); ui::NextColumn();
        ui::Text("Name"); ui::NextColumn();
        ui::Text("Path"); ui::NextColumn();
        ui::Text("Hovered"); ui::NextColumn();
        ui::Separator();
        const char* names[3] = { "One", "Two", "Three" };
        const char* paths[3] = { "/path/one", "/path/two", "/path/three" };
        static int selected = -1;
        for (int i = 0; i < 3; i++)
        {
            char label[32];
            sprintf(label, "%04d", i);
            if (ui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                selected = i;
            bool hovered = ui::IsItemHovered();
            ui::NextColumn();
            ui::Text(names[i]); ui::NextColumn();
            ui::Text(paths[i]); ui::NextColumn();
            ui::Text("%d", hovered); ui::NextColumn();
        }
        ui::Columns(1);
        ui::Separator();
        ui::TreePop();
    }

    // Create multiple items in a same cell before switching to next column
    if (ui::TreeNode("Mixed items"))
    {
        ui::Columns(3, "mixed");
        ui::Separator();

        ui::Text("Hello");
        ui::Button("Banana");
        ui::NextColumn();

        ui::Text("ImGui");
        ui::Button("Apple");
        static float foo = 1.0f;
        ui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
        ui::Text("An extra line here.");
        ui::NextColumn();

        ui::Text("Sailor");
        ui::Button("Corniflower");
        static float bar = 1.0f;
        ui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
        ui::NextColumn();

        if (ui::CollapsingHeader("Category A")) { ui::Text("Blah blah blah"); } ui::NextColumn();
        if (ui::CollapsingHeader("Category B")) { ui::Text("Blah blah blah"); } ui::NextColumn();
        if (ui::CollapsingHeader("Category C")) { ui::Text("Blah blah blah"); } ui::NextColumn();
        ui::Columns(1);
        ui::Separator();
        ui::TreePop();
    }

    // Word wrapping
    if (ui::TreeNode("Word-wrapping"))
    {
        ui::Columns(2, "word-wrapping");
        ui::Separator();
        ui::TextWrapped("The quick brown fox jumps over the lazy dog.");
        ui::TextWrapped("Hello Left");
        ui::NextColumn();
        ui::TextWrapped("The quick brown fox jumps over the lazy dog.");
        ui::TextWrapped("Hello Right");
        ui::Columns(1);
        ui::Separator();
        ui::TreePop();
    }

    if (ui::TreeNode("Borders"))
    {
        // NB: Future columns API should allow automatic horizontal borders.
        static bool h_borders = true;
        static bool v_borders = true;
        ui::Checkbox("horizontal", &h_borders);
        ui::SameLine();
        ui::Checkbox("vertical", &v_borders);
        ui::Columns(4, NULL, v_borders);
        for (int i = 0; i < 4*3; i++)
        {
            if (h_borders && ui::GetColumnIndex() == 0)
                ui::Separator();
            ui::Text("%c%c%c", 'a'+i, 'a'+i, 'a'+i);
            ui::Text("Width %.2f\nOffset %.2f", ui::GetColumnWidth(), ui::GetColumnOffset());
            ui::NextColumn();
        }
        ui::Columns(1);
        if (h_borders)
            ui::Separator();
        ui::TreePop();
    }

    // Scrolling columns
    /*
    if (ImGui::TreeNode("Vertical Scrolling"))
    {
        ImGui::BeginChild("##header", ImVec2(0, ImGui::GetTextLineHeightWithSpacing()+ImGui::GetStyle().ItemSpacing.y));
        ImGui::Columns(3);
        ImGui::Text("ID"); ImGui::NextColumn();
        ImGui::Text("Name"); ImGui::NextColumn();
        ImGui::Text("Path"); ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::EndChild();
        ImGui::BeginChild("##scrollingregion", ImVec2(0, 60));
        ImGui::Columns(3);
        for (int i = 0; i < 10; i++)
        {
            ImGui::Text("%04d", i); ImGui::NextColumn();
            ImGui::Text("Foobar"); ImGui::NextColumn();
            ImGui::Text("/path/foobar/%04d/", i); ImGui::NextColumn();
        }
        ImGui::Columns(1);
        ImGui::EndChild();
        ImGui::TreePop();
    }
    */

    if (ui::TreeNode("Horizontal Scrolling"))
    {
        ui::SetNextWindowContentSize(ImVec2(1500.0f, 0.0f));
        ui::BeginChild("##ScrollingRegion", ImVec2(0, ui::GetFontSize() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);
        ui::Columns(10);
        int ITEMS_COUNT = 2000;
        ImGuiListClipper clipper(ITEMS_COUNT);  // Also demonstrate using the clipper for large list
        while (clipper.Step())
        {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                for (int j = 0; j < 10; j++)
                {
                    ui::Text("Line %d Column %d...", i, j);
                    ui::NextColumn();
                }
        }
        ui::Columns(1);
        ui::EndChild();
        ui::TreePop();
    }

    bool node_open = ui::TreeNode("Tree within single cell");
    ui::SameLine(); ShowHelpMarker("NB: Tree node must be poped before ending the cell. There's no storage of state per-cell.");
    if (node_open)
    {
        ui::Columns(2, "tree items");
        ui::Separator();
        if (ui::TreeNode("Hello")) { ui::BulletText("Sailor"); ui::TreePop(); } ui::NextColumn();
        if (ui::TreeNode("Bonjour")) { ui::BulletText("Marin"); ui::TreePop(); } ui::NextColumn();
        ui::Columns(1);
        ui::Separator();
        ui::TreePop();
    }
    ui::PopID();
}

static void ShowDemoWindowMisc()
{
    if (ui::CollapsingHeader("Filtering"))
    {
        static ImGuiTextFilter filter;
        ui::Text("Filter usage:\n"
                    "  \"\"         display all lines\n"
                    "  \"xxx\"      display lines containing \"xxx\"\n"
                    "  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
                    "  \"-xxx\"     hide lines containing \"xxx\"");
        filter.Draw();
        const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
            if (filter.PassFilter(lines[i]))
                ui::BulletText("%s", lines[i]);
    }

    if (ui::CollapsingHeader("Inputs, Navigation & Focus"))
    {
        ImGuiIO& io = ui::GetIO();

        ui::Text("WantCaptureMouse: %d", io.WantCaptureMouse);
        ui::Text("WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
        ui::Text("WantTextInput: %d", io.WantTextInput);
        ui::Text("WantSetMousePos: %d", io.WantSetMousePos);
        ui::Text("NavActive: %d, NavVisible: %d", io.NavActive, io.NavVisible);

        if (ui::TreeNode("Keyboard, Mouse & Navigation State"))
        {
            if (ui::IsMousePosValid())
                ui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            else
                ui::Text("Mouse pos: <INVALID>");
            ui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f)   { ui::SameLine(); ui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ui::IsMouseClicked(i))          { ui::SameLine(); ui::Text("b%d", i); }
            ui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ui::IsMouseDoubleClicked(i)) { ui::SameLine(); ui::Text("b%d", i); }
            ui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ui::IsMouseReleased(i))         { ui::SameLine(); ui::Text("b%d", i); }
            ui::Text("Mouse wheel: %.1f", io.MouseWheel);

            ui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f)     { ui::SameLine(); ui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]); }
            ui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ui::IsKeyPressed(i))             { ui::SameLine(); ui::Text("%d", i); }
            ui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ui::IsKeyReleased(i))            { ui::SameLine(); ui::Text("%d", i); }
            ui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");

            ui::Text("NavInputs down:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f)                    { ui::SameLine(); ui::Text("[%d] %.2f", i, io.NavInputs[i]); }
            ui::Text("NavInputs pressed:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f)    { ui::SameLine(); ui::Text("[%d]", i); }
            ui::Text("NavInputs duration:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] >= 0.0f)   { ui::SameLine(); ui::Text("[%d] %.2f", i, io.NavInputsDownDuration[i]); }

            ui::Button("Hovering me sets the\nkeyboard capture flag");
            if (ui::IsItemHovered())
                ui::CaptureKeyboardFromApp(true);
            ui::SameLine();
            ui::Button("Holding me clears the\nthe keyboard capture flag");
            if (ui::IsItemActive())
                ui::CaptureKeyboardFromApp(false);

            ui::TreePop();
        }

        if (ui::TreeNode("Tabbing"))
        {
            ui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
            static char buf[32] = "dummy";
            ui::InputText("1", buf, IM_ARRAYSIZE(buf));
            ui::InputText("2", buf, IM_ARRAYSIZE(buf));
            ui::InputText("3", buf, IM_ARRAYSIZE(buf));
            ui::PushAllowKeyboardFocus(false);
            ui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
            //ImGui::SameLine(); ShowHelperMarker("Use ImGui::PushAllowKeyboardFocus(bool)\nto disable tabbing through certain widgets.");
            ui::PopAllowKeyboardFocus();
            ui::InputText("5", buf, IM_ARRAYSIZE(buf));
            ui::TreePop();
        }

        if (ui::TreeNode("Focus from code"))
        {
            bool focus_1 = ui::Button("Focus on 1"); ui::SameLine();
            bool focus_2 = ui::Button("Focus on 2"); ui::SameLine();
            bool focus_3 = ui::Button("Focus on 3");
            int has_focus = 0;
            static char buf[128] = "click on a button to set focus";

            if (focus_1) ui::SetKeyboardFocusHere();
            ui::InputText("1", buf, IM_ARRAYSIZE(buf));
            if (ui::IsItemActive()) has_focus = 1;

            if (focus_2) ui::SetKeyboardFocusHere();
            ui::InputText("2", buf, IM_ARRAYSIZE(buf));
            if (ui::IsItemActive()) has_focus = 2;

            ui::PushAllowKeyboardFocus(false);
            if (focus_3) ui::SetKeyboardFocusHere();
            ui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
            if (ui::IsItemActive()) has_focus = 3;
            ui::PopAllowKeyboardFocus();

            if (has_focus)
                ui::Text("Item with focus: %d", has_focus);
            else
                ui::Text("Item with focus: <none>");

            // Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
            static float f3[3] = { 0.0f, 0.0f, 0.0f };
            int focus_ahead = -1;
            if (ui::Button("Focus on X")) { focus_ahead = 0; } ui::SameLine();
            if (ui::Button("Focus on Y")) { focus_ahead = 1; } ui::SameLine();
            if (ui::Button("Focus on Z")) { focus_ahead = 2; }
            if (focus_ahead != -1) ui::SetKeyboardFocusHere(focus_ahead);
            ui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

            ui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
            ui::TreePop();
        }

        if (ui::TreeNode("Dragging"))
        {
            ui::TextWrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
            for (int button = 0; button < 3; button++)
                ui::Text("IsMouseDragging(%d):\n  w/ default threshold: %d,\n  w/ zero threshold: %d\n  w/ large threshold: %d",
                    button, ui::IsMouseDragging(button), ui::IsMouseDragging(button, 0.0f), ui::IsMouseDragging(button, 20.0f));
            ui::Button("Drag Me");
            if (ui::IsItemActive())
            {
                // Draw a line between the button and the mouse cursor
                ImDrawList* draw_list = ui::GetWindowDrawList();
                draw_list->PushClipRectFullScreen();
                draw_list->AddLine(io.MouseClickedPos[0], io.MousePos, ui::GetColorU32(ImGuiCol_Button), 4.0f);
                draw_list->PopClipRect();

                // Drag operations gets "unlocked" when the mouse has moved past a certain threshold (the default threshold is stored in io.MouseDragThreshold)
                // You can request a lower or higher threshold using the second parameter of IsMouseDragging() and GetMouseDragDelta()
                ImVec2 value_raw = ui::GetMouseDragDelta(0, 0.0f);
                ImVec2 value_with_lock_threshold = ui::GetMouseDragDelta(0);
                ImVec2 mouse_delta = io.MouseDelta;
                ui::SameLine(); ui::Text("Raw (%.1f, %.1f), WithLockThresold (%.1f, %.1f), MouseDelta (%.1f, %.1f)", value_raw.x, value_raw.y, value_with_lock_threshold.x, value_with_lock_threshold.y, mouse_delta.x, mouse_delta.y);
            }
            ui::TreePop();
        }

        if (ui::TreeNode("Mouse cursors"))
        {
            const char* mouse_cursors_names[] = { "Arrow", "TextInput", "Move", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand" };
            IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

            ui::Text("Current mouse cursor = %d: %s", ui::GetMouseCursor(), mouse_cursors_names[ui::GetMouseCursor()]);
            ui::Text("Hover to see mouse cursors:");
            ui::SameLine(); ShowHelpMarker("Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, otherwise your backend needs to handle it.");
            for (int i = 0; i < ImGuiMouseCursor_COUNT; i++)
            {
                char label[32];
                sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
                ui::Bullet(); ui::Selectable(label, false);
                if (ui::IsItemHovered() || ui::IsItemFocused())
                    ui::SetMouseCursor(i);
            }
            ui::TreePop();
        }
    }
}

//-----------------------------------------------------------------------------
// [SECTION] About Window / ShowAboutWindow()
// Access from ImGui Demo -> Help -> About
//-----------------------------------------------------------------------------

void ui::ShowAboutWindow(bool* p_open)
{
    if (!ui::Begin("About Dear ImGui", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ui::End();
        return;
    }
    ui::Text("Dear ImGui %s", ui::GetVersion());
    ui::Separator();
    ui::Text("By Omar Cornut and all dear imgui contributors.");
    ui::Text("Dear ImGui is licensed under the MIT License, see LICENSE for more information.");

    static bool show_config_info = false;
    ui::Checkbox("Config/Build Information", &show_config_info);
    if (show_config_info)
    {
        ImGuiIO& io = ui::GetIO();
        ImGuiStyle& style = ui::GetStyle();

        bool copy_to_clipboard = ui::Button("Copy to clipboard");
        ui::BeginChildFrame(ui::GetID("cfginfos"), ImVec2(0, ui::GetTextLineHeightWithSpacing() * 18), ImGuiWindowFlags_NoMove);
        if (copy_to_clipboard)
            ui::LogToClipboard();

        ui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
        ui::Separator();
        ui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));
        ui::Text("define: __cplusplus=%d", (int)__cplusplus);
#ifdef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_OBSOLETE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_WIN32_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_MATH_FUNCTIONS
        ImGui::Text("define: IMGUI_DISABLE_MATH_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_ALLOCATORS
        ImGui::Text("define: IMGUI_DISABLE_DEFAULT_ALLOCATORS");
#endif
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
        ImGui::Text("define: IMGUI_USE_BGRA_PACKED_COLOR");
#endif
#ifdef _WIN32
        ui::Text("define: _WIN32");
#endif
#ifdef _WIN64
        ImGui::Text("define: _WIN64");
#endif
#ifdef __linux__
        ImGui::Text("define: __linux__");
#endif
#ifdef __APPLE__
        ImGui::Text("define: __APPLE__");
#endif
#ifdef _MSC_VER
        ui::Text("define: _MSC_VER=%d", _MSC_VER);
#endif
#ifdef __MINGW32__
        ImGui::Text("define: __MINGW32__");
#endif
#ifdef __MINGW64__
        ImGui::Text("define: __MINGW64__");
#endif
#ifdef __GNUC__
        ImGui::Text("define: __GNUC__=%d", (int)__GNUC__);
#endif
#ifdef __clang_version__
        ui::Text("define: __clang_version__=%s", __clang_version__);
#endif
        ui::Separator();
        ui::Text("io.BackendPlatformName: %s", io.BackendPlatformName ? io.BackendPlatformName : "NULL");
        ui::Text("io.BackendRendererName: %s", io.BackendRendererName ? io.BackendRendererName : "NULL");
        ui::Text("io.ConfigFlags: 0x%08X", io.ConfigFlags);
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard)        ui::Text(" NavEnableKeyboard");
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad)         ui::Text(" NavEnableGamepad");
        if (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos)     ui::Text(" NavEnableSetMousePos");
        if (io.ConfigFlags & ImGuiConfigFlags_NavNoCaptureKeyboard)     ui::Text(" NavNoCaptureKeyboard");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)                  ui::Text(" NoMouse");
        if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)      ui::Text(" NoMouseCursorChange");
        if (io.MouseDrawCursor)                                         ui::Text("io.MouseDrawCursor");
        if (io.ConfigMacOSXBehaviors)                                   ui::Text("io.ConfigMacOSXBehaviors");
        if (io.ConfigInputTextCursorBlink)                              ui::Text("io.ConfigInputTextCursorBlink");
        if (io.ConfigWindowsResizeFromEdges)                            ui::Text("io.ConfigWindowsResizeFromEdges");
        if (io.ConfigWindowsMoveFromTitleBarOnly)                       ui::Text("io.ConfigWindowsMoveFromTitleBarOnly");
        ui::Text("io.BackendFlags: 0x%08X", io.BackendFlags);
        if (io.BackendFlags & ImGuiBackendFlags_HasGamepad)             ui::Text(" HasGamepad");
        if (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors)        ui::Text(" HasMouseCursors");
        if (io.BackendFlags & ImGuiBackendFlags_HasSetMousePos)         ui::Text(" HasSetMousePos");
        ui::Separator();
        ui::Text("io.Fonts: %d fonts, Flags: 0x%08X, TexSize: %d,%d", io.Fonts->Fonts.Size, io.Fonts->Flags, io.Fonts->TexWidth, io.Fonts->TexHeight);
        ui::Text("io.DisplaySize: %.2f,%.2f", io.DisplaySize.x, io.DisplaySize.y);
        ui::Text("io.DisplayFramebufferScale: %.2f,%.2f", io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        ui::Separator();
        ui::Text("style.WindowPadding: %.2f,%.2f", style.WindowPadding.x, style.WindowPadding.y);
        ui::Text("style.WindowBorderSize: %.2f", style.WindowBorderSize);
        ui::Text("style.FramePadding: %.2f,%.2f", style.FramePadding.x, style.FramePadding.y);
        ui::Text("style.FrameRounding: %.2f", style.FrameRounding);
        ui::Text("style.FrameBorderSize: %.2f", style.FrameBorderSize);
        ui::Text("style.ItemSpacing: %.2f,%.2f", style.ItemSpacing.x, style.ItemSpacing.y);
        ui::Text("style.ItemInnerSpacing: %.2f,%.2f", style.ItemInnerSpacing.x, style.ItemInnerSpacing.y);

        if (copy_to_clipboard)
            ui::LogFinish();
        ui::EndChildFrame();
    }
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Style Editor / ShowStyleEditor()
//-----------------------------------------------------------------------------

// Demo helper function to select among default colors. See ShowStyleEditor() for more advanced options.
// Here we use the simplified Combo() api that packs items into a single literal string. Useful for quick combo boxes where the choices are known locally.
bool ui::ShowStyleSelector(const char* label)
{
    static int style_idx = -1;
    if (ui::Combo(label, &style_idx, "Classic\0Dark\0Light\0"))
    {
        switch (style_idx)
        {
        case 0: ui::StyleColorsClassic(); break;
        case 1: ui::StyleColorsDark(); break;
        case 2: ui::StyleColorsLight(); break;
        }
        return true;
    }
    return false;
}

// Demo helper function to select among loaded fonts.
// Here we use the regular BeginCombo()/EndCombo() api which is more the more flexible one.
void ui::ShowFontSelector(const char* label)
{
    ImGuiIO& io = ui::GetIO();
    ImFont* font_current = ui::GetFont();
    if (ui::BeginCombo(label, font_current->GetDebugName()))
    {
        for (int n = 0; n < io.Fonts->Fonts.Size; n++)
        {
            ImFont* font = io.Fonts->Fonts[n];
            ui::PushID((void*)font);
            if (ui::Selectable(font->GetDebugName(), font == font_current))
                io.FontDefault = font;
            ui::PopID();
        }
        ui::EndCombo();
    }
    ui::SameLine();
    ShowHelpMarker(
        "- Load additional fonts with io.Fonts->AddFontFromFileTTF().\n"
        "- The font atlas is built when calling io.Fonts->GetTexDataAsXXXX() or io.Fonts->Build().\n"
        "- Read FAQ and documentation in misc/fonts/ for more details.\n"
        "- If you need to add/remove fonts at runtime (e.g. for DPI change), do it before calling NewFrame().");
}

void ui::ShowStyleEditor(ImGuiStyle* ref)
{
    // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to an internally stored reference)
    ImGuiStyle& style = ui::GetStyle();
    static ImGuiStyle ref_saved_style;

    // Default to using internal storage as reference
    static bool init = true;
    if (init && ref == NULL)
        ref_saved_style = style;
    init = false;
    if (ref == NULL)
        ref = &ref_saved_style;

    ui::PushItemWidth(ui::GetWindowWidth() * 0.50f);

    if (ui::ShowStyleSelector("Colors##Selector"))
        ref_saved_style = style;
    ui::ShowFontSelector("Fonts##Selector");

    // Simplified Settings
    if (ui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
        style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
    { bool window_border = (style.WindowBorderSize > 0.0f); if (ui::Checkbox("WindowBorder", &window_border)) style.WindowBorderSize = window_border ? 1.0f : 0.0f; }
    ui::SameLine();
    { bool frame_border = (style.FrameBorderSize > 0.0f); if (ui::Checkbox("FrameBorder", &frame_border)) style.FrameBorderSize = frame_border ? 1.0f : 0.0f; }
    ui::SameLine();
    { bool popup_border = (style.PopupBorderSize > 0.0f); if (ui::Checkbox("PopupBorder", &popup_border)) style.PopupBorderSize = popup_border ? 1.0f : 0.0f; }

    // Save/Revert button
    if (ui::Button("Save Ref"))
        *ref = ref_saved_style = style;
    ui::SameLine();
    if (ui::Button("Revert Ref"))
        style = *ref;
    ui::SameLine();
    ShowHelpMarker("Save/Revert in local non-persistent storage. Default Colors definition are not affected. Use \"Export Colors\" below to save them somewhere.");

    ui::Separator();

    if (ui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (ui::BeginTabItem("Sizes"))
        {
            ui::Text("Main");
            ui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
            ui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
            ui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
            ui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
            ui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
            ui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
            ui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
            ui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
            ui::Text("Borders");
            ui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
            ui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
            ui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
            ui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
            ui::SliderFloat("TabBorderSize", &style.TabBorderSize, 0.0f, 1.0f, "%.0f");
            ui::Text("Rounding");
            ui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 12.0f, "%.0f");
            ui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 12.0f, "%.0f");
            ui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
            ui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 12.0f, "%.0f");
            ui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
            ui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
            ui::SliderFloat("TabRounding", &style.TabRounding, 0.0f, 12.0f, "%.0f");
            ui::Text("Alignment");
            ui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
            ui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f"); ui::SameLine(); ShowHelpMarker("Alignment applies when a button is larger than its text content.");
            ui::SliderFloat2("SelectableTextAlign", (float*)&style.SelectableTextAlign, 0.0f, 1.0f, "%.2f"); ui::SameLine(); ShowHelpMarker("Alignment applies when a selectable is larger than its text content.");
            ui::Text("Safe Area Padding"); ui::SameLine(); ShowHelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
            ui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
            ui::EndTabItem();
        }

        if (ui::BeginTabItem("Colors"))
        {
            static int output_dest = 0;
            static bool output_only_modified = true;
            if (ui::Button("Export Unsaved"))
            {
                if (output_dest == 0)
                    ui::LogToClipboard();
                else
                    ui::LogToTTY();
                ui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;" IM_NEWLINE);
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const ImVec4& col = style.Colors[i];
                    const char* name = ui::GetStyleColorName(i);
                    if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                        ui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE, name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
                }
                ui::LogFinish();
            }
            ui::SameLine(); ui::PushItemWidth(120); ui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0"); ui::PopItemWidth();
            ui::SameLine(); ui::Checkbox("Only Modified Colors", &output_only_modified);

            static ImGuiTextFilter filter;
            filter.Draw("Filter colors", ui::GetFontSize() * 16);

            static ImGuiColorEditFlags alpha_flags = 0;
            ui::RadioButton("Opaque", &alpha_flags, 0); ui::SameLine();
            ui::RadioButton("Alpha", &alpha_flags, ImGuiColorEditFlags_AlphaPreview); ui::SameLine();
            ui::RadioButton("Both", &alpha_flags, ImGuiColorEditFlags_AlphaPreviewHalf); ui::SameLine();
            ShowHelpMarker("In the color list:\nLeft-click on colored square to open color picker,\nRight-click to open edit options menu.");

            ui::BeginChild("##colors", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
            ui::PushItemWidth(-160);
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = ui::GetStyleColorName(i);
                if (!filter.PassFilter(name))
                    continue;
                ui::PushID(i);
                ui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
                if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                {
                    // Tips: in a real user application, you may want to merge and use an icon font into the main font, so instead of "Save"/"Revert" you'd use icons.
                    // Read the FAQ and misc/fonts/README.txt about using icon fonts. It's really easy and super convenient!
                    ui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ui::Button("Save")) ref->Colors[i] = style.Colors[i];
                    ui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ui::Button("Revert")) style.Colors[i] = ref->Colors[i];
                }
                ui::SameLine(0.0f, style.ItemInnerSpacing.x);
                ui::TextUnformatted(name);
                ui::PopID();
            }
            ui::PopItemWidth();
            ui::EndChild();

            ui::EndTabItem();
        }

        if (ui::BeginTabItem("Fonts"))
        {
            ImGuiIO& io = ui::GetIO();
            ImFontAtlas* atlas = io.Fonts;
            ShowHelpMarker("Read FAQ and misc/fonts/README.txt for details on font loading.");
            ui::PushItemWidth(120);
            for (int i = 0; i < atlas->Fonts.Size; i++)
            {
                ImFont* font = atlas->Fonts[i];
                ui::PushID(font);
                bool font_details_opened = ui::TreeNode(font, "Font %d: \"%s\"\n%.2f px, %d glyphs, %d file(s)", i, font->ConfigData ? font->ConfigData[0].Name : "", font->FontSize, font->Glyphs.Size, font->ConfigDataCount);
                ui::SameLine(); if (ui::SmallButton("Set as default")) { io.FontDefault = font; }
                if (font_details_opened)
                {
                    ui::PushFont(font);
                    ui::Text("The quick brown fox jumps over the lazy dog");
                    ui::PopFont();
                    ui::DragFloat("Font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f");   // Scale only this font
                    ui::SameLine(); ShowHelpMarker("Note than the default embedded font is NOT meant to be scaled.\n\nFont are currently rendered into bitmaps at a given size at the time of building the atlas. You may oversample them to get some flexibility with scaling. You can also render at multiple sizes and select which one to use at runtime.\n\n(Glimmer of hope: the atlas system should hopefully be rewritten in the future to make scaling more natural and automatic.)");
                    ui::InputFloat("Font offset", &font->DisplayOffset.y, 1, 1, "%.0f");
                    ui::Text("Ascent: %f, Descent: %f, Height: %f", font->Ascent, font->Descent, font->Ascent - font->Descent);
                    ui::Text("Fallback character: '%c' (%d)", font->FallbackChar, font->FallbackChar);
                    const float surface_sqrt = sqrtf((float)font->MetricsTotalSurface);
                    ui::Text("Texture surface: %d pixels (approx) ~ %dx%d", font->MetricsTotalSurface, (int)surface_sqrt, (int)surface_sqrt);
                    for (int config_i = 0; config_i < font->ConfigDataCount; config_i++)
                        if (const ImFontConfig* cfg = &font->ConfigData[config_i])
                            ui::BulletText("Input %d: \'%s\', Oversample: (%d,%d), PixelSnapH: %d", config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
                    if (ui::TreeNode("Glyphs", "Glyphs (%d)", font->Glyphs.Size))
                    {
                        // Display all glyphs of the fonts in separate pages of 256 characters
                        for (int base = 0; base < 0x10000; base += 256)
                        {
                            int count = 0;
                            for (int n = 0; n < 256; n++)
                                count += font->FindGlyphNoFallback((ImWchar)(base + n)) ? 1 : 0;
                            if (count > 0 && ui::TreeNode((void*)(intptr_t)base, "U+%04X..U+%04X (%d %s)", base, base + 255, count, count > 1 ? "glyphs" : "glyph"))
                            {
                                float cell_size = font->FontSize * 1;
                                float cell_spacing = style.ItemSpacing.y;
                                ImVec2 base_pos = ui::GetCursorScreenPos();
                                ImDrawList* draw_list = ui::GetWindowDrawList();
                                for (int n = 0; n < 256; n++)
                                {
                                    ImVec2 cell_p1(base_pos.x + (n % 16) * (cell_size + cell_spacing), base_pos.y + (n / 16) * (cell_size + cell_spacing));
                                    ImVec2 cell_p2(cell_p1.x + cell_size, cell_p1.y + cell_size);
                                    const ImFontGlyph* glyph = font->FindGlyphNoFallback((ImWchar)(base + n));
                                    draw_list->AddRect(cell_p1, cell_p2, glyph ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 50));
                                    if (glyph)
                                        font->RenderChar(draw_list, cell_size, cell_p1, ui::GetColorU32(ImGuiCol_Text), (ImWchar)(base + n)); // We use ImFont::RenderChar as a shortcut because we don't have UTF-8 conversion functions available to generate a string.
                                    if (glyph && ui::IsMouseHoveringRect(cell_p1, cell_p2))
                                    {
                                        ui::BeginTooltip();
                                        ui::Text("Codepoint: U+%04X", base + n);
                                        ui::Separator();
                                        ui::Text("AdvanceX: %.1f", glyph->AdvanceX);
                                        ui::Text("Pos: (%.2f,%.2f)->(%.2f,%.2f)", glyph->X0, glyph->Y0, glyph->X1, glyph->Y1);
                                        ui::Text("UV: (%.3f,%.3f)->(%.3f,%.3f)", glyph->U0, glyph->V0, glyph->U1, glyph->V1);
                                        ui::EndTooltip();
                                    }
                                }
                                ui::Dummy(ImVec2((cell_size + cell_spacing) * 16, (cell_size + cell_spacing) * 16));
                                ui::TreePop();
                            }
                        }
                        ui::TreePop();
                    }
                    ui::TreePop();
                }
                ui::PopID();
            }
            if (ui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight))
            {
                ui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
                ui::TreePop();
            }

            static float window_scale = 1.0f;
            if (ui::DragFloat("this window scale", &window_scale, 0.005f, 0.3f, 2.0f, "%.2f"))   // scale only this window
                ui::SetWindowFontScale(window_scale);
            ui::DragFloat("global scale", &io.FontGlobalScale, 0.005f, 0.3f, 2.0f, "%.2f");      // scale everything
            ui::PopItemWidth();

            ui::EndTabItem();
        }

        if (ui::BeginTabItem("Rendering"))
        {
            ui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines); ui::SameLine(); ShowHelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");
            ui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
            ui::PushItemWidth(100);
            ui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, "%.2f", 2.0f);
            if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;
            ui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
            ui::PopItemWidth();

            ui::EndTabItem();
        }

        ui::EndTabBar();
    }

    ui::PopItemWidth();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Main Menu Bar / ShowExampleAppMainMenuBar()
//-----------------------------------------------------------------------------

// Demonstrate creating a fullscreen menu bar and populating it.
static void ShowExampleAppMainMenuBar()
{
    if (ui::BeginMainMenuBar())
    {
        if (ui::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            ui::EndMenu();
        }
        if (ui::BeginMenu("Edit"))
        {
            if (ui::MenuItem("Undo", "CTRL+Z")) {}
            if (ui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ui::Separator();
            if (ui::MenuItem("Cut", "CTRL+X")) {}
            if (ui::MenuItem("Copy", "CTRL+C")) {}
            if (ui::MenuItem("Paste", "CTRL+V")) {}
            ui::EndMenu();
        }
        ui::EndMainMenuBar();
    }
}

static void ShowExampleMenuFile()
{
    ui::MenuItem("(dummy menu)", NULL, false, false);
    if (ui::MenuItem("New")) {}
    if (ui::MenuItem("Open", "Ctrl+O")) {}
    if (ui::BeginMenu("Open Recent"))
    {
        ui::MenuItem("fish_hat.c");
        ui::MenuItem("fish_hat.inl");
        ui::MenuItem("fish_hat.h");
        if (ui::BeginMenu("More.."))
        {
            ui::MenuItem("Hello");
            ui::MenuItem("Sailor");
            if (ui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ui::EndMenu();
            }
            ui::EndMenu();
        }
        ui::EndMenu();
    }
    if (ui::MenuItem("Save", "Ctrl+S")) {}
    if (ui::MenuItem("Save As..")) {}
    ui::Separator();
    if (ui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ui::MenuItem("Enabled", "", &enabled);
        ui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ui::Text("Scrolling Text %d", i);
        ui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        static bool b = true;
        ui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ui::InputFloat("Input", &f, 0.1f);
        ui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
        ui::Checkbox("Check", &b);
        ui::EndMenu();
    }
    if (ui::BeginMenu("Colors"))
    {
        float sz = ui::GetTextLineHeight();
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ui::GetStyleColorName((ImGuiCol)i);
            ImVec2 p = ui::GetCursorScreenPos();
            ui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x+sz, p.y+sz), ui::GetColorU32((ImGuiCol)i));
            ui::Dummy(ImVec2(sz, sz));
            ui::SameLine();
            ui::MenuItem(name);
        }
        ui::EndMenu();
    }
    if (ui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ui::MenuItem("Checked", NULL, true)) {}
    if (ui::MenuItem("Quit", "Alt+F4")) {}
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Console / ShowExampleAppConsole()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, here we are using a more C++ like approach of declaring a class to hold the data and the functions.
struct ExampleAppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;

    ExampleAppConsole()
    {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
        Commands.push_back("HELP");
        Commands.push_back("HISTORY");
        Commands.push_back("CLEAR");
        Commands.push_back("CLASSIFY");  // "classify" is only here to provide an example of "C"+[tab] completing to "CL" and displaying matches.
        AutoScroll = true;
        ScrollToBottom = true;
        AddLog("Welcome to Dear ImGui!");
    }
    ~ExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Portable helpers
    static int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
    static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
    static char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
    static void  Strtrim(char* str)                                  { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
        ScrollToBottom = true;
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
        if (AutoScroll)
            ScrollToBottom = true;
    }

    void    Draw(const char* title, bool* p_open)
    {
        ui::SetNextWindowSize(ImVec2(520,600), ImGuiCond_FirstUseEver);
        if (!ui::Begin(title, p_open))
        {
            ui::End();
            return;
        }

        // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar. So e.g. IsItemHovered() will return true when hovering the title bar.
        // Here we create a context menu only available from the title bar.
        if (ui::BeginPopupContextItem())
        {
            if (ui::MenuItem("Close Console"))
                *p_open = false;
            ui::EndPopup();
        }

        ui::TextWrapped("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
        ui::TextWrapped("Enter 'HELP' for help, press TAB to use text completion.");

        // TODO: display items starting from the bottom

        if (ui::SmallButton("Add Dummy Text"))  { AddLog("%d some text", Items.Size); AddLog("some more text"); AddLog("display very important message here!"); } ui::SameLine();
        if (ui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ui::SameLine();
        if (ui::SmallButton("Clear")) { ClearLog(); } ui::SameLine();
        bool copy_to_clipboard = ui::SmallButton("Copy"); ui::SameLine();
        if (ui::SmallButton("Scroll to bottom")) ScrollToBottom = true;
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }

        ui::Separator();

        // Options menu
        if (ui::BeginPopup("Options"))
        {
            if (ui::Checkbox("Auto-scroll", &AutoScroll))
                if (AutoScroll)
                    ScrollToBottom = true;
            ui::EndPopup();
        }

        // Options, Filter
        if (ui::Button("Options"))
            ui::OpenPopup("Options");
        ui::SameLine();
        Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
        ui::Separator();

        const float footer_height_to_reserve = ui::GetStyle().ItemSpacing.y + ui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
        ui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
        if (ui::BeginPopupContextWindow())
        {
            if (ui::Selectable("Clear")) ClearLog();
            ui::EndPopup();
        }

        // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
        // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
        // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
        // To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
        //     ImGuiListClipper clipper(Items.Size);
        //     while (clipper.Step())
        //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        // However, note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
        // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
        // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
        // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
        ui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
        if (copy_to_clipboard)
            ui::LogToClipboard();
        for (int i = 0; i < Items.Size; i++)
        {
            const char* item = Items[i];
            if (!Filter.PassFilter(item))
                continue;

            // Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
            bool pop_color = false;
            if (strstr(item, "[error]"))            { ui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
            else if (strncmp(item, "# ", 2) == 0)   { ui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true; }
            ui::TextUnformatted(item);
            if (pop_color)
                ui::PopStyleColor();
        }
        if (copy_to_clipboard)
            ui::LogFinish();
        if (ScrollToBottom)
            ui::SetScrollHereY(1.0f);
        ScrollToBottom = false;
        ui::PopStyleVar();
        ui::EndChild();
        ui::Separator();

        // Command-line
        bool reclaim_focus = false;
        if (ui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
        {
            char* s = InputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ui::SetItemDefaultFocus();
        if (reclaim_focus)
            ui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ui::End();
    }

    void    ExecCommand(const char* command_line)
    {
        AddLog("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size-1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "CLEAR") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < Commands.Size; i++)
                AddLog("- %s", Commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY") == 0)
        {
            int first = History.Size - 10;
            for (int i = first > 0 ? first : 0; i < History.Size; i++)
                AddLog("%3d: %s\n", i, History[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On commad input, we scroll to bottom even if AutoScroll==false
        ScrollToBottom = true;
    }

    static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
    {
        ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
        return console->TextEditCallback(data);
    }

    int     TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
            {
                // Example of TEXT COMPLETION

                // Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf)
                {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Build a list of candidates
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++)
                    if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
                        candidates.push_back(Commands[i]);

                if (candidates.Size == 0)
                {
                    // No match
                    AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
                }
                else if (candidates.Size == 1)
                {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                    data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, " ");
                }
                else
                {
                    // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
                    int match_len = (int)(word_end - word_start);
                    for (;;)
                    {
                        int c = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0)
                                c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches)
                            break;
                        match_len++;
                    }

                    if (match_len > 0)
                    {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    AddLog("Possible matches:\n");
                    for (int i = 0; i < candidates.Size; i++)
                        AddLog("- %s\n", candidates[i]);
                }

                break;
            }
        case ImGuiInputTextFlags_CallbackHistory:
            {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos)
                {
                    const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
                    data->DeleteChars(0, data->BufTextLen);
                    data->InsertChars(0, history_str);
                }
            }
        }
        return 0;
    }
};

static void ShowExampleAppConsole(bool* p_open)
{
    static ExampleAppConsole console;
    console.Draw("Example: Console", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
    bool                AutoScroll;
    bool                ScrollToBottom;

    ExampleAppLog()
    {
        AutoScroll = true;
        ScrollToBottom = false;
        Clear();
    }

    void    Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
        if (AutoScroll)
            ScrollToBottom = true;
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        if (!ui::Begin(title, p_open))
        {
            ui::End();
            return;
        }

        // Options menu
        if (ui::BeginPopup("Options"))
        {
            if (ui::Checkbox("Auto-scroll", &AutoScroll))
                if (AutoScroll)
                    ScrollToBottom = true;
            ui::EndPopup();
        }

        // Main window
        if (ui::Button("Options"))
            ui::OpenPopup("Options");
        ui::SameLine();
        bool clear = ui::Button("Clear");
        ui::SameLine();
        bool copy = ui::Button("Copy");
        ui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ui::Separator();
        ui::BeginChild("scrolling", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();
        if (copy)
            ui::LogToClipboard();

        ui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            // In this example we don't use the clipper when Filter is enabled.
            // This is because we don't have a random access on the result on our filter.
            // A real application processing logs with ten of thousands of entries may want to store the result of search/filter.
            // especially if the filtering function is not trivial (e.g. reg-exp).
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            // The simplest and easy way to display the entire buffer:
            //   ImGui::TextUnformatted(buf_begin, buf_end);
            // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward to skip non-visible lines.
            // Here we instead demonstrate using the clipper to only process lines that are within the visible area.
            // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them on your side is recommended.
            // Using ImGuiListClipper requires A) random access into your data, and B) items all being the  same height,
            // both of which we can handle since we an array pointing to the beginning of each line of text.
            // When using the filter (in the block of code above) we don't have random access into the data to display anymore, which is why we don't use the clipper.
            // Storing or skimming through the search result would make it possible (and would be recommended if you want to search through tens of thousands of entries)
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ui::PopStyleVar();

        if (ScrollToBottom)
            ui::SetScrollHereY(1.0f);
        ScrollToBottom = false;
        ui::EndChild();
        ui::End();
    }
};

// Demonstrate creating a simple log window with basic filtering.
static void ShowExampleAppLog(bool* p_open)
{
    static ExampleAppLog log;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of the fact that multiple calls to Begin()/End() are appending to the same window.
    // Most of the contents of the window will be added by the log.Draw() call.
    ui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ui::Begin("Example: Log", p_open);
    if (ui::SmallButton("[Debug] Add 5 entries"))
    {
        static int counter = 0;
        for (int n = 0; n < 5; n++)
        {
            const char* categories[3] = { "info", "warn", "error" };
            const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
            log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
                ui::GetFrameCount(), categories[counter % IM_ARRAYSIZE(categories)], ui::GetTime(), words[counter % IM_ARRAYSIZE(words)]);
            counter++;
        }
    }
    ui::End();

    log.Draw("Example: Log", p_open);
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple Layout / ShowExampleAppLayout()
//-----------------------------------------------------------------------------

// Demonstrate create a window with multiple child windows.
static void ShowExampleAppLayout(bool* p_open)
{
    ui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    if (ui::Begin("Example: Simple layout", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ui::BeginMenuBar())
        {
            if (ui::BeginMenu("File"))
            {
                if (ui::MenuItem("Close")) *p_open = false;
                ui::EndMenu();
            }
            ui::EndMenuBar();
        }

        // left
        static int selected = 0;
        ui::BeginChild("left pane", ImVec2(150, 0), true);
        for (int i = 0; i < 100; i++)
        {
            char label[128];
            sprintf(label, "MyObject %d", i);
            if (ui::Selectable(label, selected == i))
                selected = i;
        }
        ui::EndChild();
        ui::SameLine();

        // right
        ui::BeginGroup();
            ui::BeginChild("item view", ImVec2(0, -ui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                ui::Text("MyObject: %d", selected);
                ui::Separator();
                if (ui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                {
                    if (ui::BeginTabItem("Description"))
                    {
                        ui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                        ui::EndTabItem();
                    }
                    if (ui::BeginTabItem("Details"))
                    {
                        ui::Text("ID: 0123456789");
                        ui::EndTabItem();
                    }
                    ui::EndTabBar();
                }
            ui::EndChild();
            if (ui::Button("Revert")) {}
            ui::SameLine();
            if (ui::Button("Save")) {}
        ui::EndGroup();
    }
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Property Editor / ShowExampleAppPropertyEditor()
//-----------------------------------------------------------------------------

// Demonstrate create a simple property editor.
static void ShowExampleAppPropertyEditor(bool* p_open)
{
    ui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ui::Begin("Example: Property editor", p_open))
    {
        ui::End();
        return;
    }

    ShowHelpMarker("This example shows how you may implement a property editor using two columns.\nAll objects/fields data are dummies here.\nRemember that in many simple cases, you can use ImGui::SameLine(xxx) to position\nyour cursor horizontally instead of using the Columns() API.");

    ui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
    ui::Columns(2);
    ui::Separator();

    struct funcs
    {
        static void ShowDummyObject(const char* prefix, int uid)
        {
            ui::PushID(uid);                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
            ui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
            bool node_open = ui::TreeNode("Object", "%s_%u", prefix, uid);
            ui::NextColumn();
            ui::AlignTextToFramePadding();
            ui::Text("my sailor is rich");
            ui::NextColumn();
            if (node_open)
            {
                static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
                for (int i = 0; i < 8; i++)
                {
                    ui::PushID(i); // Use field index as identifier.
                    if (i < 2)
                    {
                        ShowDummyObject("Child", 424242);
                    }
                    else
                    {
                        // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                        ui::AlignTextToFramePadding();
                        ui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
                        ui::NextColumn();
                        ui::PushItemWidth(-1);
                        if (i >= 5)
                            ui::InputFloat("##value", &dummy_members[i], 1.0f);
                        else
                            ui::DragFloat("##value", &dummy_members[i], 0.01f);
                        ui::PopItemWidth();
                        ui::NextColumn();
                    }
                    ui::PopID();
                }
                ui::TreePop();
            }
            ui::PopID();
        }
    };

    // Iterate dummy objects with dummy members (all the same data)
    for (int obj_i = 0; obj_i < 3; obj_i++)
        funcs::ShowDummyObject("Object", obj_i);

    ui::Columns(1);
    ui::Separator();
    ui::PopStyleVar();
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Long Text / ShowExampleAppLongText()
//-----------------------------------------------------------------------------

// Demonstrate/test rendering huge amount of text, and the incidence of clipping.
static void ShowExampleAppLongText(bool* p_open)
{
    ui::SetNextWindowSize(ImVec2(520,600), ImGuiCond_FirstUseEver);
    if (!ui::Begin("Example: Long text display", p_open))
    {
        ui::End();
        return;
    }

    static int test_type = 0;
    static ImGuiTextBuffer log;
    static int lines = 0;
    ui::Text("Printing unusually long amount of text.");
    ui::Combo("Test type", &test_type, "Single call to TextUnformatted()\0Multiple calls to Text(), clipped manually\0Multiple calls to Text(), not clipped (slow)\0");
    ui::Text("Buffer contents: %d lines, %d bytes", lines, log.size());
    if (ui::Button("Clear")) { log.clear(); lines = 0; }
    ui::SameLine();
    if (ui::Button("Add 1000 lines"))
    {
        for (int i = 0; i < 1000; i++)
            log.appendf("%i The quick brown fox jumps over the lazy dog\n", lines+i);
        lines += 1000;
    }
    ui::BeginChild("Log");
    switch (test_type)
    {
    case 0:
        // Single call to TextUnformatted() with a big buffer
        ui::TextUnformatted(log.begin(), log.end());
        break;
    case 1:
        {
            // Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
            ui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
            ImGuiListClipper clipper(lines);
            while (clipper.Step())
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    ui::Text("%i The quick brown fox jumps over the lazy dog", i);
            ui::PopStyleVar();
            break;
        }
    case 2:
        // Multiple calls to Text(), not clipped (slow)
        ui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
        for (int i = 0; i < lines; i++)
            ui::Text("%i The quick brown fox jumps over the lazy dog", i);
        ui::PopStyleVar();
        break;
    }
    ui::EndChild();
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Auto Resize / ShowExampleAppAutoResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window which gets auto-resized according to its content.
static void ShowExampleAppAutoResize(bool* p_open)
{
    if (!ui::Begin("Example: Auto-resizing window", p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ui::End();
        return;
    }

    static int lines = 10;
    ui::Text("Window will resize every-frame to the size of its content.\nNote that you probably don't want to query the window size to\noutput your content because that would create a feedback loop.");
    ui::SliderInt("Number of lines", &lines, 1, 20);
    for (int i = 0; i < lines; i++)
        ui::Text("%*sThis is line %d", i * 4, "", i); // Pad with space to extend size horizontally
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Constrained Resize / ShowExampleAppConstrainedResize()
//-----------------------------------------------------------------------------

// Demonstrate creating a window with custom resize constraints.
static void ShowExampleAppConstrainedResize(bool* p_open)
{
    struct CustomConstraints // Helper functions to demonstrate programmatic constraints
    {
        static void Square(ImGuiSizeCallbackData* data) { data->DesiredSize = ImVec2(IM_MAX(data->DesiredSize.x, data->DesiredSize.y), IM_MAX(data->DesiredSize.x, data->DesiredSize.y)); }
        static void Step(ImGuiSizeCallbackData* data)   { float step = (float)(int)(intptr_t)data->UserData; data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step); }
    };

    static bool auto_resize = false;
    static int type = 0;
    static int display_lines = 10;
    if (type == 0) ui::SetNextWindowSizeConstraints(ImVec2(-1, 0),    ImVec2(-1, FLT_MAX));      // Vertical only
    if (type == 1) ui::SetNextWindowSizeConstraints(ImVec2(0, -1),    ImVec2(FLT_MAX, -1));      // Horizontal only
    if (type == 2) ui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
    if (type == 3) ui::SetNextWindowSizeConstraints(ImVec2(400, -1),  ImVec2(500, -1));          // Width 400-500
    if (type == 4) ui::SetNextWindowSizeConstraints(ImVec2(-1, 400),  ImVec2(-1, 500));          // Height 400-500
    if (type == 5) ui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);                     // Always Square
    if (type == 6) ui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)(intptr_t)100); // Fixed Step

    ImGuiWindowFlags flags = auto_resize ? ImGuiWindowFlags_AlwaysAutoResize : 0;
    if (ui::Begin("Example: Constrained Resize", p_open, flags))
    {
        const char* desc[] =
        {
            "Resize vertical only",
            "Resize horizontal only",
            "Width > 100, Height > 100",
            "Width 400-500",
            "Height 400-500",
            "Custom: Always Square",
            "Custom: Fixed Steps (100)",
        };
        if (ui::Button("200x200")) { ui::SetWindowSize(ImVec2(200, 200)); } ui::SameLine();
        if (ui::Button("500x500")) { ui::SetWindowSize(ImVec2(500, 500)); } ui::SameLine();
        if (ui::Button("800x200")) { ui::SetWindowSize(ImVec2(800, 200)); }
        ui::PushItemWidth(200);
        ui::Combo("Constraint", &type, desc, IM_ARRAYSIZE(desc));
        ui::DragInt("Lines", &display_lines, 0.2f, 1, 100);
        ui::PopItemWidth();
        ui::Checkbox("Auto-resize", &auto_resize);
        for (int i = 0; i < display_lines; i++)
            ui::Text("%*sHello, sailor! Making this line long enough for the example.", i * 4, "");
    }
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Simple Overlay / ShowExampleAppSimpleOverlay()
//-----------------------------------------------------------------------------

// Demonstrate creating a simple static window with no decoration + a context-menu to choose which corner of the screen to use.
static void ShowExampleAppSimpleOverlay(bool* p_open)
{
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ui::GetIO();
    if (corner != -1)
    {
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ui::SetNextWindowBgAlpha(0.3f); // Transparent background
    if (ui::Begin("Example: Simple overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
        ui::Separator();
        if (ui::IsMousePosValid())
            ui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ui::Text("Mouse Position: <invalid>");
        if (ui::BeginPopupContextWindow())
        {
            if (ui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ui::MenuItem("Close")) *p_open = false;
            ui::EndPopup();
        }
    }
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Manipulating Window Titles / ShowExampleAppWindowTitles()
//-----------------------------------------------------------------------------

// Demonstrate using "##" and "###" in identifiers to manipulate ID generation.
// This apply to all regular items as well. Read FAQ section "How can I have multiple widgets with the same label? Can I have widget without a label? (Yes). A primer on the purpose of labels/IDs." for details.
static void ShowExampleAppWindowTitles(bool*)
{
    // By default, Windows are uniquely identified by their title.
    // You can use the "##" and "###" markers to manipulate the display/ID.

    // Using "##" to display same title but have unique identifier.
    ui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    ui::Begin("Same title as another window##1");
    ui::Text("This is window 1.\nMy title is the same as window 2, but my identifier is unique.");
    ui::End();

    ui::SetNextWindowPos(ImVec2(100, 200), ImGuiCond_FirstUseEver);
    ui::Begin("Same title as another window##2");
    ui::Text("This is window 2.\nMy title is the same as window 1, but my identifier is unique.");
    ui::End();

    // Using "###" to display a changing title but keep a static identifier "AnimatedTitle"
    char buf[128];
    sprintf(buf, "Animated title %c %d###AnimatedTitle", "|/-\\"[(int)(ui::GetTime() / 0.25f) & 3], ui::GetFrameCount());
    ui::SetNextWindowPos(ImVec2(100, 300), ImGuiCond_FirstUseEver);
    ui::Begin(buf);
    ui::Text("This window has a changing title.");
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Custom Rendering using ImDrawList API / ShowExampleAppCustomRendering()
//-----------------------------------------------------------------------------

// Demonstrate using the low-level ImDrawList to draw custom shapes.
static void ShowExampleAppCustomRendering(bool* p_open)
{
    ui::SetNextWindowSize(ImVec2(350, 560), ImGuiCond_FirstUseEver);
    if (!ui::Begin("Example: Custom rendering", p_open))
    {
        ui::End();
        return;
    }

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc.
    // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4.
    // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types)
    // In this example we are not using the maths operators!
    ImDrawList* draw_list = ui::GetWindowDrawList();

    // Primitives
    ui::Text("Primitives");
    static float sz = 36.0f;
    static float thickness = 4.0f;
    static ImVec4 col = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
    ui::DragFloat("Size", &sz, 0.2f, 2.0f, 72.0f, "%.0f");
    ui::DragFloat("Thickness", &thickness, 0.05f, 1.0f, 8.0f, "%.02f");
    ui::ColorEdit4("Color", &col.x);
    {
        const ImVec2 p = ui::GetCursorScreenPos();
        const ImU32 col32 = ImColor(col);
        float x = p.x + 4.0f, y = p.y + 4.0f, spacing = 8.0f;
        for (int n = 0; n < 2; n++)
        {
            // First line uses a thickness of 1.0, second line uses the configurable thickness
            float th = (n == 0) ? 1.0f : thickness;
            draw_list->AddCircle(ImVec2(x+sz*0.5f, y+sz*0.5f), sz*0.5f, col32, 6, th); x += sz+spacing;     // Hexagon
            draw_list->AddCircle(ImVec2(x+sz*0.5f, y+sz*0.5f), sz*0.5f, col32, 20, th); x += sz+spacing;    // Circle
            draw_list->AddRect(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 0.0f,  ImDrawCornerFlags_All, th); x += sz+spacing;
            draw_list->AddRect(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 10.0f, ImDrawCornerFlags_All, th); x += sz+spacing;
            draw_list->AddRect(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 10.0f, ImDrawCornerFlags_TopLeft|ImDrawCornerFlags_BotRight, th); x += sz+spacing;
            draw_list->AddTriangle(ImVec2(x+sz*0.5f, y), ImVec2(x+sz,y+sz-0.5f), ImVec2(x,y+sz-0.5f), col32, th); x += sz+spacing;
            draw_list->AddLine(ImVec2(x, y), ImVec2(x+sz, y   ), col32, th); x += sz+spacing;               // Horizontal line (note: drawing a filled rectangle will be faster!)
            draw_list->AddLine(ImVec2(x, y), ImVec2(x,    y+sz), col32, th); x += spacing;                  // Vertical line (note: drawing a filled rectangle will be faster!)
            draw_list->AddLine(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, th); x += sz+spacing;               // Diagonal line
            draw_list->AddBezierCurve(ImVec2(x, y), ImVec2(x+sz*1.3f,y+sz*0.3f), ImVec2(x+sz-sz*1.3f,y+sz-sz*0.3f), ImVec2(x+sz, y+sz), col32, th);
            x = p.x + 4;
            y += sz+spacing;
        }
        draw_list->AddCircleFilled(ImVec2(x+sz*0.5f, y+sz*0.5f), sz*0.5f, col32, 6); x += sz+spacing;       // Hexagon
        draw_list->AddCircleFilled(ImVec2(x+sz*0.5f, y+sz*0.5f), sz*0.5f, col32, 32); x += sz+spacing;      // Circle
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+sz, y+sz), col32); x += sz+spacing;
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 10.0f); x += sz+spacing;
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 10.0f, ImDrawCornerFlags_TopLeft|ImDrawCornerFlags_BotRight); x += sz+spacing;
        draw_list->AddTriangleFilled(ImVec2(x+sz*0.5f, y), ImVec2(x+sz,y+sz-0.5f), ImVec2(x,y+sz-0.5f), col32); x += sz+spacing;
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+sz, y+thickness), col32); x += sz+spacing;          // Horizontal line (faster than AddLine, but only handle integer thickness)
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+thickness, y+sz), col32); x += spacing+spacing;     // Vertical line (faster than AddLine, but only handle integer thickness)
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+1, y+1), col32);          x += sz;                  // Pixel (faster than AddLine)
        draw_list->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x+sz, y+sz), IM_COL32(0,0,0,255), IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255));
        ui::Dummy(ImVec2((sz+spacing)*8, (sz+spacing)*3));
    }
    ui::Separator();
    {
        static ImVector<ImVec2> points;
        static bool adding_line = false;
        ui::Text("Canvas example");
        if (ui::Button("Clear")) points.clear();
        if (points.Size >= 2) { ui::SameLine(); if (ui::Button("Undo")) { points.pop_back(); points.pop_back(); } }
        ui::Text("Left-click and drag to add lines,\nRight-click to undo");

        // Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered()
        // But you can also draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos().
        // If you only use the ImDrawList API, you can notify the owner window of its extends by using SetCursorPos(max).
        ImVec2 canvas_pos = ui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
        ImVec2 canvas_size = ui::GetContentRegionAvail();        // Resize canvas to what's available
        if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
        if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
        draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(50, 50, 50, 255), IM_COL32(50, 50, 60, 255), IM_COL32(60, 60, 70, 255), IM_COL32(50, 50, 60, 255));
        draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), IM_COL32(255, 255, 255, 255));

        bool adding_preview = false;
        ui::InvisibleButton("canvas", canvas_size);
        ImVec2 mouse_pos_in_canvas = ImVec2(ui::GetIO().MousePos.x - canvas_pos.x, ui::GetIO().MousePos.y - canvas_pos.y);
        if (adding_line)
        {
            adding_preview = true;
            points.push_back(mouse_pos_in_canvas);
            if (!ui::IsMouseDown(0))
                adding_line = adding_preview = false;
        }
        if (ui::IsItemHovered())
        {
            if (!adding_line && ui::IsMouseClicked(0))
            {
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (ui::IsMouseClicked(1) && !points.empty())
            {
                adding_line = adding_preview = false;
                points.pop_back();
                points.pop_back();
            }
        }
        draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);      // clip lines within the canvas (if we resize it, etc.)
        for (int i = 0; i < points.Size - 1; i += 2)
            draw_list->AddLine(ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), ImVec2(canvas_pos.x + points[i + 1].x, canvas_pos.y + points[i + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
        draw_list->PopClipRect();
        if (adding_preview)
            points.pop_back();
    }
    ui::End();
}

//-----------------------------------------------------------------------------
// [SECTION] Example App: Documents Handling / ShowExampleAppDocuments()
//-----------------------------------------------------------------------------

// Simplified structure to mimic a Document model
struct MyDocument
{
    const char* Name;           // Document title
    bool        Open;           // Set when the document is open (in this demo, we keep an array of all available documents to simplify the demo)
    bool        OpenPrev;       // Copy of Open from last update.
    bool        Dirty;          // Set when the document has been modified
    bool        WantClose;      // Set when the document
    ImVec4      Color;          // An arbitrary variable associated to the document

    MyDocument(const char* name, bool open = true, const ImVec4& color = ImVec4(1.0f,1.0f,1.0f,1.0f))
    {
        Name = name;
        Open = OpenPrev = open;
        Dirty = false;
        WantClose = false;
        Color = color;
    }
    void DoOpen()       { Open = true; }
    void DoQueueClose() { WantClose = true; }
    void DoForceClose() { Open = false; Dirty = false; }
    void DoSave()       { Dirty = false; }

    // Display dummy contents for the Document
    static void DisplayContents(MyDocument* doc)
    {
        ui::PushID(doc);
        ui::Text("Document \"%s\"", doc->Name);
        ui::PushStyleColor(ImGuiCol_Text, doc->Color);
        ui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
        ui::PopStyleColor();
        if (ui::Button("Modify", ImVec2(100, 0)))
            doc->Dirty = true;
        ui::SameLine();
        if (ui::Button("Save", ImVec2(100, 0)))
            doc->DoSave();
        ui::ColorEdit3("color", &doc->Color.x);  // Useful to test drag and drop and hold-dragged-to-open-tab behavior.
        ui::PopID();
    }

    // Display context menu for the Document
    static void DisplayContextMenu(MyDocument* doc)
    {
        if (!ui::BeginPopupContextItem())
            return;

        char buf[256];
        sprintf(buf, "Save %s", doc->Name);
        if (ui::MenuItem(buf, "CTRL+S", false, doc->Open))
            doc->DoSave();
        if (ui::MenuItem("Close", "CTRL+W", false, doc->Open))
            doc->DoQueueClose();
        ui::EndPopup();
    }
};

struct ExampleAppDocuments
{
    ImVector<MyDocument> Documents;

    ExampleAppDocuments()
    {
        Documents.push_back(MyDocument("Lettuce",             true,  ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("Eggplant",            true,  ImVec4(0.8f, 0.5f, 1.0f, 1.0f)));
        Documents.push_back(MyDocument("Carrot",              true,  ImVec4(1.0f, 0.8f, 0.5f, 1.0f)));
        Documents.push_back(MyDocument("Tomato",              false, ImVec4(1.0f, 0.3f, 0.4f, 1.0f)));
        Documents.push_back(MyDocument("A Rather Long Title", false));
        Documents.push_back(MyDocument("Some Document",       false));
    }
};

// [Optional] Notify the system of Tabs/Windows closure that happened outside the regular tab interface.
// If a tab has been closed programmatically (aka closed from another source such as the Checkbox() in the demo, as opposed
// to clicking on the regular tab closing button) and stops being submitted, it will take a frame for the tab bar to notice its absence.
// During this frame there will be a gap in the tab bar, and if the tab that has disappeared was the selected one, the tab bar
// will report no selected tab during the frame. This will effectively give the impression of a flicker for one frame.
// We call SetTabItemClosed() to manually notify the Tab Bar or Docking system of removed tabs to avoid this glitch.
// Note that this completely optional, and only affect tab bars with the ImGuiTabBarFlags_Reorderable flag.
static void NotifyOfDocumentsClosedElsewhere(ExampleAppDocuments& app)
{
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (!doc->Open && doc->OpenPrev)
            ui::SetTabItemClosed(doc->Name);
        doc->OpenPrev = doc->Open;
    }
}

void ShowExampleAppDocuments(bool* p_open)
{
    static ExampleAppDocuments app;

    if (!ui::Begin("Example: Documents", p_open, ImGuiWindowFlags_MenuBar))
    {
        ui::End();
        return;
    }

    // Options
    static bool opt_reorderable = true;
    static ImGuiTabBarFlags opt_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;

    // Menu
    if (ui::BeginMenuBar())
    {
        if (ui::BeginMenu("File"))
        {
            int open_count = 0;
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                open_count += app.Documents[doc_n].Open ? 1 : 0;

            if (ui::BeginMenu("Open", open_count < app.Documents.Size))
            {
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                {
                    MyDocument* doc = &app.Documents[doc_n];
                    if (!doc->Open)
                        if (ui::MenuItem(doc->Name))
                            doc->DoOpen();
                }
                ui::EndMenu();
            }
            if (ui::MenuItem("Close All Documents", NULL, false, open_count > 0))
                for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
                    app.Documents[doc_n].DoQueueClose();
            if (ui::MenuItem("Exit", "Alt+F4")) {}
            ui::EndMenu();
        }
        ui::EndMenuBar();
    }

    // [Debug] List documents with one checkbox for each
    for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
    {
        MyDocument* doc = &app.Documents[doc_n];
        if (doc_n > 0)
            ui::SameLine();
        ui::PushID(doc);
        if (ui::Checkbox(doc->Name, &doc->Open))
            if (!doc->Open)
                doc->DoForceClose();
        ui::PopID();
    }

    ui::Separator();

    // Submit Tab Bar and Tabs
    {
        ImGuiTabBarFlags tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? ImGuiTabBarFlags_Reorderable : 0);
        if (ui::BeginTabBar("##tabs", tab_bar_flags))
        {
            if (opt_reorderable)
                NotifyOfDocumentsClosedElsewhere(app);

            // [DEBUG] Stress tests
            //if ((ImGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
            //if (ImGui::GetIO().KeyCtrl) ImGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..

            // Submit Tabs
            for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
            {
                MyDocument* doc = &app.Documents[doc_n];
                if (!doc->Open)
                    continue;

                ImGuiTabItemFlags tab_flags = (doc->Dirty ? ImGuiTabItemFlags_UnsavedDocument : 0);
                bool visible = ui::BeginTabItem(doc->Name, &doc->Open, tab_flags);

                // Cancel attempt to close when unsaved add to save queue so we can display a popup.
                if (!doc->Open && doc->Dirty)
                {
                    doc->Open = true;
                    doc->DoQueueClose();
                }

                MyDocument::DisplayContextMenu(doc);
                if (visible)
                {
                    MyDocument::DisplayContents(doc);
                    ui::EndTabItem();
                }
            }

            ui::EndTabBar();
        }
    }

    // Update closing queue
    static ImVector<MyDocument*> close_queue;
    if (close_queue.empty())
    {
        // Close queue is locked once we started a popup
        for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
        {
            MyDocument* doc = &app.Documents[doc_n];
            if (doc->WantClose)
            {
                doc->WantClose = false;
                close_queue.push_back(doc);
            }
        }
    }

    // Display closing confirmation UI
    if (!close_queue.empty())
    {
        int close_queue_unsaved_documents = 0;
        for (int n = 0; n < close_queue.Size; n++)
            if (close_queue[n]->Dirty)
                close_queue_unsaved_documents++;

        if (close_queue_unsaved_documents == 0)
        {
            // Close documents when all are unsaved
            for (int n = 0; n < close_queue.Size; n++)
                close_queue[n]->DoForceClose();
            close_queue.clear();
        }
        else
        {
            if (!ui::IsPopupOpen("Save?"))
                ui::OpenPopup("Save?");
            if (ui::BeginPopupModal("Save?"))
            {
                ui::Text("Save change to the following items?");
                ui::PushItemWidth(-1.0f);
                ui::ListBoxHeader("##", close_queue_unsaved_documents, 6);
                for (int n = 0; n < close_queue.Size; n++)
                    if (close_queue[n]->Dirty)
                        ui::Text("%s", close_queue[n]->Name);
                ui::ListBoxFooter();

                if (ui::Button("Yes", ImVec2(80, 0)))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                    {
                        if (close_queue[n]->Dirty)
                            close_queue[n]->DoSave();
                        close_queue[n]->DoForceClose();
                    }
                    close_queue.clear();
                    ui::CloseCurrentPopup();
                }
                ui::SameLine();
                if (ui::Button("No", ImVec2(80, 0)))
                {
                    for (int n = 0; n < close_queue.Size; n++)
                        close_queue[n]->DoForceClose();
                    close_queue.clear();
                    ui::CloseCurrentPopup();
                }
                ui::SameLine();
                if (ui::Button("Cancel", ImVec2(80, 0)))
                {
                    close_queue.clear();
                    ui::CloseCurrentPopup();
                }
                ui::EndPopup();
            }
        }
    }

    ui::End();
}

// End of Demo code
#else

void ImGui::ShowAboutWindow(bool*) {}
void ImGui::ShowDemoWindow(bool*) {}
void ImGui::ShowUserGuide() {}
void ImGui::ShowStyleEditor(ImGuiStyle*) {}

#endif
