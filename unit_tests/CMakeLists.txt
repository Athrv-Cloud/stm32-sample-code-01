project(unit_tests CXX)

# Find CppUTest components
find_path(CPPUTEST_INCLUDE_DIR 
    NAMES CppUTest/TestHarness.h
    PATHS /usr/include
    REQUIRED
)

find_library(CPPUTEST_LIBRARY
    NAMES CppUTest
    PATHS /usr/lib/x86_64-linux-gnu
    REQUIRED
)

find_library(CPPUTESTEXT_LIBRARY
    NAMES CppUTestExt
    PATHS /usr/lib/x86_64-linux-gnu
    REQUIRED
)

# Test source files
file(GLOB TEST_SOURCES src/*.cpp)

# Create test executable
add_executable(TestRunner ${TEST_SOURCES})

target_include_directories(TestRunner PRIVATE
    ${CPPUTEST_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Core/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
)

target_link_libraries(TestRunner PRIVATE
    ${CPPUTEST_LIBRARY}
    ${CPPUTESTEXT_LIBRARY}
)

# Add compile definitions for testing
target_compile_definitions(TestRunner PRIVATE UNIT_TESTING)