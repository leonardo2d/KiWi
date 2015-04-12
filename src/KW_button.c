#include "KW_button.h"
#include "KW_button_internal.h"
#include "KW_label.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"

KW_Button * AllocButton();
void PaintButton(KW_Widget * widget);
void RenderButton(KW_Widget * widget);
void DestroyButton(KW_Widget * widget);
void ButtonGeometryChanged(KW_Widget * widget, const SDL_Rect * newgeom, const SDL_Rect * oldgeom);

static void MouseOver(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->mouseover = SDL_TRUE;
}


static void MouseLeave(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->mouseover = SDL_FALSE;
}

static void MousePress(KW_Widget * widget, int b) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->clicked = SDL_TRUE;
}

static void MouseRelease(KW_Widget * widget, int b) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  button->clicked = SDL_FALSE;
}


KW_Widget * KW_CreateButton(KW_GUI * gui, KW_Widget * parent, const char * text, const SDL_Rect * geometry) {
  SDL_Rect labelgeom;
  KW_Widget * widget = NULL;
  KW_Button * button = NULL;
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = geometry->w - TILESIZE * 2;
  labelgeom.h = geometry->h - TILESIZE * 2;
  
  button = AllocButton();
  widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_BUTTON, geometry, PaintButton, DestroyButton, button);
  button->labelwidget = KW_CreateLabel(gui, widget, text, &labelgeom);
  KW_BlockWidgetInputEvents(button->labelwidget);
  KW_AddWidgetMouseOverHandler(widget, MouseOver);
  KW_AddWidgetMouseLeaveHandler(widget, MouseLeave);
  KW_AddWidgetMouseDownHandler(widget, MousePress);
  KW_AddWidgetMouseUpHandler(widget, MouseRelease);
  KW_AddWidgetGeometryChangeHandler(widget, ButtonGeometryChanged);
  RenderButton(widget);
  return widget;
}

/** internal stuff **/

void PaintButton(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  SDL_Rect targetgeom;
  SDL_Renderer * renderer = KW_GetWidgetRenderer(widget);
  /* don't draw frame if requested. */
  if (KW_QueryWidgetHint(widget, KW_WIDGETHINT_FRAMELESS) == SDL_TRUE)
    return;

  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);
  if (button->mouseover)
    SDL_RenderCopy(renderer, button->normal, NULL, &targetgeom);
  else
    SDL_RenderCopy(renderer, button->over, NULL, &targetgeom);
}

void RenderButton(KW_Widget * widget) {
  SDL_Rect targetgeom;
  SDL_Surface * tileset = KW_GetWidgetTilesetSurface(widget);
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);  
  if (button->normal != NULL) SDL_DestroyTexture(button->normal);
  button->normal = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 3, 3, targetgeom.w, targetgeom.h);
  if (button->over != NULL) SDL_DestroyTexture(button->over);
  button->over = KW_CreateTileFrameTexture(KW_GetWidgetRenderer(widget), tileset, 3, 0, targetgeom.w, targetgeom.h);
}

void DestroyButton(KW_Widget * widget) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  SDL_DestroyTexture(button->over);
  SDL_DestroyTexture(button->normal);
  free(button);
}

void ButtonGeometryChanged(KW_Widget * widget, const SDL_Rect * newgeom, const SDL_Rect * oldgeom) {
  SDL_Rect labelgeom;
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  
  if (oldgeom->w != newgeom->w || oldgeom->h != newgeom->h) {
    RenderButton(widget);
  }
  
  labelgeom.x = TILESIZE;
  labelgeom.y = TILESIZE;
  labelgeom.w = newgeom->w - TILESIZE * 2;
  labelgeom.h = newgeom->h - TILESIZE * 2;
  KW_SetWidgetGeometry(button->labelwidget, &labelgeom);
}


KW_Button * AllocButton() {
  KW_Button * button = calloc(sizeof(KW_Button), 1);
  return button;
}

void KW_SetButtonIcon(KW_Widget * widget, const SDL_Rect * iconclip) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  KW_SetLabelIcon(button->labelwidget, iconclip);
}

void KW_SetButtonText(KW_Widget * widget, const char * text) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  KW_SetLabelText(button->labelwidget, text);
}



void KW_SetButtonFont(KW_Widget * widget, TTF_Font * font) {
  KW_Button * button = KW_GetWidgetData(widget, KW_WIDGETTYPE_BUTTON);
  KW_SetLabelFont(button->labelwidget, font);
}
