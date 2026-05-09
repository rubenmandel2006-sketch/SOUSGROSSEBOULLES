#include "sauvegarde.h"
#include <stdio.h>
#include <string.h>

// Sauvegarde le pseudo et le niveau a la fin du fichier sauvegarde.bin
void sauvegarder(char* pseudo, int niveau)
{
    FILE* f;
    char buffer_nom[50] = {0};

    // "ab" = on ajoute a la fin en mode binaire
    f = fopen("sauvegarde.bin", "ab");
    if (f == NULL) return;

    // On copie le pseudo dans un buffer de 50 octets pour avoir une taille fixe
    strncpy(buffer_nom, pseudo, 49);

    fwrite(buffer_nom, sizeof(char), 50, f);
    fwrite(&niveau, sizeof(int), 1, f);

    fclose(f);
}

// Cherche dans le fichier le meilleur niveau atteint pour ce pseudo
int charger(char* pseudo)
{
    FILE* f;
    char nom[50];
    int niv;
    int meilleur;

    // Par defaut on est au niveau 1
    meilleur = 1;

    f = fopen("sauvegarde.bin", "rb");
    if (f == NULL) return 1;

    // On parcourt tout le fichier en lisant un pseudo + un niveau a chaque iteration
    while (fread(nom, sizeof(char), 50, f) == 50 && fread(&niv, sizeof(int), 1, f) == 1) {
        if (strcmp(nom, pseudo) == 0) {
            // On garde le plus haut niveau trouve
            if (niv > meilleur) {
                meilleur = niv;
            }
        }
    }

    fclose(f);
    return meilleur;
}

// Lit tous les scores du fichier scores.bin et retourne combien on en a lus
int lire_top_scores(Score* tab)
{
    FILE* f;
    int n;

    f = fopen("scores.bin", "rb");
    if (f == NULL) return 0;

    // fread retourne le nombre d'elements lus
    n = fread(tab, sizeof(Score), MAX_SCORES, f);

    fclose(f);
    return n;
}

// Tri a bulles : on trie les scores du plus grand au plus petit
static void trier_scores(Score* tab, int n)
{
    int i, j;
    Score tmp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            // Si le score d'apres est plus grand on echange les deux
            if (tab[j].score < tab[j + 1].score) {
                tmp = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = tmp;
            }
        }
    }
}

// Ajoute un nouveau score, trie et garde uniquement les MAX_SCORES meilleurs
void ajouter_score(char* pseudo, int score)
{
    /* Tableau de taille MAX_SCORES + 1 : on a la place pour les anciens
       scores + le nouveau qu'on vient d'ajouter */
    Score tab[MAX_SCORES + 1];
    FILE* f;
    int n;

    // On lit les scores actuels
    n = lire_top_scores(tab);

    // On ajoute le nouveau score a la fin du tableau
    memset(tab[n].pseudo, 0, 50);
    strncpy(tab[n].pseudo, pseudo, 49);
    tab[n].score = score;
    n = n + 1;

    trier_scores(tab, n);

    // On garde seulement les MAX_SCORES premiers (les meilleurs)
    if (n > MAX_SCORES) {
        n = MAX_SCORES;
    }

    // On reecrit le fichier en entier en mode "wb" (ecrase l'ancien)
    f = fopen("scores.bin", "wb");
    if (f == NULL) return;

    fwrite(tab, sizeof(Score), n, f);

    fclose(f);
}
