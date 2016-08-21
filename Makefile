CC = g++
LD = g++

CFLAGS = -c -Wall -fno-elide-constructors -Iincludes -std=c++11
LFLAGS = -Wall -F/SDL.framework -framework SDL
OBJ_CFLAGS = -c -Wall -Iincludes

PROG = p6exe


GUI_LIB_CPPS := $(wildcard gui/*.cpp)
GUI_LIB_H := $(wildcard gui/*.h)

PROJ_CPPS := $(wildcard *.cpp)
PROJ_H := $(wildcard *.h)

OBJS = Compatibility.o GUIApp.o GUIAudio.o GUIController.o GUIImage.o GUIImageCache.o GUIImageView.o GUILetter.o GUIMsg.o GUIScrollView.o GUISpreadSheet.o GUITextViews.o GUITimer.o GUIUtility.o GUIValBox.o GUIView.o GUIWindow.o GameDisplay.o Letter.o NewNewGUITextView.o


default: $(OBJS)

Compatibility.o: Compatibility.cpp
	$(CC) $(CFLAGS) Compatibility.cpp
GUIApp.o: GUIApp.cpp
	$(CC) $(CFLAGS) GUIApp.cpp
GUIAudio.o: GUIAudio.cpp
	$(CC) $(CFLAGS) GUIAudio.cpp
GUIController.o: GUIController.cpp
	$(CC) $(CFLAGS) GUIController.cpp
GUIImage.o: GUIImage.cpp
	$(CC) $(CFLAGS) GUIImage.cpp
GUIImageCache.o: GUIImageCache.cpp
	$(CC) $(CFLAGS) GUIImageCache.cpp
GUIImageView.o: GUIImageView.cpp
	$(CC) $(CFLAGS) GUIImageView.cpp
GUILetter.o: GUILetter.cpp
	$(CC) $(CFLAGS) GUILetter.cpp
GUIMsg.o: GUIMsg.cpp
	$(CC) $(CFLAGS) GUIMsg.cpp
GUIScrollView.o: GUIScrollView.cpp
	$(CC) $(CFLAGS) GUIScrollView.cpp
GUISpreadSheet.o: GUISpreadSheet.cpp
	$(CC) $(CFLAGS) GUISpreadSheet.cpp
GUITextViews.o: GUITextViews.cpp
	$(CC) $(CFLAGS) GUITextViews.cpp
GUITimer.o: GUITimer.cpp
	$(CC) $(CFLAGS) GUITimer.cpp
GUIUtility.o: GUIUtility.cpp
	$(CC) $(CFLAGS) GUIUtility.cpp
GUIValBox.o: GUIValBox.cpp
	$(CC) $(CFLAGS) GUIValBox.cpp
GUIView.o: GUIView.cpp
	$(CC) $(CFLAGS) GUIView.cpp
GUIWindow.o: GUIWindow.cpp
	$(CC) $(CFLAGS) GUIWindow.cpp
GameDisplay.o: GameDisplay.cpp
	$(CC) $(CFLAGS) GameDisplay.cpp
Letter.o: Letter.cpp
	$(CC) $(CFLAGS) Letter.cpp
NewNewGUITextView.o: NewNewGUITextView.cpp
	$(CC) $(CFLAGS) NewNewGUITextView.cpp

# Objective-C source
SDLMain.o: SDLMain.mm
	echo "cflags: $(CFLAGS)"
	echo "obj_cflags: $(OBJ_CFLAGS)"
	$(CC) $(OBJ_CFLAGS) SDLMain.mm

clean:
	rm -f *.o

