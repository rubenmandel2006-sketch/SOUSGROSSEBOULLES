#include "sauvegarde.h"
#include <stdio.h>
#include <string.h>

void sauvegarder(char* pseudo, int niveau)
{
    FILE* f;

    f = fopen("sauvegarde.txt", "a");
    if (f == NULL) return;

    fprintf(f, "%s %d\n", pseudo, niveau);
    fclose(f);
}

int charger(char* pseudo)
{
    FILE* f;
    char nom[50];
    int niv;
    int meilleur;

    meilleur = 1;

    f = fopen("sauvegarde.txt", "r");
    if (f == NULL) return 1;

    while (fscanf(f, "%s %d", nom, &niv) == 2) {
        if (strcmp(nom, pseudo) == 0) {
            if (niv > meilleur) {
                meilleur = niv;
            }
        }
    }

    fclose(f);
    return meilleur;
}

int lire_top_scores(Score* tab)
{
    FILE* f;
    int n;

    n = 0;

    f = fopen("scores.txt", "r");
    if (f == NULL) return 0;

    while (n < MAX_SCORES && fscanf(f, "%s %d", tab[n].pseudo, &tab[n].score) == 2) {
        n = n + 1;
    }
    fclose(f);
    return n;
}

static void trier_scores(Score* tab, int n)
{
    int i, j;
    Score tmp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (tab[j].score < tab[j + 1].score) {
                tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
            }
        }
    }
}

void ajouter_score(char* pseudo, int score)
{
    Score tab[MAX_SCORES + 1];
    FILE* f;
    int n;
    int i;

    n = lire_top_scores(tab);

    strcpy(tab[n].pseudo, pseudo);
    tab[n].score = score;
    n = n + 1;

    trier_scores(tab, n);

    if (n > MAX_SCORES) {
        n = MAX_SCORES;
    }

    f = fopen("scores.txt", "w");
    if (f == NULL) return;
    for (i = 0; i < n; i++) {
        fprintf(f, "%s %d\n", tab[i].pseudo, tab[i].score);
    }
    fclose(f);
}
