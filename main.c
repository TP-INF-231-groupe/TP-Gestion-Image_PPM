#include "manip.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char commande[256];
    printf("=================================================================\n");
    printf(" Application de traitement d'images PPM realisee par ProgMax.\n");
    printf("=================================================================\n");

    while (1) {
        printf("ppmviewer > ");
        if (!fgets(commande, sizeof(commande), stdin)) 
            break;
            
        // Enlever le saut de ligne
        commande[strcspn(commande, "\n")] = '\0';
        
        // Ignorer les lignes vides
        if (strlen(commande) == 0)
            continue;
        
        // Commande pour quitter
        if (strncmp(commande, "quit", 4) == 0) {
            printf("Fermeture du programme...\n");
            break;
        }  
        
        // ---- Commandes de modification de l'image ---- 
        else if (strncmp(commande, "size", 4) == 0) {
            char nameImage[256];
            if (sscanf(commande, "size %s", nameImage) == 1) {
                ImagePPM *img = load_image(nameImage);
                if (img != NULL) {
                    size(*img);
                    free_image(img);
                    printf("Operation effectuee avec succes.\n");
                } else {
                    printf("Erreur de chargement de l'image %s\n", nameImage);
                }
            } else {
                printf("Erreur de syntaxe. Utilisation : size <nom_image.ppm>\n");
            }
        }
        else if (strncmp(commande, "dom", 3) == 0) {
            char nameImage[256], col;
            int val;
            if (sscanf(commande, "dom %c %i %s", &col, &val, nameImage) == 3) {
                ImagePPM *img = load_image(nameImage);
                if (img != NULL) {
                    char name_file[256];
                    new_name(nameImage, "dom", name_file);
                    ImagePPM *newImg = dom(*img, col, val, name_file);
                    free_image(img);
                    free_image(newImg);
                    printf("Operation effectuee : image sauvegardee sous %s\n", name_file);
                } else {
                    printf("Erreur de chargement de l'image %s\n", nameImage);
                }
            } else {
                printf("Erreur de syntaxe. Utilisation : dom <r|g|b> <valeur> <nom_image.ppm>\n");
            }
        }
        else if (strncmp(commande, "gris", 4) == 0) {
            char nameImage[256];
            if (sscanf(commande, "gris %s", nameImage) == 1) {
                ImagePPM *img = load_image(nameImage);
                if (img != NULL) {
                    char name_file[256];
                    new_name(nameImage, "gris", name_file);
                    ImagePPM *newImg = gris(*img, name_file);
                    free_image(img);
                    free_image(newImg);
                    printf("Operation effectuee : image sauvegardee sous %s\n", name_file);
                } else {
                    printf("Erreur de chargement de l'image %s\n", nameImage);
                }
            } else {
                printf("Erreur de syntaxe. Utilisation : gris <nom_image.ppm>\n");
            }
        }
        else if (strncmp(commande, "neg", 3) == 0) {
            char nameImage[256];
            if (sscanf(commande, "neg %s", nameImage) == 1) {
                ImagePPM *img = load_image(nameImage);
                if (img != NULL) {
                    char name_file[256];
                    new_name(nameImage, "neg", name_file);
                    ImagePPM *newImg = neg(*img, name_file);
                    free_image(img);
                    free_image(newImg);
                    printf("Operation effectuee : image sauvegardee sous %s\n", name_file);
                } else {
                    printf("Erreur de chargement de l'image %s\n", nameImage);
                }
            } else {
                printf("Erreur de syntaxe. Utilisation : neg <nom_image.ppm>\n");
            }
        }
        else if (strncmp(commande, "cut", 3) == 0) {
            char nameImage[256], name_file[256];
            int l1, l2, c1, c2;
            if (sscanf(commande, "cut %s %d %d %d %d %s", nameImage, &l1, &l2, &c1, &c2, name_file) == 6) {
                ImagePPM *img = load_image(nameImage);
                if (img != NULL) {
                    ImagePPM *newImg = cut(*img, l1, l2, c1, c2, name_file);
                    if (newImg != NULL) {
                        printf("Operation effectuee : image recadree sauvegardee sous %s\n", name_file);
                        free_image(newImg);
                    }
                    free_image(img);
                } else {
                    printf("Erreur de chargement de l'image %s\n", nameImage);
                }
            } else {
                printf("Erreur de syntaxe. Utilisation : cut <nom_image.ppm> <l1> <l2> <c1> <c2> <nom_image_cut.ppm>\n");
            }
        }
        else if (strncmp(commande, "fil", 3) == 0) {
            char nameImage[256];
            if (sscanf(commande, "fil %s", nameImage) == 1) {
                ImagePPM *img = load_image(nameImage);
                if (img != NULL) {
                    char name_file[256];
                    new_name(nameImage, "fil", name_file);
                    ImagePPM *newImg = fil(img, name_file);
                    free_image(img);
                    free_image(newImg);
                    printf("Operation effectuee : filtre median applique, %s cree\n", name_file);
                } else {
                    printf("Erreur de chargement de l'image %s\n", nameImage);
                }
            } else {
                printf("Erreur de syntaxe. Utilisation : fil <nom_image.ppm>\n");
            }
        }
        else if (strncmp(commande, "gen", 3) == 0) {
            char nameImage[256];
            int larg, haut;
            if (sscanf(commande, "gen %i %i %s", &larg, &haut, nameImage) == 3) {
                ImagePPM *img = create_image(larg, haut, 255);
                if (img != NULL) {
                    save_image(img, nameImage);
                    free_image(img);
                    printf("Operation effectuee : image aleatoire %s creee\n", nameImage);
                } else {
                    printf("Erreur de generation de l'image.\n");
                }
            } else {
                printf("Erreur de syntaxe. Utilisation : gen <largeur> <hauteur> <nom_image.ppm>\n");
            }
        }
        else {
            printf("Commande non reconnue. Tapez 'quit' pour sortir.\n");
        }
    }    
    return 0;
}
