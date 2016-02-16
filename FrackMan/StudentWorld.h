#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

/*
 class GameWorld {
 public:
	unsigned int getLives() const;
	void decLives();
	void incLives();
	unsigned int getScore() const;
	unsigned int getLevel() const;
	void increaseScore(unsigned int howMuch);
	void setGameStatText(string text);
	bool getKey(int& value);
	void playSound(int soundID);
 }
 */


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}

	virtual int init()
	{
        m_player = new FrackMan(this);
        
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                if ((i >= 30 && i <= 33 && j>= 4 && j<=59) || j > 59) {
                    m_dirt[i][j] = nullptr;
                    continue;
                }
                
                m_dirt[i][j] = new Dirt(this, i, j);
            }
        }
        
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
        
		//decLives();
        m_player->doSomething();
        
		return GWSTATUS_CONTINUE_GAME;
 	}

	virtual void cleanUp()
	{
	}
    
    void setHP(int num) { m_HP = num; }
    
    void changeHP(int num) { m_HP += num; }
    
    void setWater(int num) { m_water = num; }
    
    void changeWater(int num) { m_water += num; }
    
    void setSonar(int num) { m_sonar = num; }
    
    void changeSonar(int num) { m_sonar += num; }
    
    void setGold(int num) { m_gold = num; }
    
    void changeGold(int num) { m_gold += num; }
    
    void changeOilLeft(int num) { m_oilLeft += num; }
    
    
    Dirt* getDirt(int x, int y) { return m_dirt[x][y]; }
    
    void setDirt(Dirt* dirt, int x, int y) { m_dirt[x][y] = dirt; }

private:
    int m_HP; // hit_points of FrackMan
    int m_water; // unit_water
    int m_sonar; // sonar_charge
    int m_gold; // gold_nugget
    int m_oilLeft; // remained barrels of oil
    
    Dirt* m_dirt[64][64]; // map of Dirt
    
    std::vector<Actor*> m_actors;
    FrackMan* m_player;
};

#endif // STUDENTWORLD_H_


