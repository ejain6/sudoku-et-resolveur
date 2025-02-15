#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//déclaration des tableaux / structures

#define N 3
#define TAILLE N*N
typedef int tCand[TAILLE];

typedef struct{
   int valeur;
   tCand candidats;
   int nbCand;
}tCase;

typedef tCase tGrille[TAILLE][TAILLE];


//déclaration des procédures / fonctions

void chargerGrille(tGrille grille, int *compteur);
void afficherGrille(tGrille grille);
bool possible(tGrille grille, int numLig, int numCol, int val);

void ajouterCandidat1(tCase *c, int val);
void retirerCandidat1(tCase *c, int val, int *compCandFin);
bool estCandidat1(tCase *c, int val);
void afficherCandidats(tGrille grille);
void afficherStats(int compCasesInit, int compCasesFin, int compCandInit, int compCandFin);

//déclaration des constantes


int main(){
   //déclaration des variables
   tGrille grille;
   int compCasesInit = 0;
   int compCasesFin = 0;
   int compCandInit = 0;
   int compCandFin = 0;

   int boucle1;
   int boucle2;
   int stop;
    
   chargerGrille(grille, &compCasesInit);
   afficherGrille(grille);
   

   // initialisation de tous les candidats
    for (int i = 0; i < TAILLE; i++)
    {
        for (int j = 0; j < TAILLE; j++)
        {
            if (grille[i][j].valeur == 0)
            {
                for (int k = 1; k < TAILLE; k++)
                {
                    if (possible(grille, i, j, k))
                    {
                        ajouterCandidat1(&grille[i][j], k);
                        compCandInit++;
                    }
                }
            }
        }
    }

    compCasesFin = compCasesInit;
    compCandFin = compCandInit;
   
    stop = -1;
    while(stop == -1)
    {
        stop = 1;
        boucle1 = 0;
        while (boucle1 < TAILLE && stop == 1)
        {
            boucle2 = 0;
            while(boucle2 < TAILLE && stop == 1)
            {
                if(grille[boucle1][boucle2].valeur == 0)
                {
                    printf("%d %d %d %d %d\n", grille[boucle1][boucle2].candidats[0], grille[boucle1][boucle2].candidats[1], grille[boucle1][boucle2].candidats[2], grille[boucle1][boucle2].candidats[3], grille[boucle1][boucle2].candidats[4]);
                    if(grille[boucle1][boucle2].nbCand == 1)
                    {
                        grille[boucle1][boucle2].valeur = grille[boucle1][boucle2].candidats[0];
                        stop = -1;
                        compCasesFin--;
                        printf("%d ajouté en case %d %d\n", grille[boucle1][boucle2].valeur, boucle1, boucle2);
                        afficherGrille(grille);
                    }
                }
                boucle2++;
            }
            boucle1++;
        }

        //On supprime les candidats impactés par l'ajout d'une valeur (quand ca a lieu)
        //D'abord dans la ligne et la colonne
        if(stop == -1)
        {
            boucle1--;
            boucle2--;
            for(int i = 0; i < TAILLE; i++)
            {
                retirerCandidat1(&grille[i][boucle1], grille[boucle1][boucle2].valeur, &compCandFin);
                retirerCandidat1(&grille[boucle2][i], grille[boucle1][boucle2].valeur, &compCandFin);
            }

            //Puis dans le bloc
            for (int ligBloc = (boucle1 - boucle1 % N); ligBloc < (boucle1 - (boucle1 % N) + N); ligBloc++)
            {
                for (int colBloc = (boucle2 - (boucle2 % N)); colBloc < (boucle2 - (boucle2 % N) + N); colBloc++)
                {
                    retirerCandidat1(&grille[ligBloc][colBloc], grille[boucle1][boucle2].valeur, &compCandFin);
                }
            }
        }
    }

    afficherGrille(grille);
    afficherCandidats(grille);
    afficherCandidats(grille);
    afficherCandidats(grille);
    afficherCandidats(grille);
    afficherCandidats(grille);
    afficherStats(compCasesInit, compCasesFin, compCandInit, compCandFin);


   return EXIT_SUCCESS;
}

void chargerGrille(tGrille grille, int *compteur){
    char nomFichier[30];
    bool fichierTrouve = false;
    FILE * f;
    while (fichierTrouve == false)
    {
        printf("Nom du fichier ?\n");
        scanf("%s", nomFichier);
        f = fopen(nomFichier, "rb");
        if (f==NULL){
            printf("\n ERREUR sur le fichier %s\n", nomFichier);
        } 
        else {
            int i, j, k;
            for (i = 0; i < TAILLE; i++) {
                for (j = 0; j < TAILLE; j++) {
                    fread(&grille[i][j].valeur, sizeof(int), 1, f);

                    if(grille[i][j].valeur == 0){
                        compteur++;
                        grille[i][j].nbCand = 0;
                        for (k = 0; k < TAILLE; k++) {
                            grille[i][j].candidats[k] = 0;
                        }
                    }
                }
            }
            fclose(f);
            fichierTrouve = true;
        }
    }
}


void afficherGrille(tGrille grille){

    int ligActu;
    tCase caseActu;

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
                caseActu = grille[ligActu][col];

                //test si une valeur est présente, si oui elle est affichée (avec seulement un espace si elle prend 2 espaces), sionon un . a la place
                if (caseActu.valeur == 0)
                {
                    printf(" . ");
                }
                else if(caseActu.valeur < 10)
                {
                    printf(" %d ", caseActu.valeur);
                }
                else
                {
                    printf(" %d", caseActu.valeur);
                }
                
                
                //toute les N cases, on affiche un | pour la délimitation en N*N de la grille
                if ((col+1) % N == 0)
                {
                    printf("|");
                }
            }
            printf("\n");
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
    printf("\n");
}

bool possible(tGrille grille, int numLig, int numCol, int val)
{
    //variable ok qui reste a 1 tant que le procédure ne trouveaucune incompatibilité, sinon passe a 0
    bool ok;
    ok = true;

    for (int i = 0; i < TAILLE ; i++)
    {
        if (val == grille[i][numCol].valeur)
        {
            ok = false;
        }
        else if (val == grille[numLig][i].valeur)
        {
            ok = false;
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
            if (val == grille[ligBloc][colBloc].valeur)
            {
                ok = false;
            }
        }
    }
    return ok;
}



void ajouterCandidat1(tCase *c, int val){
   if(val > TAILLE)
   {
      printf("ERREUR de valeur");
   }
   else
   {
      c->candidats[c->nbCand] = val;
      c->nbCand ++;
   }
}

void retirerCandidat1(tCase *c, int val, int *compCandFin){
   int i;
   i = 0;
   int trouve = 0;

   if(c->valeur == 0)
   {
        while (trouve == 0 && i < c->nbCand)
        {
            if(c->candidats[i] == val)
            {
                trouve = 1;
                (*compCandFin)--;
            }

            if(trouve == 1)
            {
                while(i+1 < c->nbCand)
                {
                    c->candidats[i] = c->candidats[i+1];
                    i++;
                }
                c->nbCand--;
                c->candidats[i+1] = 0;
            }
            i++;
        }
   }
}

bool estCandidat1(tCase *c, int val){
   int i;
   i = 0;
   bool trouve;
   trouve = false;

   while(trouve == false && i <= c->nbCand)
   {
      if(c->candidats[i] == val)
      {
         trouve = true;
      }
      i++;
   }
   return trouve;
}

void afficherCandidats(tGrille grille){
    int lig, col;
    printf("Coordonnées de la case a afficher??");
    scanf("%d %d", &lig, &col);
    lig --;
    col --;
    printf("valeur de la case: %d\n", grille[lig][col].valeur);
    for (int i = 0; i < grille[lig][col].nbCand; i++)
    {
        printf(" %d ", grille[lig][col].candidats[i]);
    }
    printf("\n");
}


void afficherStats(int compCasesInit, int compCasesFin, int compCandInit, int compCandFin){
    printf("a\n");
}