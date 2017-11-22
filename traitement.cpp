/*    Cours de Traitement d'images
      TP3 - TP Final - Chaîne complète
      	         	
     Binome: P20
              ELIODOR Ednalson Guy Mirlin

*/

#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#define SUM_LEVEL 256

using namespace cv;
using namespace std;

void initHistData(int data[], int N)
{
    for (int i = 0; i < SUM_LEVEL; i++)
      data[i] = 0;
}

void initHistData(float data[], int N)
{
    for (int i = 0; i < SUM_LEVEL; i++)
      data[i] = 0;
}

void initHistData(uchar data[], int N)
{
    for (int i = 0; i < SUM_LEVEL; i++)
      data[i] = 0;
}

void setHistData(Mat src, int data[], int N)
{
    int data_index = 0;
    int img_w = src.cols;
    int img_h = src.rows;
    for(int i = 0; i < img_h; i++)
    for (int j = 0; j < img_w; ++j)
    {
        data_index = src.at<uchar>(i,j);
        data[data_index]++;
    }
}

Scalar getScalar(int chanel)
{
    switch(chanel)
    {
        case 0:
          return Scalar(255, 0, 0);
        case 1:
          return Scalar(0, 255, 0);
        case 2:
          return Scalar(0, 0, 255);
    }
    return  Scalar(255, 0, 0);
}

void drawHist(Mat src, Mat hist, int chanel)// Tracer l'histogramme
{
    int hist_h = hist.rows/3;
    int data[SUM_LEVEL];
    Scalar scalar;
    Point point_start, point_end;
    initHistData(data, SUM_LEVEL);
    setHistData(src, data, SUM_LEVEL);

    int data_element = 0;
    int max = 0;
    for (int i = 0; i < SUM_LEVEL; ++i)
    {
        data_element = data[i];
        if (max < data_element)
            max = data_element;
    }

    scalar = getScalar(chanel);
    for (int i = 1; i < SUM_LEVEL; i++)
    {
        point_start = Point(i, chanel*hist_h);
        point_end = Point(i, chanel*hist_h - hist_h*data[i]/max);
        line( hist, point_start, point_end, scalar, 2, 8, 0 );
    }
}

// Calcul du contraste
Mat calcContrastLinaireMorceaux(Mat src, float r1, float s1, float r2, float s2)
{
    if(r2 == r1 || r1 == 0 || r2 == 255)
        return src;
    int img_h = src.rows;
    int img_w = src.cols;
    uchar **I;
    I = new uchar*[img_h];
    //Init data
    for (int i = 0; i < img_h; ++i)
        I[i] = new uchar[img_w];

    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
        {
            I[i][j] = src.at<uchar>(i, j);
        }

    int T[SUM_LEVEL];
    //Calculer le niveau de gris
    for (int r = 0; r < SUM_LEVEL; ++r)
    {
        if(r <= r1)
            T[r] = r*s1/r1;
        else if(r >= r2)
                T[r] = s2 +(r-r2)*(255-s2)/(255-r2);
             else
                T[r] = s1 + (r-r1)*(s2-s1)/(r2-r1);
    }

	//Metre à jour le donnée dans l'image basé sur le résultat de T[]
    for (int i = 0; i < img_h; ++i)
        for (int j = 0; j < img_w; ++j)
        {
            src.at<uchar>(i, j) = T[I[i][j]];
        }
    for (int i = 0; i < img_h; ++i)
    {
      delete[] I[i];
    }
    delete[] I;

    return src;
}

Mat contrastLinaireMorceaux(Mat src) // Contraste Lineaire par morceaux
{
    Mat hist(600, 256, CV_8UC3, Scalar(0, 0, 0));
    Mat hist_contrast(600, 256, CV_8UC3, Scalar(0, 0, 0));

    Mat image_contrast = src.clone();
    Mat image_contrast1 = src.clone();
	
	//coordonnees de trois points dans le contraste linaire morceaux
    float r1 = 90, s1 = 10, r2 = 120, s2 = 150, r3 = 200, s3 = 230;

	//histogramme de l'image originale
    drawHist(src, hist, 1);
    imwrite("hist.jpg", hist);
    imshow("hist.jpg", hist);

    image_contrast = calcContrastLinaireMorceaux(src, r1, s1, r2, s2);
    image_contrast1 = calcContrastLinaireMorceaux(image_contrast, r2, s2, r3, s3);

	//histogramme de'image de contraste
    drawHist(src, hist_contrast, 1);
	
    imwrite("hist_contrast.jpg", hist_contrast);
    imwrite("contrast.jpg", image_contrast);
    imshow("hist_contrast.jpg", hist_contrast);
    imshow("contrast.jpg", image_contrast);
    return image_contrast;
}


int main(int argc, char** argv) {

// 1@Segmetation...................Debut 
	
	Mat img0 = imread(argv[1]); // Cette fonction nous permet de faire la lecture de l'img couleur en entrée
	if (!img0.data) {
		cout << "Attention, il faut donner le nom de l'image\n" << endl;
		return -1;
	}
         else
           cout << " Chaine de traitement en cours d'execution...\n" <<endl;

	Mat img1;
	cvtColor(img0, img1, COLOR_RGB2GRAY); // La Fx cvtColor nous permet de transformer l'img couleur en img en niveaux de gris
	
	//augmenter le contraste
	img1 = contrastLinaireMorceaux(img1);
	//diminuer les bruits
	GaussianBlur( img1, img1, Size(5,5), 0, 0, BORDER_DEFAULT );

	//diminuer la lumiere d'image
	img1 += Scalar(-135, -135, -135); //meuilleur pour objets4.jpg et objets1.jpg
	//img1 += Scalar(-160, -160, -160); //meuilleur pour objets3.jpg
	//img1 += Scalar(-165, -165, -165); //meuilleur pour objets2.jpg
	imwrite("blur.png", img1);

	//Ici, on va appliquer l'algorithme de OTSU pour etablir le seuil 
	threshold(img1, img1, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

// 1@Segmentation.......................Fin

// ------------------------------------------------------------------------------------------------

// 2@Post-Segmentation...............Debut

// Ajoutdes éléments structurants
	Mat item = getStructuringElement(MORPH_RECT, Size(5, 5));// Size pour la dilatation
	Mat item1 = getStructuringElement(MORPH_RECT, Size(2, 2)); //Size pour l'erosion
	Mat img2;

  //dilatation de limg a l'element structurant avec la fx dilate
	dilate(img1, img2, item);
  // La fonction erode pour l'erosion des imgs et les elements structurants
	erode(img2, img2, item1);

	
	vector<vector<Point> > contours; // Ce vecteur va permettre de stocker les contours des régions

//Recherche des contours des régions détectées que va contenir l'element vector
	findContours(img2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	
	Mat check = Mat::zeros(img2.size(), CV_32SC1); // Marquage des régions
	for (size_t i = 0; i < contours.size(); i++)
		drawContours(check, contours, static_cast<int>(i),
				Scalar::all(static_cast<int>(i) + 1), -1);

	//ici@Generation....Aleatoire...de....couleurs
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int blue = theRNG().uniform(0, 255);
		int green = theRNG().uniform(0, 255);
		int red = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar) blue, (uchar) green, (uchar) red));
	}

	// Coloration des régions avec des couleurs différentes
	Mat img3 = Mat::zeros(check.size(), CV_8UC3);
	for (int i = 0; i < check.rows; i++) 
              {
		for (int j = 0; j < check.cols; j++)
              {
			int index = check.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
				img3.at<Vec3b>(i, j) = colors[index - 1];
			else
				img3.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}

// Conjonction de l'img entrée et le masque obtenu
	Mat img4;
	img0.copyTo(img4, img3);

//2@Post-Segmentation----------------------Fin

//------------------------------------------------------------------------------------------------------------------

//Affichage des sorties a l'ecran
	imshow("image Donnee", img0);
	imshow("image_Segm_OTU", img1);
	imwrite("imgSegmOTSU.png", img1);
	
	imshow("imagePostSegmenation", img3);
	imwrite("imgPostSegm.png", img3);
		
	imshow("imageSegmentee", img4);
	imwrite("imageSegmentee.png", img4);
	waitKey(0);
	return 0;
}

//Fin du programme
