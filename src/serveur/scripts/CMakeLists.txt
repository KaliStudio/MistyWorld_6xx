
# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# Enable precompiled headers when using the GCC compiler.

if (USE_SCRIPTPCH)
  set(scripts_STAT_PCH_HDR PrecompiledHeaders/ScriptPCH.h)
  set(scripts_STAT_PCH_SRC PrecompiledHeaders/ScriptPCH.cpp)
endif ()

file(GLOB_RECURSE sources_PrecompiledHeaders PrecompiledHeaders/*.cpp PrecompiledHeaders/*.h)
file(GLOB_RECURSE sources_EasternKingdoms EasternKingdoms/*.cpp EasternKingdoms/*.h)
file(GLOB_RECURSE sources_Kalimdor Kalimdor/*.cpp Kalimdor/*.h)
file(GLOB_RECURSE sources_Outland Outland/*.cpp Outland/*.h)
file(GLOB_RECURSE sources_Northrend Northrend/*.cpp Northrend/*.h)
file(GLOB_RECURSE sources_Maelstrom Maelstrom/*.cpp Maelstrom/*.h)
file(GLOB_RECURSE sources_Custom Custom/*.cpp Custom/*.h)
file(GLOB_RECURSE sources_Commands Commands/*.cpp Commands/*.h)
file(GLOB_RECURSE sources_Examples Examples/*.cpp Examples/*.h)
file(GLOB_RECURSE sources_Events Events/*.cpp Events/*.h)
file(GLOB_RECURSE sources_Pandaria Pandaria/*.cpp Pandaria/*.h)
file(GLOB_RECURSE sources_Pet Pet/*.cpp Pet/*.h)
file(GLOB_RECURSE sources_OutdoorPvP OutdoorPvP/*.cpp OutdoorPvP/*.h)
file(GLOB_RECURSE sources_Spells Spells/*.cpp Spells/*.h)
file(GLOB_RECURSE sources_World World/*.cpp World/*.h)

source_group(CMake FILES CMakeLists.txt)
source_group(PrecompiledHeaders FILES ${sources_PrecompiledHeaders})
source_group(EasternKingdoms FILES ${sources_EasternKingdoms})
source_group(Kalimdor FILES ${sources_Kalimdor})
source_group(Outland FILES ${sources_Outland})
source_group(Northrend FILES ${sources_Northrend})
source_group(Maelstrom FILES ${sources_Maelstrom})
source_group(OutdoorPvP FILES ${sources_OutdoorPvP})
source_group(Commands FILES ${sources_Commands})
source_group(Examples FILES ${sources_Examples})
source_group(Events FILES ${sources_Events})
source_group(Pandaria FILES ${sources_Pandaria})
source_group(Pet FILES ${sources_Pet})
source_group(Spells FILES ${sources_Spells})
source_group(World FILES ${sources_World})

message(STATUS "SCRIPT PREPARATIONS")
include(Spells/CMakeLists.txt)

include(Commands/CMakeLists.txt)

include(Examples/CMakeLists.txt)

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  ../jeu/AI/ScriptedAI/ScriptedEscortAI.cpp
  ../jeu/AI/ScriptedAI/ScriptedCreature.cpp
  ../jeu/AI/ScriptedAI/ScriptedFollowerAI.cpp
)

if(SCRIPTS)
  include(Custom/CMakeLists.txt)
  include(World/CMakeLists.txt)
  include(OutdoorPvP/CMakeLists.txt)
  include(EasternKingdoms/CMakeLists.txt)
  include(Kalimdor/CMakeLists.txt)
  include(Outland/CMakeLists.txt)
  include(Northrend/CMakeLists.txt)
  include(Maelstrom/CMakeLists.txt)
  include(Events/CMakeLists.txt)
  include(Pandaria/CMakeLists.txt)
  include(Pet/CMakeLists.txt)
endif()

message(STATUS "SCRIPT PREPARATION COMPLETE")
message("")

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/src/serveur/commun
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Configuration
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Cryptography
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Database
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/DataStores
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Debugging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Dynamic
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Logging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Packets
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Threading
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Utilities
  ${CMAKE_SOURCE_DIR}/src/serveur/collision
  ${CMAKE_SOURCE_DIR}/src/serveur/collision/Management
  ${CMAKE_SOURCE_DIR}/src/serveur/collision/Models
  ${CMAKE_SOURCE_DIR}/src/serveur/commun
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Database
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Accounts
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Achievements
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Addons
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI/CoreAI
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI/ScriptedAI
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI/SmartScripts
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AuctionHouse
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Battlefield
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Battlefield/Zones
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Battlegrounds
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Battlegrounds/Zones
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Calendar
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Chat
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Chat/Channels
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Conditions
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Configuration
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Combat
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/DataStores
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/DungeonFinding
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/AreaTrigger
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Corpse
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Creature
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/DynamicObject
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Item
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Item/Container
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/GameObject
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Object
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Object/Updates
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Pet
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Player
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Transport
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Unit
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Vehicle
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Events
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Globals
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids/Cells
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids/Notifiers
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Groups
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Guilds
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Handlers
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Instances
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/LookingForGroup
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Loot
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Mails
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Miscellaneous
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Maps
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/MovementGenerators
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/Spline
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/Waypoints
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Opcodes
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/OutdoorPvP
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Pools
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/PrecompiledHeaders
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Quests
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Reputation
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Scripting
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Server
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Server/Protocol
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Skills
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Spells
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Spells/Auras
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Texts
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Tickets
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Tools
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Warden
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Warden/Modules
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Weather
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/World
  ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
)

add_library(scripts STATIC
  ${scripts_STAT_SRCS}
  ${scripts_STAT_PCH_SRC}
)

add_dependencies(scripts revision.h)

# Generate precompiled header
if (USE_SCRIPTPCH)
  add_cxx_pch(scripts ${scripts_STAT_PCH_HDR} ${scripts_STAT_PCH_SRC})
endif()
