#pragma once

#include <string>
#include <unordered_map>

class Shader {
public:
	Shader() = default;
	Shader(const Shader&) = default;
	Shader(Shader&&) = default;
	Shader& operator=(const Shader&) = default;
	Shader(const std::string& vertexShader, const std::string& fragmentShader);
	~Shader();
	void Bind() const;
	virtual void Reload();
	void SetUniform1f(const std::string& name, float v);
	void SetUniform2f(const std::string& name, float v1, float v2);
	void SetUniform3f(const std::string& name, float v1, float v2, float v3);
	inline bool IsVertexError() const { return !m_VertexErrorStr.empty(); }
	inline const std::string_view& GetVertexError() const { return m_VertexErrorStr; }
	inline bool IsFragmentError() const { return !m_FragmentErrorStr.empty(); }
	inline const std::string_view& GetFragmentError() const { return m_FragmentErrorStr; }
	inline bool IsError() const { return !m_ErrorStr.empty(); }
	inline const std::string_view& GetErrorStr() const { return m_ErrorStr; }
protected:
	int GetUniformLocation(const std::string& name);
	unsigned int m_ProgramObject{};
	std::string m_ErrorStr{};
	std::unordered_map<std::string, int> m_UniformMap;
private:
	std::string m_VertexFilePath{};
	std::string m_VertexErrorStr{};
	std::string m_FragmentFilePath{};
	std::string m_FragmentErrorStr{};
};

class FrameBufferShader : public Shader {
public:
	static void Init();
	static void Deinit();
	FrameBufferShader() = default;
	FrameBufferShader(const FrameBufferShader&) = default;
	FrameBufferShader(FrameBufferShader&&) = default;
	FrameBufferShader& operator=(const FrameBufferShader&) = default;
	FrameBufferShader(const std::string& filePath);
	virtual void Reload() override;
	void SetTime(float time);
	void SetResolution(int width, int height);
	void SetMousePos(float mouseX, float mouseY);
private:
	std::string m_FilePath{};
	static unsigned int s_DefaultVertexShader;
};