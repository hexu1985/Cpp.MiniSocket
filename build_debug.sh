cmake -Bstage -H. -DCMAKE_INSTALL_PREFIX="install" -DCMAKE_BUILD_TYPE=Debug
cmake --build stage --target install
