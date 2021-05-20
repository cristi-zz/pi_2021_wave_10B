// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

////// nuclee de convolutie 

int H[] = { 1, -1 };
int G[] = { 1, 1 };

void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("opened image",src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void testColor2Gray()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<Vec3b> src = imread(fname, IMREAD_COLOR);

		int height = src.rows;
		int width = src.cols;

		Mat_<uchar> dst(height, width);

		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				Vec3b v3 = src(i,j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst(i,j) = (r+g+b)/3;
			}
		}
		
		imshow("original image",src);
		imshow("gray image",dst);
		waitKey();
	}
}

///////////////////////////////////////////////////////////////////////////////
/// Wavelet grupa 10B
///////////////////////////////////////////////////////////////////////////////

int* convert2Dto1D(Mat_<uchar> mat)
{
	int* arr = (int*)malloc(sizeof(int) * mat.rows * mat.cols);
	int index = 0;

	for (int i = 0; i < mat.rows; i++)
	{
		for (int j = 0; j < mat.cols; j++)
		{
			arr[index++] = mat(i, j);
		}
	}

	return arr;
}

int* getLow(int *array1D, int length)
{
	int* low = (int*)malloc(sizeof(int) * (length / 2));
	for (int i = 0; i < length / 2; i++)
	{
		low[i] = (float)(1.0 / 2) * (array1D[i * 2] + array1D[i * 2 + 1]);
	}
	return low;
}

int* getHigh(int* array1D, int length)
{
	int* high = (int*)malloc(sizeof(int) * (length / 2));
	for (int i = 0; i < length / 2; i++)
	{
		high[i] = (float)(1.0 / 2) * (array1D[i * 2] - array1D[i * 2 + 1]);
	}
	return high;
}

int* getHighSample(int* high, int length)
{
	int* highSample = (int*)malloc(sizeof(int) * (length));
	for (int i = 0; i < length; i++)
	{
		highSample[i] = high[i / 2] * H[i % 2];
	}
	return highSample;
}

int* getLowSample(int* low, int length)
{
	int* lowSample = (int*)malloc(sizeof(int) * (length));
	for (int i = 0; i < length; i++)
	{
		lowSample[i] = low[i / 2];
	}
	return lowSample;
}

int* getUpSample(int* highSample, int* lowSample, int length)
{
	int* upSample = (int*)malloc(sizeof(int) * (length));
	for (int i = 0; i < length; i++)
	{
		upSample[i] = lowSample[i] + highSample[i];
	}
	return upSample;
}

void print(int *array, int length)
{
	for (int i = 0; i < length; i++)
	{
		printf("%d ", array[i]);
	}
}

int* concat(int* lowUpSample, int* highUpSample, int length)
{
	int* upSample = (int*)malloc(sizeof(int) * (length * 2));
	int index = 0;
	for (int i = 0; i < length; i++)
	{
		upSample[index++] = highUpSample[i];
	}
	for (int i = 0; i < length; i++)
	{
		upSample[index++] = lowUpSample[i];
	}
	return upSample;
}

void testWavelet()
{
	char fname[MAX_PATH];
	openFileDlg(fname);
	Mat_<uchar> img = imread(fname, IMREAD_GRAYSCALE);

	int array[] = { 9,7,3,5,6,10,2,6 };
	int n = 8;
	int* high = getHigh(array, n);
	int* low = getLow(array, n);

	int* highSample = getHighSample(high, n);
	int* lowSample = getLowSample(low, n);

	printf("\nTest Vector : ");
	print(array, n);

	printf("\n\n");
	printf("Primul nivel : ");

	printf("\nHigh : ");
	print(high, n / 2);
	printf("\nLow : ");
	print(low, n / 2);

	printf("\nHigh Sample : ");
	print(highSample, n);
	printf("\nLow Sample : ");
	print(lowSample, n);

	int* upSample = getUpSample(highSample, lowSample, n);
	printf("\nUpSample : ");
	print(upSample, n);

	printf("\n\n");
	printf("Al doilea nivel : ");

	int* high2_high = getHigh(high, n / 2);
	int* low2_high = getLow(high, n / 2);

	int* high2_low = getHigh(low, n / 2);
	int* low2_low = getLow(low, n / 2);

	printf("\nHigh2 High : ");
	print(high2_high, n / 4);
	printf("\nLow2 High : ");
	print(low2_high, n / 4);
	printf("\nHigh2 Low : ");
	print(high2_low, n / 4);
	printf("\nLow2 Low : ");
	print(low2_low, n / 4);

	int* high2_highSample = getHighSample(high2_high, n / 2);
	int* low2_highSample = getLowSample(low2_high, n / 2);
	int* high2_lowSample = getHighSample(high2_low, n / 2);
	int* low2_lowSample = getLowSample(low2_low, n / 2);

	printf("\nHigh2 High Sample : ");
	print(high2_highSample, n / 2);
	printf("\nLow2 High Sample : ");
	print(low2_highSample, n / 2);
	printf("\nHigh2 Low Sample : ");
	print(high2_lowSample, n / 2);
	printf("\nLow2 Low Sample : ");
	print(low2_lowSample, n / 2);

	int* upSampleHigh2 = getUpSample(high2_highSample, low2_highSample, n / 2);
	int* upSampleLow2 = getUpSample(high2_lowSample, low2_lowSample, n / 2);
	int* upSample2 = concat(upSampleHigh2, upSampleLow2, n / 2);

	printf("\nLow2 Up Sample : ");
	print(upSampleLow2, n / 2);
	printf("\nHigh2 Up Sample : ");
	print(upSampleHigh2, n / 2);
	printf("\nUp Sample : ");
	print(upSample2, n);

	imshow("Test Image", img);
	waitKey();
}

void wavelet1D()
{
	char fname[MAX_PATH];
	openFileDlg(fname);
	Mat_<uchar> img = imread(fname, IMREAD_GRAYSCALE);

	int* array = convert2Dto1D(img);
	int n = img.rows * img.cols;

	int* high = getHigh(array, n);
	int* low = getLow(array, n);

	int* highSample = getHighSample(high, n);
	int* lowSample = getLowSample(low, n);

	int* upSample = getUpSample(highSample, lowSample, n);

	int* high2_high = getHigh(high, n / 2);
	int* low2_high = getLow(high, n / 2);

	int* high2_low = getHigh(low, n / 2);
	int* low2_low = getLow(low, n / 2);

	int* high2_highSample = getHighSample(high2_high, n / 2);
	int* low2_highSample = getLowSample(low2_high, n / 2);
	int* high2_lowSample = getHighSample(high2_low, n / 2);
	int* low2_lowSample = getLowSample(low2_low, n / 2);

	int* upSampleHigh2 = getUpSample(high2_highSample, low2_highSample, n / 2);
	int* upSampleLow2 = getUpSample(high2_lowSample, low2_lowSample, n / 2);
	int* upSample2 = concat(upSampleHigh2, upSampleLow2, n / 2);

	imshow("Test Image", img);
	waitKey();
}

Mat_<uchar> LL(Mat_<uchar> img)
{
	int contor = 0;
	Mat_<uchar> dest = Mat_<uchar>(img.rows / 2, img.cols);
	Mat_<uchar> LL = Mat_<uchar>(img.rows / 2, img.cols / 2);
	for (int j = 0; j < img.cols; j++)
	{
		int* k = (int*)malloc(sizeof(int) * img.rows);
		for (int i = 0; i < img.rows; i++)
		{
			k[i] = img(i, j);
		}
		int* k1 = getLow(k, img.rows);
		for (int i = 0; i < img.rows / 2; i++)
		{
			dest(i, contor) = k1[i];
		}
		contor++;
		free(k);
		free(k1);
	}
	contor = 0;
	for (int i = 0; i < dest.rows; i++)
	{
		int* k = (int*)malloc(sizeof(int) * dest.cols);
		for (int j = 0; j < dest.cols; j++)
		{
			k[j] = dest(i, j);
		}
		int* k1 = getLow(k, dest.cols);
		for (int r = 0; r < dest.cols / 2; r++)
		{
			LL(contor, r) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	return LL;
}

Mat_<uchar> HH(Mat_<uchar> img)
{
	int contor = 0;
	Mat_<uchar> dest = Mat_<uchar>(img.rows / 2, img.cols);
	Mat_<uchar> HH = Mat_<uchar>(img.rows / 2, img.cols / 2);
	for (int j = 0; j < img.cols; j++)
	{
		int* k = (int*)malloc(sizeof(int) * img.rows);
		for (int i = 0; i < img.rows; i++)
		{
			k[i] = img(i, j);
		}
		int* k1 = getHigh(k, img.rows);
		for (int r = 0; r < img.rows / 2; r++)
		{
			dest(r, contor) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	contor = 0;
	for (int i = 0; i < dest.rows; i++)
	{
		int* k = (int*)malloc(sizeof(int) * dest.cols);
		for (int j = 0; j < dest.cols; j++)
		{
			k[j] = dest(i, j);
		}
		int* k1 = getHigh(k, dest.cols);
		for (int r = 0; r < dest.cols / 2; r++)
		{
			HH(contor, r) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	return HH;
}
Mat_<uchar> LH(Mat_<uchar> img)
{
	int contor = 0;
	Mat_<uchar> dest = Mat_<uchar>(img.rows / 2, img.cols);
	Mat_<uchar> LH = Mat_<uchar>(img.rows / 2, img.cols / 2);
	for (int j = 0; j < img.cols; j++)
	{
		int* k = (int*)malloc(sizeof(int) * img.rows);
		for (int i = 0; i < img.rows; i++)
		{
			k[i] = img(i, j);
		}
		int* k1 = getLow(k, img.rows);
		for (int r = 0; r < img.rows / 2; r++)
		{
			dest(r, contor) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	contor = 0;
	for (int i = 0; i < dest.rows; i++)
	{
		int* k = (int*)malloc(sizeof(int) * dest.cols);
		for (int j = 0; j < dest.cols; j++)
		{
			k[j] = dest(i, j);
		}
		int* k1 = getHigh(k, dest.cols);
		for (int r = 0; r < dest.cols / 2; r++)
		{
			LH(contor, r) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	return LH;
}

Mat_<uchar> HL(Mat_<uchar> img)
{
	int contor = 0;
	Mat_<uchar> dest = Mat_<uchar>(img.rows / 2, img.cols);
	Mat_<uchar> HL = Mat_<uchar>(img.rows / 2, img.cols / 2);
	for (int j = 0; j < img.cols; j++)
	{
		int* k = (int*)malloc(sizeof(int) * img.rows);
		for (int i = 0; i < img.rows; i++)
		{
			k[i] = img(i, j);
		}
		int* k1 = getHigh(k, img.rows);
		for (int r = 0; r < img.rows / 2; r++)
		{
			dest(r, contor) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	contor = 0;
	for (int i = 0; i < dest.rows; i++)
	{
		int* k = (int*)malloc(sizeof(int) * dest.cols);
		for (int j = 0; j < dest.cols; j++)
		{
			k[j] = dest(i, j);
		}
		int* k1 = getLow(k, dest.cols);
		for (int r = 0; r < dest.cols / 2; r++)
		{
			HL(contor, r) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	return HL;
}


Mat_<uchar> reconstructieLL(Mat_<uchar> img)
{
	int contor = 0;
	Mat_<uchar> dest = Mat_<uchar>(img.rows, img.cols*2);
	Mat_<uchar> rec = Mat_<uchar>(img.rows*2, img.cols*2);
	for (int j = 0; j < img.rows; j++)
	{
		int* k = (int*)malloc(sizeof(int) * img.cols);
		for (int i = 0; i < img.cols; i++)
		{
			k[i] = img(i, j);
		}
		int* k1 = getHighSample(k, img.cols*2);
		for (int r = 0; r < dest.cols; r++)
		{
			dest(contor,r) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}

	contor = 0;
	for (int i = 0; i < dest.cols; i++)
	{
		int* k = (int*)malloc(sizeof(int) * dest.rows);
		for (int j = 0; j < dest.rows; j++)
		{
			k[j] = dest(j, i);
		}
		int* k1 = getHighSample(k, dest.rows*2);
		for (int r = 0; r < rec.rows; r++)
		{
			rec(r,contor) = k1[r];
		}
		contor++;
		free(k);
		free(k1);
	}
	return rec;
}


void wavelet2D()
{
	char fname[MAX_PATH];
	openFileDlg(fname);
	Mat_<uchar> img = imread(fname, IMREAD_GRAYSCALE);
	Mat_<uchar> LLmat = Mat_<uchar>(img.rows / 2, img.cols / 2);
	Mat_<uchar> LHmat = Mat_<uchar>(img.rows / 2, img.cols / 2);
	Mat_<uchar> HLmat = Mat_<uchar>(img.rows / 2, img.cols / 2);
	Mat_<uchar> HHmat = Mat_<uchar>(img.rows / 2, img.cols / 2);
	Mat_<uchar> RecLLmat = Mat_<uchar>(img.rows, img.cols);

	LLmat = LL(img);
	HHmat = HH(img);
	LHmat = LH(img);
	HLmat = HL(img);
	//RecLLmat = reconstructieLL(LLmat);


	imshow("Imagine", img);
	imshow("HHmat", HHmat);
	imshow("LLmat", LLmat);
	imshow("LHmat", LHmat);
	imshow("HLmat", HLmat);
	//imshow("ImagineRec", RecLLmat);
	waitKey();
}

Mat_<uchar> coef_to_0(Mat_<uchar> img, int th) {
	Mat_<uchar> dst(img.rows, img.cols);
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++) {
			if (img(i, j) < th) {
				dst(i, j) = 0;
			}
			else {
				dst(i, j) = img(i, j);
			}
		}
	}
	return dst;
}

double MeanSquareError(Mat_<uchar> original, Mat_<uchar> result)
{
	double mse = 0, mseFin = 0;
	for (int i = 0; i < original.rows; i++)
	{
		for (int j = 0; j < original.cols; j++)
		{
			mse += abs(original[i][j] - result[i][j]);
		}
	}
	mse /= (double)(original.rows * original.cols);
	mseFin = sqrt(mse);
	return mseFin;
}

void MSE()
{
	char fname[MAX_PATH];
	openFileDlg(fname);
	Mat_<uchar> img = imread(fname, IMREAD_GRAYSCALE);
	Mat_<uchar> LLmat = LL(img);
	Mat_<uchar> LHmat = LH(img);
	Mat_<uchar> HLmat = HL(img);
	Mat_<uchar> HHmat = HH(img);

	int th = 100;

	Mat_<uchar> LLmatCoef = coef_to_0(LLmat,th);
	Mat_<uchar> LHmatCoef = coef_to_0(LHmat, th);
	Mat_<uchar> HLmatCoef = coef_to_0(HLmat, th);
	Mat_<uchar> HHmatCoef = coef_to_0(HHmat, th);

	double mseLL = MeanSquareError(LLmat, LLmatCoef);
	double mseLH = MeanSquareError(LHmat, LHmatCoef);
	double mseHL = MeanSquareError(HLmat, HLmatCoef);
	double mseHH = MeanSquareError(HHmat, HHmatCoef);

	printf("LL mse: %f\n LH mse: %f\n HL mse: %f\n HH mse: %f\n", mseLL, mseLH, mseHL, mseHH);

	imshow("Imagine", img);
	waitKey(0);
}
int main()
{
	int op;
	
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Basic image opening...\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Color to Gray\n");
		printf(" 4 - Test Wavelet\n");
		printf(" 5 - Wavelet1D\n");
		printf(" 6 - Wavelet2D\n");
		printf(" 7 - MSE\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testColor2Gray();
				break;
			case 4:
				testWavelet();
				break;
			case 5:
				wavelet1D();
				break;
			case 6:
				wavelet2D();
				break;
			case 7:
				MSE();
				break;
		}
	}
	while (op!=0);
	return 0;
}
