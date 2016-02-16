#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>

using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// *** Actor *** //

Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : GraphObject(imageID, startX, startY, dir, size, depth) {
    
    m_world = world;
    m_isAlive = true;
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
    
    setVisible(true);
}

void FrackMan::doSomething() {
    if (!isAlive())
        return;
    
    cerr << "Current X = " << getX() << " ; current Y = " << getY() << "." << endl;
    for (int i = getX(); i <= getX()+ 3; i++) {
        for (int j = getY(); j <= getY() + 3; j++) {
            if (i < 64 && j < 64 && getWorld()->getDirt(i, j) != nullptr) {
                getWorld()->getDirt(i, j)->destroy();
                getWorld()->setDirt(nullptr, i, j);
            }
        }
    }
    
    int keyboard = 0;
    getWorld()->getKey(keyboard);
    //cerr << "Getting keyboard stroke..." << endl;
    switch (keyboard) {
        case KEY_PRESS_LEFT:
            if (getX()-1 >= 0) {
                moveTo(getX() - 1, getY());
                setDirection(left);
                //cerr << "LEFT arrow key pressed." << endl;
            }
            break;
        case KEY_PRESS_RIGHT:
            if (getX()+1 < 64) {
                moveTo(getX() + 1, getY());
                setDirection(right);
                //cerr << "RIGHT arrow key pressed." << endl;
            }
            break;
        case KEY_PRESS_UP:
            if (getY()+1 < 64) {
                moveTo(getX(), getY() + 1);
                setDirection(up);
                //cerr << "UP arrow key pressed." << endl;
            }
            break;
        case KEY_PRESS_DOWN:
            if (getY()-1 >= 0) {
                moveTo(getX(), getY() - 1);
                setDirection(down);
                //cerr << "DOWN arrow key pressed." << endl;
            }
            break;
//        default:
            //cerr << "NO direction key pressed." << endl;
        
    }
    
}

// *** Dirt *** //
Dirt::Dirt(StudentWorld* world, int x, int y) : Actor(world, IID_DIRT, x, y, right, 0.25, 3) {
    setVisible(true);
}

void Dirt::doSomething() {
    
}

void Dirt::destroy() {
    setVisible(false);
    delete this;
}

// *** Non-member Functions *** //
double getDistance(int x1, int y1, int x2, int y2) {
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
