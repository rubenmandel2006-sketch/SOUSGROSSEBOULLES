#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

// Nombre de scores qu'on garde dans le top
#define MAX_SCORES 5

// Structure d'un score (pseudo + valeur du score)
typedef struct {
    char pseudo[50];
    int score;
} Score;

// Sauvegarde la progression du joueur (pseudo + niveau atteint)
void sauvegarder(char* pseudo, int niveau);

// Charge le meilleur niveau atteint pour ce pseudo (1 si pas de sauvegarde)
int charger(char* pseudo);

// Ajoute un score dans le top scores et le sauvegarde
void ajouter_score(char* pseudo, int score);

// Lit le top scores depuis le fichier, retourne le nombre de scores lus
int lire_top_scores(Score* tab);

#endif
