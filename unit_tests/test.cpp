#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "CppUTest/CommandLineTestRunner.h"

// Mock implementations of HAL functions
extern "C" {
    HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
        return (HAL_StatusTypeDef)mock().actualCall("HAL_I2C_Master_Transmit")
                                    .withPointerParameter("hi2c", hi2c)
                                    .withUnsignedIntParameter("DevAddress", DevAddress)
                                    .withMemoryBufferParameter("pData", pData, Size)
                                    .withUnsignedIntParameter("Size", Size)
                                    .withUnsignedIntParameter("Timeout", Timeout)
                                    .returnIntValue();
    }

    HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
        return (HAL_StatusTypeDef)mock().actualCall("HAL_UART_Transmit")
                                    .withPointerParameter("huart", huart)
                                    .withMemoryBufferParameter("pData", pData, Size)
                                    .withUnsignedIntParameter("Timeout", Timeout)
                                    .returnIntValue();
    }

    HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
        return (HAL_StatusTypeDef)mock().actualCall("HAL_SPI_Transmit")
                                    .withPointerParameter("hspi", hspi)
                                    .withMemoryBufferParameter("pData", pData, Size)
                                    .withUnsignedIntParameter("Timeout", Timeout)
                                    .returnIntValue();
    }

    void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
        mock().actualCall("HAL_GPIO_TogglePin")
              .withPointerParameter("GPIOx", GPIOx)
              .withUnsignedIntParameter("GPIO_Pin", GPIO_Pin);
    }

    void HAL_Delay(uint32_t Delay) {
        mock().actualCall("HAL_Delay").withUnsignedIntParameter("Delay", Delay);
    }
}

// Test group setup
TEST_GROUP(MainTests) {
    void setup() {
        mock().clear();
    }

    void teardown() {
        mock().checkExpectations();
        mock().clear();
    }
};

// Test cases
TEST(MainTests, TestI2C_Write_Interrupt) {
    uint8_t data[] = {0xA1, 0xB2, 0xC3};

    mock().expectOneCall("HAL_I2C_Master_Transmit_IT")
          .withPointerParameter("hi2c", &hi2c1)
          .withUnsignedIntParameter("DevAddress", SLAVE_ADDRESS)
          .withMemoryBufferParameter("pData", data, sizeof(data))
          .andReturnValue(HAL_OK);

    I2C_Write_Interrupt(SLAVE_ADDRESS, data, sizeof(data));
}

TEST(MainTests, TestI2C_Read_DMA) {
    uint8_t buffer[5];

    mock().expectOneCall("HAL_I2C_Master_Receive_DMA")
          .withPointerParameter("hi2c", &hi2c1)
          .withUnsignedIntParameter("DevAddress", SLAVE_ADDRESS)
          .withMemoryBufferParameter("pData", buffer, sizeof(buffer))
          .andReturnValue(HAL_OK);

    I2C_Read_DMA(SLAVE_ADDRESS, buffer, sizeof(buffer));
}

TEST(MainTests, TestGPIO_Toggle) {
    mock().expectOneCall("HAL_GPIO_TogglePin")
          .withPointerParameter("GPIOx", GPIOA)
          .withUnsignedIntParameter("GPIO_Pin", LD2_Pin);

    HAL_GPIO_TogglePin(GPIOA, LD2_Pin);
}

TEST(MainTests, TestUART_Transmit) {
    const char message[] = "Hello USART";

    mock().expectOneCall("HAL_UART_Transmit")
          .withPointerParameter("huart", &huart2)
          .withMemoryBufferParameter("pData", message, sizeof(message) - 1)
          .withUnsignedIntParameter("Timeout", HAL_MAX_DELAY)
          .andReturnValue(HAL_OK);

    HAL_UART_Transmit(&huart2, (uint8_t *)message, sizeof(message) - 1, HAL_MAX_DELAY);
}

TEST(MainTests, TestSPI_Transmit) {
    uint8_t spiData[] = {0xAA, 0xBB, 0xCC};

    mock().expectOneCall("HAL_SPI_Transmit")
          .withPointerParameter("hspi", &hspi2)
          .withMemoryBufferParameter("pData", spiData, sizeof(spiData))
          .withUnsignedIntParameter("Timeout", HAL_MAX_DELAY)
          .andReturnValue(HAL_OK);

    HAL_SPI_Transmit(&hspi2, spiData, sizeof(spiData), HAL_MAX_DELAY);
}

TEST(MainTests, TestError_Handler) {
    // Simulate an error condition
    mock().disable(); // No expectations needed for infinite loops
    Error_Handler();
}

int main(int argc, char** argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}