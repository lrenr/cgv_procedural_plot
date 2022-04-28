cmake_minimum_required(VERSION 3.15)
project(cgv_procedural_plot)
 
find_package(CGV REQUIRED)
 
add_library(procedural_plot SHARED src/procedural_plot.cxx)
target_link_libraries(procedural_plot cgv_gl)
add_dependencies(procedural_plot cg_fltk crg_stereo_view)

