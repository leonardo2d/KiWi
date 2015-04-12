#include "KW_label.h"
#include "KW_label_internal.h"
#include "KW_textrenderer.h"

void LabelFontChanged(KW_GUI * gui, void * data, TTF_Font * font) {
  RenderLabelText((KW_Widget*)data);
}

void RenderLabelText(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  if (label->textrender != NULL) {
    SDL_DestroyTexture(label->textrender);
  }
  /* use our own font */
  label->textrender = KW_RenderTextLine(KW_GetLabelFont(widget),
                                         KW_GetWidgetRenderer(widget),
                                         label->text, label->color, label->style);

  if (label->textrender != NULL)
    SDL_QueryTexture(label->textrender, NULL, NULL, &(label->textwidth), &(label->textheight));
  else
    TTF_SizeUTF8(KW_GetLabelFont(widget), "", &(label->textwidth), &(label->textheight));
}

void DestroyLabel(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  SDL_free(label->text);
  SDL_DestroyTexture(label->textrender);
  free(label);
}



void PaintLabel(KW_Widget * widget) {
  KW_Label * label = (KW_Label *) KW_GetWidgetData(widget, KW_WIDGETTYPE_LABEL);
  
  SDL_Rect orig;
  SDL_Rect dst, icondst;
  SDL_Rect src, iconsrc;
  
  SDL_Renderer * renderer = KW_GetWidgetRenderer(widget);
  
  /* query actual w and h */
  src.x = src.y = 0;
  src.w = label->textwidth;
  src.h = label->textheight;
  
  /* calculate target x/y */  
  KW_GetWidgetAbsoluteGeometry(widget, &dst);
  orig = dst;

  /* calculate x according to halign */
  switch (label->halign) {
    
    case KW_LABEL_ALIGN_RIGHT:
      dst.x = dst.x + dst.w - src.w;
      break;
      
    case KW_LABEL_ALIGN_CENTER:
      dst.x = dst.x + dst.w/2 - src.w/2;
      break;
      
    /* easiest. do nothing */
    case KW_LABEL_ALIGN_LEFT:
    default:
        break;
  };
  
  /* apply horizontal offset and icon offset */
  dst.x += label->hoffset;
    
  /* calculate y according to valign */
  switch (label->valign) {
    case KW_LABEL_ALIGN_BOTTOM:
      dst.y = dst.y + dst.h - src.h;
      break;
      
    case KW_LABEL_ALIGN_MIDDLE:
      dst.y = dst.y + dst.h/2 - src.h/2;
      break;
      
    case KW_LABEL_ALIGN_TOP:
    default:
      break;
  }
  
  /* apply vertical offset */
  dst.y += label->voffset;
  
  /* display icon */
  if (!SDL_RectEmpty(&label->iconclip)) {
    iconsrc = label->iconclip;
    icondst.x = dst.x - (iconsrc.w/2);
    dst.x += iconsrc.w/2;
    icondst.y = dst.y - (iconsrc.h/2 - src.h/2); /* vertically center icon */
    if (icondst.y < orig.y) {
      iconsrc.y += orig.y - icondst.y;
      icondst.y += orig.y - icondst.y;
    }
    
    if (icondst.x < orig.x) {
      iconsrc.x += orig.x - icondst.x;
      icondst.x += orig.x - icondst.x;
    }
    
    if (iconsrc.h > orig.h) iconsrc.h = orig.h;
    if (iconsrc.w > orig.w) iconsrc.w = orig.w;
    icondst.h = iconsrc.h;
    icondst.w = iconsrc.w;
    
    SDL_RenderCopy(renderer, KW_GetWidgetTilesetTexture(widget), &iconsrc, &icondst);
  }
    /* clip texture so that it doesnt overflow desired maximum geometry */
  if (dst.x < orig.x) src.x = orig.x - dst.x; /* clip left part (centering and right align overflows to the left) */
  if (dst.y < orig.y) src.y = orig.y - dst.y; /* clip top part (middle, bottom align migh overflow top) */
  if (dst.x + src.w > orig.x + orig.w) src.w = orig.w + (orig.x - dst.x) - src.x; /* clip right part (centering, left align) */
  else src.w = src.w - src.x;
  if (dst.y + src.h > orig.y + orig.h) src.h = orig.h + (orig.y - dst.y) - src.y; /* clip bottom part (middle, top) */
  else src.h = src.h - src.y;
  /* don stretch the image */
  dst.w = src.w;
  dst.h = src.h;
  dst.x += src.x;
  dst.y += src.y;
  
  SDL_RenderCopy(renderer, label->textrender, &src, &dst);
}
