#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver( const ImagePPM& image ): image_( image ) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage( const ImagePPM& image ) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const{
  return image_;
}

int SeamCarver::GetHeight() const{
  return height_;
}

 int SeamCarver::GetWidth() const{
   return width_;
 }

 int SeamCarver::GetEnergy(int row, int col) const{
   int energy = 0;
   if (col == 0 && row == 0){ // top left corner
     Pixel left_wrap = image_.GetPixel(row, image_.GetWidth() - 1);
     Pixel right = image_.GetPixel(row, col+1);
     Pixel top_wrap = image_.GetPixel(image_.GetHeight() - 1, col);
     Pixel bottom = image_.GetPixel(row+1,col);
     energy = EnergyFormula(left_wrap, right, top_wrap, bottom);
   }
   else if (col == 0 && row == height_ - 1){ // bottom left corner
     Pixel left_wrap = image_.GetPixel(row,image_.GetWidth()-1);
     Pixel right = image_.GetPixel(row, col+1);
     Pixel top = image_.GetPixel(row-1,col);
     Pixel bottom_wrap = image_.GetPixel(0,col);
     energy = EnergyFormula(left_wrap,right,top,bottom_wrap);
   }
   else if (col == 0){ // left edge
     Pixel left_wrap = image_.GetPixel(row, image_.GetWidth() - 1);
     Pixel right = image_.GetPixel(row, col+1);
     Pixel top = image_.GetPixel(row-1,col);
     Pixel bottom = image_.GetPixel(row+1,col);
     energy = EnergyFormula(left_wrap,right,top,bottom);
   }
   else if (col == width_ - 1 && row == 0){ // top right corner
     Pixel left = image_.GetPixel(row, col-1);
     Pixel right_wrap = image_.GetPixel(row,0);
     Pixel top_wrap = image_.GetPixel(image_.GetHeight() - 1, col);
     Pixel bottom = image_.GetPixel(row+1, col);
     energy = EnergyFormula(left,right_wrap,top_wrap,bottom);
   }
   else if (col == width_ - 1 && row == height_ - 1){ // bottom right corner
     Pixel left = image_.GetPixel(row, col-1);
     Pixel right_wrap = image_.GetPixel(row,0);
     Pixel top = image_.GetPixel(row-1,col);
     Pixel bottom_wrap = image_.GetPixel(0,col);
     energy = EnergyFormula(left,right_wrap,top,bottom_wrap);
   }
   else if (col == width_ - 1){
     Pixel left = image_.GetPixel(row, col-1);
     Pixel right_wrap = image_.GetPixel(row,0);
     Pixel top = image_.GetPixel(row-1,col);
     Pixel bottom = image_.GetPixel(row+1, col);
     energy = EnergyFormula(left,right_wrap,top,bottom);
   }
   else if (row == 0){ // top edge
     Pixel left = image_.GetPixel(row, col-1);
     Pixel right = image_.GetPixel(row, col+1);
     Pixel top_wrap = image_.GetPixel(image_.GetHeight() - 1, col);
     Pixel bottom = image_.GetPixel(row+1, col);
     energy = EnergyFormula(left,right,top_wrap,bottom);
   }
   else if (row == height_ - 1){ // bottom edge
     Pixel left = image_.GetPixel(row, col-1);
     Pixel right = image_.GetPixel(row, col+1);
     Pixel top = image_.GetPixel(row-1,col);
     Pixel bottom_wrap = image_.GetPixel(0,col);
     energy = EnergyFormula(left,right,top,bottom_wrap);
   }
   else {
     Pixel left = image_.GetPixel(row, col-1);
     Pixel right = image_.GetPixel(row, col+1);
     Pixel top = image_.GetPixel(row-1,col);
     Pixel bottom = image_.GetPixel(row+1, col);
     energy = EnergyFormula(left,right,top,bottom);
   }
   return energy;
 }

int SeamCarver::SquareNum(int num) const{  // helper
  return num*num;   
}     

int SeamCarver::EnergyFormula(Pixel left, Pixel right, Pixel top, Pixel bottom) const{ // helper
  int energy = SquareNum(left.GetRed() - right.GetRed()) + SquareNum(left.GetGreen() - right.GetGreen()) + SquareNum(left.GetBlue() - right.GetBlue()) + SquareNum(top.GetRed() - bottom.GetRed()) + SquareNum(top.GetGreen() - bottom.GetGreen()) + SquareNum(top.GetBlue() - bottom.GetBlue());
  return energy;
}

int** SeamCarver::AccumulateVertical() const{  
  int** values = new int*[height_];
  for (int row = 0; row < height_; row++){
    values[row] = new int[width_];
  }
  for(int row = 0; row < height_; row++){
    for (int col = 0; col < width_; col++){
      values[row][col] = GetEnergy(row,col);
    }
  }
  for (int row = height_ - 2; row >= 0; row--){
    for (int col = 0; col < width_; col++){
      if (col == 0){
        int bottom = values[row+1][col];
        int bottom_right = values[row+1][col+1];
        if (bottom <= bottom_right){
          values[row][col] = values[row][col] + bottom;
        }
        else {
          values[row][col] = values[row][col] + bottom_right;
        }
      }
      else if (col == width_ - 1){
        int bottom = values[row+1][col];
        int bottom_left = values[row+1][col-1];
        if (bottom <= bottom_left){
          values[row][col] = values[row][col] + bottom;
        }
        else {
          values[row][col] = values[row][col] + bottom_left;
        }
      }
      else {
        int bottom = values[row+1][col];
        int bottom_right = values[row+1][col+1];
        int bottom_left = values[row+1][col-1];
        if (bottom <= bottom_left && bottom <= bottom_right){
          values[row][col] = values[row][col] + bottom;
        }
        else if (bottom_left <= bottom_right && bottom_left < bottom){
          values[row][col] = values[row][col] + bottom_left;
        } 
        else {
          values[row][col] = values[row][col] + bottom_right;
        }
      }
    }
  }
  return values;
}

int** SeamCarver::AccumulateHorizontal() const{
  int** values = new int*[height_];
  for (int row = 0; row < height_; row++){
    values[row] = new int[width_];
  }
  for(int row = 0; row < height_; row++){
    for (int col = 0; col < width_; col++){
      values[row][col] = GetEnergy(row,col);
    }
  }
  for (int col = width_ - 2; col >= 0; col--){
    for (int row = 0; row < height_; row++){
      if (row == 0){
        int right = values[row][col+1];
        int bottom_right = values[row+1][col+1];
        if (right <= bottom_right){
          values[row][col] += right;
        }
        else {
          values[row][col] += bottom_right;
        }
      }
      else if (row == height_ - 1){
        int right = values[row][col+1];
        int top_right = values[row-1][col+1];
        if (right <= top_right){
          values[row][col] += right;
        }
        else {
          values[row][col] += top_right;
        }
      }
      else {
        int right = values[row][col+1];
        int bottom_right = values[row+1][col+1];
        int top_right = values[row-1][col+1];
        if (right <= top_right && right <= bottom_right){
          values[row][col] += right;
        }
        else if (top_right <= bottom_right && top_right < right){
          values[row][col] += top_right;
        }
        else {
          values[row][col] += bottom_right;
        }
      }
    }
  }
  return values;
}

int* SeamCarver::GetHorizontalSeam() const{
  int** values = AccumulateHorizontal();
  int* seam = new int[width_];
  int min_row = 0;
  for (int row = 0; row < height_; row++){
    if (values[row][0] < values[min_row][0]){
      min_row = row;
    }
  }
  int row = min_row;
  for (int col = 0; col < width_ - 1; col++){
    if (col == 0){
      seam[col] = row;
    }
    if (row == 0){
      int right = values[row][col+1];
      int bottom_right = values[row+1][col+1];
      if (right <= bottom_right){
        seam[col+1] = row;
      }
      else {
        row++;
        seam[col+1] = row;
      }
    }
    else if (row == height_ - 1){
      int right = values[row][col+1];
      int top_right = values[row-1][col+1];
      if (right <= top_right){
        seam[col+1] = row;
      }
      else {
        row--;
        seam[col+1] = row;
      }
    }
    else {
      int right = values[row][col+1];
      int bottom_right = values[row+1][col+1];
      int top_right = values[row-1][col+1];
      if (right <= top_right && right <= bottom_right){
        seam[col+1] = row;
      }
      else if (top_right <= bottom_right && top_right < right){
        row--;
        seam[col+1] = row;
      }
      else {
        row++;
        seam[col+1] = row;
      }
    }
  }
  for (int row = 0; row < height_; row++){
    delete[] values[row];
  }
  delete[] values;
  values = nullptr;
  return seam;
}

int* SeamCarver::GetVerticalSeam() const{
  int** values = AccumulateVertical();
  int* seam = new int[height_];
  int min_col = 0;
  for (int col = 0; col < width_; col++){
    if (values[0][col] < values[0][min_col]){
      min_col = col;
    }
  }
  int col = min_col; // start at col that contains smallest value
  for (int row = 0; row < height_ - 1; row++){
    if (row == 0){
      seam[row] = col; 
    }
    if (col == 0){
      int bottom = values[row+1][col];
      int bottom_right = values[row+1][col+1];
      if (bottom <= bottom_right){
        seam[row+1] = col;
      }
      else {
        col++;
        seam[row+1] = col;
      }
    }
    else if (col == width_ - 1){
      int bottom = values[row+1][col];
      int bottom_left = values[row+1][col-1];
      if (bottom <= bottom_left){
        seam[row+1] = col;
      }
      else {
        col--;
        seam[row+1] = col;
      }
    }
    else {
      int bottom = values[row+1][col];
      int bottom_left = values[row+1][col-1];
      int bottom_right = values[row+1][col+1];
      if (bottom <= bottom_left && bottom <= bottom_right){
        seam[row+1] = col;
      }
      else if (bottom_left <= bottom_right && bottom_left < bottom){
        col--;
        seam[row+1] = col;
      }
      else {
        col++;
        seam[row+1] = col;
      }
    }
  }
  for (int row = 0; row < height_; row++){
    delete[] values[row];
  }
  delete[] values;
  values = nullptr;
  return seam;
}

void SeamCarver::RemoveHorizontalSeam()	{
  int* seam = GetHorizontalSeam();
  Pixel** pixels = new Pixel*[height_ - 1];
  for (int row = 0; row < height_ - 1; row++){
    pixels[row] = new Pixel[width_];
  }
  for (int col = 0; col < width_; col++){
    for (int row = 0; row < height_; row++){
      if (row < seam[col]){
        pixels[row][col] = image_.GetPixel(row,col);
      }
      else if (row > seam[col]){
        pixels[row-1][col] = image_.GetPixel(row,col);
      }
    }
  }
  image_.SetImage(pixels, height_ - 1, width_); 
  height_--;
  //image_.~ImagePPM();  
  delete[] seam;
  seam = nullptr;
}

void SeamCarver::RemoveVerticalSeam()	{
  int* seam = GetVerticalSeam();
  Pixel** pixels = new Pixel*[height_];
  for (int row = 0; row < height_; row++){
    pixels[row] = new Pixel[width_ - 1];
  }
  for (int row = 0; row < height_; row++){
    for (int col = 0; col < width_; col++){ 
      if (col < seam[row]){
        pixels[row][col] = image_.GetPixel(row, col);
      }
      else if (col > seam[row]){   
        pixels[row][col-1] = image_.GetPixel(row, col);
      }
    }
  }
  image_.SetImage(pixels, height_, width_ - 1); 
  width_--;
  //image_.~ImagePPM();  
  delete[] seam;
  seam = nullptr;
}