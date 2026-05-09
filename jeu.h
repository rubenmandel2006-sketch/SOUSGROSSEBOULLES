#ifndef JEU_H
#define JEU_H

#include "entites.h"

// Lance la partie complete (les 3 niveaux + le boss)
int jouer_partie(char* pseudo, int niveau_depart);

// Lance un niveau classique, retourne 1 si gagne, 0 si perdu, -1 si echap
int jouer_niveau(char* pseudo, int niveau, int* score);

// Lance le combat contre le boss
int jouer_boss(char* pseudo, int* score);

// Toutes ces fonctions font bouger les objets a chaque image
void bouger_bulles(Bulle* b);

void bouger_tirs(Projectile* t);

void bouger_particules(Particule* p);

void bouger_popups(PopupScore* p);

void bouger_bonus(Bonus* b);

void bouger_explosions(Explosion* e);

void bouger_tirs_boss(TirBoss* t);

// Gere les collisions entre les tirs et les bulles, et donne les points
void gerer_collisions(Projectile* t, Bulle* b, Particule* part,
                      PopupScore* pop, Explosion* exp, Bonus* bons,
                      int* score, int* combo, int* tick_combo);

// Gere ce qui se passe quand le joueur ramasse un bonus
void gerer_bonus_collecte(Bonus* bons, Joueur* j, Bulle* bulles,
                          Particule* part, Explosion* exp,
                          PopupScore* pop, int* score);

// Place les bulles de depart selon le niveau
void init_bulles_niveau(Bulle* b, int niveau);

// Compte combien de bulles sont encore en vie
int compter_bulles(Bulle* b);

// Gere le mouvement et les attaques du boss
void bouger_boss(Boss* b, Bulle* bulles, TirBoss* tirs_boss);

#endif
