add_executable(App src/main.cpp)

target_link_libraries(App PRIVATE
    mv_lib
)