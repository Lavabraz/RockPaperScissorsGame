/**
 *\file player.c
 *\brief Contient toutes les fonctions en rapport avec le/les joueurs
 *\author Yanis Ossedat & Nathan Willay
 */

#include "game.h"

Joueur initialisationJoueur(void)
{
    Joueur j;
    printf("\n--> Choisissez un nom (4-30 caractères): ");
    scanf("%s%*c", j.nom);
    while (strlen(j.nom) < 4 || strlen(j.nom) > 30)
    {
        printf("⚠ Le nom doit contenir entre 4 et 30 caractères.\n");
        printf("--> Choisissez un nom (4-30 caractères): ");
        scanf("%s%*c", j.nom);
    }
    j.pv = 20;
    j.pa = 1;
    j.score = 0;
    return j;
}

Data initialisationData(void)
{
    Data d = {"init", 0, fScoreboardVide()};
    return d;
}

void ajouterJoueur(Data **tabData, int *tlog, char pseudo[], int where)
{
    Data d = initialisationData();
    Data *temp = (Data *)realloc(*tabData, (*tlog + 1) * sizeof(Data));
    if (temp == NULL)
    {
        printf("⚠ Erreur d'allocation de mémoire.\n");
        exit(1);
    }
    *tabData = temp;
    strcpy(d.nom, pseudo);

    for (int i = *tlog; i > where; i--)
        (*tabData)[i] = (*tabData)[i - 1];
    (*tabData)[where] = d;
    (*tlog) += 1;
}

char fChoixWeapon(void)
{
    char weapon;
    printf("\n   Vous devez à présent choisir une arme !\n");
    printf("▶------------------------------------------◀\n");
    printf("\tPierre (Gagne contre C et O)\n");
    printf("\tFeuille (Gagne contre P et O)\n");
    printf("\tCiseaux (Gagne contre F et O)\n");
    printf("▶------------------------------------------◀\n");
    printf("--> Votre choix : ");
    scanf("%c%*c", &weapon);
    while(weapon != 'P' && weapon != 'F' && weapon != 'C')
    {
        printf("Vous ne pouvez pas utiliser cette arme !\n");
        printf("--> Veuillez choisir une arme parmis (P,F,C) : ");
        scanf("%c%*c", &weapon);
    }
    return weapon;
}

void permuter(Data *a, Data *b)
{
    Data tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void quicksortPseudo(Data tab[], int first, int last)
{
    int pivot, i, j;
    if (first < last) 
    {
        pivot = first;
        i = first;
        j = last;
        while (i < j)
        {
            while (strcmp(tab[i].nom, tab[pivot].nom) <= 0 && i < last)
                i++;
            while (strcmp(tab[j].nom, tab[pivot].nom) > 0)
                j--;
            if (i < j)
                permuter(&tab[i], &tab[j]);
        }
        permuter(&tab[pivot], &tab[j]);
        quicksortPseudo(tab, first, j-1);
        quicksortPseudo(tab, j+1, last);
    }
}

int rechercheDichotomiquePseudo(Data *tabData, int debut, int fin, char *pseudo, int *trouve)
{
    int middle;
    int cmp;
    while (debut <= fin && fin != -1) {
        middle = (debut + fin) / 2;
        cmp = strcmp(tabData[middle].nom, pseudo);
        if (cmp == 0) 
        {
            *trouve = 1;
            return middle;
        } 
        else if (cmp < 0)
            debut = middle + 1;
        else
            fin = middle - 1;
    }
    *trouve = -1;
    return fin + 1;
}


void fPrintPlayersNameSorted(Data *tabData, int tailleLog)
{
    if (tailleLog == 0)
    {
        printf("----------------------------------------------------------\n");
        printf("\t---\tPersonne n'a joué pour l'instant\t---\n");
        printf("----------------------------------------------------------\n");
        printf("--> Tappez sur entrer pour continuer...\n");
        scanf("%*c");
        return;
    }

    if (tabData == NULL) {
        fprintf(stderr, "⚠ Erreur, tabData est NULL\n");
        return;
    }

    quicksortPseudo(tabData, 0, tailleLog - 1);
    printf("----------------------------------------------------------\n");
    printf("\t\tPseudo\t\tMeilleur Score\n");
    for (int i=0; i!=tailleLog; i++)
        printf("\t\t%s :\t\t\t%d\n", tabData[i].nom, tabData[i].sPlayer->nbPts);
    printf("----------------------------------------------------------\n");
    printf("--> Tappez sur entrer pour continuer...\n");
    scanf("%*c");
}

void fPrintPlayerStat(Data tabData[], int tailleLog)
{
    if (tailleLog == 0)
    {
        printf("----------------------------------------------------------\n");
        printf("\t---\tPersonne n'a joué pour l'instant\t---\n");
        printf("----------------------------------------------------------\n");
        printf("--> Tappez sur entrer pour continuer...\n");
        scanf("%*c");
        return;
    }

    if (tabData == NULL) {
        fprintf(stderr, "⚠ Erreur, tabData est NULL\n");
        return;
    }

    int place, trouve;
    char player[31];
    printf("--> Joueur dont tu veux voir les statistiques : ");
    scanf("%s%*c", player);
    place = rechercheDichotomiquePseudo(tabData, 0, tailleLog, player, &trouve);
    if (trouve == -1)
    {
        printf("---\t%s n'a pas encore joué\t---\n", player);
        printf("----------------------------------------------------------\n");
        printf("--> Tappez sur entrer pour continuer...\n");
        scanf("%*c");
        return;
    }
    printf("----------------------------------------------------------\n");
    printf("\t\t\t---  %s  ---\n", tabData[place].nom);
    printf("\t\tNombre de parties joué : %d\n", tabData[place].nbScore);
    printf("\t\tRecord : %d\n", teteScoreboard(tabData[place].sPlayer));
    printf("\t\tPire Score : %d\n", queueScoreboard(tabData[place].sPlayer));
    printf("\t\tScore moyen : %d\n", avgScoreboard(tabData[place].sPlayer));
    printf("----------------------------------------------------------\n");
    printf("--> Tappez sur entrer pour continuer...\n");
    scanf("%*c");
}

void fPrintPlayersScoreSorted(Data *tab, int tailleLog)
{
    Data tabDataTemp[tailleLog];
    int i;
    for (i=0; i!=tailleLog; i++)
        tabDataTemp[i] = tab[i];
    if (tailleLog == 0)
    {
        printf("----------------------------------------------------------\n");
        printf("\t---\tPersonne n'a joué pour l'instant\t---\n");
        printf("----------------------------------------------------------\n");
        printf("--> Tappez sur entrer pour continuer...\n");
        scanf("%*c");
        return;
    }
    triFusionScore(tabDataTemp, tailleLog);
    printf("----------------------------------------------------------\n");
    printf("\t\tPseudo\t\tMeilleur score\n");
    for (i=0; i!=tailleLog; i++)
        printf("\t\t%s :\t\t\t%d\n", tabDataTemp[i].nom, tabDataTemp[i].sPlayer->nbPts);
    printf("----------------------------------------------------------\n");
    printf("--> Tappez sur entrer pour continuer...\n");
    scanf("%*c");
}
