
#cmake_minimum_required(VERSION 2.8)

#PROJECT(MedicalDemo1)

#find_package(VTK REQUIRED)
#include(${VTK_USE_FILE})

#set(SRC_LIST MedicalDemo1.cxx
#             MedicalDemo2.cxx)
#add_executable(MedicalDemo1 MACOSX_BUNDLE ${SRC_LIST})

#if(VTK_LIBRARIES)
#  target_link_libraries(MedicalDemo1 ${VTK_LIBRARIES})
#else()
#  target_link_libraries(MedicalDemo1 vtkHybrid vtkWidgets)
#endif()

cmake_minimum_required(VERSION 2.8)

PROJECT(MedicalDemo1)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#set(SRC_LIST MedicalDemo1
#             MedicalDemo2)
aux_source_directory(. SRC_LIST)
#AUX_SOURCE_DIRECTORY(. SRC_LIST)

foreach(name ${SRC_LIST})
  string(REGEX REPLACE "[.]cxx$" "" ELF ${name})
  add_executable(${ELF} MACOSX_BUNDLE ${name})
  if(VTK_LIBRARIES)
    target_link_libraries(${ELF} ${VTK_LIBRARIES})
  else()
    target_link_libraries(${ELF} vtkHybrid vtkWidgets)
  endif()
endforeach()
