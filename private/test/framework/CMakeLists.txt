add_library(framework STATIC
  Arduino.cpp
)

target_include_directories(framework PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}
)

