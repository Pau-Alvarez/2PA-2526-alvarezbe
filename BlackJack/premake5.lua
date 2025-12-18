
PROJ_DIR = path.getabsolute("./../..")
workspace "BlackJack"
   architecture "x64"
   configurations { "Debug", "Release" }
   startproject "BlackJack"
   location "build"
   libdirs{"."}

project "BlackJack"
kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   files
   {
      "source/interfaces/IGames.h",
      "source/interfaces/IPlayer.h",
      "source/interfaces/ITable.h",
      "source/interfaces/Rules.h",

      "source/includes/PABGame.h",
      "source/includes/PABRules.h",
      "source/src/PABGame.cc",
      "source/includes/PABPlayer.h",
      "source/src/PABPlayer.cc",
      "source/includes/PABTable.h",
      "source/src/PABTable.cc",
      "source/src/main.cc",
      "source/premake5.lua",
   }