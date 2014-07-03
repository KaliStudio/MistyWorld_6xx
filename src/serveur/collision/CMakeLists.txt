
# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

if( USE_COREPCH )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif()

file(GLOB_RECURSE sources_Management Management/*.cpp Management/*.h)
file(GLOB_RECURSE sources_Maps Maps/*.cpp Maps/*.h)
file(GLOB_RECURSE sources_Models Models/*.cpp Models/*.h)
file(GLOB sources_localdir *.cpp *.h)

if (USE_COREPCH)
  set(collision_STAT_PCH_HDR PrecompiledHeaders/collisionPCH.h)
  set(collision_STAT_PCH_SRC PrecompiledHeaders/collisionPCH.cpp)
endif ()

set(collision_STAT_SRCS
  ${collision_STAT_SRCS}
  ${sources_Management}
  ${sources_Maps}
  ${sources_Models}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/src/serveur/commun
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Configuration
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Debugging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Database
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Debugging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Dynamic
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Logging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Threading
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Packets
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Utilities
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/DataStores
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Addons
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Conditions
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Item
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/GameObject
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Creature
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Object
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Object/Updates
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Unit
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Combat
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Loot
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Miscellaneous
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids/Cells
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids/Notifiers
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Maps
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/DataStores
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/Waypoints
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/Spline
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Server
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Server/Protocol
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/World
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Spells
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Spells/Auras
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Management
  ${CMAKE_CURRENT_SOURCE_DIR}/Maps
  ${CMAKE_CURRENT_SOURCE_DIR}/Models
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
)

add_library(collision STATIC
  ${collision_STAT_SRCS}
  ${collision_STAT_PCH_SRC}
)

target_link_libraries(collision
  commun
)

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(collision ${collision_STAT_PCH_HDR} ${collision_STAT_PCH_SRC})
endif ()
