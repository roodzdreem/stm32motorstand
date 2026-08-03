cmake_minimum_required(VERSION 3.15)

# Имя компонента
set(UNIT "usart")

# Настройки компонента
project(${UNIT} VERSION 1.0.0 LANGUAGES C)

string(TOUPPER ${UNIT} UNIT_UPPER)
set(${PROJECT_NAME}_SOURCES "")
set(${PROJECT_NAME}_INCLUDES "")
set(${PROJECT_NAME}_INTERFACE "")
set(${PROJECT_NAME}_DEFINES "")
set(${PROJECT_NAME}_DEPENDENCIES "")

# Добавление исходников
list(APPEND ${PROJECT_NAME}_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/${UNIT}.c
)

# Добавление заголовков
list(APPEND ${PROJECT_NAME}_INCLUDES
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	../../bsp
)

# Добавление заголовков интерфейса
list(APPEND ${PROJECT_NAME}_INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# Добавление дефайнов
list(APPEND ${PROJECT_NAME}_DEFINES
	USE_FULL_LL_DRIVER
)

# Перечисление зависимостей
list(APPEND ${PROJECT_NAME}_DEPENDENCIES
	freertos_kernel
	freertos_config
	CMSIS::STM32::${MCU_MODEL}
	HAL::STM32::${MCU_FAMILY}::LL_GPIO
	HAL::STM32::${MCU_FAMILY}::LL_USART
)



add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES})

target_include_directories(${PROJECT_NAME}
    PUBLIC  ${${PROJECT_NAME}_INTERFACE}
    PRIVATE ${${PROJECT_NAME}_INCLUDES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_DEFINES})

target_link_libraries(${PROJECT_NAME} PUBLIC ${${PROJECT_NAME}_DEPENDENCIES})