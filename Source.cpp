#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include "map.h"
#include "view.h"

using namespace std;
using namespace sf;

enum stateOfGame { play, about, menu, win, exitOfGame, countOfStates };
enum stateOfMenu { toPlay, toAbout, toMenu, toExitOfGame};

class TimeOfGame {
private:
    Clock clock;
    float time;
    float smoothnessOfGame;
public:
    TimeOfGame() {
        smoothnessOfGame = 800;
        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= smoothnessOfGame;
    }
    TimeOfGame(float smoothness) {
        this->smoothnessOfGame = smoothness;
        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= smoothnessOfGame;
    }
    void refreshTime() {
        time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= smoothnessOfGame;
    }
    float getTime() {
        return time;
    }
};

class Entity {
protected:
    Image image;
    Texture texture;
    Sprite sprite;
    int widthOfSprite, heightOfSprite;
    float currentFrame;
public:
    Entity() {
        this->currentFrame = 0;
        widthOfSprite = 0;
        heightOfSprite = 0;
    }
    Entity(Image image, Texture texture, Sprite sprite, int width, int height) {
        this->currentFrame = 0;
        this->image = image;
        this->texture = texture;
        this->sprite = sprite;
        widthOfSprite = width;
        heightOfSprite = height;
    }
    Entity(string wayToTheImage, int width, int height) {
        this->currentFrame = 0;
        this->image.loadFromFile(wayToTheImage);
        this->texture.loadFromImage(this->image);
        this->sprite.setTexture(this->texture);
        widthOfSprite = width;
        heightOfSprite = height;
    }
    Entity(Entity& entity, string wayToTheImage, int width, int height) {
        this->currentFrame = 0;
        this->image.loadFromFile(wayToTheImage);
        this->texture = entity.texture;
        this->sprite = entity.sprite;
        widthOfSprite = width;
        heightOfSprite = height;
    }
    void setImage(string image) {
        this->image.loadFromFile(image);
    }
    void setTexture(Image image) {
        this->texture.loadFromImage(image);
    }
    void setSprite(Texture heroTexture) {
        this->sprite.setTexture(texture);
    }
    void setTextureRect(int rectLeft, int rectTop, int rectWidth, int rectHeight) {
        this->sprite.setTextureRect(IntRect(rectLeft, rectTop, rectWidth, rectHeight));
    }
    void setPositionOfSprite(float x, float y) {
        this->sprite.setPosition(x, y);
    }
    void setPositionOfSprite(Vector2f &position) {
        this->sprite.setPosition(position);
    }
    void setOrigin(float x, float y) {
        this->sprite.setOrigin(x, y);
    }
    Image getImage() {
        return this->image;
    }
    Texture getTexture() {
        return this->texture;
    }
    Sprite getSprite() {
        return this->sprite;
    }
    Vector2f getPositionOfSprite() {
        return this->sprite.getPosition();
    }
    void setWayToTheImage(string way) {
        this->image.loadFromFile(way);
        this->texture.loadFromImage(this->image);
        this->sprite.setTexture(this->texture);
    }
    void setWidthAndHeight(int width, int height) {
        this->widthOfSprite = width;
        this->heightOfSprite = height;
    }
};

class Alive : public Entity {
protected:
    float dx, dy, x, y, speed; //  x/y - coordinates, dx/dy - acceleration
    bool isAlive, isMove, isOnGround;
    enum { left, right, jump, stay } state, previousState;


public:
    Alive() {
        dx = 0; dy = 0; x = 0; y = 0; speed = 0;
        isAlive = true;  isMove = false; isOnGround = true;
        previousState = stay;
        state = stay;
    }
    Alive(Image image, Texture texture, Sprite sprite, int width, int height) : Entity(image, texture, sprite, width, height) {
        dx = 1;
        dy = 1;
        x = 0;
        y = 0;
        speed = 0;
        previousState = stay;
        state = stay;
        isAlive = false, isMove = false, isOnGround = true;
        sprite.setOrigin(widthOfSprite / 2, heightOfSprite / 2);
    }
    Alive(string wayToTheImage, int width, int height) : Entity(wayToTheImage, width, height) {
        dx = 1;
        dy = 1;
        x = 0;
        y = 0;
        speed = 0;
        previousState = stay;
        state = stay;
        isAlive = false, isMove = false, isOnGround = true;
        sprite.setOrigin(widthOfSprite / 2, heightOfSprite / 2);
    }
    Alive(Entity& entity, string wayToTheImage, int width, int height) : Entity(entity, wayToTheImage, width, height) {
        dx = 1;
        dy = 1;
        x = 0;
        y = 0;
        speed = 0;
        previousState = stay;
        state = stay;
        isAlive = false, isMove = false, isOnGround = true;
        sprite.setOrigin(widthOfSprite / 2, heightOfSprite / 2);
    }
    Alive(string wayToTheImage, float dx, float dy, float x, float y, float speed, int width, int height) : Entity(wayToTheImage, width, height) {
        if (x < 0 || y < 0) {
            cerr << "Coordinates can't be negative!";
            abort();
        }
        this->dx = dx;
        this->dy = dy;
        this->x = x;
        this->y = y;
        this->speed = speed;
        state = stay;
        isAlive = false, isMove = false, isOnGround = true;
        sprite.setOrigin(widthOfSprite / 2, heightOfSprite / 2);
    }
    void setIsAlive(bool isAlive) {
        this->isAlive = isAlive;
    }
    void setIsMove(bool isMove) {
        this->isMove = isMove;
    }
    void setIsOnGround(bool isOnGround) {
        this->isOnGround = isOnGround;
    }
    void setDXY(float dx, float dy) {
        this->dx = dx;
        this->dy = dy;
    }
    void setXY(float x, float y) {
        if (x < 0 || y < 0) {
            cerr << "Coordinates can't be negative!";
            abort();
        }
        this->x = x;
        this->y = y;
    }
    void setSpeed(float speed) {
        this->speed = speed;
    }
   
};

class Hero : public Alive {
private:
    bool isTouchingLiana, isFallingDown, isEndOfTheGame;
    int score; 

    

    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = y / 50; i < (y + heightOfSprite) / 50 && i < 50; i++) {
            for (int j = x / 50; j < (x + widthOfSprite) / 50 && j < 50; j++) {
                
                if (TileMap[i][j] == '1') {
                    
                    if (dy < 0) {
                        y = i * 50 + 50; dy = 0;
                    }
                    if (dx > 0) {
                        x = j * 50 - widthOfSprite;
                    }
                    if (dx < 0) {
                        x = j * 50 + 50;
                    }
                }
                
                if (TileMap[i][j] == '0') {
                    if (Dy > 0) {
                        y = i * 50 - heightOfSprite;  dy = 0; isOnGround = true; isFallingDown = false;
                    }
                    if (Dy < 0) { y = i * 50 + 50;  dy = 0;}
                    if (Dx > 0) { x = j * 50 - widthOfSprite; }
                    if (Dx < 0) { x = j * 50 + 50; }
                }
                else if (TileMap[i][j] != '=' && TileMap[i][j] != '>' && TileMap[i][j] == '<') { isOnGround = false; isFallingDown = true; }

                if (TileMap[i][j] == '=') {
                    if (Dy > 0) {
                        y = i * 50 - heightOfSprite;  dy = 0; isOnGround = true; isFallingDown = false;

                    }
                    if (Dy < 0) { y = i * 50 + 50;  dy = 0; }
                    if (Dx > 0) { x = j * 50 - widthOfSprite; }
                    if (Dx < 0) { x = j * 50 + 50; }
                } 

                if (TileMap[i][j] == '>') {
                    if (Dy > 0) {
                        y = i * 50 - heightOfSprite;  dy = 0; isOnGround = true; isFallingDown = false;
                    }
                    if (Dy < 0) { y = i * 50 + 50;  dy = 0; }
                    if (Dx > 0) { x = j * 50 - widthOfSprite; }
                    if (Dx < 0) { x = j * 50 + 50; }
                }
                
                if (TileMap[i][j] == '<') {
                    if (Dy > 0) {
                        y = i * 50 - heightOfSprite;  dy = 0; isOnGround = true; isFallingDown = false;
                    }
                    if (Dy < 0) { y = i * 50 + 50;  dy = 0; }
                    if (Dx > 0) { x = j * 50 - widthOfSprite; }
                    if (Dx < 0) { x = j * 50 + 50; }
                }

                if (TileMap[i][j] == 'b') {
                    score++;
                    TileMap[i][j] = 'e';
                }
                if (TileMap[i][j] == 's') {
                    score += 3;
                    TileMap[i][j] = 'e';
                }
                if (TileMap[i][j] == 'g') {
                    TileMap[i][j] == ' ';
                    isEndOfTheGame = true;
                }
            }
        }
    }
    


public:
    Hero() {
    score = 0;
    isTouchingLiana = false;
    isEndOfTheGame = false;
    isFallingDown = false;
    }
    Hero(Image image, Texture texture, Sprite sprite, int width, int height) : Alive(image, texture, sprite, width, height) {
        score = 0;
        isTouchingLiana = false;
        isEndOfTheGame = false;
        isFallingDown = false;
        }
    Hero(string wayToTheImage, int width, int height) : Alive(wayToTheImage, width, height) {
        score = 0;
        isTouchingLiana = false;
        isEndOfTheGame = false;
        isFallingDown = false;
        }
    Hero(Entity& entity, string wayToTheImage, int width, int height) : Alive(entity, wayToTheImage, width, height) { 
        score = 0;
        isTouchingLiana = false;
        isEndOfTheGame = false;
        isFallingDown = false;
        }
    Hero(string wayToTheImage, float dx, float dy, float x, float y, float speed, int width, int height) : Alive(wayToTheImage, dx, dy, x, y, speed, width, height) { 
        score = 0;
        isTouchingLiana = false;
        isEndOfTheGame = false;
        isFallingDown = false;
       }
    
    void moveUpdate(TimeOfGame& timeOfGame) {
        if (!isEndOfTheGame) {
            move(timeOfGame);
            switch (state) {
            case right: dx = speed; break;
            case left: dx = -speed; break;
            case stay: dx = 0; break;
            case jump:  break;
            }
            x += dx * timeOfGame.getTime();
            checkCollisionWithMap(dx, 0);
            y += dy * timeOfGame.getTime();
            checkCollisionWithMap(0, dy);
            sprite.setPosition(x + widthOfSprite / 2, y + heightOfSprite / 2);
            dy = dy + 0.0015 * timeOfGame.getTime();
            getPlayerCoordinateForView(x, y);
        }
    }
    
    
    void move(TimeOfGame& timeOfGame) {
        if ((Keyboard::isKeyPressed(Keyboard::Left)) && isOnGround && !isFallingDown) {
            previousState = state;
            state = left;
            speed = 0.25f;
            currentFrame += 0.01f * timeOfGame.getTime();
            if (currentFrame > 3) currentFrame -= 3; 
            sprite.setTextureRect(IntRect(widthOfSprite * int(currentFrame), heightOfSprite, widthOfSprite, heightOfSprite)); 
        }
        if ((Keyboard::isKeyPressed(Keyboard::Right)) && isOnGround && !isFallingDown) {
            previousState = state;
            state = right;
            speed = 0.25f;
            currentFrame += 0.01f * timeOfGame.getTime(); 
            if (currentFrame > 3) currentFrame -= 3; 
            sprite.setTextureRect(IntRect(widthOfSprite * int(currentFrame), 0, widthOfSprite, heightOfSprite));
        }
        if ((Keyboard::isKeyPressed(Keyboard::Up)  || Keyboard::isKeyPressed(Keyboard::Space)) && isOnGround && !isTouchingLiana) {
            dy = -0.8f;
            isOnGround = false;
            currentFrame = 2;
            if (currentFrame == 3) { currentFrame -= 3; }
            if (previousState == right) {
                sprite.setTextureRect(IntRect(widthOfSprite * int(currentFrame), heightOfSprite * 2, widthOfSprite, heightOfSprite));
            }
            else if (previousState == left) {
                sprite.setTextureRect(IntRect(widthOfSprite * int(currentFrame), heightOfSprite * 3, widthOfSprite, heightOfSprite));
            }
            previousState = state;
            state = jump;
           
        }
        if ((!Keyboard::isKeyPressed(Keyboard::Left)) && (!Keyboard::isKeyPressed(Keyboard::Right)) && (!Keyboard::isKeyPressed(Keyboard::Up)) && (!Keyboard::isKeyPressed(Keyboard::Space)) && isOnGround) {
            speed = 0;
            currentFrame = 0;
            if (previousState == right) {
                currentFrame = 0;
                sprite.setTextureRect(IntRect(0, 0, widthOfSprite, heightOfSprite));
            }
            else if (previousState == left) {
                currentFrame = 0;
                sprite.setTextureRect(IntRect(0, heightOfSprite, widthOfSprite, heightOfSprite));
            }
            previousState = state;
            state = stay;
        }
        
    }


    float getX() {
        return x;
    }
    float getY() {
        return y;
    }
    int getScore() {
        return score;
    }
    bool getStateOfTheGame() {
        return isEndOfTheGame;
    }
};


class Item : public Entity {
private:
    float timeOfAnimation, counterOfTime;
    int numOfAnimations;
public:
    Item() {
        timeOfAnimation = 0.f, counterOfTime = 0.f;
        numOfAnimations = 0;
    }
    Item(Image image, Texture texture, Sprite sprite, int width, int height, int numOfAnimations) : Entity(image, texture, sprite, width, height) {
        timeOfAnimation = 0.f;
        counterOfTime = 0.f;
        this->numOfAnimations = numOfAnimations;
    }
    Item(string wayToTheImage, int width, int height, int numOfAnimations) : Entity(wayToTheImage, width, height) {
        timeOfAnimation = 0.f;
        counterOfTime = 0.f;
        this->numOfAnimations = numOfAnimations;
    }
    Item(Entity& entity, string wayToTheImage, int width, int height, int numOfAnimations) : Entity(entity, wayToTheImage, width, height) {
        timeOfAnimation = 0.f;
        counterOfTime = 0.f;
        this->numOfAnimations = numOfAnimations;
    }
    void animation(TimeOfGame &timeOfGame, float counterOfItems) {
        counterOfTime += timeOfGame.getTime();

        if (counterOfTime > timeOfAnimation) {
            currentFrame += (0.005f * timeOfGame.getTime()) / counterOfItems;
            if (currentFrame > numOfAnimations) { currentFrame -= numOfAnimations; counterOfTime = 0; }
            sprite.setTextureRect(IntRect(widthOfSprite * int(currentFrame), 0, widthOfSprite, heightOfSprite));
        }
        else {
            sprite.setTextureRect(IntRect(0, 0, widthOfSprite, heightOfSprite));
        }
    }
    void setTimeOfAnimation(float timeOfAnimation) {
        this->timeOfAnimation = timeOfAnimation;
    }
    void setNumOfAnimation(int numOfAnimations) {
        this->numOfAnimations = numOfAnimations;
    }
    float getTimeOfAnimation() {
        return timeOfAnimation;
    }
};

class IState {
public:
    IState() = default;
    ~IState() { }
    virtual void update(TimeOfGame& timeOfGame, RenderWindow& window, stateOfGame& state) = 0;
    virtual void handleEvent(stateOfGame& state) = 0;
    virtual void draw(RenderWindow & window, TimeOfGame& timeOfGame) = 0;
};

class StateMenu : public IState {
private:
    Entity select, menuBackground;
    stateOfMenu choice;
public:
    StateMenu() : IState() {
        select.setWayToTheImage("res/txrs/menu/banana(96x96).png");
        menuBackground.setWayToTheImage("res/txrs/menu/menuBg.png");
        select.setOrigin(48, 48);
        menuBackground.setPositionOfSprite(0, 0);
        select.setPositionOfSprite(3000, 3000);
        
    }
    ~StateMenu() {

    }
   void update(TimeOfGame& timeOfGame, RenderWindow& window, stateOfGame &state) {
       menuBackground.setPositionOfSprite(0, 0);
       select.setPositionOfSprite(3000, 3000);
        if (IntRect(440, 190, 335, 90).contains(Mouse::getPosition(window))) { select.setPositionOfSprite(387, 242); choice = toAbout; }
        if (IntRect(440, 310, 335, 90).contains(Mouse::getPosition(window))) { select.setPositionOfSprite(387, 362); choice = toPlay; }
        if (IntRect(440, 430, 335, 90).contains(Mouse::getPosition(window))) { select.setPositionOfSprite(387, 482); choice = toExitOfGame; }

        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (choice == toAbout) { state = about; }
            if (choice == toPlay) { state = play; }
            if (choice == toExitOfGame) { state = exitOfGame; window.close(); }

        }

    }
    void handleEvent(stateOfGame& state) {

    }
    void draw(RenderWindow& window, TimeOfGame& timeOfGame) {
        window.draw(menuBackground.getSprite());
        window.draw(select.getSprite());

        window.display();
    }
};

class StateAbout : public IState {
private:
    Entity project, aboutBackground, runningMonkey;
    float currentFrameOfMonkey;
public:
    StateAbout() : IState() {
        currentFrameOfMonkey = 0.f;
        project.setWayToTheImage("res/txrs/about/nure.png");
        aboutBackground.setWayToTheImage("res/txrs/about/about.png");
        runningMonkey.setWayToTheImage("res/txrs/about/Hero(300x300).png");

        Texture projectTexture, aboutBackgroundTexture, runningMonkeyTexture;
        aboutBackground.setPositionOfSprite(0, 0);
        runningMonkey.setOrigin(150, 150);
        runningMonkey.setPositionOfSprite(200, 280);
    }

    ~StateAbout() {

    }
    void update(TimeOfGame& timeOfGame, RenderWindow& window, stateOfGame& state) {
        timeOfGame.refreshTime();
        currentFrameOfMonkey += 0.007f * timeOfGame.getTime();
        if (currentFrameOfMonkey > 3) currentFrameOfMonkey -= 3;
        runningMonkey.setTextureRect(300 * int(currentFrameOfMonkey), 0, 300, 300);
        if (IntRect(440, 430, 335, 90).contains(Mouse::getPosition(window))) { project.setPositionOfSprite(411, 432); }
        else {
            project.setPositionOfSprite(3000, 3000);
        }
    }
    void handleEvent(stateOfGame& state) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            state = menu;
        }
    }
    void draw(RenderWindow& window, TimeOfGame& timeOfGame) {
        window.draw(aboutBackground.getSprite());
        window.draw(runningMonkey.getSprite());
        window.draw(project.getSprite());

        window.display();
    }

};

class StatePlay : public IState {
private:
    Entity map;
    Item banana, stackOfBananas, goldCup;
    Hero hero;
    Font font;
    Text score;

public:
    StatePlay() : IState() {

        map.setWayToTheImage("res/txrs/map/TileMapNew.png");
        

        banana.setWayToTheImage("res/txrs/items/Banana(40x40).png");
        banana.setWidthAndHeight(40, 40);
        banana.setNumOfAnimation(6);
        banana.setTimeOfAnimation(12000);

        stackOfBananas.setWayToTheImage("res/txrs/items/Bananas(60x60).png");
        stackOfBananas.setWidthAndHeight(60, 60);
        stackOfBananas.setNumOfAnimation(6);
        stackOfBananas.setTimeOfAnimation(12000);

        goldCup.setWayToTheImage("res/txrs/items/GoldCup(50x50).png");
        goldCup.setWidthAndHeight(50, 50);
        goldCup.setNumOfAnimation(8);
        goldCup.setTimeOfAnimation(6000);

        hero.setWayToTheImage("res/txrs/alive/Hero(100x100).png");
        hero.setOrigin(50, 50);
        hero.setDXY(0.f, 0.f);
        hero.setXY(100.f, 2290.f);
        hero.setPositionOfSprite(150.f, 2340.f);
        hero.setSpeed(0.2f);
        hero.setWidthAndHeight(100, 100);
        
        view.reset(FloatRect(0, 0, 800, 600));
        hero.setTextureRect(0, 0, 100, 100);
        mapObjectsCounter();

        font.loadFromFile("res/fonts/old_pixel-7.ttf");
        score.setCharacterSize(65);
        score.setFont(font);
        score.setFillColor(Color::Yellow);
        score.setStyle(Text::Bold);

    }

    ~StatePlay() {

    }
    void update(TimeOfGame& timeOfGame, RenderWindow& window, stateOfGame& state) {
        if (hero.getStateOfTheGame()) {
            state = win;
        }
        
        timeOfGame.refreshTime();
        window.setView(view);

   
            hero.moveUpdate(timeOfGame);


        ostringstream heroScore;
        heroScore << hero.getScore();
        score.setString("Score " + heroScore.str());
        score.setPosition(view.getCenter().x - 90, view.getCenter().y - 300);
        
    }
    void handleEvent(stateOfGame& state) {
        if (hero.getStateOfTheGame()) {
            state = win;
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            state = menu;
        }
    }
    void draw(RenderWindow& window, TimeOfGame& timeOfGame) {
        window.clear();
        for (int i = 0; i < HEIGHT_MAP; i++) {
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (TileMap[i][j] == ' ') map.setTextureRect(0, 0, 50, 50);
                if (TileMap[i][j] == '1') map.setTextureRect(250, 0, 50, 50);
                if (TileMap[i][j] == '0') map.setTextureRect(50, 0, 50, 50);
                if (TileMap[i][j] == 'd') map.setTextureRect(300, 0, 50, 50);
                if (TileMap[i][j] == '=') map.setTextureRect(100, 0, 50, 50);
                if (TileMap[i][j] == '<') map.setTextureRect(150, 0, 50, 50);
                if (TileMap[i][j] == '>') map.setTextureRect(200, 0, 50, 50);

                if (TileMap[i][j] == 'e') {
                    banana.setPositionOfSprite((WIDTH_MAP + 1) * 50, (HEIGHT_MAP + 1) * 50);
                }

                map.setPositionOfSprite(j * 50, i * 50);
                window.draw(map.getSprite());
                if ((TileMap[i][j] == 'b')) {
                    banana.animation(timeOfGame, counterBananas);
                    banana.setPositionOfSprite(j * 50, i * 50);
                    window.draw(banana.getSprite());
                }
                if ((TileMap[i][j] == 's')) {
                    stackOfBananas.animation(timeOfGame, counterStackOfBananas);
                    stackOfBananas.setPositionOfSprite(j * 50, i * 50);
                    window.draw(stackOfBananas.getSprite());
                }
                if ((TileMap[i][j] == 'g')) {
                    goldCup.animation(timeOfGame, 1);
                    goldCup.setPositionOfSprite(j * 50, i * 50);
                    window.draw(goldCup.getSprite());
                }
            }
        }
        mapObjectsCounter();
        window.draw(hero.getSprite());
        window.draw(score);
        window.display();


    }

};

class StateWin : public IState {
private:
    Entity winBackground, cup;
    


public:
    StateWin() : IState() {
        winBackground.setWayToTheImage("res/txrs/win/win.png");
        cup.setWayToTheImage("res/txrs/win/cup(200x200).png");
        winBackground.setPositionOfSprite(0, 0);
        cup.setOrigin(150, 150);
        cup.setPositionOfSprite(457, 430);
    }

    ~StateWin() {

    }
    void update(TimeOfGame& timeOfGame, RenderWindow& window, stateOfGame& state) {
        static float currentFrameOfCup = 0.f;
        timeOfGame.refreshTime();
        currentFrameOfCup += 0.007f * timeOfGame.getTime();
        if (currentFrameOfCup > 8) currentFrameOfCup -= 8;
        cup.setTextureRect(200 * int(currentFrameOfCup), 0, 200, 200);
    }
    void handleEvent(stateOfGame& state) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            state = menu;
        }
    }
    void draw(RenderWindow& window, TimeOfGame& timeOfGame) {
        window.draw(winBackground.getSprite());
        window.draw(cup.getSprite());
        window.display();
    }

};





class Game {
private:
    stateOfGame state;
public:
    Game() {
        state = menu;
    }

    int game() {
        Image icon;
        icon.loadFromFile("res/icon/icon.png");
        sf::RenderWindow window(sf::VideoMode(800, 600), "Banana Jump");
        TimeOfGame timeOfGame(1000);
        window.setIcon(32, 32, icon.getPixelsPtr());

        IState* States[exitOfGame] = { new StatePlay, new StateAbout, new StateMenu, new StateWin };
        IState* current_state = States[menu];

        while (state != exitOfGame) {
            current_state = States[state];
            window.clear();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                    window.close();
            }
            timeOfGame.refreshTime();
            current_state->handleEvent(state);
            if (current_state != States[state]) {
                continue;
            }
            current_state->update(timeOfGame, window, state);
            current_state->draw(window, timeOfGame);
        }
        if (state == exitOfGame) {
           // delete[] States;
            window.close();
        }
        
        return 0;
    }
};

 /*
    int game() {
        Image icon;
        icon.loadFromFile("res/icon/icon.png");

        while (state != exitOfGame) {
            sf::RenderWindow window(sf::VideoMode(800, 600), "Banana Jump", Style::None);
            window.setIcon(32, 32, icon.getPixelsPtr());
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            switch (state) {
            case menu: loadMenu(window); break;
            case play: loadPlay(window); break;
            case about: loadAbout(window); break;
            case win: loadWin(window); break;
            case exitOfGame: window.close(); break;
            }
        } return 0;
    }

    void loadWin(RenderWindow& window) {
        window.clear();
        Texture winBackgroundTexture, cupTexture;
        winBackgroundTexture.loadFromFile("res/txrs/win/win.png");
        cupTexture.loadFromFile("res/txrs/win/cup(200x200).png");
        Sprite winBackground(winBackgroundTexture), cup(cupTexture);
        winBackground.setPosition(0, 0);
        cup.setOrigin(150, 150);
        cup.setPosition(457, 430);
        float currentFrameOfCup = 0;
        TimeOfGame timeWin(1000);

        while (state == win) {
            window.clear();
            state = win;
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                state = menu;
                break;
            }
            
            timeWin.refreshTime();
            currentFrameOfCup += 0.007f * timeWin.getTime();
            if (currentFrameOfCup > 8) currentFrameOfCup -= 8;
            cup.setTextureRect(IntRect(200 * int(currentFrameOfCup), 0, 200, 200));

            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    state = exitOfGame;
                }
            }
            window.draw(winBackground);
            window.draw(cup);

            window.display();
        }
    }


    void loadAbout(RenderWindow& window) {
        window.clear();
        Texture projectTexture, aboutBackgroundTexture, runningMonkeyTexture;
        projectTexture.loadFromFile("res/txrs/about/nure.png");
        aboutBackgroundTexture.loadFromFile("res/txrs/about/about.png");
        runningMonkeyTexture.loadFromFile("res/txrs/about/Hero(300x300).png");
        Sprite project(projectTexture), aboutBackground(aboutBackgroundTexture), runningMonkey(runningMonkeyTexture);
        aboutBackground.setPosition(0, 0);
        runningMonkey.setOrigin(150, 150);
        runningMonkey.setPosition(200, 280);
        float currentFrameOfMonkey = 0;
        TimeOfGame timeAbout(1000);

        while (state == about) {
            window.clear();
            state = about;
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                state = menu;
                break;
            }
            timeAbout.refreshTime();
            currentFrameOfMonkey += 0.007f * timeAbout.getTime();
            if (currentFrameOfMonkey > 3) currentFrameOfMonkey -= 3;
            runningMonkey.setTextureRect(IntRect(300 * int(currentFrameOfMonkey), 0, 300, 300));

            Event event;
            while (window.pollEvent(event))  {
                if (event.type == Event::Closed) {
                    state = exitOfGame;
                }
            }
            if (IntRect(440, 430, 335, 90).contains(Mouse::getPosition(window))) { project.setPosition(411, 432); }
            else {
                project.setPosition(3000, 3000);
            }
            window.draw(aboutBackground);
            window.draw(runningMonkey);
            window.draw(project);

            window.display();
        }
    }

    void loadMenu(RenderWindow& window) {
        window.clear();
        Texture select, menuBackground;
        select.loadFromFile("res/txrs/menu/banana(96x96).png");
        menuBackground.loadFromFile("res/txrs/menu/menuBg.png");
        Sprite selectSprite(select), menuBackgroundSprite(menuBackground);
        menuBackgroundSprite.setPosition(0, 0);
        selectSprite.setOrigin(48, 48);
        selectSprite.setPosition(3000, 3000);

        while (state == menu) {
            
            state = menu;
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    state = exitOfGame;
                }
            }
            window.clear();
            if (IntRect(440, 190, 335, 90).contains(Mouse::getPosition(window))) { selectSprite.setPosition(387, 242); choice = about; }
            if (IntRect(440, 310, 335, 90).contains(Mouse::getPosition(window))) { selectSprite.setPosition(387, 362); choice = play; }
            if (IntRect(440, 430, 335, 90).contains(Mouse::getPosition(window))) { selectSprite.setPosition(387, 482); choice = exitOfGame; }

            if (Mouse::isButtonPressed(Mouse::Left))
            {
                if (choice == about) { state = about; }
                if (choice == play) { state = play; }
                if (choice == exitOfGame) { state = exitOfGame; window.close(); }

            }

            window.draw(menuBackgroundSprite);
            window.draw(selectSprite);

            window.display();
        }
    }


    void loadPlay(RenderWindow& window) {

        Image mapImg;
        mapImg.loadFromFile("res/txrs/map/TileMapNew.png");
        Texture mapTexture;
        mapTexture.loadFromImage(mapImg);
        Sprite mapSprite;
        mapSprite.setTexture(mapTexture);

        Item banana("res/txrs/items/Banana(40x40).png", 40, 40, 6);
        banana.setTimeOfAnimation(12000);
       

        Item stackOfBananas("res/txrs/items/Bananas(60x60).png", 60, 60, 6);
        stackOfBananas.setTimeOfAnimation(12000);
       

        Item goldCup("res/txrs/items/GoldCup(50x50).png", 50, 50, 8);
        goldCup.setTimeOfAnimation(3000);
        

        Hero hero("res/txrs/alive/Hero(100x100).png", 0, 0, 100, 2290, 0.2f, 100, 100);
        
        

        view.reset(FloatRect(0, 0, 800, 600));

        hero.setTextureRect(0, 0, 100, 100);

        TimeOfGame timeOfGame(1000.f);


        mapObjectsCounter();

        font.loadFromFile("res/fonts/old_pixel-7.ttf");
        Text score("", font, 65);
        score.setFillColor(Color::Yellow);
        score.setStyle(Text::Bold);

       
        while (!Keyboard::isKeyPressed(Keyboard::Escape) && state != exitOfGame && state != win) {
            if (hero.getStateOfTheGame()) {
                state = win;
                break;
            }
            timeOfGame.refreshTime();
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    state = exitOfGame; 
                }
            }
            window.setView(view);
            window.clear();
            hero.moveUpdate(timeOfGame);
            for (int i = 0; i < HEIGHT_MAP; i++) {
                for (int j = 0; j < WIDTH_MAP; j++) {
                    if (TileMap[i][j] == ' ') mapSprite.setTextureRect(IntRect(0, 0, 50, 50));
                    if (TileMap[i][j] == '1') mapSprite.setTextureRect(IntRect(250, 0, 50, 50));
                    if (TileMap[i][j] == '0') mapSprite.setTextureRect(IntRect(50, 0, 50, 50));
                    if (TileMap[i][j] == 'd') mapSprite.setTextureRect(IntRect(300, 0, 50, 50));
                    if (TileMap[i][j] == '=') mapSprite.setTextureRect(IntRect(100, 0, 50, 50));
                    if (TileMap[i][j] == '<') mapSprite.setTextureRect(IntRect(150, 0, 50, 50));
                    if (TileMap[i][j] == '>') mapSprite.setTextureRect(IntRect(200, 0, 50, 50));

                    if (TileMap[i][j] == 'e') {
                        banana.setPositionOfSprite((WIDTH_MAP + 1) * 50, (HEIGHT_MAP + 1) * 50);
                    }

                            mapSprite.setPosition(j * 50, i * 50);
                    window.draw(mapSprite);
                    if ((TileMap[i][j] == 'b')) {
                        banana.animation(timeOfGame, counterBananas);
                        banana.setPositionOfSprite(j * 50, i * 50);
                        window.draw(banana.getSprite());
                    }
                    if ((TileMap[i][j] == 's')) {
                        stackOfBananas.animation(timeOfGame, counterStackOfBananas);
                        stackOfBananas.setPositionOfSprite(j * 50, i * 50);
                        window.draw(stackOfBananas.getSprite());
                    }
                    if ((TileMap[i][j] == 'g')) {
                        goldCup.animation(timeOfGame, 1);
                        goldCup.setPositionOfSprite(j * 50, i * 50);
                        window.draw(goldCup.getSprite());
                    }
                    
                }
            }
            mapObjectsCounter();
            ostringstream heroScore;
            heroScore << hero.getScore();
            score.setString("Score " + heroScore.str());
            score.setPosition(view.getCenter().x - 90, view.getCenter().y - 300);
            
            window.draw(hero.getSprite());
            window.draw(score);
            window.display();
        }
    } 
    
};
*/


int main() {
    Game game;
    game.game();
	

	return 0;
}