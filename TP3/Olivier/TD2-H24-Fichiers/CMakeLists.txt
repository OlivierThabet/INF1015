cmake_minimum_required(VERSION 3.10)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
include(cmake_cours.cmake)  # Certaines fonctions de configuration de compilation pour ce cours.

# Le nom du projet.
project(ProjetTD2 CXX)

afficher_infos_generateur()  # Doit être après "project( )".
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fsigned-char -Wall -Wextra -Wpedantic -Woverloaded-virtual -Wno-unknown-pragmas -Wno-enum-compare --coverage")
set(BILIOTHEQUE_COURS
	"bibliotheque_cours/bibliotheque_cours.cpp"
	"include/bibliotheque_cours.hpp"
	"include/CodeCoverage_bugfix.h"
	"bibliotheque_cours/verification_allocation.cpp"
	"include/verification_allocation.hpp"
	"include/debogage_memoire.hpp"
)

# On met les fichiers sources (incluant les entêtes)
file(GLOB ALL_FILES
	*.cpp
	*.hpp
	${BILIOTHEQUE_COURS}
)

configurer_options_compilation_globales()  # Doit être avant le "add_executable".
add_executable(${PROJECT_NAME} ${ALL_FILES})

# On ajoute le dossier include/ fourni, comme des fichiers d'inclusion du système.
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE "include")


# Tous les package Vcpkg. Pour savoir quoi mettre, on fait "vcpkg install <le package>", puis on met ce qu'ils disent de mettre dans le CMakeLists.
# find_package(cppitertools CONFIG REQUIRED)  # On pourrait prendre le cppitertools de vcpkg au lieu de celui fourni dans include/
# find_package(Microsoft.GSL CONFIG  REQUIRED)  # Et GSL, noter que son nom de package est "ms-gsl" (le package "gsl" dans vcpkg est le "GNU Scientific Library" et non le "Guidelines Support Library").
