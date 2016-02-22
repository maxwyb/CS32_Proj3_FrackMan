#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}


int StudentWorld::init()
{
    m_player = new FrackMan(this);
    
    // Dirt
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
    
    // Boulder
    int temp1 = getLevel()/2 + 2, temp2 = 6;
    int B = temp1 < temp2 ? temp1 : temp2;
    cerr << "Number of Boulders this round = " << B << endl;
    
    for (int i = 0; i < B; i++) {
        int x = rand() % 64, y = rand() % 64;
        if ((y == 0 || x > 61 || y > 57) || (x >= 27 && x <= 33 && y>= 4 && y<=59)) { // at the bottom, out of Dirt's range or in the mineshaft
            i--;
            continue;
        }
        m_boulders.push_back(new Boulder(this, x, y));
    }
    
    for (int i = 0; i < nBoulders(); i++) { // remove Dirts in Boulder position
        int boulderX = getBoulder(i)->getX(), boulderY = getBoulder(i)->getY();
        for (int j = 0; j < 4; j++)
            for (int k = 0; k < 4; k++)
                if (boulderX + j < 64 && boulderY + k < 64) {
                    cerr << "m_dirt[" << boulderX + j << "][" << boulderY + k << "] is deleted because a Boulder occupies the point." << endl;
                    delete m_dirt[boulderX + j][boulderY + k];
                    m_dirt[boulderX + j][boulderY + k] = nullptr;
                }
    }

    // Barrel of oil
    temp1 = 2 + getLevel(), temp2 = 20;
    int L = temp1 < temp2 ? temp1 : temp2;
    m_oilLeft = L;
    
    for (int i = 0; i < L; i++) {
        int x = rand() % 64, y = rand() % 64;
        if ((x >= 30 && x <= 33 && y >= 4 && y <=59) || y > 59) { // the random position is not inside a Dirt
            i--;
            continue;
        }
        m_barrels.push_back(new Barrel(this, x, y));
    }
    
    
    setGameStatText(setDisplayText());
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (m_oilLeft == 0) {
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    setGameStatText(setDisplayText());
    
    // FrackMan
    m_player->doSomething();
    
    // Boulder
    for (int i = 0; i < m_boulders.size(); i++)
        m_boulders[i]->doSomething();
    
    for (int i = 0; i < m_boulders.size(); i++) {
        if (m_boulders[i]->isAlive() == false) {
            delete m_boulders[i];
            cerr << "Destructor of a Boulder called." << endl;
            m_boulders.erase(m_boulders.begin() + i);
        }
    }
    
    // Squirt
    for (int i = 0; i < m_squirts.size(); i++) {
        m_squirts[i]->doSomething();
    }
    
    for (int i = 0; i < m_squirts.size(); i++) {
        if (!m_squirts[i]->isAlive()) {
            delete m_squirts[i];
            m_squirts.erase(m_squirts.begin() + i);
        }
    }
    
    // Barrel of oil
    for (int i = 0; i < m_barrels.size(); i++) {
        m_barrels[i]->doSomething();
    }
    
    for (int i = 0; i < m_barrels.size(); i++) {
        if (m_barrels[i]->isAlive() == false) {
            delete m_barrels[i];
            m_barrels.erase(m_barrels.begin() + i);
        }
    }
    
    
    // Player: check life
    if (m_HP == 0) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    } else
        return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::cleanUp()
{
    // Dirt
    for (int i = 0; i < 64; i++)
        for (int j = 0; j < 64; j++)
            if (m_dirt[i][j] != nullptr) {
                delete m_dirt[i][j];
                m_dirt[i][j] = nullptr;
            }
    
    // FrackMan
    delete m_player;
    
    // Boulder
    for (int i = 0; i < m_boulders.size(); i++) {
        delete m_boulders[i];
        m_boulders.erase(m_boulders.begin()+i);
    }
    
    // Squirt
    for (int i = 0; i < m_squirts.size(); i++) {
        delete m_squirts[i];
        m_squirts.erase(m_squirts.begin()+i);
    }
    
    // Barrel
    for (int i = 0; i < m_barrels.size(); i++) {
        delete m_barrels[i];
        m_barrels.erase(m_barrels.begin()+i);
    }
}

string StudentWorld::setDisplayText() {
    string text;
    
    // sub-string processing
    string scr_temp = to_string(getScore());
    string scr = "";
    for (int i = 0; i < 6 - scr_temp.size(); i++)
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
    text = "Scr: " + scr + "  Lvl: " + lvl + "  Lives: " + lives + "  Hlth: " + hth + "  Wtr: " + water + "  Gld: " + gold + "  Sonar: " + sonar + "  Oil Left: " + oilLeft;
    return text;
}
