#include "SDL.h"
#include "KW_label.h"
#include "KW_label_internal.h"
#include "KW_tilerenderer.h"
#include "KW_gui.h"

KW_Widget * KW_CreateLabel(KW_GUI * gui, KW_Widget * parent, const char * text, const KW_Rect * geometry) {
  KW_Label * label = calloc(sizeof(KW_Label), 1);
  KW_Widget * widget = KW_CreateWidget(gui, parent, KW_WIDGETTYPE_LABEL, geometry, PaintLabel, DestroyLabel, label);
  KW_SetLabelText(widget, text);
  KW_SetLabelAlignment(widget, KW_LABEL_ALIGN_CENTER, 0, KW_LABEL_ALIGN_MIDDLE, 0);
  KW_AddGUIFontChangedHandler(gui, LabelFontChanged, widget);
  return widget;
}

void KW_SetLabelAlignment(KW_Widget * widget, KW_LabelHorizontalAlignment halign, int hoffset, KW_LabelVerticalAlignment valign, int voffset) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL); 
  label->halign = halign;
  label->hoffset = hoffset;
  label->valign = valign;
  label->voffset = voffset;
}


KW_Font * KW_GetLabelFont(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  return label->font == NULL ? KW_GetFont(KW_GetGUI(widget)) : label->font;
}

void KW_SetLabelFont(KW_Widget * widget, KW_Font * font) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  label->font = font;
  RenderLabelText(widget);
}


void KW_SetLabelText(KW_Widget * widget, const char * text) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  if (label->text != NULL) {
    SDL_free(label->text);
  }
  label->text = SDL_strdup(text);
  RenderLabelText(widget);
  label->dirty = SDL_TRUE;
}

const char* KW_GetLabelText(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  return label->text;
}

void KW_SetLabelStyle(KW_Widget * widget, int style) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  label->style = style;
  RenderLabelText(widget);
}

void KW_SetLabelColor(KW_Widget * widget, KW_Color color) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  label->color = color;
  RenderLabelText(widget);
}

void KW_SetLabelIcon(KW_Widget * widget, const KW_Rect * iconclip) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  if (iconclip == NULL){
    label->iconclip.x = 0;
    label->iconclip.y = 0;
    label->iconclip.w = 0;
    label->iconclip.h = 0;
  } else {
    label->iconclip = *iconclip;
  }
}

