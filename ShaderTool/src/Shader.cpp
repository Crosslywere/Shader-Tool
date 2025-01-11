#include <Shader.hpp>

#include <glad/glad.h>

#include <fstream>
#include <sstream>

#define INFO_LOG_LENGTH 1024

inline unsigned int FrameBufferShader::s_DefaultVertexShader = 0;

inline const char* frameBufferVertexSourceDefault =
R"(#version 420 core
layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 TexCoord;
out vec2 aPos;
out vec2 aTexCoord;
void main() {
	aPos = Pos;
	aTexCoord = TexCoord;
	gl_Position = vec4(Pos, 0.0, 1.0);	
})";

static GLuint LoadShaderFromFile(const std::string& filePath, GLenum type) {
	std::ifstream fs;
	fs.open(filePath);
	if (fs.is_open()) {
		GLuint shader = glCreateShader(type);
		std::stringstream ss;
		ss << fs.rdbuf();
		std::string srcStr = ss.str();
		const char* src = srcStr.c_str();
		glShaderSource(shader, 1, &src, 0);
		return shader;
	}
	std::printf("Unable to open shader '%s'\n", filePath.c_str());
	return 0;
}

static std::string CheckShaderCompileError(GLuint shader) {
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[INFO_LOG_LENGTH];
		glGetShaderInfoLog(shader, INFO_LOG_LENGTH, nullptr, infoLog);
#ifdef DEBUG
		std::printf("%s\n", infoLog);
#endif
		return infoLog;
	}
	return "";
}

Shader::Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
	: m_VertexFilePath(vertexShaderFile), m_FragmentFilePath(fragmentShaderFile) {
	Reload();
}

Shader::~Shader() {
	if (m_ProgramObject != 0) {
		glDeleteProgram(m_ProgramObject);
		m_ProgramObject = 0;
	}
}

void Shader::Bind() const {
	glUseProgram(m_ProgramObject);
}

void Shader::Reload() {
	if (m_ProgramObject != 0)
		glDeleteProgram(m_ProgramObject);
	m_ProgramObject = glCreateProgram();
	auto vs = LoadShaderFromFile(m_VertexFilePath, GL_VERTEX_SHADER);
	auto fs = LoadShaderFromFile(m_FragmentFilePath, GL_FRAGMENT_SHADER);
	if (vs == 0 || fs == 0) {
		m_ErrorStr = "Unable to load file(s)";
		return;
	}
	glCompileShader(vs);
	m_VertexErrorStr = CheckShaderCompileError(vs);
	glCompileShader(fs);
	m_FragmentErrorStr = CheckShaderCompileError(fs);
	if (IsVertexError() || IsFragmentError()) {
		return;
	}
	glAttachShader(m_ProgramObject, vs);
	glAttachShader(m_ProgramObject, fs);
	glLinkProgram(m_ProgramObject);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void Shader::SetUniform1f(const std::string& name, float v) {
	int loc = GetUniformLocation(name);
	if (loc >= 0) {
		Bind();
		glUniform1f(loc, v);
	}
}

void Shader::SetUniform2f(const std::string& name, float v1, float v2) {
	int loc = GetUniformLocation(name);
	if (loc >= 0) {
		Bind();
		glUniform2f(loc, v1, v2);
	}
}

void Shader::SetUniform3f(const std::string& name, float v1, float v2, float v3) {
	int loc = GetUniformLocation(name);
	if (loc >= 0) {
		Bind();
		glUniform3f(loc, v1, v2, v3);
	}
}

int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformMap.find(name) == m_UniformMap.end()) {
		int loc = glGetUniformLocation(m_ProgramObject, name.c_str());
		m_UniformMap.insert(std::make_pair(name, loc));
	}
	return m_UniformMap.at(name);
}

void FrameBufferShader::Init() {
	if (s_DefaultVertexShader == 0) {
		s_DefaultVertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(s_DefaultVertexShader, 1, &frameBufferVertexSourceDefault, nullptr);
		glCompileShader(s_DefaultVertexShader);
	}
}

void FrameBufferShader::Deinit() {
	if (s_DefaultVertexShader != 0) {
		glDeleteShader(s_DefaultVertexShader);
	}
}

FrameBufferShader::FrameBufferShader(const std::string& fragmentFilePath) : m_FilePath(fragmentFilePath) {
	Reload();
}

void FrameBufferShader::Reload() {
	if (m_ProgramObject != 0)
		glDeleteProgram(m_ProgramObject);
	m_ProgramObject = glCreateProgram();
	glAttachShader(m_ProgramObject, s_DefaultVertexShader);
	auto fs = LoadShaderFromFile(m_FilePath, GL_FRAGMENT_SHADER);
	if (fs == 0) {
		m_ErrorStr = "Unable to load file";
		return;
	}
	glCompileShader(fs);
	m_ErrorStr = CheckShaderCompileError(fs);
	if (IsError()) {
		return;
	}
	glAttachShader(m_ProgramObject, fs);
	glLinkProgram(m_ProgramObject);
	glDeleteShader(fs);
	m_UniformMap.clear();
}

void FrameBufferShader::SetTime(float time) {
	SetUniform1f("uTime", time);
}

void FrameBufferShader::SetResolution(int width, int height) {
	SetUniform2f("uResolution", width, height);
}

void FrameBufferShader::SetMousePos(float mouseX, float mouseY) {
	SetUniform2f("uMousePos", mouseX, mouseY);
}


