#include "entites.h"
#include <stdlib.h>
#include <math.h>

void init_joueur(Joueur* j)
{
    j->x = LARGEUR / 2;
    j->y = SOL - 140;
    j->tx = 100;
    j->ty = 140;
    j->vie = 3;
    j->direction = 0;
    j->saut = 0;
    j->vy = 0;
    j->frame = 0;
    j->compteur = 0;
    j->mort = 0;
    j->bonus_type = 0;
    j->bonus_timer = 0;
    j->invincibilite = 0;
}

Bulle* creer_bulles(void)
{
    Bulle* tab;
    int i;
    tab = (Bulle*)malloc(sizeof(Bulle) * MAX_BULLES);
    for (i = 0; i < MAX_BULLES; i++) {
        tab[i].active = 0;
    }
    return tab;
}

Projectile* creer_tirs(void)
{
    Projectile* tab;
    int i;
    tab = (Projectile*)malloc(sizeof(Projectile) * MAX_TIRS);
    for (i = 0; i < MAX_TIRS; i++) {
        tab[i].active = 0;
        tab[i].tx = 25;
        tab[i].ty = 50;
        tab[i].frame = 0;
        tab[i].compteur = 0;
    }
    return tab;
}

Particule* creer_particules(void)
{
    Particule* tab;
    int i;
    tab = (Particule*)malloc(sizeof(Particule) * MAX_PARTICULES);
    for (i = 0; i < MAX_PARTICULES; i++) {
        tab[i].active = 0;
    }
    return tab;
}

PopupScore* creer_popups(void)
{
    PopupScore* tab;
    int i;
    tab = (PopupScore*)malloc(sizeof(PopupScore) * MAX_POPUPS);
    for (i = 0; i < MAX_POPUPS; i++) {
        tab[i].active = 0;
    }
    return tab;
}

Etoile* creer_etoiles(void)
{
    Etoile* tab;
    int i;
    tab = (Etoile*)malloc(sizeof(Etoile) * MAX_ETOILES);
    for (i = 0; i < MAX_ETOILES; i++) {
        tab[i].x = rand() % LARGEUR;
        tab[i].y = rand() % HAUTEUR;
        tab[i].taille = 1 + rand() % 2;
        tab[i].phase = rand() % 100;
    }
    return tab;
}

Boss* creer_boss(void)
{
    Boss* b;
    b = (Boss*)malloc(sizeof(Boss));
    b->active = 0;
    return b;
}

Bonus* creer_bonus(void)
{
    Bonus* tab;
    int i;
    tab = (Bonus*)malloc(sizeof(Bonus) * MAX_BONUS);
    for (i = 0; i < MAX_BONUS; i++) {
        tab[i].active = 0;
    }
    return tab;
}

Explosion* creer_explosions(void)
{
    Explosion* tab;
    int i;
    tab = (Explosion*)malloc(sizeof(Explosion) * MAX_EXPLOSIONS);
    for (i = 0; i < MAX_EXPLOSIONS; i++) {
        tab[i].active = 0;
    }
    return tab;
}

TirBoss* creer_tirs_boss(void)
{
    TirBoss* tab;
    int i;
    tab = (TirBoss*)malloc(sizeof(TirBoss) * MAX_TIRS_BOSS);
    for (i = 0; i < MAX_TIRS_BOSS; i++) {
        tab[i].active = 0;
    }
    return tab;
}

void detruire_bulles(Bulle* b)         { free(b); }
void detruire_tirs(Projectile* t)      { free(t); }
void detruire_particules(Particule* p) { free(p); }
void detruire_popups(PopupScore* p)    { free(p); }
void detruire_etoiles(Etoile* e)       { free(e); }
void detruire_boss(Boss* b)            { free(b); }
void detruire_bonus(Bonus* b)          { free(b); }
void detruire_explosions(Explosion* e) { free(e); }
void detruire_tirs_boss(TirBoss* t)    { free(t); }

void ajouter_bulle(Bulle* tab, int x, int y, int taille, int dir, int couleur)
{
    int i;
    for (i = 0; i < MAX_BULLES; i++) {
        if (tab[i].active == 0) {
            tab[i].active = 1;
            tab[i].x = x;
            tab[i].y = y;
            tab[i].taille = taille;
            tab[i].couleur = couleur;
            if (taille == 0) { tab[i].tx = 60;  tab[i].ty = 60;  }
            if (taille == 1) { tab[i].tx = 120; tab[i].ty = 120; }
            if (taille == 2) { tab[i].tx = 180; tab[i].ty = 180; }
            if (dir == 0) {
                tab[i].vx = 2.5;
            } else {
                tab[i].vx = -2.5;
            }
            tab[i].vy = -3.5;
            return;
        }
    }
}

void ajouter_tir(Projectile* tab, int x, int y)
{
    int i;
    for (i = 0; i < MAX_TIRS; i++) {
        if (tab[i].active == 0) {
            tab[i].active = 1;
            tab[i].x = x;
            tab[i].y = y;
            tab[i].frame = 0;
            tab[i].compteur = 0;
            return;
        }
    }
}

void exploser_particules(Particule* tab, int x, int y, int r, int g, int b)
{
    int i, n;
    float angle, vitesse;

    n = 0;
    for (i = 0; i < MAX_PARTICULES && n < 18; i++) {
        if (tab[i].active == 0) {
            tab[i].active = 1;
            tab[i].x = x;
            tab[i].y = y;
            angle = (rand() % 360) * 3.14159 / 180.0;
            vitesse = 2.0 + (rand() % 50) / 10.0;
            tab[i].vx = cos(angle) * vitesse;
            tab[i].vy = sin(angle) * vitesse;
            tab[i].vie = 30 + rand() % 25;
            tab[i].taille = 2 + rand() % 4;
            tab[i].r = r + (rand() % 60) - 30;
            tab[i].g = g + (rand() % 60) - 30;
            tab[i].b = b + (rand() % 60) - 30;
            if (tab[i].r < 0) tab[i].r = 0;
            if (tab[i].g < 0) tab[i].g = 0;
            if (tab[i].b < 0) tab[i].b = 0;
            if (tab[i].r > 255) tab[i].r = 255;
            if (tab[i].g > 255) tab[i].g = 255;
            if (tab[i].b > 255) tab[i].b = 255;
            n = n + 1;
        }
    }
}

void ajouter_popup(PopupScore* tab, int x, int y, int valeur)
{
    int i;
    for (i = 0; i < MAX_POPUPS; i++) {
        if (tab[i].active == 0) {
            tab[i].active = 1;
            tab[i].x = x;
            tab[i].y = y;
            tab[i].valeur = valeur;
            tab[i].vie = 60;
            tab[i].couleur = 0;
            return;
        }
    }
}

void ajouter_bonus(Bonus* tab, int x, int y, int type)
{
    int i;
    for (i = 0; i < MAX_BONUS; i++) {
        if (tab[i].active == 0) {
            tab[i].active = 1;
            tab[i].x = x;
            tab[i].y = y;
            tab[i].tx = 50;
            tab[i].ty = 50;
            tab[i].vy = 12.0;
            tab[i].type = type;
            return;
        }
    }
}

void ajouter_explosion(Explosion* tab, int x, int y, int taille)
{
    int i;
    for (i = 0; i < MAX_EXPLOSIONS; i++) {
        if (tab[i].active == 0) {
            tab[i].active = 1;
            tab[i].x = x;
            tab[i].y = y;
            tab[i].tx = taille;
            tab[i].ty = taille;
            tab[i].frame = 0;
            tab[i].compteur = 0;
            return;
        }
    }
}

void ajouter_tir_boss(TirBoss* tab, int x, int y)
{
    int i;
    int dir;
    for (i = 0; i < MAX_TIRS_BOSS; i++) {
        if (tab[i].active == 0) {
            tab[i].active = 1;
            tab[i].x = x;
            tab[i].y = y;
            tab[i].tx = 35;
            tab[i].ty = 35;
            dir = rand() % 2;
            if (dir == 0) {
                tab[i].vx = -3.0;
            } else {
                tab[i].vx = 3.0;
            }
            tab[i].vy = 4.0;
            tab[i].frame = 0;
            tab[i].compteur = 0;
            return;
        }
    }
}

void init_boss(Boss* b)
{
    b->x = LARGEUR / 2 - 70;
    b->y = 120;
    b->tx = 140;
    b->ty = 120;
    b->vie = 12;
    b->vie_max = 12;
    b->vx = 3.0;
    b->active = 1;
    b->compteur_tir = 0;
    b->frame = 0;
    b->compteur = 0;
    b->touche = 0;
    b->direction = 0;
}

int collision_bulle_joueur(Bulle* b, Joueur* j)
{
    if (b->x <= j->x + j->tx &&
        b->x + b->tx >= j->x &&
        b->y <= j->y + j->ty &&
        b->y + b->ty >= j->y) {
        return 1;
    }
    return 0;
}

int collision_tir_bulle(Projectile* t, Bulle* b)
{
    if (t->x <= b->x + b->tx &&
        t->x + t->tx >= b->x &&
        t->y <= b->y + b->ty &&
        t->y + t->ty >= b->y) {
        return 1;
    }
    return 0;
}

int collision_tir_boss(Projectile* t, Boss* b)
{
    if (b->active == 0) return 0;
    if (t->x <= b->x + b->tx &&
        t->x + t->tx >= b->x &&
        t->y <= b->y + b->ty &&
        t->y + t->ty >= b->y) {
        return 1;
    }
    return 0;
}

int collision_boss_joueur(Boss* b, Joueur* j)
{
    if (b->active == 0) return 0;
    if (b->x <= j->x + j->tx &&
        b->x + b->tx >= j->x &&
        b->y <= j->y + j->ty &&
        b->y + b->ty >= j->y) {
        return 1;
    }
    return 0;
}

int collision_bonus_joueur(Bonus* b, Joueur* j)
{
    if (b->active == 0) return 0;
    if (b->x <= j->x + j->tx &&
        b->x + b->tx >= j->x &&
        b->y <= j->y + j->ty &&
        b->y + b->ty >= j->y) {
        return 1;
    }
    return 0;
}

int collision_tirboss_joueur(TirBoss* t, Joueur* j)
{
    if (t->active == 0) return 0;
    if (t->x <= j->x + j->tx &&
        t->x + t->tx >= j->x &&
        t->y <= j->y + j->ty &&
        t->y + t->ty >= j->y) {
        return 1;
    }
    return 0;
}
