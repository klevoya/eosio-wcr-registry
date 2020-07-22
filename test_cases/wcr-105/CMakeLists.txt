cmake_minimum_required(VERSION 3.5)
get_filename_component(PROJ_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME )

project(${PROJ_NAME} VERSION 1.0.0)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
if(EOSIO_CDT_ROOT STREQUAL "" OR NOT EOSIO_CDT_ROOT)
   find_package(eosio.cdt)
endif()

add_contract( vulnerable vulnerable vulnerable.cpp )
add_contract( fix fix fix.cpp )
add_contract( freezevuln freezevuln freezevuln.cpp )
# add_contract( ${PROJ_NAME} ${PROJ_NAME} ${PROJ_NAME}.cpp )
# target_include_directories( ${PROJ_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/../include ${CMAKE_SOURCE_DIR}/../.. )
# target_ricardian_directory( ${PROJ_NAME} ${CMAKE_SOURCE_DIR}/../ricardian )