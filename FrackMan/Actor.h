#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
#include <list>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/*
 GraphObject(int imageID, int startX, int startY, DIRECTION startDirection, float size = 1.0, unsigned int depth = 0) {
 public:
	void setVisible(bool shouldIDisplay);
	void getX() const;
	void getY() const;
	void moveTo(int x, int y);
	DIRECTION getDirection() const; // Directions: up, down, left, right void
	setDirection(DIRECTION d); // Directions: up, down, left, right
 }
 */

// *** Other classes and functions *** //

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


double distance(int x1, int y1, int x2, int y2);

bool didCollide(int x1, int y1, int x2, int y2);

class StudentWorld;


// *** Actors *** //

class Actor : public GraphObject {
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~Actor();
    
    StudentWorld* getWorld();
    bool isAlive();
    void setDead();
    virtual void doSomething() = 0;
    
private:
    StudentWorld* m_world;
    bool m_isAlive;
};


class FrackMan : public Actor {
public:
    FrackMan(StudentWorld* world);
    virtual ~FrackMan();
    
    virtual void doSomething();
    void getAnnoyed(int num);
    
private:
    
};


class Protester : public Actor {
public:
    Protester(StudentWorld* world, int imageID);
    virtual ~Protester();
    virtual void doSomething() = 0;
    
    int getHP() { return m_HP; }
    void setHP(int num) { m_HP = num; };
    void changeHP(int num) { m_HP += num; };
    
    bool isLeaving() { return m_isLeaving; }
    void setLeaving() { m_isLeaving = true; }
    
    // tick-related functions
    void setTicks(int num) { m_ticks = num; }
    
    int getMoveInDir() { return m_moveInDir; }
    void setMoveInDir(int num) { m_moveInDir = num; }
    void changeMoveInDir(int num) { m_moveInDir += num; }
    
    void resetTicksShout() { m_ticksAfterShout = 0; }
    int getTicksShout() { return m_ticksAfterShout; }
    void resetTicksRotate() { m_ticksAfterRotate = 0; }
    int getTicksRotate() { return m_ticksAfterRotate; }
    
    int getWaitingTicksExtension() { return m_waitingTicksExtension; }
    
    // Map-related functions
    void updateMap();
    char getMap(int x, int y) { return m_map[x][y]; }
    bool isEmptyPoint(int x, int y);
    
    // sub-functions for doSomething()
    bool addTick(); // return true if to move at this tick
    
    void leaveOilField();
    
    bool shoutAtPlayer(); // return true if shouted
    bool walkToPlayerInSight(); // return true if the FrackMan is in sight
    bool decreaseMoveAndRotate(); // decrease m_moveInDir by 1; return true if it changes direction
    bool rotateAtIntersection(); // return true if it can, and do change moving direction at one point
    void moveInDir(); // move one step in current direction
    
    void getAnnoyed(int actorType, int scrSquirt, int scrBoulder); // a general function to be called by derived classes
    virtual void getAnnoyed(int actorType) = 0;
    
private:
    int m_HP;
    int m_moveInDir; // number of squares to move in current direction
    bool m_isLeaving; // state of leave-the-oil-field
    
    int m_ticks; // temporary, to keep track of which tick to move
    
    int m_activeTicks; // ticks except the waiting ticks
    int m_ticksAfterShout; // non-resting ticks undergone after last shout
    int m_ticksAfterRotate; // non-resting ticks undergone after last rotation
    
    int m_waitingTicks; // CONST ticks of waiting before each move
    int m_waitingTicksExtension; // CONST total ticks of waiting when stunned by Squirt
    
    bool m_isStunned;
    
    std::string m_map[64]; // contains Dirt and Boulder positions
    std::list<Coord> m_pathToExit;
};


class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld* world);
    virtual ~RegularProtester();
    virtual void doSomething();
    
    virtual void getAnnoyed(int actorType);
    
private:
    
};


class HardcoreProtester : public Protester {
public:
    HardcoreProtester(StudentWorld* world);
    virtual ~HardcoreProtester();
    virtual void doSomething();
    
    virtual void getAnnoyed(int actorType);
    
    
private:
    int m_M;
    int m_ticksStaringGold;
};


class Goodies : public Actor {
public:
    Goodies(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~Goodies();
    
    virtual void doSomething() = 0;
    
//    void pickItUp();
    
private:
//    int pickup_able; // 0 for pickup-able by FrackMan, 1 for pickup-able by Protesters, 2 for both
//    int ticks_existing; // already existing for how many ticks; -1 for permanent state
};


class SonarKit : public Goodies {
public:
    SonarKit(StudentWorld* world, int x, int y);
    virtual ~SonarKit();
    virtual void doSomething();
    
private:
    int m_ticks;
    
};


class GoldNugget : public Goodies {
public:
    GoldNugget(StudentWorld* world, int x, int y, int pickup);
    virtual ~GoldNugget();
    virtual void doSomething();
    
private:
    int m_pickup; // 1 for pickup-able by FrackMan, 2 for pickup-able by Protester
    int m_ticks; // ticks undergone when pickup-able by FrackMan
    bool m_isVisible;
    
};


class Water : public Goodies {
public:
    Water(StudentWorld* world, int x, int y);
    virtual ~Water();
    virtual void doSomething();
    
private:
    int m_ticks;
};


class Barrel : public Goodies {
public:
    Barrel(StudentWorld* world, int x, int y);
    virtual ~Barrel();
    
    virtual void doSomething();
    
private:
    bool isDiscovered;
};


class Boulder : public Actor {
public:
    Boulder(StudentWorld* world, int x, int y);
    virtual ~Boulder();
    
    virtual void doSomething();
    
private:
    int m_state; // 0 for stable, 1 for waiting, 2 for falling
    int ticks; // ticks undergone after in waiting state
    
};


class Dirt : public Actor {
public:
    Dirt(StudentWorld* world, int x, int y);
    virtual ~Dirt();
    
    virtual void doSomething();
    void destroy();
    
private:

};


class Squirt : public Actor {
public:
    Squirt(StudentWorld* world, int x, int y, Direction dir);
    virtual ~Squirt();
    
    virtual void doSomething();
    
private:
    int m_steps; // number of steps it has gone
    
};


#endif // ACTOR_H_
