#ifndef __RENDERING_ENGINE__
#define __RENDERING_ENGINE__

#include "neopixel.hpp"

class RendringEngine
{
  public:
    RendringEngine();

    void InitRendering(uint8_t pattern_index);
    void Render();
};

#define LPRendringEngine RendringEngine*

#endif