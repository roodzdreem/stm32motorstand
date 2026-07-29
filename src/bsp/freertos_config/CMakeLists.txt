add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
	INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/
)
target_compile_definitions(freertos_config
	INTERFACE
)

target_link_libraries(freertos_config INTERFACE CMSIS)

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "")
