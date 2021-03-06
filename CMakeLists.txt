cmake_minimum_required(VERSION 2.8)

project(HeroKart)

find_package(OpenGL REQUIRED)
find_package(SDL REQUIRED)
find_package(Freetype REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_ttf REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${SDLTTF_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR} third-party/glm)

set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_ASSIMP_TOOLS OFF)

include_directories(third-party/glew-1.10.0/include)
add_definitions(-DGLEW_STATIC=1)
add_subdirectory(third-party/glew-1.10.0)

set(CMAKE_CXX_FLAGS "-std=gnu++0x")

# set(EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE})

# Glimac
include_directories(lib/include)
add_subdirectory(lib)

# Assimp
include_directories(third-party/assimp/include)
add_subdirectory(third-party/assimp)

#SOIL
include_directories(third-party/soil)

#Freetype
include_directories(third-party/freetype/include)
include_directories(third-party/freetype/include/freetype2)

set(ALL_LIBRARIES glew ${OPENGL_LIBRARIES} ${SDLTTF_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL_LIBRARY} glimac)

file(GLOB_RECURSE source_files src/*)
add_executable(HeroKart ${source_files})
target_link_libraries(HeroKart assimp SOIL ${SDLTTF_LIBRARY} ${ALL_LIBRARIES} freetype)