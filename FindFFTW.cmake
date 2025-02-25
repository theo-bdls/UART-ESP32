# Locate FFTW library
# This module defines
# FFTW_INCLUDE_DIRS - where to find fftw3.h, etc.
# FFTW_LIBRARIES - List of libraries when using FFTW.
# FFTW_FOUND - True if FFTW found.

find_path(FFTW_INCLUDE_DIR fftw3.h
  HINTS
    ENV FFTW_HOME
    PATH_SUFFIXES include
)

find_library(FFTW_LIBRARY_RELEASE
  NAMES fftw3
  HINTS
    ENV FFTW_HOME
    PATH_SUFFIXES lib
)

#find_library(FFTW_THREADS_LIBRARY_RELEASE
#  NAMES fftw3_threads
#  HINTS
#  ENV FFTW_HOME
#  PATH_SUFFIXES lib
#)

if (FFTW_INCLUDE_DIR AND FFTW_LIBRARY_RELEASE) #AND FFTW_THREADS_LIBRARY_RELEASE
  set(FFTW_FOUND TRUE)
  set(FFTW_INCLUDE_DIRS ${FFTW_INCLUDE_DIR})
  set(FFTW_LIBRARIES optimized ${FFTW_LIBRARY_RELEASE}) #${FFTW_THREADS_LIBRARY_RELEASE}
else()
  set(FFTW_FOUND FALSE)
endif()

if (FFTW_FOUND)
  if (NOT FFTW_FIND_QUIETLY)
    message(STATUS "Found FFTW: ${FFTW_LIBRARIES}")
  endif()
else()
  if (FFTW_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find FFTW")
  endif()
endif()

mark_as_advanced(FFTW_INCLUDE_DIR FFTW_LIBRARY_RELEASE) #FFTW_THREADS_LIBRARY_RELEASE
