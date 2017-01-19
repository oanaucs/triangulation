#ifndef GL_VIEWER_HPP
#define GL_VIEWER_HPP

#include <GL/glew.h>
#ifdef __APPLE__
# define __gl3_h_
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include "fensterchen.hpp"
#include <memory>

class GLViewer
{
public:
  GLViewer(const std::string vertexShaderFilepath, const std::string fragmentShaderFilepath, Window const &viewerWindow);
  ~GLViewer();
  GLuint loadShaders(std::string const& vs, std::string const& fs);
  void loadProgram(bool reload);
  void init();
  void setupViewport();
  void startProgram();
  void endProgram();
  void zoomIn();
  void zoomOut();
  void goLeft();
  void goRight();
  void goUp();
  void goDown();
  void setViewMatrix();
  void setColor(glm::vec4 color);

private:
	GLuint g_volume_program;
	std::string g_error_message;
	bool g_reload_shader_error;
	std::string m_vertexFilepath;
	std::string m_fragFilepath;
	std::shared_ptr<Window> m_viewerWindow;
	glm::vec3 g_background_color;
	glm::mat4 projection;
	glm::vec3 camera_position;
	glm::mat4 view;
	glm::mat4 model;
	float current_color[4];
};

#endif
