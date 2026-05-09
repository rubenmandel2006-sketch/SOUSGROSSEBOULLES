#include "sauvegarde.h"
#include <stdio.h>
#include <string.h>

void sauvegarder(char* pseudo, int niveau)
{
    FILE* f;
    char buffer_nom[50] = {0};

    f = fopen("sauvegarde.bin", "ab");
    if (f == NULL) return;

    strncpy(buffer_nom, pseudo, 49);

    fwrite(buffer_nom, sizeof(char), 50, f);
    fwrite(&niveau, sizeof(int), 1, f);

    fclose(f);
}

int charger(char* pseudo)
{
    FILE* f;
    char nom[50];
    int niv;
    int meilleur;

    meilleur = 1;

    f = fopen("sauvegarde.bin", "rb");
    if (f == NULL) return 1;

    while (fread(nom, sizeof(char), 50, f) == 50 && fread(&niv, sizeof(int), 1, f) == 1) {
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

    f = fopen("scores.bin", "rb");
    if (f == NULL) return 0;

    n = fread(tab, sizeof(Score), MAX_SCORES, f);

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

    n = lire_top_scores(tab);

    memset(tab[n].pseudo, 0, 50);
    strncpy(tab[n].pseudo, pseudo, 49);
    tab[n].score = score;
    n = n + 1;

    trier_scores(tab, n);

    if (n > MAX_SCORES) {
        n = MAX_SCORES;
    }

    f = fopen("scores.bin", "wb");
    if (f == NULL) return;

    fwrite(tab, sizeof(Score), n, f);

    fclose(f);
}