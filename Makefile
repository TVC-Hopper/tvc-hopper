all: clean build test

BUILD_DIR=build
TEST_BUILD_DIR=build-test

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(TEST_BUILD_DIR)

.PHONY: cmakeclean
cmakeclean:
	$(MAKE) -C$(BUILD_DIR) clean
	$(MAKE) -C$(TEST_BUILD_DIR) clean

.PHONY: build
build:
	cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -B$(BUILD_DIR)
	$(MAKE) -C$(BUILD_DIR)

.PHONY: test
test:
	cmake -B$(TEST_BUILD_DIR) -Stests
	$(MAKE) -C$(TEST_BUILD_DIR)
	./${TEST_BUILD_DIR}/tests
