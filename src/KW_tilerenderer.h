/*
  Copyright (c) 2014, Leonardo Guilherme de Freitas
  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

     1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

     2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

     3. This notice may not be removed or altered from any source
     distribution.
*/

#ifndef KW_TILERENDERER_H
#define KW_TILERENDERER_H

/**
 * \file KW_tilerenderer.h
 * 
 * Declare functions for rendering tiles from a tile set.
 * The macro TILESIZE defines the size of the tile (w/h).
 */

#include "SDL.h"
#include "KW_macros.h"
#define TILESIZE 8

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   Render a single tile from a tileset using the specified renderer.
 * \param   renderer The SDL_Renderer that will render this tile.
 * \param   tileset The tileset texture to get this tile from.
 * \param   column The column in the tile set (starts at 0).
 * \param   line The line in the tile set (starts at 0).
 * \param   x The x coordinate in the screen to render the tile.
 * \param   y The y coordinate in the screen to render the tile.
 */
extern DECLSPEC void KW_RenderTile(SDL_Renderer * renderer, SDL_Texture * tileset, int column, int line, int x, int y);

/**
 * \brief   Blit a single tile from a tileset to a surface.
 * \param   dst The destination SDL_Surface.
 * \param   tileset The tileset surface to get this tile from.
 * \param   column The column in the tile set (starts at 0).
 * \param   line The line in the tile set (starts at 0).
 * \param   x The x coordinate in the destination surface to blit the tile.
 * \param   y The y coordinate in the destination surface to blit the tile.
 */
extern DECLSPEC void KW_BlitTile(SDL_Surface * dst, SDL_Surface * tileset, int column, int line, int x, int y);

/**
 * \brief   Render a tile multiple times filling the whole w/h specified.
 * \details The tile rendering will start at x/y and will fill until x+w/y+h,
 *          clipping when necessary.
 * \param   renderer The SDL_Renderer that will render this tile.
 * \param   tileset The tileset texture to get this tile from.
 * \param   column The column in the tile set (starts at 0).
 * \param   line The line in the tile set (starts at 0).
 * \param   x The x coordinate in the screen to render the tile.
 * \param   y The y coordinate in the screen to render the tile.
 * \param   w The width to fill.
 * \param   h The height to fill.
 */
extern DECLSPEC void KW_RenderTileFill(SDL_Renderer * renderer, SDL_Texture * tileset, int column, int line, int x, int y, int w, int h);

/**
 * \brief   Blit (copy) a tile multiple times filling the whole w/h specified.
 * \details The tile rendering will start at x/y and will fill until x+w/y+h,
 *          clipping when necessary.
 * \param   dst Destination surface for this tile.
 * \param   tileset The tileset surface to get this tile from.
 * \param   column The column in the tile set (starts at 0).
 * \param   line The line in the tile set (starts at 0).
 * \param   x The x coordinate in the surface to blit the tile.
 * \param   y The y coordinate in the surface to blit the tile.
 * \param   w The width to fill.
 * \param   h The height to fill.
 */
extern DECLSPEC void KW_BlitTileFill(SDL_Surface * dst,  SDL_Surface * tileset, int column, int line, int x, int y, int w, int h);

/**
 * \brief   Render a frame using the set of tiles specified at startcolumn and startline.
 * \details This function will make use of the 3x3 set of tiles to draw a frame. They are addressed by column-then-line, like this:
 *          - 0,0 is the top-left corner
 *          - 1,0 is the top
 *          - 2,0 is the top-right corner
 * 
 *          - 0,1 is the left
 *          - 1,1 is the middle
 *          - 2,1 is the right
 * 
 *          - 0,2 is the bottom-left corner
 *          - 1,2 is the bottom
 *          - 2,2 is the bottom-right corner
 * 
 *          They are read column first, line second.
 * 
 *          .---------.---------.---------.
 *          |         |         |         |
 *          |   0,0   |   1,0   |   2,0   |
 *          |         |         |         |
 *          .---------.---------.---------.
 *          |         |         |         |
 *          |   0,1   |   1,1   |   2,1   |
 *          |         |         |         |
 *          .---------.---------.---------.
 *          |         |         |         |
 *          |   0,2   |   1,2   |   2,2   |
 *          |         |         |         |
 *          `---------`---------`---------´
 * 
 * 
 * \param   renderer The SDL_Renderer that will render this frame.
 * \param   tileset The tileset texture to get this frame set from.
 * \param   startcolumn The column in the tile set (starts at 0).
 * \param   startline The line in the tile set (starts at 0).
 * \param   x The x coordinate in the screen to render the tiles.
 * \param   y The y coordinate in the screen to render the tiles.
 * \param   w The width to fill.
 * \param   h The height to fill.
 */
extern DECLSPEC void KW_RenderTileFrame(SDL_Renderer * renderer, SDL_Texture * tileset, int startcolumn, int startline, int x, int y, int w, int h);

/**
 * \brief   Blit a frame from tileset into a surface.
 * \details The arguments of this function are equivalent to KW_RenderTileFrame,
 *          except that instead of SDL_Texture, it uses a SDL_Surface for the
 *          tileset. Useful for caching and pixel manipulations before
 *          having a texture.
 * \param   dst The destination surface. Must be created preferably using the
 *          same format as the tileset surface. (tilesest->format field).
 * \param   tileset The source tileset surface.
 * \param   startcolumn The column in the tile set (starts at 0).
 * \param   startline The line in the tile set (starts at 0).
 * \param   x The x coordinate in the screen to render the tiles.
 * \param   y The y coordinate in the screen to render the tiles.
 * \param   w The width to fill.
 * \param   h The height to fill.
 */
extern DECLSPEC void KW_BlitTileFrame(SDL_Surface * dst, SDL_Surface * tileset, int startcolumn, int startline, int x, int y, int w, int h);

/**
 * \brief   Creates a SDL_Texture from a frame to be used as cache.
 * \details Use this function to create a single SDL_Texture made of
 *          tiles from the tileset (as in KW_BlitFrame or KW_RenderTileFrame) to
 *          be rendered later as a whole instead of rendering each tile
 *          separately (which is very slow for big frames).
 * 
 *          Keep in mind that calling this function every frame is *much* slower
 *          than render each tile of a frame directly. This function is for
 *          widgets that are very big and/or don't change very often.
 * \param   renderer The SDL_Renderer that will render this frame.
 * \param   tileset The tileset surface to get this frame set from.
 * \param   startcolumn The column in the tile set (starts at 0).
 * \param   startline The line in the tile set (starts at 0).
 * \param   w The width to fill.
 * \param   h The height to fill.
 */
extern DECLSPEC SDL_Texture * KW_CreateTileFrameTexture(SDL_Renderer * renderer, SDL_Surface * tileset, int startcolumn, int startline, int w, int h);

#ifdef __cplusplus
}
#endif

#endif
