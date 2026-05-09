#ifndef MENU_H
#define MENU_H

// Affiche le menu principal et retourne le choix du joueur (1 a 5)
int menu_principal(void);

// Demande au joueur de taper son pseudo et le met dans pseudo
void saisir_pseudo(char* pseudo);

// Affiche l'ecran des regles du jeu
void afficher_regles(void);

// Affiche le top 5 des meilleurs scores
void afficher_top_scores(void);

#endif
