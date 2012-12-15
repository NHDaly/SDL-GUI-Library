



#ifndef NEW_GUI_TEXT_BOX_H
#define NEW_GUI_TEXT_BOX_H


#include "NewGUIView.h"
#include "NewGUIImageView.h"
#include "GUIUtility.h"
#include "Ptr_config.h"
#include "GUITimer.h"

#include <list>
#include <vector>


class NewLetter_Disp_Obj;

class GUILetter;
class SDL_Color;

class NewGUITextBox : public NewGUIView {
public:	
	
	NewGUITextBox(int w_ = 200, int h_ = 200);
	virtual ~NewGUITextBox();
	
	virtual void add_letter(char ltr, int index);
	virtual void remove_letter(int index);
	
	virtual void clear();
    
	virtual void update();

	std::string get_text() const;
	
protected:
	typedef std::vector<NewLetter_Disp_Obj> letters_ctr_t;
	
	const letters_ctr_t& get_letters() const
	{ return letters; }
	
	int index_at_pos(DispPoint pos_);
	DispPoint pos_at_index(size_t i);
    
//    virtual void got_focus();
    virtual void lost_focus();
	
private:
	
	letters_ctr_t letters;
	
	virtual bool handle_mouse_down(DispPoint pos_);
    virtual bool handle_key_down(SDL_keysym key);
    virtual bool handle_key_up(SDL_keysym key);
	
    bool key_is_held;
    SDLKey key_held;
    GUITimer time_key_held; 
    
    void handle_key_held();
    void handle_key();
    
    void handle_alpha_num(char ltr);

	int text_size;
	SDL_Color color;
	
	class Cursor : public NewGUIImageView {
	public:
		Cursor(NewGUITextBox* tb_ptr) : NewGUIImageView(GUIImage("images/cursor1.bmp")), position(0,0), index(0), text_box_ptr(tb_ptr), flicker(true) { }
		
		void display(int text_size);
		
		void move_right();
		void move_left();
		void move_up();
		void move_down();
		void move_to(int index_);
		
		int get_index() const
		{ return index; }
		DispPoint get_pos() const
		{ return position; }
		
		
	private:
		DispPoint position;
		int index; 
		NewGUITextBox* text_box_ptr;
		
		bool flicker;
	} cursor;
	
	
	SDL_Surface *background;
};


class NewLetter_Disp_Obj{
public:
	NewLetter_Disp_Obj(char ltr, int size, DispPoint pos, SDL_Color color);
	
	void drawself(NewGUIView *dest) const;
	int get_width() const;
	DispPoint get_pos() const
    { return position; }
	void set_pos(DispPoint point)
    { position = point; }
	
	static int get_line_height() {
		return line_height;
	}
	
	char get_ltr() const { return ltr; }
private:
	DispPoint position;
	GUILetter* letter;
	char ltr;
    int height;
    
    static int line_height;

};

#endif /* NEW_GUI_TEXT_BOX_H */


