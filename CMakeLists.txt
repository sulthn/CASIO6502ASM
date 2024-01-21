# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(CASIO6502ASM)

include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.10 REQUIRED)

set(SOURCES
  src/main.cpp
  src/mos6502/mos6502.cpp
  src/graphics.cpp
  src/computer.cpp
  src/assembler.cpp
  src/input.cpp
  # ...
)
set(ASSETS
  assets/font5x7.png
  # ...
)

fxconv_declare_assets(${ASSETS} WITH_METADATA)

add_executable(casio6502asm ${SOURCES} ${ASSETS})
target_compile_options(casio6502asm PRIVATE -Wall -Wextra -Os -std=c++20)
target_link_libraries(casio6502asm Gint::Gint -lstdc++ -lc)

generate_g3a(TARGET casio6502asm OUTPUT "6502ASM.g3a"
  NAME "6502ASM" ICONS assets/uns.png assets/sel.png)
