#include "affichage.h"
#include "init.h"
#include <allegro.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

static void texte_centre_geant(int x, int y, const char* t, int r, int g, int b, float echelle)
{
    int largeur_normale = strlen(t) * 8;
    int hauteur_normale = 8;
    int nouvelle_largeur = (int)(largeur_normale * echelle);
    int nouvelle_hauteur = (int)(hauteur_normale * echelle);

    BITMAP* temp = create_bitmap(largeur_normale + 2, hauteur_normale + 2);
    clear_to_color(temp, makecol(255, 0, 255));

    textout_ex(temp, font, t, 1, 1, makecol(0, 0, 0), -1);
    textout_ex(temp, font, t, 0, 0, makecol(r, g, b), -1);

    stretch_sprite(page, temp, x - (nouvelle_largeur / 2), y - (nouvelle_hauteur / 2), nouvelle_largeur, nouvelle_hauteur);

    destroy_bitmap(temp);
}

int g_tick = 0;
int g_shake = 0;

void declencher_shake(int force)
{
    if (force > g_shake) {
        g_shake = force;
    }
}

static void texte_ombre(int x, int y, const char* t, int r, int g, int b)
{
    textout_ex(page, font, t, x + 2, y + 2, makecol(0, 0, 0), -1);
    textout_ex(page, font, t, x, y, makecol(r, g, b), -1);
}

static void texte_centre_ombre(int x, int y, const char* t, int r, int g, int b)
{
    textout_centre_ex(page, font, t, x + 2, y + 2, makecol(0, 0, 0), -1);
    textout_centre_ex(page, font, t, x, y, makecol(r, g, b), -1);
}

static void degrade_vertical(int y1, int y2, int r1, int g1, int b1, int r2, int g2, int b2)
{
    int y;
    int r, g, b;
    float t;

    for (y = y1; y < y2; y++) {
        t = (float)(y - y1) / (float)(y2 - y1);
        r = (int)(r1 + (r2 - r1) * t);
        g = (int)(g1 + (g2 - g1) * t);
        b = (int)(b1 + (b2 - b1) * t);
        hline(page, 0, y, LARGEUR, makecol(r, g, b));
    }
}

static void dessiner_fond_proportionnel(BITMAP* bmp)
{
    float ratio_w = (float)LARGEUR / bmp->w;
    float ratio_h = (float)HAUTEUR / bmp->h;
    float ratio = (ratio_w > ratio_h) ? ratio_w : ratio_h;
    int new_w = bmp->w * ratio;
    int new_h = bmp->h * ratio;
    int offset_x = (LARGEUR - new_w) / 2;
    int offset_y = (HAUTEUR - new_h) / 2;
    stretch_blit(bmp, page, 0, 0, bmp->w, bmp->h, offset_x, offset_y, new_w, new_h);
}

void afficher_fond(int niveau, Etoile* etoiles)
{
    int i;
    int br;
    int idx;

    idx = niveau - 1;
    if (idx < 0) idx = 0;
    if (idx > 3) idx = 3;

    dessiner_fond_proportionnel(fonds[idx]);

    for (i = 0; i < MAX_ETOILES; i++) {
        br = 150 + (int)(105 * sin((g_tick + etoiles[i].phase * 5) * 0.05));
        if (br < 100) br = 100;
        if (br > 255) br = 255;
        circlefill(page, etoiles[i].x, etoiles[i].y, etoiles[i].taille, makecol(br, br, br));
    }

    rectfill(page, 0, SOL, LARGEUR, SOL + 6, makecol(30, 30, 50));
    rectfill(page, 0, SOL + 6, LARGEUR, BAS_HUD, makecol(50, 30, 70));
}

void afficher_joueur(Joueur* j)
{
    BITMAP* sprite;
    sprite = NULL;
    if (j->invincibilite > 0 && (g_tick / 5) % 2 == 0) {
        return;
    }
    if (key[KEY_Q] || key[KEY_W] || key[KEY_X]) {
        if (j->direction == 0) {
            sprite = saut_droite[0];
        } else {
            sprite = saut_gauche[0];
        }
    }
    else if (j->saut == 1) {
        if (j->direction == 0) {
            sprite = saut_droite[j->frame % 2];
        } else {
            sprite = saut_gauche[j->frame % 2];
        }
    } else {
        if (j->direction == 0) {
            sprite = marche_droite[j->frame];
        } else {
            sprite = marche_gauche[j->frame];
        }
    }

    if (sprite != NULL) {
        stretch_sprite(page, sprite, j->x, j->y, j->tx, j->ty);
    } else {
        rectfill(page, j->x, j->y, j->x + j->tx, j->y + j->ty, makecol(80, 180, 220));
    }
}

void afficher_mort(Joueur* j)
{
    if (mort_img != NULL) {
        stretch_sprite(page, mort_img, j->x, j->y, j->tx, j->ty);
    } else {
        rectfill(page, j->x, j->y, j->x + j->tx, j->y + j->ty, makecol(180, 30, 30));
        line(page, j->x, j->y, j->x + j->tx, j->y + j->ty, makecol(255, 255, 255));
        line(page, j->x + j->tx, j->y, j->x, j->y + j->ty, makecol(255, 255, 255));
    }
}

void afficher_bulles(Bulle* b, int niveau)
{
    int i;
    int x, y, r;
    int cr, cg, cb;
    int idx_niv = niveau - 1;
    if(idx_niv < 0) idx_niv = 0;
    if(idx_niv > 3) idx_niv = 3;

    for (i = 0; i < MAX_BULLES; i++) {
        if (b[i].active == 1) {
            stretch_sprite(page, bulles_img[idx_niv][b[i].taille], (int)b[i].x, (int)b[i].y, b[i].tx, b[i].ty);

        }
    }
}
void afficher_tirs(Projectile* t)
{
    int i;
    int cx;
    for (i = 0; i < MAX_TIRS; i++) {
        if (t[i].active == 1) {
            stretch_sprite(page, tir_img[t[i].frame], t[i].x, t[i].y, t[i].tx, t[i].ty);
        }
    }
}


void afficher_popups(PopupScore* p)
{
    int i;
    char texte[20];
    for (i = 0; i < MAX_POPUPS; i++) {
        if (p[i].active == 1) {
            sprintf(texte, "+%d", p[i].valeur);
            if (p[i].valeur >= 200) {
                texte_centre_ombre(p[i].x, p[i].y, texte, 255, 100, 200);
            } else if (p[i].valeur >= 100) {
                texte_centre_ombre(p[i].x, p[i].y, texte, 255, 220, 50);
            } else {
                texte_centre_ombre(p[i].x, p[i].y, texte, 255, 255, 255);
            }
        }
    }
}

void afficher_boss(Boss* b)
{
    BITMAP* sprite;
    int cx, cy;
    int rouge;

    if (b->active == 0) return;

    cx = (int)b->x + b->tx / 2;
    cy = (int)b->y + b->ty / 2;

    rouge = 0;
    if (b->touche > 0) rouge = 100;

    if (b->direction == 1) {
        sprite = boss_g;
    } else {
        sprite = boss_d;
    }
    stretch_sprite(page, sprite, (int)b->x, (int)b->y, b->tx, b->ty);

}

void afficher_bonus(Bonus* b)
{
    int i;
    int idx;
    for (i = 0; i < MAX_BONUS; i++) {
        if (b[i].active == 1) {
            idx = b[i].type;
            if (idx < 0) idx = 0;
            if (idx > 2) idx = 2;
            if (bonus_img[idx] != NULL) {
                stretch_sprite(page, bonus_img[idx], (int)b[i].x, (int)b[i].y, b[i].tx, b[i].ty);
            }
        }
    }
}

void afficher_explosions(Explosion* e)
{
    int i;
    int idx;
    for (i = 0; i < MAX_EXPLOSIONS; i++) {
        if (e[i].active == 1) {
            idx = e[i].frame;
            if (idx < 0) idx = 0;
            if (idx > 2) idx = 2;
            stretch_sprite(page, explosion_img[idx], e[i].x, e[i].y, e[i].tx, e[i].ty);


        }
    }
}

void afficher_tirs_boss(TirBoss* t)
{
    int i;
    int idx;
    for (i = 0; i < MAX_TIRS_BOSS; i++) {
        if (t[i].active == 1) {
            idx = t[i].frame;
            if (idx < 0) idx = 0;
            if (idx > 2) idx = 2;
            stretch_sprite(page, tirboss_img[idx], (int)t[i].x, (int)t[i].y, t[i].tx, t[i].ty);
            }
    }
}

void afficher_barre_boss(Boss* b)
{
    int largeur_barre;
    int x1, y1;

    if (b->active == 0) return;

    largeur_barre = (b->vie * 300) / b->vie_max;

    x1 = LARGEUR / 2 - 150;
    y1 = HAUT_HUD + 5;

    rectfill(page, x1 - 2, y1 - 2, x1 + 302, y1 + 16, makecol(0, 0, 0));
    rectfill(page, x1, y1, x1 + 300, y1 + 14, makecol(60, 60, 60));

    if (largeur_barre > 0) {
        rectfill(page, x1, y1, x1 + largeur_barre, y1 + 14, makecol(0, 128, 0));
    }
    rect(page, x1 - 1, y1 - 1, x1 + 301, y1 + 15, makecol(255, 255, 255));

    texte_centre_ombre(LARGEUR / 2, y1 + 18, "BOSS", 0, 128, 0);
}

void afficher_hud(char* pseudo, int score, int temps, int niveau, int combo)
{
    char texte[100];
    int largeur_temps;
    int couleur_temps_r, couleur_temps_g, couleur_temps_b;

    rectfill(page, 0, 0, LARGEUR, HAUT_HUD, makecol(15, 15, 30));
    rectfill(page, 0, HAUT_HUD - 4, LARGEUR, HAUT_HUD, makecol(80, 100, 200));

    sprintf(texte, "Joueur: %s", pseudo);
    texte_ombre(20, 15, texte, 255, 255, 255);

    sprintf(texte, "Niveau %d", niveau);
    texte_ombre(20, 40, texte, 255, 220, 50);

    sprintf(texte, "SCORE  %06d", score);
    texte_centre_ombre(LARGEUR / 2, 15, texte, 100, 255, 100);

    if (combo > 1) {
        sprintf(texte, "x%d  COMBO !", combo);
        texte_centre_ombre(LARGEUR / 2, 40, texte, 255, 100, 50);
    }

    largeur_temps = temps * 200 / 60;
    if (largeur_temps < 0) largeur_temps = 0;
    if (largeur_temps > 200) largeur_temps = 200;

    if (temps > 30) {
        couleur_temps_r = 50; couleur_temps_g = 220; couleur_temps_b = 50;
    } else if (temps > 15) {
        couleur_temps_r = 230; couleur_temps_g = 200; couleur_temps_b = 30;
    } else {
        couleur_temps_r = 230; couleur_temps_g = 50; couleur_temps_b = 50;
    }

    rectfill(page, LARGEUR - 230, 22, LARGEUR - 25, 38, makecol(50, 50, 50));
    rectfill(page, LARGEUR - 230, 22, LARGEUR - 230 + largeur_temps, 38,
             makecol(couleur_temps_r, couleur_temps_g, couleur_temps_b));
    rect(page, LARGEUR - 231, 21, LARGEUR - 24, 39, makecol(255, 255, 255));

    sprintf(texte, "TEMPS %02d", temps);
    texte_ombre(LARGEUR - 220, 5, texte, 255, 255, 255);

    rectfill(page, 0, BAS_HUD, LARGEUR, HAUTEUR, makecol(15, 15, 30));
    rectfill(page, 0, BAS_HUD, LARGEUR, BAS_HUD + 4, makecol(80, 100, 200));
    texte_centre_ombre(LARGEUR / 2, BAS_HUD + 18,
                       "FLECHES : bouger    ESPACE : sauter    Q : tirer    ECHAP : quitter",
                       200, 200, 220);
}

void afficher_ecran(void)
{
    int dx, dy;

    g_tick = g_tick + 1;

    if (g_shake > 0) {
        dx = (rand() % (g_shake * 2 + 1)) - g_shake;
        dy = (rand() % (g_shake * 2 + 1)) - g_shake;
        clear_to_color(screen, makecol(0, 0, 0));
        blit(page, screen, 0, 0, dx, dy, LARGEUR, HAUTEUR);
        g_shake = g_shake - 1;
    } else {
        blit(page, screen, 0, 0, 0, 0, LARGEUR, HAUTEUR);
    }
}

void afficher_victoire(int score)
{
    char texte[100];
    int sortir;

    sortir = 0;
    while (key[KEY_ENTER]) { rest(10); }

    while (sortir == 0) {
        degrade_vertical(0, HAUTEUR, 20, 30, 80, 60, 180, 80);

        if (img_victoire != NULL) {
            stretch_sprite(page, img_victoire, LARGEUR/4, HAUTEUR/4, LARGEUR/2, HAUTEUR/2);
        }

        sprintf(texte, "Score final : %d", score);
        texte_centre_ombre(LARGEUR / 2, HAUTEUR - 150, texte, 100, 255, 100);

        texte_centre_ombre(LARGEUR / 2, HAUTEUR - 80,
                           "Appuie sur ENTREE pour revenir au menu", 200, 200, 200);

        afficher_ecran();
        if (key[KEY_ENTER]) sortir = 1;
        rest(20);
    }

    while (key[KEY_ENTER]) { rest(10); }
}

void afficher_defaite(int score)
{
    char texte[100];
    int sortir;

    sortir = 0;
    while (key[KEY_ENTER]) { rest(10); }

    while (sortir == 0) {
        degrade_vertical(0, HAUTEUR, 60, 10, 10, 130, 20, 30);

        if (img_defaite != NULL) {
            stretch_sprite(page, img_defaite, LARGEUR/4, HAUTEUR/4, LARGEUR/2, HAUTEUR/2);
        }

        sprintf(texte, "Score : %d", score);
        texte_centre_ombre(LARGEUR / 2, HAUTEUR - 150, texte, 255, 200, 100);

        texte_centre_ombre(LARGEUR / 2, HAUTEUR - 100,
                           "Appuie sur ENTREE pour revenir au menu", 200, 200, 200);

        afficher_ecran();
        if (key[KEY_ENTER]) sortir = 1;
        rest(20);
    }

    while (key[KEY_ENTER]) { rest(10); }
}

void afficher_decompte(int chiffre)
{
    char texte[10];
    sprintf(texte, "%d", chiffre);

    texte_centre_geant(LARGEUR / 2, HAUTEUR / 2, texte, 255, 255, 0, 8.0);

    texte_centre_geant(LARGEUR / 2, HAUTEUR / 2 + 60, "PRET ?", 255, 255, 255, 3.0);
}
