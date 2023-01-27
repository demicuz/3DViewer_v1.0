#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui/cimgui.h"
#include "cimgui/cimgui_impl.h"
#include "3DViewer.h"

#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include <stdlib.h>
#include <string.h>

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

// TODO set custom font
void init_ui(GLFWwindow *window, t_app *app) {
  const char *glsl_version = "#version 330";

  igCreateContext(NULL);

  app->ioptr = igGetIO();
  // Enable Keyboard Controls
  app->ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // app->ioptr->FontDefault;

  float xscale;
  float yscale;
  // Get the ratio between the current DPI and the platform's default DPI.
  // TODO does this work well for HDPI displays? Probably not:
  // https://github.com/ocornut/imgui/issues/5081
  glfwGetWindowContentScale(window, &xscale, &yscale);

  // To get the physical size of the monitor in millimeters, use this:
  // int widthMM, heightMM;
  // glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &widthMM, &heightMM);

  // Should work for HDPI:
  // CGFloat scale = NSScreen.mainScreen.backingScaleFactor;
  // ImGuiStyle_ScaleAllSizes(igGetStyle(), scale);
  // ImFontConfig cfg;
  // cfg.SizePixels = 13.0f * scale;
  // io.Fonts->AddFontDefault(&cfg);
  // io.FontGlobalScale = 1.0f / scale;

  if (xscale != 0) {
    app->ioptr->FontGlobalScale = xscale;
  }

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void load_file(t_app *app, t_object *obj) {
  nfdchar_t *outPath = NULL;
  nfdresult_t result = NFD_OpenDialog("obj", "./models", &outPath);

  if (result == NFD_OKAY) {
    strcpy(app->filepath, outPath);
    free(outPath);
    puts(app->filepath);
    app->basename = strrchr(app->filepath, '/') + 1;

    if (parse_obj(app->filepath, obj)) {
      app->parse_error = false;
      app->model_was_updated = true;
    } else {
      app->parse_error = true;
    }
  } else if (result == NFD_CANCEL) {
    puts("User pressed cancel.");
  } else {
    (void)fprintf(stderr, "Error: %s\n", NFD_GetError());
  }
}

void render_ui(t_app *app, t_object *obj) {
  // TODO does this have an overhead?
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  igNewFrame();

  // static bool open;
  // igShowDemoWindow(&open);

  igBegin("Control Panel", NULL, 0);

  igDummy((ImVec2){.x = 0, .y = 3});
  ImVec2 buttonSize = {.x = 0, .y = 0};
  if (igButton("Load .obj file", buttonSize)) {
    load_file(app, obj);
  }

  if (*app->filepath) {
    igDummy((ImVec2){.x = 0, .y = 5});
    igText(app->basename);
    if (!app->parse_error) {
      igText("Vertices: %d", obj->vertex_count);
      igText("Edges: %d", obj->edge_count);
    } else {
      igTextColored((ImVec4){1, 0, 0, 1}, "Parse error!");
    }
  }

  igDummy((ImVec2){.x = 0, .y = 5});
  igSeparator();

  igDummy((ImVec2){.x = 0, .y = 5});
  igText("Font scale");
  igDragFloat("###", &app->ioptr->FontGlobalScale, 0.005f, 0.75f, 3.0f, "%.2f",
              ImGuiSliderFlags_AlwaysClamp);

  igDummy((ImVec2){.x = 0, .y = 5});
  igColorEdit3("bg_col", app->bg_col.raw, 0);
  igColorEdit3("line_col", app->line_col.raw, 0);

  igDummy((ImVec2){.x = 0, .y = 5});
  igSeparator();

  igDummy((ImVec2){.x = 0, .y = 5});
  igText("Translation");
  obj->view_was_updated |=
      igSliderFloat("X", &obj->translation.x, -1, 1, "%.3f", 0);
  obj->view_was_updated |=
      igSliderFloat("Y", &obj->translation.y, -1, 1, "%.3f", 0);
  obj->view_was_updated |=
      igSliderFloat("Z", &obj->translation.z, -1, 1, "%.3f", 0);

  igDummy((ImVec2){.x = 0, .y = 5});
  igText("Rotation");
  obj->view_was_updated |=
      igSliderFloat("OX", &obj->rotation.x, -floatPI, floatPI, "%.3f", 0);
  obj->view_was_updated |=
      igSliderFloat("OY", &obj->rotation.y, -floatPI, floatPI, "%.3f", 0);
  obj->view_was_updated |=
      igSliderFloat("OZ", &obj->rotation.z, -floatPI, floatPI, "%.3f", 0);

  igDummy((ImVec2){.x = 0, .y = 5});
  igText("Scale");
  obj->view_was_updated |=
      igSliderFloat("##", &obj->scale, 0.01f, 5, "%.3f", 0);

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
