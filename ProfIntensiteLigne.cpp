#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // Pour la fonction strcat
 
#define DEBUG        // Pour eviter les Bugs
 
typedef unsigned char uc;   //pour faciliter la saisie
typedef unsigned int ui; 
typedef unsigned short us;
// Declaration de la structure de l'en tête du fichier
typedef struct {
        char sign[2];  // Signature BMP
        int offset; 
        ui taille_fichier; // taille totale du fichier (sur 4 Octets)
        ui reserve;  // codée sur 4 octets
 
// Declaration de la structure de l'en tête de l'image
 
      ui  taille_entete; //taille en tête image
      ui  largeur; //largeur (width)
      ui  hauteur; //hauteur (height)
      ui  compression;  // = O si pas de compression , et 1 2 ou 3 selon types
      ui  resolution_h;  //la resolution horizontale (nbre pixels)
      ui  resolution_v;  // la resolution verticale
      ui  nbre_coul;  // nombre de couleurs 
      us  nbr_plans; // toujours egal à 1
      us codage;  // sur 1,4,8,16,24, ou 32 bits
      uc * pixel; 
      int  taille_image; //taille totale de l'image en octet 
 
     }bitmap;
////////////////////////////////////////////////////////
        //prototypes des fcts
bitmap* lecture(char*) ;
int conv (uc hex[4],ui) ;
int main(int argc, char *argv[])
{   
    //declaration des variables utiles dans notre programme
    int h,l,i=0,j=0,k,nbl,nbc; 
    char nom[100];  
    unsigned char A[320],R[240][320],x,V[240][320],B[240][320],x1,x2; 
    bitmap *b;   // recupération des infos de l'image
 
 
    // Recup du nom de l'image
    printf("Entrez le nom de l'image \n"); 
    gets(nom);
 
    strcat(nom,".bpm");  
      b=lecture(nom); 
    printf("Nom de l'image avec l'extension : %s\n",nom); 
    // ouverture du fichier
 
 
    //sinon on récupèr les infos        
     printf("La signature BMP de l'image :%.2s\n",b->sign);
     printf("Taille du Fichier : %d\n ",b->taille_fichier);
     printf("Taille de la zone reservee :%d\n",b->reserve);
     printf("Offset de l'image : %d \n",b->offset);
     printf("Taille en tete de l'image : %d\n",b->taille_entete);
     printf("Largeur (nombre de colonnes) : %d \n",b->largeur);
     printf("Hauteur (nombre de lignes) :%d \n",b->hauteur);
     printf("Resolution Horizontale :%d\n",b->resolution_h);
     printf("Resolution Verticale : %d\n",b->resolution_v);
     printf("Nombre de couleurs :%d\n",b->nbre_coul);
     printf("Compression : %d\n",b->compression);
     printf("Nombre de plans : %d\n",b->nbr_plans);
     printf("Codage : %d\n",b->codage);
     printf("Taille totale de l'image : %d\n",b->taille_image);
 
#ifdef DEBUG 
printf("fin du programme"); 
scanf("%*c"); 
#endif    
 
system("PAUSE");
}
 
 
////////////////////////////////////////////// FONCTIONS //////////////////////////////////////////////////////////////
  bitmap* lecture(char* nom)
{      
    bitmap *bmp;        // La variable que renvoie bitmap()
    FILE *fichier;        
    bmp=(bitmap*)malloc(sizeof(bitmap)); // on alloue dynamiquement le pointeur sur bitmap
    fichier=fopen(nom,"r"); 
     //si l'image n'existe pas on affiche un message d'erreur
      if (fichier==NULL){ 
       printf("Ouverture impossible de %s\n  ( - Erreur : Image introuvable )  \n",nom); 
       scanf("%*c"); 
       exit(0);
      }
    // on ouvre le fichier en lecture seule
    fread (&(bmp->sign),2,1,fichier);    // on récupère la signature
    fread (&(bmp->taille_image),4,1,fichier);    //  la taille de l'image 
    fread (&(bmp->reserve),4,1,fichier);   	
    fread (&(bmp->offset),4,1,fichier);    // l'offset    	
    fread (&(bmp->taille_entete),4,1,fichier);    // taille de la zone info    	
    fread (&(bmp->largeur),4,1,fichier);    // la largeur de l'image    
    fread (&(bmp->hauteur),4,1,fichier);    // la hauteur de l'image    	
    fread (&(bmp->nbr_plans),2,1,fichier);    // le nombre de plans (=1)   
    fread (&(bmp->codage),2,1,fichier);    // le mode d'affichage    	
    fread (&(bmp->compression),4,1,fichier);    // le format de compression    
    fread (&(bmp->taille_fichier),4,1,fichier);    // la taille de l'image    
    fread (&(bmp->resolution_h),4,1,fichier);    // les résolutions horizontale
    fread (&(bmp->resolution_v),4,1,fichier);        
    fread (&(bmp->nbre_coul),4,1,fichier);        
    bmp->pixel=(unsigned char*)malloc(sizeof(bmp->taille_image));
    fread(bmp->pixel,1,bmp->taille_image,fichier);        
    fclose (fichier);        // on ferme le fichier
 
    return bmp;        // on retourne le pointeur sur la structure bitmap
}
