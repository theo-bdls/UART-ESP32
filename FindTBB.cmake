# Locate TBB library
# This module defines
# TBB_INCLUDE_DIRS - where to find tbb/tbb.h, etc.
# TBB_LIBRARIES - List of libraries when using TBB.
# TBB_FOUND - True if TBB found.

find_path(TBB_INCLUDE_DIR tbb/tbb.h
  HINTS
    ENV TBB_HOME
    PATH_SUFFIXES include
)

find_library(TBB_LIBRARY_RELEASE
  NAMES tbb
  HINTS
    ENV TBB_HOME
    PATH_SUFFIXES lib
)

if (TBB_INCLUDE_DIR AND (TBB_LIBRARY_RELEASE))
  set(TBB_FOUND TRUE)
  set(TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIR})
  set(TBB_LIBRARIES optimized ${TBB_LIBRARY_RELEASE})
else()
  set(TBB_FOUND FALSE)
endif()

if (TBB_FOUND)
  if (NOT TBB_FIND_QUIETLY)
    message(STATUS "Found TBB: ${TBB_LIBRARIES}")
  endif()
else()
  if (TBB_FIND_REQUIRED)
    message(FATAL_ERROR "Could not find TBB")
  endif()
endif()

mark_as_advanced(TBB_INCLUDE_DIR TBB_LIBRARY_RELEASE)
