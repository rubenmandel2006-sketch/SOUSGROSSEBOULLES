#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "entites.h"

extern int g_tick;

extern int g_shake;

void declencher_shake(int force);

void afficher_fond(int niveau, Etoile* etoiles);
void afficher_joueur(Joueur* j);
void afficher_bulles(Bulle* b, int niveau);
void afficher_tirs(Projectile* t);
void afficher_particules(Particule* p);
void afficher_popups(PopupScore* p);
void afficher_boss(Boss* b);
void afficher_bonus(Bonus* b);
void afficher_explosions(Explosion* e);
void afficher_tirs_boss(TirBoss* t);
void afficher_mort(Joueur* j);

void afficher_hud(char* pseudo, int score, int temps, int niveau, int combo);
void afficher_barre_boss(Boss* b);

void afficher_ecran(void);

void afficher_victoire(int score);
void afficher_defaite(int score);

void afficher_decompte(int chiffre);

#endif
