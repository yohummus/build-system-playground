cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug -G "NMake Makefiles"

nmake yogi-core-test && yogi-core\yogi-core-test.exe --gtest_repeat=1 --gtest_break_on_failure --gtest_catch_exceptions=0
