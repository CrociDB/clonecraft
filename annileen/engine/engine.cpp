#include <engine/engine.h>
#include <engine/renderer.h>
#include <engine/serviceprovider.h>
#include <engine/core/logger.h>
#include <sstream>

namespace annileen
{
    // Initialize static variables
    bool Engine::m_Running = true;

    void Engine::glfw_errorCallback(int error, const char* description)
    {
        fprintf(stderr, "GLFW error %d: %s\n", error, description);
    }

    void Engine::glfw_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            getInstance()->getInput()->_setKeyDown(key, true);
        }
        else if (action == GLFW_RELEASE)
        {
            getInstance()->getInput()->_setKeyDown(key, false);
        }
    }

    void Engine::glfw_mouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        getInstance()->getInput()->_setMousePosition(xpos, ypos);
    }

    void Engine::glfw_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            getInstance()->getInput()->_setMouseButton(button, true);
        } 
        else if (action == GLFW_RELEASE)
        {
            getInstance()->getInput()->_setMouseButton(button, false);
        }
    }

    void Engine::glfw_mouseCursorEnterCallback(GLFWwindow* window, int entered)
    {
        if (entered)
        {
            // The cursor entered the content area of the window
        }
        else
        {
            // The cursor left the content area of the window
        }
    }

    void Engine::glfw_mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
    }

    void Engine::glfw_joystickCallback(int jid, int event)
    {
        if (event == GLFW_CONNECTED)
        {
            // The joystick was connected
        }
        else if (event == GLFW_DISCONNECTED)
        {
            // The joystick was disconnected
        }
    }


    int Engine::init(int width, int height, std::string assetfile)
    {
        m_Width = width;
        m_Height = height;

        glfwSetErrorCallback(&Engine::glfw_errorCallback);
        if (!glfwInit())
            return 1;
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_Window = glfwCreateWindow(m_Width, m_Height, "Annileen", nullptr, nullptr);
        if (!m_Window)
            return 1;

        glfwSetKeyCallback(m_Window, glfw_keyCallback);
        glfwSetMouseButtonCallback(m_Window, glfw_mouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, glfw_mouseCursorPositionCallback);
        //glfwSetCursorEnterCallback(m_Window, glfw_mouseCursorEnterCallback);
        //glfwSetScrollCallback(m_Window, glfw_mouseScrollCallback);
        //glfwSetJoystickCallback(glwf_joystickCallback);

        bgfx::renderFrame();

        bgfx::Init init;
        init.type = bgfx::RendererType::OpenGL;
    #if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        init.platformData.ndt = glfwGetX11Display();
        init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(m_Window);
    #elif BX_PLATFORM_OSX
        init.platformData.nwh = glfwGetCocoaWindow(m_Window);
    #elif BX_PLATFORM_WINDOWS
        init.platformData.nwh = glfwGetWin32Window(m_Window);
    #endif

        init.resolution.width = m_Width;
        init.resolution.height = m_Height;
        init.resolution.reset = BGFX_RESET_VSYNC;
        if (!bgfx::init(init))
            return 1;

        // Initialize services
        Logger* logger = new Logger();
        ServiceProvider::provideLogger(logger);
        //

        ANNILEEN_LOG(LoggingLevel::Info, LoggingChannel::Core, "This is a message specifying level");
        ANNILEEN_LOG_WARNING(LoggingChannel::General, "This is a warning message");
        ANNILEEN_LOG_ERROR(LoggingChannel::Renderer, "This is a error message");
        ANNILEEN_LOG_INFO(LoggingChannel::Core, "This is a regular message");

        ANNILEEN_LOGF(LoggingLevel::Info, LoggingChannel::Core, "This {0} {1} {2} {3} {4} {5}", "is", "a", "formatted", "message", "specifying", "level");
        ANNILEEN_LOGF_WARNING(LoggingChannel::General, "This is a formatted warning message: {0} {1} {2}", 1, 2.0f, 3.0F);
        ANNILEEN_LOGF_ERROR(LoggingChannel::Renderer, "This is a formatted error message: {0}", "bla");
        ANNILEEN_LOGF_INFO(LoggingChannel::Core, "This is a formatted regular message: {0}", "ble");

        m_Renderer = new Renderer();
        m_Renderer->init(this);

        m_Gui = std::make_shared<Gui>();
        m_Gui->init();

        m_TargetFPS = 60; 
        m_Time.timeScale = 1.0f;

        m_Running = true;
        m_Time.deltaTime = m_Time.unscaledDeltaTime = m_Time.time = 0;

        m_AssetManager = std::make_shared<AssetManager>(assetfile);

        return 0;
    }

    std::shared_ptr<Input> Engine::getInput()
    {
	    return m_Input;
    }

    std::shared_ptr<Gui> Engine::getGui()
    {
        return m_Gui;
    }

    AssetManager* annileen::Engine::getAssetManager()
    {
        return m_AssetManager.get();
    }

    Renderer* Engine::getRenderer()
    {
        return m_Renderer;
    }

    Uniform* Engine::getUniform()
    {
        return &m_Uniform;
    }

    GLFWwindow* Engine::getGLFWWindow()
    {
        return m_Window;
    }

    uint16_t Engine::getWidth() const
    {
        return m_Width;
    }

    uint16_t Engine::getHeight() const
    {
        return m_Height;
    }


    void Engine::setWindowTitle(std::string title)
    {
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    void Engine::setFPSLock(uint8_t fps)
    {
        m_TargetFPS = fps;
    }

    int Engine::getFPS() const
    {
        return 1.0f / m_Time.deltaTime;
    }

    annileen::Time Engine::getTime()
    {
        return m_Time;
    }


    void Engine::setScene(Scene* scene)
    {
        m_CurrentScene = scene;
    }

    bool Engine::run()
    {
        double time = glfwGetTime();
        m_Time.unscaledDeltaTime = time - m_Time.time;
        m_Time.deltaTime = m_Time.unscaledDeltaTime * m_Time.timeScale;
        m_Time.time = time;

        static float fpsCount = 0.0;
        fpsCount += m_Time.deltaTime;
        if (fpsCount >= 0.3f)
        {
            fpsCount = 0;
            std::stringstream ss;
            ss << "Annileen - Framerate: " << getFPS() << " FPS";
            setWindowTitle(ss.str());
        }

        m_Running = m_Running && !glfwWindowShouldClose(m_Window);
        return m_Running;
    }

    void Engine::terminate()
    {
        m_Running = false;
    }

    void Engine::setMouseCapture(bool value)
    {
        if (value)
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Engine::checkInputEvents()
    {
        m_Input->flushEvents();
        glfwPollEvents();
        int oldWidth = m_Width;
        int oldHeight = m_Height;
        glfwGetWindowSize(m_Window, &m_Width, &m_Height);

        if (m_Width != oldWidth || m_Height != oldHeight)
        {
            bgfx::reset(m_Width, m_Height, BGFX_RESET_VSYNC);
            m_Renderer->clear();
        }
    }

    void Engine::renderFrame()
    {
        bool hasValidScene = m_CurrentScene != nullptr;

        std::list<SceneNodePtr> sceneNode = m_CurrentScene->getNodeList();
        
        if (hasValidScene)
        {
            // Setup camera
            auto camera = m_CurrentScene->getCamera();
            bgfx::setViewTransform(0, camera->getViewMatrixFloatArray(), camera->getProjectionMatrixFloatArray());
            bgfx::setViewRect(0, 0, 0, uint16_t(m_Width), uint16_t(m_Height));
            bgfx::touch(0);

            m_Uniform.setVec3Uniform("u_viewPos", camera->transform().position);

            // Setup lights
            for (const auto& light : m_CurrentScene->getLightList())
            {
                if (light->type == LightDirectional)
                {
                    m_Uniform.setVec3Uniform("u_lightDirection", light->transform.getForward());
                    m_Uniform.setVec3Uniform("u_lightColor", light->color);
                    m_Uniform.setFloatUniform("u_lightIntensity", light->intensity);
                }
            }

            // Render scene
            m_Renderer->initFrame(m_CurrentScene);

            for (auto sceneNode : m_CurrentScene->getNodeList())
            {
                if (!sceneNode->hasModel() || !sceneNode->getAcive()) continue;
                m_Renderer->renderSceneNode(m_CurrentScene, sceneNode);
            }

            if (camera->clearType == CameraClearType::CameraClearSkybox)
            {
                bgfx::setViewName(1, "Skybox");
                bgfx::setViewRect(1, 0, 0, uint16_t(m_Width), uint16_t(m_Height));
                bgfx::touch(1);
                bgfx::setViewTransform(1, camera->getViewRotationMatrixFloatArray(), camera->getProjectionMatrixFloatArray());
                m_Renderer->renderSkybox(camera, m_CurrentScene->getSkybox());
            }
        }

        bgfx::frame();
    }

    Engine::Engine()
    {
        m_Input = std::make_shared<Input>();
    }

    Engine* Engine::getInstance()
    {
	    static Engine* instance = nullptr;

	    if (instance == nullptr)
	    {
		    instance = new Engine();
	    }

	    return instance;
    }

    Engine::~Engine()
    {
        Logger* logger = ServiceProvider::getLogger();
        ServiceProvider::provideLogger(nullptr);
        if (logger != nullptr)
        {
            delete logger;
        }

        m_Gui->destroy();
        m_Uniform.destroy();
        bgfx::shutdown();
        glfwTerminate();
    }
}
