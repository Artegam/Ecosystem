#include "Views.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Views::Texture::Texture()
{
  init();
}

Views::Texture::~Texture()
{
  free();
}

void Views::Texture::init()
{
  //Initialize
  renderer = NULL;
  texture = NULL;
  width = 0;
  height = 0;
}

void Views::Texture::free()
{
  //Free texture if it exists
  if( texture != NULL )
  {
   SDL_DestroyTexture (texture);
   init();
  }
}

bool Views::Texture::createTextureFromSurface(SDL_Surface * surface) {
  SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0xFF, 0xFF));

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if(texture == NULL)
  {
   printf("Unable to create texture ! SDL Error: %s\n", SDL_GetError());
   return 0;
  }

  //Get image dimensions
  width = surface->w;
  height = surface->h;

  SDL_FreeSurface(surface);//Get rid of old loaded surface
  return texture != NULL;
}

bool Views::Texture::loadFromFile (string path, SDL_Renderer * render)
{
  //Get rid of preexisting texture
  free();
  renderer = render;

  //Load image at specified path
  SDL_Surface * loadedSurface = IMG_Load(path.c_str());
  if(loadedSurface == NULL)
  {
   cout << "Unable to load image " <<path.c_str() << "! SDL_image Error: " <<  IMG_GetError() << endl;
   return 0;
  }
  return createTextureFromSurface(loadedSurface);
}

bool Views::Texture::loadFromRenderedText(string textureText, SDL_Renderer * render, TTF_Font * font, SDL_Color textColor)
{
  //Get rid of preexisting texture
  free();
  renderer = render;

  //Render text surface
  SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
  if(textSurface == NULL)
  {
   printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
   return 0;
  }

  //Create texture from surface pixels
  return createTextureFromSurface(textSurface);
}

void Views::Texture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
  //Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, width, height };

  //Set clip rendering dimensions
  if( clip != NULL )
  {
   renderQuad.w = clip->w;
   renderQuad.h = clip->h;
  }

  //Render to screen
  SDL_RenderCopyEx (renderer, texture, clip, &renderQuad, angle, center, flip);
}

int Views::Texture::getWidth()
{
  return width;
}

int Views::Texture::getHeight()
{
  return height;
}


