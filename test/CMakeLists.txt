project(ElecrowLoRaTest)
cmake_minimum_required(VERSION 3.11)

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

add_executable(scenario1
  scenario1.cpp
)

target_link_libraries(scenario1 PUBLIC application)

add_executable(scenario2
  scenario2.cpp
)

target_link_libraries(scenario2 PUBLIC framework)
