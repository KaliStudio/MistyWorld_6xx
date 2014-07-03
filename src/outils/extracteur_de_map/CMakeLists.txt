
# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources *.cpp *.h)

include_directories (
  ${CMAKE_SOURCE_DIR}/src/serveur/commun
  ${CMAKE_SOURCE_DIR}/dep/StormLib/src
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/loadlib
)

include_directories(${include_Dirs})

add_executable(extracteurdemap
  ${sources}
)

target_link_libraries(extracteurdemap
  ${BZIP2_LIBRARIES}
  ${ZLIB_LIBRARIES}
  storm
)

add_dependencies(extracteurdemap storm)

if( UNIX )
  install(TARGETS extracteurdemap DESTINATION bin)
elseif( WIN32 )
  install(TARGETS extracteurdemap DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
