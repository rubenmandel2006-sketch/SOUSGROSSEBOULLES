#ifndef CLAVIER_H
#define CLAVIER_H

#include "entites.h"

// Deplace le joueur a gauche/droite avec les fleches
void deplacer_joueur(Joueur* j);

// Gere le tir avec la touche W (et les bonus de double/triple tir)
void gerer_tir(Joueur* j, Projectile* tirs);

// Gere le saut avec ESPACE et la gravite quand on est en l'air
void gerer_saut(Joueur* j);

#endif
