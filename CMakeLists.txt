cmake_minimum_required(VERSION 3.15)
project(cgv_procedural_plot)

include(FetchContent)
FetchContent_Declare(CGV
	GIT_REPOSITORY https://github.com/sgumhold/cgv.git
	GIT_TAG 2d4be8b970d8fb40bcfe0bddde4c251d6633a652
	PATCH_COMMAND git am -3 --keep-cr ${CMAKE_CURRENT_LIST_DIR}/Patch-for-Mesa-driver.diff
	CMAKE_ARGS -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=True
)
FetchContent_MakeAvailable(CGV)

add_library(procedural_plot SHARED src/procedural_plot.cxx)
target_link_libraries(procedural_plot cgv_gl cgv_utils cgv_type cgv_data cgv_base cgv_math
        cgv_media cgv_gui cgv_render glew crg_stereo_view)
add_dependencies(procedural_plot cg_fltk crg_stereo_view)

