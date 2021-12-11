function(AddPluginTarget targetname files)
    add_library(${targetname} MODULE ${files})
    target_link_directories(${targetname} PUBLIC ${ARNOLD_LIBRARY_DIR})
    target_include_directories(${targetname} PUBLIC ${ARNOLD_INCLUDE_DIR})
    target_link_libraries(${targetname} PUBLIC ai)
    set_target_properties(${targetname} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )
endfunction()