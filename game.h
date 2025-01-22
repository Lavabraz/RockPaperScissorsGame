/**
 *\file game.h
 *\brief Include + def des structures + prototypes
 *\author Yanis Ossedat & Nathan Willay
 * 
 * Ce fichier contient toutes les librairies utilisées dans le programme, 
 * ainsi que les définitions des structure utilisées, 
 * enfin les prototypes de chacune des fonctions présentes (triés par fichier) 
 */


//Inclusion Utiles au Code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Définition des structures du Code
/**
 * \struct Score game.h "Définition Score"
 * \brief File contenant des scores dans l'ordre décroissant
*/
typedef struct score
{
    int nbPts;
    struct score *suiv;
}Score, *Scoreboard;

/**
 * \struct Data game.h "Définition Data"
 * \brief Composé d'un pseudo, d'un nombre de score et des ces scores dans une structure Scoreboard (*Score)
 *        Cette structure permet de stocker tout les scores par joueur dans un tableau de Data
*/
typedef struct
{
    char nom[31];
    int nbScore;
    Scoreboard sPlayer;
}Data;

/**
 * \struct Joueur game.h "Définition Joueur"
 * \brief Composé d'un pseudo, d'un nombre de pv, d'un nombre de pa et d'un score
 *        Cette structure est utilisé pour le joueur jouant une partie afin de stocker ses stats
*/
typedef struct 
{
    char nom[31];
    int pv;
    int pa;
    int score;
}Joueur;

/**
 * \struct Mob game.h "Définition Mob"
 * \brief Composé d'un nom, d'un nombre de pv et d'un nombre de pa
 *        Cette structure est utilisé pour stocker les stats des monstres et pour les mettres dans une file
*/
typedef struct
{
    char nom[21];
    int type;
    int pv;
    int pa;
}Mob;

/**
 * \struct ElementMob game.h "Définition ElementMob"
 * \brief Composé d'un Mob (structure précédente) et d'un pointeur sur un autre ElementMob
 *        Cette structure est un element de la file de mob que l'on définit juste en dessous
*/
typedef struct elementMob
{
    Mob mob;
    struct elementMob *suiv;
}ElementMob;

/**
 * \struct FileMob game.h "Définition FileMob"
 * \brief Composé de deux pointeurs sur des ElementMob (structure précécente) qui sont le début et la fin de la file
 *        Cette structure stock les monstres pour les différentes parties de l'histoire
*/
typedef struct 
{
    ElementMob *start;
    ElementMob *end;
}FileMob;

/**
 * \struct Story game.h "Définition Story"
 * \brief Composé de 4 str chacun étant un texte d'une partie de l'histoire pour une partie.
 *        Composé aussi de 2 FileMob (structure précédente), une pour chaque partie de l'histoire
*/
typedef struct 
{
    char contexte[500];
    char secondPartText[500];
    char deathText[500];
    char winText[500];
    FileMob Part1;
    FileMob Part2;
}Story;

/**
 * \enum Booleen
 * \brief Implémente un type Booléen utile pour les primitives
*/
typedef enum 
{
    faux,
    vrai,
}Booleen;

//------------------------------------------------------------------------------------------------------------------------------

//initialisation.C

/**
 * \brief Affiche le menu et récupère le choix du user parmis les 5 fonctionnalités
 * \return le choix de l'utilisateur
 */
int fChoice(void);

/**
 * \brief Charge le fichier data.bin dans le tableau de pointeur sur des structures Data
 * \param players Tableau de pointeurs sur des structures Data (vide)
 * \return La taille logique du tableau players
 */
int fChargement(Data **players);

/**
 * \brief Sauvegarde le tableau players dans le fichier data.bin
 * \param players Tableau de pointeurs sur des structures Data
 * \param tlog Taille logique de players
 */
void sauvegarde(Data *players, int tlog);

/**
 * \brief Compare les deux armes pour déterminer le gagnant du combat
 * \param pw Arme du joueur
 * \param mw Arme du monstre
 * \return Un code représentant le gagnant du combat
 */
int weaponCompare(char pw, char mw);

/**
 * \brief Réalise un tour d'un combat entre un joueur et un monstre (1 attaque)
 * \param j Structure Joueur contenant les infos du joueur
 * \param m Structure Mob contenant les infos du monstres
 */
void fightMob(Joueur *j, Mob *m);

/**
 * \brief Réalise la première partie des parties
 * \param j Structure Joueur contenant les infos du joueur
 * \param story Structure Story contenant le contenu de la partie (textes/mob)
 */
void combatPhase1(Joueur *j, Story story);

/**
 * \brief Réalise la deuxième partie des parties
 * \param j Structure Joueur contenant les infos du joueur
 * \param story Structure Story contenant le contenu de la partie (textes/mob)
 */
void combatPhase2(Joueur *j, Story story);

/**
 * \brief Gère l'entiéreté de la partie, en appelant notamment les deux fonctions précédentes
 * \param tabData tableau de pointeurs sur structure Data (équivalent à players)
 * \param tlog taille logique de tabData
*/
void fPlayGame(Data **tabData, int *tlog);

/**
 * \brief Charge le fichier voulu dans la structure Story
 * \param story Structure Story à remplir avec le fichier
 * \param file fichier dans lequel lire
*/
void fChargementGameFile(Story *story, FILE *file);

/**
 * \brief Supprime les caractères de saut de ligne dans un str
 * \param texte Le str sur lequel agir
*/
void retireSautLigneCara(char texte[]);

/**
 * \brief Fonctions permettant d'entrer un texte de 500 car (utilisé par fCreateGame)
 * \param flot fichier dans lequel écrire
 * \param part str contenant la partie de l'histoire à écrire
*/
void EnterText500Car(FILE *flot, char part[]);

/**
 * \brief Supprime les espaces dans les str
 * \param chaine le str dans lequel supprimer les espaces
*/
void fSuppSpace(char *chaine);

/**
 * \brief Initialise Story correctement
 * \return Story initialisé
*/
Story fInitStory(void);

/**
 * \brief Permet la création d'un fichier de jeu personnalisé
*/
void fCreateGame(void);

//------------------------------------------------------------------------------------------------------------------------------

//mob.c

/**
 * \brief crée une file vide
 * \return une file vide
*/
FileMob fileVide (void);

/**
 * \brief Vérifie si une file est vide
 * \param file la file à vérifier
 * \return un Booleen 
*/
Booleen estFileVide (FileMob file);

/**
 * \brief Ajout d'un mob en fin de FileMob
 * \param file file de Mob où ajouté le Mob
 * \param mobData structure Mob contenant les infos du monstre
 * \return la FileMob modifiée
*/
FileMob ajoutQueueFileMob (FileMob file, Mob mobData);

/**
 * \brief renvoie le mob du premier élement de la FileMob
 * \param file la file dans lequel regarder
 * \return le mob en tête de file
*/
Mob teteFileMob(FileMob file);

/**
 * \brief renvoie le premier élement de la FileMob
 * \param file la file dans lequel regarder
 * \return l'élement en tête de file
*/
ElementMob teteFile(FileMob file);

/**
 * \brief enleve la tete de la file
 * \param file la file dans lequel regarder
 * \return le mob en tête de file
*/
void couperTete(FileMob *file);

/**
 * \brief met la tête en queue de FileMob
 * \param file la file dans lequel travailler
*/
void bougerTete(FileMob *file);

/**
 * \brief Initialise un Mob
 * \param m le mob à initialisé
 * \param nom le nom du mob à initialisé
 * \param type le type du monstre à initialisé
 * \return le mob en tête de file
*/
Mob initMob(Mob m, char nom[], int type);

/**
 * \brief choisi une arme au hasard pour le monstre
 * \param m le mob pour lequel il faut choisir une arme
 * \return l'arme choisi
*/
char randomWeapon(Mob m);

/**
 * \brief Gère l'ajout des monstres dans le fichier de partie personnalisé
 * \param flot le fichier où il faut écrire
 * \param part prend 1 ou 2 en fonction de la partie de la game qu'on écrit
*/
void fPrintFileMob(FILE *flot, int part);

/**
 * \brief Gère l'ajout d'un monstre dans le fichier de partie personnalisé
 * \param flot le fichier où écrire
*/
void fPrintMob(FILE *flot);

/**
 * \brief ajoute les monstres d'un fichier dans une FileMob
 * \param m le mob à initialiser
 * \param nom le nom du mob à initialiser
*/
void fAjoutMobsFile(FILE *file, FileMob *fileMob);

//------------------------------------------------------------------------------------------------------------------------------


//player.c

/**
 * \brief initialise correctement Joueur avec un nom personnalisé
 * \return le Joueur initialisé
*/
Joueur initialisationJoueur();

/**
 * \brief initialise correctement Data
 * \return la Data initialisée
*/
Data initialisationData(void);

/**
 * \brief ajoute un joueur dans une table de Data
 * \param tabData la table où ajouter le joueur
 * \param tlog la taille logique du tableau
 * \param pseudo le pseudo du joueur à ajouter
 * \param where l'indice dans le tableau où il faut insérer le joueur
*/
void ajouterJoueur(Data **tabData, int *tlog, char pseudo[], int where);

/**
 * \brief affiche le menu de choix de l'arme pour le joueur et renvoie son choix
 * \return la lettre choisi par le joueur
*/
char fChoixWeapon(void);

/**
 * \brief echange deux valeurs de type Data
 * \param a la première valeur à échanger
 * \param b la première valeur à échanger
*/
void permuter(Data *a, Data *b);

/**
 * \brief tri Rapide utilisant la fonction précédente
 * \param tab tableau de Data dans lequel travailler
 * \param first l'indice de la première valeur à trier
 * \param last l'indice de la dernière valeur à trier
*/
void quicksortPseudo(Data tab[], int first, int last);

/**
 * \brief recherche dichotomique d'un pseudo dans une table de Data, renvoie aussi l'indice d'insertion
 * \param tabData tableau où il faut rechercher
 * \param debut l'indice de debut de la table
 * \param fin l'indice de fin de la table
 * \param pseudo le pseudo à rechercher
 * \param trouve variable permettant de savoir si la valeur à été trouvé où nom
 * \return l'emplacement/l'indice d'insertion
*/
int rechercheDichotomiquePseudo(Data *tabData, int debut, int fin, char *pseudo, int *trouve);

/**
 * \brief affiche les joueurs dans l'ordre alphabétique des noms
 * \param tabData tableau d'où prendre les datas
 * \param tailleLog taille logique de tabData
*/
void fPrintPlayersNameSorted(Data *tabData, int tailleLog);

/**
 * \brief affiche les informations d'un joueur en particulier
 * \param tabData tableau d'où prendre les datas
 * \param tailleLog taille logique de tabData
*/
void fPrintPlayerStat(Data tabData[], int tailleLog);

/**
 * \brief affiche les joueurs dans l'ordre décroissant des score
 * \param tab tableau d'où prendre les datas
 * \param tailleLog taille logique de tab
*/
void fPrintPlayersScoreSorted(Data *tab, int tailleLog);

//------------------------------------------------------------------------------------------------------------------------------


//score.c

/**
 * \brief intialise un Scoreboard correctement
 * \return Scoreboard initialisé
*/
Scoreboard fScoreboardVide();

/**
 * \brief ajoute un score en tête d'un Scoreboard
 * \param s Scoreboard à modifier
 * \param x valeur à insérer
*/
Scoreboard fAjoutTete(Scoreboard s, int x);

/**
 * \brief ajoute un score dans un Scoreboard (utilise la fonction précédente)
 * \param s Scoreboard à modifier
 * \param x valeur à insérer
*/
Scoreboard fAjoutScore(Scoreboard s, int x);

/**
 * \brief vérifie si un Scoreboard est vide ou non
 * \param s le Scoreboard à vérifier
 * \return Booléen contenant le résultat
*/
Booleen estVide(Scoreboard s);

/**
 * \brief renvoie le premier score d'un Scoreboard
 * \param s Scoreboard sur lequel travailler
 * \return le premier score du Scoreboard
*/
int teteScoreboard(Scoreboard s);

/**
 * \brief renvoie le dernier score d'un Scoreboard
 * \param s Scoreboard sur lequel travailler
 * \return le dernier score du Scoreboard
*/
int queueScoreboard(Scoreboard s);

/**
 * \brief renvoie la moyenne des scores d'un Scoreboard
 * \param s Scoreboard sur lequel travailler
 * \return la moyenne des scores du Scoreboard
*/
int avgScoreboard(Scoreboard s);

/**
 * \brief utilise le tri Fusion pour trier la table par score décroissant
 * \param tabData la table à modifier
 * \param n la taille du tableau
*/
void triFusionScore(Data tabData[], int n);

/**
 * \brief fusionne deux tableau de Data ensemble (utilisé par la fonction précédente)
 * \param left le premier tableau à fusionner
 * \param sizeLeft taille de left
 * \param right le deuxième tableau à fusionner
 * \param sizeRight taille de right
 * \param tabData table dans laquelle mettre les deux autres une fois triées
*/
void fusion (Data left[], int sizeLeft, Data right[], int sizeRight, Data tabData[]);

/**
 * \brief copie une table de Data dans une autre
 * \param depart les données à copier
 * \param indiceDep l'indice de la première donnée à copier
 * \param tailleDep la taille logique de la table de depart
 * \param destination l'endroit où copier les données
*/
void copier (Data depart[], int indiceDep, int tailleDep, Data destination[]);
