



#ifndef GUI_TEXT_BOX_H
#define GUI_TEXT_BOX_H


#include "GUIView.h"
#include "GUIUtility.h"
#include "Ptr_config.h"

#include <list>


class Letter_Disp_Obj;

class GUILetter;
class GUIView;
class SDL_Color;

class GUIText_box : public GUIView, public GUIView_Ctrl {
public:	
	
	GUIText_box(int w_ = 200, int h_ = 200);
	~GUIText_box();
	
	void add_letter(char ltr, int index);
	void remove_letter(int index);
	
	void add_to_view(char ltr);
	virtual void clear();

	virtual void update();
	virtual void display();
	
	virtual void mouse_click(const SDL_Event& event);
	virtual void key_down(const SDL_Event& event);
	
	std::string get_text();
	
protected:
	typedef std::vector<Letter_Disp_Obj> letters_ctr_t;
	
	const letters_ctr_t& get_letters() const
	{return letters; }
	
	int index_at_pos(DispPoint pos_);
	DispPoint pos_at_index(size_t i);
	
private:
	
	letters_ctr_t letters;
	
	void handle_key_down(SDLKey key);
	void handle_alpha_num(char ltr);
	char handle_special_char(char ltr);
	void handle_mouse_down(DispPoint pos_);
	
	int text_size;
	SDL_Color color;
	
	class Cursor {
	public:
		Cursor(GUIText_box* tb_ptr) : position(0,0), index(0), text_box_ptr(tb_ptr), flicker(true) { }
		
		void display(SDL_Surface* screen, int text_size);
		
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
		GUIText_box* text_box_ptr;
		
		bool flicker;
	} cursor;
	
	
	SDL_Surface *background;
};

class Letter_Disp_Obj{
public:
	Letter_Disp_Obj(char ltr, int size, DispPoint pos, SDL_Color color);
	
	void drawself(SDL_Surface *dest) const;
	int get_width() const;
	DispPoint get_pos() const
		{ return position; }
	void set_pos(DispPoint point)
		{ position = point; }
	
	static int get_line_height(){
		return line_height;
	}
	
	char get_ltr() { return ltr; }
private:
	DispPoint position;
	GUILetter* letter;
	static int line_height;
	char ltr;
};




#endif /* TEXT_BOX_H */


