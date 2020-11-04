#include "Window.h"

void GLAPIENTRY glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    printf("\n GL Message: %s\n", message);
}

void glfwErrorCallback(int error, const char* description)
{
    printf("\n GLFW Error: %s\n", description);
}

Window::Window(int width, int height)
    :
    width(width),
    height(height)
{
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  

    window = glfwCreateWindow(width, height, "Fairy Engine", NULL, NULL); // glfwGetPrimaryMonitor()
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);


    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1); // Enable v-sync

    if (glewInit() != GLEW_OK)
        glfwTerminate();

    std::cout << "OpenGL version " << glGetString(GL_VERSION) << std::endl;

    // Error Handling
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glErrorCallback, NULL);

    // Enable Depth Buffer (z-buffer)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // Enable Antialisaing
    glEnable(GL_MULTISAMPLE);

    // Imgui Setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiStyle& styles = ImGui::GetStyle();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Docking
    io.Fonts->AddFontFromFileTTF("res/fonts/Roboto-Regular.ttf", 20.0f);

    styles.FramePadding = ImVec2(15.0f, 8.0f);
    styles.WindowPadding = ImVec2(10.0f, 10.0f);
    styles.AntiAliasedLines = false;

    styles.TabRounding = 0.0f;
    styles.WindowRounding = 0.0f;
    styles.ScrollbarSize = 20.0f;
    styles.FrameBorderSize = 1.0f;
    styles.WindowBorderSize = 0.0f;
    styles.ScrollbarRounding = 0.0f;

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void Window::StartFrame()
{
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

GLFWwindow* Window::Get()
{
	return window;
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}