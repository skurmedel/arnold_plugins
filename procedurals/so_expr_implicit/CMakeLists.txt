include(AddPlugin)

set(LIB_NAME "so_expr_implicit")

AddPluginTarget(${LIB_NAME} expr_implicit.cpp)