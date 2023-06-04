#ifndef ENGINE_H
#define ENGINE_H

// -----------------------------------------------------------------------------
// Standard library
// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// -----------------------------------------------------------------------------
// SDL Headers
// -----------------------------------------------------------------------------
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_vulkan.h>

// -----------------------------------------------------------------------------
// OpenGL Header
// -----------------------------------------------------------------------------
#ifdef _WIN32
    #include <windows.h>
    #include <GL/GL.h>
#elif __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl.h>
#endif

// -----------------------------------------------------------------------------
// VulkanAPI Header
// -----------------------------------------------------------------------------
#include <vulkan/vulkan.h>

// -----------------------------------------------------------------------------
// Macros
// -----------------------------------------------------------------------------
#define MAIN_ENGINE_LOOP 1

// -----------------------------------------------------------------------------
// Project headers
// -----------------------------------------------------------------------------
#include "window.h"

#include "render.h"
#include "render_opengl.h"
#include "render_vulkan.h"

#include "map.h"
#include "player.h"

#endif