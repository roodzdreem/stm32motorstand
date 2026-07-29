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

find_package(CMSIS COMPONENTS ${MCU} REQUIRED)
find_package(HAL COMPONENTS STM32${MCU_FAMILY} REQUIRED)


add_executable(${PROJECT_NAME} 
src/main.c
) 

target_include_directories(${PROJECT_NAME} PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR} 
)
target_compile_definitions(${PROJECT_NAME} PUBLIC USE_FULL_LL_DRIVER)
target_compile_options(${PROJECT_NAME} PUBLIC 
    -Os -fno-exceptions -fno-rtti
    -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb
    -Wall -ffunction-sections -fdata-sections
    --specs=nano.specs
)

target_link_options(${PROJECT_NAME} PUBLIC
    --specs=nosys.specs
    -mcpu=cortex-m7
    -mfpu=fpv5-sp-d16
    -mfloat-abi=hard
    -mthumb
    -Wl,--gc-sections
)

target_link_libraries(${PROJECT_NAME}
    HAL::STM32::${MCU_FAMILY}::LL_GPIO
    HAL::STM32::${MCU_FAMILY}::LL_UTILS
    HAL::STM32::${MCU_FAMILY}::LL_RCC
    CMSIS::STM32::${MCU_MODEL}
)

set(CMAKE_CXX_STANDARD 20)

stm32_print_size_of_target(${PROJECT_NAME})
stm32_generate_elf_bin(${PROJECT_NAME})
stm32_generate_elf_hex(${PROJECT_NAME})