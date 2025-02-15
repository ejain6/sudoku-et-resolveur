#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// déclaration de la grille

#define N 3
#define TAILLE N*N
#define NB_LIGNES TAILLE
#define NB_COLONNES TAILLE
typedef int tGrille[NB_LIGNES][NB_COLONNES];


//déclaration des procédures / fonctions

void chargerGrille(tGrille grille);

void afficherGrille(tGrille grille);

void saisir(int *nb);

bool possible(tGrille grille, int numLig, int numCol, int val);

bool grillePleine(tGrille grille);


int nbElementsLigne(tGrille g, int lig);

int nbElementsColonne(tGrille g, int col);


int main(){

    //déclaration des variables
    tGrille grille1;
    int numLigne, numColonne;
    int valeur;
    int remplie;

    remplie = false;

    chargerGrille(grille1);

    //la boucle suivante en commentaire sert a initialiser une grille de taille N*N vide (pratique pour tester avec différentes valeurs de N)
    /*
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            grille1[i][j] = 0;
        } 
    }
    */
    
    //on joue tant que la grille n'est pas pleine
    while (remplie == false)
    {
        afficherGrille(grille1);

        printf("entrez le numéeo de ligne\n");
        saisir(&numLigne);
        //on décrémente le numéro de la ligne car les tableaux commencent à l'indice 0 en C
        numLigne--;

        printf("entrez le numéro de colonne\n");
        saisir(&numColonne);
        //pareil que pour numLigne
        numColonne--;

        //on vérifie que la case est vide
        if(grille1[numLigne][numColonne] != 0)
        {
            printf("%d\n", grille1[numLigne][numColonne]);
            printf("IMPOSSIBLE, la case n’est pas vide.\n");
        }
        else
        {
            printf("valeur a insérer ?\n");
            saisir(&valeur);

            if(possible(grille1, numLigne, numColonne, valeur) == true)
            {
                grille1[numLigne][numColonne] = valeur;
            }
        }
        remplie = grillePleine(grille1);
    }
    
    afficherGrille(grille1);
    printf("\n Bravo ! Vous avez réussi a finir la grille de Sudoku !\n");
   
    return EXIT_SUCCESS;
}


void chargerGrille(tGrille grille){
    char nomFichier[30];
    int fichierTrouve = 0;
    FILE * f;
    while (fichierTrouve == 0)
    {
        printf("Nom du fichier ?\n");
        scanf("%s", nomFichier);
        f = fopen(nomFichier, "rb");
        if (f==NULL){
            printf("\n ERREUR sur le fichier %s\n", nomFichier);
        } 
        else {
            fread(grille, sizeof(int), TAILLE*TAILLE, f);
            fclose(f);
            fichierTrouve = 1;
        }
    }
}


void afficherGrille(tGrille grille){

    int ligActu;

    //affichage de la ligne d'en-tete des colonnes (différent pour les nombres a 2 chiffres)
    printf("\n   ");
    for (int teteCol = 1; teteCol <= TAILLE; teteCol++)
    {
        if (teteCol < 9)
        {
            printf(" %d ", teteCol);
        }
        else
        {
            printf(" %d", teteCol);
        }

        if (teteCol % N == 0)
        {
            printf(" ");
        }
    }

    //affichage de la bordure supérieure de la grille
    printf("\n  +");
        for (int i = 1; i <= TAILLE*3; i++)
        {
            printf("-");
            if (i%(3*N) == 0)
            {
                printf("+");
            }  
        }
        printf("\n");
    

    //boucle affichant tout le reste de la grille: on affiche N fois des lignes avec N*N valeurs, puis la délimitation des cases
    for (int lig = 0; lig < N; lig++)
    {

        // affichage de N lignes de valeurs
        for (int ligN = 0; ligN < (N); ligN++)
        {
            //affichage du numéro de ligne (différent pour les nombres a 2 chiffres)
            ligActu = N*lig+ligN;
            if (ligActu < 9)
            {
                printf("%d |", ligActu + 1);
            }
            else
            {
                printf("%d|", ligActu + 1);
            }
            

            //affichage de chaque case dans une boucle 
            for (int col = 0; col < TAILLE; col++)
            {
                //test si une valeur est présente, si oui elle est affichée (avec seulement un espace si elle prend 2 espaces), sionon un . a la place
                if (grille[ligActu][col] == 0)
                {
                    printf(" . ");
                }
                else if(grille[ligActu][col] < 10)
                {
                    printf(" %d ", grille[ligActu][col]);
                }
                else
                {
                    printf(" %d", grille[ligActu][col]);
                }
                
                
                //toute les N cases, on affiche un | pour la délimitation en N*N de la grille
                if ((col+1) % N == 0)
                {
                    printf("|");
                }
            }

            //===================================================== modification exercice TP noté sudoku (affichage du nombre d'éléments par ligne) =====================================================
            printf(" (%d)\n", nbElementsLigne(grille, ligActu));
        }
        
        //affichage de la ligne de séparation, présente toutes les N lignes
        printf("  +");
        for (int i = 1; i <= TAILLE*3; i++)
        {
            printf("-");
            if (i%(3*N) == 0)
            {
                printf("+");
            }  
        }
        printf("\n"); 
    }


    //===================================================== modification exercice TP noté sudoku (affichage du nombre d'éléments par colonne) =====================================================
    printf("   ");
    for (int i = 0; i < TAILLE; i++)
    {
        printf("(%d)", nbElementsColonne(grille, i));
        if ((i+1) % N == 0)
        {
            printf(" ");
        }
    }
    printf("\n");
}


void saisir(int *nb){
    bool saisieOk;
    char ch[10];
    saisieOk = false;

    // on recommance la saisie tant qu'elle n'est pas valide
    while (saisieOk == false)
    {
        scanf("%s", ch);
        if (sscanf(ch, "%d", &(*nb)) !=0)
        {
            if (*nb <= TAILLE && *nb > 0)
            {
                saisieOk = true;
            }
            else
            {
                printf("ERREUR – saisissez un chiffre entre 1 et %d\n", TAILLE);
            }
        } 
        else 
        {
            printf("ERREUR – La conversion en entier a échoué\n");
        }
    }
}


bool possible(tGrille grille, int numLig, int numCol, int val)
{
    //variable ok qui reste a 1 tant que le procédure ne trouveaucune incompatibilité, sinon passe a 0
    bool ok;
    ok = true;

    for (int i = 0; i < TAILLE ; i++)
    {
        if (val == grille[i][numCol])
        {
            ok = false;
            printf("INCORRECT - %d est déjà présent dans la colonne %d !\n", val, numCol+1);
        }
        else if (val == grille[numLig][i])
        {
            ok = false;
            printf("INCORRECT - %d est déjà présent dans la ligne %d !\n", val, numLig+1);
        }
    }


    // Explication de la recherche de la valeur dans le bloc car les boucles sont complexes
    // On teste si une valeur est dans le bloc de taille N en le parcourant comme un sous-tableau, en partant de la case
    // supérieure gauche de ce bloc, qui a pour indice de ligne le multiple de N le plus proche (inférieur ou égal)
    // de la ligne actuelle (pareil pour la colonne)

    for (int ligBloc = (numLig - numLig % N); ligBloc < (numLig - (numLig % N) + N); ligBloc++)
    {
        for (int colBloc = (numCol - numCol % N); colBloc < (numCol - (numCol % N) + N); colBloc++)
        {
            if (val == grille[ligBloc][colBloc])
            {
                printf("%d est déjà présent dans le bloc !\n", val);
                ok = false;
            }
        }
    }
    return ok;
}


bool grillePleine(tGrille grille){
    bool pleine;
    pleine = true;

    int lig, col;
    lig = 0;


    //double boucle tant que pour vérifier toutes les cases de la grille et s'arrêter dès qu'une case vide est trouvée
    while (lig < TAILLE && pleine == true)
    {
        col = 0;
        while (col < TAILLE && pleine == true)
        {
            if (grille[lig][col] == 0)
            {
                pleine = false;
            }
            col++;
        }
        lig++;
    }
    return pleine;
}



//===================================================== modification exercice TP noté sudoku (les 2 fonctions de calcul du nombre d'éléments) =====================================================
int nbElementsLigne(tGrille g, int lig){
    int nbElt;
    nbElt = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        if (g[lig][i] > 0)
        {
            nbElt = nbElt + 1;
        }
    }
    return nbElt;
}

int nbElementsColonne(tGrille g, int col){
    int nbElt;
    nbElt = 0;
    for (int i = 0; i < TAILLE; i++)
    {
        if (g[i][col] > 0)
        {
            nbElt = nbElt + 1;
        }
    }
    return nbElt;
}