#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

string StudentWorld::setDisplayText() {
    string text;
    
    // sub-string processing
    string scr_temp = to_string(getScore());
    string scr = "";
    for (int i = 0; i < 8 - scr_temp.size(); i++)
        scr += "0";
    scr += scr_temp;
    
    string lvl_temp = to_string(getLevel());
    string lvl = "";
    for (int i = 0; i < 2 - lvl_temp.size(); i++)
        lvl += " ";
    lvl += lvl_temp;
    
    string lives = to_string(getLives());
    
    string hth_temp = to_string(m_HP * 10);
    string hth = "";
    for (int i = 0; i < 3 - hth_temp.size(); i++)
        hth += " ";
    hth += hth_temp; hth += "%";
    
    string water_temp = to_string(m_water);
    string water = "";
    for (int i = 0; i < 2 - water_temp.size(); i++)
        water += " ";
    water += water_temp;
    
    string gold_temp = to_string(m_gold);
    string gold = "";
    for (int i = 0; i < 2 - gold_temp.size(); i++)
        gold += " ";
    gold += gold_temp;
    
    string sonar_temp = to_string(m_sonar);
    string sonar = "";
    for (int i = 0; i < 2 - sonar_temp.size(); i++)
        sonar += " ";
    sonar += sonar_temp;
    
    string oilLeft_temp = to_string(m_oilLeft);
    string oilLeft = "";
    for (int i = 0; i < 2 - oilLeft_temp.size(); i++)
        oilLeft += " ";
    oilLeft += oilLeft_temp;
    
    // final displayed string
    text = "Str: " + scr + "  Lvl: " + lvl + "  Lives: " + lives + "  Hlth: " + hth + "  Water: " + water + "  Gld: " + gold + "  Sonar: " + sonar + "  Oil Left: " + oilLeft;
    return text;
}
