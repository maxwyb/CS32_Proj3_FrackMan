#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

using namespace std;

// *** Non-member Functions *** //

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

// *** Actor *** //

Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : GraphObject(imageID, startX, startY, dir, size, depth) {
    
    m_world = world;
    m_isAlive = true;
    
    setVisible(true);
}

Actor::~Actor() {
    setVisible(false);
}

StudentWorld* Actor::getWorld() {
    return m_world;
}

bool Actor::isAlive() {
    return (m_isAlive);
}

void Actor::setDead() {
    m_isAlive = false;
}


// *** FrackMan *** //

FrackMan::FrackMan(StudentWorld* world) : Actor(world, IID_PLAYER, 30, 60, right, 1, 0) {
    world->setHP(10);
    world->setWater(5);
    world->setSonar(1);
    world->setGold(0);
    
}

FrackMan::~FrackMan() {

}

void FrackMan::doSomething() {
    if (!isAlive()) {
        getWorld()->decLives();
        return;
    }
    
    cerr << "Current X = " << getX() << " ; current Y = " << getY() << "." << endl;
    for (int i = getX(); i <= getX()+ 3; i++) {
        for (int j = getY(); j <= getY() + 3; j++) {
            if (i < 64 && j < 64 && getWorld()->getDirt(i, j) != nullptr) {
                getWorld()->getDirt(i, j)->destroy();
                getWorld()->setDirt(nullptr, i, j);
                getWorld()->playSound(SOUND_DIG);
            }
        }
    }
    
    int keyboard = 0;
    if (getWorld()->getKey(keyboard) == true) {
        //cerr << "Getting keyboard stroke..." << endl;
        switch (keyboard) {
            case KEY_PRESS_ESCAPE:
                cerr << "ESCAPE key pressed" << endl;
                getWorld()->setHP(0);
                setDead();
                return;
            
            case KEY_PRESS_SPACE:
                getWorld()->changeWater(-1);
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                break;
            
            case KEY_PRESS_LEFT:
                if (getDirection() != left)
                    setDirection(left);
                else if (getX()-1 >= 0) {
                    moveTo(getX() - 1, getY());
                    //cerr << "LEFT arrow key pressed." << endl;
                }
                break;
            
            case KEY_PRESS_RIGHT:
                if (getDirection() != right)
                    setDirection(right);
                else if (getX()+1 < 64) {
                    moveTo(getX() + 1, getY());
                    //cerr << "RIGHT arrow key pressed." << endl;
                }
                break;
            
            case KEY_PRESS_UP:
                if (getDirection() != up)
                    setDirection(up);
                else if (getY()+1 < 64) {
                    moveTo(getX(), getY() + 1);
                    //cerr << "UP arrow key pressed." << endl;
                }
                break;
            
            case KEY_PRESS_DOWN:
                if (getDirection() != down)
                    setDirection(down);
                else if (getY()-1 >= 0) {
                    moveTo(getX(), getY() - 1);
                    //cerr << "DOWN arrow key pressed." << endl;
                }
                break;
        }
    }
}

// *** Dirt *** //

Dirt::Dirt(StudentWorld* world, int x, int y) : Actor(world, IID_DIRT, x, y, right, 0.25, 3) {

}

Dirt::~Dirt() {

}

void Dirt::doSomething() {
    
}

void Dirt::destroy() {
    delete this;
}

// *** Boulder *** //

Boulder::Boulder(StudentWorld* world, int x, int y) : Actor(world, IID_BOULDER, x, y, down, 1, 1) {
    m_state = 0;
    ticks = 0;
}

Boulder::~Boulder() {
    
}

void Boulder::doSomething() {
    if (!isAlive())
        return;
    
    if (m_state == 0) {
        bool isDirt = false;
        for (int i = 0; i < 4; i++) {
            if (getY() - 1 >= 0 && getX() + i < 64 && getWorld()->getDirt(getX()+i, getY()-1) != nullptr)
                isDirt = true;
        }
        
        if (!isDirt) {
            m_state = 1;
            cerr << "Boulder entered State 1 (waiting)." << endl;
        }
    } else if (m_state == 1) {
        ticks++;
        if (ticks >= 30) {
            m_state = 2;
            getWorld()->playSound(SOUND_FALLING_ROCK);
            cerr << "Boulder entered State 2 (falling)." << endl;
        }
    } else if (m_state == 2) {
        moveTo(getX(), getY() - 1);
        if (distance(getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY(), getX(), getY()) <= 3) {
            getWorld()->setHP(0);
            getWorld()->getPlayer()->setDead();
        }
        
        bool isObstructed = false;
        for (int i = 0; i < 4; i++) {
            if ((getY() - 1 >= 0 && getX() + i < 64 && getWorld()->getDirt(getX()+i, getY()-1) != nullptr) || getY() - 1 == 0)  { // runs into a Dirt or falls to bottom
                isObstructed = true;
                break;
            }
        }
        
        for (int i = 0; i < getWorld()->nBoulders(); i++) { // runs into another Boulder
            Boulder* aBoulder = getWorld()->getBoulder(i);
            if (getY() == aBoulder->getY() + 1 && getX() >= aBoulder->getX() && getX() < aBoulder->getX() - 4) {
                isObstructed = true;
                break;
            }
        }
            
        if (isObstructed) {
            setDead();
            cerr << "Boulder entered State 3 (dead)." << endl;
            cerr << "Boulder.isAlive == " << isAlive() << endl;
        }
    }
}
