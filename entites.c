#include "entites.h"
#include <stdlib.h>
#include <math.h>

// Met les valeurs de depart pour le joueur
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

// Cree le tableau des bulles et met tout a inactif
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

// Cree le tableau des tirs
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

// Cree le tableau des particules
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

// Cree le tableau des popups de score
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

// Cree les etoiles du fond avec une position aleatoire
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

// Cree le boss (un seul donc pas de tableau)
Boss* creer_boss(void)
{
    Boss* b;
    b = (Boss*)malloc(sizeof(Boss));
    b->active = 0;
    return b;
}

// Cree le tableau des bonus
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

// Cree le tableau des explosions
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

// Cree le tableau des tirs du boss
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

// Petites fonctions qui liberent la memoire de chaque tableau
void detruire_bulles(Bulle* b)         { free(b); }
void detruire_tirs(Projectile* t)      { free(t); }
void detruire_particules(Particule* p) { free(p); }
void detruire_popups(PopupScore* p)    { free(p); }
void detruire_etoiles(Etoile* e)       { free(e); }
void detruire_boss(Boss* b)            { free(b); }
void detruire_bonus(Bonus* b)          { free(b); }
void detruire_explosions(Explosion* e) { free(e); }
void detruire_tirs_boss(TirBoss* t)    { free(t); }

// Cherche une place libre dans le tableau et y met une nouvelle bulle
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
            // La taille de l'image change selon le type de bulle
            if (taille == 0) { tab[i].tx = 60;  tab[i].ty = 60;  }
            if (taille == 1) { tab[i].tx = 120; tab[i].ty = 120; }
            if (taille == 2) { tab[i].tx = 180; tab[i].ty = 180; }
            // Direction de depart : droite ou gauche
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

// Ajoute un tir a la premiere place libre
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

// Cree 18 particules qui partent dans tous les sens
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
            // On choisit un angle au hasard pour que la particule parte dans une direction aleatoire
            angle = (rand() % 360) * 3.14159 / 180.0;
            vitesse = 2.0 + (rand() % 50) / 10.0;
            // cos et sin pour transformer l'angle en vitesse x et y
            tab[i].vx = cos(angle) * vitesse;
            tab[i].vy = sin(angle) * vitesse;
            tab[i].vie = 30 + rand() % 25;
            tab[i].taille = 2 + rand() % 4;
            // On varie un peu la couleur pour que ca fasse plus naturel
            tab[i].r = r + (rand() % 60) - 30;
            tab[i].g = g + (rand() % 60) - 30;
            tab[i].b = b + (rand() % 60) - 30;
            // On s'assure que les couleurs restent entre 0 et 255
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

// Ajoute un popup de score a la premiere place libre
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

// Ajoute un bonus a la premiere place libre
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

// Ajoute une explosion a la premiere place libre
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

// Ajoute un tir du boss avec une direction aleatoire
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
            // On tire a gauche ou a droite au hasard
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

// Met les valeurs de depart pour le boss
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

// Verifie si une bulle touche le joueur (boite de collision rectangulaire)
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

// Verifie si un tir touche une bulle
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

// Verifie si un tir touche le boss
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

// Verifie si le boss touche le joueur
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

// Verifie si le joueur ramasse un bonus
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

// Verifie si un tir du boss touche le joueur
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
