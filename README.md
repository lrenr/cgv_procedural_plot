# CGV-Plugin to generate plots in the fragment shader

```console
export CGV_DIR=[path to cgv]

export CGV_SHADER_PATH=\
[path to cgv]/libs/cgv_gl/glsl;\
[path to cgv]/libs/plot/glsl;\
[path to cgv]/plugins/examples;\
[path to cgv_procedural_plot]/src/glsl

mkdir build
cd build
cmake ..
cmake --build . -j

bin/cgv_viewer plugin:cg_fltk [...] plugin:my_plugin
```

