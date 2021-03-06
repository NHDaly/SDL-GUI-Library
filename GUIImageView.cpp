//
//  GUIImageView.cpp
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#include "GUIImageView.h"

#include "GUIImage.h"
#include "GameDisplay.h"

namespace GUI {

ImageView::ImageView(const GUIImage &image_src)
: View(image_src.getw(), image_src.geth()) 
{ 
    draw_onto_self(image_src, DispPoint(0,0));
}

GUIImage ImageView::get_image() {
        
    return GUIImage::create_copy(get_image_ptr());
}

} // namespace GUI