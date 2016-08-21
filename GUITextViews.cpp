

#include "GUITextViews.h"
#include "GUIImageView.h"
#include "GUIApp.h"

#include "GameDisplay.h"
#include "GUIView.h"
#include "GUIImage.h"

#include "GUILetter.h"

#include <iostream>
#include <ctime>
#include <vector>
#include <cctype>
#include <algorithm>
#include <functional>
using std::cout; using std::endl;
using std::string;
using std::clock;
using std::vector;
using std::for_each;
using std::min;
using std::bind;
using namespace std::placeholders;


static char SDL_to_a(SDLKey key);
static char symbol_to_upper(char ltr);

const SDL_Color bg_color_c = {255,0,255,0};
const SDL_Color default_color_c = {0,0,0,0};


GUITextView::GUITextView(int w_, int h_,
                               bool resizeable_down_, bool resizeable_right_)
:GUIView(w_,h_), resizeable_down(resizeable_down_), resizeable_right(resizeable_right_), 
w_init(w_), h_init(h_), text_size(30), color(default_color_c)
{
    set_clear_color(bg_color_c);
            
    update();
    
}

void GUITextView::update(){
//    resize(200, get_h());
    
    if (!letters.empty()) {
//        resize(200, get_h());

//        const NewLetter_Disp_Obj &last_letter = letters[letters.size()-1];
//        if (resizeable_right && last_letter.get_pos().x + last_letter.get_width() > get_w()) {
//            resize(200, get_h());
//        }
    }
    
	for (size_t i = 0; i < letters.size(); i++){
		letters[i].set_pos(pos_at_index(i));
	}
    
    if (!letters.empty()) {
        const NewLetter_Disp_Obj &last_letter = letters[letters.size()-1];
        if (resizeable_right && last_letter.get_pos().x + last_letter.get_width() > get_w()) {
            resize(300, get_h());
            
//            resize(last_letter.get_pos().x + last_letter.get_width(), get_h());
//            return update();    // start over!
        }
        if (resizeable_down && last_letter.get_pos().y >= get_h()) {
            const NewLetter_Disp_Obj &last_letter = letters[letters.size()-1];
            resize(get_w(), last_letter.get_pos().y + last_letter.get_line_height());
            return update();    // start over!
        }
    }
    
    fill_with_color(bg_color_c);
    
    for (size_t i = 0; i < letters.size(); i++){
        letters[i].drawself(this);
	}
    
	mark_changed();
}
void GUITextView::clear_text(){
	letters.clear();
    update();
}

void GUITextView::set_text(const string& text) {
    clear_text();
    
    append_text(text);
}
void GUITextView::append_text(const string& text) {
    
    for (int i = 0; i < (int)text.length(); ++i) {
        add_letter_no_redraw(text[i], i);
    };
    update();
}
string GUITextView::get_text() const {
	string text;
	for (letters_ctr_t::const_iterator it = letters.begin(); it != letters.end(); ++it){
		text += it->get_ltr();
	}
	return text;	
}

void GUITextView::set_text_size(int size) {
    text_size = size;
    set_text(get_text());
}
void GUITextView::set_text_color(SDL_Color color_) {
    color = color_;
    set_text(get_text());
}
int GUITextView::get_text_size() {
    return text_size;
}
SDL_Color GUITextView::get_text_color() {
    return color;
}

//void GUITextView::did_resize(int w, int h)
//{
////    if (GUITextField* tb = dynamic_cast<GUITextField*>(get_parent())) {
////        tb->resize(w,h);
////    }
//}



void GUITextView::add_letter(char ltr, int index){
    	
	add_letter_no_redraw(ltr, index);
	update();
}
void GUITextView::remove_letter(int index){

    remove_letter_no_redraw(index);
	update();
}

void GUITextView::add_letter_no_redraw(char ltr, int index){
    
	try{
		letters.insert(letters.begin()+index, NewLetter_Disp_Obj(ltr, text_size, pos_at_index(index), color));
        //        cout << "letter: '" << letters[index].get_ltr() << "'\n";
        //        cout << "width: " << letters[index].get_width() << "\n";
	}
	catch(const GUIError& e){
		cout << e.msg << endl;
	}
}
void GUITextView::remove_letter_no_redraw(int index){
    
	if (index < 0) return;
	if (index >= (int)letters.size()) return;
	
	letters.erase(letters.begin()+index);
}



DispPoint GUITextView::pos_at_index(size_t i){
	
	DispPoint position;
	if (i == 0){
		position = DispPoint(3,0);
	}
	else {
//        if (i >= letters.size()) throw GUIError("GUITextView: Couldn't get position -- index out of range");
        
		const NewLetter_Disp_Obj& letter = letters[i-1];
		int width = letter.get_width();
		position = DispPoint(letter.get_pos().x + width + 1, 
							 letter.get_pos().y);
		
        if (!resizeable_right && position.x + width >= get_w()){
			position.x = 0;
            //			position.y += NewLetter_Disp_Obj::get_line_height();
			position.y += text_size;
		}		
	}
	return position;
}
int GUITextView::index_at_pos(DispPoint pos_){
	if (letters.empty()){
		return 0;
	}
	if (pos_.x < letters[0].get_pos().x || pos_.y < letters[0].get_pos().y){
		return 0;
	}
	int i;
	for (i = 0; i < (int)letters.size(); i++) {
		if ((pos_.x >= letters[i].get_pos().x 
			 && pos_.x < letters[i].get_pos().x + letters[i].get_width()/2) 
			&& (pos_.y >= letters[i].get_pos().y 
				&& pos_.y < letters[i].get_pos().y + NewLetter_Disp_Obj::get_line_height())) {
				
				break;
			}		
		
		else if (i > 0 && ((pos_.x >= letters[i-1].get_pos().x + letters[i-1].get_width()/2
							&& pos_.x <= letters[i-1].get_pos().x + letters[i-1].get_width()) 
                           && (pos_.y >= letters[i-1].get_pos().y 
                               && pos_.y <= letters[i-1].get_pos().y + NewLetter_Disp_Obj::get_line_height()))) 
		{
            
			break;
		}		
	}
	return i;
}



GUITextField::GUITextField(int w_, int h_,
                             bool resizeable_down_, bool resizeable_right_)
:GUITextView(w_,h_, resizeable_down_, resizeable_right_),
key_is_held(false), modifiers_held(KMOD_NONE), cursor(new Cursor(this)), flicker(false)
{        
    attach_subview(cursor, DispPoint(-1,0)); // start it out off screen.
    
    GUIApp::get()->repeat_on_timer(bind(&GUITextField::handle_key_held, this), 0.05);
//    GUIApp::get()->repeat_on_timer(bind(&GUITextField::blink_cursor, this), 0.5);

}

void GUITextField::blink_cursor() {
    cursor->display(get_text_size());
    if (flicker) {
        attach_subview(cursor, cursor->get_rel_pos());
    }
    else {
        remove_subview(cursor);
    }
    flicker = !flicker;
}

#include "GUIApp.h"
bool GUITextField::handle_key_down(SDL_keysym key_in) {
    
    cout << "text box key down: '" << key_in.sym << "'" << endl;
    
    key_is_held = true;
    key_held = key_in.sym;
    time_key_held.reset();

    handle_modifier(SDL_Keymod(key_in.mod));
    handle_key();

    return true;
}

bool GUITextField::handle_key_up(SDL_keysym key_in) {
    
    cout << "text box key up" << endl;

    key_is_held = false;
    time_key_held.stop();
    modifiers_held = KMOD_NONE;
    
    return false;
}
void GUITextField::handle_key_held() {
    
    if (!key_is_held) return;
    if (!(time_key_held.get_time() > 500)) return; 
    
    handle_key();
}

void GUITextField::handle_key() {
 	
    SDLKey key = key_held;

    switch (key) {
		case SDLK_LEFT:
			cursor->move_left();
			break;
		case SDLK_RIGHT:
			cursor->move_right();
			break;
		case SDLK_UP:
			cursor->move_up();
			break;
		case SDLK_DOWN:
			cursor->move_down();
			break;
			
        case SDLK_BACKSPACE:
			remove_letter(cursor->get_index()-1);
			cursor->move_left();
			break;
        case SDLK_DELETE:
            remove_letter(cursor->get_index());
			break;

		default:
			handle_alpha_num(SDL_to_a(key));
			break;
	}
}



void GUITextField::handle_alpha_num(char ltr){
    
    if (!isprint(ltr)) return;

	int index = cursor->get_index();
	
    if (modifiers_held & KMOD_SHIFT) {
        if (isalpha(ltr)) ltr = toupper(ltr);
        else ltr = symbol_to_upper(ltr);
    }
    else if (modifiers_held & KMOD_CAPS) {
        if (isalpha(ltr)) ltr = toupper(ltr);
    }
    
    add_letter(ltr, index);
	
	cursor->move_right();
}

void GUITextField::handle_modifier(SDLMod mod){
    
    modifiers_held = mod;

}


bool GUITextField::handle_mouse_down(DispPoint pos_){
	
    if (!rel_point_is_on_me(pos_)) {
        lose_focus();
       
        return false;
    }
    
	cursor->move_to(index_at_pos(pos_));

    capture_focus();
    
    return true;
}

#include <cctype>

static char SDL_to_a(SDLKey key){
	
	if (key >= 0xFF) {
		return 0;
	}
	return static_cast<char>(key);
}

void GUITextField::lost_focus() {
    cout << "LOSTTTTT FOCUSSSSS" << endl;
    move_subview(cursor, DispPoint(-100,-100));
    update();
}

const SDL_Color cursor_clear_c = {0, 0xff, 0};

GUITextField::Cursor::Cursor(GUITextField* tb_ptr) 
: GUIView(1,1), position(0,0), index(0),
text_box_ptr(tb_ptr), flicker(true)
{ 

    const SDL_Color black = {0,0,0};
    fill_with_color(black);

    set_clear_color(cursor_clear_c);
    GUIApp::get()->repeat_on_timer(bind(&Cursor::display, this,
                                        bind(&GUITextField::get_text_size, text_box_ptr)), 0.5);
}

void GUITextField::Cursor::move_right(){
	if (index == (int)text_box_ptr->get_letters().size()) return;
	move_to(index + 1);
}
void GUITextField::Cursor::move_left(){
	if (index == 0) return;
	move_to(index - 1);
}
void GUITextField::Cursor::move_up(){
	
	DispPoint new_pos = position;
	new_pos.y -= NewLetter_Disp_Obj::get_line_height();
	
	int new_index = text_box_ptr->index_at_pos(new_pos);
	
	move_to(new_index);
}
void GUITextField::Cursor::move_down(){
	DispPoint new_pos = position;
	new_pos.y += NewLetter_Disp_Obj::get_line_height();
	
	int new_index = text_box_ptr->index_at_pos(new_pos);
	
	move_to(new_index);
}

void GUITextField::Cursor::move_to(int index_) {
	
	if (index_ < 0 || index_ > (int)text_box_ptr->get_letters().size()) return;
	
	index = index_;
	
	position = text_box_ptr->pos_at_index(index);

    text_box_ptr->move_subview(this, position);}

void GUITextField::Cursor::display(int text_size) {
    flicker = !flicker;
    
    const SDL_Color colors[2] = {{0,0,0}, cursor_clear_c};

    resize(1, text_size+2);

    SDL_Color color;
//    if (flicker) { 
        color = colors[0];
//    }
//    else {
//        color = colors[1];
//    }
    fill_with_color(color); // bool will be 0 or 1 index

//    if (!flicker) {
//        text_box_ptr->remove_subview(this);
//    }
//    if (!flicker) {
//        text_box_ptr->attach_subview(this, get_rel_pos());
//    }
}


GUITextBox::GUITextBox(int w_, int h_)
:GUIView(w_,h_), field(new GUITextField(w_-4,h_-4))
{
    const SDL_Color clear = {0xff, 0, 0xff};
    fill_with_color(clear);
    set_clear_color(clear);
    
    const SDL_Color field_bg_color = {0xff,0xff,0xff};

    fill_with_color(field_bg_color);

    attach_subview(field, DispPoint(2,2));
   
    draw_onto_self(GUIImage("GUIImages/corner0.bmp"), DispPoint(0,    0));
    draw_onto_self(GUIImage("GUIImages/corner1.bmp"), DispPoint(w_-5, 0));
    draw_onto_self(GUIImage("GUIImages/corner2.bmp"), DispPoint(w_-5, h_-5));
    draw_onto_self(GUIImage("GUIImages/corner3.bmp"), DispPoint(0,    h_-5));

    // Should ideally use a mask layer instead so that bg color could be changed,
    // but right now the problem is it receives the mouse clicks instead of field.
//    const SDL_Color mask_clear = {0, 0xff, 0xff};
//    GUIView *mask = new GUIView(w_,h_);
//    mask->fill_with_color(mask_clear);
//    mask->set_clear_color(mask_clear);
//    
//    mask->draw_onto_self(GUIImage("GUIImages/corner0.bmp"), DispPoint(0,    0));
//    mask->draw_onto_self(GUIImage("GUIImages/corner1.bmp"), DispPoint(w_-5, 0));
//    mask->draw_onto_self(GUIImage("GUIImages/corner2.bmp"), DispPoint(w_-5, h_-5));
//    mask->draw_onto_self(GUIImage("GUIImages/corner3.bmp"), DispPoint(0,    h_-5));
// 
//    attach_subview(mask, DispPoint(0,0));

}






int NewLetter_Disp_Obj::line_height = 0;


NewLetter_Disp_Obj::NewLetter_Disp_Obj(char ltr, int size, DispPoint pos, SDL_Color color)
:position(pos), ltr(ltr), height(size)
{
	letter = GUILetter::get_letter(ltr, size, color);
	if (!letter) {
		string error_msg = string("Couldn't load image for letter") 
                            + ltr + string(": ")+ SDL_GetError() +"\n";
		throw GUIError(error_msg.c_str());
	}
	line_height = letter->get_height()+2; // for a line spacing
}

int NewLetter_Disp_Obj::get_width() const{
	return letter->get_width();
}

void NewLetter_Disp_Obj::drawself(GUIView *dest) const{
	
	letter->drawself(dest, position.x, position.y); //+ height-letter->get_height());
}



static char symbol_to_upper(char ltr) {
    switch (ltr) {
        case '`':
            return '~';
        case '1':
            return '!';
        case '2':
            return '@';
        case '3':
            return '#';
        case '4':
            return '$';
        case '5':
            return '%';
        case '6':
            return '^';
        case '7':
            return '&';
        case '8':
            return '*';
        case '9':
            return '(';
        case '0':
            return ')';
        case '-':
            return '_';
        case '=':
            return '+';
        case '[':
            return '{';
        case ']':
            return '}';
        case '\\':
            return '|';
        case ';':
            return ':';
        case '\'':
            return '"';
        case ',':
            return '<';
        case '.':
            return '>';
        case '/':
            return '?';
          
        default:
            return ltr;
    }
}
