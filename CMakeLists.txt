cmake_minimum_required(VERSION 3.16)
project(KeyboardHero)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compiles commands every build
# (gets rid of red error for sfml libraries like: window undeclared)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find SFML (make sure it's installed on your system)
find_package(SFML 3.0 COMPONENTS System Window Graphics Audio REQUIRED)

# create executable
add_executable(KeyboardHero src/main.cpp)

# link header files
# target_include_directories(KeyboardHero PRIVATE include)

# link SFML libraries
target_link_libraries(KeyboardHero PRIVATE
    SFML::Graphics
    SFML::Audio
)

# At the end of yo
add_custom_command(
  TARGET KeyboardHero POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:KeyboardHero>/assets
)

