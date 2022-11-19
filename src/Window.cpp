#include "Window.h"

Window::Window(const char *title) : backgroundColor(glm::vec4(0, 0, 0, 1))
{
    m_Title = title;

    if(!init())
        glfwTerminate();

    for(int i = 0; i < MAX_KEYS ; i++)
    {
        m_HeldKeys[i] = false;
		m_PressedKeys[i] = false;
		m_ReleasedKeys[i] = false;
    }
    for(int i = 0; i < MAX_BUTTONS ; i++)
    {
        m_HeldMouseButtons[i] = false;
    }

    firstMouse = true;
    lastMouseX = m_Width / 2;
    lastMouseY = m_Height / 2;
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::init()
{
    glfwSetErrorCallback(glfw_initialisation_error);
    if(!glfwInit())
        return false;

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
#endif

    glfwWindowHint(GLFW_SAMPLES, 16);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    m_Width = (float)mode->width;
    m_Height = (float)mode->height;

    m_Window = glfwCreateWindow(mode->width, mode->height, m_Title, NULL, NULL);
    if(!m_Window)
    {
        glfwTerminate();
        std::cerr << "ERROR::GLFW::Could not initialise GLFW Window" << '\n';
        return false;
    }
    glfwMakeContextCurrent(m_Window);

    glfwSetWindowUserPointer(m_Window, this);

    // Window Callback Functions
    glfwSetFramebufferSizeCallback(m_Window, window_resize_callback);
    glfwSetKeyCallback(m_Window, key_callback);
    glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
    glfwSetCursorPosCallback(m_Window, mouse_position_callback);
    glfwSetScrollCallback(m_Window, scroll_callback);

    // initialize GLAD using glfw
    //int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        //getchar();
        glfwTerminate();
        return false;
    }

    // Print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    //glEnable(GL_POLYGON_SMOOTH);
    //glEnable(GL_LINE_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    //glShadeModel(GL_SMOOTH);
    //glDisable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

bool Window::isKeyPressed(unsigned int keycode)
{
	if (keycode >= MAX_KEYS)
		return false;

    bool result = m_PressedKeys[keycode];
    m_PressedKeys[keycode] = false;

    return result;
}

bool Window::isKeyReleased(unsigned int keycode)
{
	if (keycode >= MAX_KEYS)
		return false;

	bool result = m_ReleasedKeys[keycode];
    m_ReleasedKeys[keycode] = false;

    return result;
}

bool Window::isKeyHeld(unsigned int keycode) const
{
	// TODO: Log this
	if (keycode >= MAX_KEYS)
		return false;

	return m_HeldKeys[keycode];
}

bool Window::isMouseButtonPressed(unsigned int button)
{
	if (button >= MAX_BUTTONS)
		return false;

	bool result = m_PressedMouseButtons[button];
    m_PressedMouseButtons[button] = false;

	return result;
}

bool Window::isMouseButtonReleased(unsigned int button)
{
	if (button >= MAX_BUTTONS)
		return false;

	bool result = m_ReleasedMouseButtons[button];
    m_ReleasedMouseButtons[button] = false;

	return result;
}

bool Window::isMouseButtonHeld(unsigned int button) const
{
	// TODO: Log this
	if (button >= MAX_BUTTONS)
		return false;

	return m_HeldMouseButtons[button];
}

void Window::getMousePosition(double& x, double& y) const
{
    x = m_MouseX;
    y = m_MouseY;
}

bool Window::isMouseScrollRoll(float& roll) const
{
    if (roll != 0)
        return true;
    else
        return false;
}

glm::mat4 Window::ProjMat(const float& Th, const float& Zn, const float& Zf)
{
    float A = getHeight() / getWidth();
    glm::mat4 ProjMat(0.0f);
    float F = 1.0f / tanf(Th / (2.0f * 180.0f * 3.14159f));
    const float q = Zf / (Zf - Zn);

    ProjMat[0][0] = A * F;
    ProjMat[1][1] = F;
    ProjMat[2][2] = q;
    ProjMat[3][2] = -Zn * q;
    ProjMat[2][3] = 1;

    return ProjMat;
}

void Window::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w);
}

void Window::update() const
{
    double currentTime = glfwGetTime(); deltaTime = currentTime - m_LastTime; m_LastTime = currentTime;

    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

bool Window::closed() const
{
    return glfwWindowShouldClose(m_Window) == 1;
}

void glfw_initialisation_error(int error, const char* description)
{
    std::cerr << "ERROR::GLFW::" << error << "::DESCRIPTION::" << description << std::endl;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* wind = (Window *) glfwGetWindowUserPointer(window);
	wind->m_ReleasedKeys[key] = ((action == GLFW_RELEASE) && wind->m_HeldKeys[key]);
    wind->m_HeldKeys[key] = action != GLFW_RELEASE;
    if(!wind->m_PressedKeys[key])
        wind->m_PressedKeys[key] = (action == GLFW_PRESS);


    // Window closing
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout << "Escape Key Pressed..." << "Quiting..." <<'\n';
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void window_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, (float)width, (float)height);
    Window* wind = (Window *) glfwGetWindowUserPointer(window);
    wind->m_Width = (float)width;
    wind->m_Height = (float)height;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Window* wind = (Window *) glfwGetWindowUserPointer(window);
	wind->m_ReleasedMouseButtons[button] = ((action == GLFW_RELEASE) && wind->m_ReleasedMouseButtons[button]);
    wind->m_HeldMouseButtons[button] = action != GLFW_RELEASE;
	if (!wind->m_PressedMouseButtons[button])
	    wind->m_PressedMouseButtons[button] = action == GLFW_PRESS;
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window* wind = (Window *) glfwGetWindowUserPointer(window);
    wind->m_MouseX = xpos;
    wind->m_MouseY = ypos;

    if (wind->firstMouse)
    {
        wind->lastMouseX = xpos;
        wind->lastMouseY = ypos;
        wind->firstMouse = false;
    }
    wind->deltaMouseX = xpos - wind->lastMouseX;
    wind->deltaMouseY = wind->lastMouseY - ypos;

    wind->lastMouseX = xpos;
    wind->lastMouseY = ypos;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Window* wind = (Window*)glfwGetWindowUserPointer(window);
    wind->fov -= yoffset * 0.5f;
}