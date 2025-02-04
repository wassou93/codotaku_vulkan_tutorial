#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define PANIC( ERROR, FORMAT, ... )                                                                                              \
    {                                                                                                                            \
        if ( ERROR )                                                                                                             \
        {                                                                                                                        \
            fprintf( stderr,                                                                                                     \
                "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n", __FILE_NAME__,  __FUNCTION__, __LINE__, ERROR,  ##__VA_ARGS__ ); \
            raise( SIGABRT );                                                                                                    \
        }                                                                                                                        \
    }

typedef struct
{
    char* windowTitle;
    int windowWidth, windowHeight;
    bool windowResizable;
    bool windowFullscreen;
    uint32_t apiVersion;

    GLFWmonitor* windowMonitor;
    GLFWwindow* window;

    VkAllocationCallbacks* allocator;
    VkInstance instance;
} State;

void glfwErrorCallback( int error_code, const char* description )
{
    PANIC( error_code, "GLFW: %s", description );
}

void exitCallback()
{
    glfwTerminate();
}

void setupErrorHandling( void )
{
    glfwSetErrorCallback( glfwErrorCallback );
    atexit( exitCallback );
}

void createWindow( State* state )
{
    glfwInit();
    glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
    glfwWindowHint( GLFW_RESIZABLE, state->windowResizable );
    if ( state->windowFullscreen )
    {
        state->windowMonitor = glfwGetPrimaryMonitor();
    }
    state->window = glfwCreateWindow( state->windowWidth, state->windowHeight, state->windowTitle, state->windowMonitor, NULL );
}

void createInstance( State* state )
{
    uint32_t extensionsCount;
    const char** requiredExtensions = glfwGetRequiredInstanceExtensions( &extensionsCount );

    VkApplicationInfo applicationInfo = {
        .sType      = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .apiVersion = state->apiVersion,
    };

    VkInstanceCreateInfo createInfo = {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo        = &applicationInfo,
        .enabledExtensionCount   = extensionsCount,
        .ppEnabledExtensionNames = requiredExtensions,
    };

    PANIC( vkCreateInstance( &createInfo, state->allocator, &state->instance ), "Error creating Vulkan instance." );
}

void logVersionInfo()
{
    uint32_t instanceApiVersion;
    vkEnumerateInstanceVersion( &instanceApiVersion );
    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT( instanceApiVersion );
    uint32_t apiVersionMajor   = VK_API_VERSION_MAJOR( instanceApiVersion );
    uint32_t apiVersionMinor   = VK_API_VERSION_MINOR( instanceApiVersion );
    uint32_t apiVersionPatch   = VK_API_VERSION_PATCH( instanceApiVersion );
    printf( "Vulkan API %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch );
    printf( "%s\n", glfwGetVersionString() );
}

void init( State* state )
{
    setupErrorHandling();
    logVersionInfo();
    createWindow( state );
    createInstance( state );
}

void loop( State* state )
{
    while ( !glfwWindowShouldClose( state->window ) )
    {
        glfwPollEvents();
    }
}
void cleanup( State* state )
{
    vkDestroyInstance( state->instance, state->allocator );
    glfwDestroyWindow( state->window );
}

int main()
{
    State state = {
        .windowTitle      = "CODOTAKU",
        .windowWidth      = 720,
        .windowHeight     = 480,
        .windowResizable  = false,
        .windowFullscreen = false,
        .apiVersion       = VK_API_VERSION_1_3,
    };

    init( &state );
    loop( &state );
    cleanup( &state );

    return EXIT_SUCCESS;
}
