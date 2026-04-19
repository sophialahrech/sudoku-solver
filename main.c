//


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define DigitBitmapHeight 24
#define DigitBitmapWidth 16



int16_t DigitBitmap[9][DigitBitmapHeight] =
{
    {0, 0, 96, 224, 224, 992, 4064, 4064, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 0, 0},
    {0, 0, 496, 2044, 4092, 7710, 7182, 14350, 14351, 14, 30, 60, 124, 496, 2016, 3968, 7680, 7168, 14336, 16382, 16382, 16382, 0, 0},
    {0, 0, 992, 4088, 8188, 7196, 14350, 14350, 14350, 28, 252, 504, 508, 30, 14, 14, 14350, 14350, 14350, 15422, 8188, 4080, 0, 0},
    {0, 0, 56, 120, 120, 248, 504, 440, 952, 1848, 3640, 3640, 7224, 14392, 14392, 16383, 16383, 56, 56, 56, 56, 56, 0, 0 },
    {0, 0, 4092, 4092, 4092, 3072, 7168, 7168, 7168, 8176, 8184, 8188, 7198, 14, 14, 15, 14, 14350, 14350, 15932, 8188, 4080, 0, 0},
    {0, 0, 496, 2044, 4092, 7710, 7182, 7174, 14336, 14832, 15352, 16380, 15902, 15374, 14350, 14343, 14343, 14350, 7182, 7708, 4092, 2032, 0, 0},
    {0, 0, 16382, 16383, 16383, 14, 28, 28, 56, 112, 112, 224, 224, 448, 448, 960, 896, 896, 1920, 1792, 1792, 1792, 0, 0, },
    {0, 0, 992, 4088, 8188, 7198, 7182, 14350, 7182, 7196, 4092, 4088, 8188, 15390, 14350, 14350, 14350, 14350, 15374, 7710, 8188, 2040, 128, 0, },
    {0, 0, 992, 4088, 8188, 7196, 14350, 14350, 14350, 14350, 14350, 15390, 7742, 8190, 2030, 14, 14, 14364, 14364, 7224, 8184, 4080, 0, 0, }
};

//déclaration des fonctions externes à main
double ratio_pixels_blanc (unsigned char Cell[], int largeur, int hauteur); //déclaration de la fonction qui retourne le ratio de pixels blancs dans chaque cellule en pourcentage.

unsigned char GetCellBit(unsigned char* Cell, int line, int col, int lenght_cellule);//déclaration de la fonction qui retourne le bit selectionné de la cellule du sudoku en ayant sa position col(0_43) et line(0-43)

unsigned char GetDigitBitmapBit (int Digit, int line, int col); //déclaration de la fonction qui retourne le bit selectionné du bitmap du chiffre (0-9) (digit) en ayant sa position col(0-15) et line(0-23)


//fonction main
int main(int argc, const char * argv[]) {
   if (argc != 12) {  //vérification du nombre de paramètres, il doit être de 10 plus le nom du fichier plus le chemin du fichier donc 12
        printf( "Le nombre d'arguments doit être de 12.\n");
        exit(EXIT_FAILURE); //arrêt du programme
    }
    int seuil[10];
    for (int i = 2; i < argc; i++) { //conversion des chaînes de caractères passés en ligne de commande dans les arguments de main en entiers
        seuil[i-2] = atoi(argv[i]);
        if ((atoi(argv[i]) < 5)||(atoi(argv[i]) > 100)){
            fprintf(stderr, "Le seuil d'un des paramètres n'est pas dans les bornes prédéfinies\n");
            exit(EXIT_FAILURE); // Arrêt de l'éxécutable.
        }
    }
    
    FILE *file_b; //création du pointeur du fichier cell.bin
    file_b = fopen(argv[1], "rb"); //Ouvrir le fichier en mode binaire pour le lire
    // au lieu de argv 1
    if (file_b == NULL) { //erreur lors de l'ouverture du fichier
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE); //arrêt de l'éxecution du programme
    }
    int lenght_cellule; // on assigne la valeur lue à une variable représentant la longeur de l'image
    int height_cellule;// même chose mais en assignant une variable à la hauteur de l'image
    
    size_t element_lus = fread(&lenght_cellule, sizeof(int), 1, file_b); //lecture des 32 premiers bits du fichier cell.bin car ils contiennent les informations de la longueur de l'image
    if (element_lus !=1 ) {
        fprintf(stderr, "Erreur lors de la lecture des 32 premiers bits.\n");
        fclose(file_b); //erreur de la lecture du fichier, fermeture du fichier et arrêt de l'executable.
        return EXIT_FAILURE;
    }
    
    size_t elements_lus = fread(&height_cellule, sizeof(int), 1, file_b);
    //lecture des 32 premiers bits du fichier cell.bin car ils contiennent les informations de la hauteur de l'image
    
    if (elements_lus !=1 ) {
        fprintf(stderr, "Erreur lors de la lecture des 32 premiers bits.\n");
        fclose(file_b); //traitement de l'erreur de la lecture du fichier, fermeture du fichier et arrêt de l'executable.
        return EXIT_FAILURE;
    }
  
    if (lenght_cellule < 16 || lenght_cellule > 100) {
        fprintf(stderr, "Erreur : La longueur de l'image n'est pas comprise entre 16 et 100\n");
        fclose(file_b);
        return EXIT_FAILURE; //vérification des bornes de la longueur de l'image, fermeture du fichier et arrêt de l'éxécutable
    }
    if (height_cellule < 24 || height_cellule > 100) {
        fprintf(stderr, "Erreur : La largeur de l'image n'est pas comprise entre 24 et 100\n");
        fclose(file_b);
        return EXIT_FAILURE; //vérification des bornes de la largeur de l'image, fermeture du fichier et arrêt de l'éxécutable
    }
   

    //FIN DE PARTIE 1, nous avons vérifié les bornes et la bonne taille de l'image du fichier cell.bin
    
    unsigned char *Cell = (unsigned char*)malloc((height_cellule* lenght_cellule +1) * sizeof(unsigned char));
    // allocation de la mémoire pour un tableau dynamique Cell de taille, la variable globale taille_case
    
    if (Cell == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour le tableau Cell" );
        fclose(file_b);
        return EXIT_FAILURE; //arrêt de l'executable, fermeture du fichier car erreur lors de la création de Cell
    }
    
    size_t bytes_read = fread(Cell, sizeof(unsigned char), height_cellule * lenght_cellule + 1, file_b); //lecture de tous les pixels contenus dans le tableau dynamique qui ont été copié depuis le fichier cell.bin, bytes_read retourne le nombre de pixels lus
    
    size_t t_image = height_cellule * lenght_cellule;
    size_t pixels_missing = t_image - bytes_read;
    
    if (bytes_read < t_image) {
        fprintf(stderr, "Erreur, il manque %zu pixels.\n", pixels_missing);
        free(Cell);
        fclose(file_b);
        exit(EXIT_FAILURE); //le fichier ne contenait pas le bon nombre de pixels, le programme s'arrête, il manque des pixels
    }
    if (bytes_read > t_image) {
        fprintf(stderr,"Avertissement:la taille du fichier est plus grande que la taille attendue");
        free(Cell);
        fclose(file_b);
        exit(EXIT_FAILURE);//lorsqu'on a plus de pixels que demandé le programme s'arrête.
    }
    
    if(fclose(file_b)) {
        fprintf(stderr, "Erreur de fermeture du fichier binaire.\n");
        exit(EXIT_FAILURE); //fermeture du fichier cell.bin
    }
    //FIN PARTIE 2
    
    //DEBUT PARTIE 3
    FILE *f_digit;//création d'un pointeur pour le fichier texte
    f_digit= fopen("CellValue.txt", "w+");//Création et ouverture du fichier texte "CellValue.txt"
    
    if (f_digit == NULL){ // Erreur lors de la création/ouverture du fichier "CellValue.txt".
        fprintf(stderr, "Erreur lors de la création/ouverture du fichier texte.\n");
        exit(EXIT_FAILURE); // Arrêt de l'éxécutable.
    }

    if (ratio_pixels_blanc(Cell, lenght_cellule, height_cellule)>=seuil[0]){//boucle vérifiant si la case est vide en comparant le ratio de pixels blanc à celui de la case vide donné par seuil[0]
        fprintf(f_digit, "d: '%d', %.0f%%\n",0, ratio_pixels_blanc(Cell, lenght_cellule, height_cellule));//la fonction fprintf écrit dans le fichier CellValue.txt grâce au pointeur f_digit la valeur 0 pour la case blanche et le pourcentage de pixels blancs de la case.
        printf("BLANC %f", ratio_pixels_blanc(Cell, lenght_cellule, height_cellule));

        if(fclose(f_digit)!=0){ // Fermeture du fichier texte CellValue.txt
            fprintf(stderr, "Erreur lors de la fermeture du fichier texte.\n");
            exit(EXIT_FAILURE);
        }
        return 0;
    }
    else{//La case n'est pas vide il faut donc la tester avec tous les bitmaps pour savoir quel chiffre est à l'intérieur de celle-ci
        int digit_reconnu= 0; //Variable stockant le chiffre(digit) ayant le plus grand ratio de simillitude entre la cellule et le bitmap
        int MAX=0; //Variable stockant le nombre maximal de bit identiques entre la cellule et le bitmap
        double ratio_max=0; //Variable stockant le ratio maximale de bit identiques entre la cellule et le bitmap
        for (int digit=1;digit<=9;digit++){//on fait varier le bitmap de 1 à 9 (digit 1-9) pour tester tous les chiffres
            MAX=0;
            for (int offset_col=0; offset_col<(lenght_cellule-DigitBitmapWidth);offset_col++){//Boucle 'for' qui ballaie les colonne de la cellule de 0 à 27 puisque le bitmap ne peut pas aller au delà sinon il sortirait de la cellule
                //remplacer (lenght_cellule-DigitBitmapWidth) par 28 pour les tests
                for (int offset_line=0; offset_line<(height_cellule-DigitBitmapHeight);offset_line++){//Boucle 'for' qui ballaie les lignes de la cellule de 0 à 19 puisque le bitmap ne peut pas aller au delà sinon il sortirait de la cellule
                    // remplacer (height_cellule-DigitBitmapHeight) par 20 pour les tests
                    int n_pixels_identiques=0;//variable qui stocke le nombre de pixels identiques entre le bitmap et la cellule
                    for(int line=0; line<DigitBitmapHeight; line++){//Boucle for qui ballaie les lignes du bitmap de 0 à 23
                        for(int col=0; col<DigitBitmapWidth;col++){//Boucle qui ballaie les collonnes du bitmap de 0 à 15
                            if (GetDigitBitmapBit (digit, line, col)==GetCellBit(Cell, line+offset_line, col+offset_col, lenght_cellule)){
                                n_pixels_identiques+=1;//condition'if' qui ajoute 1 au nombre de pixels identiques si la valeur du bit de la cellule est égale à la valeur du bit du bitmap
                            }
                            if (n_pixels_identiques > MAX){
                                MAX=n_pixels_identiques;
                            }
                        }
                    }
                    
                }
            }
            float ratio_pixels_identiques= ((double)MAX*100)/((double)DigitBitmapHeight*(double)DigitBitmapWidth);
            printf("%d :, ratio %f\n, seuil = %d\n",digit, ratio_pixels_identiques, seuil[digit]);
            if (ratio_pixels_identiques>=seuil[digit]){
                if (ratio_pixels_identiques>ratio_max){
                    ratio_max= ratio_pixels_identiques;
                    digit_reconnu= digit; //variable qui prend comme valeur le digit (1-9) dont le ratio de pixels identiques est le plus grand des ) testés
                    printf("rtio %f, digit %d\n", ratio_max, digit_reconnu);
                    
                }
            }
            
        }
        fprintf(f_digit, "d: '%d', %.0f%%\r", digit_reconnu,ratio_max); // Ecrit dans le fichier "CellValue.txt" grâce au pointeur f_digit le chiffre retenu (digit_reconnu) ainsi que son pourcentage (ratio_max)
        if(fclose(f_digit)!=0){ // Fermeture du fichier texte CellValue.txt
            fprintf(stderr, "Erreur lors de la fermeture du fichier texte.\n");
            exit(EXIT_FAILURE);
        }
    }
    
free (Cell);
return EXIT_SUCCESS;
}



//fonction qui retourne le ratio de pixels blancs dans chaque cellule en poucentage.
double ratio_pixels_blanc (unsigned char Cell[], int lenght_cellule, int height_cellule) {
    int noir= 0;
    for (int i=0; i<height_cellule*lenght_cellule; i++){
        noir+= Cell[i]; //chaque bit noir a une valeur 1 dans le tableau
}
    double blanc= (lenght_cellule*height_cellule) - noir;
    double output = blanc / (lenght_cellule*height_cellule);
    output *= 100;
    return output;
}


//Fonction qui retourne le bit selectionné de la cellule du sudoku en ayant sa position col(0_43) et line(0-43)
 unsigned char GetCellBit(unsigned char* Cell, int line, int col, int lenght_cellule){
   return Cell[line*lenght_cellule+col];
}

 

//Fonction qui retourne le bit selectionné du bitmap du chiffre (0-9) (digit) en ayant sa position col(0-15) et line(0-23)
unsigned char GetDigitBitmapBit (int Digit, int line, int col){
    return (DigitBitmap [Digit-1][line] >> (15-col)) & 1; // On sélectionne la ligne du digit qui est un chiffre codé sur 16 bits. Pour avoir la valeur du bit qu'on veut on shift vers la gauche de la valeur de col puis on le compare à 1 avec l'opérateur bit à bit '&'.
}
