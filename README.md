# TPS Project

## Overview
This is a Third-Person Shooter (TPS) project developed using Unreal Engine. The project includes a customizable character class (`ATPSCharacter`), inventory management, and camera controls, with Doxygen-generated documentation for code reference.

## Prerequisites
- **Unreal Engine**: Version 5.x or later
- **Doxygen**: Version 1.9.x for generating documentation
- **C++ Compiler**: Compatible with Unreal Engine (e.g., Visual Studio 2022)
- **OS**: Windows 10/11

## Setup
1. **Clone the Repository**:
   ```bash
   git clone <repository-url>
   ```

2. **Open in Unreal Engine**:
    - Open the `.uproject` file in Unreal Engine.
    - Ensure all dependencies are resolved via the Unreal Editor.

3. **Generate Documentation**:
    - Navigate to `D:\Projects\Unreal Engine\TPS\devops\docs`.
    - Run the Doxygen configuration script:
      ```batch
      config_doxygen.bat
      ```
    - Edit `Doxyfile` to include private members (set `EXTRACT_PRIVATE = YES` and `EXTRACT_ALL = YES`).
    - Generate documentation:
      ```batch
      doxygen "D:\Projects\Unreal Engine\TPS\devops\docs\Doxyfile"
      ```

## Project Structure
- **Source Code**: Located in `Source/TPS/`.
    - `TPSCharacter.h`/`TPSCharacter.cpp`: Core character class with movement, camera, and health mechanics.
    - `TPSInventoryComponent`: Manages player inventory.
- **Documentation**: Generated in `devops/docs/html/` after running Doxygen.
- **DevOps Scripts**: Located in `devops/docs/` (e.g., `config_doxygen.bat`).

## Running the Project
1. Open the project in Unreal Engine.
2. Set the default map and game mode in the Unreal Editor.
3. Press **Play** to test the game in the editor or build the project for standalone execution.

## Documentation
- **View Documentation**: Open `devops/docs/html/index.html` in a browser after generating with Doxygen.
- **Customization**: Modify `Doxyfile` for light/dark theme or additional settings (e.g., `HTML_COLORSTYLE = LIGHT`).

## Notes
- Ensure paths with spaces (e.g., `Unreal Engine`) are enclosed in quotes in scripts to avoid issues.
- For older Doxygen styles, use a custom `doxygen.css` or revert to Doxygen 1.8.16.

## Contributing
- Fork the repository.
- Create a feature branch (`git checkout -b feature-name`).
- Submit a pull request with detailed changes.

## License
Copyright Epic Games, Inc. All Rights Reserved. See `LICENSE` for details.