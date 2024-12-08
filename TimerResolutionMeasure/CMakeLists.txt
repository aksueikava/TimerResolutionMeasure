add_executable (TimerResolutionMeasure "main.cpp" "pch.h" "utils.cpp" "utils.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TimerResolutionMeasure PROPERTY CXX_STANDARD 20)
endif()

target_precompile_headers(TimerResolutionMeasure PRIVATE pch.h)