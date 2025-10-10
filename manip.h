#include <stdio.h>
#ifndef MANIP_H
#define MANIP_H

// -----------Definition des differentes structures------------ 
typedef struct {
    int r, g, b; 
} Pixel ;

typedef struct {
    char version [3];
    int largeur, hauteur, max;
    Pixel ** pixels ;   
} ImagePPM ;

// -------------Declaration des prototypes -----------

// Fonction de Creation et sauvegarde d'une image ppm version P3 avec valeurs aleatoires 
ImagePPM* create_image (int larg, int hauteur, int max);
ImagePPM* load_image(char *nom_fichier);
void save_image (ImagePPM* image, char nom_image[]);
void new_name(const char *, const char *, char *); 

// Fonctions de modification des images 
void size (ImagePPM);
ImagePPM* dom (ImagePPM , char , float , char *);
ImagePPM* gris (ImagePPM, char *);
ImagePPM* neg (ImagePPM, char *);
ImagePPM* cut (ImagePPM, int, int, int, int, char *);
ImagePPM* fil (ImagePPM *, char *);

// Autres fonctions utilitaires
void no_commenent(FILE *);
int calculer_mediane(int *, int );
void free_image(ImagePPM *);

#endif