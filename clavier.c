#include "clavier.h"
#include <allegro.h>

void deplacer_joueur(Joueur* j)
{
    int bouge;
    bouge = 0;

    if (key[KEY_RIGHT]) {
        j->x = j->x + 15;
        j->direction = 0;
        bouge = 1;
    }

    if (key[KEY_LEFT]) {
        j->x = j->x - 15;
        j->direction = 1;
        bouge = 1;
    }

    if (bouge == 1) {
        j->compteur = j->compteur + 1;
        if (j->compteur > 4) {
            j->frame = j->frame + 1;
            if (j->frame > 2) {
                j->frame = 0;
            }
            j->compteur = 0;
        }
    }

    if (j->x < 0) {
        j->x = 0;
    }
    if (j->x + j->tx > LARGEUR) {
        j->x = LARGEUR - j->tx;
    }
}

void gerer_saut(Joueur* j)
{
    if (key[KEY_SPACE] && j->saut == 0) {
        j->saut = 1;
        j->vy = -12.0;
    }

    if (j->saut == 1) {
        j->y = j->y + (int)j->vy;
        j->vy = j->vy + 0.55;

        if (j->y >= SOL - j->ty) {
            j->y = SOL - j->ty;
            j->saut = 0;
            j->vy = 0;
        }
    }
}

void gerer_tir(Joueur* j, Projectile* tirs)
{
    static int dernier = 0;
    int cx;

    if (key[KEY_W] && dernier == 0) {
        cx = j->x + j->tx / 2;
        if (j->bonus_timer > 0 && j->bonus_type == 1) {
            ajouter_tir(tirs, cx - 25, j->y);
            ajouter_tir(tirs, cx + 15, j->y);
        } else if (j->bonus_timer > 0 && j->bonus_type == 2) {
            ajouter_tir(tirs, cx - 35, j->y);
            ajouter_tir(tirs, cx - 5, j->y);
            ajouter_tir(tirs, cx + 25, j->y);
        } else {
            ajouter_tir(tirs, cx - 4, j->y);
        }
        dernier = 1;
    }

    if (!key[KEY_W]) {
        dernier = 0;
    }
}
