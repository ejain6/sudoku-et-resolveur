#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


// déclaration de la grille

/** @def N
 * 
 * @brief indique au programme la taille de la grille
 * 
 * constante définie avant le lancement du programme en fonction de la taille de 
 * la grille allant être utilisée (ici N =  3, pour la dimension de grille standard). \n
 * Constante très importante, permettant d'adapter le programme a des grilles de 
 * n'importe quelle dimension
 */
#define N 3 

/** @def TAILLE
 * 
 * autre constante très utile, qui définit notamment la taille 
 * des lignes et colonnes, et qui est égale au carré de N
 */
#define TAILLE N*N 

#define NB_LIGNES TAILLE 
#define NB_COLONNES TAILLE

/**
 * @typedef tGrille
 * 
 * @brief tableau de TAILLE*TAILLE valeurs entières
 * 
 * Ce tableau va contienir toutes les valeurs présentes dans la grille de sudoku,
 * les valeurs sont des entiers compris entre 1 et TAILLE, et les cases 
 * "vides" sont représentées par des 0.
*/
typedef int tGrille[NB_LIGNES][NB_COLONNES];
 

//déclaration des procédures / fonctions

void chargerGrille(tGrille grille);

void afficherGrille(tGrille grille);

void saisir(int *nb);

bool possible(tGrille grille, int numLig, int numCol, int val);

bool grillePleine(tGrille grille);


/**
 * @fn int main()
 * 
 * @brief boucle principale du jeu de Sudoku
 *
 * Le programme commencer par charger la grille avec l'aide de la procédure chargerGrille.
 * Maintenant que la grille est chargée, la partie de Sudoku est lancée donc le programme
 * tourne en boucle jusqu'à ce que la grille soit remplie (test du remplissage par la 
 * fonction grillePleine). \n
 * A chaque tour, l'utilisateur va donc devoir saisir les coordonnées d'une case (numéro 
 * de ligne et de colonne), puis entrer le nombre a insérer dans la case. \n Si la case est
 * vide et que l'on peut insérer le nombre dedans (testé par la fonction possible), alors
 * le nombre est inséré dans la grille, puis on recommence tant que le jeu n'est pas fini. \n
 * Si a case choisie est déjà remplie ou que la valeur ne peut pas y être insérée, 
 * l'utilisateur en est informé et on recommence une nouvelle fois la boucle.
 * 
*/

int main(){

    //déclaration des variables

    tGrille grille1;
    int numLigne, numColonne; 
    int valeur; 
    int remplie; 

    remplie = false;

    chargerGrille(grille1);
 
    //on joue tant que la grille n'est pas pleine
    while (remplie == false)
    {
        afficherGrille(grille1);

        printf("entrez le numéro de ligne\n");
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

/**
 * @fn void chargerGrille(tGrille grille)
 * 
 * @brief charge une grille de sudoku provenant d'un fichier
 * 
 * @param grille : le tableau a remplir 
 * 
 * La procédure demande un nom de fichier .sud (redemande tant que 
 * ca ne marche pas) et charge son contenu dans le tableau grille
*/

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


/**
 * @fn afficherGrille(tGrille grille)
 * 
 * @brief affiche la grille de Sudoku
 * 
 * @param grille : tableau de valeurs a afficher 
 *
 * La procédure affiche le tableau de valeurs, avec un affichage des numéros 
 * de ligne/colonnes, des bordures internes (toutes les N lignes/colonnes) 
 * et des bordures externes. Les 0 sont remplacés par des points.
*/

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
}


/**
 * @fn saisir(int *nb)
 * 
 * @brief effectue une saisie au clavier, en vérifiant que la valeur entrée est un entier, compris entre 1 et N*N
 * 
 * @param nb : la variable a laquelle sera affectée le nombre
 *
 * La procédure damande une saisie tant que la valeur saisie n'est pas un entier ou qu'elle n'est 
 * pas comprise entre 1 et N*N.
*/

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


/**
 * @fn bool possible(tGrille grille, int numLig, int numCol, int val)
 * 
 * @brief vérifie si une valeur peut être insérée dans la grille 
 * 
 * @param grille : tableau contenant toutes les valeurs de la grille de sudoku
 * @param numLig : index de ligne choisie 
 * @param numCol : index de colonne choisie
 * @param val : valeur choisie, a insérer dans la case (lignes & colonnes) choisie
 * 
 * @return true si la valeur peut être insérée dans la case, false sinon
 * 
 * Cette fonction utilise 2 recherches séquencielles. \n
 * Elle vérifie d'abord dans toute la ligne si la valeur est déjà présente.
 * En même temps, elle vérifie aussi dans toute le colonne si la valeur y est. \n
 * Après ca, elle cherche dans le Bloc (de taille N*N) si la valeur est présente
 * en parcourant le bloc, en partant de la case qui a pour coordonnée les 1er 
 * multiples de N inférieurs ou égaux les plus proches du numéro de colonne 
 * et du numéro de ligne, et s'arrête en ayant atteint la somme de ce meme 
 * multiple additionné a N (pour le numéro de ligne et celui de la colonne).
*/

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


/**
 * @fn bool grillePleine(tGrille grille)
 * 
 * @brief Vérifie si la grille est pleine
 * 
 * @param grille : tableau contenant les valeurs
 * 
 * @return true si la grille est entièrement remplie, false sinon
 *
 * La fonction va effectuer une recherche séquencielle, en parcourant le tableau et 
 * en s'arrêtant dès qu'une case vide (symbolysée par la présence d'un 0) est trouvée.
*/

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