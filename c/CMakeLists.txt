set(lib_name "c_utils")
aux_source_directory(. lib_src)
add_library(${lib_name} ${lib_src})

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)
