#include "KW_renderdriver_sdl2.h"
#include "KW_renderdriver.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include <stdio.h>
#include <string.h>

typedef struct KWSDL {
  SDL_Renderer * renderer;
  SDL_Window * window;
} KWSDL;

static void KWSDL_renderCopy(KW_RenderDriver * driver, KW_Texture * texture, const KW_Rect * src, const KW_Rect * dst);
static KW_Texture * KWSDL_renderText(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
static KW_Font * KWSDL_loadFont(KW_RenderDriver * driver, const char * font, unsigned ptSize);
static KW_Font * KWSDL_createTexture(KW_RenderDriver * driver, KW_Surface * surface);
static KW_Surface * KWSDL_createRGBA32Surface(KW_RenderDriver * driver, unsigned width, unsigned height);
static KW_Texture * KWSDL_loadTexture(KW_RenderDriver * driver, const char * texturefile);
static KW_Surface * KWSDL_loadSurface(KW_RenderDriver * driver, const char * texturefile);
static void KWSDL_getSurfaceExtents(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height);
static void KWSDL_releaseTexture(KW_RenderDriver * driver, KW_Texture * texture);
static void KWSDL_releaseFont(KW_RenderDriver * driver, KW_Font * font);
static void KWSDL_blitSurface(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect);
static void KWSDL_releaseSurface(KW_RenderDriver * driver, KW_Surface * font);

struct KW_RenderDriver * KW_CreateSDL2RenderDriver(SDL_Renderer * renderer, SDL_Window * window) {
  struct KWSDL * kwsdl = calloc(1, sizeof(*kwsdl));
  struct KW_RenderDriver * rd = calloc(1, sizeof(*rd));

  kwsdl->renderer = renderer;
  kwsdl->window = window;

  rd->renderCopy = KWSDL_renderCopy;
  rd->renderText = KWSDL_renderText;
  rd->loadFont = KWSDL_loadFont;
  rd->createTexture = KWSDL_createTexture;
  rd->createSurface = KWSDL_createRGBA32Surface;
  rd->loadTexture = KWSDL_loadTexture;
  rd->loadSurface = KWSDL_loadSurface;
  rd->getSurfaceExtents = KWSDL_getSurfaceExtents;
  rd->blitSurface = KWSDL_blitSurface;
  rd->releaseFont = KWSDL_releaseFont;
  rd->releaseSurface = KWSDL_releaseSurface;
  rd->releaseTexture = KWSDL_releaseTexture;

  rd->priv = kwsdl;
  return rd;
}

static KW_Texture * KWSDL_createTexture(KW_RenderDriver * driver, KW_Surface * surface) {
    KWSDL * kwsdl = (KWSDL *) driver->priv;
    return SDL_CreateTextureFromSurface(kwsdl->renderer, (SDL_Surface *)surface);
}

static KW_Surface * KWSDL_createRGBA32Surface(KW_RenderDriver * driver, unsigned width, unsigned height) {
    unsigned rmask, gmask, bmask, amask;
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    return SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
}

static KW_Font * KWSDL_loadFont(KW_RenderDriver * driver, const char * font, unsigned ptSize) {
  TTF_Font * f = TTF_OpenFont(font, ptSize);
  (void)driver;
  if (f == NULL) {
    fprintf(stderr, "KW_RenderDriver_SDL: Could not load font %s: %s\n", font, TTF_GetError());
    return NULL;
  }
  return f;
}

static KW_Surface * KWSDL_loadSurface(KW_RenderDriver * driver, const char * texturefile) {
  SDL_Surface * s = IMG_Load(texturefile);
  if (s == NULL) {
    fprintf(stderr, "KW_RenderDriver_SDL: Could not load texture %s: %s\n", texturefile, IMG_GetError());
    return NULL;
  }
  return s;
}

static void KWSDL_getSurfaceExtents(KW_RenderDriver * driver, const KW_Surface * surface, unsigned * width, unsigned * height) {
    (void)driver;
    if (width) *width = ((SDL_Surface*)surface)->w;
    if (height) *height = ((SDL_Surface*)surface)->h;
}

static void KWSDL_blitSurface(KW_RenderDriver * driver, KW_Surface * src, const KW_Rect * srcRect, KW_Surface * dst, const KW_Rect * dstRect) {
  SDL_Rect s, d;
  s.x = srcRect->x; s.y = srcRect->y; s.w = srcRect->w; s.h = srcRect->h;
  d.x = dstRect->x; d.y = dstRect->y; d.w = dstRect->w; d.h = dstRect->h;
  SDL_BlitSurface((SDL_Surface *) src, &s, (SDL_Surface *) dst, &d);
}

static KW_Texture * KWSDL_loadTexture(KW_RenderDriver * driver, const char * texturefile) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Texture * t = IMG_LoadTexture(kwsdl->renderer, texturefile);
  if (t == NULL) {
    fprintf(stderr, "KW_RenderDriver_SDL: Could not load texture %s: %s\n", texturefile, IMG_GetError());
    return NULL;
  }
  return t;
}

static KW_Texture * KWSDL_renderText(KW_RenderDriver * driver, KW_Font * font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  int previousstyle;
  SDL_Color sdlcolor;
  SDL_Surface * textsurface;
  SDL_Texture * ret;

  sdlcolor.r = color.r, sdlcolor.g = color.g, sdlcolor.b = color.b, sdlcolor.a = color.a;
  if (font == NULL || text == NULL)
    return NULL;

  previousstyle = TTF_GetFontStyle(font);
  TTF_SetFontStyle(font, style);
  textsurface = TTF_RenderUTF8_Blended(font, text, sdlcolor);
  ret = SDL_CreateTextureFromSurface(kwsdl->renderer, textsurface);
  SDL_FreeSurface(textsurface);
  TTF_SetFontStyle(font, previousstyle);
  return ret;
}

static void KWSDL_releaseTexture(KW_RenderDriver * driver, KW_Texture * texture) {
  (void)driver;
  SDL_DestroyTexture(texture);
}

static void KWSDL_releaseSurface(KW_RenderDriver * driver, KW_Surface * surface) {
  (void)driver;
  SDL_FreeSurface(surface);
}

static void KWSDL_releaseFont(KW_RenderDriver * driver, KW_Font * font) {
  (void)driver;
  TTF_CloseFont(font);
}

static void KWSDL_renderCopy(KW_RenderDriver * driver, KW_Texture * texture, const KW_Rect * src, const KW_Rect * dst) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Rect srcRect, dstRect;
  srcRect.x = src->x; srcRect.y = src->y; srcRect.w = src->w; srcRect.h = src->h;
  dstRect.x = dst->x; dstRect.y = dst->y; dstRect.w = dst->w; dstRect.h = dst->h;
  SDL_RenderCopy(kwsdl->renderer, texture, &srcRect, &dstRect);
}
