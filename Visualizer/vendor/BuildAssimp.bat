
@echo off

setlocal

pushd "%~dp0/assimp/"

SET SOURCE_DIR=.
SET GENERATOR=Visual Studio 17 2022
SET BINARIES_DIR="./build"

cmake . -G "%GENERATOR%" -A x64 -S %SOURCE_DIR% -B %BINARIES_DIR% -DUSE_STATIC_CRT=ON -DBUILD_SHARED_LIBS=OFF -DASSIMP_BUILD_ZLIB=ON -DASSIMP_BUILD_TESTS=OFF -DASSIMP_INSTALL=OFF -DASSIMP_INSTALL_PDB=OFF
cmake --build %BINARIES_DIR% --config debug
cmake --build %BINARIES_DIR% --config release

popd

endlocal