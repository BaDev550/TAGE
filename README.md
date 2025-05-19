# TAGE Engine

![Screenshot 2025-05-19 163115](https://github.com/user-attachments/assets/5477c388-aef2-4a22-9043-5a09b2c9fd64)

**TAGE** is a modular, lightweight, and efficient game engine developed in C++ for Windows platforms. Designed with simplicity and flexibility in mind, TAGE provides a solid foundation for developing 2D and 3D games, making it an excellent choice for indie developers and hobbyists.

## Features

* **Cross-Platform Architecture**: While currently tailored for Windows, the engine's modular design facilitates potential cross-platform support in the future.
* **Modular Design**: Organized into distinct modules such as `Engine`, `Sandbox` for better code management and scalability.
* **Build System**: Utilizes [Premake5](https://premake.github.io/) for generating project files, simplifying the build process.
* **Third-Party Integrations**: Incorporates third-party libraries through the `thirdParty` directory, enhancing functionality without reinventing the wheel.
* **Sample Projects**: Includes a `Sandbox` project to demonstrate engine capabilities and serve as a starting point for new projects.

## Getting Started

### Prerequisites

* **Operating System**: Windows 10 or later
* **Development Environment**: [Visual Studio 2019](https://visualstudio.microsoft.com/) or later
* **Build System**: [Premake5](https://premake.github.io/)

### Building the Engine

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/BaDev550/TAGE.git
   cd TAGE
   ```

2. **Generate Project Files**:

   Run the provided batch file to generate Visual Studio project files:

   ```bash
   Build.bat
   ```

   Alternatively, use Premake5 directly:

   ```bash
   premake5 vs2022
   ```

3. **Open the Solution**:

   Open `TAGE.sln` in Visual Studio.

4. **Build the Solution**:

   Build the solution in Visual Studio to compile the engine and sample projects.

![Screenshot 2025-05-17 194009](https://github.com/user-attachments/assets/6603502d-9481-4b07-acf2-bcbc4bad7d32)

## Directory Structure

* `Engine/`: Core engine source code.
* `Brand/`: Contains branding assets and related resources.
* `Sandbox/`: Sample project demonstrating engine usage.
* `thirdParty/`: External libraries and dependencies.
* `premake5.lua`: Premake5 configuration script.
* `Build.bat`: Batch script to generate project files.
* `TAGE.sln`: Visual Studio solution file.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

Developed by [Baran Keklik](mailto:keklikbaran91@gmail.com)
