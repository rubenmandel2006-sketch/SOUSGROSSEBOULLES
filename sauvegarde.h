#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

#define MAX_SCORES 5

typedef struct {
    char pseudo[50];
    int score;
} Score;

void sauvegarder(char* pseudo, int niveau);

int charger(char* pseudo);

void ajouter_score(char* pseudo, int score);

int lire_top_scores(Score* tab);

#endif
