/*    Cours de Traitement d'images
      TP2 - 1e Partie
      Programme de La Transformee de Fourrier d'une Image 
      Filtre passe-bas, Filtre passe-haut - correction de bruit sinusoïdal	         	
      Etudiant: ELIODOR Ednalson Guy Mirlin  


*/

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;



Mat RedimensionnerImage (Mat InputImage) // On veut que l'image entree soit de forme carree
                                         //Cette fonction permet de redimensionner l image entree, c'est a di
               
           {

	    Mat padded;     //permet de renvoyer la nouvelle image redimensionnee  en sortie

	    // Ici on definit la taille optimale prevue pour l image redimensionnee
	    int m = getOptimalDFTSize( InputImage.rows );
	    int n = getOptimalDFTSize( InputImage.cols );

	    //Une fois l'image redimensionnee, on doit ajouter les valeurs de bourrage dans l'image 
	    copyMakeBorder(InputImage, padded, 0, m - InputImage.rows, 0, n - InputImage.cols, BORDER_CONSTANT, Scalar::all(0));
	    return padded; //renvoyer la nouvelle image redimensionnee  en sortie

}

            Mat TransformationFourier (Mat ImgResized) // Cette fonction peut ainsi prendre l'image redimensionnee en compte
               {


	    //  Les deux conteneurs ci-dessous, permettent de stocker les valeurs flottantes pour
            //  les parties reelles et imaginaires de la transformee de fourier.

	    Mat Conteneurs[] = {Mat_<float>(ImgResized), Mat::zeros(ImgResized.size(), CV_32F)};

	    //Ici, on fait la concatenation des deux conteneurs crees plus haut
	    Mat ImgDeFourrier;
	    merge(Conteneurs, 2, ImgDeFourrier);

	    //calcul de la Transformee de fourier avec dft
	    dft(ImgDeFourrier, ImgDeFourrier);

	    return ImgDeFourrier; //on retourne l'image resultante
     }

    Mat NormDeCalcul (Mat ImgDeFourrier)  // 

      {


	    vector<Mat> Conteneurs;
	    // Ici on s'assure de separer les parties reelles et imaginaires de l'image resultant de la Transformation de Fourrier
	    split(ImgDeFourrier, Conteneurs);

	    // Calcul de norme de la transformee de fourier
	    magnitude(Conteneurs[0], Conteneurs[1], Conteneurs[0]);
	    Mat ImageNormeFourier = Conteneurs[0];

	    // Ci dessous, on se refere a l echelle logarithmique
	    ImageNormeFourier += Scalar::all(1);
	    log(ImageNormeFourier, ImageNormeFourier);
            // Ici on calcul le nouvau cadre du spectre
	    ImageNormeFourier = ImageNormeFourier(Rect(0, 0, ImageNormeFourier.cols & -2, ImageNormeFourier.rows & -2));

	    return ImageNormeFourier;
         }

Mat NewCadran (Mat ImageNormeFourier) // La fonction ci permet de prendre le cadrage de l'image de la transformee
                                          // on en denote 4 cadrantnts
           {

	    int cx = ImageNormeFourier.cols/2;
	    int cy = ImageNormeFourier.rows/2;

	    Mat cadran0(ImageNormeFourier, Rect(0, 0, cx, cy));   // Cadrant 0,  en haut à gauche
	    Mat cadran1(ImageNormeFourier, Rect(cx, 0, cx, cy));  // Cadrant 1,  en haut à droite
	    Mat cadran2(ImageNormeFourier, Rect(0, cy, cx, cy));  // Cadrant 3,  en bas à gauche
	    Mat cadran3(ImageNormeFourier, Rect(cx, cy, cx, cy)); // Cadrant 2,  en bas à droite

	    Mat temponCadran; // tampon pour l echange des cadrants

	    //Echange entre le Cadrant 0 et le Cadrant 3
	    cadran0.copyTo(temponCadran);
	    cadran3.copyTo(cadran0);
	    temponCadran.copyTo(cadran3);

	    //Echange entre le Cadrant 1 et le Cadrant 2
	    cadran1.copyTo(temponCadran);
	    cadran2.copyTo(cadran1);
	    temponCadran.copyTo(cadran2);

	    normalize(ImageNormeFourier, ImageNormeFourier, 0, 1, CV_MINMAX);
            Mat Sortie;
            ImageNormeFourier.convertTo(Sortie, CV_8UC1,255);

	    return Sortie;

         }

        Mat TansFourierInverse (Mat ImgDeFourrier, int nblignes, int nbcolones){
	Mat ImageRestauree;
	Mat ImageFourierInverse;
	vector<Mat> Conteneurs;

	//Calcul de la Transformee Inverse de Fourier
	dft( ImgDeFourrier,ImageFourierInverse,DFT_INVERSE + DFT_SCALE);
        split(ImageFourierInverse,Conteneurs);
        ImageRestauree = Conteneurs[0];

	//Recadrage de l image suivant nos nouveaux cadrants
	ImageRestauree = ImageRestauree(Rect(0, 0, ImageRestauree.cols & -2, ImageRestauree.rows & -2));

	//Conversion en image à niveaux de gris
	ImageRestauree.convertTo(ImageRestauree, CV_8UC1);
	Mat Sortie (ImageRestauree, Rect(0,0, nbcolones, nblignes));

	return Sortie;
}

// Le main de notre program

int main (int argc, char** argv){


	if(argc!= 2 ){
		cout << "Syntax error, please enter ./Fourier suivi de l'image"<<endl;
	}
	else
	{
		cout << "Resultat du Spectre de l image donnee et l'image issue de la Transformee Inverse du Spectre"<<endl;

		const char* ImgName = argv[1];
		//LA fonction ci permet d'ouvrir l'image avec une nuuance de gris
		Mat InputImage = imread(ImgName, CV_LOAD_IMAGE_GRAYSCALE);


 // Ci dessous on va calculer de la transformé de fourier de l'image
 Mat TransFourierImage = TransformationFourier(RedimensionnerImage(InputImage));
 Mat SpectreImage = NewCadran(NormDeCalcul(TransFourierImage));
 Mat ImageRestauree = TansFourierInverse(TransFourierImage,InputImage.rows, InputImage.cols);

 // Avec imshow, on affiche les images dans leur fenetre respective
   imshow("Image Donnee", InputImage);
   imshow("Spectre de Fourier de l image donnee", SpectreImage);
   imshow("Transformee inverse du spectre", ImageRestauree);

// Enregistrement des sorties 
 imwrite("Image_Entree.png", InputImage);
 imwrite("Spectre.png", SpectreImage);
 imwrite("Image_Transformee-Inverse.png", ImageRestauree);		


	}
	waitKey();
	return 0;
     }

// Fin de notre program


