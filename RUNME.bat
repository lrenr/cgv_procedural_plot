set WORKING_DIR=%cd%

setx CGV_DIR "%WORKING_DIR%\build\_deps\cgv-src" /m

set LIB_CGV_GL="%CGV_DIR%\libs\cgv_gl\glsl;"
set LIB_PLOT="%CGV_DIR%\libs\plot\glsl;"
set EXAMPLES ="%CGV_DIR%\plugins\examples;"
set SRC_GLSL="%WORKING_DIR%\src\glsl;"

setx CGV_SHADER_PATH "%LIB_CGV_GL%%LIB_PLOT%%EXAMPLES%%SRC_GLSL%" \m

build\bin\cgv_viewer plugin:cg_fltk plugin:cg_ext plugin:cg_icons plugin:crg_stereo_view plugin:crg_antialias plugin:crg_depth_of_field plugin:crg_light plugin:cg_meta plugin:cmi_io plugin:crg_grid plugin:cmf_tt_gl_font plugin:examples plugin:procedural_plot
