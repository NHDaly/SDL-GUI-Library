//
//  GUIImageView.h
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef GUIIMAGEVIEW_H
#define GUIIMAGEVIEW_H

#include "GUIView.h"

class GUIImage;

namespace GUI {

class ImageView : public GUI::View {
public:
    
    ImageView(const GUIImage &image_src);    
    
    GUIImage get_image();
    
//private:
//    void attach_subview(GUIView* view, DispPoint pos);
};

} // namespace GUI

#endif /* GUIIMAGEVIEW_H */
