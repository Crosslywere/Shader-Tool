#include <Shader.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cstdlib>
#include <memory>
#include <iostream>

static void ProcessInputs(GLFWwindow* window, FrameBufferShader& shader) {
	static bool refreshed = false;
	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (!refreshed) {
			shader.Reload();
			refreshed = true;
#ifdef DEBUG
			if (shader.IsError())
				std::cout << shader.GetErrorStr() << std::endl;
#endif
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			void* userpointer = glfwGetWindowUserPointer(window);
			if (userpointer != nullptr) {
				FrameBufferShader* shader = reinterpret_cast<FrameBufferShader*>(userpointer);
				shader->SetResolution(w, h);
			}
		}
	}
	else {
		refreshed = false;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

static void WindowResizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	void* userpointer = glfwGetWindowUserPointer(window);
	if (userpointer != nullptr) {
		FrameBufferShader* shader = reinterpret_cast<FrameBufferShader*>(userpointer);
		shader->SetResolution(width, height);
	}
}

static void MousePosCallback(GLFWwindow* window, double mouseX, double mouseY) {
	void* userpointer = glfwGetWindowUserPointer(window);
	if (userpointer != nullptr) {
		FrameBufferShader* shader = reinterpret_cast<FrameBufferShader*>(userpointer);
		shader->SetMousePos(mouseX, mouseY);
	}
}

int main(int argc, char** argv) {
	glfwSetErrorCallback([](int error_code, const char* description) {
		std::printf("ErrorCode %i - %s\n", error_code, description);
		});
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Shader Tool", nullptr, nullptr);
	if (window == nullptr) {
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwDestroyWindow(window);
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glViewport(0, 0, 800, 600);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
	FrameBufferShader::Init();
	{
		FrameBufferShader frameBufferShader{ "res/basic.glsl" };
		glfwSetWindowUserPointer(window, &frameBufferShader);
		glfwSetWindowSizeCallback(window, WindowResizeCallback);
		glfwSetCursorPosCallback(window, MousePosCallback);
		frameBufferShader.Bind();
		frameBufferShader.SetResolution(800, 600);
		float frameBufferVertices[16] = {
			-1.0f,-1.0f, 0.0f, 0.0f,
			 1.0f,-1.0f, 1.0f, 0.0f,
			 1.0f, 1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 0.0f, 1.0f,
		};
		unsigned int frameBufferIndices[6] = {
			0, 1, 2,
			2, 3, 0,
		};
		unsigned int vao, vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(frameBufferVertices), frameBufferVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 0));
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(sizeof(float) * 2));
		glEnableVertexAttribArray(1);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsDark();

		// From backends/imgui_impl_glfw.h
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		// From backends/imgui_impl_opengl3.h
		ImGui_ImplOpenGL3_Init();

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			// ImGui Render code
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGui::Begin("Shader Tool");
			ImGui::Text("Hello, Shader! %.3fs", glfwGetTime());
			ImGui::End();
			ImGui::Render();
			ProcessInputs(window, frameBufferShader);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			frameBufferShader.Bind();
			frameBufferShader.SetTime(glfwGetTime());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, frameBufferIndices);
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
		}
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);
	}
	FrameBufferShader::Deinit();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}