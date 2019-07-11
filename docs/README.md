# Visualizer

![GitHub](https://img.shields.io/github/license/frodal/Visualizer.svg)
![GitHub top language](https://img.shields.io/github/languages/top/frodal/Visualizer.svg)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/frodal/Visualizer.svg)
![GitHub issues](https://img.shields.io/github/issues-raw/frodal/Visualizer.svg)
![GitHub tag (latest by date)](https://img.shields.io/github/tag-date/frodal/Visualizer.svg)

A visualizer written in C++ using OpenGL

![Test](Test.gif "A Visualizer Test")

## Getting Started

To get a local copy up and running follow these simple steps.

1. Clone the project with all its submodules (`git clone --recursive https://github.com/frodal/Visualizer.git`)
2. Make sure you have [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com) installed with the visual C++ compiler
3. Run the `GenerateProjects.bat` file located in the root directory to generate the Visual Studio solution files with [Premake](https://github.com/premake/premake-core)
4. Open up the generated `Visualizer.sln` file in Visual Studio
5. Build the solution (`Ctrl`+`Shift`+`B`) :sunglasses:

### Dependencies

The project depends on the following libraries and are located in the `./Visualizer/vendor/` directory

* [GLEW](https://github.com/nigels-com/glew)
* [GLFW](https://github.com/glfw/glfw)
* [glm](https://github.com/g-truc/glm)
* [imgui](https://github.com/ocornut/imgui)
* [stb](https://github.com/nothings/stb)

(Make sure to initialize and update the git submodules)

## Contributing

To contribute:

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

Distributed under the [MIT License](https://mit-license.org/).
See `LICENSE.md` for more information.

See also the License of each dependency.

## Contact

Bjørn Håkon Frodal - [@frodal](https://github.com/frodal) - bjorn.h.frodal@ntnu.no

Project Link: [https://github.com/frodal/Visualizer](https://github.com/frodal/Visualizer)
