#ifndef INIT_H
#define INIT_H

#include <allegro.h>
#include "entites.h"

extern BITMAP* page;

extern BITMAP* fonds[4];

extern BITMAP* img_victoire;
extern BITMAP* img_defaite;

extern BITMAP* marche_droite[3];
extern BITMAP* marche_gauche[3];
extern BITMAP* saut_haut[3];
extern BITMAP* saut_droite[2];
extern BITMAP* saut_gauche[2];
extern BITMAP* mort_img;

extern BITMAP* bulles_img[3];

extern BITMAP* tir_img[3];

extern BITMAP* explosion_img[3];

extern BITMAP* bonus_img[3];

extern BITMAP* boss_g;
extern BITMAP* boss_d;

extern BITMAP* tirboss_img[3];

void init_allegro(void);

void charger_images(void);

void liberer_images(void);

#endif
