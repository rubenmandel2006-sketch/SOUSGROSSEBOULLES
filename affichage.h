#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "entites.h"

// Compteur global qui s'incremente a chaque image (utile pour les animations)
extern int g_tick;

// Force du shake de l'ecran (effet quand on prend un coup)
extern int g_shake;

// Lance un shake d'ecran avec une certaine force
void declencher_shake(int force);

// Toutes les fonctions d'affichage des objets du jeu
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

// Affiche le HUD (score, temps, niveau, combo)
void afficher_hud(char* pseudo, int score, int temps, int niveau, int combo);
// Affiche la barre de vie du boss
void afficher_barre_boss(Boss* b);

// Copie le buffer page sur l'ecran (avec eventuellement le shake)
void afficher_ecran(void);

// Ecrans victoire et defaite
void afficher_victoire(int score);
void afficher_defaite(int score);

// Affiche un grand chiffre du decompte (3, 2, 1)
void afficher_decompte(int chiffre);

#endif
