#ifndef INIT_H
#define INIT_H

#include <allegro.h>
#include "entites.h"

// Buffer ou on dessine tout avant de l'afficher (double buffering)
extern BITMAP* page;

// Images de fond pour les 4 niveaux
extern BITMAP* fonds[4];

// Images des ecrans victoire/defaite
extern BITMAP* img_victoire;
extern BITMAP* img_defaite;

// Images du joueur (animation a 3 frames pour la marche, 2 pour le saut)
extern BITMAP* marche_droite[3];
extern BITMAP* marche_gauche[3];
extern BITMAP* saut_haut[3];
extern BITMAP* saut_droite[2];
extern BITMAP* saut_gauche[2];
extern BITMAP* mort_img;

// Images des bulles : [niveau][taille]
extern BITMAP* bulles_img[4][3];

// Images du tir (animation 3 frames)
extern BITMAP* tir_img[3];

// Images de l'explosion (animation 3 frames)
extern BITMAP* explosion_img[3];

// Images des bonus (x2, x3, BOOM)
extern BITMAP* bonus_img[3];

// Images du boss (a gauche et a droite)
extern BITMAP* boss_g;
extern BITMAP* boss_d;

// Images des tirs du boss (animation 3 frames)
extern BITMAP* tirboss_img[3];

// Initialise Allegro (clavier, souris, mode graphique)
void init_allegro(void);

// Charge toutes les images du jeu
void charger_images(void);

// Libere toutes les images quand on quitte
void liberer_images(void);

#endif
