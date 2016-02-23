#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"

#include <string>
#include <vector>
#include <cstdlib>

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
    
    virtual ~StudentWorld() {
        cleanUp();
    }

    virtual int init();
    
    virtual int move();
    
    virtual void cleanUp();
    
    // member functions for FrackMan-related member variables
    void setHP(int num) { m_HP = num; }
    
    void changeHP(int num) { m_HP += num; }
    
    int getWater() { return m_water; }
    
    void setWater(int num) { m_water = num; }
    
    void changeWater(int num) { m_water += num; }
    
    void setSonar(int num) { m_sonar = num; }
    
    void changeSonar(int num) { m_sonar += num; }
    
    void setGold(int num) { m_gold = num; }
    
    int getGold() { return m_gold; }
    
    void changeGold(int num) { m_gold += num; }
    
    void changeOilLeft(int num) { m_oilLeft += num; }
    
    
    // Dirt
    Dirt* getDirt(int x, int y) { return m_dirt[x][y]; }
    
    void setDirt(Dirt* dirt, int x, int y) { m_dirt[x][y] = dirt; }
    
    // FrackMan
    FrackMan* getPlayer() { return m_player; }
    
    // Boulder
    int nBoulders() { return m_boulders.size(); }
    
    Boulder* getBoulder(int num) { return m_boulders[num]; }
    
    // Squirt
    void addSquirt(Squirt* aSquirt) { m_squirts.push_back(aSquirt); }
    
    int nSquirts() { return m_squirts.size(); }
    
    Squirt* getSquirt(int num) { return m_squirts[num]; }
    
    // GoldNugget
    void addGoldNugget(GoldNugget* aGold) { m_golds.push_back(aGold); }
    

private:
    
    int m_HP; // hit_points of FrackMan
    int m_water; // unit_water
    int m_sonar; // sonar_charge
    int m_gold; // gold_nugget
    
    int m_oilLeft; // remained barrels of oil
    
    Dirt* m_dirt[64][64];
    std::vector<Boulder*> m_boulders;
    std::vector<Squirt*> m_squirts;
    std::vector<Barrel*> m_barrels;
    std::vector<GoldNugget*> m_golds;
    std::vector<SonarKit*> m_sonars;
    std::vector<Water*> m_waters;
    
    
    //std::vector<Actor*> m_actors;
    FrackMan* m_player;
    
    std::string setDisplayText();
};

#endif // STUDENTWORLD_H_
