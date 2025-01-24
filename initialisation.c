/**
 *\file initialisation.c
 *\brief Contient les fonctions principales du programme
 *\author Yanis Ossedat & Nathan Willay
 * 
 * Ce fichier contient les fonctions importantes du programme comme le chargement des fichiers ou la sauvegarde.
 * Il contient aussi les fonctions qui ne rentrent pas dans les catégories des autres fichiers
 */

#include "game.h"

int fChoice(void)
{
    int c;
    printf("\n----------------------Menu Principal----------------------\n");
    printf("➀ Jouer une partie prédéfinie\n");
    printf("➁ Créer une nouvelle partie\n");
    printf("➂ Afficher la liste des joueurs triée par nom\n");
    printf("➃ Afficher la liste des joueurs triée par meilleur score\n");
    printf("➄ Afficher les statistiques d'un joueur\n");
    printf("➈ Quitter\n");
    printf("----------------------------------------------------------\n");
    printf("\n--> Votre choix : ");
    scanf("%d%*c", &c);
    return c;
}

int fChargement(Data **players)
{
    int tlog, i, j, x;
    FILE *flot;
    flot = fopen("data.bin", "r");
    if(flot == NULL)
    {
        printf("⚠ Problème d'ouverture du fichier data.bin en lecture.\n");
        return -1;
    }
    fscanf(flot, "%d", &tlog);
    *players = (Data *)malloc(sizeof(Data) * tlog);
    if (*players == NULL){printf("Problème de Malloc.\n"); fclose(flot); return -2;}
    for(i = 0; i < tlog; i++)
    {
        fscanf(flot, "%s %d", (*players)[i].nom, &(*players)[i].nbScore);
        (*players)[i].sPlayer = fScoreboardVide();
        for (j = 0; j < (*players)[i].nbScore; j++)
        {
            fscanf(flot ,"%d", &x);
            (*players)[i].sPlayer = fAjoutScore((*players)[i].sPlayer, x);
        }
    }
    fclose(flot);
    return tlog;
}

void sauvegarde(Data *players, int tlog)
{
    int i, j;
    FILE *flot;
    flot = fopen("data.bin", "w");
    if(flot == NULL){printf("⚠ Problème d'ouverture du fichier data.bin en écriture.\n"); fclose(flot); exit(1);}
    fprintf(flot, "%d\n", tlog);
    for(i=0; i<tlog; i++)
    {
        fprintf(flot, "%s %d\n", players[i].nom, players[i].nbScore);
        for(j=1; j<=players[i].nbScore; j++)
        {
            fprintf(flot, "%d\n", players[i].sPlayer->nbPts);
            players[i].sPlayer = players[i].sPlayer->suiv;
        }
    }
    fclose(flot);
    return;
}

int weaponCompare(char pw, char mw)
{
    if(mw == 'O') return 1;
    if(mw == '#') return 0;
    if(mw == pw) return 5;
    if(pw == 'P' && mw == 'C') return 1;
    if(pw == 'C' && mw == 'F') return 1;
    if(pw == 'F' && mw == 'P') return 1;
    else return 0;
}

void fightMob(Joueur *j, Mob *m)
{
    char playerWeapon, monsterWeapon;
    int winner;
    playerWeapon = fChoixWeapon();
    monsterWeapon = randomWeapon(*m);
    winner = weaponCompare(playerWeapon, monsterWeapon);
    printf("%s a choisi %c et le monstre a choisi %c.\n",j->nom ,playerWeapon, monsterWeapon);
    if(winner == 1)
    {
        printf("Le Chevalier a Gagné !\n");
        j->score += 10;
        m->pv -= j->pa;
        printf("Le monstre a %d points de vie.\n", m->pv);
    }
    else if(winner == 0)
    {
        printf("Oh non, le monstre a gagné !\n");
        j->pv = j->pv - m->pa;
        printf("Le Chevalier a %d points de vie.\n", j->pv);
    }
    else{printf("Combat nul !\n"); return;}
    return;
}

void combatPhase1(Joueur *j, Story story)
{
    FileMob m = story.Part1;
    printf("\n%s\n", story.contexte);
    while (!estFileVide(m) && m.start != NULL)
    {
        printf("Le monstre %s (%dpv, %dpa) s'avance determiné à se battre !\n", teteFileMob(m).nom, teteFileMob(m).pv, teteFileMob(m).pa);
        while(j->pv > 0 && teteFileMob(m).pv > 0)
        {
            fightMob(j, &m.start->mob);
        }
        if (j->pv <= 0)
        {
            printf("%s\n", story.deathText);
            printf("Votre score est de %d.\n", j->score);
            return;
        }
        if (m.start->mob.pv <= 0)
        {
            printf("Vous avez défait %s !\n", teteFileMob(m).nom);
            j->score += 50*m.start->mob.type;
            couperTete(&m);
        }
    }
}

void combatPhase2(Joueur *j, Story story)
{
    FileMob m = story.Part2;
    printf("\n%s\n", story.secondPartText);
    while (!estFileVide(m) && j->pv != 0)
    {
        printf("Le monstre %s (%d pv, %d pa) s'avance pour tenter sa chance !\n", teteFileMob(m).nom, teteFileMob(m).pv, teteFileMob(m).pa);
        fightMob(j, &m.start->mob);
        if (m.start->mob.pv <= 0)
        {
            printf("Vous avez défait %s !\n", teteFileMob(m).nom);
            j->score += 100*m.start->mob.type;
            couperTete(&m);
        }
        else bougerTete(&m);
    }
    if(j->pv <= 0)
    {
        printf("%s\n", story.deathText);
        printf("Votre score est de %d.\n", j->score);
    }
    else
    {
        printf("%s\n", story.winText);
        printf("Vous avez défait tout les monstres !\n");
        printf("Votre score est de %d.\n", j->score);
    }
}

void fChoixPartie(int *choix)
{
    printf("Choisissez une partie parmis les suivantes :\n");
    printf("\t1- Facile\n");
    printf("\t2- Normal\n");
    printf("\t3- Difficile\n");
    printf("\t4- Impossible\n");
    printf("\t5- Personnalisée\n");
    printf("\t6- Retourner au Menu Principal\n");
    printf("--> Votre choix : ");
    scanf("%d%*c", choix);
    while(*choix < 1 || *choix > 6)
    {
        printf("⚠ Choix incorrect, veuillez réessayer : ");
        scanf("%d%*c", choix);
    }
}

void chargerPartiePerso(char gameFile[])
{
    printf("\n--> Nom de la partie (30 char max): ");
    scanf("%s", gameFile);
    strcat(gameFile, ".data");
}

void fPlayGame(Data **tabData, int *tlog)
{
    Story story = fInitStory();
    FILE *flot;
    Joueur j;
    int choix, emplacement = -1, trouve;
    char gameFilePath[45] = "gameFiles/", gameFile[30];
    fChoixPartie(&choix);
    switch (choix)
    {
    case 1:strcat(gameFilePath, "facile.data"); break;
    case 2:strcat(gameFilePath, "normal.data"); break;
    case 3:strcat(gameFilePath, "difficile.data"); break;
    case 4:strcat(gameFilePath, "impossible.data"); break;
    case 5:chargerPartiePerso(gameFile); strcat(gameFilePath, gameFile); break;
    case 6:return;
    }
    printf("Importation du fichier %s...\n", gameFilePath);
    flot = fopen(gameFilePath, "r");
    if (flot == NULL)
    {
        printf("⚠ Le fichier de jeu %s n'existe pas !\n", gameFilePath);
        return;
    }
    fChargementGameFile(&story, flot);
    fclose(flot);
    j = initialisationJoueur();
    combatPhase1(&j, story);
    if (j.pv > 0)
        combatPhase2(&j, story);
    emplacement = rechercheDichotomiquePseudo(*tabData, 0, *tlog - 1, j.nom, &trouve);
    if (trouve == -1)
        ajouterJoueur(tabData, tlog, j.nom, emplacement);
    (*tabData)[emplacement].sPlayer = fAjoutScore((*tabData)[emplacement].sPlayer, j.score);
    (*tabData)[emplacement].nbScore++;
    printf("Fin de la partie.\n");
}

void fChargementGameFile(Story *story, FILE *file)
{
    fgets(story->contexte, 500, file);
    retireSautLigneCara(story->contexte);
    fgets(story->secondPartText, 500, file);
    retireSautLigneCara(story->secondPartText);
    fgets(story->deathText, 500, file);
    retireSautLigneCara(story->deathText);
    fgets(story->winText, 500, file);
    retireSautLigneCara(story->winText);
    fAjoutMobsFile(file, &(story->Part1));
    fAjoutMobsFile(file, &(story->Part2));
}

void retireSautLigneCara(char texte[])
{
    int i = 0;
    while(texte[i] != '\0')
    {
        if(texte[i] == '\n')
            texte[i] = '\0';
        i++;
    }
}

void fCreateGame(void)
{
    char confirm;
    char gameName[40], gamePath[40]="gameFiles/";
    FILE *flot;
    printf("--> Voulez-vous créer une nouvelle partie ? (o/n) : ");
    scanf("%c%*c", &confirm);
    if (confirm == 'n') 
        return;
    
    printf("Veuillez choisir un nom pour votre partie, maximum 20 caractères\n");        
    printf("\t\t⚠ ATTENTION ⚠\nSi ce nom est déjà utilisé, l'ancienne partie sera écrasée\n");
    printf("--> Entrez le nom de la partie : ");
    fgets(gameName, 21, stdin);
    fSuppSpace(gameName);
    strcat(gamePath, gameName);
    retireSautLigneCara(gamePath);
    strcat(gamePath, ".data");
    flot = fopen(gamePath, "w");

    EnterText500Car(flot, "contexte");
    EnterText500Car(flot, "secondPartText");
    EnterText500Car(flot, "deathText");
    EnterText500Car(flot, "winText");
    
    fPrintFileMob(flot, 1);
    fPrintFileMob(flot, 2);
    fprintf(flot, "\n");
}

void EnterText500Car(FILE *flot, char part[])
{
    char textPart[500];
    printf("--> Entrez le texte de %s (maximum 499 caractères) : ", part);
    fgets(textPart, 500, stdin);
    fprintf(flot, "%s", textPart);
}

void fSuppSpace(char *chaine)
{
    int i = 0;
    while(chaine[i] != '\0')
    {
        if(chaine[i] == ' ')
            chaine[i] = '_';
        i++;
    }
}

Story fInitStory(void)
{
    Story story={"contexte", "secondPartText", "deathText", "winText", fileVide(), fileVide()};
    return story;
}
