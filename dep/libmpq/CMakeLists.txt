

#-----------------------------------------------------------------------------
# Define the mpq library
file(GLOB sources *.cpp)
file(GLOB headers *.h)

set(mpq_LIB_SRCS ${sources} ${headers})


include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/common.h
  ${CMAKE_CURRENT_SOURCE_DIR}/explode.h
  ${CMAKE_CURRENT_SOURCE_DIR}/huffman.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mpq.h
  ${CMAKE_CURRENT_SOURCE_DIR}/wave.h
)

#-----------------------------------------------------------------------------
# Build the mpq library
add_library(libmpq STATIC ${mpq_LIB_SRCS})

if(WIN32)
    # Disabled for now. GnuWin32-zlib does not need this
    # set(BUILD_PROPERTIES "-DZLIB_WINAPI" )
    # set_target_properties(libmpq PROPERTIES COMPILE_FLAGS ${BUILD_PROPERTIES})
endif()
