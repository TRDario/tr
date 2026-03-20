add_executable(norm_cast norm_cast.cpp)
tr_target_template(norm_cast)
add_test(NAME norm_cast COMMAND norm_cast)

add_executable(utf8 utf8.cpp)
tr_target_template(utf8)
add_test(NAME utf8 COMMAND utf8)