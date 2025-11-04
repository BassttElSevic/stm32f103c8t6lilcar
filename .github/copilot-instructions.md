# GitHub Copilot Instructions

This repository contains firmware for an STM32F103C8T6-based small car (小车) with PS2 controller interface.

## Project Overview

- **Hardware**: STM32F103C8T6 microcontroller (ARM Cortex-M3)
- **Framework**: STM32 HAL (Hardware Abstraction Layer)
- **Build System**: CMake (minimum version 3.22)
- **Configuration Tool**: STM32CubeMX (PS2_2.ioc)
- **Languages**: C11 (primary), C++ (for specific modules like ax_ps2)
- **Purpose**: Motor control firmware for a robotic car with PS2 joystick control

## Coding Standards

### General Style
- **Indentation**: 4 spaces (no tabs)
- **Line Length**: Keep lines under 120 characters when possible
- **Braces**: K&R style (opening brace on same line for functions, control structures)
- **Comments**: Use C-style `/* */` for multi-line, `//` for single-line comments

### Naming Conventions
- **Functions**: Use descriptive names with module prefix (e.g., `AX_PS2_Init`, `HAL_GPIO_WritePin`)
- **Variables**: 
  - Local variables: `camelCase` or `snake_case`
  - Global variables: Prefix with module name
  - Constants: `UPPER_CASE_WITH_UNDERSCORES`
- **Types**: Use `TypeDef` suffix for structs (e.g., `JOYSTICK_TypeDef`)
- **Macros**: `UPPER_CASE_WITH_UNDERSCORES`

### C/C++ Specific
- **C Standard**: C11 with extensions enabled
- **Headers**: Always include header guards with format `#ifndef __MODULE_NAME_H`
- **Extern C**: Wrap C++ headers with `extern "C"` blocks for C compatibility
- **Includes**: Group in order: standard library, HAL, application headers
- **Types**: Use fixed-width types (`uint8_t`, `uint16_t`, etc.) from `<stdint.h>`

### STM32 HAL Conventions
- Use HAL library functions (prefix `HAL_`) for peripheral operations
- Initialize peripherals in dedicated functions (e.g., `MX_GPIO_Init()`, `MX_TIM2_Init()`)
- Handle interrupts in `stm32f1xx_it.c`
- Keep hardware configuration in STM32CubeMX and regenerate code when needed
- User code should be placed between `/* USER CODE BEGIN */` and `/* USER CODE END */` markers

## Project Structure

```
.
├── Core/
│   ├── Inc/          # Application headers
│   └── Src/          # Application source files
├── Drivers/          # STM32 HAL and CMSIS drivers (do not modify)
├── cmake/            # CMake configuration files
├── CMakeLists.txt    # Main build configuration
├── PS2_2.ioc         # STM32CubeMX project file
└── startup_stm32f103xb.s  # Startup code
```

## Build and Development

### Building the Project
```bash
# Configure with CMake
cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build
```

### STM32CubeMX Workflow
1. Modify hardware configuration in `PS2_2.ioc` using STM32CubeMX
2. Regenerate code from STM32CubeMX
3. Preserve user code sections (between USER CODE BEGIN/END markers)
4. Update CMakeLists.txt if new source files are added

### Flashing and Debugging
- Use ST-Link or compatible programmer
- Debug configurations are in `cmake-build-debug-stm-32/`

## Architecture Guidelines

### Module Organization
- **ax_ps2**: PS2 controller communication protocol
- **gpio**: GPIO initialization and control
- **tim**: Timer configuration for PWM and delays
- **main**: Application logic and main loop

### Peripheral Usage
- **Timers**: Used for PWM motor control and microsecond delays
- **GPIO**: Motor control pins, PS2 controller interface
- **Interrupts**: Handle time-critical events

### Best Practices
- Keep interrupt handlers short and fast
- Use volatile for variables accessed in ISRs
- Disable interrupts during critical sections if needed
- Be mindful of stack usage (limited RAM on STM32F103)
- Avoid dynamic memory allocation (malloc/free) in embedded context
- Use HAL timeouts to prevent infinite loops
- Test code incrementally on hardware

## Testing and Validation

### Before Committing
- Ensure code compiles without errors or warnings
- Verify STM32CubeMX regeneration doesn't break user code
- Test on actual hardware when possible

### Safety Considerations
- This is embedded firmware for a physical device (car)
- Changes to motor control or timing can affect hardware behavior
- Test motor control changes carefully to prevent damage
- Verify PWM duty cycles are within safe ranges

## Common Tasks

### Adding a New Peripheral
1. Configure in STM32CubeMX (PS2_2.ioc)
2. Regenerate code
3. Add initialization call in `main.c`
4. Create module-specific files in Core/Inc and Core/Src

### Modifying Motor Control
- Update PWM duty cycles via timer compare registers
- Keep duty cycles between safe limits (0-100%)
- Test with low power first, then gradually increase

### PS2 Controller Integration
- Communication handled in `ax_ps2.cpp`
- Use `AX_PS2_ScanKey()` to read controller state
- Map joystick values to motor speeds appropriately

## Known Constraints

- **Flash**: 64KB (STM32F103C8T6)
- **RAM**: 20KB
- **Clock**: Configured in SystemClock_Config()
- **Mixed C/C++**: Ensure proper linkage with extern "C"
- **No Standard Library**: Limited standard library support in embedded environment

## Additional Notes

- Project uses Chinese comments in some places - preserve them when present
- Respect USER CODE sections in STM32CubeMX-generated files
- When in doubt about HAL usage, refer to STM32F1 HAL documentation
- This is a hobbyist/educational project - balance best practices with simplicity
