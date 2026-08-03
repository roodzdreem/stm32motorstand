cmake_minimum_required(VERSION 3.19)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake/stm32_gcc.cmake)
include(tools/fetch_svd.cmake)

set(MCU STM32F767ZIT6)

get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME motorstand)
set(DRIVERS_DIR src/drivers)
set(MODELS_DIR src/models)
project(${PROJECT_NAME})
enable_language(C ASM)

# Флаги компиляции — глобально, ДО add_subdirectory, чтобы FreeRTOS их тоже получил
add_compile_options(
    -Os -fno-exceptions
    -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb
    -Wall -ffunction-sections -fdata-sections
    --specs=nano.specs
)

string(SUBSTRING ${MCU} 5 2 MCU_FAMILY)
string(SUBSTRING ${MCU} 5 6 MCU_MODEL)

fetch_svd(${MCU})
stm32_fetch_cmsis(${MCU_FAMILY})
stm32_fetch_hal(${MCU_FAMILY})
update_launch_json(${MCU})

find_package(CMSIS COMPONENTS ${MCU} REQUIRED)
find_package(HAL COMPONENTS STM32${MCU_FAMILY} REQUIRED)
set(FREERTOS_CLI_COMMIT 592732b4d8e8da21f122322de4c421f89e0b4d18)
set(FREERTOS_CLI_DIR ${CMAKE_BINARY_DIR}/_deps/freertos-plus-cli-src)
file(MAKE_DIRECTORY ${FREERTOS_CLI_DIR})
file(DOWNLOAD
    https://raw.githubusercontent.com/FreeRTOS/FreeRTOS/${FREERTOS_CLI_COMMIT}/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI/FreeRTOS_CLI.c
    ${FREERTOS_CLI_DIR}/FreeRTOS_CLI.c
)
file(DOWNLOAD
    https://raw.githubusercontent.com/FreeRTOS/FreeRTOS/${FREERTOS_CLI_COMMIT}/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI/FreeRTOS_CLI.h
    ${FREERTOS_CLI_DIR}/FreeRTOS_CLI.h
)

add_library(freertos_plus_cli STATIC ${FREERTOS_CLI_DIR}/FreeRTOS_CLI.c)
target_include_directories(freertos_plus_cli PUBLIC ${FREERTOS_CLI_DIR})
target_link_libraries(freertos_plus_cli PUBLIC freertos_kernel)
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bsp/freertos_config
)

set(FREERTOS_PORT "GCC_ARM_CM7" CACHE STRING "" FORCE)
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_subdirectory(src/FreeRTOS-Kernel)
add_subdirectory(${DRIVERS_DIR}/usart)
add_subdirectory(${MODELS_DIR}/cli)

add_executable(${PROJECT_NAME} 
    src/main.c
    src/bsp/syscalls.c
    src/bsp/sysmem.c
) 

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR} 
)
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_FULL_LL_DRIVER)

target_link_options(${PROJECT_NAME} PUBLIC
    --specs=nosys.specs
    -mcpu=cortex-m7
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -mthumb
    -Wl,--gc-sections
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    freertos_plus_cli
    usart
    cli
    HAL::STM32::${MCU_FAMILY}::LL_GPIO
    HAL::STM32::${MCU_FAMILY}::LL_UTILS
    HAL::STM32::${MCU_FAMILY}::LL_RCC
    HAL::STM32::${MCU_FAMILY}::LL_USART
    CMSIS::STM32::${MCU_MODEL}
    freertos_kernel
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_elf_bin(${PROJECT_NAME})
stm32_generate_elf_hex(${PROJECT_NAME})