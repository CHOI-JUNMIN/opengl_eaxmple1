# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src/dep_glad")
  file(MAKE_DIRECTORY "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src/dep_glad")
endif()
file(MAKE_DIRECTORY
  "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src/dep_glad-build"
  "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix"
  "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/tmp"
  "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src/dep_glad-stamp"
  "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src"
  "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src/dep_glad-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src/dep_glad-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/Public/opengl_eaxmple1/build/dep_glad-prefix/src/dep_glad-stamp${cfgdir}") # cfgdir has leading slash
endif()
