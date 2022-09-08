#ifndef SDL_CLASSES_H
#define SDL_CLASSES_H

#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>

class win {
    public:
        int w;
        int h;
        SDL_Window* window = NULL;
        SDL_Renderer* renderer= NULL;
        SDL_Color renderColor = {0, 0, 0, 0xFF};
        
        win() {}
        win(int widthVal, int heightVal) {
            w = widthVal;
            h = heightVal;
            window = SDL_CreateWindow("No Title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
        win(const char* title, int widthVal, int heightVal) {
            w = widthVal;
            h = heightVal;
            window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }

        void setWindowSize(int widthVal, int heightVal) {
            w = widthVal;
            h = heightVal;
            SDL_SetWindowSize(window, w, h);
        }

        SDL_Texture* createSurfaceTexture(int widthVal, int heightVal, int r = 255, int g = 255, int b = 255, int a = 0xFF) {
            SDL_Surface* surface = SDL_CreateRGBSurface(0, widthVal, heightVal, 32, 0, 0, 0, 0);
            SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, r, g, b, a));
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            return texture;
        }

        auto loadImage() {
            textureImage newTexture;
            return newTexture;
        }
        auto loadImage(const char* file, int widthVal = 0, int heightVal = 0, int xPos = 0, int yPos = 0) {
            textureImage newTexture(renderer, file, widthVal, heightVal, xPos, yPos);
            return newTexture;
        }

        auto loadText() {
            textureText newTexture;
            return newTexture;
        }
        auto loadText(const char* fontFile, const char* text, int fontSizeVal, int xPos = 0, int yPos = 0) {
            textureText newTexture(renderer, fontFile, text, fontSizeVal, xPos, yPos);
            return newTexture;
        }

        void clearRender() {
            SDL_SetRenderDrawColor(renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
            SDL_RenderClear(renderer);
        }
        void drawLine(int x1, int y1, int x2, int y2, SDL_Color* color, float xScale = 1, float yScale = 1) {
            if (color == NULL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            }
            SDL_RenderSetScale(renderer, xScale, yScale);
            SDL_RenderDrawLine(renderer, x1/xScale, y1/yScale, x2/xScale, y2/yScale);
            SDL_RenderSetScale(renderer, 1, 1);
        }
        void drawRect(SDL_Color* color, SDL_Rect rect, float xScale = 1, float yScale = 1) {
            if (color == NULL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            }
            SDL_RenderSetScale(renderer, xScale, yScale);
            rect.x /= xScale;
            rect.y /= yScale;
            rect.w /= xScale;
            rect.h /= yScale;
            SDL_RenderDrawRect(renderer, &rect);
            SDL_RenderSetScale(renderer, 1, 1);
        }
        void drawRect(SDL_Color* color, float xScale = 1, float yScale = 1) {
            if (color == NULL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            }
            SDL_RenderSetScale(renderer, xScale, yScale);
            SDL_RenderDrawRect(renderer, NULL);
            SDL_RenderSetScale(renderer, 1, 1);
        }
        void fillRect(SDL_Color* color, SDL_Rect rect, float xScale = 1, float yScale = 1) {
            if (color == NULL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            }
            SDL_RenderSetScale(renderer, xScale, yScale);
            rect.x /= xScale;
            rect.y /= yScale;
            rect.w /= xScale;
            rect.h /= yScale;
            SDL_RenderFillRect(renderer, &rect);
            SDL_RenderSetScale(renderer, 1, 1);
        }
        void fillRect(SDL_Color* color, float xScale = 1, float yScale = 1) {
            if (color == NULL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
            }
            SDL_RenderSetScale(renderer, xScale, yScale);
            SDL_RenderFillRect(renderer, NULL);
            SDL_RenderSetScale(renderer, 1, 1);
        }

    private:
        struct textureImage {
            SDL_Texture* texture = NULL;
            SDL_Rect originalRect;
            SDL_Rect newRect;

            textureImage() {}
            textureImage(SDL_Renderer* renderer, const char* file) {
                SDL_Surface* imageSurface = IMG_Load(file);
                texture = SDL_CreateTextureFromSurface(renderer, imageSurface);

                originalRect.x = 0;
                originalRect.y = 0;
                originalRect.w = imageSurface->w;
                originalRect.h = imageSurface->h;
                originalRect.makeDimensions();
                newRect.x = 0;
                newRect.y = 0;
                newRect.w = imageSurface->w;
                newRect.h = imageSurface->h;
                newRect.makeDimensions();
                            
                SDL_FreeSurface(imageSurface);
            }
            textureImage(SDL_Renderer* renderer, const char* file, int widthVal, int heightVal, int xPos = 0, int yPos = 0) {
                SDL_Surface* imageSurface = IMG_Load(file);
                if (imageSurface == NULL) {
                    std::cout << "image was not loaded" << std::endl;
                }
                texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
                if (texture == NULL) {
                    std::cout << "image was not loaded" << std::endl;
                }


                originalRect.x = 0;
                originalRect.y = 0;
                originalRect.w = imageSurface->w;
                originalRect.h = imageSurface->h;
                originalRect.makeDimensions();
                newRect.x = xPos;
                newRect.y = yPos;
                newRect.w = widthVal;
                newRect.h = heightVal;
                newRect.makeDimensions();

                SDL_FreeSurface(imageSurface);
            }

            void setup(SDL_Renderer* renderer, const char* file, int widthVal, int heightVal, int xPos = 0, int yPos = 0) {
                SDL_Surface* imageSurface = IMG_Load(file);
                texture = SDL_CreateTextureFromSurface(renderer, imageSurface);

                originalRect.x = 0;
                originalRect.y = 0;
                originalRect.w = imageSurface->w;
                originalRect.h = imageSurface->h;
                originalRect.makeDimensions();
                newRect.x = xPos;
                newRect.y = yPos;
                newRect.w = widthVal;
                newRect.h = heightVal;
                newRect.makeDimensions();

                SDL_FreeSurface(imageSurface);
            }

            void reSize(int wVal, int hVal) {
                newRect.w = wVal;
                newRect.h = hVal;
                newRect.makeDimensions();
            }

            void changePos(int xPos, int yPos) {
                newRect.x = xPos;
                newRect.y = yPos;
                newRect.makeDimensions();
            }

            void changePosAndSize(int wVal, int hVal, int xPos, int yPos) {
                newRect.x = xPos;
                newRect.y = yPos;
                newRect.w = wVal;
                newRect.h = hVal;
                newRect.makeDimensions();
            }

            void render(SDL_Renderer* renderer) {
                SDL_RenderCopy(renderer, texture, &originalRect, &newRect);
            }
        };

        struct textureText {
            TTF_Font* font = NULL;
            SDL_Texture* texture = NULL;
            SDL_Rect rect;
            SDL_Color color = {0, 0, 0, 0xFF};
            int fontSize = 10;
            const char* fontFile;
            std::string text = "";

            textureText() {}
            textureText(SDL_Renderer* renderer, const char* fontFileVal, const char* textInput, int fontSizeVal, int xPos = 0, int yPos = 0) {
                text = textInput;
                fontSize = fontSizeVal;
                fontFile = fontFileVal;
                font = TTF_OpenFont(fontFile, fontSize);                
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
                texture = SDL_CreateTextureFromSurface(renderer, textSurface);

                rect.x = xPos;
                rect.y = yPos;
                if (text != "") {
                    rect.w = textSurface->w;
                    rect.h = textSurface->h;
                    rect.makeDimensions();
                }

                SDL_FreeSurface(textSurface);
            }

            void setup(SDL_Renderer* renderer, const char* fontFileVal, const char* textInput, int fontSizeVal, int xPos = 0, int yPos = 0) {
                text = textInput;
                fontSize = fontSizeVal;
                fontFile = fontFileVal;
                font = TTF_OpenFont(fontFile, fontSize);
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
                texture = SDL_CreateTextureFromSurface(renderer, textSurface);

                rect.x = xPos;
                rect.y = yPos;
                if (text != "") {
                    rect.w = textSurface->w;
                    rect.h = textSurface->h;
                    rect.makeDimensions();
                }

                SDL_FreeSurface(textSurface);
            }

            void reSize(int fontSizeVal) {
                if (font != NULL) {
                    fontSize = fontSizeVal;
                    font = TTF_OpenFont(fontFile, fontSize);
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
                    
                    if (text != "") {
                        rect.w = textSurface->w;
                        rect.h = textSurface->h;
                        rect.makeDimensions();
                    }

                    SDL_FreeSurface(textSurface);
                }
                else {
                    std::cout << "Font is NULL" << std::endl;
                }
            }

            void changePos(int xPos, int yPos) {
                rect.x = xPos;
                rect.y = yPos;
                rect.makeDimensions();
            }

            void changePosAndSize(int fontSizeVal, int xPos, int yPos) {
                if (font != NULL) {
                    fontSize = fontSizeVal;
                    font = TTF_OpenFont(fontFile, fontSize);
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
                    
                    rect.x = xPos;
                    rect.y = yPos;
                    if (text != "") {
                        rect.w = textSurface->w;
                        rect.h = textSurface->h;
                        rect.makeDimensions();
                    }

                    SDL_FreeSurface(textSurface);
                }
                else {
                    std::cout << "Font is NULL" << std::endl;
                }
            }

            void render(SDL_Renderer* renderer) {
                SDL_RenderCopy(renderer, texture, NULL, &rect);
            }

            void createTexture(SDL_Renderer* renderer) {
                if (font != NULL) {
                    texture = NULL;
                    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
                    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    
                    if (text != "") {
                        rect.w = textSurface->w;
                        rect.h = textSurface->h;
                        rect.makeDimensions();
                    }

                    SDL_FreeSurface(textSurface);
                }
                else {
                    std::cout << "Font is NULL" << std::endl;
                }
            }

            void changColor(int r, int g, int b, int a = 255) {
                color.r = r;
                color.g = g;
                color.b = b;
                color.a = a;
            }

            void changeFont(const char* fontFileVal) {
                font = NULL;
                fontFile = fontFileVal;
                font = TTF_OpenFont(fontFile, fontSize);
            }

        };
};

class file {
    public:
        std::fstream fileStream;
        std::string fileName;

        file() {}
        file(std::string fileNameVal) {
            fileName = fileNameVal;
            fileStream.open(fileName, std::ios::in);
            if (fileStream.is_open()) {
                format();
            }
            fileStream.close();
        }

        void getFile(std::string fileNameVal) {
            fileName = fileNameVal;
            fileStream.open(fileName, std::ios::in);
            if (fileStream.is_open()) {
                format();
            }
            fileStream.close();
        }

        void createFile() {
            fileStream.open(fileName, std::ios::out);
            fileStream.close();
        }

        void writeFile(std::string inputVars) {
            fileStream.open(fileName, std::ios::out);
            if (fileStream.is_open()) {
                fileStream << inputVars;                
            }
            fileStream.close();
        }

        void appendFile(std::string inputVars) {
            fileStream.open(fileName, std::ios::app);
            if (fileStream.is_open()) {
                fileStream << inputVars;                
            }
            fileStream.close();
        }

        bool checkLoaded() {
            fileStream.open(fileName, std::ios::in);
            bool check = fileStream.is_open();
            fileStream.close();
            return check;
        }

        int getIntVar(std::string varName) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    if (var == varName) {
                        int whichType = checkType(type);
                        if (whichType == 1 || whichType == 3) { // int
                            int makeInt = stoi(type);
                            fileStream.close();
                            return makeInt;
                        }                        
                    }
                }              
            }
            fileStream.close();
            return 0;
        }

        double getDoubleVar(std::string varName) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    if (var == varName) {
                        int whichType = checkType(type);
                        if (whichType == 1 || whichType == 3) { // int
                            double makedouble = stod(type);
                            fileStream.close();
                            return makedouble;
                        }                        
                    }
                }              
            }
            fileStream.close();
            return 0;
        }

        char getCharVar(std::string varName) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    if (var == varName) {
                        int whichType = checkType(type);
                        if (whichType == 1 || whichType == 2) { // char
                            char makeChar = type[0];
                            fileStream.close();
                            return makeChar;
                        }
                    }
                }              
            }
            fileStream.close();
            return 0;
        }

        std::string getStringVar(std::string varName) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    if (var == varName) {
                        fileStream.close();
                        return type;
                    }
                }              
            }
            fileStream.close();
            return 0;
        }

        bool getBoolVar(std::string varName) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    if (var == varName) {
                        bool makeBool;
                        if (type == "true") {
                            makeBool = true;
                        }
                        else {
                            makeBool = false;
                        }
                        fileStream.close();
                        return makeBool;
                    }
                }
            }
            fileStream.close();
            return 0;
        }

        void updateVar(std::string varName, int intVal) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            std::string contents;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    var = findInvalidVar(var);
                    contents.append(var);
                    contents.append(" ");
                    if (var == varName) {
                        contents.append(std::to_string(intVal));
                    }
                    else {
                        contents.append(type);
                    }
                    contents.append("\n");
                }
            }
            contents.pop_back();
            fileStream.close();
            fileStream.open(fileName, std::ios::out);
            fileStream << contents;
            fileStream.close();
        }
        void updateVar(std::string varName, double doubleVal) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            std::string contents;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    var = findInvalidVar(var);
                    contents.append(var);
                    contents.append(" ");
                    if (var == varName) {
                        std::string temp = checkZeros(std::to_string(doubleVal)); //6 decimal places
                        contents.append(temp);
                    }
                    else {
                        contents.append(type);
                    }
                    contents.append("\n");
                }
            }
            contents.pop_back();
            fileStream.close();
            fileStream.open(fileName, std::ios::out);
            fileStream << contents;
            fileStream.close();
        }
        void updateVar(std::string varName, char charVal) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            std::string contents;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    var = findInvalidVar(var);
                    contents.append(var);
                    contents.append(" ");
                    if (var == varName) {
                        contents.append(1, charVal);
                    }
                    else {
                        contents.append(type);
                    }
                    contents.append("\n");
                }
            }
            contents.pop_back();
            fileStream.close();
            fileStream.open(fileName, std::ios::out);
            fileStream << contents;
            fileStream.close();
        }
        void updateVar(std::string varName, std::string stringVal) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            std::string contents;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    var = findInvalidVar(var);
                    contents.append(var);
                    contents.append(" ");
                    if (var == varName) {
                        contents.append(stringVal);
                    }
                    else {
                        contents.append(type);
                    }
                    contents.append("\n");
                }
            }
            contents.pop_back();
            fileStream.close();
            fileStream.open(fileName, std::ios::out);
            fileStream << contents;
            fileStream.close();
        }

        void updateVar(std::string varName, bool boolVal) {
            fileStream.open(fileName, std::ios::in);
            std::string var;
            std::string type;
            std::string contents;
            if (fileStream.is_open()) {
                while (fileStream >> var >> type) {
                    var = findInvalidVar(var);
                    contents.append(var);
                    contents.append(" ");
                    if (var == varName) {
                        if (boolVal) {
                            contents.append("true");
                        }
                        else {
                            contents.append("false");
                        }
                    }
                    else {
                        contents.append(type);
                    }
                    contents.append("\n");
                }
            }
            contents.pop_back();
            fileStream.close();
            fileStream.open(fileName, std::ios::out);
            fileStream << contents;
            fileStream.close();
        }
        
        std::string boolToString(bool booleanVal) {
            return booleanVal ? "true" : "false";
        }

    private:
        void format() {
            int i = 0;
            std::string var;
            std::string type;
            std::string contents;
            while (fileStream >> var >> type) {
                var = findInvalidVar(var);
                contents.append(var);
                contents.append(" ");
                contents.append(type);
                contents.append("\n");
            }
            contents.pop_back();
            fileStream.close();
            fileStream.open(fileName, std::ios::out);
            fileStream << contents;
            fileStream.close();
        }

        std::string findInvalidVar(std::string input) {
            for (int i = 0; i < input.length(); ++i) {
                if ('0' <= input[0] && input[0] <= '9') {
                    input.erase(0, 1);
                }

                if (('!' <= input[i] && input[i] <= '/') || (':' <= input[i] && input[i] <= '@') || ('[' <= input[i] && input[i] <= '`') || ('{' <= input[i] && input[i] <= '~')) {
                    input.erase(i, 1);
                    --i;
                }
            }

            return input;
        }

        int checkType(std::string type) {
            bool checkDecimal = false;

            if (type.length() == 1) { //check single int or char
                if ('0' <= type[0] && type[0] <= '9') {
                    return 1; //int
                }
                else {
                    return 2; //char
                }
            }

            for (int i = 0; i < type.length(); ++i) {
                if ('0' <= type[i] && type[i] <= '9') {
                    // pass
                }
                else if (type[i] == '.') {
                    if (checkDecimal) {
                        return 4; //string
                    }
                    checkDecimal = true;
                }
                else {
                    return 4; // string
                }
            }

            if (checkDecimal) {
                return 3; // double
            }
            else {
                return 1;
            }

            return 0;
        }

        std::string checkZeros(std::string input) {
            bool check = true;
            int stringLength = input.length();
            while (check) {
                if (input[stringLength-1] == '0' && input[stringLength-2] != '.') {
                    input.pop_back();
                    --stringLength;
                }
                else {
                    check = false;
                }
            }
            return input;
        }
};

#endif