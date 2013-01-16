

#include "GUIText_box.h"

#include "GameDisplay.h"
#include "GUIWin_Ctrl.h"
#include "GUIView.h"
#include "GUIImage.h"

#include "GUILetter.h"

#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
using std::cout; using std::endl;
using std::string;
using std::clock;
using std::vector;
using std::for_each;

static char SDL_to_a(SDLKey key);

const SDL_Color bg_color_c = {255,255,255,0};
const SDL_Color default_color_c = {0,0,0,0};

GUIText_box::GUIText_box(int w_, int h_)
:GUIView(w_,h_), cursor(this), GUIView_Ctrl(GUIView_shptr_t()),
text_size(13), color(default_color_c)
{
	background = create_SDL_Surface(screen.getw(), screen.geth());
	if (!background) throw Error("Couldn't load background. Not enough mem.");
	
	SDL_PixelFormat *fmt = background->format;
	Uint32 background_color = SDL_MapRGB(fmt, bg_color_c.r, bg_color_c.g, bg_color_c.b);
	SDL_FillRect(background, 0, background_color);
}
GUIText_box::~GUIText_box() {
	
	SDL_FreeSurface(background);
}

void GUIText_box::add_letter(char ltr, int index){
	
	try{
		letters.insert(letters.begin()+index, Letter_Disp_Obj(ltr, text_size, pos_at_index(index), color));
	}
	catch(const Error& e){
		cout << e.msg << endl;
	}
	update();
}
void GUIText_box::remove_letter(int index){
	if (index < 0) return;
	if (index >= letters.size()) return;
	
	letters.erase(letters.begin()+index);
	update();
}

void GUIText_box::update(){
	
	for (int i = 0; i < letters.size(); i++){
		letters[i].set_pos(pos_at_index(i));
	}
	
}
void GUIText_box::clear(){
	letters.clear();
}

void GUIText_box::display() {
	display_image(background, screen, 0, 0, 0);
	
	//	GUIView::display();
	
	cursor.display(screen, text_size);
	for (int i = 0; i < letters.size(); i++){
		letters[i].drawself(screen);
	}
//	for_each(letters.begin(), letters.end(), bind(&Letter_Disp_Obj::drawself, _1, screen));
	
	updateScreen(screen);	
}

void GUIText_box::mouse_click(const SDL_Event& event){
		
	handle_mouse_down(DispPoint(event.button.x, event.button.y));

	// TEMPORARY
//	GUIWin_Ctrl::get()->add_responder(GUIView_Ctrl::shared_from_this());
}
void GUIText_box::key_down(const SDL_Event& event){
	
	handle_key_down(event.key.keysym.sym);
}

void GUIText_box::handle_key_down(SDLKey key){
		
	switch (key) {
		case SDLK_LEFT:
			cursor.move_left();
			break;
		case SDLK_RIGHT:
			cursor.move_right();
			break;
		case SDLK_UP:
			cursor.move_up();
			break;
		case SDLK_DOWN:
			cursor.move_down();
			break;
			
		case SDLK_BACKSPACE:
			remove_letter(cursor.get_index()-1);
			cursor.move_left();
			break;
			
			//		case SDLK_SPACE:
			//			handle_special_char(key);
			//			break;
			
		default:
			handle_alpha_num(SDL_to_a(key));
			break;
	}	
}
void GUIText_box::handle_alpha_num(char ltr){
	
	//	if (!isalnum(ltr)) {
	//		return;
	//	}
	//	if (!ltr) return;
	
	int index = cursor.get_index();
	add_letter(ltr, index);
	
	cursor.move_right();
	
}

void GUIText_box::handle_mouse_down(DispPoint pos_){
	
	cursor.move_to(index_at_pos(pos_));
	
}


static char SDL_to_a(SDLKey key){
	
	if (key >= 0xFF) {
		return 0;
	}
	return static_cast<char>(key);
}


void GUIText_box::Cursor::move_right(){
	if (index == text_box_ptr->get_letters().size()) return;
	move_to(index + 1);
}
void GUIText_box::Cursor::move_left(){
	if (index == 0) return;
	move_to(index - 1);
}
void GUIText_box::Cursor::move_up(){
	
	DispPoint new_pos = position;
	new_pos.y -= Letter_Disp_Obj::get_line_height();
	
	int new_index = text_box_ptr->index_at_pos(new_pos);
	
	move_to(new_index);
}
void GUIText_box::Cursor::move_down(){
	DispPoint new_pos = position;
	new_pos.y += Letter_Disp_Obj::get_line_height();
	
	int new_index = text_box_ptr->index_at_pos(new_pos);
	
	move_to(new_index);
}

void GUIText_box::Cursor::move_to(int index_) {
	
	if (index_ < 0 || index_ > text_box_ptr->get_letters().size()) return;
	
	index = index_;
	
	position = text_box_ptr->pos_at_index(index);
}

void GUIText_box::Cursor::display(SDL_Surface* screen, int text_size) {
	if (std::clock() % 300000 >= 150000){
		flicker = 0;
	}
	else flicker = 1;
	
	SDL_Rect box = {0,0,1,text_size};
	
	GUIImage* image = GUIImage::get_image( string("GUIImages/cursor") + (flicker ? "1" : "0") + string(".bmp"));
	display_image(*image, screen, position.x, position.y, 0, box);
//	ShowBMP("GUIImages/fireball.bmp", screen, position.x, position.y, 0);
}


DispPoint GUIText_box::pos_at_index(size_t i){
	
	DispPoint position;
	if (i == 0){
		position = DispPoint(0,0);
	}
	else {
		const Letter_Disp_Obj& letter = letters[i-1];
		int width = letter.get_width();
		position = DispPoint(letter.get_pos().x + width, 
							 letter.get_pos().y);
		if (position.x + width >= screen.getw()){
			position.x = 0;
//			position.y += Letter_Disp_Obj::get_line_height();
			position.y += text_size;
		}
		
	}
	return position;
}


int GUIText_box::index_at_pos(DispPoint pos_){
	if (letters.empty()){
		return 0;
	}
	if (pos_.x < letters[0].get_pos().x || pos_.y < letters[0].get_pos().y){
		return 0;
	}
	int i;
	for (i = 0; i < letters.size(); i++) {
		if ((pos_.x >= letters[i].get_pos().x 
			 && pos_.x < letters[i].get_pos().x + letters[i].get_width()/2) 
			&& (pos_.y >= letters[i].get_pos().y 
				&& pos_.y < letters[i].get_pos().y + Letter_Disp_Obj::get_line_height())) {
				
				break;
			}		
		
		else if (i > 0 && ((pos_.x >= letters[i-1].get_pos().x + letters[i-1].get_width()/2
							&& pos_.x < letters[i-1].get_pos().x + letters[i-1].get_width()) 
						    && (pos_.y >= letters[i-1].get_pos().y 
						    && pos_.y < letters[i-1].get_pos().y + Letter_Disp_Obj::get_line_height()))) 
		{
							   
			break;
		}		
	}
	return i;
}


string GUIText_box::get_text() const {
	string text;
	for (letters_ctr_t::const_iterator it = letters.begin(); it != letters.end(); ++it){
		text += it->get_ltr();
	}
	return text;	
}



int Letter_Disp_Obj::line_height = 0;

Letter_Disp_Obj::Letter_Disp_Obj(char ltr, int size, DispPoint pos, SDL_Color color)
:position(pos), ltr(ltr)
{
	letter = GUILetter::get_letter(ltr, size, color);
	if (!letter) {
		string error_msg = string("Couldn't load image for letter") + ltr + string(": ")+ SDL_GetError() +"\n";
		cout << error_msg << endl;
		throw Error(error_msg.c_str());
	}
	line_height = letter->get_height()+2; // for a line spacing
}

int Letter_Disp_Obj::get_width() const{
	return letter->get_width();
}

void Letter_Disp_Obj::drawself(SDL_Surface *dest) const{
	
	letter->drawself(dest, position.x, position.y);
}


