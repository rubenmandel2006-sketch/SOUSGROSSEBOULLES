#include "jeu.h"
#include "init.h"
#include "clavier.h"
#include "affichage.h"
#include "sauvegarde.h"
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static int points_bulle(int taille)
{
    if (taille == 2) return 50;
    if (taille == 1) return 100;
    return 200;
}

void init_bulles_niveau(Bulle* b, int niveau)
{
    int i;
    int nb_meteores;
    int espacement_y;
    int pos_x, pos_y;
    int taille;
    int direction;
    int couleur;

    nb_meteores = 0;
    espacement_y = 400;

    for (i = 0; i < MAX_BULLES; i++) {
        b[i].active = 0;
    }

    if (niveau == 1) {
        nb_meteores = 8;
        espacement_y = 500;
    }
    else if (niveau == 2) {
        nb_meteores = 10;
        espacement_y = 400;
    }
    else if (niveau == 3) {
        nb_meteores = 25;
        espacement_y = 380;
    }

    for (i = 0; i < nb_meteores; i++) {
        pos_x = 50 + rand() % (LARGEUR - 100);
        pos_y = 80 - (i * espacement_y);

        taille = 2;
        if (rand() % 3 == 0) {
            taille = 1;
        }

        direction = rand() % 2;
        couleur = rand() % 4;

        ajouter_bulle(b, pos_x, pos_y, taille, direction, couleur);
    }
}

int compter_bulles(Bulle* b)
{
    int i;
    int n;
    n = 0;
    for (i = 0; i < MAX_BULLES; i++) {
        if (b[i].active == 1) {
            n = n + 1;
        }
    }
    return n;
}

void bouger_bulles(Bulle* b)
{
    int i;
    for (i = 0; i < MAX_BULLES; i++) {
        if (b[i].active == 1) {
            b[i].x = b[i].x + b[i].vx;
            b[i].y = b[i].y + b[i].vy;

            b[i].vy = b[i].vy + 0.22;

            if (b[i].vy > 12) {
                b[i].vy = 12;
            }

            if (b[i].x < 0) {
                b[i].x = 0;
                b[i].vx = -b[i].vx;
            }
            if (b[i].x + b[i].tx > LARGEUR) {
                b[i].x = LARGEUR - b[i].tx;
                b[i].vx = -b[i].vx;
            }

            if (b[i].y < PLAFOND && b[i].y > 0 && b[i].vy < 0) {
                b[i].y = PLAFOND;
                b[i].vy = -b[i].vy;
            }

            if (b[i].y + b[i].ty > SOL) {
                b[i].y = SOL - b[i].ty;
                if (b[i].taille == 0) b[i].vy = -15;
                if (b[i].taille == 1) b[i].vy = -14;
                if (b[i].taille == 2) b[i].vy = -13;
            }
        }
    }
}

void bouger_tirs(Projectile* t)
{
    int i;
    for (i = 0; i < MAX_TIRS; i++) {
        if (t[i].active == 1) {
            t[i].y = t[i].y - 10;

            t[i].compteur = t[i].compteur + 1;
            if (t[i].compteur > 3) {
                t[i].compteur = 0;
                t[i].frame = t[i].frame + 1;
                if (t[i].frame > 2) {
                    t[i].frame = 0;
                }
            }

            if (t[i].y < 0) {
                t[i].active = 0;
            }
        }
    }
}

void bouger_particules(Particule* p)
{
    int i;
    for (i = 0; i < MAX_PARTICULES; i++) {
        if (p[i].active == 1) {
            p[i].x = p[i].x + p[i].vx;
            p[i].y = p[i].y + p[i].vy;
            p[i].vy = p[i].vy + 0.18;
            p[i].vie = p[i].vie - 1;
            if (p[i].vie <= 0) {
                p[i].active = 0;
            }
        }
    }
}

void bouger_popups(PopupScore* p)
{
    int i;
    for (i = 0; i < MAX_POPUPS; i++) {
        if (p[i].active == 1) {
            p[i].y = p[i].y - 1;
            p[i].vie = p[i].vie - 1;
            if (p[i].vie <= 0) {
                p[i].active = 0;
            }
        }
    }
}

void bouger_bonus(Bonus* b)
{
    int i;
    for (i = 0; i < MAX_BONUS; i++) {
        if (b[i].active == 1) {
            b[i].y = b[i].y + b[i].vy;
            if (b[i].y > HAUTEUR) {
                b[i].active = 0;
            }
        }
    }
}

void bouger_explosions(Explosion* e)
{
    int i;
    for (i = 0; i < MAX_EXPLOSIONS; i++) {
        if (e[i].active == 1) {
            e[i].compteur = e[i].compteur + 1;
            if (e[i].compteur > 6) {
                e[i].compteur = 0;
                e[i].frame = e[i].frame + 1;
                if (e[i].frame > 2) {
                    e[i].active = 0;
                }
            }
        }
    }
}

void bouger_tirs_boss(TirBoss* t)
{
    int i;
    for (i = 0; i < MAX_TIRS_BOSS; i++) {
        if (t[i].active == 1) {
            t[i].x = t[i].x + t[i].vx;
            t[i].y = t[i].y + t[i].vy;

            if (t[i].x < 0) {
                t[i].x = 0;
                t[i].vx = -t[i].vx;
            }
            if (t[i].x + t[i].tx > LARGEUR) {
                t[i].x = LARGEUR - t[i].tx;
                t[i].vx = -t[i].vx;
            }

            if (t[i].y > HAUTEUR) {
                t[i].active = 0;
            }
            if (t[i].y < 0) {
                t[i].active = 0;
            }

            t[i].compteur = t[i].compteur + 1;
            if (t[i].compteur > 5) {
                t[i].compteur = 0;
                t[i].frame = t[i].frame + 1;
                if (t[i].frame > 2) {
                    t[i].frame = 0;
                }
            }
        }
    }
}

static void couleur_bulle(int c, int* r, int* g, int* bl)
{
    if (c == 0) { *r = 230; *g = 80;  *bl = 80;  }
    else if (c == 1) { *r = 80;  *g = 200; *bl = 90;  }
    else if (c == 2) { *r = 80;  *g = 130; *bl = 230; }
    else { *r = 230; *g = 200; *bl = 60; }
}

void gerer_collisions(Projectile* t, Bulle* b, Particule* part,
                      PopupScore* pop, Explosion* exp, Bonus* bons,
                      int* score, int* combo, int* tick_combo)
{
    int i, j;
    int x, y, taille, couleur;
    int tx;
    int gain;
    int r, g, bl;

    for (i = 0; i < MAX_TIRS; i++) {
        if (t[i].active == 1) {
            for (j = 0; j < MAX_BULLES; j++) {
                if (b[j].active == 1) {
                    if (collision_tir_bulle(&t[i], &b[j]) == 1) {
                        t[i].active = 0;

                        x = (int)b[j].x + b[j].tx / 2;
                        y = (int)b[j].y + b[j].ty / 2;
                        tx = b[j].tx;
                        taille = b[j].taille;
                        couleur = b[j].couleur;

                        b[j].active = 0;

                        *combo = *combo + 1;
                        if (*combo > 10) *combo = 10;
                        *tick_combo = 90;

                        gain = points_bulle(taille) * (*combo);
                        *score = *score + gain;

                        ajouter_popup(pop, x, y, gain);

                        couleur_bulle(couleur, &r, &g, &bl);
                        exploser_particules(part, x, y, r, g, bl);

                        ajouter_explosion(exp, x - tx / 2, y - tx / 2, tx);

                        declencher_shake(2);

                        if (rand() % 6 == 0) {
                            ajouter_bonus(bons, x - 25, y, rand() % 2);
                        }

                        if (taille == 2) {
                            ajouter_bulle(b, x - 10, y, 1, 0, couleur);
                            ajouter_bulle(b, x + 10, y, 1, 1, couleur);
                        } else if (taille == 1) {
                            ajouter_bulle(b, x - 15, y, 0, 0, couleur);
                            ajouter_bulle(b, x, y - 10, 0, rand() % 2, couleur);
                            ajouter_bulle(b, x + 15, y, 0, 1, couleur);
                        }

                        break;
                    }
                }
            }
        }
    }
}

void gerer_bonus_collecte(Bonus* bons, Joueur* j, Bulle* bulles,
                          Particule* part, Explosion* exp,
                          PopupScore* pop, int* score)
{
    int i, k;
    int xb, yb;
    int type;

    for (i = 0; i < MAX_BONUS; i++) {
        if (bons[i].active == 1) {
            if (collision_bonus_joueur(&bons[i], j) == 1) {
                type = bons[i].type;
                bons[i].active = 0;

                if (type == 0) {
                    j->bonus_type = 1;
                    j->bonus_timer = 600;
                    ajouter_popup(pop, j->x + j->tx / 2, j->y, 50);
                    *score = *score + 50;
                } else if (type == 1) {
                    j->bonus_type = 2;
                    j->bonus_timer = 600;
                    ajouter_popup(pop, j->x + j->tx / 2, j->y, 100);
                    *score = *score + 100;
                } else {
                    for (k = 0; k < MAX_BULLES; k++) {
                        if (bulles[k].active == 1) {
                            xb = (int)bulles[k].x + bulles[k].tx / 2;
                            yb = (int)bulles[k].y + bulles[k].ty / 2;
                            ajouter_explosion(exp, (int)bulles[k].x, (int)bulles[k].y, bulles[k].tx);
                            exploser_particules(part, xb, yb, 255, 150, 50);
                            *score = *score + 100;
                            ajouter_popup(pop, xb, yb, 100);
                            bulles[k].active = 0;
                        }
                    }
                    declencher_shake(15);
                }
            }
        }
    }
}

void bouger_boss(Boss* b, Bulle* bulles, TirBoss* tirs_boss)
{
    float old_x;
    float vitesse_facteur;
    int amplitude;

    if (b->active == 0) return;

    old_x = b->x;

    vitesse_facteur = 0.02 + (float)(b->vie_max - b->vie) * 0.005;

    amplitude = LARGEUR / 2 - b->tx / 2 - 60;

    b->compteur = b->compteur + 1;
    b->x = (LARGEUR / 2 - b->tx / 2) + amplitude * sin(b->compteur * vitesse_facteur);
    b->y = 100 + (int)(30 * sin(b->compteur * 0.04));

    if (b->x > old_x) {
        b->direction = 0;
    } else if (b->x < old_x) {
        b->direction = 1;
    }

    b->compteur_tir = b->compteur_tir + 1;
    if (b->compteur_tir > 60) {
        ajouter_tir_boss(tirs_boss, (int)b->x + b->tx / 2 - 17, (int)b->y + b->ty);
        b->compteur_tir = 0;
        if (rand() % 3 == 0) {
            ajouter_bulle(bulles, (int)b->x + b->tx / 2 - 20,
                          (int)b->y + b->ty, 1, rand() % 2, rand() % 4);
        }
    }

    if (b->touche > 0) b->touche = b->touche - 1;
}

static void sequence_mort(Joueur* j, int niveau, char* pseudo, int score, int temps,
                          Bulle* bulles, Etoile* etoiles, Boss* boss, int est_boss)
{
    int i;
    j->mort = 1;
    for (i = 0; i < 150; i++) {
        afficher_fond(niveau, etoiles);
        if (est_boss == 1) {
            afficher_bulles(bulles, niveau);
            afficher_boss(boss);
        } else {
            afficher_bulles(bulles, niveau);
        }
        afficher_mort(j);
        afficher_hud(pseudo, score, temps, niveau, 0);
        if (est_boss == 1) {
            afficher_barre_boss(boss);
        }
        afficher_ecran();
        rest(20);
    }
}

int jouer_niveau(char* pseudo, int niveau, int* score)
{
    Joueur joueur;
    Bulle* bulles;
    Projectile* tirs;
    Particule* particules;
    PopupScore* popups;
    Etoile* etoiles;
    Bonus* bons;
    Explosion* explos;
    int temps;
    int compteur_temps;
    int gagne;
    int perdu;
    int echap;
    int i;
    int combo;
    int tick_combo;
    int bonus_temps;
    int compteur_bonus;

    init_joueur(&joueur);
    bulles = creer_bulles();
    tirs = creer_tirs();
    particules = creer_particules();
    popups = creer_popups();
    etoiles = creer_etoiles();
    bons = creer_bonus();
    explos = creer_explosions();
    init_bulles_niveau(bulles, niveau);

    temps = 60;
    compteur_temps = 0;
    gagne = 0;
    perdu = 0;
    echap = 0;
    combo = 0;
    tick_combo = 0;
    compteur_bonus = 0;

    for (i = 3; i > 0; i--) {
        afficher_fond(niveau, etoiles);
        afficher_hud(pseudo, *score, temps, niveau, 1);
        afficher_joueur(&joueur);
        afficher_bulles(bulles, niveau);
        afficher_decompte(i);
        afficher_ecran();
        rest(700);
    }

    while (!key[KEY_ESC] && gagne == 0 && perdu == 0) {
        deplacer_joueur(&joueur);
        gerer_saut(&joueur);
        gerer_tir(&joueur, tirs);

        bouger_bulles(bulles);
        bouger_tirs(tirs);
        bouger_particules(particules);
        bouger_popups(popups);
        bouger_bonus(bons);
        bouger_explosions(explos);

        gerer_collisions(tirs, bulles, particules, popups, explos, bons,
                         score, &combo, &tick_combo);

        gerer_bonus_collecte(bons, &joueur, bulles, particules, explos, popups, score);

        if (joueur.bonus_timer > 0) {
            joueur.bonus_timer = joueur.bonus_timer - 1;
            if (joueur.bonus_timer == 0) {
                joueur.bonus_type = 0;
            }
        }
        if (joueur.invincibilite > 0) {
            joueur.invincibilite = joueur.invincibilite - 1;
        }
        compteur_bonus = compteur_bonus + 1;
        if (compteur_bonus > 240 && rand() % 60 == 0) {
            ajouter_bonus(bons, rand() % (LARGEUR - 50), 0, rand() % 2);
            compteur_bonus = 0;
        }

        if (tick_combo > 0) {
            tick_combo = tick_combo - 1;
        } else {
            combo = 0;
        }

        for (i = 0; i < MAX_BULLES; i++) {
            if (bulles[i].active == 1) {
                if (collision_bulle_joueur(&bulles[i], &joueur) == 1 && joueur.invincibilite == 0) {
                    joueur.vie = joueur.vie - 1;
                    joueur.invincibilite = 90;

                    declencher_shake(10);
                    exploser_particules(particules, joueur.x + joueur.tx / 2,
                                        joueur.y + joueur.ty / 2, 255, 80, 80);

                    if (joueur.vie <= 0) {
                        perdu = 1;
                    }
                }
            }
        }

        if (compter_bulles(bulles) == 0) {
            gagne = 1;
        }

        compteur_temps = compteur_temps + 1;
        if (compteur_temps >= 60) {
            temps = temps - 1;
            compteur_temps = 0;
        }
        if (temps <= 0) {
            perdu = 1;
        }

        afficher_fond(niveau, etoiles);
        afficher_bulles(bulles, niveau);
        afficher_bonus(bons);
        afficher_tirs(tirs);
        afficher_explosions(explos);
        afficher_joueur(&joueur);
        afficher_popups(popups);
        afficher_hud(pseudo, *score, temps, niveau, combo);
        afficher_ecran();

        rest(16);
    }

    if (perdu == 1) {
        sequence_mort(&joueur, niveau, pseudo, *score, temps, bulles, etoiles, NULL, 0);
    }

    if (gagne == 1) {
        joueur.invincibilite = 0;
        bonus_temps = temps * 50;
        *score = *score + bonus_temps;

        for (i = 0; i < 30; i++) {
            afficher_fond(niveau, etoiles);
            afficher_joueur(&joueur);
            afficher_hud(pseudo, *score, temps, niveau, 1);
            {
                char texte[100];
                sprintf(texte, "BONUS DE TEMPS : +%d  (gagne en %ds)",
                        bonus_temps, 60 - temps);
                textout_centre_ex(page, font, texte,
                                  LARGEUR / 2 + 2, HAUTEUR / 2 + 2,
                                  makecol(0, 0, 0), -1);
                textout_centre_ex(page, font, texte, LARGEUR / 2, HAUTEUR / 2,
                                  makecol(255, 220, 50), -1);
            }
            bouger_particules(particules);
            afficher_ecran();
            rest(50);
        }
    }

    if (key[KEY_ESC]) echap = 1;

    detruire_bulles(bulles);
    detruire_tirs(tirs);
    detruire_particules(particules);
    detruire_popups(popups);
    detruire_etoiles(etoiles);
    detruire_bonus(bons);
    detruire_explosions(explos);

    if (echap == 1) return -1;
    if (gagne == 1) return 1;
    return 0;
}

int jouer_boss(char* pseudo, int* score)
{
    Joueur joueur;
    Bulle* bulles;
    Projectile* tirs;
    Particule* particules;
    PopupScore* popups;
    Etoile* etoiles;
    Boss* boss;
    Bonus* bons;
    Explosion* explos;
    TirBoss* tirs_boss;
    int temps;
    int compteur_temps;
    int gagne;
    int perdu;
    int echap;
    int i, j;
    int combo;
    int tick_combo;
    int bonus_temps;
    int compteur_bonus;

    init_joueur(&joueur);
    bulles = creer_bulles();
    tirs = creer_tirs();
    particules = creer_particules();
    popups = creer_popups();
    etoiles = creer_etoiles();
    boss = creer_boss();
    bons = creer_bonus();
    explos = creer_explosions();
    tirs_boss = creer_tirs_boss();
    init_boss(boss);

    temps = 90;
    compteur_temps = 0;
    gagne = 0;
    perdu = 0;
    echap = 0;
    combo = 0;
    tick_combo = 0;
    compteur_bonus = 0;

    for (i = 3; i > 0; i--) {
        afficher_fond(4, etoiles);
        afficher_hud(pseudo, *score, temps, 4, 1);
        afficher_joueur(&joueur);
        afficher_boss(boss);
        afficher_barre_boss(boss);
        afficher_decompte(i);
        afficher_ecran();
        rest(700);
    }

    while (!key[KEY_ESC] && gagne == 0 && perdu == 0) {
        deplacer_joueur(&joueur);
        gerer_saut(&joueur);
        gerer_tir(&joueur, tirs);

        bouger_boss(boss, bulles, tirs_boss);
        bouger_bulles(bulles);
        bouger_tirs(tirs);
        bouger_tirs_boss(tirs_boss);
        bouger_particules(particules);
        bouger_popups(popups);
        bouger_bonus(bons);
        bouger_explosions(explos);

        gerer_collisions(tirs, bulles, particules, popups, explos, bons,
                         score, &combo, &tick_combo);

        gerer_bonus_collecte(bons, &joueur, bulles, particules, explos, popups, score);

        if (joueur.bonus_timer > 0) {
            joueur.bonus_timer = joueur.bonus_timer - 1;
            if (joueur.bonus_timer == 0) {
                joueur.bonus_type = 0;
            }
        }

        compteur_bonus = compteur_bonus + 1;
        if (compteur_bonus > 240 && rand() % 60 == 0) {
            ajouter_bonus(bons, rand() % (LARGEUR - 50), 0, rand() % 2);
            compteur_bonus = 0;
        }

        if (tick_combo > 0) {
            tick_combo = tick_combo - 1;
        } else {
            combo = 0;
        }

        for (i = 0; i < MAX_TIRS; i++) {
            if (tirs[i].active == 1) {
                if (collision_tir_boss(&tirs[i], boss) == 1) {
                    tirs[i].active = 0;
                    boss->vie = boss->vie - 1;
                    boss->touche = 10;
                    *score = *score + 200;
                    ajouter_popup(popups, (int)boss->x + boss->tx / 2,
                                  (int)boss->y, 200);
                    exploser_particules(particules, (int)boss->x + boss->tx / 2,
                                        (int)boss->y + boss->ty / 2, 255, 100, 200);
                    declencher_shake(5);
                    if (boss->vie <= 0) {
                        boss->active = 0;
                        gagne = 1;
                        *score = *score + 2000;
                        for (j = 0; j < 5; j++) {
                            exploser_particules(particules,
                                (int)boss->x + (rand() % boss->tx),
                                (int)boss->y + (rand() % boss->ty),
                                200 + rand() % 56, 100 + rand() % 100, 50);
                        }
                        declencher_shake(20);
                    }
                }
            }
        }

        if (collision_boss_joueur(boss, &joueur) == 1) {
            perdu = 1;
            declencher_shake(15);
            exploser_particules(particules, joueur.x + joueur.tx / 2,
                                joueur.y + joueur.ty / 2, 255, 80, 80);
        }

        for (i = 0; i < MAX_BULLES; i++) {
            if (bulles[i].active == 1) {
                if (collision_bulle_joueur(&bulles[i], &joueur) == 1) {
                    perdu = 1;
                    declencher_shake(15);
                    exploser_particules(particules, joueur.x + joueur.tx / 2,
                                        joueur.y + joueur.ty / 2, 255, 80, 80);
                }
            }
        }

        for (i = 0; i < MAX_TIRS_BOSS; i++) {
            if (tirs_boss[i].active == 1) {
                if (collision_tirboss_joueur(&tirs_boss[i], &joueur) == 1) {
                    tirs_boss[i].active = 0;
                    perdu = 1;
                    declencher_shake(15);
                    exploser_particules(particules, joueur.x + joueur.tx / 2,
                                        joueur.y + joueur.ty / 2, 255, 80, 80);
                }
            }
        }

        compteur_temps = compteur_temps + 1;
        if (compteur_temps >= 60) {
            temps = temps - 1;
            compteur_temps = 0;
        }
        if (temps <= 0) {
            perdu = 1;
        }

        afficher_fond(4, etoiles);
        afficher_bulles(bulles, 4);
        afficher_bonus(bons);
        afficher_tirs(tirs);
        afficher_tirs_boss(tirs_boss);
        afficher_boss(boss);
        afficher_explosions(explos);
        afficher_joueur(&joueur);
        afficher_popups(popups);
        afficher_hud(pseudo, *score, temps, 4, combo);
        afficher_barre_boss(boss);
        afficher_ecran();

        rest(16);
    }

    if (perdu == 1) {
        sequence_mort(&joueur, 4, pseudo, *score, temps, bulles, etoiles, boss, 1);
    }

    if (gagne == 1) {
        bonus_temps = temps * 100;
        *score = *score + bonus_temps;
        for (i = 0; i < 40; i++) {
            afficher_fond(4, etoiles);
            afficher_joueur(&joueur);
            afficher_hud(pseudo, *score, temps, 4, 1);
            {
                char texte[100];
                sprintf(texte, "BOSS VAINCU !  +2000   BONUS TEMPS : +%d", bonus_temps);
                textout_centre_ex(page, font, texte,
                                  LARGEUR / 2 + 2, HAUTEUR / 2 + 2,
                                  makecol(0, 0, 0), -1);
                textout_centre_ex(page, font, texte, LARGEUR / 2, HAUTEUR / 2,
                                  makecol(255, 220, 50), -1);
            }
            bouger_particules(particules);
            if (i % 3 == 0) {
                exploser_particules(particules,
                    rand() % LARGEUR,
                    100 + rand() % 300,
                    255, 200 + rand() % 56, 50);
            }
            afficher_ecran();
            rest(60);
        }
    }

    if (key[KEY_ESC]) echap = 1;

    detruire_bulles(bulles);
    detruire_tirs(tirs);
    detruire_particules(particules);
    detruire_popups(popups);
    detruire_etoiles(etoiles);
    detruire_boss(boss);
    detruire_bonus(bons);
    detruire_explosions(explos);
    detruire_tirs_boss(tirs_boss);

    if (echap == 1) return -1;
    if (gagne == 1) return 1;
    return 0;
}

int jouer_partie(char* pseudo, int niveau_depart)
{
    int niveau;
    int score;
    int res;

    niveau = niveau_depart;
    score = 0;

    while (niveau <= 3) {
        res = jouer_niveau(pseudo, niveau, &score);
        if (res == -1) return score;
        if (res == 0) {
            afficher_defaite(score);
            if (niveau > 1) sauvegarder(pseudo, niveau - 1);
            ajouter_score(pseudo, score);
            return score;
        }
        sauvegarder(pseudo, niveau);
        niveau = niveau + 1;
    }

    res = jouer_boss(pseudo, &score);
    if (res == -1) {
        ajouter_score(pseudo, score);
        return score;
    }
    if (res == 0) {
        afficher_defaite(score);
        sauvegarder(pseudo, 3);
        ajouter_score(pseudo, score);
        return score;
    }

    sauvegarder(pseudo, 4);
    afficher_victoire(score);
    ajouter_score(pseudo, score);
    return score;
}
