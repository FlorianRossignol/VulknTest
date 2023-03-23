#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif



class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:



    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan() {

        createInstance();
        setupDebugMessenger();
    }

    void setupDebugMessenger()
    {
	    if (!enableValidationLayers)
	    {
		  return;  
	    }
    }

    void mainLoop() {
	    while (!glfwWindowShouldClose(m_window))
	    {
            glfwPollEvents();
	    }

    }

    void cleanup() {

        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(m_window);

        glfwTerminate();
    }

    void createInstance()
    {
	    if (enableValidationLayers && !checkValidationLayersSupport())
	    {
            throw std::runtime_error("Les validations layers sont activées mais ne sont pas disponible!");
	    }

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

	    if (enableValidationLayers)
	    {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledExtensionNames = validationLayers.data();
	    }
	    else
	    {
            createInfo.enabledLayerCount = 0;
	    }

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (vkCreateInstance(&createInfo,nullptr,&instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Echec de la création de l'instance !");
        }

    }

    bool checkValidationLayersSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers)
            {
	            if (strcmp(layerName,layerProperties.layerName) == 0)
	            {
                    layerFound = true;
                    break;
	            }
            }

        	if (!layerFound)
            {
                return false;
            }
        }

    	return true;
    }

    std::vector<const char*> getRequiredExtensions()
    {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers)
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT pcallbackData,
        void* pUserData)
    {
        std::cerr << "validation layer:" << pcallbackData.pMessage << std::endl;

        return VK_FALSE;
    }

    GLFWwindow* m_window;

    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    VkInstance instance;

    uint32_t extensionCount = 0;

    //validation layers
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    VkDebugUtilsMessengerEXT callback;

};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}