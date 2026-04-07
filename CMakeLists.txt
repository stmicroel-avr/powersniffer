cmake_minimum_required(VERSION 3.20)

project(PowerSniffer C)

# --- MCU и частота ---
set(MCU atmega328p)
set(F_CPU 8000000UL)

# --- Путь к тулчейну от Atmel Studio 7 ---
set(AVR_TOOLCHAIN_ROOT "C:/Program Files (x86)/Atmel/Studio/7.0/toolchain/avr8/avr8-gnu-toolchain")

# --- Настройка под AVR ---
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ГОВОРИМ CMake: никаких стандартных либ, которые он нашёл у MinGW
set(CMAKE_C_STANDARD_LIBRARIES "")
set(CMAKE_EXE_LINKER_FLAGS_INIT "")

set(CMAKE_C_COMPILER "${AVR_TOOLCHAIN_ROOT}/bin/avr-gcc.exe")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Флаги компиляции
set(CMAKE_C_FLAGS
        "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os -std=gnu11 -Wall -Wextra -ffunction-sections -fdata-sections -Wl,-u,vfprintf -lprintf_flt -lm"
)

# Флаги линковки
set(CMAKE_EXE_LINKER_FLAGS
        "-mmcu=${MCU} -Wl,--gc-sections"
)

# Будем собирать ELF-файл
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

# Переопределяем правило линковки, чтобы не было --out-implib и прочего мусора
set(CMAKE_C_LINK_EXECUTABLE
        "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
)

# --- Инклуды ---
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/board
        ${CMAKE_CURRENT_SOURCE_DIR}/src/drivers
        ${CMAKE_CURRENT_SOURCE_DIR}/src/devices
        ${CMAKE_CURRENT_SOURCE_DIR}/src/modules
)

# --- Исходники: всё .c внутри src/** ---
file(GLOB_RECURSE SRC_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/devices/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/drivers/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/*.c"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# --- Генерация HEX и вывод размера ---
set(AVR_OBJCOPY "${AVR_TOOLCHAIN_ROOT}/bin/avr-objcopy.exe")
set(AVR_SIZE    "${AVR_TOOLCHAIN_ROOT}/bin/avr-size.exe")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${AVR_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
        COMMAND ${AVR_SIZE} -C --mcu=${MCU} $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Создаю HEX и показываю размер"
)
