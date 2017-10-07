cmake_minimum_required(VERSION 3.0)
project(simple-compiler)

set(CMAKE_VERBOSE_MAKEFILE on)
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/source)
set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

include_directories(
  ${INCLUDE_PATH}
)

set(SOURCE_FILES
  ${SOURCE_PATH}/token.c
  ${SOURCE_PATH}/lex.c
  ${SOURCE_PATH}/syntatic.c
  ${SOURCE_PATH}/semantic.c
  ${SOURCE_PATH}/symbol_table.c
  ${SOURCE_PATH}/tables.c
  ${SOURCE_PATH}/main.c
)

set(CMAKE_C_FLAGS
  "${CMAKE_C_FLAGS} -ggdb -std=c11 -Wall -Wextra -Werror -Wfatal-errors"
)

SET(DEBUG $ENV{DEBUG})
if (DEBUG STREQUAL "true")
  set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -g -O0"
  )
else ()
  set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -O3"
  )
endif ()

add_executable(compiler ${SOURCE_FILES})
