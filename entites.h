#ifndef ENTITES_H
#define ENTITES_H

#include <allegro.h>

#define LARGEUR 1024
#define HAUTEUR 768

#define HAUT_HUD 70
#define BAS_HUD  710
#define SOL      BAS_HUD
#define PLAFOND  HAUT_HUD

#define MAX_BULLES     60
#define MAX_TIRS       30
#define MAX_PARTICULES 250
#define MAX_POPUPS     20
#define MAX_ETOILES    80
#define MAX_BONUS      10
#define MAX_EXPLOSIONS 25
#define MAX_TIRS_BOSS  15

typedef struct {
    int x, y;
    int tx, ty;
    int vie;
    int direction;
    int saut;
    float vy;
    int frame;
    int compteur;
    int mort;
    int bonus_type;
    int bonus_timer;
} Joueur;

typedef struct {
    float x, y;
    int tx, ty;
    float vx, vy;
    int taille;
    int active;
    int couleur;
} Bulle;

typedef struct {
    int x, y;
    int tx, ty;
    int active;
    int frame;
    int compteur;
} Projectile;

typedef struct {
    float x, y;
    int tx, ty;
    int vie;
    int vie_max;
    float vx;
    int active;
    int compteur_tir;
    int frame;
    int compteur;
    int touche;
    int direction;
} Boss;

typedef struct {
    float x, y;
    float vx, vy;
    int vie;
    int taille;
    int r, g, b;
    int active;
} Particule;

typedef struct {
    int x, y;
    int valeur;
    int vie;
    int active;
    int couleur;
} PopupScore;

typedef struct {
    int x, y;
    int taille;
    int phase;
} Etoile;

typedef struct {
    float x, y;
    int tx, ty;
    float vy;
    int type;
    int active;
} Bonus;

typedef struct {
    int x, y;
    int tx, ty;
    int frame;
    int compteur;
    int active;
} Explosion;

typedef struct {
    float x, y;
    int tx, ty;
    float vx, vy;
    int frame;
    int compteur;
    int active;
} TirBoss;

void init_joueur(Joueur* j);

Bulle* creer_bulles(void);
Projectile* creer_tirs(void);
Particule* creer_particules(void);
PopupScore* creer_popups(void);
Etoile* creer_etoiles(void);
Boss* creer_boss(void);
Bonus* creer_bonus(void);
Explosion* creer_explosions(void);
TirBoss* creer_tirs_boss(void);

void detruire_bulles(Bulle* b);
void detruire_tirs(Projectile* t);
void detruire_particules(Particule* p);
void detruire_popups(PopupScore* p);
void detruire_etoiles(Etoile* e);
void detruire_boss(Boss* b);
void detruire_bonus(Bonus* b);
void detruire_explosions(Explosion* e);
void detruire_tirs_boss(TirBoss* t);

void ajouter_bulle(Bulle* tab, int x, int y, int taille, int dir, int couleur);
void ajouter_tir(Projectile* tab, int x, int y);
void exploser_particules(Particule* tab, int x, int y, int r, int g, int b);
void ajouter_popup(PopupScore* tab, int x, int y, int valeur);
void ajouter_bonus(Bonus* tab, int x, int y, int type);
void ajouter_explosion(Explosion* tab, int x, int y, int taille);
void ajouter_tir_boss(TirBoss* tab, int x, int y);

int collision_bulle_joueur(Bulle* b, Joueur* j);
int collision_tir_bulle(Projectile* t, Bulle* b);
int collision_tir_boss(Projectile* t, Boss* b);
int collision_boss_joueur(Boss* b, Joueur* j);
int collision_bonus_joueur(Bonus* b, Joueur* j);
int collision_tirboss_joueur(TirBoss* t, Joueur* j);

void init_boss(Boss* b);

#endif
