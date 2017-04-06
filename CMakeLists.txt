cmake_minimum_required(VERSION 2.8.5 FATAL_ERROR)
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()

project (VolumeRendering)
find_package(VTK REQUIRED)
vtk_module_config(VTK
  vtkCommonCore
  vtkIOImage
  vtkIOXML
  vtkInteractionWidgets
  vtkRenderingVolume${VTK_RENDERING_BACKEND}
)
include(${VTK_USE_FILE})

aux_source_directory(. SRC_LIST)

foreach(name ${SRC_LIST})
  string(REGEX REPLACE "[.]cxx$" "" ELF ${name})
  add_executable(${ELF} MACOSX_BUNDLE ${name})
  target_link_libraries(${ELF} ${VTK_LIBRARIES})
endforeach()

