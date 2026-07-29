cmake_minimum_required(VERSION 3.19)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stm32-cmake/cmake/stm32_gcc.cmake)
include(tools/fetch_svd.cmake)

# Select MCU model:
set(MCU STM32F767ZIT6)

# Set project name to match directory name. If set othervise tasks.json and launch.json should be updated
get_filename_component(BASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(PROJECT_NAME ${BASE_FOLDER})

project(${PROJECT_NAME} C CXX ASM)


# MCU familiy from MCU
string(SUBSTRING ${MCU} 5 2 MCU_FAMILY)
string(SUBSTRING ${MCU} 5 6 MCU_MODEL)

# Fetch HAL, CMSIS, and CMSIS-SVD
fetch_svd(${MCU})
stm32_fetch_cmsis(${MCU_FAMILY})
stm32_fetch_hal(${MCU_FAMILY})
update_launch_json(${MCU})

find_package(CMSIS COMPONENTS  ${MCU} REQUIRED)
find_package(HAL COMPONENTS STM32${MCU_FAMILY} REQUIRED)


add_executable(${PROJECT_NAME} 
Core/Src/main.c
Core/Src/stm32f7xx_it.c
Core/Src/stm32f7xx_hal_msp.c
) 

target_include_directories(${PROJECT_NAME} PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR} 
${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc 
${CMAKE_CURRENT_SOURCE_DIR}/Core/Src
)

target_compile_options(${PROJECT_NAME} PUBLIC -Os -fno-exceptions -fno-rtti)

# Link required libraries
target_link_libraries(${PROJECT_NAME}
    HAL::STM32::${MCU_FAMILY}::RCC
    HAL::STM32::${MCU_FAMILY}::GPIO
    HAL::STM32::${MCU_FAMILY}::UART
    HAL::STM32::${MCU_FAMILY}::CORTEX
    HAL::STM32::${MCU_FAMILY}::LL_USB
    HAL::STM32::${MCU_FAMILY}::PCD
    HAL::STM32::${MCU_FAMILY}::PCDEx
    CMSIS::STM32::${MCU_MODEL}
    STM32::Nano
    STM32::NoSys
)
set(CMAKE_CXX_STANDARD 20)

stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_elf_bin(${PROJECT_NAME})
stm32_generate_elf_hex(${PROJECT_NAME})