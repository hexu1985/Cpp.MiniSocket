cmake -G "MSYS Makefiles" -Bbuild -H. -DCMAKE_INSTALL_PREFIX="install" -DCMAKE_BUILD_TYPE=Release
cmake --build build --target install
