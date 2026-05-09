#include <allegro.h>
#include "init.h"
#include "menu.h"
#include "jeu.h"
#include "sauvegarde.h"

int main(void)
{
    int choix;
    int fini;
    char pseudo[50];
    int niveau;

    init_allegro();
    charger_images();

    fini = 0;

    while (fini == 0) {
        choix = menu_principal();

        while (key[KEY_ENTER]) {
            rest(10);
        }

        if (choix == 1) {
            afficher_regles();
        }
        if (choix == 2) {
            saisir_pseudo(pseudo);
            jouer_partie(pseudo, 1);
        }
        if (choix == 3) {
            saisir_pseudo(pseudo);
            niveau = charger(pseudo);
            jouer_partie(pseudo, niveau);
        }
        if (choix == 4) {
            afficher_top_scores();
        }
        if (choix == 5) {
            fini = 1;
        }
    }

    liberer_images();
    allegro_exit();

    return 0;
}
END_OF_MAIN()
