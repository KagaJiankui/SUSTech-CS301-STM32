cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE
    USE_HAL_DRIVER
    STM32F103xE
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../Drivers/STM32F1xx_HAL_Driver/Inc
    ../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    ../../Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ../../Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/24cxx.c
    ../../Core/Src/24l01.c
    ../../Core/Src/ctiic.c
    ../../Core/Src/delay.c
    ../../Core/Src/ft5206.c
    ../../Core/Src/gt9147.c
    ../../Core/Src/key.c
    ../../Core/Src/lcd.c
    ../../Core/Src/led.c
    ../../Core/Src/main.c
    ../../Core/Src/myiic.c
    ../../Core/Src/ott2001a.c
    ../../Core/Src/spi.c
    ../../Core/Src/stm32f1xx_hal_msp.c
    ../../Core/Src/stm32f1xx_it.c
    ../../Core/Src/sys.c
    ../../Core/Src/syscalls.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/system_stm32f1xx.c
    ../../Core/Src/touch.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
    ../../startup_stm32f103xe.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()
