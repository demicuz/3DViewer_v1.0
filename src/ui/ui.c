#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "3DViewer.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

// TODO adapt for High DPI displays
// TODO set custom font
void init_ui(GLFWwindow *window) {
  const char* glsl_version = "#version 330";

  igCreateContext(NULL);

  ImGuiIO *ioptr = igGetIO();
  // Enable Keyboard Controls
  ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void render_ui(t_app *app) {
	// TODO does this have an overhead?
	ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();

  igShowDemoWindow(&app->show_ui);

  igRender();
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void ui_cleanup(void) {
	ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);
}
