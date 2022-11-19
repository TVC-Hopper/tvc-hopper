# TVC Hopper Flight Software

Initially designed to run on i.MX RTxxxx systems.
The project integrates well with MCUXpresso Config Tools to auto-generate low level configurations for peripherals and clocks (see `bsp`)
Being based on standard MCUXpresso IDE projects but reorganized and built with CMake enables easy integration with MCUXpresso tools
but also with custom tools, build needs, flashing, and testing.

The repository has several major components:
- Flight software
- Telemetry and property server
- MATLAB telemetry and property interface
- Tools, tests, and support software

## Building

### Dependencies

- `cmake` (>=3.17)
    Windows WSL users:
    ```
    $ sudo apt-get update
    $ sudo apt-get install apt-transport-https ca-certificates gnupg software-properties-common wget
    $ wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
    $ sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
    $ sudo apt-get update
    $ sudo apt-get install cmake
    ```
- `arm-none-eabi-gcc` (>=10.3.1)
- MCUXpresso Config Tools v12
- MCUXpresso IDE
- `catch2` (v3)
    ```
    $ git clone https://github.com/catchorg/Catch2.git
    $ cd Catch2
    $ cmake -Bbuild -H. -DBUILD_TESTING=OFF
    $ sudo cmake --build build/ --target install
    ```
- `ninja` (`sudo apt install ninja-build` or `pip install ninja`)

For Windows users, it is recommended to set up WSL for this toolchain.

### Programming and Debugging 

#### Debugging

To debug, use MCUXpresso IDE to open the project located at `support/tvc_ide_build` and use the `tvc_debug` target.
This target should be configured with the relative path to `build-debug-fsnor/hopper.elf` if the project is cloned using git.
Build the project using the regular command line interface (`make flexspi_nor_debug`) to generate `hopper.elf`.
Debug as normal using the IDE's GDB interface.

A standard CMSIS-DAP probe like the MCU Link 2 can be used to debug and program the development board and flight controller via JTAG.

When connecting the development board, MCUXpresso sometimes has issues connecting if the board is connected before
the IDE is launched. For a most reproducible and consistent environment, launch the IDE first.

#### Programming the Flight Controller

The flight controller uses the same JTAG/SWD interface found on the MIMXRT1011 EVK. A CMSIS-DAP probe like the MCU Link 2
can be used to program and debug.

### Build targets

The build supports 3 different targets:

- Debug: loaded to RAM, with debug symbols
- Release: loaded to RAM, no debug symbols
- FlexSPI NOR flash debug: loaded to external flash memory, with debug symbols
- FlexSPI NOR flash release: loaded to external flash memory, no debug symols
- Tests: build and run tests


### `make` targets

- `clean`: remove all build artifacts
- `debug`: debug build
- `release`: release build
- `flexspi_nor_debug`: debug build for flexspi nor flash
- `flexspi_nor_release`: release build for flexspi nor flash
- `test`: build and run unit tests
- `generate_spp_headers`: regenerate or update SPP property lists in various directories
- `telemetry_server`: build telemetry server
- `telemetry_emulator`: build telemetry emulator for testing
- `start_telemetry_server`: starts telemetry server
- `start_telemetry_emulator`: starts telemetry emulator (start from different terminal)


## Design and Architecture

### Board Support Pacakges (BSPs)

The BSPs are generated using the MCUXpresso Config tools. The configuration tools allows us to easily make changes
to clock, pin, and peripheral configurations between the development board and flight controlers. The configuration
file `*.mex` defines the BSP configuration and the configuration tool auto-generates the BSP source code.

BSPs are selected in the project's root `CMakeLists.txt` file options and are linked statically to the executable.

### FSLHAL

The Freescale Semiconductor HAL libraries are used and statically linked to the executable.

### Drivers

Two drivers are provided.
- TFLuna LiDAR
- BNO085 IMU

### Sensor Hub

Sensor Hub is a library provided by CEVA to interface with the BNO085 IMU. The BNO085 driver uses this library to
interface with the sensor fusion hub in the IMU.


### Communication and Data Handling

#### SPP - Simple Property Protocol

Key-value pair protocol taken from a different project. The client (the hopper) has a property list (names, ids, types, size, etc.) that
it can send to a host upon connection. Once connected, the host makes read and write requests to get and set property values.

Its interface is simple containing only an initialization function, a function to parse incoming messages, and a callback used to send messages.

Additionally, it uses two callbacks to get and set property values. These are simple and take a property id, a byte buffer, and size.
The callbacks use the property id to call a specific module's getter or setter for the value.

In practice, the callback to send a message simply calls the STCP write function to handle packet framing.

Property list generation is handled using an automated python script. The property list is defined in a TOML file in the support directory
and then the python script (`support/generate_spp_headers.py`) generates a pair of header and source files with the property list.

#### STCP - Simple Transport Control Protocol

Packet framing protocol taken from a different project. Takes a packet, adds a header, footer, and checksum. Escapes all header/footer characters
inside the packet body.

STCP provides an even simpler interface than SPP with just a function to handle incoming messages, a function to frame packets, and a callback to send messages.

### Callbacks

In order to separate application level logic from the hardware and the operating system, layers interact with each other
using callbacks. For example, the file `src/app_hal_xconnect.c` contains the definitions of several callbacks which
connect the HAL functions to write to Xbee over UART with SPP. Additionally, the file will contain the interrupt definitions
to connect the incoming data with the appropriate packet parser.

## License

Some parts under MIT, some under GPLv3.

## Testing

Unit tests are run using Catch2, a standard C/C++ unit test framework.
The unit tests are focused on only testing the drivers and application code.

All tests are located in `tests` directory

## Telemetry and Property Management

A goal of this project was to create a software architecture that supports easy development and testing during the project's lifecycle.
By exploring the needs of data and commands moving through the system, we decided to use the simple distributed key-value pair protocol SPP.
The protocol enables data and commands to be moved throughout the system. 

The telemetry and property infrastructure contains 3 major components:
1. SPP client on the flight controller
2. SPP host integrated with a TCP server
3. Clients that read, write, and visualize SPP data

### SPP host

The host is stateless and provides the SPP host access to all properties throguh a set of callbacks to get and set data.
Commands can be sent via a property write. Each property has an id, name, size, read/write flags. The software in which SPP
operates is responsible for managing the data associated with each property.

### Telemetry and Property Server

The server is designed to run on a Raspberry Pi. When the vehicle starts up and broadcasts it's presence, the SPP client
transfers the property list to the host. The TCP server uses the following API to allow users to read and write any 
property on the client.

#### Server API

- `id`: property id (2 bytes)
- `period`: period in milliseconds (4 bytes)
- `value`: property value (size bytes)

***Requests***

- `get/<id:2>`: Request property value from SPP client
- `val/<id:2>`: Request property value from telemetry server
- `str/<id:2><period:4>`: Start stream
- `set/<id:2><value:size>`: Set property value
- `emdat/<id:2><value:size>`: Provide emulated data

***Response***
- Value response: `<id:2><size:1><timestamp:4><value:size>`

### Telemetry Viewer

Start the server using `make start_telemetry_server port=<serial port> mode=s`.
Use MATLAB, switch working directory to `tools/telemetry_viewer`. Run the script to plot incoming data.
To add new data, modify the server and the matlab script to accept and plot new values.

Unless using the telemetry emulator, the order of connections does not matter. The flight software, telemetry server, and telemetry viewer may be started in any sequence.
Additionally, the telemetry server can be restarted without needing to restart the flight software. Same is true for the flight software.

The telemetry server operates an SPP host instance that streams data from the client (either vehicle or emulated). The MATLAB visualization/viewer client connects to the server and reads data to plot.

![Telemetry Viewer plotting data streams](docs/readme-assets/matlab-telemetry.png)


#### Data emulation

Modify `tools/telemetry_emulator/src/main.cpp` to generate whatever telemetry data you'd like, may need to update property
lists.
Open a terminal and run `make telemetry_emulator start_telemetry_emulator`.
If starting the server with the emulator, use `make start_telemetry_server mode=e`

When using the emulator, the telemetry emulator must be started before the MATLAB viewer is launched.



