#ifndef ENTITES_H
#define ENTITES_H

#include <allegro.h>

// Taille de la fenetre du jeu
#define LARGEUR 1024
#define HAUTEUR 768

// Limites de la zone de jeu (pour le HUD en haut et en bas)
#define HAUT_HUD 70
#define BAS_HUD  710
#define SOL      BAS_HUD
#define PLAFOND  HAUT_HUD

// Nombre max d'objets de chaque type qu'on peut avoir en meme temps
#define MAX_BULLES     60
#define MAX_TIRS       30
#define MAX_PARTICULES 250
#define MAX_POPUPS     20
#define MAX_ETOILES    80
#define MAX_BONUS      1
#define MAX_EXPLOSIONS 25
#define MAX_TIRS_BOSS  15

// Structure du joueur
typedef struct {
    int x, y;             // position du joueur
    int tx, ty;           // taille (largeur, hauteur)
    int vie;              // nombre de vies
    int direction;        // 0 = droite, 1 = gauche
    int saut;             // 1 si en train de sauter
    float vy;             // vitesse verticale (pour le saut)
    int frame;            // image actuelle de l'animation
    int compteur;         // pour faire avancer l'animation
    int mort;             // 1 si le joueur est mort
    int bonus_type;       // type de bonus actif (0 = aucun, 1 = x2, 2 = x3)
    int bonus_timer;      // duree restante du bonus
    int invincibilite;    // duree restante d'invincibilite apres un coup
} Joueur;

// Structure d'une bulle (cible a detruire)
typedef struct {
    float x, y;           // position
    int tx, ty;           // taille
    float vx, vy;         // vitesse en x et y
    int taille;           // 0 = petite, 1 = moyenne, 2 = grosse
    int active;           // 1 si la bulle existe
    int couleur;          // couleur de la bulle
} Bulle;

// Structure d'un tir du joueur
typedef struct {
    int x, y;
    int tx, ty;
    int active;
    int frame;
    int compteur;
} Projectile;

// Structure du boss
typedef struct {
    float x, y;
    int tx, ty;
    int vie;              // vie actuelle
    int vie_max;          // vie max (pour la barre de vie)
    float vx;
    int active;
    int compteur_tir;     // compteur pour savoir quand tirer
    int frame;
    int compteur;
    int touche;           // > 0 pendant un court instant quand le boss est touche
    int direction;
} Boss;

// Petite particule pour les effets d'explosion
typedef struct {
    float x, y;
    float vx, vy;
    int vie;
    int taille;
    int r, g, b;          // couleur
    int active;
} Particule;

// Texte qui apparait quand on gagne des points (genre "+200")
typedef struct {
    int x, y;
    int valeur;
    int vie;
    int active;
    int couleur;
} PopupScore;

// Etoile dans le fond
typedef struct {
    int x, y;
    int taille;
    int phase;            // pour decaler le clignotement
} Etoile;

// Bonus qui tombe quand on detruit une bulle
typedef struct {
    float x, y;
    int tx, ty;
    float vy;
    int type;             // type de bonus
    int active;
} Bonus;

// Animation d'explosion
typedef struct {
    int x, y;
    int tx, ty;
    int frame;
    int compteur;
    int active;
} Explosion;

// Tir du boss
typedef struct {
    float x, y;
    int tx, ty;
    float vx, vy;
    int frame;
    int compteur;
    int active;
} TirBoss;

// Initialise le joueur (position, vie, etc)
void init_joueur(Joueur* j);

// Toutes ces fonctions creent un tableau d'objets et le retournent
Bulle* creer_bulles(void);
Projectile* creer_tirs(void);
Particule* creer_particules(void);
PopupScore* creer_popups(void);
Etoile* creer_etoiles(void);
Boss* creer_boss(void);
Bonus* creer_bonus(void);
Explosion* creer_explosions(void);
TirBoss* creer_tirs_boss(void);

// Toutes ces fonctions liberent la memoire des tableaux
void detruire_bulles(Bulle* b);
void detruire_tirs(Projectile* t);
void detruire_particules(Particule* p);
void detruire_popups(PopupScore* p);
void detruire_etoiles(Etoile* e);
void detruire_boss(Boss* b);
void detruire_bonus(Bonus* b);
void detruire_explosions(Explosion* e);
void detruire_tirs_boss(TirBoss* t);

// Ajoute une bulle dans le tableau a la premiere place libre
void ajouter_bulle(Bulle* tab, int x, int y, int taille, int dir, int couleur);
// Ajoute un tir
void ajouter_tir(Projectile* tab, int x, int y);
// Cree plein de particules pour faire un effet d'explosion
void exploser_particules(Particule* tab, int x, int y, int r, int g, int b);
// Ajoute un popup de score
void ajouter_popup(PopupScore* tab, int x, int y, int valeur);
// Fait tomber un bonus
void ajouter_bonus(Bonus* tab, int x, int y, int type);
// Lance une animation d'explosion
void ajouter_explosion(Explosion* tab, int x, int y, int taille);
// Ajoute un tir du boss
void ajouter_tir_boss(TirBoss* tab, int x, int y);

// Toutes les fonctions de collision (renvoient 1 si collision, 0 sinon)
int collision_bulle_joueur(Bulle* b, Joueur* j);
int collision_tir_bulle(Projectile* t, Bulle* b);
int collision_tir_boss(Projectile* t, Boss* b);
int collision_boss_joueur(Boss* b, Joueur* j);
int collision_bonus_joueur(Bonus* b, Joueur* j);
int collision_tirboss_joueur(TirBoss* t, Joueur* j);

// Initialise le boss (position, vie, etc)
void init_boss(Boss* b);

#endif
