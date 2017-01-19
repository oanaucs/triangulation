// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
//                  2015 Sebastian Thiele
// License    : MIT (see the file LICENSE)
// Maintainer : Sebastian Thiele <sebastian.thiele@uni-weimar.de>
// Stability  : experimantal exercise
//
// scivis exercise Example
// -----------------------------------------------------------------------------
#ifdef _MSC_VER
#pragma warning (disable: 4996)         // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#endif

#define _USE_MATH_DEFINES
#include "fensterchen.hpp"
#include <string>
#include <iostream>
#include <sstream>      // std::stringstream
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <memory>

///GLM INCLUDES
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>

///PROJECT INCLUDES
#include <volume_loader_raw.hpp>
#include <transfer_function.hpp>
#include <utils.hpp>
#include <turntable.hpp>
#include <imgui.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>        // stb_image.h for PNG loading
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
#include "Gui.hpp"
#include "TriangulationModel.hpp"
#include "glViewer.hpp"

///IMGUI INCLUDES
#include <imgui_impl_glfw_gl3.h>


//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#undef PI
const float PI = 3.14159265358979323846f;

#ifdef INT_MAX
#define IM_INT_MAX INT_MAX
#else
#define IM_INT_MAX 2147483647
#endif

// Play it nice with Windows users. Notepad in 2014 still doesn't display text data with Unix-style \n.
#ifdef _MSC_VER
#define STR_NEWLINE "\r\n"
#else
#define STR_NEWLINE "\n"
#endif

const std::string g_file_vertex_shader("../source/shader/vertexShader.vert");
const std::string g_file_fragment_shader("../source/shader/fragmentShader.frag");

glm::ivec2  g_window_res = glm::ivec2(1600, 800);
Window g_win(g_window_res);

// imgui variables
static bool g_show_gui = true;
static bool mousePressed[2] = { false, false };

//imgui values
bool g_over_gui = false;
bool  g_pause = false;

Volume_loader_raw g_volume_loader;
volume_data_type g_volume_data;
glm::ivec3 g_vol_dimensions;
glm::vec3 g_max_volume_bounds;
unsigned g_channel_size = 0;
unsigned g_channel_count = 0;
GLuint g_volume_texture = 0;
//Cube g_cube;
//Rectangle g_rectangle;
GLViewer glViewer(g_file_vertex_shader, g_file_fragment_shader, g_win);

int g_bilinear_interpolation = true;

bool first_frame = true;

void UpdateImGui()
{
    ImGuiIO& io = ImGui::GetIO();

    // Setup resolution (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    glfwGetWindowSize(g_win.getGLFWwindow(), &w, &h);
    glfwGetFramebufferSize(g_win.getGLFWwindow(), &display_w, &display_h);
    io.DisplaySize = ImVec2((float)display_w, (float)display_h);                                   // Display size, in pixels. For clamping windows positions.

    // Setup time step
    static double time = 0.0f;
    const double current_time = glfwGetTime();
    io.DeltaTime = (float)(current_time - time);
    time = current_time;
    
    // Setup inputs
    // (we already got mouse wheel, keyboard keys & characters from glfw callbacks polled in glfwPollEvents())
    double mouse_x, mouse_y;
    glfwGetCursorPos(g_win.getGLFWwindow(), &mouse_x, &mouse_y);
    mouse_x *= (float)display_w / w;                                                               // Convert mouse coordinates to pixels
    mouse_y *= (float)display_h / h;
    io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);                                          // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
    io.MouseDown[0] = mousePressed[0] || glfwGetMouseButton(g_win.getGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    io.MouseDown[1] = mousePressed[1] || glfwGetMouseButton(g_win.getGLFWwindow(), GLFW_MOUSE_BUTTON_RIGHT) != 0;

    // Start the frame
    //ImGui::NewFrame();
    ImGui_ImplGlfwGL3_NewFrame();
}


void showGUI(Gui &gui){

    ImGui::Begin("Application Settings", &g_show_gui, ImVec2(300, 500));
    static float f;
    g_over_gui = ImGui::IsMouseHoveringAnyWindow();

    // Calculate and show frame rate
    static ImVector<float> ms_per_frame; if (ms_per_frame.empty()) { ms_per_frame.resize(400); memset(&ms_per_frame.front(), 0, ms_per_frame.size()*sizeof(float)); }
    static int ms_per_frame_idx = 0;
    static float ms_per_frame_accum = 0.0f;
    if (!g_pause){
        ms_per_frame_accum -= ms_per_frame[ms_per_frame_idx];
        ms_per_frame[ms_per_frame_idx] = ImGui::GetIO().DeltaTime * 1000.0f;
        ms_per_frame_accum += ms_per_frame[ms_per_frame_idx];

        ms_per_frame_idx = (ms_per_frame_idx + 1) % ms_per_frame.size();
    }
    const float ms_per_frame_avg = ms_per_frame_accum / 120;

	gui.showGUIElements();

    ImGui::End();
}



void setupKeyActions()
{
    if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE)) {                
        g_win.stop();
    }

	if (ImGui::IsKeyPressed(GLFW_KEY_W))
	{
		glViewer.zoomIn();
		std::cout<<"zoom out";
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_S))
	{
		glViewer.zoomOut();
		std::cout<<"zoom out";
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_LEFT)) {
		glViewer.goLeft();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_RIGHT)) {
		glViewer.goRight();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_UP)) {
		glViewer.goUp();
	}

	if (ImGui::IsKeyPressed(GLFW_KEY_DOWN)) {
		glViewer.goDown();
	}


    /*if (ImGui::IsKeyPressed(GLFW_KEY_LEFT)) {
        g_light_pos.x -= 0.5f;
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_RIGHT)) {
        g_light_pos.x += 0.5f;
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_UP)) {
        g_light_pos.z -= 0.5f;
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_DOWN)) {
        g_light_pos.z += 0.5f;
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_MINUS)) {
        g_iso_value -= 0.002f;
        g_iso_value = std::max(g_iso_value, 0.0f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_EQUAL) || ImGui::IsKeyPressed(GLFW_KEY_KP_ADD)) {
        g_iso_value += 0.002f;
        g_iso_value = std::min(g_iso_value, 1.0f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_D)) {
        g_sampling_distance -= 0.0001f;
        g_sampling_distance = std::max(g_sampling_distance, 0.0001f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_S)) {
        g_sampling_distance += 0.0001f;
        g_sampling_distance = std::min(g_sampling_distance, 0.2f);
    }

    if (ImGui::IsKeyPressed(GLFW_KEY_R)) {
        g_reload_shader = true;
    }

    if (ImGui::IsMouseDown(GLFW_MOUSE_BUTTON_LEFT) || ImGui::IsMouseDown(GLFW_MOUSE_BUTTON_MIDDLE) || ImGui::IsMouseDown(GLFW_MOUSE_BUTTON_RIGHT)) {
        sampling_fact = g_sampling_distance_fact_move;
    }   
    if (ImGui::IsKeyPressed(GLFW_KEY_R)) {
            if (g_reload_shader_pressed != true) {
                g_reload_shader = true;
                g_reload_shader_pressed = true;
            }
            else{
                g_reload_shader = false;
            }
        }
        else {
            g_reload_shader = false;
            g_reload_shader_pressed = false;
        }*/
}

int main(int argc, char* argv[])
{
    ImGui_ImplGlfwGL3_Init(g_win.getGLFWwindow(), true);
	
	TriangulationModel* triangModel = new TriangulationModel(1.0f, 1.0f);
	Gui gui(triangModel);

	glViewer.init();
    // manage keys here
    // add new input if neccessary (ie changing sampling distance, isovalues, ...)
    while (!g_win.shouldClose()) {
        if (!first_frame > 0.0){
            setupKeyActions();
        }
		glViewer.setupViewport();
		glViewer.startProgram();
		// draw commands:
			// draw rectangle for domain
		glViewer.setColor(glm::vec4(0.839216f, 0.839216f, 0.839216f, 1.0f));
		triangModel->drawDomainRectangle();
		glViewer.setColor(glm::vec4(0.184314f, 0.309804f, 0.309804f, 1.0f));
		triangModel->drawCracks();
		glViewer.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		triangModel->drawPoints();
		glViewer.setColor(glm::vec4(0.6f, 0.7f, 0.3f, 1.0f));
		triangModel->drawQuads();
		glViewer.setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		triangModel->drawTriangulationPoints();
		glViewer.setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
		triangModel->drawTriangles();

		glViewer.endProgram();

        //IMGUI ROUTINE begin    
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel = 0;
        mousePressed[0] = mousePressed[1] = false;
        glfwPollEvents();
        UpdateImGui();
		showGUI(gui);

        // Rendering
        ImGui::Render();
        //IMGUI ROUTINE end

        g_win.update();
        first_frame = false;
    }

    //ImGui::Shutdown();
    ImGui_ImplGlfwGL3_Shutdown();

    return 0;
}
