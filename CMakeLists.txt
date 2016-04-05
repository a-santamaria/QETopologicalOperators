cmake_minimum_required(VERSION 2.8)

project(QETest)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})


add_executable(QETest qdtest3.cxx)
add_executable(QETest2 qdtest2.cxx)


  target_link_libraries(QETest ${ITK_LIBRARIES})
  target_link_libraries(QETest2 ${ITK_LIBRARIES})
