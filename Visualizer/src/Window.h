#pragma once

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;
	bool Vsync;

	WindowProps(const std::string& title = "Visualizer",
		unsigned int width = 1280,
		unsigned int height = 720, bool vsync=true)
		: Title(title), Width(width), Height(height), Vsync(vsync)
	{
	}
};

class Window {
public:
	Window(const WindowProps& props);
	virtual ~Window();

	void OnUpdate();

	inline unsigned int GetWidth() const { return windowprops.Width; }
	inline unsigned int GetHeight() const { return windowprops.Height; }
	inline int GetKey(int key) const { return glfwGetKey(window, key); }

	void SetVSync(bool enabled);
	inline bool IsVSync() const { return windowprops.Vsync; }

	static inline double GetTime() { return glfwGetTime(); }
	double GetDeltaTime();

	inline GLFWwindow* GetNativeWindow() const { return window; }
	inline bool ShouldClose() const { return glfwWindowShouldClose(window); }
	inline void Close() const { glfwSetWindowShouldClose(window, GLFW_TRUE); }

	static void TerminateWindow();
private:
	virtual void Init(const WindowProps& props);
	virtual void Shutdown();
private:
	GLFWwindow* window;
	WindowProps windowprops;
	double timeLastGetDeltaTime = 0.0;
};