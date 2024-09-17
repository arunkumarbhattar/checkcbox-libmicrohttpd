cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_COMPILER /home/twinturbo/Desktop/ExtraSpace/CheckCBox_Compiler/llvm/cmake-build-debug/bin/clang)
project(testBench)



add_compile_options( -fPIC -g )

add_link_options(-ldl -lstdc++)
add_executable(testBench "${CMAKE_SOURCE_DIR}/test_str_quote.c")
target_include_directories(testBench PUBLIC "${CMAKE_SOURCE_DIR}/../include"
        "${CMAKE_SOURCE_DIR}/../../")
target_link_libraries(testBench
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-basicauth.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-connection.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-daemon.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-digestauth.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-gen_auth.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-internal.o"

        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-md5.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-memorypool.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_compat.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_itc.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_mono_clock.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_panic.o"

        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_send.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_sockets.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_str.o"

        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-mhd_threads.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-postprocessor.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-reason_phrase.o"

        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-response.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-sha256.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-sha512_256.o"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-libmicrohttpd/src/microhttpd/Libs/libmicrohttpd_la-sysfdsetsize.o"


        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-proftpd/Checkcbox_LIBS/libisc_lib_final.a"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-proftpd/Checkcbox_LIBS/libSBX_CON_LIB.a"
        "/home/twinturbo/Desktop/ExtraSpace/checkcbox-proftpd/Checkcbox_LIBS/libisc_lib_final.so"
        -fsanitize=address  -static-libsan
        dl)

#
#add_executable(SIMPLE "${CMAKE_SOURCE_DIR}/sbx_interface_def/simple_main.c")
#target_link_libraries(SIMPLE final)
