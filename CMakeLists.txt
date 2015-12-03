cmake_minimum_required(VERSION 3.2)
set(TRANSFER_SRC m3u_transfer.c)
set(RANDOM_SRC random_m3u.c)
set(GCC_CFLAGS "-Wall")

add_definitions(${GCC_CFLAGS})
add_executable(m3u_transfer ${TRANSFER_SRC})
add_executable(m3u_random ${RANDOM_SRC})
