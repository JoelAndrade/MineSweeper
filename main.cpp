#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <Minesweeper_Classes.h>
#include <SDL_Functions.h>
#include <SDL_Classes.h>

//Read save file
file settings("settings.txt");

int blockLength = settings.getIntVar("blockLength");
int xPos;
int yPos;
const int fps = 30;

struct intermediate_struct {
    int row = 16;
    int col = 16;
    int mines = 40;
} intermediate;

struct beginner_struct {
    int row = 9;
    int col = 9;
    int mines = 10;
} beginner;

//Creats window
win window("Loading MineSweeper", blockLength*30, blockLength*16); //1200 by 640
Uint32 startingTick;
SDL_Event event;

field currentField;
field expertField;
field intermediateField(intermediate.row, intermediate.col, intermediate.mines);
field beginnerField(beginner.row, beginner.col, beginner.mines);
field customField(settings.getIntVar("row"), settings.getIntVar("col"), settings.getIntVar("mines"));

enum gameState_enum {
    EXIT, STARTMENU, MAINMENU, CUSTOMMENU, SETTINGSMENU, GAME
};
int gameState = STARTMENU;

enum difficulty_enum {
    BEGINNER, INTERMEDIATE, EXPERT, CUSTOM
};
int difficulty = BEGINNER;

int startMenu();
int mainMenu();
int customMenu();
int settingsMenu();
int game();

struct menuBlock_struct {
    int w = window.w*(0.61 - 0.39) + 1;
    int h = window.h*(0.75 - 0.64) + 1;
} menuBlock;

//Load images
auto startMenuImage = window.loadImage("Images/Title4.jpg", window.w, window.h);
auto pressedPlay = window.loadImage("Images/Play2.png", menuBlock.w, menuBlock.h, window.w*0.39, window.h*0.55625); //windowWidth*0.39, windowHeight*0.64
auto pressedExit = window.loadImage("Images/Exit2.png", menuBlock.w, menuBlock.h, window.w*0.39, window.h*0.69375);

auto mainMenuImage = window.loadImage("Images/Title5.jpg", window.w, window.h);
auto pressedBeginner = window.loadImage("Images/Beginner2.png", menuBlock.w, menuBlock.h, window.w*0.2642, window.h*0.5);
auto pressedIntermediate = window.loadImage("Images/Intermediate2.png", menuBlock.w, menuBlock.h, window.w*0.2642, window.h*0.6391);
auto pressedExpert = window.loadImage("Images/Expert2.png", menuBlock.w, menuBlock.h, window.w*0.2642, window.h*0.778125);
auto pressedCustom = window.loadImage("Images/Custom2.png", menuBlock.w, menuBlock.h, window.w*0.5158, window.h*0.5);
auto pressedSettings = window.loadImage("Images/Settings2.png", menuBlock.w, menuBlock.h, window.w*0.5158, window.h*0.6391);
auto pressedBack = window.loadImage("Images/Back2.png", menuBlock.w, menuBlock.h, window.w*0.5158, window.h*0.778125);

auto customMenuImage = window.loadImage("Images/Title.jpg", window.w, window.h);
auto playButton = window.loadImage("Images/Play.png", menuBlock.w, menuBlock.h, window.w*0.35, window.h*0.83); // 0.23 | 0.8
auto backButton = window.loadImage("Images/Back.png", menuBlock.w, menuBlock.h, window.w*0.65, window.h*0.83); // 0.52 | 0.8
auto blankBlock = window.loadImage("Images/blankBlock.png");
auto rowText = window.loadText();
auto rowValText = window.loadText();
auto colText = window.loadText();
auto colValText = window.loadText();
auto minesText = window.loadText();
auto minesValText = window.loadText();

SDL_Color color = {255, 0, 0};
SDL_Rect rowRect;
SDL_Rect colRect;
SDL_Rect minesRect;

bool darkMode = settings.getBoolVar("mode");
auto lightButton = window.loadImage("Images/Light Theme.jpg", menuBlock.w*0.85, menuBlock.h*0.85);
auto pressedLight = window.loadImage("Images/Light Theme 2.jpg", menuBlock.w*0.85, menuBlock.h*0.85);
auto darkButton = window.loadImage("Images/Dark Theme.jpg", menuBlock.w*0.85, menuBlock.h*0.85);
auto pressedDark = window.loadImage("Images/Dark Theme 2.jpg", menuBlock.w*0.85, menuBlock.h*0.85);
auto applyButton = window.loadImage("Images/Apply.png", menuBlock.w, menuBlock.h, window.w*0.35, window.h*0.83);
auto pressedApply = window.loadImage("Images/Apply2.png", menuBlock.w, menuBlock.h, window.w*0.35, window.h*0.83);
auto blockLengthText = window.loadText();
auto blockLengthValText = window.loadText();
auto modeText = window.loadText();

auto oneBlock = window.loadImage("Images/1.png", blockLength, blockLength);
auto twoBlock = window.loadImage("Images/2.png", blockLength, blockLength);
auto threeBlock = window.loadImage("Images/3.png", blockLength, blockLength);
auto fourBlock = window.loadImage("Images/4.png", blockLength, blockLength);
auto fiveBlock = window.loadImage("Images/5.png", blockLength, blockLength);
auto sixBlock = window.loadImage("Images/6.png", blockLength, blockLength);
auto sevenBlock = window.loadImage("Images/7.png", blockLength, blockLength);
auto eightBlock = window.loadImage("Images/8.png", blockLength, blockLength);
auto coveredBlock = window.loadImage("Images/block.png", blockLength, blockLength);
auto emptyBlock = window.loadImage("Images/EmptyBlock.png", blockLength, blockLength);
auto flagBlock = window.loadImage("Images/flag.png", blockLength, blockLength);
auto mineBlock = window.loadImage("Images/mine.png", blockLength, blockLength);
auto wrongMineBlock = window.loadImage("Images/wrongMine.png", blockLength, blockLength);

auto oneBlockDark = window.loadImage("Images/1dark.png", blockLength, blockLength);
auto twoBlockDark = window.loadImage("Images/2dark.png", blockLength, blockLength);
auto threeBlockDark = window.loadImage("Images/3dark.png", blockLength, blockLength);
auto fourBlockDark = window.loadImage("Images/4dark.png", blockLength, blockLength);
auto fiveBlockDark = window.loadImage("Images/5dark.png", blockLength, blockLength);
auto sixBlockDark = window.loadImage("Images/6dark.png", blockLength, blockLength);
auto sevenBlockDark = window.loadImage("Images/7dark.png", blockLength, blockLength);
auto eightBlockDark = window.loadImage("Images/8dark.png", blockLength, blockLength);
auto coveredBlockDark = window.loadImage("Images/blockDark.png", blockLength, blockLength);
auto emptyBlockDark = window.loadImage("Images/EmptyBlockDark.png", blockLength, blockLength);
auto flagBlockDark = window.loadImage("Images/flagDark.png", blockLength, blockLength);
auto mineBlockDark = window.loadImage("Images/mineDark.png", blockLength, blockLength);
auto wrongMineBlockDark = window.loadImage("Images/wrongMineDark.png", blockLength, blockLength);

//Sets the game title
void setGameTitle(const char* middleTitle) {
    std::string title = "Number of Mines: " + std::to_string(currentField.mines - currentField.flags);
    
    int leftLength = title.length() + 4;
    int middleLength = strlen(middleTitle);
    int numSpaces = 12*(window.w/(2*blockLength) + 1) - 2*leftLength - middleLength;
    int i;
    char spaces[200];
    for (i = 0; i < numSpaces; ++i) {
        spaces[i] = ' ';
    }
    spaces[i] = '\0';

    title.append(spaces);
    title.append(middleTitle);
    const char* newTitle = title.c_str();

    SDL_SetWindowTitle(window.window, newTitle);
}

//Takes mouse coordinates and turns them to the field index
void pos2Index(int* i, int* j) {
    *i = (yPos / blockLength) + 1;
    *j = (xPos / blockLength) + 1;
}

// Saves settings when exiting
void save() {
    settings.updateVar("blockLength", blockLength);
    if (rowValText.text != "") {
        settings.updateVar("row", rowValText.text);
    }
    if (colValText.text != "") {
        settings.updateVar("col", colValText.text);
    }
    if (minesValText.text != "") {
        settings.updateVar("mines", minesValText.text);
    }
    settings.updateVar("mode", darkMode);
}

// resizes window
void reSizeEverthing() {
    window.w = blockLength*30;
    window.h = blockLength*16;
    menuBlock.w = window.w*(0.61 - 0.39) + 1;
    menuBlock.h = window.h*(0.75 - 0.64) + 1;
    int fontSize = blockLength*0.875;

    startMenuImage.reSize(window.w, window.h);
    pressedPlay.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.39, window.h*0.55625); //windowWidth*0.39, windowHeight*0.64
    pressedExit.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.39, window.h*0.69375);

    mainMenuImage.reSize(window.w, window.h);
    pressedBeginner.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.2642, window.h*0.5);
    pressedIntermediate.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.2642, window.h*0.6391);
    pressedExpert.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.2642, window.h*0.778125);
    pressedCustom.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.5158, window.h*0.5);
    pressedSettings.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.5158, window.h*0.6391);
    pressedBack.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.5158, window.h*0.778125);

    customMenuImage.reSize(window.w, window.h);
    playButton.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.35, window.h*0.83); // 0.23 | 0.8
    backButton.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.65, window.h*0.83); // 0.52 | 0.8
    rowText.changePosAndSize(fontSize, window.w*0.30, window.h*0.48);
    rowValText.changePosAndSize(fontSize, window.w*0.65, window.h*0.48);
    colText.changePosAndSize(fontSize, window.w*0.30, window.h*0.60);
    colValText.changePosAndSize(fontSize, window.w*0.65, window.h*0.60);
    minesText.changePosAndSize(fontSize, window.w*0.30, window.h*0.72);
    minesValText.changePosAndSize(fontSize, window.w*0.65, window.h*0.72);

    lightButton.reSize(menuBlock.w*0.85, menuBlock.h*0.85);
    pressedLight.reSize(menuBlock.w*0.85, menuBlock.h*0.85);
    darkButton.reSize(menuBlock.w*0.85, menuBlock.h*0.85);
    pressedDark.reSize(menuBlock.w*0.85, menuBlock.h*0.85);
    applyButton.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.35, window.h*0.83);
    pressedApply.changePosAndSize(menuBlock.w, menuBlock.h, window.w*0.35, window.h*0.83);
    blockLengthText.changePosAndSize(fontSize, window.w*0.30, window.h*0.48);
    blockLengthValText.changePosAndSize(fontSize, window.w*0.65, window.h*0.48);
    modeText.changePosAndSize(fontSize, window.w*0.30, window.h*0.60);

    oneBlock.reSize(blockLength, blockLength);
    twoBlock.reSize(blockLength, blockLength);
    threeBlock.reSize(blockLength, blockLength);
    fourBlock.reSize(blockLength, blockLength);
    fiveBlock.reSize(blockLength, blockLength);
    sixBlock.reSize(blockLength, blockLength);
    sevenBlock.reSize(blockLength, blockLength);
    eightBlock.reSize(blockLength, blockLength);
    coveredBlock.reSize(blockLength, blockLength);
    emptyBlock.reSize(blockLength, blockLength);
    flagBlock.reSize(blockLength, blockLength);
    mineBlock.reSize(blockLength, blockLength);
    wrongMineBlock.reSize(blockLength, blockLength);

    oneBlockDark.reSize(blockLength, blockLength);
    twoBlockDark.reSize(blockLength, blockLength);
    threeBlockDark.reSize(blockLength, blockLength);
    fourBlockDark.reSize(blockLength, blockLength);
    fiveBlockDark.reSize(blockLength, blockLength);
    sixBlockDark.reSize(blockLength, blockLength);
    sevenBlockDark.reSize(blockLength, blockLength);
    eightBlockDark.reSize(blockLength, blockLength);
    coveredBlockDark.reSize(blockLength, blockLength);
    emptyBlockDark.reSize(blockLength, blockLength);
    flagBlockDark.reSize(blockLength, blockLength);
    mineBlockDark.reSize(blockLength, blockLength);
    wrongMineBlockDark.reSize(blockLength, blockLength);

    playButton.newRect.shiftX();
    backButton.newRect.shiftX();
    rowRect.x = rowValText.rect.x - blockLength*0.25;
    rowRect.y = rowValText.rect.y - blockLength*0.25;
    rowRect.w = blockLength*1.55;
    rowRect.h = blockLength*1.55;
    rowRect.makeDimensions();
    colRect.x = colValText.rect.x - blockLength*0.25;
    colRect.y = colValText.rect.y - blockLength*0.25;
    colRect.w = blockLength*1.55;
    colRect.h = blockLength*1.55;
    colRect.makeDimensions();
    minesRect.x = minesValText.rect.x - blockLength*0.25;
    minesRect.y = minesValText.rect.y - blockLength*0.25;
    minesRect.w = blockLength*1.55;
    minesRect.h = blockLength*1.55;
    minesRect.makeDimensions();

    lightButton.newRect.x = colRect.x;
    lightButton.newRect.y = colRect.y;
    lightButton.newRect.makeDimensions();
    pressedLight.newRect.x = colRect.x;
    pressedLight.newRect.y = colRect.y;
    pressedLight.newRect.makeDimensions();
    darkButton.newRect.x = colRect.x;
    darkButton.newRect.y = colRect.y;
    darkButton.newRect.makeDimensions();
    pressedDark.newRect.x = colRect.x;
    pressedDark.newRect.y = colRect.y;
    pressedDark.newRect.makeDimensions();
    lightButton.newRect.shiftX();
    pressedLight.newRect.shiftX();
    darkButton.newRect.shiftX();
    pressedDark.newRect.shiftX();
    applyButton.newRect.shiftX();
    pressedApply.newRect.shiftX();
}

void drawStartMenu(bool hold, bool playRightSpot, bool exitRightSpot) {
    SDL_RenderClear(window.renderer);
    startMenuImage.render(window.renderer);
    if (hold == SDL_PRESSED && (pressedPlay.newRect.leftX - 7 < xPos && xPos < pressedPlay.newRect.rightX + 7) && (pressedPlay.newRect.topY - 7 < yPos && yPos < pressedPlay.newRect.bottomY + 7) && playRightSpot) {
        pressedPlay.render(window.renderer);
    }
    if (hold == SDL_PRESSED && (pressedExit.newRect.leftX - 7 < xPos && xPos < pressedExit.newRect.rightX + 7) && (pressedExit.newRect.topY - 7 < yPos && yPos < pressedExit.newRect.bottomY + 7) && exitRightSpot) {
        pressedExit.render(window.renderer);
    }
    SDL_RenderPresent(window.renderer);
}

void drawMainMenu(bool hold, bool beginnerRightSpot, bool intermediateRightSpot, bool expertRightSpot, bool customRightSpot, bool settingsRightSpot, bool backRightSpot) {
    SDL_RenderClear(window.renderer);
    mainMenuImage.render(window.renderer);
    if (hold == SDL_PRESSED && (pressedBeginner.newRect.x - 7 < xPos && xPos < pressedBeginner.newRect.trX + 7) && (pressedBeginner.newRect.y - 7 < yPos && yPos < pressedBeginner.newRect.brY + 7) && beginnerRightSpot) {
        pressedBeginner.render(window.renderer);
    }
    else if (hold == SDL_PRESSED && (pressedIntermediate.newRect.x - 7 < xPos && xPos < pressedIntermediate.newRect.trX + 7) && (pressedIntermediate.newRect.y - 7 < yPos && yPos < pressedIntermediate.newRect.brY + 7) && intermediateRightSpot) {
        pressedIntermediate.render(window.renderer);
    }
    else if (hold == SDL_PRESSED && (pressedExpert.newRect.x - 7 < xPos && xPos < pressedExpert.newRect.trX + 7) && (pressedExpert.newRect.y - 7 < yPos && yPos < pressedExpert.newRect.brY + 7) && expertRightSpot) {
        pressedExpert.render(window.renderer);
    }
    else if (hold == SDL_PRESSED && (pressedCustom.newRect.x - 7 < xPos && xPos < pressedCustom.newRect.trX + 7) && (pressedCustom.newRect.y - 7 < yPos && yPos < pressedCustom.newRect.brY + 7) && customRightSpot) {
        pressedCustom.render(window.renderer);
    }
    else if (hold == SDL_PRESSED && (pressedSettings.newRect.x - 7 < xPos && xPos < pressedSettings.newRect.trX + 7) && (pressedSettings.newRect.y - 7 < yPos && yPos < pressedSettings.newRect.brY + 7) && settingsRightSpot) {
        pressedSettings.render(window.renderer);
    }
    else if (hold == SDL_PRESSED && (pressedBack.newRect.x - 7 < xPos && xPos < pressedBack.newRect.trX + 7) && (pressedBack.newRect.y - 7 < yPos && yPos < pressedBack.newRect.brY + 7) && backRightSpot) {
        pressedBack.render(window.renderer);
    }
    SDL_RenderPresent(window.renderer);

}

void drawCustomMenu(bool hold, bool rowRightSpot, bool colRightSpot, bool minesRightSpot, bool playRightSpot, bool backRightSpot) {
    SDL_Rect tempRect;

    SDL_RenderClear(window.renderer);
    customMenuImage.render(window.renderer);
    SDL_RenderCopy(window.renderer, playButton.texture, NULL, &applyButton.newRect);
    backButton.render(window.renderer);

    coveredBlock.newRect.x = rowRect.x;
    coveredBlock.newRect.y = rowRect.y;
    coveredBlock.newRect.w = rowRect.w;
    coveredBlock.newRect.h = rowRect.h;
    coveredBlock.render(window.renderer);
    
    coveredBlock.newRect.x = colRect.x;
    coveredBlock.newRect.y = colRect.y;
    coveredBlock.newRect.w = colRect.w;
    coveredBlock.newRect.h = colRect.h;
    coveredBlock.render(window.renderer);

    coveredBlock.newRect.x = minesRect.x;
    coveredBlock.newRect.y = minesRect.y;
    coveredBlock.newRect.w = minesRect.w;
    coveredBlock.newRect.h = minesRect.h;
    coveredBlock.render(window.renderer);

    if (hold == SDL_PRESSED && (playButton.newRect.leftX - 7 < xPos && xPos < playButton.newRect.rightX + 7) && (playButton.newRect.topY - 7 < yPos && yPos < playButton.newRect.bottomY + 7) && playRightSpot) {
        SDL_RenderCopy(window.renderer, pressedPlay.texture, NULL, &applyButton.newRect);
    }
    if (hold == SDL_PRESSED && (backButton.newRect.leftX - 7 < xPos && xPos < backButton.newRect.rightX + 7) && (backButton.newRect.topY - 7 < yPos && yPos < backButton.newRect.bottomY + 7) && backRightSpot) {
        SDL_RenderCopy(window.renderer, pressedBack.texture, NULL, &backButton.newRect);
    }

    if (rowRightSpot || rowValText.text == "") {
        //tempRect = {rowRect.x + 7, rowRect.y + 7, rowRect.w - 14, rowRect.h - 14};
        tempRect.x = rowRect.x + blockLength*0.175;
        tempRect.y = rowRect.y + blockLength*0.175;
        tempRect.w = rowRect.w - blockLength*0.175*2;
        tempRect.h = rowRect.h - blockLength*0.175*2;
        window.fillRect(&color, tempRect);
        //SDL_RenderCopy(window.renderer, whiteSurface, NULL, &tempRect);
    }
    if (colRightSpot || colValText.text == "") {
        //tempRect = {colRect.x + 7, colRect.y + 7, colRect.w - 14, colRect.h - 14};
        tempRect.x = colRect.x + blockLength*0.175;
        tempRect.y = colRect.y + blockLength*0.175;
        tempRect.w = colRect.w - blockLength*0.175*2;
        tempRect.h = colRect.h - blockLength*0.175*2;
        window.fillRect(&color, tempRect);
        //SDL_RenderCopy(window.renderer, whiteSurface, NULL, &tempRect);
    }
    if (minesRightSpot || minesValText.text == "") {
        //tempRect = {minesRect.x + 7, minesRect.y + 7, minesRect.w - 14, minesRect.h - 14};
        tempRect.x = minesRect.x + blockLength*0.175;
        tempRect.y = minesRect.y + blockLength*0.175;
        tempRect.w = minesRect.w - blockLength*0.175*2;
        tempRect.h = minesRect.h - blockLength*0.175*2;
        window.fillRect(&color, tempRect);
        //SDL_RenderCopy(window.renderer, whiteSurface, NULL, &tempRect);
    }

    rowText.render(window.renderer);
    rowValText.render(window.renderer);
    colValText.render(window.renderer);
    colText.render(window.renderer);
    minesText.render(window.renderer);
    minesValText.render(window.renderer);

    SDL_RenderPresent(window.renderer);
}

void drawSettingsMenu(bool hold, bool blockLengthRightSpot, bool modeRightSpot, bool applyRightSpot, bool backRightSpot) {
    SDL_Rect tempRect;

    SDL_RenderClear(window.renderer);
    customMenuImage.render(window.renderer);
    applyButton.render(window.renderer);
    backButton.render(window.renderer);

    coveredBlock.newRect.x = rowRect.x;
    coveredBlock.newRect.y = rowRect.y;
    coveredBlock.newRect.w = rowRect.w;
    coveredBlock.newRect.h = rowRect.h;
    coveredBlock.render(window.renderer);

    if (darkMode) {
        darkButton.render(window.renderer);
    }
    else {
        lightButton.render(window.renderer);
    }

    if (hold == SDL_PRESSED && (lightButton.newRect.leftX - 7 < xPos && xPos < lightButton.newRect.rightX + 7) && (lightButton.newRect.topY - 7 < yPos && yPos < lightButton.newRect.bottomY + 7) && modeRightSpot) {
        if (darkMode) {
            pressedDark.render(window.renderer);
        }
        else {
            pressedLight.render(window.renderer);
        }
    }
    if (hold == SDL_PRESSED && (applyButton.newRect.leftX - 7 < xPos && xPos < applyButton.newRect.rightX + 7) && (applyButton.newRect.topY - 7 < yPos && yPos < applyButton.newRect.bottomY + 7) && applyRightSpot) {
        SDL_RenderCopy(window.renderer, pressedApply.texture, NULL, &applyButton.newRect);
    }
    if (hold == SDL_PRESSED && (backButton.newRect.leftX - 7 < xPos && xPos < backButton.newRect.rightX + 7) && (backButton.newRect.topY - 7 < yPos && yPos < backButton.newRect.bottomY + 7) && backRightSpot) {
        SDL_RenderCopy(window.renderer, pressedBack.texture, NULL, &backButton.newRect);
    }

    if (blockLengthRightSpot || blockLengthValText.text == "") {
        tempRect.x = rowRect.x + blockLength*0.175;
        tempRect.y = rowRect.y + blockLength*0.175;
        tempRect.w = rowRect.w - blockLength*0.175*2;
        tempRect.h = rowRect.h - blockLength*0.175*2;
        window.fillRect(&color, tempRect);
        //SDL_RenderCopy(window.renderer, whiteSurface, NULL, &tempRect);
    }

    blockLengthText.render(window.renderer);
    blockLengthValText.render(window.renderer);
    modeText.render(window.renderer);

    SDL_RenderPresent(window.renderer);
}

void drawGameScreen(bool hold, int iIndex, int jIndex) {
    SDL_RenderClear(window.renderer);
    if (!darkMode) {
        for (int i = 1; i < currentField.row + 1; ++i) {
            for (int j = 1; j < currentField.col + 1; ++j) {
                if (currentField.open[i][j] == '*') {
                    mineBlock.newRect.x = blockLength*(j - 1);
                    mineBlock.newRect.y = blockLength*(i - 1);
                    mineBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == ' ') {
                    emptyBlock.newRect.x = blockLength*(j - 1);
                    emptyBlock.newRect.y = blockLength*(i - 1);
                    emptyBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '#') {
                    coveredBlock.newRect.x = blockLength*(j - 1);
                    coveredBlock.newRect.y = blockLength*(i - 1);
                    coveredBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == 'f') {
                    flagBlock.newRect.x = blockLength*(j - 1);
                    flagBlock.newRect.y = blockLength*(i - 1);
                    flagBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '0') {
                    emptyBlock.newRect.x = blockLength*(j - 1);
                    emptyBlock.newRect.y = blockLength*(i - 1);
                    emptyBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '1') {
                    oneBlock.newRect.x = blockLength*(j - 1);
                    oneBlock.newRect.y = blockLength*(i - 1);
                    oneBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '2') {
                    twoBlock.newRect.x = blockLength*(j - 1);
                    twoBlock.newRect.y = blockLength*(i - 1);
                    twoBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '3') {
                    threeBlock.newRect.x = blockLength*(j - 1);
                    threeBlock.newRect.y = blockLength*(i - 1);
                    threeBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '4') {
                    fourBlock.newRect.x = blockLength*(j - 1);
                    fourBlock.newRect.y = blockLength*(i - 1);
                    fourBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '5') {
                    fiveBlock.newRect.x = blockLength*(j - 1);
                    fiveBlock.newRect.y = blockLength*(i - 1);
                    fiveBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '6') {
                    sixBlock.newRect.x = blockLength*(j - 1);
                    sixBlock.newRect.y = blockLength*(i - 1);
                    sixBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '7') {
                    sevenBlock.newRect.x = blockLength*(j - 1);
                    sevenBlock.newRect.y = blockLength*(i - 1);
                    sevenBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == '8') {
                    eightBlock.newRect.x = blockLength*(j - 1);
                    eightBlock.newRect.y = blockLength*(i - 1);
                    eightBlock.render(window.renderer);
                }
                else if (currentField.open[i][j] == 'x') {
                    wrongMineBlock.newRect.x = blockLength*(j - 1);
                    wrongMineBlock.newRect.y = blockLength*(i - 1);
                    wrongMineBlock.render(window.renderer);
                }
            }
        }
        if (hold == SDL_PRESSED && currentField.open[iIndex][jIndex] == '#') {
            emptyBlock.newRect.x = blockLength*(jIndex - 1);
            emptyBlock.newRect.y = blockLength*(iIndex - 1);
            emptyBlock.render(window.renderer);
        }
    }
    else {
        for (int i = 1; i < currentField.row + 1; ++i) {
            for (int j = 1; j < currentField.col + 1; ++j) {
                if (currentField.open[i][j] == '*') {
                    mineBlockDark.newRect.x = blockLength*(j - 1);
                    mineBlockDark.newRect.y = blockLength*(i - 1);
                    mineBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == ' ') {
                    emptyBlockDark.newRect.x = blockLength*(j - 1);
                    emptyBlockDark.newRect.y = blockLength*(i - 1);
                    emptyBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '#') {
                    coveredBlockDark.newRect.x = blockLength*(j - 1);
                    coveredBlockDark.newRect.y = blockLength*(i - 1);
                    coveredBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == 'f') {
                    flagBlockDark.newRect.x = blockLength*(j - 1);
                    flagBlockDark.newRect.y = blockLength*(i - 1);
                    flagBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '0') {
                    emptyBlockDark.newRect.x = blockLength*(j - 1);
                    emptyBlockDark.newRect.y = blockLength*(i - 1);
                    emptyBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '1') {
                    oneBlockDark.newRect.x = blockLength*(j - 1);
                    oneBlockDark.newRect.y = blockLength*(i - 1);
                    oneBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '2') {
                    twoBlockDark.newRect.x = blockLength*(j - 1);
                    twoBlockDark.newRect.y = blockLength*(i - 1);
                    twoBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '3') {
                    threeBlockDark.newRect.x = blockLength*(j - 1);
                    threeBlockDark.newRect.y = blockLength*(i - 1);
                    threeBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '4') {
                    fourBlockDark.newRect.x = blockLength*(j - 1);
                    fourBlockDark.newRect.y = blockLength*(i - 1);
                    fourBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '5') {
                    fiveBlockDark.newRect.x = blockLength*(j - 1);
                    fiveBlockDark.newRect.y = blockLength*(i - 1);
                    fiveBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '6') {
                    sixBlockDark.newRect.x = blockLength*(j - 1);
                    sixBlockDark.newRect.y = blockLength*(i - 1);
                    sixBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '7') {
                    sevenBlockDark.newRect.x = blockLength*(j - 1);
                    sevenBlockDark.newRect.y = blockLength*(i - 1);
                    sevenBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == '8') {
                    eightBlockDark.newRect.x = blockLength*(j - 1);
                    eightBlockDark.newRect.y = blockLength*(i - 1);
                    eightBlockDark.render(window.renderer);
                }
                else if (currentField.open[i][j] == 'x') {
                    wrongMineBlockDark.newRect.x = blockLength*(j - 1);
                    wrongMineBlockDark.newRect.y = blockLength*(i - 1);
                    wrongMineBlockDark.render(window.renderer);
                }
            }
        }
        if (hold == SDL_PRESSED && currentField.open[iIndex][jIndex] == '#') {
            emptyBlockDark.newRect.x = blockLength*(jIndex - 1);
            emptyBlockDark.newRect.y = blockLength*(iIndex - 1);
            emptyBlockDark.render(window.renderer);
        }
    }
    SDL_RenderPresent(window.renderer);
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_EVERYTHING); //Intializes everything
    IMG_Init(IMG_INIT_PNG); //Intializes the image type
    IMG_Init(IMG_INIT_JPG);
    TTF_Init(); //Initializes font stuff
    
    //checks to see if every thing was loaded corectly.
    if (settings.checkLoaded()) {
        customField.fixValues();
    }
    else { // This will proper default settigns if it run into any issues
        settings.createFile();
        settings.appendFile("blockLength " + std::to_string(40) + "\n");
        settings.appendFile("row " + std::to_string(16) + "\n");
        settings.appendFile("col " + std::to_string(30) + "\n");
        settings.appendFile("mines " + std::to_string(99) + "\n");
        settings.appendFile("mode " + settings.boolToString(false) + "\n");
        std::cout << "settings.txt was not found" << std::endl;
        std::cout << "new settings.txt was created" << std::endl;
        std::cout << "Please reopen Minesweeper" << std::endl;

        SDL_Delay(5000);
        return EXIT;
    }

    //Sets up all the text stuff
    applyButton.newRect.shiftX();
    backButton.newRect.shiftX();
    int fontSize = blockLength*0.875; //should scale to 35
    rowText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", "# Rows", fontSize, window.w*0.30, window.h*0.48);
    rowValText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", std::to_string(customField.row).c_str(), fontSize, window.w*0.65, window.h*0.48);
    colText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", "# Columns", fontSize, window.w*0.30, window.h*0.60);
    colValText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", std::to_string(customField.col).c_str(), fontSize, window.w*0.65, window.h*0.60);
    minesText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", "# Mines", fontSize, window.w*0.30, window.h*0.72);
    minesValText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", std::to_string(customField.mines).c_str(), fontSize, window.w*0.65, window.h*0.72);
    rowRect.x = rowValText.rect.x - blockLength*0.25;
    rowRect.y = rowValText.rect.y - blockLength*0.25;
    rowRect.w = blockLength*1.55;
    rowRect.h = blockLength*1.55;
    rowRect.makeDimensions();
    colRect.x = colValText.rect.x - blockLength*0.25;
    colRect.y = colValText.rect.y - blockLength*0.25;
    colRect.w = blockLength*1.55;
    colRect.h = blockLength*1.55;
    colRect.makeDimensions();
    minesRect.x = minesValText.rect.x - blockLength*0.25;
    minesRect.y = minesValText.rect.y - blockLength*0.25;
    minesRect.w = blockLength*1.55;
    minesRect.h = blockLength*1.55;
    minesRect.makeDimensions();

    blockLengthText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", "Block Length", fontSize, window.w*0.30, window.h*0.48);
    blockLengthValText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", std::to_string(blockLength).c_str(), fontSize, window.w*0.65, window.h*0.48);
    modeText.setup(window.renderer, "Fonts/UNISPACE BD.ttf", "Current mode", fontSize, window.w*0.30, window.h*0.60);
    lightButton.newRect.x = colRect.x;
    lightButton.newRect.y = colRect.y;
    lightButton.newRect.makeDimensions();
    pressedLight.newRect.x = colRect.x;
    pressedLight.newRect.y = colRect.y;
    pressedLight.newRect.makeDimensions();
    darkButton.newRect.x = colRect.x;
    darkButton.newRect.y = colRect.y;
    darkButton.newRect.makeDimensions();
    pressedDark.newRect.x = colRect.x;
    pressedDark.newRect.y = colRect.y;
    pressedDark.newRect.makeDimensions();
    lightButton.newRect.shiftX();
    pressedLight.newRect.shiftX();
    darkButton.newRect.shiftX();
    pressedDark.newRect.shiftX();

    //goes through the game states 
    bool run = true;
    while (run) {
        if (gameState == EXIT) {
            run = false;
        }
        if (gameState == STARTMENU) {
            gameState = startMenu();
        }
        if (gameState == MAINMENU) {
            gameState = mainMenu();
            if (difficulty == BEGINNER) {
                currentField = beginnerField;
            }
            else if (difficulty == INTERMEDIATE) {
                currentField = intermediateField;
            }
            else if (difficulty == EXPERT) {
                currentField = expertField;
            }
            else if (difficulty == CUSTOM) {
                currentField = customField;
            }
        }
        if (gameState == CUSTOMMENU) {
            gameState = customMenu();
            if (difficulty == CUSTOM) {
                currentField = customField;
            }
        }
        if (gameState == SETTINGSMENU) {
            gameState = settingsMenu();
        }
        if (gameState == GAME) {
            gameState = game();
        }
    }

    save();
    SDL_DestroyWindow(window.window); //This closed the window
    //need to make deconstructors 
    window.window = NULL;
    SDL_Quit(); //Closed the SDL program
    IMG_Quit();
    std::cout << "=== Program Ended Well ===" << std::endl;
    return EXIT;
}

int startMenu() {
    bool runStartMenu = true;
    bool hold = false;
    bool playRightSpot = false; // The right spot is the play square
    bool exitRightSpot = false;
    SDL_SetWindowTitle(window.window, "MineSweeper");

    while (runStartMenu) {
        startingTick = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            SDL_GetMouseState(&xPos, &yPos);
            if (event.type == SDL_QUIT) {
                return EXIT;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << xPos << "   " << yPos << std::endl;
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if ((pressedPlay.newRect.x - 7 < xPos && xPos < pressedPlay.newRect.trX + 7) && (pressedPlay.newRect.y - 7 < yPos && yPos < pressedPlay.newRect.brY + 7)) {
                        playRightSpot = true;
                    }
                    else if ((pressedExit.newRect.x - 7 < xPos && xPos < pressedExit.newRect.trX + 7) && (pressedExit.newRect.y - 7 < yPos && yPos < pressedExit.newRect.brY + 7)) {
                        exitRightSpot = true;
                    }
                }
                hold = event.button.state;
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if ((pressedPlay.newRect.x - 7 < xPos && xPos < pressedPlay.newRect.trX + 7) && (pressedPlay.newRect.y - 7 < yPos && yPos < pressedPlay.newRect.brY + 7) && playRightSpot) {
                        return MAINMENU;
                    }
                    else if ((pressedExit.newRect.x - 7 < xPos && xPos < pressedExit.newRect.trX + 7) && (pressedExit.newRect.y - 7 < yPos && yPos < pressedExit.newRect.brY + 7) && exitRightSpot) {
                        return EXIT;
                    }
                }
                playRightSpot = false;
                exitRightSpot = false;
                hold = event.button.state;
            }
        }

        drawStartMenu(hold, playRightSpot, exitRightSpot);
        frameCap(fps, startingTick);
    }
    return EXIT;
}

int mainMenu() {
    bool runMainMenu = true;
    bool hold = false;
    bool beginnerRightSpot = false; // The right spot is the play square
    bool intermediateRightSpot = false;
    bool expertRightSpot = false;
    bool customRightSpot = false;
    bool settingsRightSpot = false;
    bool backRightSpot = false;
    SDL_SetWindowTitle(window.window, "MineSweeper");

    while (runMainMenu) {
        startingTick = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            SDL_GetMouseState(&xPos, &yPos);
            if (event.type == SDL_QUIT) {
                return EXIT;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << xPos << "   " << yPos << std::endl;

                if (event.button.button == SDL_BUTTON_LEFT) {
                    if ((pressedBeginner.newRect.x - 7 < xPos && xPos < pressedBeginner.newRect.trX + 7) && (pressedBeginner.newRect.y - 7 < yPos && yPos < pressedBeginner.newRect.brY + 7)) {
                        beginnerRightSpot = true;
                    }
                    else if ((pressedIntermediate.newRect.x - 7 < xPos && xPos < pressedIntermediate.newRect.trX + 7) && (pressedIntermediate.newRect.y - 7 < yPos && yPos < pressedIntermediate.newRect.brY + 7)) {
                        intermediateRightSpot = true;
                    }
                    else if ((pressedExpert.newRect.x - 7 < xPos && xPos < pressedExpert.newRect.trX + 7) && (pressedExpert.newRect.y - 7 < yPos && yPos < pressedExpert.newRect.brY + 7)) {
                        expertRightSpot = true;
                    }
                    else if ((pressedCustom.newRect.x - 7 < xPos && xPos < pressedCustom.newRect.trX + 7) && (pressedCustom.newRect.y - 7 < yPos && yPos < pressedCustom.newRect.brY + 7)) {
                        customRightSpot = true;
                    }
                    else if ((pressedSettings.newRect.x - 7 < xPos && xPos < pressedSettings.newRect.trX + 7) && (pressedSettings.newRect.y - 7 < yPos && yPos < pressedSettings.newRect.brY + 7)) {
                        settingsRightSpot = true;
                    }
                    else if ((pressedBack.newRect.x - 7 < xPos && xPos < pressedBack.newRect.trX + 7) && (pressedBack.newRect.y - 7 < yPos && yPos < pressedBack.newRect.brY + 7)) {
                        backRightSpot = true;
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    return STARTMENU;
                }
                hold = event.button.state;
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {

                    if ((pressedBeginner.newRect.x - 7 < xPos && xPos < pressedBeginner.newRect.trX + 7) && (pressedBeginner.newRect.y - 7 < yPos && yPos < pressedBeginner.newRect.brY + 7) && beginnerRightSpot) {
                        difficulty = BEGINNER;
                        return GAME;
                    }
                    else if ((pressedIntermediate.newRect.x - 7 < xPos && xPos < pressedIntermediate.newRect.trX + 7) && (pressedIntermediate.newRect.y - 7 < yPos && yPos < pressedIntermediate.newRect.brY + 7) && intermediateRightSpot) {
                        difficulty = INTERMEDIATE;
                        return GAME;
                    }
                    else if ((pressedExpert.newRect.x - 7 < xPos && xPos < pressedExpert.newRect.trX + 7) && (pressedExpert.newRect.y - 7 < yPos && yPos < pressedExpert.newRect.brY + 7) && expertRightSpot) {
                        difficulty = EXPERT;
                        return GAME;
                    }
                    else if ((pressedCustom.newRect.x - 7 < xPos && xPos < pressedCustom.newRect.trX + 7) && (pressedCustom.newRect.y - 7 < yPos && yPos < pressedCustom.newRect.brY + 7) && customRightSpot) {
                        return CUSTOMMENU;
                    }
                    else if ((pressedSettings.newRect.x - 7 < xPos && xPos < pressedSettings.newRect.trX + 7) && (pressedSettings.newRect.y - 7 < yPos && yPos < pressedSettings.newRect.brY + 7) && settingsRightSpot) {
                        return SETTINGSMENU;
                    }
                    else if ((pressedBack.newRect.x - 7 < xPos && xPos < pressedBack.newRect.trX + 7) && (pressedBack.newRect.y - 7 < yPos && yPos < pressedBack.newRect.brY + 7) && backRightSpot) {
                        return STARTMENU;
                    }
                }
                beginnerRightSpot = false;
                intermediateRightSpot = false;
                expertRightSpot = false;
                customRightSpot = false;
                settingsRightSpot = false;
                backRightSpot = false;
                hold = event.button.state;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return STARTMENU;
                }
            }
        }

        drawMainMenu(hold, beginnerRightSpot, intermediateRightSpot, expertRightSpot, customRightSpot, settingsRightSpot, backRightSpot);
        frameCap(fps, startingTick);
    }

    return EXIT;
}

int customMenu() {
    bool runCustomMenu = true;
    bool hold = false;
    bool rowRightSpot = false; // The right spot is the play square
    bool colRightSpot = false;
    bool minesRightSpot = false;
    bool playRightSpot = false;
    bool backRightSpot = false;
    SDL_SetWindowTitle(window.window, "MineSweeper");
    SDL_StartTextInput();
    while (runCustomMenu) {
        startingTick = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            SDL_GetMouseState(&xPos, &yPos);
            if (event.type == SDL_QUIT) {
                return EXIT;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << xPos << "   " << yPos << std::endl;
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if ((rowRect.x < xPos && xPos < rowRect.trX) && (rowRect.y < yPos && yPos < rowRect.brY)) {
                        rowRightSpot = true;
                        rowValText.text = "";
                        rowValText.createTexture(window.renderer);
                    }
                    else {
                        rowRightSpot = false;
                    }
                    if ((colRect.x < xPos && xPos < colRect.trX) && (colRect.y< yPos && yPos < colRect.brY)) {
                        colRightSpot = true;
                        colValText.text = "";
                        colValText.createTexture(window.renderer);
                    }
                    else {
                        colRightSpot = false;
                    }

                    if ((minesRect.x < xPos && xPos < minesRect.trX) && (minesRect.y < yPos && yPos < minesRect.brY)) {
                        minesRightSpot = true;
                        minesValText.text = "";
                        minesValText.createTexture(window.renderer);
                    }
                    else {
                        minesRightSpot = false;
                    }

                    if ((playButton.newRect.x - 7 < xPos && xPos < playButton.newRect.trX + 7) && (playButton.newRect.y - 7 < yPos && yPos < playButton.newRect.brY + 7)) {
                        playRightSpot = true;
                    }
                    else if ((backButton.newRect.x - 7 < xPos && xPos < backButton.newRect.trX + 7) && (backButton.newRect.y - 7 < yPos && yPos < backButton.newRect.brY + 7)) {
                        backRightSpot = true;
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    coveredBlock.newRect.w = blockLength;
                    coveredBlock.newRect.h = blockLength;
                    return MAINMENU;
                }

                hold = event.button.state;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (rowRightSpot && rowValText.text.length() > 0) {
                        rowValText.text.pop_back();
                        rowValText.createTexture(window.renderer);
                        
                    }
                    else if(colRightSpot && colValText.text.length() > 0) {
                        colValText.text.pop_back();
                        colValText.createTexture(window.renderer);
                    }

                    else if (minesRightSpot && minesValText.text.length() > 0) {
                        minesValText.text.pop_back();
                        minesValText.createTexture(window.renderer);
                    }
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    if (colRightSpot) {
                        colRightSpot = false;
                        rowRightSpot = true;
                    }
                    else if (minesRightSpot) {
                        minesRightSpot = false;
                        colRightSpot = true;
                    }
                }
                else if (event.key.keysym.sym == SDLK_DOWN) {
                    if (rowRightSpot) {
                        rowRightSpot = false;
                        colRightSpot = true;
                    }
                    else if (colRightSpot) {
                        colRightSpot = false;
                        minesRightSpot = true;
                    }
                }
            }
            else if (event.type == SDL_TEXTINPUT && '0' <= *event.text.text && *event.text.text <= '9') {
                if (rowRightSpot && rowValText.text.length() < 2) {
                    rowValText.text = rowValText.text + event.text.text;
                    rowValText.createTexture(window.renderer);
                }
                if (colRightSpot && colValText.text.length() < 2) {
                    colValText.text = colValText.text + event.text.text;
                    colValText.createTexture(window.renderer);
                }

                else if (minesRightSpot && minesValText.text.length() < 2) {
                    minesValText.text = minesValText.text + event.text.text;
                    minesValText.createTexture(window.renderer);
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if ((playButton.newRect.x - 7 < xPos && xPos < playButton.newRect.trX + 7) && (playButton.newRect.y - 7 < yPos && yPos < playButton.newRect.brY + 7) && playRightSpot && (rowValText.text != "" && colValText.text != "" && minesValText.text != "") && (rowValText.text != "0" && colValText.text != "0" && minesValText.text != "0")) {
                        difficulty = CUSTOM;
                        customField.row = stoi(rowValText.text);
                        customField.col = stoi(colValText.text);
                        customField.mines = stoi(minesValText.text);
                        if (customField.validField()) {
                            SDL_StopTextInput();
                            coveredBlock.newRect.w = blockLength;
                            coveredBlock.newRect.h = blockLength;
                            return GAME;
                        }
                    }
                    else if ((backButton.newRect.x - 7 < xPos && xPos < backButton.newRect.trX + 7) && (backButton.newRect.y - 7 < yPos && yPos < backButton.newRect.brY + 7) && backRightSpot) {
                        SDL_StopTextInput();
                        coveredBlock.newRect.w = blockLength;
                        coveredBlock.newRect.h = blockLength;
                        return MAINMENU;
                    }
                }
                playRightSpot = false;
                backRightSpot = false;
                hold = event.button.state;
            }
        }

        drawCustomMenu(hold, rowRightSpot, colRightSpot, minesRightSpot, playRightSpot, backRightSpot);        
        frameCap(fps, startingTick);
    }
    SDL_StopTextInput();
    return EXIT;
}

int settingsMenu() {
    bool runSettingsMenu = true;
    bool hold = false;
    bool blockLengthRightSpot = false; // The right spot is the play square
    bool modeRightSpot = false;
    bool applyRightSpot = false;
    bool backRightSpot = false;
    SDL_SetWindowTitle(window.window, "MineSweeper");
    SDL_StartTextInput();
    while (runSettingsMenu) {
        startingTick = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            SDL_GetMouseState(&xPos, &yPos);
            if (event.type == SDL_QUIT) {
                return EXIT;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                std::cout << xPos << "   " << yPos << std::endl;
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if ((rowRect.x - 7 < xPos && xPos < rowRect.trX + 7) && (rowRect.y - 7 < yPos && yPos < rowRect.brY + 7)) {
                        blockLengthRightSpot = true;
                        blockLengthValText.text = "";
                        blockLengthValText.createTexture(window.renderer);
                    }
                    else {
                        blockLengthRightSpot = false;
                    }
                    if ((lightButton.newRect.x - 7 < xPos && xPos < lightButton.newRect.trX + 7) && (lightButton.newRect.y - 7 < yPos && yPos < lightButton.newRect.brY + 7)) {
                        modeRightSpot = true;
                    }

                    if ((applyButton.newRect.x - 7 < xPos && xPos < applyButton.newRect.trX + 7) && (applyButton.newRect.y - 7 < yPos && yPos < applyButton.newRect.brY + 7)) {
                        applyRightSpot = true;
                    }
                    if ((backButton.newRect.x - 7 < xPos && xPos < backButton.newRect.trX + 7) && (backButton.newRect.y - 7 < yPos && yPos < backButton.newRect.brY + 7)) {
                        backRightSpot = true;
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    coveredBlock.newRect.w = blockLength;
                    coveredBlock.newRect.h = blockLength;
                    return MAINMENU;
                }

                hold = event.button.state;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (blockLengthRightSpot && blockLengthValText.text.length() > 0) {
                        blockLengthValText.text.pop_back();
                        blockLengthValText.createTexture(window.renderer);
                        
                    }
                }
            }
            else if (event.type == SDL_TEXTINPUT && '0' <= *event.text.text && *event.text.text <= '9') {
                if (blockLengthRightSpot && blockLengthValText.text.length() < 2) {
                    blockLengthValText.text = blockLengthValText.text + event.text.text;
                    blockLengthValText.createTexture(window.renderer);
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if ((lightButton.newRect.x - 7 < xPos && xPos < lightButton.newRect.trX + 7) && (lightButton.newRect.y - 7 < yPos && yPos < lightButton.newRect.brY + 7) && modeRightSpot) {
                        if (darkMode) {
                            darkMode = false;
                        }
                        else {
                            darkMode = true;
                        }
                    }
                    else if ((applyButton.newRect.x - 7 < xPos && xPos < applyButton.newRect.trX + 7) && (applyButton.newRect.y - 7 < yPos && yPos < applyButton.newRect.brY + 7) && applyRightSpot) {
                        blockLength = stoi(blockLengthValText.text);
                        reSizeEverthing();
                        window.setWindowSize(mainMenuImage.newRect.w, mainMenuImage.newRect.h);
                    }
                    else if ((backButton.newRect.x - 7 < xPos && xPos < backButton.newRect.trX + 7) && (backButton.newRect.y - 7 < yPos && yPos < backButton.newRect.brY + 7) && backRightSpot) {
                        coveredBlock.newRect.w = blockLength;
                        coveredBlock.newRect.h = blockLength;
                        SDL_StopTextInput();
                        return MAINMENU;
                    }
                }

                modeRightSpot = false;
                applyRightSpot = false;
                backRightSpot = false;
                hold = event.button.state;
            }
        }

        drawSettingsMenu(hold, blockLengthRightSpot, modeRightSpot, applyRightSpot, backRightSpot);        
        frameCap(fps, startingTick);
    }
    SDL_StopTextInput();
    return EXIT;
}

int game() {
    bool runGame = true;
    bool firstBlock = true;
    bool hold = false; 
    int i;
    int j;

    //currentField = expertField;
    window.setWindowSize(blockLength*currentField.col, blockLength*currentField.row);
    currentField.hiddenSetup();
    currentField.openSetup();
    setGameTitle("MineSweeper");
    while (runGame) {
        startingTick = SDL_GetTicks(); //SDL_GetTicks() counts the elapse time in millisecounds
        while (SDL_PollEvent(&event)) {
            SDL_GetMouseState(&xPos, &yPos);
            pos2Index(&i, &j);

            if (event.type == SDL_QUIT) {
                return EXIT;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    hold = event.button.state;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (!firstBlock) {
                        currentField.placeFlag(i, j);
                        setGameTitle("MineSweeper");
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    hold = event.button.state;
                    if (firstBlock) {
                        int x = 0;
                        while (currentField.hidden[i][j] != '0') {
                            if (x == 100000) {
                                break;
                            }
                            ++x;
                            currentField.hiddenSetup();
                        }
                        firstBlock = false;
                    }
                    if (currentField.hidden[i][j] == '*' && currentField.open[i][j] != 'f') {
                        currentField.loseScreen();
                        setGameTitle("Game Over");
                        runGame = false;
                    }
                    else if (currentField.hidden[i][j] == '0' && currentField.open[i][j] != 'f') {
                        currentField.checkEmpty(i, j);
                        runGame = currentField.checkWinCondition();
                        if (!runGame) {
                            setGameTitle("You Win");
                        }
                    }
                    else if (currentField.open[i][j] != 'f') {
                        currentField.open[i][j] = currentField.hidden[i][j];
                        runGame = currentField.checkWinCondition();
                        if (!runGame) {
                            setGameTitle("You Win");
                        }
                    }
                }
                if (event.button.button == SDL_BUTTON_MIDDLE) {
                    std::cout << "Middle key was called" << std::endl;
                    window.setWindowSize(mainMenuImage.newRect.w, mainMenuImage.newRect.h);
                    return MAINMENU;
                }
            }
            if (event.type == SDL_KEYDOWN) { 
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    std::cout << "Escape key was called" << std::endl;
                    window.setWindowSize(mainMenuImage.newRect.w, mainMenuImage.newRect.h);
                    return MAINMENU;
                }
            }
        }

        while (!runGame) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    return EXIT;
                }
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    if (event.button.button == SDL_BUTTON_MIDDLE) {
                        window.setWindowSize(mainMenuImage.newRect.w, mainMenuImage.newRect.h);
                        return MAINMENU;
                    }
                }
                if (event.type == SDL_MOUSEBUTTONUP) {
                    currentField.openSetup();
                    currentField.hiddenSetup();
                    setGameTitle("MineSweeper");
                    firstBlock = true;
                    runGame = true;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        window.setWindowSize(mainMenuImage.newRect.w, mainMenuImage.newRect.h);
                        return MAINMENU;
                    }
                }
            }
            frameCap(fps, startingTick); 
            drawGameScreen(hold, i, j);
        }
        frameCap(fps, startingTick); 
        drawGameScreen(hold, i, j);
    }
    return EXIT;
}