/**
 *\file score.c
 *\brief Contient toutes les fonctions en rapport avec le/les scores
 *\author Yanis Ossedat & Nathan Willay
 */

#include "game.h"

Scoreboard fScoreboardVide()
{
    return NULL;
}

Scoreboard fAjoutTete(Scoreboard s, int x)
{
    Scoreboard aux = (Score *)malloc(sizeof(Score));
    if (aux == NULL)
    {
        printf("Problème malloc fAjouterTete");
        exit(1);
    }
    aux->nbPts = x;
    aux->suiv = s;
    return aux;
}

Scoreboard fAjoutScore(Scoreboard s, int x)
{
    if (s == NULL)
        return fAjoutTete(s, x);
    if (x >= s->nbPts)
        return fAjoutTete(s, x);
    s->suiv = fAjoutScore(s->suiv, x);
    return s;
}

Booleen estVide(Scoreboard s)
{
    return s == NULL;
}

int teteScoreboard(Scoreboard s)
{
    return s->nbPts;
}

int queueScoreboard(Scoreboard s)
{
    if (s->suiv == NULL)
        return s->nbPts;
    return queueScoreboard(s->suiv);
}

int avgScoreboard(Scoreboard s)
{
    int i=0, total=0;
    while (s != NULL)
    {
        total += s->nbPts;
        s=s->suiv;
        i++;
    }
    return total / i;
}

void triFusionScore(Data tabData[], int n)
{
    Data *left, *right;
    if (n==1)
        return;
    left = (Data *)malloc((n/2)*sizeof(Data));
    right = (Data *)malloc((n - n/2)*sizeof(Data));
    if (left==NULL || right==NULL)
    {
        printf("Erreur malloc");
        exit(1);
    }
    copier(tabData, 0, n/2, left);
    copier(tabData, n/2, n, right);
    triFusionScore(left, n/2);
    triFusionScore(right, n-n/2);
    fusion(left, n/2, right, n- n/2, tabData);
    free(left);
    free(right);
}

void fusion (Data left[], int sizeLeft, Data right[], int sizeRight, Data tabData[])
{
    int indiceLeft=0, indiceRight=0, indiceResult=0;
    while (indiceLeft<sizeLeft && indiceRight<sizeRight)
    {
        if (left[indiceLeft].sPlayer->nbPts > right[indiceRight].sPlayer->nbPts)
        {
            tabData[indiceResult]=left[indiceLeft];
            indiceLeft++;
            indiceResult++;
        }
        else 
        {
            tabData[indiceResult]=right[indiceRight];
            indiceRight++;
            indiceResult++;
        }
    }
    while (indiceLeft<sizeLeft)
    {
        tabData[indiceResult]=left[indiceLeft];
        indiceLeft++;
        indiceResult++;
    }
    while (indiceRight<sizeRight)
    {
        tabData[indiceResult]=right[indiceRight];
        indiceRight++;
        indiceResult++;
    }
}

void copier (Data depart[], int indiceDep, int tailleDep, Data destination[])
{
    int indiceDest = 0;
    while (indiceDep < tailleDep)
    {
        destination[indiceDest] = depart[indiceDep];
        indiceDep++;
        indiceDest++;
    }
}

