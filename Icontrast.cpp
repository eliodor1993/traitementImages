/*  Cours de Traitement d'images
      TP1  - 1ere Partie
      Programme de Calcul du Profil d'Intensité d'une Image  	         	
      Etudiant: ELIODOR Ednalson Guy Mirlin 

    Purpose of this program ?
     
     This program makes it possible to obtain the
     Intensity of an image along a line or a selected column.  																                                     
     How to execute ? 																						////
      In the command line, while in the main directory, type:
       1- cmake
     then       												 		
      2- make
      3- ./Icontrast
      4-  Then you must provide a number for r1, s1, r2 and s2	
																							////
			 
*/


#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
 
using namespace cv;
using namespace std;
 
int computeOutput(int, int, int, int, int);
 
int main()
{
    Mat image = imread("image.tif");// Fournir l'image ici
    Mat new_image = image.clone();
 
    int r1, s1, r2, s2;
    cout<<"Bienvenue sur cet editeur de contraste\nIci, vous devez d'abord specifier une image dans le main du code,\nvous devez donner 4 parametres pour\nobtenir le contraste de l'image donnee\n *******\nVeuillez enter une valeur pour r1: "<<endl; 
    cin>>r1;
    cout<<"Valeur pour s1: "<<endl; 
    cin>>s1;
    cout<<"Valeur pour r2: "<<endl; 
    cin>>r2;
    cout<<"Valeur pour s2: "<<endl; 
    cin>>s2;
 
    for(int y = 0; y < image.rows; y++){
        for(int x = 0; x < image.cols; x++){
            for(int c = 0; c < 3; c++){
                int output = computeOutput(image.at<Vec3b>(y,x)[c], r1, s1, r2, s2);
                new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(output);
            }
        }
    }
 
    namedWindow("Original Image", 1);
    imshow("Original Image", image);
 
    namedWindow("New Image", 1);
    imshow("New Image", new_image);
    imwrite("Contrast.png", new_image);
    waitKey();
 
    return 0;
}
 
int computeOutput(int x, int r1, int s1, int r2, int s2)
{
    float result;
    if(0 <= x && x <= r1){
        result = s1/r1 * x;
    }else if(r1 < x && x <= r2){
        result = ((s2 - s1)/(r2 - r1)) * (x - r1) + s1;
    }else if(r2 < x && x <= 255){
        result = ((255 - s2)/(255 - r2)) * (x - r2) + s2;
    }
    return (int)result;
}
