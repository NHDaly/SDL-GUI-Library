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
class GUIImageView : public GUI::View {
public:
    
    GUIImageView(const GUIImage &image_src);    
    
    GUIImage get_image();
    
//private:
//    void attach_subview(GUIView* view, DispPoint pos);
};


#endif /* GUIIMAGEVIEW_H */
