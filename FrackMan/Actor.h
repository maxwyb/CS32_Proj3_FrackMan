#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

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


class StudentWorld;


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


class Protester {
public:
    virtual void receiveGold();
    virtual void shout();
    
private:
    int hit_points;
    
    int num_to_move_in_direction; // numSquaresToMoveInCurrentDirection
    bool leavingOilField;
    int moving_period;
    int ticks; // total ticks undergone
};


class RegularProtester : public Protester {
public:
    
private:
    
};


class HardcoreProtester : public Protester {
public:
    
private:
    
};


class Goodies : public GraphObject {
public:
    void pickItUp();
    
private:
    int pickup_able; // 0 for pickup-able by FrackMan, 1 for pickup-able by Protesters, 2 for both
    int ticks_existing; // already existing for how many ticks; -1 for permanent state
};


class SonarKits : public Goodies {
public:
    
private:
    
};


class GoldNugget : public Goodies {
public:
    GoldNugget(int state);
    
private:
    
};


class Water : public Goodies {
public:
    
private:

};


class Barrel : public Goodies {
public:
    
private:
    
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
