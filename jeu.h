#ifndef JEU_H
#define JEU_H

#include "entites.h"

int jouer_partie(char* pseudo, int niveau_depart);

int jouer_niveau(char* pseudo, int niveau, int* score);

int jouer_boss(char* pseudo, int* score);

void bouger_bulles(Bulle* b);

void bouger_tirs(Projectile* t);

void bouger_particules(Particule* p);

void bouger_popups(PopupScore* p);

void bouger_bonus(Bonus* b);

void bouger_explosions(Explosion* e);

void bouger_tirs_boss(TirBoss* t);

void gerer_collisions(Projectile* t, Bulle* b, Particule* part,
                      PopupScore* pop, Explosion* exp, Bonus* bons,
                      int* score, int* combo, int* tick_combo);

void gerer_bonus_collecte(Bonus* bons, Joueur* j, Bulle* bulles,
                          Particule* part, Explosion* exp,
                          PopupScore* pop, int* score);

void init_bulles_niveau(Bulle* b, int niveau);

int compter_bulles(Bulle* b);

void bouger_boss(Boss* b, Bulle* bulles, TirBoss* tirs_boss);

#endif
