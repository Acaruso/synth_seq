#include "graphics_wrapper.hpp"

#include <iostream>

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "src/main/graphics/color.hpp"

GraphicsWrapper::GraphicsWrapper()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL_Error: %s\n", SDL_GetError());
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("SDL_Error: %s\n", SDL_GetError());
    }

    if (TTF_Init() < 0) {
        printf("SDL_Error: %s\n", SDL_GetError());
    }

    this->window = SDL_CreateWindow(
        this->windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        this->screenWidth,
        this->screenHeight,
        SDL_WINDOW_SHOWN
    );

    if (this->window == NULL) {
        printf("SDL_Error: %s\n", SDL_GetError());
    }

    this->windowSurface = SDL_GetWindowSurface(this->window);

    if (this->windowSurface == NULL) {
        printf("SDL_Error: %s\n", SDL_GetError());
    }

    this->windowRenderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

    if (this->windowRenderer == NULL) {
        printf("SDL_Error: %s\n", SDL_GetError());
    }

    drawSystem = DrawSystem(windowRenderer);
}

void GraphicsWrapper::clearWindow()
{
    SDL_SetRenderDrawColor(this->windowRenderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderClear(this->windowRenderer);
}

void GraphicsWrapper::drawRect(Rect rect)
{
    drawSystem.push(DrawRectCommand(rect));
}

void GraphicsWrapper::drawImage(std::string name, Coord coord)
{
    drawSystem.push(DrawImageCommand(this->images[name], coord));
}

void GraphicsWrapper::drawText(std::string text, std::string fontName, Coord coord)
{
    drawSystem.push(DrawTextCommand(text, this->fonts[fontName], coord));
}

void GraphicsWrapper::drawLine(Coord begin, Coord end)
{
    drawSystem.push(DrawLineCommand(begin, end));
}

void GraphicsWrapper::loadImage(std::string name, std::string path)
{
    SDL_Surface* buffer = IMG_Load(path.c_str());

    if (!buffer) {
        std::string err = SDL_GetError();
        throw std::runtime_error("SDL ERROR " + err + ": getImage IMG_Load");
    }

    Image image;
    image.p_texture = SDL_CreateTextureFromSurface(this->windowRenderer, buffer);
    image.w = buffer->w;
    image.h = buffer->h;

    if (!image.p_texture) {
        std::string err = SDL_GetError();
        throw std::runtime_error("SDL ERROR " + err + ": getImage SDL_CreateTextureFromSurface");
    }

    SDL_FreeSurface(buffer);

    this->images[name] = image;
}

void GraphicsWrapper::loadFont(
    std::string name,
    std::string path,
    int height,
    int width,
    int lineHeight
) {
    FC_Font* p_font = FC_CreateFont();

    if (p_font == NULL) {
        throw std::runtime_error("SDL ERROR: loadFont FC_CreateFont");
    }

    Uint8 res = FC_LoadFont(
        p_font,
        this->windowRenderer,
        path.c_str(),
        height,
        FC_MakeColor(textColor.r, textColor.g, textColor.b, 255),
        TTF_STYLE_NORMAL
    );

    if (res != 1) {
        throw std::runtime_error("SDL ERROR: loadFont FC_LoadFont");
    }

    this->fonts[name] = Font(p_font, height, width, lineHeight);
}

Font& GraphicsWrapper::getFont(std::string name)
{
    return this->fonts[name];
}

void GraphicsWrapper::render()
{
    drawSystem.draw();
    SDL_RenderPresent(windowRenderer);
}

void GraphicsWrapper::destroyWindow()
{
    SDL_DestroyWindow(this->window);
}

void GraphicsWrapper::quit()
{
    SDL_Quit();
}

