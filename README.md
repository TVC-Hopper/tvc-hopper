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
    ```
    $ git clone https://github.com/catchorg/Catch2.git
    $ cd Catch2
    $ cmake -Bbuild -H. -DBUILD_TESTING=OFF
    $ sudo cmake --build build/ --target install
    ```
- `ninja` (`sudo apt install ninja-build`)

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
make release 

# flexspi nor debug build
make flexspi_nor_debug

# flexspi nor release build
make flexspi_nor_release

# build, run tests
make test
```

## Design and Architecture

### SPP - Simple Property Protocol

Key-value pair protocol taken from a different project. The client (the hopper) has a property list (names, ids, types, size, etc.) that
it can send to a host upon connection. Once connected, the host makes read and write requests to get and set property values.

Its interface is simple, an initialization function, a function to parse incoming messages, and a callback used to send messages.

Additionally, it uses two callbacks to get and set property values. These are simple and take a property id, a byte buffer, and size.
The callbacks use the property id to call a specific module's getter or setter for the value.

In practice, the callback to send a message simply calls the STCP write function to handle packet framing.

Property list generation is handled using an automated python script. The property list is defined in a TOML file in the support directory
and then the python script generates a pair of header and source files with the property list.

### STCP - Simple Transport Control Protocol

Packet framing protocol taken from a different project. Takes a packet, adds a header, footer, and checksum. Escapes all header/footer characters
inside the packet body.

An even simpler interface with just a function to handle incoming messages, a function to frame packets, and a callback to send messages.

### Circbuf - Circular Buffer

Library for a circular buffer. There's some work to be done here for making it more generic and usable but it'll work for now.

### Callbacks

In order to separate application level logic from the hardware and the operating system, layers interact with each other
using callbacks. For example, the file `src/app_hal_xconnect.c` contains the definitions of several callbacks which
connect the HAL functions to write to Xbee over UART with SPP. Additionally, the file will contain the interrupt definitions
to connect the incoming data with the appropriate packet parser.

## License

TBD

## Testing

Unit tests are run using Catch2, a standard C/C++ unit test framework.
The unit tests are focused on only testing the drivers and application code.

All tests are located in `tests` directory



