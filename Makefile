all: clean build test

BUILD_DIR=build-release
DEBUG_DIR=build-debug
BUILD_FSNOR_DIR=build-release-fsnor
DEBUG_FSNOR_DIR=build-debug-fsnor
TEST_BUILD_DIR=build-test
BUILD_TELEM_SERVER_DIR=build-telem

.PHONY:
generate_spp_headers:
	python tools/generate_spp_lists.py support/prop_list.toml src/modules/spp_property_list tools/telemetry_viewer/include/telemetry/spp_property_list


.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BUILD_FSNOR_DIR)
	rm -rf $(DEBUG_FSNOR_DIR)
	rm -rf $(DEBUG_DIR)
	rm -rf $(TEST_BUILD_DIR)
	rm -rf $(BUILD_TELEM_SERVER_DIR)

.PHONY: release
release: generate_spp_headers
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=release -B$(BUILD_DIR) -GNinja
	ninja -C$(BUILD_DIR)

debug: generate_spp_headers
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=debug -B$(DEBUG_DIR) -GNinja
	ninja -C$(DEBUG_DIR)

.PHONY: flexspi_nor_debug
flexspi_nor_debug: generate_spp_headers
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=flexspi_nor_debug -B$(DEBUG_FSNOR_DIR) -GNinja
	ninja -C$(DEBUG_FSNOR_DIR)

.PHONY: flexspi_nor_release
flexspi_nor_release: generate_spp_headers
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=flexspi_nor_release -B$(BUILD_FSNOR_DIR) -GNinja
	ninja -C$(BUILD_FSNOR_DIR)

.PHONY: telemetry_server
telemetry_server:
	cmake -B$(BUILD_TELEM_SERVER_DIR) -Stools/telemetry_viewer -GNinja
	ninja -C$(BUILD_TELEM_SERVER_DIR)

.PHONY: start_telemetry_server
start_telemetry_server:
	./$(BUILD_TELEM_SERVER_DIR)/telemetry_server


.PHONY: test
test:
	cmake -B$(TEST_BUILD_DIR) -Stests -GNinja
	ninja -C$(TEST_BUILD_DIR)
	./${TEST_BUILD_DIR}/tests
