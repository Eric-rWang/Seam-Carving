#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using namespace std;

Pixel** createImage(int width, int height) {
  cout << "Start createImage... " << endl;
  
  // Create a one dimensional array on the heap of pointers to Pixels 
  //    that has width elements (i.e. the number of columns)
  Pixel** image = new Pixel*[width];
  
  bool fail = false;
  
  for (int i=0; i < width; ++i) { // loop through each column
    // assign that column to a one dimensional array on the heap of Pixels
    //  that has height elements (i.e. the number of rows)
    image[i] = new Pixel[height];
    
    if (image[i] == nullptr) { // failed to allocate
      fail = true;
    }
  }
  
  if (fail) { // if any allocation fails, clean up and avoid memory leak
    // deallocate any arrays created in for loop
    for (int i=0; i < width; ++i) {
      delete [] image[i]; // deleting nullptr is not a problem
    }
    delete [] image; // dlete array of pointers
    return nullptr;
  }
  
  // initialize cells
  //cout << "Initializing cells..." << endl;
  for (int row=0; row<height; ++row) {
    for (int col=0; col<width; ++col) {
      //cout << "(" << col << ", " << row << ")" << endl;
      image[col][row] = { 0, 0, 0 };
    }
  }
  cout << "End createImage... " << endl;
  return image;
}

void deleteImage(Pixel** image, int width) {
  cout << "Start deleteImage..." << endl;
  // avoid memory leak by deleting the array
  for (int i=0; i<width; ++i) {
    delete [] image[i]; // delete each individual array placed on the heap
  }
  delete [] image;
  image = nullptr;
}

// implement for part 1

int* createSeam(int length) {
  int* seam = new int[length];
  return seam;
}

void deleteSeam(int* seam) {
  delete [] seam;
}

bool loadImage(string filename, Pixel** image, int width, int height) {
  // declare/define and open input file stream
  ifstream ifs (filename);
  
  // check if input stream opened successfully
  if (!ifs.is_open()) {
    cout << "Error: failed to open input file - " << filename << endl;
    return false;
  }

  char type[3];
  if (!(ifs >> type)) {
    cout << "Error: read non-integer value" << endl;
    return false;
  }

  if ((toupper(type[0]) != 'P') || (type[1] != '3')) { // check that type is correct
    cout << "Error: type is " << type << " instead of P3" << endl;
    return false;
  }

  int w = 0, h = 0;
  if (!(ifs >> w) || !(ifs >> h)) {
    cout << "Error: read non-integer value" << endl;
    return false;
  }

  if (w != width || !width) { // check that width matches what was passed into the function
    cout << "Error: input width (" << width <<  ") does not match value in file ("
    << w << ")" << endl;
    return false;
  }
  if (h != height || !height) { // check that height matches what was passed into the function
    cout << "Error: input height (" << height <<  ") does not match value in file ("
    << h << ")" << endl;
    return false;
  }

  int max = 0;
  ifs >> max;

  if(ifs.fail()) {
    if (ifs.eof()) {
      cout << "Error: not enough color values" << endl;
      return false;
    }
    cout << "Error: read non-integer value" << endl;
    return false;
  }

  if (max != 255) {
    cout << "Error: colorMax not 255" << endl;
    return false;
  }

  int r, g, b;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      ifs >> r;

      if (ifs.fail()) {
        if (ifs.eof()) {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        cout << "Error: read non-integer value" << endl;
        return false;
      }

      ifs >> g;

      if (ifs.fail()) {
        if (ifs.eof()) {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        cout << "Error: read non-integer value" << endl;
        return false;
      }

      ifs >> b;

      if (ifs.fail()) {
        if (ifs.eof()) {
          cout << "Error: not enough color values" << endl;
          return false;
        }
        cout << "Error: read non-integer value" << endl;
        return false;
      }

      if (r < 0 || r > 255) {
        cout << "Error: invalid color value " << r << endl;
        return false;
      } else if (g < 0 || g > 255) {
        cout << "Error: invalid color value " << g << endl;
        return false;
      } else if (b < 0 || b > 255) {
        cout << "Error: invalid color value " << b << endl;
        return false;
      }

      struct Pixel p;
      p.r = r;
      p.g = g;
      p.b = b;

      image[j][i] = p;
    }
  }

  char extra;
  ifs >> extra;

  while (!ifs.eof()) {
    if (!isspace(extra)) {
      cout << "Error: too many color values" << endl;
      return false;
    }
    ifs >> extra;
  }

  ifs.close();
  return true;
}

bool outputImage(string filename, Pixel** image, int width, int height) {
  ofstream ofs (filename);

  // check if output stream opened successfully
  if (!ofs.is_open()) {
    cout << "Error: failed to open input file " << filename << endl;
    return false;
  }
  
  // output preamble
  cout << "P3" << endl;

  cout << width << " " << height << endl;

  cout << "255" << endl;

  // output pixels
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      ofs << image[j][i].r << " " << image[j][i].g << " " << image[j][i].b << " ";
    }
  }

  ofs.close();
  return true;
}

int energy(Pixel** image, int x, int y, int width, int height) { 
  int xr, xg, xb;
  int yr, yg, yb;
  int xEnergy, yEnergy, pxlEnergy;

  if (x == 0 && y == 0) {
    xr = abs(image[x + 1][y].r - image[width - 1][y].r);
    xg = abs(image[x + 1][y].g - image[width - 1][y].g);
    xb = abs(image[x + 1][y].b - image[width - 1][y].b);

    yr = abs(image[x][y + 1].r - image[x][height - 1].r);
    yg = abs(image[x][y + 1].g - image[x][height - 1].g);
    yb = abs(image[x][y + 1].b - image[x][height - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else if (x == 0 && y == height - 1) {
    xr = abs(image[x + 1][y].r - image[width - 1][y].r);
    xg = abs(image[x + 1][y].g - image[width - 1][y].g);
    xb = abs(image[x + 1][y].b - image[width - 1][y].b);

    yr = abs(image[x][0].r - image[x][y - 1].r);
    yg = abs(image[x][0].g - image[x][y - 1].g);
    yb = abs(image[x][0].b - image[x][y - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else if (x == width - 1 && y == 0) {
    xr = abs(image[0][y].r - image[x - 1][y].r);
    xg = abs(image[0][y].g - image[x - 1][y].g);
    xb = abs(image[0][y].b - image[x - 1][y].b);

    yr = abs(image[x][y + 1].r - image[x][height - 1].r);
    yg = abs(image[x][y + 1].g - image[x][height - 1].g);
    yb = abs(image[x][y + 1].b - image[x][height - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else if (x == 0) {
    xr = abs(image[x + 1][y].r - image[width - 1][y].r);
    xg = abs(image[x + 1][y].g - image[width - 1][y].g);
    xb = abs(image[x + 1][y].b - image[width - 1][y].b);

    yr = abs(image[x][y + 1].r - image[x][y - 1].r);
    yg = abs(image[x][y + 1].g - image[x][y - 1].g);
    yb = abs(image[x][y + 1].b - image[x][y - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else if (y == 0) {
    xr = abs(image[x + 1][y].r - image[x - 1][y].r);
    xg = abs(image[x + 1][y].g - image[x - 1][y].g);
    xb = abs(image[x + 1][y].b - image[x - 1][y].b);

    yr = abs(image[x][y + 1].r - image[x][height - 1].r);
    yg = abs(image[x][y + 1].g - image[x][height - 1].g);
    yb = abs(image[x][y + 1].b - image[x][height - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else if (x == width - 1 && y == height - 1) {
    xr = abs(image[0][y].r - image[x - 1][y].r);
    xg = abs(image[0][y].g - image[x - 1][y].g);
    xb = abs(image[0][y].b - image[x - 1][y].b);

    yr = abs(image[x][0].r - image[x][y - 1].r);
    yg = abs(image[x][0].g - image[x][y - 1].g);
    yb = abs(image[x][0].b - image[x][y - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else if (x == width - 1) {
    xr = abs(image[0][y].r - image[x - 1][y].r);
    xg = abs(image[0][y].g - image[x - 1][y].g);
    xb = abs(image[0][y].b - image[x - 1][y].b);

    yr = abs(image[x][y + 1].r - image[x][y - 1].r);
    yg = abs(image[x][y + 1].g - image[x][y - 1].g);
    yb = abs(image[x][y + 1].b - image[x][y - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else if (y == height - 1) {
    xr = abs(image[x + 1][y].r - image[x - 1][y].r);
    xg = abs(image[x + 1][y].g - image[x - 1][y].g);
    xb = abs(image[x + 1][y].b - image[x - 1][y].b);

    yr = abs(image[x][0].r - image[x][y - 1].r);
    yg = abs(image[x][0].g - image[x][y - 1].g);
    yb = abs(image[x][0].b - image[x][y - 1].b);

    xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
    yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

    pxlEnergy = xEnergy + yEnergy;

    return pxlEnergy;
  } else {
  xr = abs(image[x + 1][y].r - image[x - 1][y].r);
  xg = abs(image[x + 1][y].g - image[x - 1][y].g);
  xb = abs(image[x + 1][y].b - image[x - 1][y].b);

  yr = abs(image[x][y + 1].r - image[x][y - 1].r);
  yg = abs(image[x][y + 1].g - image[x][y - 1].g);
  yb = abs(image[x][y + 1].b - image[x][y - 1].b);

  xEnergy = pow(xr, 2)  + pow(xg, 2) + pow(xb, 2);
  yEnergy = pow(yr, 2)  + pow(yg, 2) + pow(yb, 2);

  pxlEnergy = xEnergy + yEnergy;

  return pxlEnergy;
  }
}


int loadVerticalSeam(Pixel** image, int start_col, int width, int height, int* seam) {
  int left, center, right, total_Energy;

  total_Energy = energy(image, start_col, 0, width, height);
  seam[0] = start_col;

  for (int i = 1; i < height; i++) {
    if (start_col == 0) {
      center = energy(image, start_col, i, width, height);
      right = energy(image, start_col + 1, i, width, height);
      left = center + right;
    } else if (start_col == width - 1) {
      left = energy(image, start_col - 1, i, width, height);
      center = energy(image, start_col, i, width, height);
      right = left + center;
    } else {
      left = energy(image, start_col - 1, i, width, height);
      center = energy(image, start_col, i, width, height);
      right = energy(image, start_col + 1, i, width, height);
    }

    if (left < center && left < right) {
      // sets start_col to the column one pixel to the left and adds value to seam[i]
      start_col -= 1;
      seam[i] = start_col;
      total_Energy += left;
    } else if (center < left && center < right) {
      // adds center energy value to seam[i]
      seam[i] = start_col;
      total_Energy += center;
    } else if (right < left && right < center) {
      // sets start_col to the column one pixel to the right and adds value to seam[i]
      start_col += 1;
      seam[i] = start_col;
      total_Energy += right;
    } else if (left == center && left < right) {
      // prefer to go directly forward
      seam[i] = start_col;
      total_Energy += center;
    } else if (center == right && center < left) {
      // prefer to go directly forward
      seam[i] = start_col;
      total_Energy += center;
    } else if (right == left && right < center) {
      start_col += 1;
      seam[i] = start_col;
      total_Energy += right;
    } else if (left == center && center == right) {
      // prefer to go directly forward
      seam[i] = start_col;
      total_Energy += center;
    }

  }

  return total_Energy;
}

int loadHorizontalSeam(Pixel** image, int start_row, int width, int height, int* seam) {
  int left, center, right, total_Energy;

  total_Energy = energy(image, 0, start_row, width, height);
  seam[0] = start_row;

  for (int i = 1; i < width; i++) {
    if (start_row == 0) {
      center = energy(image, i, start_row, width, height);
      right = energy(image, i, start_row + 1, width, height);
      left = center + right;
    } else if (start_row == height - 1) {
      left = energy(image, i, start_row - 1, width, height);
      center = energy(image, i, start_row, width, height);
      right = left + center;
    } else {
      left = energy(image, i, start_row - 1, width, height);
      center = energy(image, i, start_row, width, height);
      right = energy(image, i, start_row + 1, width, height);
    }

    if (left < center && left < right) {
      // sets start_col to the column one pixel to the left and adds value to seam[i]
      start_row -= 1;
      seam[i] = start_row;
      total_Energy += left;
    } else if (center < left && center < right) {
      // adds center energy value to seam[i]
      seam[i] = start_row;
      total_Energy += center;
    } else if (right < left && right < center) {
      // sets start_col to the column one pixel to the right and adds value to seam[i]
      start_row += 1;
      seam[i] = start_row;
      total_Energy += right;
    } else if (left == center && left < right) {
      // prefer to go directly forward
      seam[i] = start_row;
      total_Energy += center;
    } else if (center == right && center < left) {
      // prefer to go directly forward
      seam[i] = start_row;
      total_Energy += center;
    } else if (right == left && right < center) {
      start_row -= 1;
      seam[i] = start_row;
      total_Energy += right;
    } else if (left == center && center == right) {
      // prefer to go directly forward
      seam[i] = start_row;
      total_Energy += center;
    }

  }

  return total_Energy;
}

int* findMinVerticalSeam(Pixel** image, int width, int height) {
  int* min_Seam = new int[height];
  int* seam = new int[height];
  int min_Energy, curr_Energy;

  min_Energy = loadVerticalSeam(image, 0, width, height, seam);
  curr_Energy = -1;

  for (int i = 0; i < height; i++) {
        min_Seam[i] = seam[i];
  }

  for (int i = 1; i < width; i++) {
    curr_Energy = loadVerticalSeam(image, i, width, height, seam);

    if (curr_Energy < min_Energy) {
      min_Energy = curr_Energy;

      for (int i = 0; i < height; i++) {
        min_Seam[i] = seam[i];
      }
    }
  }

  delete [] seam;
  seam = nullptr;

  return min_Seam;
}

int* findMinHorizontalSeam(Pixel** image, int width, int height) {
  int* min_Seam = new int[width];
  int* seam = new int[width];
  int min_Energy, curr_Energy;

  min_Energy = loadHorizontalSeam(image, 0, width, height, seam);
  curr_Energy = -1;

  for (int i = 0; i < width; i++) {
        min_Seam[i] = seam[i];
  }

  for (int i = 1; i < height; i++) {
    curr_Energy = loadHorizontalSeam(image, i, width, height, seam);

    if (curr_Energy < min_Energy) {
      min_Energy = curr_Energy;

      for (int i = 0; i < width; i++) {
        min_Seam[i] = seam[i];
      }
    }
  }

  delete [] seam;
  seam = nullptr;

  return min_Seam;
}

void removeVerticalSeam(Pixel** image, int width, int height, int* verticalSeam) {
  Pixel temp; 

  for (int i = 0; i < height; i++) {
    temp = image[verticalSeam[i]][i];

    for (int j = verticalSeam[i]; j < width - 1; j++) {
      image[j][i] = image[j + 1][i];
    }

    image[width - 1][i] = temp;
  }
}

void removeHorizontalSeam(Pixel** image, int width, int height, int* horizontalSeam) {
  Pixel temp; 

  for (int i = 0; i < width; i++) {
    temp = image[i][horizontalSeam[i]];

    for (int j = horizontalSeam[i]; j < height - 1; j++) {
      image[i][j] = image[i][j + 1];
    }

    image[i][height - 1] = temp;
  }
}


















