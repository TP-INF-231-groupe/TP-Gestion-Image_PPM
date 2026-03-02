#include "manip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ------------- Creation d'une image ppm avec des valeurs aleatoires ---------------------------- 
ImagePPM* create_image(int larg, int hauteur, int max) {
    ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image) return NULL;
    
    image->largeur = larg;
    image->hauteur = hauteur;
    image->max = max;

    image->pixels = malloc(hauteur * sizeof(Pixel*));
    for (int i = 0; i < hauteur; i++) {
        image->pixels[i] = malloc(larg * sizeof(Pixel));
        for (int j = 0; j < larg; j++) {
            image->pixels[i][j].r = rand() % (max + 1);
            image->pixels[i][j].g = rand() % (max + 1);
            image->pixels[i][j].b = rand() % (max + 1);
        }
    }
    return image;
}

// Fonction pour ignorer les commentaires
void no_comment(FILE *f) {
    if (!f) return;
    char c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '#') {
            while ((c = fgetc(f)) != EOF && c != '\n');
        } else if (!isspace(c)) {
            ungetc(c, f);
            break;
        }
    }
}

ImagePPM* load_image(const char *name_image) {
    FILE *f = fopen(name_image, "r");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }
    
    ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image) {
        fclose(f);
        return NULL;
    }

    no_comment(f);
    fscanf(f, "%2s", image->version);
    if (strcmp(image->version, "P3") != 0) {
        fprintf(stderr, "Erreur : format non pris en charge (doit etre P3)\n");
        free(image);
        fclose(f);
        return NULL;
    }

    no_comment(f);
    fscanf(f, "%d %d", &image->largeur, &image->hauteur);

    no_comment(f);
    fscanf(f, "%d", &image->max);

    image->pixels = malloc(image->hauteur * sizeof(Pixel*));
    for (int i = 0; i < image->hauteur; i++) {
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        for (int j = 0; j < image->largeur; j++) {
            fscanf(f, "%d %d %d", &image->pixels[i][j].r, &image->pixels[i][j].g, &image->pixels[i][j].b);
        }
    }
    fclose(f);
    return image;
}

void save_image(ImagePPM* image, const char *nom_image) {
    FILE *f = fopen(nom_image, "w");
    if (!f) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    fprintf(f, "P3\n");
    fprintf(f, "# Image generee par l'application ProgMax\n");
    fprintf(f, "%d %d\n", image->largeur, image->hauteur);
    fprintf(f, "%d\n", image->max);
    
    for (int i = 0; i < image->hauteur; i++) {
        for (int j = 0; j < image->largeur; j++) {
            Pixel pix = image->pixels[i][j];
            fprintf(f, "%d %d %d ", pix.r, pix.g, pix.b);
        } 
        fprintf(f, "\n");   
    }
    fclose(f);
}

void new_name(const char *input_name, const char *suffix, char *output_name) {
    strcpy(output_name, input_name);
    char *dot = strrchr(output_name, '.');
    if (dot != NULL) *dot = '\0';
    strcat(output_name, "_");
    strcat(output_name, suffix);
    strcat(output_name, ".ppm");
}

// --------------- Manipulation de l'image ---------------------

void size(ImagePPM image) {
    printf("Taille de l'image : %d x %d pixels\n", image.largeur, image.hauteur);
}

ImagePPM* dom(ImagePPM img, char couleur, float valeur, const char *nom_image) {
    ImagePPM *image = malloc(sizeof(ImagePPM));
    image->largeur = img.largeur;
    image->hauteur = img.hauteur;
    image->max = img.max;
    image->pixels = malloc(image->hauteur * sizeof(Pixel*));

    for (int i = 0; i < image->hauteur; i++) {
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        for (int j = 0; j < image->largeur; j++) { 
            image->pixels[i][j] = img.pixels[i][j]; // Copie de base
            
            if (couleur == 'R' || couleur == 'r') {
                int new_val = img.pixels[i][j].r + (int)valeur;
                image->pixels[i][j].r = (new_val > image->max) ? image->max : (new_val < 0 ? 0 : new_val);
            } else if (couleur == 'G' || couleur == 'g') {
                int new_val = img.pixels[i][j].g + (int)valeur;
                image->pixels[i][j].g = (new_val > image->max) ? image->max : (new_val < 0 ? 0 : new_val);
            } else if (couleur == 'B' || couleur == 'b') {
                int new_val = img.pixels[i][j].b + (int)valeur;
                image->pixels[i][j].b = (new_val > image->max) ? image->max : (new_val < 0 ? 0 : new_val);
            }
        }
    } 
    save_image(image, nom_image);
    return image;    
}

ImagePPM* gris(ImagePPM img, const char *nom_image) {
    ImagePPM *image = malloc(sizeof(ImagePPM));
    image->largeur = img.largeur;
    image->hauteur = img.hauteur;
    image->max = img.max;
    image->pixels = malloc(image->hauteur * sizeof(Pixel*));

    for (int i = 0; i < image->hauteur; i++) {
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        for (int j = 0; j < image->largeur; j++) { 
            int moyenne = (img.pixels[i][j].r + img.pixels[i][j].g + img.pixels[i][j].b) / 3;
            image->pixels[i][j].r = moyenne;
            image->pixels[i][j].g = moyenne;
            image->pixels[i][j].b = moyenne; 
        }
    } 
    save_image(image, nom_image);
    return image;    
}

ImagePPM* neg(ImagePPM img, const char *nom_image) {
    ImagePPM *image = malloc(sizeof(ImagePPM));
    image->largeur = img.largeur;
    image->hauteur = img.hauteur;
    image->max = img.max;
    image->pixels = malloc(image->hauteur * sizeof(Pixel*));

    for (int i = 0; i < image->hauteur; i++) {
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        for (int j = 0; j < image->largeur; j++) { 
            image->pixels[i][j].r = img.max - img.pixels[i][j].r;
            image->pixels[i][j].g = img.max - img.pixels[i][j].g;
            image->pixels[i][j].b = img.max - img.pixels[i][j].b; 
        }
    } 
    save_image(image, nom_image);
    return image;    
}

ImagePPM* cut(ImagePPM img, int l1, int l2, int c1, int c2, const char *nom_image) {
    if (l1 >= 0 && c1 >= 0 && l2 <= img.hauteur && c2 <= img.largeur && l1 < l2 && c1 < c2) {
        ImagePPM *image = malloc(sizeof(ImagePPM));
        image->largeur = (c2 - c1) + 1;
        image->hauteur = (l2 - l1) + 1;
        image->max = img.max;
        
        // CORRECTION : Allocation basée sur image->hauteur, pas (l2-l1)
        image->pixels = malloc(image->hauteur * sizeof(Pixel*));

        for (int i = 0; i < image->hauteur; i++) {
            image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
            for (int j = 0; j < image->largeur; j++) { 
                image->pixels[i][j] = img.pixels[l1 + i][c1 + j]; // Retrait des -1 pour eviter les debordements
            }
        } 
        save_image(image, nom_image);
        return image;
    } else {
        printf("Erreur : coordonnees invalides pour le recadrage.\n");
        return NULL;
    }
}

int calculer_mediane(int vals[], int taille) {
    // Tri par selection
    for (int i = 0; i < taille - 1; i++) {
        int min = i;
        for (int j = i + 1; j < taille; j++) {
            if (vals[j] < vals[min]) min = j;
        }
        int temp = vals[min];
        vals[min] = vals[i];
        vals[i] = temp;
    }
    return vals[taille / 2]; // Retourne l'element du milieu
}

ImagePPM* fil(ImagePPM *originale, const char *nom_image) {
    ImagePPM *nouvelle_img = malloc(sizeof(ImagePPM));
    *nouvelle_img = *originale;
    nouvelle_img->pixels = malloc(originale->hauteur * sizeof(Pixel*));

    for (int i = 0; i < originale->hauteur; i++) {
        nouvelle_img->pixels[i] = malloc(originale->largeur * sizeof(Pixel));
        for (int j = 0; j < originale->largeur; j++) {
            int r_voisins[9], g_voisins[9], b_voisins[9];
            int index = 0;
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    int voisin_i = i + k;
                    int voisin_j = j + l;
                    if (voisin_i < 0) voisin_i = 0;
                    if (voisin_i >= originale->hauteur) voisin_i = originale->hauteur - 1;
                    if (voisin_j < 0) voisin_j = 0;
                    if (voisin_j >= originale->largeur) voisin_j = originale->largeur - 1;
                    
                    r_voisins[index] = originale->pixels[voisin_i][voisin_j].r;
                    g_voisins[index] = originale->pixels[voisin_i][voisin_j].g;
                    b_voisins[index] = originale->pixels[voisin_i][voisin_j].b;
                    index++;
                }
            }
            nouvelle_img->pixels[i][j].r = calculer_mediane(r_voisins, 9);
            nouvelle_img->pixels[i][j].g = calculer_mediane(g_voisins, 9);
            nouvelle_img->pixels[i][j].b = calculer_mediane(b_voisins, 9);
        }
    }
    save_image(nouvelle_img, nom_image);
    return nouvelle_img;
}

// CORRECTION CRITIQUE : on libere sur la hauteur (lignes), pas sur la largeur !
void free_image(ImagePPM *image) {
    if (image) {
        if (image->pixels) {
            for (int i = 0; i < image->hauteur; i++) {
                free(image->pixels[i]);
            }
            free(image->pixels);
        }
        free(image);
    }
}
