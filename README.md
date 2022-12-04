# Anwill

Anwill is an early-stage personal project. The primary goal is educational: to implement 
as many components as possible (often from scratch) thereby learning
the ins and outs. The result may serve as a baseline for various
game projects that come to mind.

## Installation

Anwill is being developed in CLion and is therefore compiled with CMake. It is recommended
to use CLion to build and run the project since it automizes the build and run process 
down to a single button, but it's not necessary.

1. Clone the repo.
2. Build G manually with CMake under `Anwill/vendor/glfw/build`

*More detailed and general installation guide coming soon!*

## How does it work?
Checkout [Sandbox](Sandbox/src) for various examples of how to use specific systems.

## Currently working on

* GUI
* Installation guide

## Features

* Event System
* 2D Renderer with Batch Rendering
* (Efficient) Text Rendering
* Sprites and simple Sprite Animations (which supports SpriteSheet loading)
* ECS
* 2D Physics - Dynamics and Collision
* Time profiling
* Custom Precompiler Shader Macros
* Logging
* .. and more! 
  * Keyboard and Mouse input
  * Vector and Matrix library
  * RNG module
  * Very simple FileIO (to and from string)


### Supported platforms

* Windows 64-bit
* OpenGL

## Dependencies

* [FreeType](https://github.com/freetype/freetype)
* [GLAD](https://github.com/Dav1dde/glad)
* [GLFW](https://www.glfw.org/)
* [stb_image](https://github.com/nothings/stb)
* [Willog](https://github.com/WeRiano/Willog)

## Acknowledgements

It would be impossible to cite every resource that I have used to create Anwill but 
here are a few of them.
* [TheCherno](https://www.youtube.com/@TheCherno) 
* [Learn OpenGL](https://learnopengl.com/)
* [Two-Bit Coding](https://www.youtube.com/@two-bitcoding8018)
* ECS articles: [1](https://austinmorlan.com/posts/entity_component_system/), 
[2](https://bitsquid.blogspot.com/2014/08/building-data-oriented-entity-system.html) and 
[3](https://www.gamedeveloper.com/design/the-entity-component-system---an-awesome-game-design-pattern-in-c-part-1-#refs)