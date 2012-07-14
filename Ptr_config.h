/* Ptr_config.h */

#include "Compatibility.h"


class GUIView;
typedef std::tr1::shared_ptr<GUIView> GUIView_shptr_t;
typedef std::tr1::weak_ptr<GUIView> GUIView_wptr_t;


class GUIView_Ctrl;
typedef std::tr1::shared_ptr<GUIView_Ctrl> GUIVC_shptr_t;
typedef std::tr1::weak_ptr<GUIView_Ctrl> GUIVC_wptr_t;

class GUIText_box;
typedef std::tr1::shared_ptr<GUIText_box> GUIText_box_shptr_t;
typedef std::tr1::weak_ptr<GUIText_box> GUIText_box_wptr_t;


class GUIWindow;
typedef std::tr1::shared_ptr<GUIWindow> GUIWindow_shptr_t;
typedef std::tr1::weak_ptr<GUIWindow> GUIWindow_wptr_t;


