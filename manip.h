#ifndef MANIP_H
#define MANIP_H

#include <stdio.h>

// ----------- Definition des differentes structures ------------ 
typedef struct {
    int r, g, b; 
} Pixel;

typedef struct {
    char version[3];
    int largeur, hauteur, max;
    Pixel **pixels;   
} ImagePPM;

// ------------- Declaration des prototypes -----------

// Fonctions de création, chargement et sauvegarde
ImagePPM* create_image(int larg, int hauteur, int max);
ImagePPM* load_image(const char *nom_fichier);
void save_image(ImagePPM* image, const char *nom_image);
void new_name(const char *input_name, const char *suffix, char *output_name); 

// Fonctions de modification des images 
void size(ImagePPM image);
ImagePPM* dom(ImagePPM img, char couleur, float valeur, const char *nom_image);
ImagePPM* gris(ImagePPM img, const char *nom_image);
ImagePPM* neg(ImagePPM img, const char *nom_image);
ImagePPM* cut(ImagePPM img, int l1, int l2, int c1, int c2, const char *nom_image);
ImagePPM* fil(ImagePPM *img, const char *nom_image);

// Autres fonctions utilitaires
void no_comment(FILE *f);
int calculer_mediane(int *vals, int taille);
void free_image(ImagePPM *image);

#endif
