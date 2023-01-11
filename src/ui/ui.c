#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "3DViewer.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include <stdlib.h>

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

// TODO adapt for High DPI displays
// TODO set custom font
void init_ui(GLFWwindow *window, t_app *app) {
  const char* glsl_version = "#version 330";

  igCreateContext(NULL);

  app->ioptr = igGetIO();
  // Enable Keyboard Controls
  app->ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // app->ioptr->FontGlobalScale = 1.25f;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void render_ui(t_app *app, t_object *obj) {
	// TODO does this have an overhead?
	ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();

  // igShowDemoWindow(&app->show_ui);

  // static int counter = 0;

  igBegin("Affine transformations", NULL, 0);

  igText("Translation");
  obj->view_was_updated |= igSliderFloat("X", &obj->translation.x, -1, 1, "%.3f", 0);
  obj->view_was_updated |= igSliderFloat("Y", &obj->translation.y, -1, 1, "%.3f", 0);
  obj->view_was_updated |= igSliderFloat("Z", &obj->translation.z, -1, 1, "%.3f", 0);
  igText("Rotation");
  obj->view_was_updated |= igSliderFloat("OX", &obj->rotation.x, -3.1415926535f, 3.1415926535f, "%.3f", 0);
  obj->view_was_updated |= igSliderFloat("OY", &obj->rotation.y, -3.1415926535f, 3.1415926535f, "%.3f", 0);
  obj->view_was_updated |= igSliderFloat("OZ", &obj->rotation.z, -3.1415926535f, 3.1415926535f, "%.3f", 0);
  igText("Scale");
  obj->view_was_updated |= igSliderFloat("##", &obj->scale, 0, 2, "%.3f", 0);

  igText("Font scale");
  igDragFloat("###", &app->ioptr->FontGlobalScale, 0.005f, 0.75f, 3.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

  ImVec2 buttonSize;
  buttonSize.x = 0;
  buttonSize.y = 0;
  if (igButton("Load .obj file", buttonSize)) {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

    if (result == NFD_OKAY) {
        puts("Success!");
        puts(outPath);
        free(outPath);
    }
    else if (result == NFD_CANCEL) {
        puts("User pressed cancel.");
    }
    else {
        printf("Error: %s\n", NFD_GetError());
    }
  }
  // igSameLine(0.0f, -1.0f);
  // igText("counter = %d", counter);

  // igText("Application average %.3f ms/frame (%.1f FPS)",
         // 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
  igEnd();
}

void draw_ui(void) {
  igRender();
  ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void ui_cleanup(void) {
	ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  igDestroyContext(NULL);
}
