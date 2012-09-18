//
//  NewGUIImageView.h
//  Deep
//
//  Created by Nathan Daly on 9/18/12.
//  Copyright (c) 2012 Lions Entertainment. All rights reserved.
//

#ifndef NEWGUIIMAGEVIEW_H
#define NEWGUIIMAGEVIEW_H

#include "NewGUIView.h"

class GUIImage;
class NewGUIImageView : public NewGUIView {
public:
    
    NewGUIImageView(const GUIImage &image_src);    
};


#endif /* NEWGUIIMAGEVIEW_H */
