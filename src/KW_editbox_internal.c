#include "KW_editbox.h"
#include "KW_editbox_internal.h"
#include "KW_tilerenderer.h"
#include "KW_textrenderer.h"
#include "utf8.h"

/* private functions */
void PaintEditbox(KW_Widget * widget) {
  SDL_Rect targetgeom;  
  SDL_Renderer * renderer;
  SDL_Texture * tileset;  
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  /* base column for tile rendering */
  int basec = 0;
  if (editbox->mouseover || editbox->active) basec = 3;
  if (editbox->clicked) basec = 0;


  KW_GetWidgetAbsoluteGeometry(widget, &targetgeom);

  renderer = KW_GetWidgetRenderer(widget);
  tileset = KW_GetWidgetTilesetTexture(widget);

  KW_RenderTileFrame(renderer, tileset, 6, basec, targetgeom.x, targetgeom.y, targetgeom.w, targetgeom.h);

  /* adjust target geom */
  targetgeom.x += TILESIZE;
  targetgeom.y += TILESIZE;
  targetgeom.w = targetgeom.w - TILESIZE * 2;
  targetgeom.h = targetgeom.h - TILESIZE * 2;

  PaintEditboxText(editbox, &targetgeom);
}

/* paints the editbox text */
void PaintEditboxText(KW_Editbox * editbox, SDL_Rect * _dst) {
  SDL_Rect dst = *_dst; /* destination rectangle */
  SDL_Rect orig = dst;  /* intended dst rectangle */
  SDL_Rect src;         /* text clipping */
  int cursorx;

  SDL_Renderer * renderer = KW_GetWidgetRenderer(editbox->widget);
  SDL_Texture * tileset = KW_GetWidgetTilesetTexture(editbox->widget);

  /* query actual w and h */
  src.x = src.y = 0; src.w = editbox->textwidth; src.h = editbox->textheight;

  /* will paint label in the middle */
  dst.y = dst.y + dst.h / 2 - src.h / 2;

  /* clip bottom part (middle, top) */
  if (dst.y + src.h > orig.y + orig.h) src.h = orig.h + (orig.y - dst.y) - src.y;

  /* ajust text src rect and render cursor if active */
  cursorx = editbox->cursorx;
  if (editbox->active) {
    int adjust = editbox->cursoradjustx;
    /* if the cursor is left to the clipping rectangle */
    if (cursorx < adjust) {
      adjust = cursorx;
    }
    /* if the cursor is right of the clipping rectangle (dst.w or less) */
    else if (cursorx > adjust + dst.w) {
      adjust = cursorx - dst.w;
      cursorx = adjust;
    }

    /* sets the clipping origin as calculated */
    src.x = adjust;
    
    /* save the adjust for next time */
    editbox->cursoradjustx = adjust;
    
    /* see if the clip rect is bigger than dst */
    if (src.w > dst.w) src.w = dst.w;
    
    /* see if clipping rect is bigger than total width, might have
     * to adjust clipping origin */
    if ((src.x + src.w) > editbox->textwidth) {
      src.x -= (src.x + src.w) - editbox->textwidth;
    }
    
    /* basic checks */
    if (src.w < dst.w) dst.w = src.w;
    if (src.h < dst.h) dst.h = src.h;

    /* render cursor *below* text */
    KW_RenderTileFill(renderer, tileset, 7, 7, dst.x + cursorx - src.x, dst.y,
                      TILESIZE, dst.h);
    
    /* render text */
    SDL_RenderCopy(renderer, editbox->textrender, &src, &dst);
  } else {
    /* just the basic checks */
    if (src.w > dst.w) src.w = dst.w;
    if (src.w < dst.w) dst.w = src.w;
    if (src.h < dst.h) dst.h = src.h;
    SDL_RenderCopy(renderer, editbox->textrender, &src, &dst);
  }
}


void RenderEditboxText(KW_Editbox * editbox) {
  if (editbox->textrender != NULL) {
    SDL_DestroyTexture(editbox->textrender);
  }
  /* use our own font */
  editbox->textrender = KW_RenderTextLine(KW_GetEditboxFont(editbox->widget),
                                         KW_GetWidgetRenderer(editbox->widget),
                                         editbox->text, editbox->color,
                                         TTF_STYLE_NORMAL);

  if (editbox->textrender != NULL)
    SDL_QueryTexture(editbox->textrender, NULL, NULL, &(editbox->textwidth),
                     &(editbox->textheight));
  else
    TTF_SizeUTF8(KW_GetEditboxFont(editbox->widget), "", &(editbox->textwidth), &(editbox->textheight));
}

KW_Editbox * AllocEditbox() {
  KW_Editbox * editbox = calloc(sizeof(*editbox), 1);
  return editbox;
}

void DestroyEditbox(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  free(editbox);
}
void AdjustCursor(KW_Editbox * editbox, int cursormove) {
  char save;
  int len = SDL_strlen(editbox->text);

  /* adjust cursormove, don't let it over/underflow. */
  if (cursormove > 0) {
    if (editbox->cursor + cursormove > len) {
      cursormove = len - editbox->cursor;
    }
  } else if (cursormove < 0) {
    if (-cursormove > editbox->cursor) {
      cursormove = -editbox->cursor;
    }
  }

  /* now account for UTF-8 */
  if (cursormove > 0) {
    /* its a UTF-8 sequence */
    while (cursormove-- > 0) {
      (void)(
           isutf(editbox->text[++(editbox->cursor)])
        || isutf(editbox->text[++(editbox->cursor)])
        || isutf(editbox->text[++(editbox->cursor)])
        || ++(editbox->cursor)
      );
    }
  } else if (cursormove < 0) {
    while (cursormove++ < 0) {
      (void)(
        isutf(editbox->text[--(editbox->cursor)])
        || isutf(editbox->text[--(editbox->cursor)])
        || isutf(editbox->text[--(editbox->cursor)])
        || --(editbox->cursor)
      );
    }
  }

  /* recalculate cursor position */
  save = editbox->text[editbox->cursor];
  editbox->text[editbox->cursor] = '\0';
  TTF_SizeUTF8(KW_GetEditboxFont(editbox->widget), editbox->text, &editbox->cursorx, NULL);
  editbox->text[editbox->cursor] = save;
}

void TextBackspace(KW_Editbox * editbox) {
  AdjustCursor(editbox, -1);
  TextDelete(editbox);
}

void TextDelete(KW_Editbox * editbox) {
  int i = 0;
  int len = SDL_strlen(editbox->text);
  int seq = u8_seqlen(editbox->text[editbox->cursor]);
  printf("seqlen is %d\n", seq);
  for (i = editbox->cursor; i < len; i++) {
    editbox->text[i] = editbox->text[i + seq];
  }
  RenderEditboxText(editbox);
}

/* KiWi callbacks */
void EditboxKeyDown(KW_Widget * widget, SDL_Keycode key, SDL_Scancode scan) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);  
  switch (scan) {
    /* set up cursor states */
    case SDL_SCANCODE_LEFT:
      AdjustCursor(editbox, -1);
      break;
    case SDL_SCANCODE_RIGHT:
      AdjustCursor(editbox, 1);
      break;
      
    case SDL_SCANCODE_BACKSPACE:
      TextBackspace(editbox);
      break;
      
    case SDL_SCANCODE_DELETE:
      TextDelete(editbox);
      break;
      
    case SDL_SCANCODE_HOME:
      AdjustCursor(editbox, -u8_strlen(editbox->text));
      break;
      
    case SDL_SCANCODE_END:
      AdjustCursor(editbox, u8_strlen(editbox->text));
      break;
      
    default:
      break;
  }
}

void EditboxTextInput(KW_Widget * widget, const char * text) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  int i = 0, insertlen, textlen, cursoradjust;

  /* make room in the middle */
  i = editbox->cursor;
  insertlen = SDL_strlen(text);
  cursoradjust = insertlen;
  textlen = SDL_strlen(editbox->text);

  for (i = textlen; i >= editbox->cursor && i > 0; i--) {
    editbox->text[textlen + insertlen - (textlen - i)] = editbox->text[i];
  }

  for (i = 0; i < insertlen; i++) {
    editbox->text[editbox->cursor + i] = text[i];
  }
  
  /* correct insertlen if its utf8 because AdjustCursor is in terms
   *  of perceived chars */
  cursoradjust = u8_strlen(text);
  AdjustCursor(editbox, cursoradjust);
  RenderEditboxText(editbox);
}

void EditboxMouseOver(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->mouseover = SDL_TRUE;
}

void EditboxMouseLeave(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->mouseover = SDL_FALSE;
}

void EditboxMousePress(KW_Widget * widget, int b) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->clicked = SDL_TRUE;
}

void EditboxMouseRelease(KW_Widget * widget, int b) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->clicked = SDL_FALSE;
}

void EditboxFocusGain(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->active = SDL_TRUE;
  SDL_StartTextInput();
}

void EditboxFocusLose(KW_Widget * widget) {
  KW_Editbox * editbox = KW_GetWidgetData(widget, KW_WIDGETTYPE_EDITBOX);
  editbox->active = SDL_FALSE;
  SDL_StopTextInput();
}

void EditboxFontChanged(KW_GUI * gui, void * priv, TTF_Font * font) {
  KW_Editbox * editbox = KW_GetWidgetData((KW_Widget*)priv, KW_WIDGETTYPE_EDITBOX);
  RenderEditboxText(editbox);
}
