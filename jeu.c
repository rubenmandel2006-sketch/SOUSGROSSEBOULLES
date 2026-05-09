#include "jeu.h"
#include "init.h"
#include "clavier.h"
#include "affichage.h"
#include "sauvegarde.h"
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Donne le nombre de points selon la taille de la bulle (les petites valent plus)
static int points_bulle(int taille)
{
    if (taille == 2) return 50;
    if (taille == 1) return 100;
    return 200;
}

// Place les bulles de depart selon le niveau (plus c'est dur, plus il y en a)
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

    // On reset toutes les bulles
    for (i = 0; i < MAX_BULLES; i++) {
        b[i].active = 0;
    }

    // Le nombre de bulles change selon le niveau
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

    // On boucle pour placer chaque bulle au-dessus de l'ecran
    for (i = 0; i < nb_meteores; i++) {
        pos_x = 50 + rand() % (LARGEUR - 100);
        pos_y = 80 - (i * espacement_y);

        // 1 chance sur 3 d'avoir une bulle moyenne au lieu d'une grosse
        taille = 2;
        if (rand() % 3 == 0) {
            taille = 1;
        }

        direction = rand() % 2;
        couleur = rand() % 4;

        ajouter_bulle(b, pos_x, pos_y, taille, direction, couleur);
    }
}

// Compte combien de bulles sont actives
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

// Met a jour la position des bulles avec rebond et gravite
void bouger_bulles(Bulle* b)
{
    int i;
    for (i = 0; i < MAX_BULLES; i++) {
        if (b[i].active == 1) {
            b[i].x = b[i].x + b[i].vx;
            b[i].y = b[i].y + b[i].vy;

            // Gravite : on augmente la vitesse vers le bas a chaque image
            b[i].vy = b[i].vy + 0.22;

            // On limite la vitesse pour pas que ca aille trop vite
            if (b[i].vy > 12) {
                b[i].vy = 12;
            }

            // Rebond sur les murs gauche/droite
            if (b[i].x < 0) {
                b[i].x = 0;
                b[i].vx = -b[i].vx;
            }
            if (b[i].x + b[i].tx > LARGEUR) {
                b[i].x = LARGEUR - b[i].tx;
                b[i].vx = -b[i].vx;
            }

            // Rebond sur le plafond
            if (b[i].y < PLAFOND && b[i].y > 0 && b[i].vy < 0) {
                b[i].y = PLAFOND;
                b[i].vy = -b[i].vy;
            }

            // Rebond sur le sol : la hauteur du rebond depend de la taille
            if (b[i].y + b[i].ty > SOL) {
                b[i].y = SOL - b[i].ty;
                if (b[i].taille == 0) b[i].vy = -15;
                if (b[i].taille == 1) b[i].vy = -14;
                if (b[i].taille == 2) b[i].vy = -13;
            }
        }
    }
}

// Fait monter les tirs et gere leur animation
void bouger_tirs(Projectile* t)
{
    int i;
    for (i = 0; i < MAX_TIRS; i++) {
        if (t[i].active == 1) {
            t[i].y = t[i].y - 10;

            // On change de frame tous les 3 ticks
            t[i].compteur = t[i].compteur + 1;
            if (t[i].compteur > 3) {
                t[i].compteur = 0;
                t[i].frame = t[i].frame + 1;
                if (t[i].frame > 2) {
                    t[i].frame = 0;
                }
            }

            // Si le tir sort en haut on le desactive
            if (t[i].y < 0) {
                t[i].active = 0;
            }
        }
    }
}

// Fait bouger les particules avec gravite et reduit leur duree de vie
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

// Fait monter les popups de score et reduit leur duree de vie
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

// Fait tomber les bonus, on les desactive s'ils sortent de l'ecran
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

// Fait avancer l'animation des explosions (3 frames)
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

// Bouge les tirs du boss avec rebond sur les bords
void bouger_tirs_boss(TirBoss* t)
{
    int i;
    for (i = 0; i < MAX_TIRS_BOSS; i++) {
        if (t[i].active == 1) {
            t[i].x = t[i].x + t[i].vx;
            t[i].y = t[i].y + t[i].vy;

            // Rebond gauche/droite
            if (t[i].x < 0) {
                t[i].x = 0;
                t[i].vx = -t[i].vx;
            }
            if (t[i].x + t[i].tx > LARGEUR) {
                t[i].x = LARGEUR - t[i].tx;
                t[i].vx = -t[i].vx;
            }

            // Si ca sort de l'ecran on desactive
            if (t[i].y > HAUTEUR) {
                t[i].active = 0;
            }
            if (t[i].y < 0) {
                t[i].active = 0;
            }

            // Animation
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

// Donne la couleur d'une bulle pour l'effet d'explosion (en RGB)
static void couleur_bulle(int c, int* r, int* g, int* bl)
{
    if (c == 0) { *r = 230; *g = 80;  *bl = 80;  }
    else if (c == 1) { *r = 80;  *g = 200; *bl = 90;  }
    else if (c == 2) { *r = 80;  *g = 130; *bl = 230; }
    else { *r = 230; *g = 200; *bl = 60; }
}

// Gere les collisions tir/bulle : explosion, score, division en bulles plus petites
void gerer_collisions(Projectile* t, Bulle* b, Particule* part,
                      PopupScore* pop, Explosion* exp, Bonus* bons,
                      int* score, int* combo, int* tick_combo)
{
    int i, j;
    int x, y, taille, couleur;
    int tx;
    int gain;
    int r, g, bl;

    // On boucle sur tous les tirs et toutes les bulles pour tester les collisions
    for (i = 0; i < MAX_TIRS; i++) {
        if (t[i].active == 1) {
            for (j = 0; j < MAX_BULLES; j++) {
                if (b[j].active == 1) {
                    if (collision_tir_bulle(&t[i], &b[j]) == 1) {
                        t[i].active = 0;

                        // On garde les infos de la bulle avant de la desactiver
                        x = (int)b[j].x + b[j].tx / 2;
                        y = (int)b[j].y + b[j].ty / 2;
                        tx = b[j].tx;
                        taille = b[j].taille;
                        couleur = b[j].couleur;

                        b[j].active = 0;

                        // Combo : a chaque bulle cassee on monte le multiplicateur
                        *combo = *combo + 1;
                        if (*combo > 10) *combo = 10;
                        *tick_combo = 90;

                        // Le gain depend de la taille et du combo
                        gain = points_bulle(taille) * (*combo);
                        *score = *score + gain;

                        ajouter_popup(pop, x, y, gain);

                        // Effet visuel : explosion + particules de la bonne couleur
                        couleur_bulle(couleur, &r, &g, &bl);
                        exploser_particules(part, x, y, r, g, bl);

                        ajouter_explosion(exp, x - tx / 2, y - tx / 2, tx);

                        declencher_shake(2);

                        // 1 chance sur 6 de faire tomber un bonus
                        if (rand() % 6 == 0) {
                            ajouter_bonus(bons, x - 25, y, rand() % 2);
                        }

                        // La bulle se divise en 2 (grosse) ou 3 (moyenne) plus petites
                        if (taille == 2) {
                            ajouter_bulle(b, x - 10, y, 1, 0, couleur);
                            ajouter_bulle(b, x + 10, y, 1, 1, couleur);
                        } else if (taille == 1) {
                            ajouter_bulle(b, x - 15, y, 0, 0, couleur);
                            ajouter_bulle(b, x, y - 10, 0, rand() % 2, couleur);
                            ajouter_bulle(b, x + 15, y, 0, 1, couleur);
                        }

                        // Important : on sort de la boucle des bulles pour pas casser plusieurs bulles avec un seul tir
                        break;
                    }
                }
            }
        }
    }
}

// Gere ce qui se passe quand le joueur attrape un bonus
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

                // Bonus type 0 : double tir pendant 600 ticks
                if (type == 0) {
                    j->bonus_type = 1;
                    j->bonus_timer = 600;
                    ajouter_popup(pop, j->x + j->tx / 2, j->y, 50);
                    *score = *score + 50;
                // Bonus type 1 : triple tir pendant 600 ticks
                } else if (type == 1) {
                    j->bonus_type = 2;
                    j->bonus_timer = 600;
                    ajouter_popup(pop, j->x + j->tx / 2, j->y, 100);
                    *score = *score + 100;
                // Bonus type BOOM : detruit toutes les bulles a l'ecran
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

// Bouge le boss avec un mouvement sinusoidal et le fait tirer regulierement
void bouger_boss(Boss* b, Bulle* bulles, TirBoss* tirs_boss)
{
    float old_x;
    float vitesse_facteur;
    int amplitude;

    if (b->active == 0) return;

    old_x = b->x;

    // Plus le boss perd de vie, plus il bouge vite
    vitesse_facteur = 0.02 + (float)(b->vie_max - b->vie) * 0.005;

    amplitude = LARGEUR / 2 - b->tx / 2 - 60;

    // Mouvement avec sin/cos pour faire un deplacement fluide
    b->compteur = b->compteur + 1;
    b->x = (LARGEUR / 2 - b->tx / 2) + amplitude * sin(b->compteur * vitesse_facteur);
    b->y = 100 + (int)(30 * sin(b->compteur * 0.04));

    // On regarde dans quelle direction le boss bouge pour choisir le bon sprite
    if (b->x > old_x) {
        b->direction = 0;
    } else if (b->x < old_x) {
        b->direction = 1;
    }

    // Le boss tire toutes les 60 frames
    b->compteur_tir = b->compteur_tir + 1;
    if (b->compteur_tir > 60) {
        ajouter_tir_boss(tirs_boss, (int)b->x + b->tx / 2 - 17, (int)b->y + b->ty);
        b->compteur_tir = 0;
        // 1 chance sur 3 de cracher une bulle en plus
        if (rand() % 3 == 0) {
            ajouter_bulle(bulles, (int)b->x + b->tx / 2 - 20,
                          (int)b->y + b->ty, 1, rand() % 2, rand() % 4);
        }
    }

    if (b->touche > 0) b->touche = b->touche - 1;
}

// Affiche le joueur mort pendant 150 frames avant de finir le niveau
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

// Boucle principale d'un niveau (1, 2 ou 3)
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

    // Init de tous les objets du niveau
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

    // Petit decompte 3, 2, 1 avant le debut
    for (i = 3; i > 0; i--) {
        afficher_fond(niveau, etoiles);
        afficher_hud(pseudo, *score, temps, niveau, 1);
        afficher_joueur(&joueur);
        afficher_bulles(bulles, niveau);
        afficher_decompte(i);
        afficher_ecran();
        rest(700);
    }

    // Boucle principale du jeu : tant que pas gagne, pas perdu, pas echap
    while (!key[KEY_ESC] && gagne == 0 && perdu == 0) {
        // 1) Gestion des entrees clavier
        deplacer_joueur(&joueur);
        gerer_saut(&joueur);
        gerer_tir(&joueur, tirs);

        // 2) On bouge tous les objets
        bouger_bulles(bulles);
        bouger_tirs(tirs);
        bouger_particules(particules);
        bouger_popups(popups);
        bouger_bonus(bons);
        bouger_explosions(explos);

        // 3) Collisions tirs/bulles
        gerer_collisions(tirs, bulles, particules, popups, explos, bons,
                         score, &combo, &tick_combo);

        gerer_bonus_collecte(bons, &joueur, bulles, particules, explos, popups, score);

        // Decompte du timer du bonus
        if (joueur.bonus_timer > 0) {
            joueur.bonus_timer = joueur.bonus_timer - 1;
            if (joueur.bonus_timer == 0) {
                joueur.bonus_type = 0;
            }
        }
        if (joueur.invincibilite > 0) {
            joueur.invincibilite = joueur.invincibilite - 1;
        }
        // De temps en temps on fait apparaitre un bonus
        compteur_bonus = compteur_bonus + 1;
        if (compteur_bonus > 240 && rand() % 60 == 0) {
            ajouter_bonus(bons, rand() % (LARGEUR - 50), 0, rand() % 2);
            compteur_bonus = 0;
        }

        // Si on tarde trop a casser une bulle le combo retombe a 0
        if (tick_combo > 0) {
            tick_combo = tick_combo - 1;
        } else {
            combo = 0;
        }

        // On verifie si une bulle touche le joueur (s'il n'est pas invincible)
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

        // Si plus aucune bulle, on a gagne
        if (compter_bulles(bulles) == 0) {
            gagne = 1;
        }

        // Le timer descend de 1 chaque seconde (60 ticks)
        compteur_temps = compteur_temps + 1;
        if (compteur_temps >= 60) {
            temps = temps - 1;
            compteur_temps = 0;
        }
        if (temps <= 0) {
            perdu = 1;
        }

        // 4) On dessine tout
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

    // Si on a gagne, on donne un bonus de temps en points
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

    // On libere toute la memoire allouee
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

// Boucle principale du combat contre le boss (niveau 4)
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

    // Init de tout
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

    // Decompte avant le combat
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
        // Entrees clavier
        deplacer_joueur(&joueur);
        gerer_saut(&joueur);
        gerer_tir(&joueur, tirs);

        // On bouge tout
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

        // On gere les tirs qui touchent le boss
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
                    // Si le boss n'a plus de vie, on a gagne
                    if (boss->vie <= 0) {
                        boss->active = 0;
                        gagne = 1;
                        *score = *score + 2000;
                        // Plein d'explosions au hasard sur le boss pour faire un gros effet
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

        // Si le boss touche le joueur c'est mort direct
        if (collision_boss_joueur(boss, &joueur) == 1) {
            perdu = 1;
            declencher_shake(15);
            exploser_particules(particules, joueur.x + joueur.tx / 2,
                                joueur.y + joueur.ty / 2, 255, 80, 80);
        }

        // Pareil si une bulle touche le joueur
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

        // Si un tir du boss touche le joueur c'est mort
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

        // Decompte du temps
        compteur_temps = compteur_temps + 1;
        if (compteur_temps >= 60) {
            temps = temps - 1;
            compteur_temps = 0;
        }
        if (temps <= 0) {
            perdu = 1;
        }

        // Affichage de tout
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

    // Animation de victoire avec plein de particules
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
            // On fait apparaitre des particules un peu partout pour le show
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

    // On libere toute la memoire
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

// Joue toute la partie : niveaux 1 a 3 puis le boss
int jouer_partie(char* pseudo, int niveau_depart)
{
    int niveau;
    int score;
    int res;

    niveau = niveau_depart;
    score = 0;

    // On enchaine les niveaux 1, 2, 3
    while (niveau <= 3) {
        res = jouer_niveau(pseudo, niveau, &score);
        if (res == -1) return score;
        // Si on perd : afficher defaite, sauvegarder, ajouter au top scores, et stop
        if (res == 0) {
            afficher_defaite(score);
            if (niveau > 1) sauvegarder(pseudo, niveau - 1);
            ajouter_score(pseudo, score);
            return score;
        }
        sauvegarder(pseudo, niveau);
        niveau = niveau + 1;
    }

    // Apres les 3 niveaux on enchaine sur le boss
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

    // Si on bat le boss : sauvegarde au niveau 4 et affichage victoire
    sauvegarder(pseudo, 4);
    afficher_victoire(score);
    ajouter_score(pseudo, score);
    return score;
}
