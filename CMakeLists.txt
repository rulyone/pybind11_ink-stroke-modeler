cmake_minimum_required(VERSION 3.19)
project(smooth_lines VERSION 0.1 LANGUAGES CXX)
add_subdirectory(pybind11)

include(FetchContent)
#set(ABSL_USE_EXTERNAL_GOOGLETEST ON)
#set(ABSL_FIND_GOOGLETEST ON)
set(ABSL_PROPAGATE_CXX_STD ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# if(INK_STROKE_MODELER_ENABLE_INSTALL)
#   set(ABSL_ENABLE_INSTALL ON)
# endif()
FetchContent_Declare(
    absl
    GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
    GIT_TAG        20211102.0
    GIT_SHALLOW    TRUE
    GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(absl)

include_directories("${CMAKE_SOURCE_DIR}/ink-stroke-modeler")
include_directories("${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler")
include_directories("${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal")
include_directories("${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/prediction")
include_directories("${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/prediction/kalman_filter")

add_subdirectory(ink-stroke-modeler)

pybind11_add_module(
    smooth_lines 
    wrapper.cpp 
    #The following dependencies are not needed if there weren't an abseil dependency.
    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/types.cc
    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/params.cc
    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/stroke_modeler.cc

    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/wobble_smoother.cc
    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/stylus_state_modeler.cc

    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/prediction/kalman_predictor.cc
    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/prediction/stroke_end_predictor.cc

    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/prediction/kalman_filter/axis_predictor.cc
    ${CMAKE_SOURCE_DIR}/ink-stroke-modeler/ink_stroke_modeler/internal/prediction/kalman_filter/kalman_filter.cc
)
target_link_libraries(smooth_lines PRIVATE absl::status absl::statusor absl::strings)
