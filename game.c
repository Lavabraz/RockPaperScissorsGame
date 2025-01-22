/**
 *\file game.c
 *\brief Main du programme
 *\author Yanis Ossedat & Nathan Willay
 * 
 * Ce fichier contient la fonction principale du programme, c'est elle qui appelle toute les autres.
 */

#include "game.h"

int main(void)
{
    int choice, tailleLog = -1;
    Data *tabData = NULL;
    tailleLog = fChargement(&tabData);
    if (tailleLog < 0) {
        printf("Erreur lors du chargement des donnés.\n");
        return 1;
    }
    choice = fChoice();
    while (choice != 9)
    {
        switch (choice)
        {
            case 1 : fPlayGame(&tabData, &tailleLog); break;
            case 2 : fCreateGame(); break;
            case 3 : fPrintPlayersNameSorted(tabData, tailleLog); break;
            case 4 : fPrintPlayersScoreSorted(tabData, tailleLog); break;
            case 5 : fPrintPlayerStat(tabData, tailleLog); break;
        }
        choice = fChoice();
    }
    printf("----------------------------------------------------------\n");
    printf("\t\tA la revoyure, Aventurier !\n");
    printf("----------------------------------------------------------\n");
    sauvegarde(tabData, tailleLog);
    free(tabData);
    return 0;
}
