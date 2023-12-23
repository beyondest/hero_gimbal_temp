cmake_minimum_required(VERSION 3.10)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(root_dir_name ${ROOT_DIR} NAME)

set(myapp_name ${root_dir_name})#myapp_name must match program name in launch.json and code-runner settings.json
set(mylib_name ${root_dir_name}_lib)
set(compile_mode 1) #0 for both lib and app, 1 for app, 2 for lib   
set(EXECUTABLE_OUTPUT_PATH ${ROOT_DIR}/build/exe)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/lib)
set(CMAKE_BUILD_TYPE Debug)     #Debug Release RelWithDebInfo MinSizeRel


project(mypro C CXX ASM)
enable_language(ASM)
set(asm_list ${ROOT_DIR}/startup_stm32f427xx.s)
set_source_files_properties(${asm_list} PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")   
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)   #MCU bare does not have mmu, so set to static library
set(CMAKE_C_COMPILER_FORCED TRUE) # skip compiler test,cause cross compiler cannot run program locally
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_C_COMPILER /home/liyuxuan/gcc-arm/bin/arm-none-eabi-gcc)      #has to set to your path/to/armgcc
set(CMAKE_CXX_COMPILER /home/liyuxuan/gcc-arm/bin/arm-none-eabi-g++)    
set(CMAKE_ASM_COMPILER /home/liyuxuan/gcc-arm/bin/arm-none-eabi-gcc)
set(CMAKE_OBJCOPY /home/liyuxuan/gcc-arm/bin/arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP /home/liyuxuan/gcc-arm/bin/arm-none-eabi-objdump)
set(SIZE /home/liyuxuan/gcc-arm/bin/arm-none-eabi-size) 
set(CMAKE_AR /home/liyuxuan/gcc-arm/bin/arm-none-eabi-ar)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(LINKER_SCRIPT ${ROOT_DIR}/STM32F427IIHx_FLASH.ld)
set(USE_EIGEN ON)


set(libsrc_list)
set(src_list)
set(lib_list)
set(inc_list)
set(header_list)
file(GLOB_RECURSE libsrc_list ${ROOT_DIR}/libsrc/*.c)
file(GLOB_RECURSE src_list ${ROOT_DIR}/Core/Src/*.c
                           ${ROOT_DIR}/Core/Src/*.cpp
                           ${ROOT_DIR}/bsp/*.c
                           ${ROOT_DIR}/bsp/*.cpp
                           ${ROOT_DIR}/modules/*.c
                           ${ROOT_DIR}/modules/*.cpp
                           ${ROOT_DIR}/apps/*.c
                           ${ROOT_DIR}/apps/*.cpp
                           #${ROOT_DIR}/Drivers/CMSIS/DSP/Source/*.c
                           #${ROOT_DIR}/Drivers/CMSIS/DSP/Source/*.cpp
                           )
file(GLOB_RECURSE header_list ${ROOT_DIR}/Core/Inc/*.h
                              ${ROOT_DIR}/bsp/*.h
                              ${ROOT_DIR}/modules/*.h
                              ${ROOT_DIR}/apps/*.h
                              ${ROOT_DIR}/eigen3/*.h)


foreach(header_file ${header_list})
    get_filename_component(header_dir ${header_file} DIRECTORY)
    list(APPEND inc_list ${header_dir} " ")
endforeach(header_file ${header_list})

file(GLOB lib_list ${ROOT_DIR}/lib/*)




list(APPEND inc_list ${ROOT_DIR}/Core/Inc
                     ${ROOT_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc
                     ${ROOT_DIR}/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
                     ${ROOT_DIR}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                     ${ROOT_DIR}/Drivers/CMSIS/Include
                     ${ROOT_DIR}/eigen3/Eigen
                     ${ROOT_DIR}/eigen3
                    # ${ROOT_DIR}/Drivers/CMSIS/DSP/Include
                     )

list(APPEND src_list    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c 
                        Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c 
                    )
list(REMOVE_DUPLICATES inc_list)
list(REMOVE_DUPLICATES src_list)


#add predefine
add_definitions(-DUSE_HAL_DRIVER
                -DSTM32F427xx)
                #-DARM_MATH_CM4 enable arm math cortex m4

#compile flags
set(MCU_FLAGS -mcpu=cortex-m4       #assign cpu cortex 
              -mthumb               #assign Thumb instructions
              -mthumb-interwork     #allow Thumb mixed with ARM
              -mfloat-abi=hard      #assign float application binary interface: Hardware
              -mfpu=fpv4-sp-d16     #assign single float calculation and data 16 bit
              )
set(LINKER_FLAGS -pipe ${MCU_FLAGS}
                 -T${LINKER_SCRIPT}
                 -lm -lc -lnosys -lstdc++       #link to C_Math lib, C_std, C++_std lib; Not link to C_sys
                 -Wl,--gc-sections -flto -specs=nano.specs -specs=nosys.specs #strip no using sections; use Link-Time Optimization; assign linker specs file
                 -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map -Wl,--cref -Wl,--print-memory-usage   #generate map file to show memory usage in exe; generate cross-reference report to show combinations between simbols
                ) # -s : strip all debug info, this will make executable very small

add_compile_options(-pipe ${MCU_FLAGS} -Wall -Werror -fmessage-length=0 # basic options: Warning all; Warning treated as error;force message length disabled; 
                    -ffunction-sections -fdata-sections -fno-common # optimize options ;force function sections and data sections; force no common variebles set in BSS
                    -fstack-usage
                    )


if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g -gdwarf-2)
    add_definitions(-DESC_DEBUG) # ESC Debug
endif ()



add_executable(${myapp_name}.elf ${src_list} ${asm_list})
target_include_directories(${myapp_name}.elf PRIVATE ${inc_list})
target_link_libraries(${myapp_name}.elf PRIVATE ${LINKER_FLAGS} ${lib_list})











#make hex/bin

add_custom_command(
    TARGET ${myapp_name}.elf POST_BUILD
    COMMENT "Building hex & bin file in build/out"
    COMMAND mkdir -p ${ROOT_DIR}/build/out
    COMMAND ${CMAKE_OBJCOPY} -Oihex ${ROOT_DIR}/build/exe/${myapp_name}.elf ${ROOT_DIR}/build/out/${myapp_name}.hex
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${ROOT_DIR}/build/exe/${myapp_name}.elf ${ROOT_DIR}/build/out/${myapp_name}.bin
    COMMENT "EXCUTABLE SIZE:"
    COMMAND ${SIZE} ${ROOT_DIR}/build/exe/${myapp_name}.elf
    COMMAND cp ${ROOT_DIR}/build/out/${myapp_name}.hex /mnt/d/out_stm/hex
    COMMAND cp ${ROOT_DIR}/build/exe/${myapp_name}.elf /mnt/d/out_stm/elf
  )

  
  
