/* Ptr_config.h */

#include "Compatibility.h"

namespace GUI {
class View;
}
typedef std::tr1::shared_ptr<GUI::View> GUIView_shptr_t;
typedef std::tr1::weak_ptr<GUI::View> GUIView_wptr_t;


class GUIView_Ctrl;
typedef std::tr1::shared_ptr<GUIView_Ctrl> GUIVC_shptr_t;
typedef std::tr1::weak_ptr<GUIView_Ctrl> GUIVC_wptr_t;

class GUIText_box;
typedef std::tr1::shared_ptr<GUIText_box> GUIText_box_shptr_t;
typedef std::tr1::weak_ptr<GUIText_box> GUIText_box_wptr_t;

namespace GUI {
class Window;
}
typedef std::tr1::shared_ptr<GUI::Window> GUIWindow_shptr_t;
typedef std::tr1::weak_ptr<GUI::Window> GUIWindow_wptr_t;


class GUIValue_Display;
typedef std::tr1::shared_ptr<GUIValue_Display> GUIValue_Disp_shptr_t;
typedef std::tr1::weak_ptr<GUIValue_Display> GUIValue_Disp_wptr_t;

