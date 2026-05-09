#include "init.h"
#include <time.h>
#include <stdlib.h>

BITMAP* page = NULL;

BITMAP* fonds[4];

BITMAP* img_victoire = NULL;
BITMAP* img_defaite = NULL;

BITMAP* marche_droite[3];
BITMAP* marche_gauche[3];
BITMAP* saut_haut[3];
BITMAP* saut_droite[2];
BITMAP* saut_gauche[2];
BITMAP* mort_img = NULL;

BITMAP* bulles_img[3];

BITMAP* tir_img[3];

BITMAP* explosion_img[3];

BITMAP* bonus_img[3];

BITMAP* boss_g = NULL;
BITMAP* boss_d = NULL;

BITMAP* tirboss_img[3];

void init_allegro(void)
{
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();

    set_color_depth(desktop_color_depth());

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, LARGEUR, HAUTEUR, 0, 0) != 0) {
        allegro_message("probleme mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    page = create_bitmap(LARGEUR, HAUTEUR);
    clear_bitmap(page);

    srand((unsigned)time(NULL));
}

static void mettre_zero(void)
{
    int i;
    img_victoire = NULL;
    img_defaite = NULL;
    mort_img = NULL;
    boss_g = NULL;
    boss_d = NULL;
    for (i = 0; i < 4; i++) {
        fonds[i] = NULL;
    }
    for (i = 0; i < 3; i++) {
        marche_droite[i] = NULL;
        marche_gauche[i] = NULL;
        saut_haut[i] = NULL;
        bulles_img[i] = NULL;
        tir_img[i] = NULL;
        explosion_img[i] = NULL;
        bonus_img[i] = NULL;
        tirboss_img[i] = NULL;
    }
    for (i = 0; i < 2; i++) {
        saut_droite[i] = NULL;
        saut_gauche[i] = NULL;
    }
}

void charger_images(void)
{
    mettre_zero();

    fonds[0] = load_bitmap("images/fond1.bmp", NULL);
    fonds[1] = load_bitmap("images/fond2.bmp", NULL);
    fonds[2] = load_bitmap("images/fond3.bmp", NULL);
    fonds[3] = load_bitmap("images/fond4.bmp", NULL);

    img_victoire = load_bitmap("images/victoire.bmp", NULL);
    img_defaite = load_bitmap("images/defaite.bmp", NULL);

    marche_droite[0] = load_bitmap("images/md0.bmp", NULL);
    marche_droite[1] = load_bitmap("images/md1.bmp", NULL);
    marche_droite[2] = load_bitmap("images/md2.bmp", NULL);

    marche_gauche[0] = load_bitmap("images/mg0.bmp", NULL);
    marche_gauche[1] = load_bitmap("images/mg1.bmp", NULL);
    marche_gauche[2] = load_bitmap("images/mg2.bmp", NULL);

    saut_haut[0] = load_bitmap("images/sh0.bmp", NULL);
    saut_haut[1] = load_bitmap("images/sh1.bmp", NULL);
    saut_haut[2] = load_bitmap("images/sh2.bmp", NULL);

    saut_droite[0] = load_bitmap("images/sd0.bmp", NULL);
    saut_droite[1] = load_bitmap("images/sd1.bmp", NULL);

    saut_gauche[0] = load_bitmap("images/sg0.bmp", NULL);
    saut_gauche[1] = load_bitmap("images/sg1.bmp", NULL);

    mort_img = load_bitmap("images/mort.bmp", NULL);

    bulles_img[0] = load_bitmap("images/bulle_p.bmp", NULL);
    bulles_img[1] = load_bitmap("images/bulle_m.bmp", NULL);
    bulles_img[2] = load_bitmap("images/bulle_g.bmp", NULL);

    tir_img[0] = load_bitmap("images/tir0.bmp", NULL);
    tir_img[1] = load_bitmap("images/tir1.bmp", NULL);
    tir_img[2] = load_bitmap("images/tir2.bmp", NULL);

    explosion_img[0] = load_bitmap("images/explosion0.bmp", NULL);
    explosion_img[1] = load_bitmap("images/explosion1.bmp", NULL);
    explosion_img[2] = load_bitmap("images/explosion2.bmp", NULL);

    bonus_img[0] = load_bitmap("images/bx2.bmp", NULL);
    bonus_img[1] = load_bitmap("images/bx3.bmp", NULL);
    bonus_img[2] = load_bitmap("images/bexplosion.bmp", NULL);

    boss_g = load_bitmap("images/bossg.bmp", NULL);
    boss_d = load_bitmap("images/bossd.bmp", NULL);

    tirboss_img[0] = load_bitmap("images/tirb0.bmp", NULL);
    tirboss_img[1] = load_bitmap("images/tirb1.bmp", NULL);
    tirboss_img[2] = load_bitmap("images/tirb2.bmp", NULL);
}
//rtsd
static void liberer(BITMAP** b)
{
    if (*b != NULL) {
        destroy_bitmap(*b);
        *b = NULL;
    }
}

void liberer_images(void)
{
    int i;
    liberer(&page);
    liberer(&img_victoire);
    liberer(&img_defaite);
    liberer(&mort_img);
    liberer(&boss_g);
    liberer(&boss_d);

    for (i = 0; i < 4; i++) {
        liberer(&fonds[i]);
    }
    for (i = 0; i < 3; i++) {
        liberer(&marche_droite[i]);
        liberer(&marche_gauche[i]);
        liberer(&saut_haut[i]);
        liberer(&bulles_img[i]);
        liberer(&tir_img[i]);
        liberer(&explosion_img[i]);
        liberer(&bonus_img[i]);
        liberer(&tirboss_img[i]);
    }
    for (i = 0; i < 2; i++) {
        liberer(&saut_droite[i]);
        liberer(&saut_gauche[i]);
    }
}
