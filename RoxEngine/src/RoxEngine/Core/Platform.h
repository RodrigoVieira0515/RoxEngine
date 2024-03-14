namespace RoxEngine {
    // Initialize all platform specific code e.g glfw
    class Platform {
    public:
        static void Init();
        static void Shutdown();
    };
}