cmake_minimum_required(VERSION 3.21)
project(KeyboardHero)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compiles commands every build
# (gets rid of red error for sfml libraries like: window undeclared)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find SFML (make sure it's installed on your system)
find_package(SFML REQUIRED COMPONENTS System Window Graphics Audio REQUIRED)

# create executable
if(APPLE)
  add_executable(KeyboardHero MACOSX_BUNDLE src/main.cpp)
else()
  add_executable(KeyboardHero src/main.cpp)
endif()

# link header files
# target_include_directories(KeyboardHero PRIVATE include)

# link SFML libraries
target_link_libraries(KeyboardHero PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
)

# Post Build - copy the assets folder
add_custom_command(
  TARGET KeyboardHero POST_BUILD
  COMMENT "Copying assets to /build directory"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:KeyboardHero>/assets
)

# Install in dist folder
install(TARGETS KeyboardHero
  RUNTIME DESTINATION .
  BUNDLE DESTINATION .
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
  DESTINATION .
)

