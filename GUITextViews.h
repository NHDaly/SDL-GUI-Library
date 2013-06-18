



#ifndef GUI_TEXT_VIEWS_H
#define GUI_TEXT_VIEWS_H


#include "GUIView.h"
#include "GUIImageView.h"
#include "GUIUtility.h"
#include "Ptr_config.h"
#include "GUITimer.h"

#include <list>
#include <vector>
#include <map>

class GUILetter;

namespace GUI {
    
class NewLetter_Disp_Obj;
    
// A View that displays text atop a clear background.
class TextView : public GUI::View {
public:	

	TextView(int w_ = 200, int h_ = 200, 
                   bool resizeable_down = false, bool resizable_right = false);
	
    // Text functions
	virtual void clear_text();
    void set_text(const std::string& text);
    void append_text(const std::string& text);
	std::string get_text() const;

    // Display features
    void set_text_size(int size);
    void set_text_color(SDL_Color color);
    int get_text_size();
    SDL_Color get_text_color();
    
    /// @todo 
    // Justification
//    enum Justification_e { LEFT, MIDDLE, RIGHT };
//    
//    void set_justification(Justification_e just);
    
    
protected:    
    typedef std::vector<NewLetter_Disp_Obj> letters_ctr_t;
	
	const letters_ctr_t& get_letters() const
        { return letters; }
    
    virtual void add_letter(char ltr, int index);
	virtual void remove_letter(int index);

    void add_letter_no_redraw(char ltr, int index);
	void remove_letter_no_redraw(int index);

    DispPoint pos_at_index(size_t i);
	int index_at_pos(DispPoint pos_);

    virtual void update();

private:
	
    bool resizeable_down, resizeable_right;
    int w_init, h_init;
    
	letters_ctr_t letters;
	int text_size;
	SDL_Color color;
//    Justification_e justification;
};


// A TextView that allows for text entry (including mouse motion, deletion, etc.)
class TextField : public TextView {
public:	
	
	TextField(int w_ = 200, int h_ = 200, 
                  bool resizeable_down = false, bool resizable_right = false);
	
	
protected:
    
    // remove cursor when lose focus.
    virtual void lost_focus();
	
private:
    
    bool resizeable_down, resizeable_right;

	virtual bool handle_mouse_down(DispPoint pos_);
    virtual bool handle_key_down(SDL_keysym key);
    virtual bool handle_key_up(SDL_keysym key);
	
    bool key_is_held;
    SDLKey key_held;
    GUITimer time_key_held; 
    
    SDLMod modifiers_held;
    
    void handle_key_held();
    void handle_key();
    
    void handle_alpha_num(char ltr);
    void handle_modifier(SDLMod key);

    
	class Cursor : public GUI::View {
	public:
		Cursor(TextField* tb_ptr);

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
		TextField* text_box_ptr;
		
		bool flicker;
	};
    Cursor *cursor;
	bool flicker;
    void blink_cursor();
};


// Wraps a TextField with a white background and bevelled corners.
class GUITextBox : public GUI::View {
public:	
	
	GUITextBox(int w_, int h_);

    void set_text(const std::string& text) { field->set_text(text); }
	std::string get_text() const { return field->get_text(); }
    
    void set_text_size(int size) { return field->set_text_size(size); }
    void set_text_color(SDL_Color color) { return field->set_text_color(color);}
    
    int get_text_size() { return field->get_text_size(); }
    SDL_Color get_text_color() { return field->get_text_color(); }

private:
    TextField *field;

};


// Implements the Flyweight pattern.
class NewLetter_Disp_Obj{
public:
	NewLetter_Disp_Obj(char ltr, int size, DispPoint pos, SDL_Color color);
	
	void drawself(GUI::View *dest) const;
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

} // namespace GUI

#endif /* NEW_GUI_TEXT_BOX_H */


