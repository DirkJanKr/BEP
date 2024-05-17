include_guard(GLOBAL)


if (CONFIG_USE_BOARD_Project_Template_evkmimx8mnddr3l)
# Add set(CONFIG_USE_BOARD_Project_Template_evkmimx8mnddr3l true) in config.cmake to use this component

message("BOARD_Project_Template_evkmimx8mnddr3l component is included from ${CMAKE_CURRENT_LIST_FILE}.")

if((CONFIG_BOARD STREQUAL evkmimx8mnddr3l))

add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/board.h "" BOARD_Project_Template_evkmimx8mnddr3l)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/board.c "" BOARD_Project_Template_evkmimx8mnddr3l)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/clock_config.h ${CMAKE_CURRENT_LIST_DIR}/project_template/. BOARD_Project_Template_evkmimx8mnddr3l)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/clock_config.c "" BOARD_Project_Template_evkmimx8mnddr3l)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/pin_mux.h ${CMAKE_CURRENT_LIST_DIR}/project_template/. BOARD_Project_Template_evkmimx8mnddr3l)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/pin_mux.c "" BOARD_Project_Template_evkmimx8mnddr3l)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/peripherals.h ${CMAKE_CURRENT_LIST_DIR}/project_template/. BOARD_Project_Template_evkmimx8mnddr3l)
add_config_file(${CMAKE_CURRENT_LIST_DIR}/project_template/peripherals.c "" BOARD_Project_Template_evkmimx8mnddr3l)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/project_template/.
)

else()

message(SEND_ERROR "BOARD_Project_Template_evkmimx8mnddr3l dependency does not meet, please check ${CMAKE_CURRENT_LIST_FILE}.")

endif()

endif()

