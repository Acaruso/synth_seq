#include "input_system.hpp"

#include <iostream>

void InputSystem::run()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_QUIT:
            uiState.quit = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouse_button_down(event);
            break;
        case SDL_MOUSEBUTTONUP:
            mouse_button_up(event);
            break;
        case SDL_MOUSEMOTION:
            mouse_motion(event);
            break;
        case SDL_KEYDOWN:
            keydown(event);
            // std::cout << uiState.c << std::endl;
            break;
        case SDL_KEYUP:
            keyup(event);
            // std::cout << uiState.c << std::endl;
            break;
        }
    }
}

void InputSystem::mouse_button_down(SDL_Event& event)
{
    uiState.click = true;
}

void InputSystem::mouse_button_up(SDL_Event& event)
{
    uiState.click = false;
    uiState.curEltId = 0;
}

void InputSystem::mouse_motion(SDL_Event& event)
{
    uiState.mousePos.x = event.motion.x;
    uiState.mousePos.y = event.motion.y;
}

void InputSystem::keydown(SDL_Event& event)
{
    uiState.keydown_event = true;

    switch (event.key.keysym.sym) {
    case SDLK_a:
        uiState.a = true;
        break;
    case SDLK_b:
        uiState.b = true;
        break;
    case SDLK_c:
        uiState.c = true;
        break;
    case SDLK_d:
        uiState.d = true;
        break;
    case SDLK_e:
        uiState.e = true;
        break;
    case SDLK_f:
        uiState.f = true;
        break;
    case SDLK_g:
        uiState.g = true;
        break;
    case SDLK_h:
        uiState.h = true;
        break;
    case SDLK_i:
        uiState.i = true;
        break;
    case SDLK_j:
        uiState.j = true;
        break;
    case SDLK_k:
        uiState.k = true;
        break;
    case SDLK_l:
        uiState.l = true;
        break;
    case SDLK_m:
        uiState.m = true;
        break;
    case SDLK_n:
        uiState.n = true;
        break;
    case SDLK_o:
        uiState.o = true;
        break;
    case SDLK_p:
        uiState.p = true;
        break;
    case SDLK_q:
        uiState.q = true;
        break;
    case SDLK_r:
        uiState.r = true;
        break;
    case SDLK_s:
        uiState.s = true;
        break;
    case SDLK_t:
        uiState.t = true;
        break;
    case SDLK_u:
        uiState.u = true;
        break;
    case SDLK_v:
        uiState.v = true;
        break;
    case SDLK_w:
        uiState.w = true;
        break;
    case SDLK_x:
        uiState.x = true;
        break;
    case SDLK_y:
        uiState.y = true;
        break;
    case SDLK_z:
        uiState.z = true;
        break;

    case SDLK_UP:
        uiState.up = true;
        break;
    case SDLK_DOWN:
        uiState.down = true;
        break;
    case SDLK_LEFT:
        uiState.left = true;
        break;
    case SDLK_RIGHT:
        uiState.right = true;
        break;

    case SDLK_SPACE:
        uiState.space = true;
        break;
    case SDLK_LSHIFT:
        uiState.lshift = true;
        break;
    case SDLK_LCTRL:
        uiState.lctrl = true;
        break;
    case SDLK_LALT:
        uiState.lalt = true;
        break;
    case SDLK_TAB:
        uiState.tab = true;
        break;
    case SDLK_1:
        uiState.i1 = true;
        break;
    case SDLK_2:
        uiState.i2 = true;
        break;
    case SDLK_3:
        uiState.i3 = true;
        break;
    case SDLK_4:
        uiState.i4 = true;
        break;
    case SDLK_5:
        uiState.i5 = true;
        break;
    case SDLK_6:
        uiState.i6 = true;
        break;
    case SDLK_ESCAPE:
        uiState.esc = true;
        break;
    }
}

void InputSystem::keyup(SDL_Event& event)
{
    switch (event.key.keysym.sym) {
    case SDLK_a:
        uiState.a = false;
        break;
    case SDLK_b:
        uiState.b = false;
        break;
    case SDLK_c:
        uiState.c = false;
        break;
    case SDLK_d:
        uiState.d = false;
        break;
    case SDLK_e:
        uiState.e = false;
        break;
    case SDLK_f:
        uiState.f = false;
        break;
    case SDLK_g:
        uiState.g = false;
        break;
    case SDLK_h:
        uiState.h = false;
        break;
    case SDLK_i:
        uiState.i = false;
        break;
    case SDLK_j:
        uiState.j = false;
        break;
    case SDLK_k:
        uiState.k = false;
        break;
    case SDLK_l:
        uiState.l = false;
        break;
    case SDLK_m:
        uiState.m = false;
        break;
    case SDLK_n:
        uiState.n = false;
        break;
    case SDLK_o:
        uiState.o = false;
        break;
    case SDLK_p:
        uiState.p = false;
        break;
    case SDLK_q:
        uiState.q = false;
        break;
    case SDLK_r:
        uiState.r = false;
        break;
    case SDLK_s:
        uiState.s = false;
        break;
    case SDLK_t:
        uiState.t = false;
        break;
    case SDLK_u:
        uiState.u = false;
        break;
    case SDLK_v:
        uiState.v = false;
        break;
    case SDLK_w:
        uiState.w = false;
        break;
    case SDLK_x:
        uiState.x = false;
        break;
    case SDLK_y:
        uiState.y = false;
        break;
    case SDLK_z:
        uiState.z = false;
        break;

    case SDLK_UP:
        uiState.up = false;
        break;
    case SDLK_DOWN:
        uiState.down = false;
        break;
    case SDLK_LEFT:
        uiState.left = false;
        break;
    case SDLK_RIGHT:
        uiState.right = false;
        break;

    case SDLK_SPACE:
        uiState.space = false;
        break;
    case SDLK_LSHIFT:
        uiState.lshift = false;
        break;
    case SDLK_LCTRL:
        uiState.lctrl = false;
        break;
    case SDLK_LALT:
        uiState.lalt = false;
        break;
    case SDLK_TAB:
        uiState.tab = false;
        break;
    case SDLK_1:
        uiState.i1 = false;
        break;
    case SDLK_2:
        uiState.i2 = false;
        break;
    case SDLK_3:
        uiState.i3 = false;
        break;
    case SDLK_4:
        uiState.i4 = false;
        break;
    case SDLK_5:
        uiState.i5 = false;
        break;
    case SDLK_6:
        uiState.i6 = false;
        break;
    case SDLK_ESCAPE:
        uiState.esc = false;
        break;
    }
}

void InputSystem::nextState()
{
    prevUiState = uiState;
    // uiState = UiState();
}
