
#include "GUIWindow.h"
#include "GUIView.h"
#include "GUIImage.h"

#include "GameDisplay.h"

using std::string;
using std::vector;




#include <algorithm> 
#include <iostream> 
using std::swap; using std::for_each;


bool GUIView_Box_ptr_less (const GUIWindow::GUIView_Box_shptr_t a, const GUIWindow::GUIView_Box_shptr_t b);
bool GUIView_Box_ptr_equal (const GUIWindow::GUIView_Box_shptr_t a, const GUIWindow::GUIView_Box_shptr_t b);



const SDL_Color bg_color_c = {330,120,210,0};
const SDL_Color clear_color_c = {0xFF, 0, 0xFF, 0};

GUIWindow::GUIWindow(int w_, int h_, string name_)
:name(name_), w(w_), h(h_), grid(h_, view_rows_t(w_))
{
	const int numColors = 256;
	SDL_Color colors[numColors];
	// Create a display surface with a 16 bit palette 
	screen = createDisplay(screen, w, h, 16, colors, numColors);
	
	if (!screen) throw Error("Couldn't Load Screen");
	
	//Set the window caption 
	SDL_WM_SetCaption( name.c_str(), 0 );
	
	//background/foreground:
	background = GUIImage(w_,h_);
	foreground = GUIImage(w_,h_);
	SDL_PixelFormat *fmt = background->format;
	Uint32 background_color = SDL_MapRGB(fmt, bg_color_c.r, bg_color_c.g, bg_color_c.b);
	SDL_FillRect(background, 0, background_color);
	
	fmt = foreground->format;
	Uint32 clear_color = SDL_MapRGB(fmt, clear_color_c.r, clear_color_c.g, clear_color_c.b);
	SDL_FillRect(foreground, 0, clear_color);
	
}

GUIWindow::~GUIWindow()
{
	std::cout << "destroying GUIWindow" << std::endl;

	clear();
}


void GUIWindow::display_on_screen(const GUIImage& image, DispPoint pos){
	//	foreground.clear();
	display_image(image, foreground, pos.x, pos.y, 0);
}



class Display_a_GUIView {
public:
	Display_a_GUIView(GUIWindow* window_) : window(window_) { }
	void operator()(GUIWindow::GUIView_Box_shptr_t view_box)  {
		view_box->view->display();
		display_image(view_box->view->screen, window->screen, view_box->pos.x, view_box->pos.y, 0);
		view_box->view->draw_bg();
	}
private:
	GUIWindow* window;
};
void GUIWindow::display() {
	// display from back to front
	display_image(background, screen, 0, 0, 0);
	for_each(views.rbegin(), views.rend(), Display_a_GUIView(this));
	//	display_image(foreground, screen, 0, 0, 0);
	updateScreen(screen);
}
void GUIWindow::clear(){
	
	views.clear();
	
//	for (int i = 0; i < grid.size(); i++) {
//		for (int j = 0; j < grid[i].size(); j++) {
//			for (int k = 0; k < grid[i][j].size(); k++) {
//				detach(grid[i][j].front()->view);
//			}
//		}
//	}
//	grid = view_grid_t(h, view_rows_t(w));
	
	
}


void GUIWindow::attach(GUIView_shptr_t view, DispPoint pos){
	
	GUIView_Box_shptr_t view_box(new GUIView_Box(view, pos));
	
	views.push_front(view_box);
	
//	perform_on_grid(&view_layers_t::push_front, view);
	
	for (int row = pos.y; row < pos.y + view->get_h(); row++) {
		for (int col = pos.x; col < pos.x + view->get_w(); col++) {
			
			if (row >= grid.size()) continue;
			if (col >= grid[row].size()) continue;
			
			grid[row][col].push_front(view_box);
		}
	}
}
void GUIWindow::detach(GUIView_shptr_t view){
	
	views_ctr_t::iterator view_it = find_view_box(view);	
	if (view_it == views.end()) return;
	
	GUIView_Box_shptr_t to_detach = *view_it;
	DispPoint pos = to_detach->pos;
	views.erase(view_it);	// erase it
	
	//	perform_on_grid(&view_layers_t::remove, *view_it);
	
	// remove it from the grid
	for (int row = pos.y; row < pos.y + view->get_h(); row++) {
		for (int col = pos.x; col < pos.x + view->get_w(); col++) {
			
			if (row >= grid.size()) continue;
			if (col >= grid[row].size()) continue;
			
			grid[row][col].remove(to_detach);
		}
	}
	
}

void GUIWindow::bring_forward(GUIView_shptr_t view){
	
	GUIView_Box_shptr_t view_box = get_view_box(view);
	
	DispPoint pos =  view_box->pos;
	
	// remove it from the grid
	for (int row = pos.y; row < pos.y + view->get_h(); row++) {
		for (int col = pos.x; col < pos.x + view->get_w(); col++) {
			
			if (row >= grid.size()) continue;
			if (col >= grid[row].size()) continue;
			
			GUIView_Box_shptr_t dummy(new GUIView_Box(view, DispPoint(0,0)));
			view_layers_t::iterator layer_it = find(grid[row][col].begin(), grid[row][col].end(), dummy);
			if (layer_it == grid[row][col].end())
				continue;
			
			if (grid[row][col].begin() != layer_it) {
				view_layers_t::iterator prev = layer_it--;
				swap(*layer_it, *prev);
			}
		}
	}
}
GUIWindow::GUIView_Box_shptr_t GUIWindow::get_view_box(GUIView_shptr_t view){
	
	views_ctr_t::iterator view_it = find_view_box(view);
	if (view_it == views.end()) return GUIView_Box_shptr_t();
	return *view_it;
}
GUIWindow::views_ctr_t::iterator GUIWindow::find_view_box(GUIView_shptr_t view){
	
	GUIView_Box_shptr_t dummy(new GUIView_Box(view, DispPoint(0,0)));
	views_ctr_t::iterator view_it = find_if(views.begin(), views.end(), bind(GUIView_Box_ptr_equal, dummy, _1));
	return view_it;
}
void GUIWindow::bring_to_front(GUIView_shptr_t view){
	
}
void GUIWindow::send_backward(GUIView_shptr_t view){
	
}
void GUIWindow::send_to_back(GUIView_shptr_t view){
	
}

void GUIWindow::move_view_to(GUIView_shptr_t view, DispPoint pos) {
	
	detach(view);
	attach(view,pos);
	
}



int GUIWindow::GUIView_Box::get_h() const {
	return view->get_h(); 
}
int GUIWindow::GUIView_Box::get_w() const {
	return view->get_w(); 
}
bool GUIWindow::GUIView_Box::operator< (const GUIView_Box& vb) const{
	return view < vb.view;
}
bool GUIWindow::GUIView_Box::operator== (const GUIView_Box& vb) const{
	return view == vb.view;
}

bool GUIView_Box_ptr_less (const GUIWindow::GUIView_Box_shptr_t a, const GUIWindow::GUIView_Box_shptr_t b) {
	return *a < *b;
}
bool GUIView_Box_ptr_equal (const GUIWindow::GUIView_Box_shptr_t a, const GUIWindow::GUIView_Box_shptr_t b) {
	return *a == *b;
}





