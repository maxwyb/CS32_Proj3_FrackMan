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
        m_oilLeft = 2;
	}

	virtual int init()
	{
        std::cerr << "init() function called." << std::endl;
        m_player = new FrackMan(this);
        
        for (int i = 0; i < 64; i++) {
            for (int j = 0; j < 64; j++) {
                // leave a mineshaft without Dirt
                if ((i >= 30 && i <= 33 && j>= 4 && j<=59) || j > 59) {
                    m_dirt[i][j] = nullptr;
                    continue;
                }
                m_dirt[i][j] = new Dirt(this, i, j);
            }
        }
        
        std::cerr << "Dirt map construction finished. nullptr entries:" << std::endl;
        for (int i = 0; i < 64; i++)
            for (int j = 0; j < 64; j++)
                if (m_dirt[i][j] == nullptr)
                    std::cerr << "i = " << i << "; j = " << j << std::endl;
        
        setGameStatText(setDisplayText());
        
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
	{
        //std::cerr << "move() function called." << std::endl;
        setGameStatText(setDisplayText());
        m_player->doSomething();
        
        if (m_HP == 0) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        } else
            return GWSTATUS_CONTINUE_GAME;
 	}

	virtual void cleanUp()
	{
        std::cerr << "cleanUp() function called." << std::endl;
        
        for (int i = 0; i < 64; i++) 
            for (int j = 0; j < 64; j++)
                if (m_dirt[i][j] != nullptr) {
                    delete m_dirt[i][j];
                    m_dirt[i][j] = nullptr;
                }
        
        std::cerr << "Dirt map destruction finished. Non-nullptr entries:" << std::endl;
        for (int i = 0; i < 64; i++)
            for (int j = 0; j < 64; j++)
                if (m_dirt[i][j] != nullptr)
                    std::cerr << "i = " << i << "; j = " << j << std::endl;
        
        delete m_player;
	}
    
    
    // member functions for FrackMan-related member variables
    void setHP(int num) { m_HP = num; }
    
    void changeHP(int num) { m_HP += num; }
    
    void setWater(int num) { m_water = num; }
    
    void changeWater(int num) { m_water += num; }
    
    void setSonar(int num) { m_sonar = num; }
    
    void changeSonar(int num) { m_sonar += num; }
    
    void setGold(int num) { m_gold = num; }
    
    void changeGold(int num) { m_gold += num; }
    
    void changeOilLeft(int num) { m_oilLeft += num; }
    
    
    // member functions for Dirt-related member variables
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
    
    std::string setDisplayText();
};

#endif // STUDENTWORLD_H_


