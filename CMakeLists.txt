cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(RadarDataProcessing LANGUAGES CXX)

# Spécifier le standard C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ajouter le répertoire des en-têtes
include_directories(${PROJECT_SOURCE_DIR}/include)

# Récupérer tous les fichiers .cpp dans le dossier src
file(GLOB SOURCES src/*.cpp)

# Définir le répertoire de sortie pour l'exécutable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Ajouter le chemin pour FindTBB.cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Dépendances Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Python3 COMPONENTS NumPy)

# Ajouter la bibliothèque matplotlib_cpp
add_library(matplotlib_cpp INTERFACE)
target_include_directories(matplotlib_cpp INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(matplotlib_cpp INTERFACE cxx_std_11)
target_link_libraries(matplotlib_cpp INTERFACE Python3::Python Python3::Module)

if(Python3_NumPy_FOUND)
  target_link_libraries(matplotlib_cpp INTERFACE Python3::NumPy)
else()
  target_compile_definitions(matplotlib_cpp INTERFACE WITHOUT_NUMPY)
endif()

# Trouver FFTW
find_package(FFTW REQUIRED)
if(FFTW_FOUND)
  include_directories(${FFTW_INCLUDE_DIRS})
  set(FFTW_LIBRARIES ${FFTW_LIBRARIES})
else()
  message(FATAL_ERROR "Could not find FFTW")
endif()

# Trouver TBB
find_package(TBB REQUIRED)
if(TBB_FOUND)
  include_directories(${TBB_INCLUDE_DIRS})
  set(TBB_LIBRARIES ${TBB_LIBRARIES})
else()
  message(FATAL_ERROR "Could not find TBB")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")

if(NOT DEFINED EXEC_NAME)
  set(EXEC_NAME "RadarDataDefault")
endif()

# Add executable
add_executable(${EXEC_NAME} ${SOURCES})
target_compile_options(${EXEC_NAME} PRIVATE "-g")
target_link_libraries(${EXEC_NAME} PRIVATE matplotlib_cpp ${FFTW_LIBRARIES} ${TBB_LIBRARIES})

# FULLPATH, IPCLIENT, UDPPORT definition
target_compile_definitions(${EXEC_NAME} PRIVATE
    FULLPATH="${FULLPATH}"
    IPCLIENT="${IPCLIENT}"
    UDPPORT=${UDPPORT}
)

# Option compilation
option(RN_THREAD "Compile with RN thread" OFF)
option(RH_THREAD "Compile with RH thread" OFF)
option(RT_THREAD "Compile with RT thread" OFF)
option(RS_THREAD "Compile with RS thread" OFF)
option(RH_PLOT "Compile with RH plot" OFF)
option(RN_PLOT "Compile with RN plot" OFF)

if(RN_THREAD)
  add_definitions(-DRN_THREAD)
endif()
if(RH_THREAD)
  add_definitions(-DRH_THREAD)
endif()
if(RT_THREAD)
  add_definitions(-DRT_THREAD)
endif()
if(RS_THREAD)
  add_definitions(-DRS_THREAD)
endif()
if(RH_PLOT)
  add_definitions(-DRH_PLOT)
endif()
if(RN_PLOT)
  add_definitions(-DRN_PLOT)
endif()
