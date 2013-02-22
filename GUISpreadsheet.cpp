//
//  GUISpreadSheet.cpp
//  GUI_Showcase
//
//  Created by Nathan Daly on 2/19/13.
//  Copyright (c) 2013 Lions Entertainment. All rights reserved.
//

#include "GUISpreadsheet.h"
#include <iostream>
#include <algorithm>

template <typename T>
class Matrix2D {
public:
    
    Matrix2D(int rows_, int cols_) 
    :rows(rows_), cols(cols_)
    {
        T *data = new T[rows*cols];
        arr = new T*[rows];
        for (int i = 0; i < rows; ++i) {
            arr[i] = &data[i*cols];
        }
    }
    Matrix2D(const Matrix2D &rhs) 
    :rows(rhs.rows_), cols(rhs.cols_)
    {
        T *data = new T[rows*cols];
        arr = new T*[rows];
        for (int i = 0; i < rows; ++i) {
            arr[i] = &data[i*cols];
            T *rhs_data = rhs[i];
            for (int j = 0; j < cols; ++j) {
                data[i+j] = rhs_data[j];
            }
        }
    }
    ~Matrix2D() {
        delete [] arr[0];
        delete [] arr;
    }
    
    Matrix2D& operator=(const Matrix2D &rhs) 
    {
        Matrix2D temp(rhs);
        std::swap(arr, rhs.arr);
        rows = rhs.rows;
        cols = rhs.cols;
    }

    
    T& operator()(int i, int j) {
        if (i >= rows || j >= cols) {
            throw GUIError("out of bounds Matrix access.");
        }
        return arr[i][j];
    }
    
    void grow(int rows_, int cols_) {
        if (rows_ == rows && cols_ == cols) {
            return;
        }
        if (rows_ < rows || cols_ < cols) {
            throw GUIError("new size smaller than current size.");
        }
        
        Matrix2D temp(rows_, cols_);
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                temp(i,j) = arr[i][j];
            }
        }

        *this = temp;
    }
    
private:
    T **arr;
    int rows, cols;
};


class GUICell;
class GUICellsView : public GUIView {
public:
    GUICellsView(int w_, int h_);
    
private:
    Matrix2D<GUICell*> cells;
};

GUICellsView::GUICellsView(int w_, int h_)
:GUIView(w_,h_), cells(1,1)
{
    
}
