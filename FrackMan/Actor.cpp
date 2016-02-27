#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
#include <queue>

using namespace std;

// *** Non-member Functions *** //

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

bool didCollide(int x1, int y1, int x2, int y2) { // check if two 4*4 objects collide
    for (int i1 = 0; i1 < 4; i1++)
        for (int j1 = 0; j1 < 4; j1++)
            for (int i2 = 0; i2 < 4; i2++)
                for (int j2 = 0; j2 < 4; j2++) {
                    if (x1+i1 == x2+i2 && y1+j1 == y2+j2)
                        return true;
                }
    return false;
}


list<Coord> getMazePath(string maze[], int sr, int sc, int er, int ec) {
    const int nRows = 64, nCols = 64;
    list<Coord> pop; // store all popped Coords in the search process
    
    queue<Coord> coordQueue;
    Coord start(sr, sc);
    coordQueue.push(start);
    maze[sr][sc] = '0';
    
    while (!coordQueue.empty()) {
        Coord current = coordQueue.front();
        pop.push_front(current);
        coordQueue.pop();
        
        if (current.r() == er && current.c() == ec) {
            
            // generate path from end-point to start-point
            list<Coord> path;
            for (list<Coord>::iterator it = pop.begin(); it != pop.end(); it++) {
                if (path.empty()) {
                    path.push_front(*it);
                    continue;
                }
                
                if (((*it).r() == path.front().r() && (*it).c() == path.front().c() + 1) ||
                    ((*it).r() == path.front().r() && (*it).c() == path.front().c() - 1) ||
                    ((*it).r() == path.front().r() + 1 && (*it).c() == path.front().c()) ||
                    ((*it).r() == path.front().r() - 1 && (*it).c() == path.front().c())) {
                    path.push_front(*it);
                }
            }
//            // ..Debug..
//            cerr << "non-member function: getMazePath() " << endl;
//            for (list<Coord>::iterator it = path.begin(); it != path.end(); it++){
//                cerr << "path: (" <<(*it).r() << ", " << (*it).c() << ")" << endl;
//            }
            
            return path;
        }
        
        // north
        if (current.r() - 1 >= 0 && maze[current.r()-1][current.c()] == '.'){
            Coord next(current.r()-1, current.c());
            coordQueue.push(next);
            maze[current.r()-1][current.c()] = '0';
        }
        // east
        if (current.c() + 1 < nCols && maze[current.r()][current.c()+1] == '.') {
            Coord next(current.r(), current.c()+1);
            coordQueue.push(next);
            maze[current.r()][current.c()+1] = '0';
        }
        // south
        if (current.r() + 1 < nRows && maze[current.r()+1][current.c()] == '.') {
            Coord next(current.r()+1, current.c());
            coordQueue.push(next);
            maze[current.r()+1][current.c()] = '0';
        }
        // west
        if (current.c() - 1 >= 0 && maze[current.r()][current.c()-1] == '.') {
            Coord next(current.r(), current.c()-1);
            coordQueue.push(next);
            maze[current.r()][current.c()-1] = '0';
        }
    }
    
    // no solution
    list<Coord> empty;
    return empty;
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

// *** Goodies *** //

Goodies::Goodies(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) : Actor(world, imageID, startX, startY, dir, size, depth) {
    
}

Goodies::~Goodies() {
    
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
    
    // dig the Dirt
    cerr << "Current X = " << getX() << " ; current Y = " << getY() << "." << endl;
    for (int i = getX(); i <= getX() + 3; i++) {
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
        int wantMoveX = -1, wantMoveY = -1;
        bool willMove = true;
        
        if (keyboard == KEY_PRESS_ESCAPE) {
            
            cerr << "ESCAPE key pressed" << endl;
            getWorld()->setHP(0);
            setDead();
            return;
            
        } else if (keyboard == KEY_PRESS_SPACE) {
            
            if (getWorld()->getWater() > 0) {
                
                getWorld()->changeWater(-1);
                getWorld()->playSound(SOUND_PLAYER_SQUIRT);
                
                // add a Squirt in proper direction, if possible
                int squirtX = -1, squirtY = -1;
                if (getDirection() == left && getX()-4 >= 0) {
                    squirtX = getX() - 4;
                    squirtY = getY();
                } else if (getDirection() == right && getX()+4 <= 60) {
                    squirtX = getX() + 4;
                    squirtY = getY();
                } else if (getDirection() == up && getY()+4 <= 60) {
                    squirtX = getX();
                    squirtY = getY() + 4;
                } else if (getDirection() == down && getY()-4 >= 0) {
                    squirtX = getX();
                    squirtY = getY() - 4;
                }
            
                if (squirtX != -1 && squirtY != -1) {
                    Squirt* aSquirt = new Squirt(getWorld(), squirtX, squirtY, getDirection());
                    getWorld()->addSquirt(aSquirt);
                }
            }
            
        } else if (keyboard == KEY_PRESS_LEFT) {
            
            if (getDirection() != left)
                setDirection(left);
            else if (getX()-1 >= 0) {
                wantMoveX = getX() - 1;
                wantMoveY = getY();
                
            //cerr << "LEFT arrow key pressed." << endl;
            }
            
        } else if (keyboard == KEY_PRESS_RIGHT) {
            
            if (getDirection() != right)
                setDirection(right);
            else if (getX()+1 <= 60) {
                wantMoveX = getX() + 1;
                wantMoveY = getY();
                
            //cerr << "RIGHT arrow key pressed." << endl;
            }
            
        } else if (keyboard == KEY_PRESS_UP) {
            
            if (getDirection() != up)
                setDirection(up);
            else if (getY()+1 <= 60) {
                wantMoveX = getX();
                wantMoveY = getY() + 1;
            }
            //cerr << "UP arrow key pressed." << endl;
            
        } else if (keyboard == KEY_PRESS_DOWN) {
            
            if (getDirection() != down)
                setDirection(down);
            else if (getY()-1 >= 0) {
                wantMoveX = getX();
                wantMoveY = getY() - 1;
            }
            
            //cerr << "DOWN arrow key pressed." << endl;
            
        } else if (keyboard == KEY_PRESS_TAB) {
            
            if (getWorld()->getGold() > 0) {
                GoldNugget* aGold = new GoldNugget(getWorld(), getX(), getY(), 2);
                getWorld()->addGoldNugget(aGold);
                getWorld()->changeGold(-1);
            }
            
        } else if (keyboard == 122) { // "Z" key
            
            cerr << "User pressed 'Z' key." << endl;
            if (getWorld()->getSonar() > 0) {
                getWorld()->changeSonar(-1);
            
                cerr << "Finding Gold Nugget close to FrackMan..." << endl;
                for (int i = 0; i < getWorld()->nGolds(); i++) {
                    cerr << "Gold Nugget i = " << i << ", distance = " << distance(getX(), getY(), getWorld()->getGoldNugget(i)->getX(), getWorld()->getGoldNugget(i)->getY()) << endl;
                    if (distance(getX(), getY(), getWorld()->getGoldNugget(i)->getX(), getWorld()->getGoldNugget(i)->getY()) < 12) {
                        getWorld()->getGoldNugget(i)->setVisible(true);
                        cerr << "Found a Gold Nugget near; set it visible." << endl;
                    }
                }
            
                cerr << "Finding Barrels of Oil close to FrackMan..." << endl;
                for (int i = 0; i < getWorld()->nBarrels(); i++) {
                    cerr << "Barrel i = " << i << ", distance = " << distance(getX(), getY(), getWorld()->getBarrel(i)->getX(), getWorld()->getBarrel(i)->getY()) << endl;
                    if (distance(getX(), getY(), getWorld()->getBarrel(i)->getX(), getWorld()->getBarrel(i)->getY()) < 12) {
                        getWorld()->getBarrel(i)->setVisible(true);
                        cerr << "Found a Barrel near; set it visible." << endl;
                    }
                }
            }
            
        } else {
            cerr << "User pressed key: keyboard = " << keyboard << endl;
        }
        
        // check if there is a Boulder on the desired-to-move position; if so, do not move
        for (int i = 0; i < getWorld()->nBoulders(); i++) {
            //if ((wantMoveX > getWorld()->getBoulder(i)->getX()-4 && wantMoveX < getWorld()->getBoulder(i)->getX()+4) && (wantMoveY > getWorld()->getBoulder(i)->getY()-4 && wantMoveY < getWorld()->getBoulder(i)->getY()+4)) {
            if (didCollide(wantMoveX, wantMoveY, getWorld()->getBoulder(i)->getX(), getWorld()->getBoulder(i)->getY())) {
                willMove = false;
                cerr << "willMove = false; FrackMan not moving this tick." << endl;
                break;
            }
        }
        
        if (willMove && (wantMoveX != -1 && wantMoveY != -1))
            moveTo(wantMoveX, wantMoveY);
    }
}

void FrackMan::getAnnoyed(int num) {
    getWorld()->changeHP(-1 * num);
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
    cerr << "A Boulder constructed at x = " << x << ", y = " << y << endl;
    m_state = 0;
    ticks = 0;
}

Boulder::~Boulder() {
    
}

void Boulder::doSomething() {
    if (!isAlive())
        return;
    
    if (m_state == 0) { // stable state
        
        bool isDirt = false;
        for (int i = 0; i < 4; i++) {
            if (getY()-1 >= 0 && getX()+i < 64 && getWorld()->getDirt(getX()+i, getY()-1) != nullptr)
                isDirt = true;
        }
        
        if (!isDirt) {
            m_state = 1;
            cerr << "Boulder entered State 1 (waiting)." << endl;
        }
        
    } else if (m_state == 1) { // waiting state
        
        ticks++;
        if (ticks >= 30) {
            m_state = 2;
            getWorld()->playSound(SOUND_FALLING_ROCK);
            cerr << "Boulder entered State 2 (falling)." << endl;
        }
        
    } else if (m_state == 2) { // falling state
        
        moveTo(getX(), getY() - 1);
        if (distance(getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY(), getX(), getY()) <= 3) { // collides with the FrackMan
            getWorld()->setHP(0);
            getWorld()->getPlayer()->setDead();
            setDead();
        }
        
        for (int i = 0; i < getWorld()->nProtesters(); i++) { // collides with a Protester
            if (distance(getX(), getY(), getWorld()->getProtester(i)->getX(), getWorld()->getProtester(i)->getY()) <= 3) {
                getWorld()->getProtester(i)->getAnnoyed(2);
            }
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
            if (getY() == aBoulder->getY()+1 && getX() >= aBoulder->getX() && getX() < aBoulder->getX()+4) {
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

// *** Squirt *** //
Squirt::Squirt(StudentWorld* world, int x, int y, Direction dir) : Actor(world, IID_WATER_SPURT, x, y, dir, 1, 1) {
    m_steps = 0;
}

Squirt::~Squirt() {
    
}

void Squirt::doSomething() {
    
    if (m_steps >= 4) {
        setDead();
        return;
    }
    
    int wantMoveX = -1, wantMoveY = -1;
    Direction dir = getDirection();
    switch (dir) {
        case left:
            if (getX() - 1 >= 0) {
                wantMoveX = getX() - 1;
                wantMoveY = getY();
            } else {
                setDead();
                return;
            }
            break;
            
        case right:
            if (getX() + 1 <= 60) {
                wantMoveX = getX() + 1;
                wantMoveY = getY();
            } else {
                setDead();
                return;
            }
            break;
            
        case up:
            if (getY() + 1 <= 60) {
                wantMoveX = getX();
                wantMoveY = getY() + 1;
            } else {
                setDead();
                return;
            }
            break;
            
        case down:
            if (getY() - 1 >= 0) {
                wantMoveX = getX();
                wantMoveY = getY() - 1;
            } else {
                setDead();
                return;
            }
            break;
            
        default:
            break;
    }
    
    // if it collides with a Dirt
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (getWorld()->getDirt(wantMoveX + i, wantMoveY + j) != nullptr) {
                cerr << "A Squirt collided with a Dirt." << endl;
                setDead();
                return;
            }
        }
    }
    
    // if it collides with a Boulder
    for (int i = 0; i < getWorld()->nBoulders(); i++) {
        if (didCollide(getX(), getY(), getWorld()->getBoulder(i)->getX(), getWorld()->getBoulder(i)->getY())) {
            cerr << "A Squirt collided with a Boulder." << endl;
            setDead();
            return;
        }
    }
    
    // if it collides with a Protester
    for (int i = 0; i < getWorld()->nProtesters(); i++) {
        if (distance(getX(), getY(), getWorld()->getProtester(i)->getX(), getWorld()->getProtester(i)->getY()) <= 3) {
            cerr << "A Squirt collided with a Protester." << endl;
            getWorld()->getProtester(i)->getAnnoyed(1);
            setDead();
            return;
        }
    }
    
    
    moveTo(wantMoveX, wantMoveY);
    m_steps++;
    return;
}


// *** Barrel *** //

Barrel::Barrel(StudentWorld* world, int x, int y) : Goodies(world, IID_BARREL, x, y, right, 1, 2) {
    cerr << "A Barrel constructed at x = " << x << ", y = " << y << endl;
    setVisible(false);
    isDiscovered = false;
}

Barrel::~Barrel() {
    
}

void Barrel::doSomething() {
    if (!isAlive())
        return;
    
    if (!isDiscovered && distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= 4) {
        cerr << "A Barrel is discovered." << endl;
        isDiscovered = true;
        setVisible(true);
        return;
    }
    
    if (distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= 3) {
        cerr << "A Barrel is picked up." << endl;
        setDead();
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->changeOilLeft(-1);
    }
}

// *** GoldNugget *** //
GoldNugget::GoldNugget(StudentWorld* world, int x, int y, int pickup) : Goodies(world, IID_GOLD, x, y, right, 1, 2) {
    m_pickup = pickup;
    m_ticks = 0;
    
    if (m_pickup == 1) {
        setVisible(false);
        m_isVisible = false;
    } else if (m_pickup == 2) {
        setVisible(true);
        m_isVisible = true;
    }
    
    cerr << "A Gold Nugget is constructed at x = " << x << ", y = " << y << endl;
}

GoldNugget::~GoldNugget() {
    
}

void GoldNugget::doSomething() {
    if (!isAlive())
        return;
    
    if (m_pickup == 2) { // pickup-able by Protesters
        m_ticks++;
        if (m_ticks >= 100) {
            setDead();
            return;
        }
        
        for (int i = 0; i < getWorld()->nProtesters(); i++) {
            if (distance(getX(), getY(), getWorld()->getProtester(i)->getX(), getWorld()->getProtester(i)->getY()) <= 3) {
                cerr << "A Gold Nugget is pick-up by a Protester." << endl;
                getWorld()->getProtester(i)->getBribed();
                setDead();
                return;
            }
        }
        
    } else if (m_pickup == 1) { // pickup-able by FrackMan
    
        if (!m_isVisible && distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= 4) {
        
            cerr << "A Gold Nugget pickup-able by FrackMan is discovered." << endl;
            setVisible(true);
            m_isVisible = true;
            return;
        
        } else if (distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= 3) {
        
            cerr << "A Gold Nugget is picked-up by FrackMan." << endl;
            setDead();
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getWorld()->changeGold(1);
            
        }
    }
    
}

// *** SonarKit *** //
SonarKit::SonarKit(StudentWorld* world, int x, int y) : Goodies(world, IID_SONAR, x, y, right, 1, 2) {
    m_ticks = 0;
}

SonarKit::~SonarKit() {
    
}

void SonarKit::doSomething() {
    if (!isAlive())
        return;
    
    if(distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= 3) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->changeSonar(1);
        getWorld()->increaseScore(75);
    }
    
    m_ticks++;
    int temp1 = 100, temp2 = 300 - getWorld()->getLevel()*10;
    int totalTick = temp1 > temp2 ? temp1 : temp2;
    if (m_ticks >= totalTick) {
        setDead();
        return;
    }
}


// *** Water Pool *** //
Water::Water(StudentWorld* world, int x, int y) : Goodies(world, IID_WATER_POOL, x, y, right, 1, 2) {
    m_ticks = 0;
}

Water::~Water() {
    
}

void Water::doSomething() {
    if (!isAlive())
        return;
    
    if(distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) <= 3) {
        setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->changeWater(5);
        getWorld()->increaseScore(100);
    }
    
    m_ticks++;
    int temp1 = 100, temp2 = 300 - getWorld()->getLevel()*10;
    int totalTick = temp1 > temp2 ? temp1 : temp2;
    if (m_ticks >= totalTick) {
        setDead();
        return;
    }
}


// *** Protester *** //

Protester::Protester(StudentWorld* world, int imageID) : Actor(world, imageID, 60, 60, left, 1, 0) {
    int temp1 = 0, temp2 = 3 - getWorld()->getLevel()/4;
    m_waitingTicks = (temp1 > temp2) ? temp1 : temp2;
    
    temp1= 50; temp2 = 100 - getWorld()->getLevel()*10;
    m_waitingTicksExtension = (temp1 > temp2) ? temp1 : temp2;
    
    m_ticks = 0; m_activeTicks = 0;
    m_ticksAfterShout = 15; m_ticksAfterRotate = 0; // Protester can shout just after construction
    
    m_isLeaving = false;
    m_isStunned = false;
    m_beingBribed = false;
    
    temp1 = 50, temp2 = 100 - getWorld()->getLevel()*10;
    m_ticksStaringGold = temp1 > temp2 ? temp1 : temp2;
    
    m_moveInDir = rand() % 53 + 8; // 8 to 60 inclusive
    cerr << "Constructed Protester with m_moveInDir = " << m_moveInDir << endl;
}

Protester::~Protester() {
    
}


bool Protester::addTick() {
    m_ticks++;
    
    if ((!m_isStunned && !m_beingBribed && m_ticks > m_waitingTicks) || (m_isStunned && m_ticks > m_waitingTicksExtension) || (m_beingBribed && m_ticks > m_ticksStaringGold)) {
        
        m_ticks = 0;
        m_activeTicks++;
        m_ticksAfterShout++;
        m_ticksAfterRotate++;
        
        if (m_isStunned)
            m_isStunned = false;
        if (m_beingBribed)
            m_beingBribed = false;
        
        return true;
    }
    return false;
}


void Protester::updateMap() {
    for (int i = 0; i < 64; i++) {
        m_map[i] = "";
        for (int j = 0; j < 64; j++)
            m_map[i] += '.';
    }
    
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 64; j++) {
            if (getWorld()->getDirt(i, j) != nullptr) {
                
                for (int a = -3; a <= 0; a++) // a 4*4 Actor at following positions would collide with a Dirt
                    for (int b = -3; b <= 0; b++) {
                        if (i+a >= 0 && j+b >= 0)
                            m_map[i+a][j+b] = 'D';
                    }
                continue;
            }
            
            for (int k = 0; k < getWorld()->nBoulders(); k++) // the current position is too close to an existing Boulder
                if (distance(i, j, getWorld()->getBoulder(k)->getX(), getWorld()->getBoulder(k)->getY()) <= 3)
                    m_map[i][j] = 'B';
        }
    
    // Debug: print out the map
//    cerr << "------------------------" << endl;
//    for (int i = 0; i < 64; i++) {
//        for (int j = 0; j < 64; j++)
//            cerr << m_map[i][j];
//        cerr << endl;
//    }
//    cerr << "------------------------" << endl;
    
}

bool Protester::isEmptyPoint(int x, int y) {
    if (x < 0 || y < 0 || x >= 64 || y >= 64 || m_map[x][y] != '.')
        return false;
    
//    for (int i = 0; i < 4; i++)
//        for (int j = 0; j < 4; j++) {
//            if (x + i >= 64 || y + j >= 64 || m_map[x+i][y+j] != '.')
//                return false;
//        }
    return true;
}

bool Protester::shoutAtPlayer() {
    
    int playerX = getWorld()->getPlayer()->getX();
    int playerY = getWorld()->getPlayer()->getY();
    
    if (distance(getX(), getY(), playerX, playerY) <= 4 && getTicksShout() > 15) {
        
        if ((getX() >= playerX && getY() >= playerY && (getDirection() == left || getDirection() == down)) ||
            (getX() < playerX && getY() >= playerY && (getDirection() == right || getDirection() == down)) ||
            (getX() < playerX && getY() < playerY && (getDirection() == right || getDirection() == up)) ||
            (getX() >= playerX && getY() < playerY && (getDirection() == left || getDirection() == up))) {
            // the Protester is facing the FrackMan's direction
            
            cerr << "A Protester shouted at the player." << endl;
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            getWorld()->getPlayer()->getAnnoyed(2);
            resetTicksShout();
            
            return true;
        }
    }
    
    return false;
}

bool Protester::walkToPlayerInSight() {
    
    bool canSeePlayer = false;
    int playerX = getWorld()->getPlayer()->getX();
    int playerY = getWorld()->getPlayer()->getY();
    
    if ( ((getX() == playerX) || (getY() == playerY)) && distance(getX(), getY(), playerX, playerY) > 4 ) { // if it is in the same line as the FrackMan
        int maxX = getX() >= playerX ? getX() : playerX;
        int minX = getX() < playerX ? getX() : playerX;
        int maxY = getY() >= playerY ? getY() : playerY;
        int minY = getY() < playerY ? getY() : playerY;
        
        bool noObstruct = true; // if it can directly walk to the FrackMan
        for (int i = minX; i <= maxX; i++) {
            for (int j = minY; j <= maxY; j++) {
                if (!isEmptyPoint(i, j)) {
                    noObstruct = false;
                    break;
                }
            }
            if (!noObstruct)
                break;
        }
        
        if (noObstruct) {
            canSeePlayer = true;
            
            if (playerX < getX()) {
                setDirection(left);
                moveTo(getX() - 1, getY());
            } else if (playerX > getX()) {
                setDirection(right);
                moveTo(getX() + 1, getY());
            } else if (playerY < getY()) {
                setDirection(down);
                moveTo(getX(), getY() - 1);
            } else if (playerY > getY()) {
                setDirection(up);
                moveTo(getX(), getY() + 1);
            }
            
            setMoveInDir(0);
        }
    }
    
    return canSeePlayer;
}


bool Protester::decreaseMoveAndRotate() {
    
    bool hasRotated = false;
    
    changeMoveInDir(-1);
    cerr << "m_moveInDir = " << m_moveInDir << " in Protester::decreaseMoveAndRotate()." << endl;
    
    if (getMoveInDir() <= 0) {
        bool canMove = false; // can move in new direction
        do {
            int dir = rand() % 4;
            if (dir == 0) {
                
                setDirection(left);
                if (getX() - 1 >= 0 && isEmptyPoint(getX()-1, getY())) {
                    canMove = true;
                }
                
            } else if (dir == 1) {
                
                setDirection(right);
                if (getX() + 1 <= 60 && isEmptyPoint(getX()+1, getY())) {
                    canMove = true;
                }
                
            } else if (dir == 2) {
                
                setDirection(up);
                if (getY() + 1 <= 60 && isEmptyPoint(getX(), getY()+1)) {
                    canMove = true;
                }
                
            } else if (dir == 3) {
                
                setDirection(down);
                if (getY() - 1 >= 0 && isEmptyPoint(getX(), getY()-1)) {
                    canMove = true;
                }
                
            }
        } while (!canMove);
        
        int new_moveInDir = rand() % 53 + 8;
        setMoveInDir(new_moveInDir);
        
        hasRotated = true;
    }
    
    return hasRotated;
    
}

bool Protester::rotateAtIntersection() {
//    cerr << "rotateAtIntersection(): m_ticksAfterRotate = " << getTicksRotate() << endl;
    if (getTicksRotate() > 200) { // rotate in an intersection if possible
        
        vector<Direction> possibleDir;
        if (getX() - 1 >= 0 && isEmptyPoint(getX()-1, getY()))
            possibleDir.push_back(left);
        if (getX() + 1 <= 60 && isEmptyPoint(getX()+1, getY()))
            possibleDir.push_back(right);
        if (getY() - 1 >= 0 && isEmptyPoint(getX(), getY()-1))
            possibleDir.push_back(down);
        if (getY() + 1 <= 60 && isEmptyPoint(getX(), getY()+1))
            possibleDir.push_back(up);
        
        if (possibleDir.empty())
            return false;
        
        int pos = rand() % (possibleDir.size());
        setDirection(possibleDir[pos]);
        
        int new_moveInDir = rand() % 53 + 8;
        setMoveInDir(new_moveInDir);
        
        resetTicksRotate();
        
        return true;
    }
    return false;
}

void Protester::moveInDir() {
    
    bool isBlocked = false;
    
    if (getDirection() == left) {
        if (getX() - 1 >= 0 && isEmptyPoint(getX()-1, getY()))
            moveTo(getX() - 1, getY());
        else
            isBlocked = true;
        
    } else if (getDirection() == right) {
        if (getX() + 1 <= 60 && isEmptyPoint(getX()+1, getY()))
            moveTo(getX() + 1, getY());
        else
            isBlocked = true;
        
    } else if (getDirection() == up) {
        if (getY() + 1 <= 60 && isEmptyPoint(getX(), getY()+1))
            moveTo(getX(), getY() + 1);
        else
            isBlocked = true;
        
    } else if (getDirection() == down) {
        if (getY() -1 >= 0 && isEmptyPoint(getX(), getY()-1))
            moveTo(getX(), getY() - 1);
        else
            isBlocked = true;
        
    }
    
    if (isBlocked) {
        setMoveInDir(0);
    }
}


void Protester::getAnnoyed(int actorType, int scrSquirt, int scrBoulder) {
    
    getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    m_isStunned = true;
    m_ticks = 0;
    if (actorType == 1) {
        m_HP -= 2;
        cerr << "A Protester is annoyed by actorType = " << actorType << "; remaining HP = " << m_HP << endl;
        getWorld()->increaseScore(scrSquirt);
    }
    else if (actorType == 2) {
        m_HP -= 100;
        cerr << "A Protester is annoyed by actorType = " << actorType << "; remaining HP = " << m_HP << endl;
        getWorld()->increaseScore(scrBoulder);
    }
}

void Protester::leaveOilField() { // move one step on the path of leaving oil field

    if (m_pathToExit.empty()) { // generate a path to exit
        list<Coord> path;
        path = getMazePath(m_map, getX(), getY(), 60, 60);
        
        for (list<Coord>::iterator it = path.begin(); it != path.end(); it++) {
            m_pathToExit.push_back(*it);
        }
        
        cerr << "A path generated to exit for Protester: ";
        for (list<Coord>::iterator it = m_pathToExit.begin(); it != m_pathToExit.end(); it++){
            cerr << " -> (" <<(*it).r() << ", " << (*it).c() << ")";
        }
        cerr << endl;
    }

    
    int posX = m_pathToExit.front().r(), posY = m_pathToExit.front().c();
    m_pathToExit.pop_front();
    
    if (posX == getX() - 1 && posY == getY())
        setDirection(left);
    else if (posX == getX() + 1 && posY == getY())
        setDirection(right);
    else if (posY == getY() - 1 && posX == getX())
        setDirection(down);
    else if (posY == getY() + 1 && posX == getX())
        setDirection(up);
    
    moveTo(posX, posY);
//    cerr << "Protestor moves to posX = " << posX << "; posY = " << posY << endl;
    if (posX == 60 && posY == 60) {
        setDead();
        return;
    }
}


// *** Regular Protester *** //
RegularProtester::RegularProtester(StudentWorld* world) : Protester(world, IID_PROTESTER) {
    setHP(5);
    cerr << "A Regular Protester is added." << endl;
}

RegularProtester::~RegularProtester() {
    
}

void RegularProtester::doSomething() {
    
    if (!isAlive())
        return;
    if (getHP() <= 0 && !isLeaving()) {
        cerr << "A Protester enters leaving state." << endl;
        setLeaving();
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setTicks(getWaitingTicksExtension() + 1);
        return;
    }
    
    if (!addTick()) // not an active tick
        return;
    
    updateMap();
    
    if (isLeaving()) {
        leaveOilField();
        return;
    }
    
//    updateMap();
    
    // check if to shout the FrackMan
    shoutAtPlayer();
    
    // check if walk toward the FrackMan
    bool canSeePlayer = walkToPlayerInSight();
    if (canSeePlayer)
        return;
    
    // decrease its moveInDir by 1, and change moving direction if its value <= 0
    bool hasRotated = decreaseMoveAndRotate();
    
    // change to another direction that can move >= 1 step
    if (!hasRotated)
        rotateAtIntersection();
    
    // try to move one step in current direction
    moveInDir();
    
}

void RegularProtester::getAnnoyed(int actorType) {
    Protester::getAnnoyed(actorType, 100, 500);
}

void RegularProtester::getBribed() {
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    setLeaving();
}


// *** Hardcore Protester *** //
HardcoreProtester::HardcoreProtester(StudentWorld* world) : Protester(world, IID_HARD_CORE_PROTESTER) {
    setHP(20);
    // *Debugging statement*:
    m_M = 16 + getWorld()->getLevel() * 2;
//    m_M = 200;
    
    cerr << "A Hardcore Protester is added." << endl;
}

HardcoreProtester::~HardcoreProtester() {
    
}

void HardcoreProtester::doSomething() {
    
    if (!isAlive())
        return;
    if (getHP() <= 0 && !isLeaving()) {
        cerr << "A Protester enters leaving state." << endl;
        setLeaving();
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setTicks(getWaitingTicksExtension() + 1);
        return;
    }
    
    if (!addTick()) // not an active tick
        return;
    
    updateMap();
    
    if (isLeaving()) {
        leaveOilField();
        return;
    }
    
//    updateMap();
    
    // check if to shout the FrackMan
    shoutAtPlayer();
    
    // unique to Hardcore Protester: pursue the FrackMan if possible
    bool pursuedPlayer = false;
    if (distance(getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY()) > 4) {
        pursuedPlayer = pursuePlayer();
    }
    if (pursuedPlayer)
        return;
    
    // check if walk toward the FrackMan
    bool canSeePlayer = walkToPlayerInSight();
    if (canSeePlayer)
        return;
    
    // decrease its moveInDir by 1, and change moving direction if its value <= 0
    bool hasRotated = decreaseMoveAndRotate();
    
    // change to another direction that can move >= 1 step
    if (!hasRotated)
        rotateAtIntersection();
    
    // try to move one step in current direction
    moveInDir();
}


void HardcoreProtester::getAnnoyed(int actorType) {
    Protester::getAnnoyed(actorType, 250, 500);
}

void HardcoreProtester::getBribed() {
    cerr << "A Hardcore Protester is bribed." << endl;
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    setBribed();
    return;
}

bool HardcoreProtester::pursuePlayer() {
    string m_map[64];
    for (int i = 0; i < 64; i++) {
        m_map[i] = "";
        for (int j = 0; j < 64; j++)
            m_map[i] += getMap(i, j);
    }
//    cerr << "pursuePlayer(): reading Map succeed." << endl;
    
    // Debug: print out map
//    cerr << "-----------------" << endl;
//    for (int i = 0; i < 64; i++) {
//        for (int j = 0; j < 64; j++)
//            cerr << m_map[i][j];
//        cerr << endl;
//    }
//    cerr << endl;
//    cerr << "-----------------" << endl;
    
    m_pathToPlayer = getMazePath(m_map, getX(), getY(), getWorld()->getPlayer()->getX(), getWorld()->getPlayer()->getY());
    
//    cerr << getX() << "; " << getY() << "; " << getWorld()->getPlayer()->getX() << "; " << getWorld()->getPlayer()->getY() << endl;
    
//    cerr << "pursuePlayer(): getMazePath to Player succeed." << endl;

    cerr << "Protester's current location: X = " << getX() << "; Y = " << getY() << endl;
    cerr << "m_M = " << m_M << endl;
    
    if (m_pathToPlayer.size()-1 <= m_M) {
        
        cerr << "m_pathToPlayer() = " << m_pathToPlayer.size() << endl;
        cerr << "A path generated to the FrackMan: ";
        for (list<Coord>::iterator it = m_pathToPlayer.begin(); it != m_pathToPlayer.end(); it++){
            cerr << " -> (" <<(*it).r() << ", " << (*it).c() << ")";
        }
        cerr << endl;
        cerr << "A Hardcore Protester begins to pursue the Frackman..." << endl;

        m_pathToPlayer.pop_front(); // first element (starting position) not useful
        
        int posX = m_pathToPlayer.front().r(), posY = m_pathToPlayer.front().c();
        m_pathToPlayer.pop_front();
        
        if (posX == getX() - 1 && posY == getY())
            setDirection(left);
        else if (posX == getX() + 1 && posY == getY())
            setDirection(right);
        else if (posY == getY() - 1 && posX == getX())
            setDirection(down);
        else if (posY == getY() + 1 && posX == getX())
            setDirection(up);
        
        moveTo(posX, posY);
        return true;
    }
    return false;
}



