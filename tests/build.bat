if not exist "build" (
	mkdir build
)

cd Build
cmake ../ -G"Visual Studio 16 2019"
cd ..