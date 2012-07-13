

#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include "GUIUtility.h"
#include "GUIImage.h"
#include "Ptr_config.h"

#include <set>
#include <vector>
#include <list>

class GUIView;
class SDL_Surface;
class GUIImage;

class GUIWin_Ctrl;

class GUIWindow {
public:
	
	GUIWindow(int w_, int h_, std::string name_ = "GUIWindow");
	virtual ~GUIWindow();
	
	void display_on_screen(const GUIImage& image, DispPoint pos);
	virtual void display();
	virtual void clear();
	
	void attach(GUIView_shptr_t view, DispPoint pos);
	void detach(GUIView_shptr_t view);
	
	void bring_forward(GUIView_shptr_t view);
	void bring_to_front(GUIView_shptr_t view);
	void send_backward(GUIView_shptr_t view);
	void send_to_back(GUIView_shptr_t view);
	
	void move_view_to(GUIView_shptr_t view, DispPoint pos);
	
	SDL_Rect get_dim() { SDL_Rect rect; rect.w = w; rect.h = h; rect.x = 0; rect.y = 0; return rect; }
	
	
	SDL_Surface* screen;
private:
	
	GUIImage background;
	GUIImage foreground;
	
	std::string name;
	int w,h;
	//	SDL_Surface* screen;
	
	GUIView_shptr_t main_view;	// NOTE: switch this in for screen!!!	
	
	struct GUIView_Box {
		GUIView_Box(GUIView_shptr_t view_, DispPoint pos_) : view(view_), pos(pos_) { }
		
		GUIView_shptr_t view;
		DispPoint pos;
		
		int get_h() const;
		int get_w() const;
		
		bool operator< (const GUIView_Box&) const;
		bool operator== (const GUIView_Box&) const;
	};
	typedef std::tr1::shared_ptr<GUIView_Box> GUIView_Box_shptr_t;

	friend bool GUIView_Box_ptr_less (const GUIView_Box_shptr_t a, const GUIView_Box_shptr_t b);
	friend bool GUIView_Box_ptr_equal (const GUIView_Box_shptr_t a, const GUIView_Box_shptr_t b);
	
	typedef std::list<GUIView_Box_shptr_t> views_ctr_t;
	views_ctr_t views;	// a list of all the attached views
	
	typedef std::list<GUIView_Box_shptr_t> view_layers_t;
	typedef std::vector<view_layers_t> view_rows_t;
	typedef std::vector<view_rows_t> view_grid_t;
	
	// a pixel-by-pixel grid of the layer of views, indentifying which view(s) should handle input:
	view_grid_t grid;	
	
	
	GUIView_Box_shptr_t get_view_box(GUIView_shptr_t);
	GUIWindow::views_ctr_t::iterator find_view_box(GUIView_shptr_t);
	
	
	friend class Display_a_GUIView;
	friend class GUIWin_Ctrl;
	
	//	template <typename Function>
	//	void perform_on_grid(Function f, const GUIView_Box& view_box) {
	//		
	//		
	//		for (int i = view_box.pos.x; i < view_box.pos.x + view_box.get_w(); i++) {
	//			for (int j = view_box.pos.y; j < view_box.pos.y + view_box.get_h(); j++) {
	//				
	//				(grid[i][j].*f)(view_box.view);
	//			}
	//		}
	//	}
	
	
};




#endif /* WINDOW_H */
