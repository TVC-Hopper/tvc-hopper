all: clean build test

BUILD_DIR=build-release
DEBUG_DIR=build-debug
TEST_BUILD_DIR=build-test

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(DEBUG_DIR)
	rm -rf $(TEST_BUILD_DIR)

.PHONY: cmakeclean
cmakeclean:
	$(MAKE) -C$(BUILD_DIR) clean
	$(MAKE) -C$(TEST_BUILD_DIR) clean

.PHONY: release
release:
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=release -B$(BUILD_DIR)
	$(MAKE) -C$(BUILD_DIR)

.PHONY: debug
debug:
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=debug -B$(DEBUG_DIR)
	$(MAKE) -C$(DEBUG_DIR)

.PHONY: test
test:
	cmake -B$(TEST_BUILD_DIR) -Stests
	$(MAKE) -C$(TEST_BUILD_DIR)
	./${TEST_BUILD_DIR}/tests
