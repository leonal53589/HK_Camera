add_library(mv_lib STATIC
    src/Camera.cpp
)

# 头文件
target_include_directories(mv_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "/opt/MVS/include"
    ${OpenCV_INCLUDE_DIRS}
)

# 库文件
target_link_directories(mv_lib PUBLIC 
    "/opt/MVS/lib/64"
    ${OpenCV_LIB_DIR}
)

# 库名
target_link_libraries(mv_lib PUBLIC 
    MvCameraControl
    ${OpenCV_LIBS}
)