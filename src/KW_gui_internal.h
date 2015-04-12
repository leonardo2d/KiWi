#ifndef KW_KW_INTERNAL_H
#define KW_KW_INTERNAL_H

#include "KW_widget.h"
#include "SDL_ttf.h"

typedef void (*GUIHandler)(void);

typedef enum KW_GUIEventHandlerType {
  KW_GUI_ONFONTCHANGED,
  KW_GUIEVENTHANDLER_TOTAL
} KW_GUIEventHandlerType;

struct KW_GUI {
  SDL_Texture * tilesettexture;
  SDL_Surface * tilesetsurface;
  SDL_Renderer * renderer;
  KW_Widget * rootwidget; /* the parent to all parent-less widgets :) */
  TTF_Font * font;
  
  KW_Widget * currentmouseover; /* the current widget that has mouse over */
  KW_Widget * currentfocus; /* which widget was the last focused/clicked */
  KW_Widget * currentdrag; /* the widget where the drag started */
  SDL_bool cursordown; /* indicates whether the cursor is clicked or not */
  
  SDL_Event evqueue[1024];
  int evqueuesize;
  SDL_mutex * evqueuelock;
  
  struct {
    struct KW_GUICallback {
      GUIHandler handler;
      void * priv;
    } * handlers;
    unsigned int      count;
  } eventhandlers[KW_GUIEVENTHANDLER_TOTAL];
};

void AddGUIHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, GUIHandler handler, void * priv);
void RemoveGUItHandler(KW_GUI * gui, KW_GUIEventHandlerType handlertype, GUIHandler handler, void * priv);





#endif
