cmake_minimum_required(VERSION 3.16)
project(marquee_console VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(marquee_app
  src/main.cpp
  src/Marquee.cpp
  src/CommandHandler.cpp
  src/AsciiArt.cpp
)
target_include_directories(marquee_app PRIVATE include)
if(MSVC)
  target_compile_options(marquee_app PRIVATE /W4)
else()
  target_compile_options(marquee_app PRIVATE -Wall -Wextra)
endif()
find_package(Threads REQUIRED)
target_link_libraries(marquee_app PRIVATE Threads::Threads)

# Build + run in one step: cmake --build build --target run
add_custom_target(run
  COMMAND marquee_app
  DEPENDS marquee_app
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  USES_TERMINAL
  COMMENT "Build and run marquee_app"
)
