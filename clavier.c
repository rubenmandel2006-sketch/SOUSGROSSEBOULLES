#include "clavier.h"
#include <allegro.h>

// Deplace le joueur avec les fleches gauche/droite et fait avancer l'animation
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

    // Si on a bouge, on fait avancer l'animation toutes les 4 frames
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

    // On empeche le joueur de sortir de l'ecran
    if (j->x < 0) {
        j->x = 0;
    }
    if (j->x + j->tx > LARGEUR) {
        j->x = LARGEUR - j->tx;
    }
}

// Gere le saut : on appuie sur ESPACE et le joueur monte puis retombe avec la gravite
void gerer_saut(Joueur* j)
{
    // On lance le saut seulement si on est pas deja en train de sauter
    if (key[KEY_SPACE] && j->saut == 0) {
        j->saut = 1;
        j->vy = -12.0;
    }

    if (j->saut == 1) {
        j->y = j->y + (int)j->vy;
        // Gravite : on augmente vy a chaque frame pour faire retomber le joueur
        j->vy = j->vy + 0.55;

        // Quand on touche le sol on arrete le saut
        if (j->y >= SOL - j->ty) {
            j->y = SOL - j->ty;
            j->saut = 0;
            j->vy = 0;
        }
    }
}

// Gere le tir avec la touche W : 1 tir, 2 ou 3 selon le bonus actif
void gerer_tir(Joueur* j, Projectile* tirs)
{
    // static pour garder la valeur entre les appels (anti rafale)
    static int dernier = 0;
    int cx;

    // On tire seulement si on appuie ET qu'on a relache la touche avant
    if (key[KEY_W] && dernier == 0) {
        cx = j->x + j->tx / 2;
        // Bonus type 1 : 2 tirs en parallele
        if (j->bonus_timer > 0 && j->bonus_type == 1) {
            ajouter_tir(tirs, cx - 25, j->y);
            ajouter_tir(tirs, cx + 15, j->y);
        // Bonus type 2 : 3 tirs en parallele
        } else if (j->bonus_timer > 0 && j->bonus_type == 2) {
            ajouter_tir(tirs, cx - 35, j->y);
            ajouter_tir(tirs, cx - 5, j->y);
            ajouter_tir(tirs, cx + 25, j->y);
        // Tir normal
        } else {
            ajouter_tir(tirs, cx - 4, j->y);
        }
        dernier = 1;
    }

    // Quand on relache la touche, on remet a 0 pour pouvoir retirer
    if (!key[KEY_W]) {
        dernier = 0;
    }
}
