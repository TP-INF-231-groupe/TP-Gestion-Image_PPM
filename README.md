# TP-Gestion-Image_PPM

## 📝 Description du Projet
Le but de ce projet est de réaliser une application de traitement d'images au format PPM (Portable PixMap). Ce format permet de stocker une image non compressée en écrivant la liste des pixels qu'elle contient ainsi que leurs couleurs associées.

## 🚀 Fonctionnalités et Réalisation
Pour mener à bien ce projet, nous avons créé une structure `ImagePPM` capable de contenir l'en-tête et la liste des pixels d'un fichier PPM. Voici les différentes fonctionnalités implémentées :

* **Création et Sauvegarde :**
  * `create_image()` : Crée une structure `ImagePPM` avec des valeurs de pixels aléatoires.
  * `save_image()` : Sauvegarde les données de la structure dans un fichier PPM.
  * `load_image()` : Fonction essentielle permettant de récupérer les informations d'un fichier PPM et de les stocker dans la structure.
  * `free_image()` : Libère la mémoire allouée pour l'image.

* **Manipulation et Traitement d'images :**
  * `dom()` : Permet de foncer ou d'éclaircir une image.
  * `gris()` : Convertit l'image en noir et blanc (nuances de gris).
  * `size()` : Calcule ou modifie la taille d'une image.
  * `cut()` : Découpe une partie de l'image.
  * `neg()` : Applique un filtre négatif à l'image.
  * `fil()` et `calcul_median()` : Appliquent un filtre médian sur l'image.

## 👥 Contributeurs et Répartition des Tâches

| Membre de l'équipe | Tâches effectuées |
| :--- | :--- |
| **MBEZOU DJAMEN JORDAN BENI** | `create_image()`, `save_image()`, `load_image()`, `cut()`, `free_image()`, fonction principale `main()`. |
| **NJITCHEU TEUMEBA CONSTY RAPHAEL** | `dom()`, contribution à `load_image()` et à la fonction principale `main()`. |
| **TCHOFFO ROUSSEL NATHAN** | `neg()`, contribution à `load_image()`. |
| **APOUFONG FOYIE DUVEL NIKE** | `calcul_median()`, `fil()`. |
| **KISSIME KISSIME PRINCE NATHAN**| `gris()`. |
| **NJANGA KOME JACQUES YOAN** | `size()`. |
