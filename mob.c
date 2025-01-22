/**
 *\file mob.c
 *\brief Contient toutes les fonctions en rapport avec les monstres 
 *\author Yanis Ossedat & Nathan Willay
 */

#include "game.h"

FileMob fileVide (void)
{
    FileMob file={NULL, NULL};
    return file;
}

Booleen estFileVide (FileMob file)
{
    if (file.start == NULL && file.end == NULL)
        return 1;
    return 0;
}

FileMob ajoutQueueFileMob (FileMob file, Mob mobData)
{
    ElementMob *mob;
    mob = (ElementMob *)malloc(sizeof(ElementMob));
    if (mob == NULL)
    {
        printf("\n⚠ Probleme malloc ajoutQueueFileMob %s", mobData.nom);
        exit(1);
    }
    mob->mob = mobData;
    mob->suiv = NULL;
    if (estFileVide(file))
    {
        file.end = mob;
        file.start = mob;
        return file;
    }
    file.end->suiv = mob;
    file.end = mob;
    return file;
}

Mob teteFileMob(FileMob file)
{
    return file.start->mob;
}

ElementMob teteFile(FileMob file)
{
    return *(file.start);
}

void couperTete(FileMob *file)
{
    if (estFileVide(*file))
    {
        printf("\n⚠ Probleme couperTete");
        exit(1);
    }
    if (teteFile(*file).suiv == NULL)
    {
        free(file->start);
        file->start = NULL;
        file->end = NULL;
        return;
    }
    ElementMob *tmp = file->start;
    file->start = file->start->suiv;
    free(tmp);
    return;
}

void bougerTete(FileMob *file)
{
    if (estFileVide(*file))
    {
        printf("\n⚠ Probleme bougerTete");
        exit(1);
    }
    if (teteFile(*file).suiv == NULL)
    {
        return;
    }
    ElementMob *tmp = file->start;
    file->start = file->start->suiv;
    file->end->suiv = tmp;
    file->end = tmp;
    file->end->suiv = NULL;
    return;
}

Mob initMob(Mob m, char nom[], int type)
{
    strcpy(m.nom, nom);
    if (type < 1 || type > 3)
    {
        printf("⚠ Problème avec le type %d du monstre %s. (opération interdite)\n",type ,nom);
        exit(1);
    }
    if(type == 1)
    {
        m.type = 1;
        m.pv = 4;
        m.pa = 1;
    }
    else if(type == 2)
    {
        m.type = 2;
        m.pv = 6;
        m.pa = 1;
    }
    else
    {
        m.type = 3;
        m.pv = 4;
        m.pa = 2;
    }
    return m;
}

char randomWeapon(Mob m)
{
    char weapon[5];
    srand(time(NULL));
    weapon[0] = 'P';
    weapon[1] = 'F';
    weapon[2] = 'C';
    weapon[3] = 'O';
    weapon[4] = '#';
    
    if(m.type == 1) return weapon[rand() % 4];
    if(m.type == 2) return weapon[rand() % 3];
    else return weapon[rand() % 5];
}

void fPrintFileMob(FILE *flot, int part)
{
    int nbrMob, i;
    if (part == 1)
        printf("\n--> Entrez le nombre de monstres de la première partie : ");
    else
        printf("--> Entrez le nombre de monstres de la deuxième partie : ");
    scanf("%d", &nbrMob);
    fprintf(flot, "%d", nbrMob);
    if (nbrMob > 0) fprintf(flot, "\n");
    for (i=1; i<=nbrMob; i++)
    {
        fPrintMob(flot);
    }
}

void fPrintMob(FILE *flot) 
{
    char nom[21];
    int type;
    printf("--> Quel est le nom du monstre (max. 20 car.) : ");
    scanf("%s", nom);
    printf("--> Quel est le type de %s (1/2/3) : ", nom);
    scanf("%d", &type);
    while (type != 1 && type != 2 && type != 3)
    {
        printf("⚠ Type incorrect pour %s !\n--> Réessayez : ", nom);
        scanf("%d", &type);
    }
    fprintf(flot, "%s\n%d\n", nom, type);
}

void fAjoutMobsFile(FILE *file, FileMob *fileMob)
{
    Mob mob;
    int i;
    char nom[21];
    int nbrMobPart, type;
    fscanf(file, "%d", &nbrMobPart);
    *fileMob = fileVide();
    for (i=1; i<=nbrMobPart; i++)
    {
        fscanf(file, "%s", nom);
        fscanf(file, "%d", &type);
        mob = initMob(mob, nom, type);
        *fileMob = ajoutQueueFileMob(*fileMob, mob);
    }
}