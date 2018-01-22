//============================================================================
// Name        : TesseractTrials.cpp
// Author      : Harold Martin
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <string>
using std::string;

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main() {
	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, "eng_lstm", tesseract::OEM_DEFAULT)) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	// Open input image with leptonica library
	Pix *image = pixRead("/home/harold/Pictures/book1.tiff");
	api->SetImage(image);
//	api->SetPageSegMode(tesseract::PSM_AUTO);

//	// Get OCR result
//	outText = api->GetUTF8Text();
//	printf("OCR output:\n%s", outText);

	Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);
		char* ocrResult = "";
		int conf = -1;
		ocrResult = api->GetUTF8Text();
		conf = api->MeanTextConf();
		fprintf(stdout,
				"Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s \n", i,
				box->x, box->y, box->w, box->h, conf, ocrResult);
	}

	// Destroy used object and release memory
	api->End();
	delete[] outText;
	pixDestroy(&image);

	return 0;
}
