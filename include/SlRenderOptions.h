// part of SDL2lazy
// author: Ulrike Hager

/*! \file SlRenderOptions.h
  \brief enum SlRenderOptions

*/

/*! \enum SlRenderOptions
  \brief specifies where, what, and how to render. 

  Specifies whether to use the SlTexture's source or destination SDL_Rect. 
  Also specifies use of colour mod and alpha mod.\n
  Currently implemented:\n
  sourceRect to clip part of the texture\n
  destinationRect to specify where and how big to render\n
  colour mod (SDL_SetTextureColorMod)\n
  alpha mod (SDL_SetTextureAlphaMod)\n
*/
enum SlRenderOptions {
  SL_RENDER_DEFAULT         = (1u << 0),
  SL_RENDER_COLORMOD        = (1u << 1),
  SL_RENDER_ALPHAMOD        = (1u << 2),
};
