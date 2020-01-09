cmake_policy(SET CMP0048 NEW)
project(ElecrowLoRaTest VERSION 1.0)
cmake_minimum_required(VERSION 3.11)

include(CTest)

add_subdirectory(lib/Catch2)
add_subdirectory(framework)

add_library(application STATIC
  ${CMAKE_SOURCE_DIR}/../src/main.cpp
)

target_include_directories(application PUBLIC
  ${CMAKE_SOURCE_DIR}/../src
)

target_compile_definitions(application PRIVATE
  -DPIN_LORA_SS=1
  -DPIN_LORA_RESET=2
  -DPIN_LORA_DIO0=3
)

target_link_libraries(application PUBLIC
  framework
)

add_executable(scenario1App
  scenario1.cpp
)

target_link_libraries(scenario1App PUBLIC
  application
  Catch2::Catch2
)

add_test(NAME scenario1 COMMAND scenario1App)

add_executable(scenario2App
  scenario2.cpp
)

target_link_libraries(scenario2App PUBLIC
  framework
  Catch2::Catch2
)

add_test(NAME scenario2 COMMAND scenario2App)

add_executable(scenario3App
  scenario3.cpp
)

target_link_libraries(scenario3App PUBLIC
  Catch2::Catch2
)

add_test(NAME scenario3 COMMAND scenario3App)
