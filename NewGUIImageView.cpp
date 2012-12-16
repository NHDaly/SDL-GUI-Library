//
//  NewGUIImageView.cpp
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "NewGUIImageView.h"

#include "GUIImage.h"

NewGUIImageView::NewGUIImageView(const GUIImage &image_src)
: NewGUIView(image_src.getw(), image_src.geth()) 
{ 
    draw_onto_self(image_src, DispPoint(0,0));
}

