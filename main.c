#include <allegro.h>
#include "init.h"
#include "menu.h"
#include "jeu.h"
#include "sauvegarde.h"

// Point d'entree du programme : boucle sur le menu principal jusqu'a quitter
int main(void)
{
    int choix;
    int fini;
    char pseudo[50];
    int niveau;

    // Init Allegro et chargement de toutes les images
    init_allegro();
    charger_images();

    fini = 0;

    while (fini == 0) {
        choix = menu_principal();

        // On attend que entree soit relachee pour pas valider direct dans la suite
        while (key[KEY_ENTER]) {
            rest(10);
        }

        // 1 : afficher les regles
        if (choix == 1) {
            afficher_regles();
        }
        // 2 : nouvelle partie depuis le niveau 1
        if (choix == 2) {
            saisir_pseudo(pseudo);
            jouer_partie(pseudo, 1);
        }
        // 3 : reprendre depuis le dernier niveau atteint
        if (choix == 3) {
            saisir_pseudo(pseudo);
            niveau = charger(pseudo);
            jouer_partie(pseudo, niveau);
        }
        // 4 : afficher le top scores
        if (choix == 4) {
            afficher_top_scores();
        }
        // 5 : quitter
        if (choix == 5) {
            fini = 1;
        }
    }

    // On libere les images et on ferme Allegro proprement
    liberer_images();
    allegro_exit();

    return 0;
}
END_OF_MAIN()
