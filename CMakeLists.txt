cmake_minimum_required(VERSION 4.1)
project(SOUSGROSSEBOULLES C)

set(CMAKE_C_STANDARD 99)

add_executable(SOUSGROSSEBOULLES main.c
        jeu.c
        jeu.h
        sauvegarde.c
        sauvegarde.h
        entites.c
        entites.h
        clavier.c
        clavier.h
        affichage.c
        affichage.h
        init.c
        init.h
        main.c
        menu.c
        menu.h)
target_link_libraries(SOUSGROSSEBOULLES -lmingw32 -lalleg44)
