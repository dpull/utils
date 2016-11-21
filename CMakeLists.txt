set(lib_name "c_utils")
aux_source_directory(. lib_src)
add_executable(${lib_name} ${lib_src})
