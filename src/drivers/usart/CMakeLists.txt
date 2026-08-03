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
)

# Перечисление зависимостей
list(APPEND ${PROJECT_NAME}_DEPENDENCIES
	freertos_kernel
	freertos_config
)



