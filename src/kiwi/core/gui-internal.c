#include "gui-internal.h"
#include "widget-internal.h"
#include "kiwi/core/gui.h"

#define KW_FireWidgetEvent(widget, callbacktype, event, args) { \
  unsigned __i; \
  for (__i = 0; __i < widget->eventhandlers[event].count; __i++) { \
    ((callbacktype) ((callbacktype *) widget->eventhandlers[event].handlers)[__i]) args; \
  } \
}

KW_Widget * CalculateMouseOver(KW_Widget * widget, int x, int y) {
  int i;
  KW_Widget * found = NULL;
  KW_Rect g = widget->composed;
  if (widget->parent) {
    g.x += widget->parent->absolute.x;
    g.y += widget->parent->absolute.y;
  }
  /* mouseover is a input event, avoid calculating it */
  if (KW_IsWidgetInputEventsBlocked(widget) || KW_IsWidgetHidden(widget)) {
    return NULL;
  }

  /* if not in composed geometry, nothing to see here then. rootwidget always borked. */
  if (!(x > g.x && x < g.x + g.w && y > g.y && y < g.y + g.h) && widget != widget->gui->rootwidget) {
    return NULL;
  }

  for (i = widget->childrencount-1; i >= 0 && found == NULL; i--)
    found = CalculateMouseOver(widget->children[i], x, y);

  /* children don't have it. Maybe its this widget then? */
  if (found == NULL) {
    g = widget->absolute;
    /* use our own absolute to test, then */
    if (x > g.x && x < g.x + g.w && y > g.y && y < g.y + g.h)
      found = widget;
  }
  return found;
}

void MouseMoved(KW_GUI * gui, int mousex, int mousey, int xrel, int yrel) {
  KW_Widget * current = gui->currentmouseover;
  KW_Widget * widget = NULL;
  /* first check if we are in drag mode */
  if (gui->cursordown == SDL_TRUE) {
    /* check if drag is starting */
    if (gui->currentdrag == NULL) {
      /* drag is starting on the current mouse over */
      if (current != NULL) {
        KW_FireWidgetEvent(current, KW_OnDragStart, KW_ON_DRAGSTART, (current, mousex, mousey));
        gui->currentdrag = current;
      }
    } else {
      /* drag HAS started already. Update widh drag positions */
      KW_FireWidgetEvent(current, KW_OnDrag, KW_ON_DRAG, (current, mousex, mousey, xrel, yrel));
    }
    /* no mouse movement events are calculated while dragging */
    return;
  }
  widget = CalculateMouseOver(gui->rootwidget, mousex, mousey);
  if (widget == current) return;

  /* gotta notify the previous mouseover */
  if (current != NULL) {
    KW_FireWidgetEvent(current, KW_OnMouseLeave, KW_ON_MOUSELEAVE, (current));
  }

  /* warn the current mouseover */
  gui->currentmouseover = widget;
  if (widget != NULL) {
    KW_FireWidgetEvent(widget, KW_OnMouseLeave, KW_ON_MOUSEOVER, (widget));
  }
}


void MousePressed(KW_GUI * gui, int mousex, int mousey, int button) {
  KW_Widget *widget = gui->currentmouseover;
  (void) mousex;
  (void) mousey;
  if (widget != NULL) {
    if (KW_IsWidgetInputEventsBlocked(widget) || KW_IsWidgetHidden(widget)) return;
    KW_FireWidgetEvent(widget, KW_OnMouseDown, KW_ON_MOUSEDOWN, (widget, button));
    gui->cursordown = SDL_TRUE;
  }
}

void MouseReleased(KW_GUI * gui, int mousex, int mousey, int button) {
  KW_Widget * widget = gui->currentmouseover, * actualmouseover;

  gui->cursordown = SDL_FALSE;
  gui->cursorwasdown = SDL_TRUE;

  if (widget && (KW_IsWidgetInputEventsBlocked(widget) || KW_IsWidgetHidden(widget))) return;

  /* check if was under drag */
  if (gui->currentdrag != NULL) {
    KW_FireWidgetEvent(gui->currentdrag, KW_OnDragStop, KW_ON_DRAGSTOP, (gui->currentdrag, mousex, mousey));
    gui->currentdrag = NULL;
  }
  if (!widget) return;

  actualmouseover = CalculateMouseOver(widget->parent, mousex, mousey);
  if (widget == actualmouseover) {
    KW_FireWidgetEvent(widget, KW_OnMouseUp, KW_ON_MOUSEUP, (widget, button));
    KW_SetFocusedWidget(widget);
  }
}

void TextInputReady(KW_GUI * gui, uint8_t const * text) {
  if (gui->currentfocus == NULL) return;
  KW_FireWidgetEvent(gui->currentfocus, KW_OnTextInput, KW_ON_TEXTINPUT, (gui->currentfocus, text));
}

void KeyUp(KW_GUI * gui, KW_InputKey key) {
  if (gui->currentfocus == NULL) return;
  KW_FireWidgetEvent(gui->currentfocus, KW_OnKeyUp, KW_ON_KEYUP, (gui->currentfocus, key));
}

void KeyDown(KW_GUI * gui, KW_InputKey key) {
  if (gui->currentfocus == NULL) return;
  KW_FireWidgetEvent(gui->currentfocus, KW_OnKeyDown, KW_ON_KEYDOWN, (gui->currentfocus, key));
}

#undef KW_FireWidgetEvent
