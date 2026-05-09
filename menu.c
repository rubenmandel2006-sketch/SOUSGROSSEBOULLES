#include "menu.h"
#include "init.h"
#include "affichage.h"
#include "entites.h"
#include "sauvegarde.h"
#include <allegro.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Etoiles du fond du menu (init la premiere fois)
static Etoile* etoiles_menu = NULL;

// Pareil que dans affichage.c : ecrit un texte en grand en l'agrandissant
static void texte_o_geant(int x, int y, const char* t, int r, int g, int b, float echelle)
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

// Dessine le fond du menu : degrade violet + etoiles qui clignotent
static void fond_menu(void)
{
    int i, br;

    // On cree les etoiles seulement la premiere fois
    if (etoiles_menu == NULL) {
        etoiles_menu = creer_etoiles();
    }

    // Degrade vertical pour le fond
    for (i = 0; i < HAUTEUR; i++) {
        int t = i * 100 / HAUTEUR;
        hline(page, 0, i, LARGEUR,
              makecol(20 + t / 2, 10 + t / 4, 50 + t));
    }

    // Etoiles qui clignotent (avec sin pour faire varier la luminosite)
    for (i = 0; i < MAX_ETOILES; i++) {
        br = 150 + (int)(105 * sin((g_tick + etoiles_menu[i].phase * 5) * 0.05));
        if (br < 100) br = 100;
        if (br > 255) br = 255;
        circlefill(page, etoiles_menu[i].x, etoiles_menu[i].y,
                   etoiles_menu[i].taille, makecol(br, br, br));
    }
}

// Affiche le menu principal et retourne le choix du joueur
int menu_principal(void)
{
    int choix;
    int fini;
    int oy;

    int start_y = 300;
    int step = 60;

    choix = 1;
    fini = 0;

    // On attend que entree soit relachee pour pas valider tout de suite
    while (key[KEY_ENTER]) { rest(10); }

    while (fini == 0) {
        fond_menu();

        // oy = petit decalage qui flotte avec un sin pour faire bouger le titre
        oy = (int)(10 * sin(g_tick * 0.05));
        texte_o_geant(LARGEUR / 2, 120 + oy, "SOUS GROSSES BULLES", 100, 220, 255, 5.0);
        texte_o_geant(LARGEUR / 2, 180 + oy, "A.Amouri,G.Sersiron,A.Limame,R.Mandel", 255, 200, 100, 2.5);

        // On affiche chaque option avec une fleche '>' devant celle qui est selectionnee
        if (choix == 1) texte_o_geant(LARGEUR / 2, start_y, "> 1. Lire les regles <", 255, 255, 100, 2.5);
        else            texte_o_geant(LARGEUR / 2, start_y, "1. Lire les regles", 220, 220, 220, 2.5);

        if (choix == 2) texte_o_geant(LARGEUR / 2, start_y + step, "> 2. Nouvelle partie <", 255, 255, 100, 2.5);
        else            texte_o_geant(LARGEUR / 2, start_y + step, "2. Nouvelle partie", 220, 220, 220, 2.5);

        if (choix == 3) texte_o_geant(LARGEUR / 2, start_y + step*2, "> 3. Reprendre <", 255, 255, 100, 2.5);
        else            texte_o_geant(LARGEUR / 2, start_y + step*2, "3. Reprendre", 220, 220, 220, 2.5);

        if (choix == 4) texte_o_geant(LARGEUR / 2, start_y + step*3, "> 4. Top Scores <", 255, 255, 100, 2.5);
        else            texte_o_geant(LARGEUR / 2, start_y + step*3, "4. Top Scores", 220, 220, 220, 2.5);

        if (choix == 5) texte_o_geant(LARGEUR / 2, start_y + step*4, "> 5. Quitter <", 255, 255, 100, 2.5);
        else            texte_o_geant(LARGEUR / 2, start_y + step*4, "5. Quitter", 220, 220, 220, 2.5);

        texte_o_geant(LARGEUR / 2, 650, "FLECHES : Choisir  |  ENTREE : Valider", 180, 180, 200, 1.5);

        afficher_ecran();

        // Fleches haut/bas pour changer le choix (avec un petit delai pour pas defiler trop vite)
        if (key[KEY_DOWN]) {
            choix = choix + 1;
            if (choix > 5) choix = 1;
            rest(150);
        }
        if (key[KEY_UP]) {
            choix = choix - 1;
            if (choix < 1) choix = 5;
            rest(150);
        }
        if (key[KEY_ENTER]) fini = 1;

        rest(20);
    }

    return choix;
}

// Permet au joueur de saisir son pseudo lettre par lettre
void saisir_pseudo(char* pseudo)
{
    int i;
    int fini;
    char c;

    i = 0;
    fini = 0;
    pseudo[0] = '\0';

    while (key[KEY_ENTER]) { rest(10); }

    // On vide le buffer du clavier pour eviter les touches deja appuyees
    clear_keybuf();

    while (fini == 0) {
        fond_menu();

        texte_o_geant(LARGEUR / 2, 200, "Entre ton pseudo :", 255, 255, 255, 3.0);

        // Cadre ou on tape le pseudo
        rectfill(page, LARGEUR / 2 - 200, 300, LARGEUR / 2 + 200, 360, makecol(30, 30, 60));
        rect(page, LARGEUR / 2 - 200, 300, LARGEUR / 2 + 200, 360, makecol(255, 255, 255));

        texte_o_geant(LARGEUR / 2, 330, pseudo, 255, 255, 100, 3.0);

        // Curseur clignotant
        if ((g_tick / 20) % 2 == 0 && i < 20) {
            int largeur_texte = i * 24;
            line(page,
                 LARGEUR / 2 + largeur_texte / 2 + 8, 315,
                 LARGEUR / 2 + largeur_texte / 2 + 8, 345,
                 makecol(255, 255, 255));
        }

        texte_o_geant(LARGEUR / 2, 450, "ENTREE pour valider", 200, 200, 200, 2.0);

        afficher_ecran();

        // Lecture des touches
        if (keypressed()) {
            // & 0xff pour recuperer juste le caractere ASCII
            c = readkey() & 0xff;
            // 13 = ENTREE, on valide si on a au moins 1 caractere
            if (c == 13 && i > 0) {
                fini = 1;
            // 8 = BACKSPACE, on efface le dernier caractere
            } else if (c == 8 && i > 0) {
                i = i - 1;
                pseudo[i] = '\0';
            // Sinon on ajoute le caractere si c'est imprimable et qu'il y a la place
            } else if (i < 20 && c >= 32 && c <= 126) {
                pseudo[i] = c;
                i = i + 1;
                pseudo[i] = '\0';
            }
        }
        rest(20);
    }
}

// Affiche l'ecran des regles du jeu
void afficher_regles(void)
{
    int sortir;

    sortir = 0;
    while (key[KEY_ENTER]) { rest(10); }

    while (sortir == 0) {
        fond_menu();

        texte_o_geant(LARGEUR / 2, 80, "REGLES DU JEU", 255, 220, 80, 4.0);

        texte_o_geant(LARGEUR / 2, 160, "- Detruis toutes les cibles pour gagner -", 255, 255, 255, 1.5);
        texte_o_geant(LARGEUR / 2, 200, "- Une grosse cible se divise en 2 -", 255, 255, 255, 1.5);
        texte_o_geant(LARGEUR / 2, 240, "- Touche par une cible ou le boss = mort -", 255, 255, 255, 1.5);
        texte_o_geant(LARGEUR / 2, 280, "- Termine avant la fin du temps ! -", 255, 255, 255, 1.5);

        texte_o_geant(LARGEUR / 2, 330, "4 niveaux dont 1 BOSS final", 255, 200, 100, 2.0);

        texte_o_geant(LARGEUR / 2, 380, "Bonus :", 100, 255, 200, 2.0);
        texte_o_geant(LARGEUR / 2, 420, "x2 : 2 tirs - x3 : 3 tirs - BOOM : detruit tout", 255, 255, 255, 1.5);

        texte_o_geant(LARGEUR / 2, 470, "Score :", 100, 255, 200, 2.0);
        texte_o_geant(LARGEUR / 2, 510, "Grosse cible : 50 | Moyenne : 100 | Petite : 200", 255, 255, 255, 1.5);

        texte_o_geant(LARGEUR / 2, 560, "Commandes :", 100, 200, 255, 2.0);
        texte_o_geant(LARGEUR / 2, 600, "FLECHES : bouger | ESPACE : sauter | A,Q,X : tirer", 255, 255, 255, 1.5);

        texte_o_geant(LARGEUR / 2, 680, "Appuie sur ENTREE pour revenir", 200, 200, 200, 1.5);

        afficher_ecran();

        if (key[KEY_ENTER]) sortir = 1;
        rest(20);
    }

    while (key[KEY_ENTER]) { rest(10); }
}

// Affiche les meilleurs scores enregistres
void afficher_top_scores(void)
{
    Score scores[MAX_SCORES];
    int n;
    int i;
    int sortir;
    char ligne[100];

    // On lit les scores depuis le fichier
    n = lire_top_scores(scores);

    sortir = 0;
    while (key[KEY_ENTER]) { rest(10); }

    while (sortir == 0) {
        fond_menu();

        texte_o_geant(LARGEUR / 2, 100, "TOP SCORES", 255, 220, 80, 4.0);
        texte_o_geant(LARGEUR / 2, 160, "* * * * *", 100, 200, 255, 2.0);

        // S'il y a aucun score on affiche un message
        if (n == 0) {
            texte_o_geant(LARGEUR / 2, 350, "Aucun score pour l'instant", 200, 200, 200, 2.0);
            texte_o_geant(LARGEUR / 2, 400, "Joue une partie !", 200, 200, 200, 2.0);
        } else {
            // On boucle sur les scores avec une couleur differente pour le top 3
            for (i = 0; i < n; i++) {
                sprintf(ligne, "%d.  %-20s   %d", i + 1, scores[i].pseudo, scores[i].score);
                if (i == 0) {
                    // Or pour le 1er
                    texte_o_geant(LARGEUR / 2, 240 + i * 60, ligne, 255, 220, 50, 2.0);
                } else if (i == 1) {
                    // Argent pour le 2eme
                    texte_o_geant(LARGEUR / 2, 240 + i * 60, ligne, 200, 200, 200, 2.0);
                } else if (i == 2) {
                    // Bronze pour le 3eme
                    texte_o_geant(LARGEUR / 2, 240 + i * 60, ligne, 200, 130, 80, 2.0);
                } else {
                    texte_o_geant(LARGEUR / 2, 240 + i * 60, ligne, 220, 220, 220, 2.0);
                }
            }
        }

        texte_o_geant(LARGEUR / 2, 680, "Appuie sur ENTREE pour revenir", 200, 200, 200, 1.5);
        afficher_ecran();

        if (key[KEY_ENTER]) sortir = 1;
        rest(20);
    }

    while (key[KEY_ENTER]) { rest(10); }
}
