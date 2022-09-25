#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>

using namespace Catch;

#include <drivers/imu.h>


#define ACCEL_ADDRESS   0xAB
#define MAG_ADDRESS     0xCD
#define GYRO_ADDRESS    0xEF
#define TEMP_ADDRESS    0xEF

uint16_t last_write_address;
uint8_t last_write_bus_data[1024];
uint16_t last_write_size;

uint16_t last_read_address;
uint8_t last_read_bus_data[1024];
uint16_t last_read_size;

uint16_t response_data;

void IMU_I2CWriteCallback(uint16_t addr, uint8_t *bus_data, uint16_t size) {
    last_write_address = addr;
    memcpy(last_write_bus_data, bus_data, size);
    last_write_size = size;
}

uint8_t IMU_I2CReadCallback(uint16_t addr, uint8_t *bus_data, uint16_t size) {
    last_read_address = addr;
    memcpy(last_read_bus_data, bus_data, size);
    last_read_size = size;

    return response_data;
}



IMUCallbacks_t callbacks = {
    .onWrite = IMU_I2CWriteCallback,
    .onRead = IMU_I2CReadCallback,
};

IMU_t imu;


void setup() {
    IMU_Init(&imu);
}

TEST_CASE( "IMU Init" ) {
    setup();

    REQUIRE(imu.callbacks.onRead == IMU_I2CReadCallback);
    REQUIRE(imu.callbacks.onWrite == IMU_I2CWriteCallback);
    REQUIRE(imu.accel.addr == ACCEL_ADDRESS);
    REQUIRE(imu.gyro.addr == GYRO_ADDRESS);
    REQUIRE(imu.mag.addr == MAG_ADDRESS);
    REQUIRE(imu.tsense.addr == TEMP_ADDRESS);
}

TEST_CASE( "Accelerometer" ) {
    setup();


}

TEST_CASE( "Gyroscope" ) {
    setup();
}

TEST_CASE( "Magnetometer" ) {
    setup();
}

TEST_CASE( "Temperature" ) {
}
