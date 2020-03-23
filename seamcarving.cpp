#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

int main() {
  string filename;
  int width = 0;
  int height = 0;
  int targetWidth = 0;
  int targetHeight = 0;
  
  cout << "Input filename: ";
  cin >> filename;
  
  cout << "Input width and height: ";

  if (!(cin >> width)) {
    cout << "Error: width is a non-integer value" << endl;
    return -1;
  } else if (!(cin >> height)) {
    cout << "Error: height is a non-integer value" << endl;
    return -1;
  }

  cout << "Input target width and target height: ";

  if (!(cin >> targetWidth)) {
    cout << "Error: target width is a non-integer value" << endl;
    return -1;
  } else if (!(cin >> targetHeight)) {
    cout << "Error: target height is a non-integer value" << endl;
    return -1;
  }

  
  if (width <= 0) {
    cout << "Error: width must be greater than 0. You entered " << width << endl;
    return -1;
  } else if (height <= 0) {
    cout << "Error: height must be greater than 0. You entered " << height << endl;
    return -1;
  } else if (targetWidth <= 0) {
    cout << "Error: target width must be greater than 0. You entered " << targetWidth << endl;
    return -1;
  } else if (targetHeight <= 0) {
    cout << "Error: target height must be greater than 0. You entered " << targetHeight << endl;
    return -1;
  } else if (targetWidth > width) {
    cout << "Error: target width must be less than width, " << targetWidth << " is greater than " << width << endl;
    return -1;
  } else if (targetHeight > height) {
    cout << "Error: target height must be less than height, " << targetHeight << " is greater than " << height << endl;
    return -1;
  }

  
  int originalWidth = width;
  int* verticalSeam = new int[height];
  int* horizontalSeam = new int[width];

  Pixel** image = createImage(width, height); // create array of size
  if (image != nullptr) {
    if (loadImage(filename, image, width, height)) {
      cout << "Start carving..." << endl;
      
      while (width > targetWidth) {
        verticalSeam = findMinVerticalSeam(image, width, height);
        removeVerticalSeam(image, width, height, verticalSeam);
        delete [] verticalSeam;
        width -= 1;
      }
      
      while (height > targetHeight) {
        horizontalSeam = findMinHorizontalSeam(image, width, height);
        removeHorizontalSeam(image, width, height, horizontalSeam);
        delete [] horizontalSeam;
        height -= 1;
      }
      
      // set up output filename
      stringstream ss;
      ss << "carved" << width << "X" << height << "." << filename;
      outputImage(ss.str().c_str(), image, width, height);
    }
  
    deleteImage(image, originalWidth);
  } else {
    return -1;
  }
}