cmake_minimum_required(VERSION 3.0...3.25)
project(
    LearnWebGPU # name of the project, which will also be the name of the visual studio solution if you use it
    VERSION 0.1.0 # any version number
    LANGUAGES CXX C # programming languages used by the project
)


file(GLOB_RECURSE app_files_src "${CMAKE_SOURCE_DIR}/src/*.hpp" "${CMAKE_SOURCE_DIR}/src/*.cpp")

# add_executable(App src/main.cpp)
add_executable(App ${app_files_src})

# target_include_directories(App PRIVATE "${CMAKE_SOURCE_DIR}/src")

# Include more files from our project
target_sources(App PRIVATE ${app_files_src})

set_target_properties(App PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
)

# Include webgpu directory, to define the 'webgpu' target
add_subdirectory(webgpu)

# Include glfw directory
add_subdirectory(glfw3webgpu)

# Add extra libs to target as a dependency of our App
target_link_libraries(App PRIVATE glfw webgpu glfw3webgpu)

# Include other directories
# target_include_directories(App PRIVATE glfw3webgpu/include)

# The application's binary must find wgpu.dll or libwgpu.so at runtime,
# so we automatically copy it (it's called WGPU_RUNTIME_LIB in general)
# next to the binary.
target_copy_webgpu_binaries(App)


# if (MSVC)
# 	target_compile_options(App PRIVATE /W4)
# else()
# 	target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
# endif()

if (XCODE)
	set_target_properties(App PROPERTIES
		XCODE_GENERATE_SCHEME ON
		XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal"
	)
endif()
