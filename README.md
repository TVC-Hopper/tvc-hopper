# TVC Hopper Flight Software

Initially designed to run on i.MX RTxxxx systems.
The project integrates well with MCUXpresso Config Tools to auto-generate low level configurations for peripherals and clocks (see `bsp`)
Being based on standard MCUXpresso IDE projects but reorganized and built with CMake enables easy integration with MCUXpresso tools
but also with custom tools, build needs, flashing, and testing.

## Building

### Dependencies

- `cmake` (>=3.17)
- `arm-none-eabi-gcc` (>=10.3.1)
- MCUXpresso Config Tools v12
- `catch2` (v3)

For Windows users, it is recommended to set up WSL for this toolchain.

### Build targets

The build supports 3 different targets:

- Debug: loaded to RAM, with debug symbols
- Release: loaded to RAM, no debug symbols
- FlexSPI NOR flash debug: loaded to external flash memory, with debug symbols
- FlexSPI NOR flash release: loaded to external flash memory, no debug symols
- Tests: build and run tests

```
make clean

# debug build
make debug

# release build
make build

# flexspi nor debug build
make flexspi_nor_debug

# flexspi nor release build
make flexspi_nor_release

# build, run tests
make test
```



## License

TBD

## Testing

Unit tests are run using Catch2, a standard C/C++ unit test framework.
The unit tests are focused on only testing the drivers and application code.

All tests are located in `tests` directory



