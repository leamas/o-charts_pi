cmake_minimum_required(VERSION 3.5)

if (TARGET ocpn::geoprim)
    return ()
endif ()

if (NOT CMAKE_MODULE_PATH)
  set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
endif ()

SET(SRC
    src/bbox.cpp
    src/bbox.h
    src/LLRegion.cpp
    src/LLRegion.h
    src/line_clip.cpp
    src/line_clip.h
    src/poly_math.cpp
    src/poly_math.h
    src/LOD_reduce.cpp
    src/LOD_reduce.h
    src/linmath.h
    src/vector2D.cpp
    )

if (NOT wxWidgets_INCLUDE_DIRS)
    include(SimpleWxConfig)
    SimpleWxConfig()
endif ()

add_library(GEOPRIM STATIC ${SRC})
add_library(ocpn::geoprim ALIAS GEOPRIM)

set_property(TARGET GEOPRIM PROPERTY COMPILE_FLAGS "${OBJ_VISIBILITY}")
target_include_directories(GEOPRIM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(GEOPRIM PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_include_directories(GEOPRIM PRIVATE ../../include)
target_link_libraries(GEOPRIM PRIVATE glew2::glew2)

if(QT_ANDROID)
    target_include_directories(GEOPRIM PRIVATE ../../includeAndroid)
endif(QT_ANDROID)
