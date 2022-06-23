# CGV-Plugin to generate plots in the fragment shader

### Compile
The CGV framework will automatically be fetched by CMake.
Compile time can be long because the entire framework is compiled. 
```commandline
git clone https://github.com/dora-the-explorer/cgv_procedural_plot.git
cd cgv_procedural_plot

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=True
cmake --build . -j
cd ..
```
### Run
All necessary environment variables are set by the script.
#### Linux
```commandline
chmod +x RUNME.sh
./RUNME.sh
```
#### Windows
```commandline
.\RUNME.bat
```
