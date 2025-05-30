cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 23)

option(SERVER "build server" OFF)

if(SERVER)
    project(server)
endif()

if(NOT SERVER)
    project(client)
endif()



set(CMAKE_PREFIX_PATH /usr/local/lib/libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
include_directories(
  "${TORCH_INCLUDE_DIRS}"
)

option(MYGAME_VENDORED "Use vendored libraries" OFF)

if(MYGAME_VENDORED)
    add_subdirectory(vendored/sdl EXCLUDE_FROM_ALL)
else()
    find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

    find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)

endif()
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2_image REQUIRED SDL2_image>=2.0.0)

# add_library(game_client_lib SHARED)
# target_include_directories(game_client_lib PRIVATE "${PROJECT_SOURCE_DIR}")

list( APPEND CMAKE_BUILD_RPATH "/home/lennart/cpp_projects/game_engine/GameNetworkingSockets/bin/" )

add_subdirectory("GameNetworkingSockets")
add_subdirectory("src/Game_Client")


add_executable(${PROJECT_NAME})


add_subdirectory("alpha_zero")
target_link_libraries(${PROJECT_NAME} PRIVATE alpha_zero_lib)

add_subdirectory("src/Networking")


if(SERVER)
    add_subdirectory("src/Game_Server")
    target_sources(${PROJECT_NAME} PRIVATE "src/Game_Server/main.cpp")
    target_link_libraries(${PROJECT_NAME} PRIVATE game_server_lib)
endif()


if(NOT SERVER)
    target_sources(${PROJECT_NAME} PRIVATE "src/main.cpp")
endif()

# list(APPEND CMAKE_PREFIX_PATH "~/cpp_projects/game_engine/alpha_zero_build")
# find_package(AlphaZero REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE AlphaZero)

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     BUILD_RPATH "${CMAKE_SOURCE_DIR}/GameNetworkingSockets/bin"
# )

# target_link_libraries(${PROJECT_NAME} PRIVATE protobuf)
target_link_libraries(${PROJECT_NAME} PUBLIC "${TORCH_LIBRARIES}")

# target_link_libraries(${PROJECT_NAME} PUBLIC libprotobuf)

target_link_libraries(${PROJECT_NAME} PUBLIC networking_lib)

target_link_libraries(${PROJECT_NAME} PUBLIC networking)

target_link_libraries(${PROJECT_NAME} PRIVATE game_client_lib)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2_image)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf)


add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${PROJECT_NAME} copy_assets)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME})
install(FILES 
"/home/lennart/cpp_projects/game_engine/GameNetworkingSockets/bin/libGameNetworkingSockets.so"
"/home/lennart/cpp_projects/game_engine/GameNetworkingSockets/bin/libprotobuf.so.23" TYPE LIB)
install (TARGETS
game_client_lib
RUNTIME DESTINATION bin
LIBRARY DESTINATION lib)
