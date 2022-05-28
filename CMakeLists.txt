cmake_minimum_required(VERSION 3.15)
project(cgv_procedural_plot)

include(FetchContent)
FetchContent_Declare(CGV
	GIT_REPOSITORY https://github.com/sgumhold/cgv.git
	GIT_TAG 798bbc9590e68a596650d2f3afc2f51106a07584
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=True
)

FetchContent_MakeAvailable(CGV)

add_library(procedural_plot SHARED src/procedural_plot.cxx)
target_link_libraries(procedural_plot cgv_gl cgv_utils cgv_type cgv_data cgv_base cgv_math
        cgv_media cgv_gui cgv_render glew crg_stereo_view)
add_dependencies(procedural_plot cg_fltk crg_stereo_view)

