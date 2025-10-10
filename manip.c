#include "manip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// -------------Creation d'une image ppm version P3 avec des valeur aleatoire---------------------------- 

ImagePPM* create_image (int larg, int hauteur, int max)
{
    ImagePPM *image = malloc(sizeof(ImagePPM));
    image->largeur = larg;
    image->hauteur = hauteur;
    image->max = max;

    // Allocation dynamique du tableau de pixels
    image->pixels = malloc(hauteur * sizeof(Pixel*));
    for (int i = 0; i < hauteur; i++) {
        image->pixels[i] = malloc(larg * sizeof(Pixel));
        // insertion des valeurs aleatoires des pixels 
        for (int j = 0; j < larg; j++) {
            image->pixels[i][j].r = rand() % (max + 1);
            image->pixels[i][j].g = rand() % (max + 1);
            image->pixels[i][j].b = rand() % (max + 1);
        }
    }
    return image;
}

// Fonction pour ignorer les commentaires dans le fichier PPM
void no_commenent(FILE *f)
{
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        exit (EXIT_FAILURE);
    }
    char c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '#') {
            // Ignorer jusqu'a la fin de la ligne
            while ((c = fgetc(f)) != EOF && c != '\n');
        }else if (!isspace(c)) {
            ungetc(c, f);
            break;
        }
    }
}

ImagePPM* load_image(char *name_image)
{
    FILE *f = fopen(name_image, "r");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }
    // --- Lecture de l'en-tete PPM ---
    ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image) {
        fprintf(stderr, "Erreur d'allocation de memoire pour ImagePPM\n");
        fclose(f);
        return NULL;
    }

    no_commenent(f);
    fscanf(f, "%2s", image->version);
    if (strcmp(image->version, "P3") != 0) {
        fprintf(stderr, "Erreur : format non pris en charge (doit etre P3)\n");
        fclose(f);
        return NULL;;
    }

    no_commenent(f);
    fscanf(f, "%d %d", &image->largeur, &image->hauteur);

    no_commenent(f);
    fscanf(f, "%d", &image->max);

    // --- Allocation dynamique du tableau de pixels ---
    image->pixels = malloc(image->hauteur * sizeof(Pixel*));
    if (!image->pixels) {
        fprintf(stderr, "Erreur d'allocation memoire (lignes pixels)\n");
        free(image);
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < image->hauteur; i++) {
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        if (!image->pixels[i]) {
            fprintf(stderr, "Erreur d'allocation memoire (colonnes pixels)\n");
            // Libérer les lignes precedentes avant de quitter
            for (int k = 0; k < i; k++) free(image->pixels[k]);
            free(image->pixels);
            free(image);
            fclose(f);
            return NULL;
        }
        // --- Lecture des valeurs RGB ---
        for (int j = 0; j < image->largeur; j++) {
            fscanf(f, "%d %d %d", &image->pixels[i][j].r, &image->pixels[i][j].g, &image->pixels[i][j].b);
        }
    }
    fclose(f);
    return image;
}

// Sauvegarde des valeurs dans un fichier pour obtenir une image.ppm
void save_image (ImagePPM* image, char nom_image[])
{
    FILE * f = fopen (nom_image, "w");
    if (!f) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    fprintf (f, "P3 \n");
    fprintf (f, "#Image de %d sur %d pixels \n", image->largeur, image->hauteur);
    fprintf (f, "%d %d \n", image->largeur, image->hauteur);
    fprintf (f,"%d \n", image->max);
    for (int i=0; i<image->hauteur; i++ ) {
        for (int j=0; j<image->largeur; j++ ) {
            Pixel pix = image->pixels[i][j];
            fprintf (f,"%d %d %d ", pix.r, pix.g, pix.b);
        } 
        fprintf(f, "\n");   
    }
    fprintf(f, "\n");
    fclose (f);
}

void new_name(const char *input_name, const char *suffix, char *output_name) 
{
    strcpy(output_name, input_name);
    char *dot = strrchr(output_name, '.');
    if (dot != NULL)
        *dot = '\0';
    strcat(output_name, "_");
    strcat(output_name, suffix);
    strcat(output_name, ".ppm");
}

// ---------------Manipulation de l'image ---------------------

void size (ImagePPM image )
{
    printf (" %d x %d ", image.largeur, image.hauteur) ;
}

ImagePPM* dom (ImagePPM img, char couleur, float valeur, char nom_image[])
{
    ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image){
        fprintf (stderr, "Erreur d'allocation \n");
        free (image);
        exit (EXIT_FAILURE);
    }
    image->largeur = img.largeur;
    image->hauteur = img.hauteur;
    image->max = img.max;
    image->pixels = malloc(image->hauteur * sizeof(Pixel*));
    if (!image->pixels) {
        fprintf(stderr, "Erreur d'allocation memoire (lignes pixels)\n");
        free(image);
        return NULL;
    }   
    for (int i=0; i<image->hauteur; i++){
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        if (!image->pixels[i]) {
            fprintf(stderr, "Erreur d'allocation memoire \n");
            for (int k = 0; k < i; k++)
                free(image->pixels[k]);
            free(image->pixels);
            free(image);
            return NULL; 
        }
        // insertion des nouvelles valeurs des pixels 
        for (int j=0; j<image->largeur; j++){ 
            if ( couleur == 'R' || couleur == 'r'){
                image->pixels[i][j].r = img.pixels[i][j].r + valeur ;
                if ( image->pixels[i][j].r > image->max )
                    image->pixels[i][j].r = image->max ;
                if ( image->pixels[i][j].r < 0 )
                    image->pixels[i][j].r = 0 ;
            }
            else if ( couleur == 'G' || couleur == 'g') {
                image->pixels[i][j].g = img.pixels[i][j].g + valeur ;
                if ( image->pixels[i][j].g > image->max )
                    image->pixels[i][j].g = image->max ;
                if ( image->pixels[i][j].g < 0 )
                    image->pixels[i][j].g = 0 ;
            }
            else if ( couleur == 'B' || couleur == 'b') {
                image->pixels[i][j].b = img.pixels[i][j].b + valeur ;
                if ( image->pixels[i][j].b > image->max )
                    image->pixels[i][j].b = image->max ;
                if ( image->pixels[i][j].b < 0 )
                    image->pixels[i][j].b = 0 ;
            }
            else{
                printf (" Couleur incorrecte : %c \n", couleur);
            }
        }
    } 
    save_image (image, nom_image);
    return image ;    
}

ImagePPM* gris (ImagePPM img, char nom_image[])
{
    ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image) {
        fprintf(stderr, "Erreur d'allocation de memoire pour ImagePPM\n");
        return NULL;
    }
    image->largeur = img.largeur;
    image->hauteur = img.hauteur;
    image->max = img.max;
    image->pixels = malloc(image->hauteur * sizeof(Pixel*));
    if (!image->pixels) {
        fprintf(stderr, "Erreur d'allocation memoire (lignes pixels)\n");
        free(image);
        return NULL;
    }
    for (int i=0; i<image->hauteur; i++){
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        if (!image->pixels[i]){
            for (int k = 0; k < i; k ++)
                free (image->pixels[k]);
            free(image->pixels);
            free (image);
            exit (EXIT_FAILURE);
        }
        // insertion des nouvelles valeurs des pixels 
        for (int j=0; j<image->largeur; j++){ 
            int moyenne = (img.pixels[i][j].r + img.pixels[i][j].g + img.pixels[i][j].b)/3 ;
            image->pixels[i][j].r = moyenne ;
            image->pixels[i][j].g = moyenne ;
            image->pixels[i][j].b = moyenne ; 
        }
    } 
    save_image (image, nom_image);
    return image ;    
}

ImagePPM* neg (ImagePPM img, char nom_image[])
{
    ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image) {
        fprintf(stderr, "Erreur d'allocation de memoire pour ImagePPM\n");
        return NULL;
    }
    image->largeur = img.largeur;
    image->hauteur = img.hauteur;
    image->max = img.max;
    image->pixels = malloc(image->hauteur * sizeof(Pixel*));
    if (!image->pixels) {
        fprintf(stderr, "Erreur d'allocation memoire (lignes pixels)\n");
        free(image);
        return NULL;
    }
    for (int i=0; i<image->hauteur; i++){
        image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
        if (!image->pixels[i]) {
            fprintf(stderr, "Erreur d'allocation memoire (colonnes pixels)\n");
            for (int k = 0; k < i; k++)
                free(image->pixels[k]);
            free(image->pixels);
            free(image);
            return NULL; 
        }   
        // insertion des nouvelles valeurs des pixels et verification si chaque pixels est toujours un entiers
        for (int j=0; j<image->largeur; j++){ 
            Pixel pix = image->pixels[i][j] ;
            image->pixels[i][j].r = img.max - img.pixels[i][j].r ;
            image->pixels[i][j].g = img.max - img.pixels[i][j].g ;
            image->pixels[i][j].b = img.max - img.pixels[i][j].b ; 
        }
    } 
    save_image (image, nom_image);
    return image ;    
}

ImagePPM* cut (ImagePPM img, int l1, int l2, int c1, int c2, char nom_image[])
{
    if (l1 >= 0 && c1 >= 0 && l2 <= img.hauteur && c2 <= img.largeur && l1 < l2 && c1 < c2)
    {
        ImagePPM *image = malloc(sizeof(ImagePPM));
    if (!image) {
        fprintf(stderr, "Erreur d'allocation de memoire pour ImagePPM\n");
        return NULL;
    }
        image->pixels = malloc((l2-l1) * sizeof(Pixel*));
        image->largeur = (c2 - c1) + 1 ;
        image->hauteur = (l2 - l1) + 1;
        image->max = img.max;
        if (!image->pixels) {
            fprintf(stderr, "Erreur d'allocation memoire (lignes pixels)\n");
            free(image);
            return NULL;
        }

        for (int i=0; i<image->hauteur; i++){
            image->pixels[i] = malloc(image->largeur * sizeof(Pixel));
            if (!image->pixels[i]) {
                fprintf(stderr, "Erreur d'allocation memoire (colonnes pixels)\n");
                for (int k = 0; k < i; k++)
                    free(image->pixels[k]);
                free(image->pixels);
                free(image);
                return NULL; 
            }
            // insertion des valeurs coupees des pixels
            for (int j=0; j<image->largeur; j++){ 
                image->pixels[i][j] = img.pixels[l1 + i - 1][c1 + j - 1];
            }
        } 
        save_image (image, nom_image);
        return image ;
    }
    else {
        printf("Erreur : coordonnees invalides pour le recadrage.\n");
        return NULL ;
    }
}

int calculer_mediane(int vals[], int taille) {
    int min ;
    for (int i = 0 ; i < (taille-1) ; i ++ ) {
        min = i ;
        for ( int j = (i+1); j < taille; j ++ ) {
            if (vals[j] < vals[min])
                min = j ;
            int x = vals[min];
            vals[min] = vals[i];
            vals[i] = x;
        }
    }
    return vals[4];
}

// --- Operation 6: Filtre Median ---
ImagePPM* fil (ImagePPM *originale, char nom_image[]) 
{
    int i, j, k, l, index;
    ImagePPM *nouvelle_img = (ImagePPM*)malloc(sizeof(ImagePPM));
    if (!nouvelle_img) {
        perror("Erreur d'allocation memoire pour la nouvelle structure ImagePPM");
        return NULL;
    }
    *nouvelle_img = *originale;
    nouvelle_img->pixels = NULL; 
    nouvelle_img->pixels = (Pixel**)malloc(originale->hauteur * sizeof(Pixel*));
    if (!nouvelle_img->pixels) {
        perror("Erreur d'allocation memoire pour les lignes de la nouvelle image");
        free(nouvelle_img);
        return NULL;
    }
    for (i = 0; i < originale->hauteur; i++) {
        nouvelle_img->pixels[i] = (Pixel*)malloc(originale->largeur * sizeof(Pixel));
        if (!nouvelle_img->pixels[i]) {
            perror("Erreur d'allocation memoire pour les pixels d'une ligne de la nouvelle image");
            for (int m = 0; m < i; m++) free(nouvelle_img->pixels[m]);
            free(nouvelle_img->pixels);
            free(nouvelle_img);
            return NULL;
        }
        for (j = 0; j < originale->largeur; j++) {
            int r_voisins[9], g_voisins[9], b_voisins[9];
            index = 0;
            for (k = -1; k <= 1; k++) {
                for (l = -1; l <= 1; l++) {
                    int voisin_i = i + k;
                    int voisin_j = j + l;
                    if (voisin_i < 0) 
                        voisin_i = 0;
                    if (voisin_i >= originale->hauteur) 
                        voisin_i = originale->hauteur - 1;
                    if (voisin_j < 0) 
                        voisin_j = 0;
                    if (voisin_j >= originale->largeur) 
                        voisin_j = originale->largeur - 1;
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
    save_image (nouvelle_img, nom_image);
    return nouvelle_img;
}
// Liberation de la memoire allouee pour une image
void free_image(ImagePPM *image)
{
    for (int i = 0; i < image->largeur; i ++)
        free(image->pixels[i]);
    free(image->pixels);
    free(image);
}

