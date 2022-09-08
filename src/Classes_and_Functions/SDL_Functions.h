void frameCap(int fps, Uint32 startingTick) {
    if (1000/fps > SDL_GetTicks() - startingTick) {
        SDL_Delay(1000/fps - (SDL_GetTicks() - startingTick));
    }
}

