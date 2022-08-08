#!/bin/bash

WORKING_DIR=$(pwd);

export CGV_DIR="$WORKING_DIR/build/_deps/cgv-src"

CGV_SHADER_PATH=\
"$CGV_DIR/libs/cgv_gl/glsl;\
$CGV_DIR/libs/plot/glsl;\
$CGV_DIR/plugins/examples;\
$WORKING_DIR/src/glsl"

bin/linux/cgv_viewer plugin:cg_fltk "type(shader_config):shader_path='bin/glsl'" plugin:cg_ext plugin:cg_icons plugin:crg_stereo_view plugin:crg_antialias plugin:crg_depth_of_field plugin:crg_light plugin:cg_meta plugin:cmi_io plugin:crg_grid plugin:cmf_tt_gl_font plugin:examples plugin:procedural_plot
