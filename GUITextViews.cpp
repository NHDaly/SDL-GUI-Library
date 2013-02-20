

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
#include <tr1/functional>
using std::cout; using std::endl;
using std::string;
using std::clock;
using std::vector;
using std::for_each;
using std::tr1::bind;
using namespace std::tr1::placeholders;


static char SDL_to_a(SDLKey key);
static char symbol_to_upper(char ltr);

const SDL_Color bg_color_c = {255,255,255,0};
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
    
	for (int i = 0; i < letters.size(); i++){
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
    
    GUIImage bg = GUIImage::create_blank(get_w(), get_h());
    SDL_FillRect(bg, 0, SDL_MapRGB(bg->format, bg_color_c.r, bg_color_c.g, bg_color_c.b));
    draw_onto_self(bg, DispPoint());
    
    for (int i = 0; i < letters.size(); i++){
        letters[i].drawself(this);
	}
    
	mark_changed();
}
void GUITextView::clear(){
	letters.clear();
}



void GUITextView::set_text(const string& text) {
    clear();
    
    for (int i = 0; i < text.length(); ++i) {
        add_letter(text[i], i);
    };
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
void GUITextView::did_resize(int w, int h)
{
//    if (GUITextBox* tb = dynamic_cast<GUITextBox*>(get_parent())) {
//        tb->resize(w,h);
//    }
}



void GUITextView::add_letter(char ltr, int index){
    
    cout << "ADDING LETTER: '" << ltr << "'" << endl;
	
	try{
		letters.insert(letters.begin()+index, NewLetter_Disp_Obj(ltr, text_size, pos_at_index(index), color));
        cout << "letter: '" << letters[index].get_ltr() << "'\n";
        cout << "width: " << letters[index].get_width() << "\n";
	}
	catch(const GUIError& e){
		cout << e.msg << endl;
	}
	update();
}
void GUITextView::remove_letter(int index){
    cout << "REMOVVVVEEEEE LETTTERRRRR: " << letters.size() << endl;
    cout << "index: " << index << " SIZE: " << letters.size() << endl;

	if (index < 0) return;
	if (index >= letters.size()) return;
	
	letters.erase(letters.begin()+index);
	update();
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
	for (i = 0; i < letters.size(); i++) {
		if ((pos_.x >= letters[i].get_pos().x 
			 && pos_.x < letters[i].get_pos().x + letters[i].get_width()/2) 
			&& (pos_.y >= letters[i].get_pos().y 
				&& pos_.y < letters[i].get_pos().y + NewLetter_Disp_Obj::get_line_height())) {
				
				break;
			}		
		
		else if (i > 0 && ((pos_.x >= letters[i-1].get_pos().x + letters[i-1].get_width()/2
							&& pos_.x < letters[i-1].get_pos().x + letters[i-1].get_width()) 
                           && (pos_.y >= letters[i-1].get_pos().y 
                               && pos_.y < letters[i-1].get_pos().y + NewLetter_Disp_Obj::get_line_height()))) 
		{
            
			break;
		}		
	}
	return i;
}



GUITextBox::GUITextBox(int w_, int h_,
                             bool resizeable_down_, bool resizeable_right_)
:GUITextView(w_,h_, resizeable_down_, resizeable_right_), 
cursor(this), key_is_held(false), modifiers_held(KMOD_NONE)
{        
    attach_subview(&cursor, DispPoint());
    
    GUIApp::get()->repeat_on_timer(bind(&GUITextBox::handle_key_held, this), 0.05);
}

#include "GUIApp.h"
bool GUITextBox::handle_key_down(SDL_keysym key_in) {
    
    cout << "text box key down: '" << key_in.sym << "'" << endl;
    
    key_is_held = true;
    key_held = key_in.sym;
    time_key_held.reset();

    handle_modifier(key_in.mod);
    handle_key();

    return true;
}

bool GUITextBox::handle_key_up(SDL_keysym key_in) {
    
    cout << "text box key up" << endl;

    key_is_held = false;
    time_key_held.stop();
    modifiers_held = KMOD_NONE;
    
    return false;
}
void GUITextBox::handle_key_held() {
    
    if (!key_is_held) return;
    if (!(time_key_held.get_time() > 500)) return; 
    
    handle_key();
}

void GUITextBox::handle_key() {
 	
    SDLKey key = key_held;

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
        case SDLK_DELETE:
            remove_letter(cursor.get_index());
			break;

		default:
			handle_alpha_num(SDL_to_a(key));
			break;
	}
}



void GUITextBox::handle_alpha_num(char ltr){
    
    if (!isprint(ltr)) return;

	int index = cursor.get_index();
	
    if (modifiers_held & KMOD_SHIFT) {
        if (isalpha(ltr)) ltr = toupper(ltr);
        else ltr = symbol_to_upper(ltr);
    }
    
    add_letter(ltr, index);
	
	cursor.move_right();
}

void GUITextBox::handle_modifier(SDLMod mod){
    
    modifiers_held = mod;

}


bool GUITextBox::handle_mouse_down(DispPoint pos_){
	
    if (!rel_point_is_on_me(pos_)) {
        lose_focus();
       
        return false;
    }
    
	cursor.move_to(index_at_pos(pos_));

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

void GUITextBox::lost_focus() {
    cout << "LOSTTTTT FOCUSSSSS" << endl;
    move_subview(&cursor, DispPoint(-100,-100));
    update();
}

void GUITextBox::Cursor::move_right(){
	if (index == text_box_ptr->get_letters().size()) return;
	move_to(index + 1);
}
void GUITextBox::Cursor::move_left(){
	if (index == 0) return;
	move_to(index - 1);
}
void GUITextBox::Cursor::move_up(){
	
	DispPoint new_pos = position;
	new_pos.y -= NewLetter_Disp_Obj::get_line_height();
	
	int new_index = text_box_ptr->index_at_pos(new_pos);
	
	move_to(new_index);
}
void GUITextBox::Cursor::move_down(){
	DispPoint new_pos = position;
	new_pos.y += NewLetter_Disp_Obj::get_line_height();
	
	int new_index = text_box_ptr->index_at_pos(new_pos);
	
	move_to(new_index);
}

void GUITextBox::Cursor::move_to(int index_) {
	
	if (index_ < 0 || index_ > text_box_ptr->get_letters().size()) return;
	
	index = index_;
	
	position = text_box_ptr->pos_at_index(index);

    text_box_ptr->move_subview(this, position);
}

void GUITextBox::Cursor::display(int text_size) {
	if (std::clock() % 300000 >= 150000){
		flicker = 0;
	}
	else flicker = 1;
	
//	SDL_Rect box = {0,0,1,text_size};
	
	GUIImage* image = GUIImage::get_image( string("GUIImages/cursor") + (flicker ? "1" : "0") + string(".bmp"));
	draw_onto_self(*image, position);
    //	ShowBMP("GUIImages/fireball.bmp", screen, position.x, position.y, 0);
}






int NewLetter_Disp_Obj::line_height = 0;


NewLetter_Disp_Obj::NewLetter_Disp_Obj(char ltr, int size, DispPoint pos, SDL_Color color)
:position(pos), ltr(ltr), height(size)
{
	letter = GUILetter::get_letter(ltr, size, color);
	if (!letter) {
		string error_msg = string("Couldn't load image for letter") + ltr + string(": ")+ SDL_GetError() +"\n";
		cout << error_msg << endl;
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
            throw "GUITextBox: Symbol To Upper: invalid symbol";
            break;
    }
}
