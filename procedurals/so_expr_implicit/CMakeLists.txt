include(AddPlugin)

set(LIB_NAME "so_expr_implicit")

add_library(tinyexpr STATIC tinyexpr/tinyexpr.c)
AddPluginTarget(${LIB_NAME} expr_implicit.cpp)
target_link_libraries(${LIB_NAME} PRIVATE tinyexpr)