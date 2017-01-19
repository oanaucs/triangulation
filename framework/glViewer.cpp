#include "glViewer.hpp"
#include <array>
#include <utils.hpp>
#include <sstream>
#include <iostream>

GLViewer::GLViewer(const std::string vertexShaderFilepath, const std::string fragmentShaderFilepath, Window const &viewerWindow)
{
	m_vertexFilepath = vertexShaderFilepath;
	m_fragFilepath = fragmentShaderFilepath;
	g_volume_program = 0;
	bool g_reload_shader_error = false;
	g_error_message = "";
	m_viewerWindow = std::make_shared<Window>(viewerWindow);
	g_background_color = glm::vec3(0.08f, 0.08f, 0.08f); 
	camera_position = glm::vec3(0.0f, 0.0f, 8.0f);
	setColor(glm::vec4(0.0, 1.0, 0.0, 1.0));
	 
}

void GLViewer::setColor(glm::vec4 color)
{
	current_color[0] = color.x;
	current_color[1] = color.y;
	current_color[2] = color.z;
	current_color[3] = color.t;
	glUniform4fv(glGetUniformLocation(g_volume_program, "Color"), 1, current_color);
}

GLViewer::~GLViewer()
{

}

GLuint GLViewer::loadShaders(std::string const& vs, std::string const& fs)
{
    std::string v = readFile(vs);
    std::string f = readFile(fs);
    return createProgram(v, f);
}

void GLViewer::loadProgram(bool reload)
{
    GLuint newProgram(0);
    try {
		g_volume_program = loadShaders(m_vertexFilepath, m_fragFilepath);
    }
    catch (std::logic_error& e) {
        std::stringstream ss;
        ss << e.what() << std::endl;
        g_error_message = ss.str();
        g_reload_shader_error = true;
        if (reload) newProgram = 0;
    }
    if (0 != newProgram && reload) {
        glDeleteProgram(g_volume_program);
        g_volume_program = newProgram;
        g_reload_shader_error = false;
    }
    else
    {
        g_reload_shader_error = true;

    }
}

void GLViewer::setupViewport()
{
	glm::ivec2 size = m_viewerWindow->windowSize();
    glViewport(0, 0, size.x, size.y);
    glClearColor(g_background_color.x, g_background_color.y, g_background_color.z, 1.0);
       // glClearColor(1, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float fovy = 90.0f;
    float aspect = (float)size.x / (float)size.y;
    float zNear = 0.1f, zFar = 100.0f;
    projection = glm::perspective(fovy, aspect, zNear, zFar); 
	//projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);

	model = glm::mat4(1.0f);
	//model = glm::translate(glm::mat4(1.0f), camera_position);

	view = glm::lookAt(camera_position, glm::vec3(0,0,0), glm::vec3(0, 1, 0));

}


void GLViewer::init()
{
	g_volume_program = loadShaders(m_vertexFilepath, m_fragFilepath);

}

void GLViewer::setViewMatrix()
{
	view = glm::lookAt(camera_position, glm::vec3(0,0,0), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(g_volume_program, "View"), 1, GL_FALSE,
            glm::value_ptr(view));
}

void GLViewer::startProgram()
{
	glUseProgram(g_volume_program);

    glUniformMatrix4fv(glGetUniformLocation(g_volume_program, "Projection"), 1, GL_FALSE,
            glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(g_volume_program, "Model"), 1, GL_FALSE,
            glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(g_volume_program, "View"), 1, GL_FALSE,
            glm::value_ptr(view));
	glUniform4fv(glGetUniformLocation(g_volume_program, "Color"), 1, current_color);
}

void GLViewer::endProgram()
{
	glUseProgram(0);
}

void GLViewer::zoomIn()
{
	camera_position.z = camera_position.z - 0.5f;
}

void GLViewer::zoomOut()
{
	camera_position.z = camera_position.z + 0.5f;
}

void GLViewer::goLeft()
{
	camera_position.x = camera_position.x - 0.5f;
}

void GLViewer::goRight()
{
	camera_position.x = camera_position.x + 0.5f;
}

void GLViewer::goUp()
{
	camera_position.y = camera_position.y - 0.5f;
}

void GLViewer::goDown()
{
	camera_position.y = camera_position.y + 0.5f;
}
/*GLuint GLViewer::loadShaders(
    std::string const& vs,
    std::string const& fs,
    const int task_nbr,
    const int enable_lighting,
    const int enable_shadowing,
    const int enable_opeacity_cor)
{
    std::string v = readFile(vs);
    std::string f = readFile(fs);

    //std::cout << f << std::endl;

    return createProgram(v, f);
}*/

