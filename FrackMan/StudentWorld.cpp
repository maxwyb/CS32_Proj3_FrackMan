#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

void StudentWorld::addAProtester() {
    int temp1 = 90, temp2 = getLevel()*10 + 30;
    int probOfHardcore = temp1 < temp2 ? temp1 : temp2;
    int prob = rand() % probOfHardcore;
    if (prob == 0) {
        m_protesters.push_back(new HardcoreProtester(this));
    } else {
        m_protesters.push_back(new RegularProtester(this));
    }
}

int StudentWorld::init()
{
    m_ticks = 0;
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
    
    
    // Gold Nugget
    temp1 = 5 - getLevel()/2; temp2 = 2;
    int G = temp1 > temp2 ? temp1 : temp2;
    
    for (int i = 0; i < G; i++) {
        int x = rand() % 64, y = rand() % 64;
        
        bool canConstruct = true;
        if ((x >= 30 && x <= 33 && y >= 4 && y <=59) || y > 59) { // the random position is not inside a Dirt
            canConstruct = false;
        }
        for (int i = 0; i < m_boulders.size(); i++) { // the random position collides with a Boulder
            if (didCollide(x, y, m_boulders[i]->getX(), m_boulders[i]->getY())) {
                canConstruct = false;
                break;
            }
        }
        if (!canConstruct) {
            cerr << "Attempt to construct a Gold Nugget but the random position is not legal; try again." << endl;
            i--;
            continue;
        }
        
        m_golds.push_back(new GoldNugget(this, x, y, 1));
    }
    
    // Protester
    addAProtester();
    
    setGameStatText(setDisplayText());
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    if (m_oilLeft == 0) {
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    m_ticks++;
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
    
    // Gold Nugget
    for (int i = 0; i < m_golds.size(); i++) {
        m_golds[i]->doSomething();
    }
    
    for (int i = 0; i < m_golds.size(); i++) {
        if (!m_golds[i]->isAlive()) {
            delete m_golds[i];
            m_golds.erase(m_golds.begin() + i);
        }
    }
    
    // Sonar Kit and Water Pool: construct and doSomething
    int prob1 = rand() % (getLevel()*25 + 300);
    if (prob1 == 0) {
        cerr << "Prepare to construct a Sonar Kit OR a Water Pool." << endl;
        int prob2 = rand() % 5;
        if (prob2 == 0) { // add Sonar Kit
            
            cerr << "Constructing a Sonar Kit..." << endl;
            SonarKit* aSonar = new SonarKit(this, 0, 60);
            m_sonars.push_back(aSonar);
            
        } else { // add Water Pool
            cerr << "Constructing a Water Pool..." << endl;
            
            vector<int> emptyX, emptyY;
            for (int i = 0; i < 61; i++) {
                for (int j = 0; j < 61; j++) {
                    bool isEmpty = true; // the 4*4 grid at (i, j) is empty
                    for (int a = 0; a < 4; a++) {
                        for (int b = 0; b < 4; b++) {
                            if (m_dirt[i+a][j+b] != nullptr) {
                                isEmpty = false;
                                break;
                            }
                        }
                        if (!isEmpty)
                            break;
                    }
                    
                    if (isEmpty) {
                        emptyX.push_back(i);
                        emptyY.push_back(j);
                    }
                }
            }
            
            int pos = rand() % emptyX.size();
            Water* aWater = new Water(this, emptyX[pos], emptyY[pos]);
            m_waters.push_back(aWater);
            
        }
    }
    
    for (int i = 0; i < m_sonars.size(); i++) {
        m_sonars[i]->doSomething();
    }
    
    for (int i = 0; i < m_waters.size(); i++) {
        m_waters[i]->doSomething();
    }
    
    for (int i = 0; i < m_sonars.size(); i++) {
        if (!m_sonars[i]->isAlive()) {
            delete m_sonars[i];
            m_sonars.erase(m_sonars.begin() + i);
        }
    }
    
    
//    for (vector<Water*>::iterator it = m_waters.begin(); it != m_waters.end(); it++) {
//        if (!(*it)->isAlive()) {
//            delete (*it);
//            m_waters.erase(it);
//        }
//    }
    for (int i = 0; i < m_waters.size(); i++) {
        if (!m_waters[i]->isAlive()) {
            delete m_waters[i];
            m_waters.erase(m_waters.begin() + i);
        }
    }
    
    // Protester
    int temp1 = 25, temp2 = 200 - getLevel();
    int T = temp1 > temp2 ? temp1 : temp2;
    if (m_ticks > T) {
        m_ticks = 0;
        int temp3 = 15, temp4 = 2 + getLevel() * 1.5;
        int P = temp3 < temp4 ? temp3 : temp4;
        if (m_protesters.size() < P) { // add a Protester
            addAProtester();
        }
    }
    
    for (int i = 0; i < m_protesters.size(); i++) {
        m_protesters[i]->doSomething();
    }
    
    for (int i = 0; i < m_protesters.size(); i++) {
        if (!m_protesters[i]->isAlive()) {
            delete m_protesters[i];
            m_protesters.erase(m_protesters.begin() + i);
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
    
    // Gold Nugget
    for (int i = 0; i < m_golds.size(); i++) {
        delete m_golds[i];
        m_golds.erase(m_golds.begin()+i);
    }
    
    // Sonar Kit
    cerr << "Before cleanUp, m_sonars.size() = " << m_sonars.size() << endl;
    for (int i = 0; i < m_sonars.size(); i++) {
        delete m_sonars[i];
        cerr << "Now the ith object is deleted; i = " << i << endl;
        m_sonars.erase(m_sonars.begin() + i);
    }
    cerr << "After cleanUp, m_sonars.size() = " << m_sonars.size() << endl;
    
    // Water Pool
//    for (vector<Water*>::iterator it = m_waters.begin(); it != m_waters.end(); it++) {
//        delete *it;
//        m_waters.erase(it);
//    }
    for (int i = 0; i < m_waters.size(); i++) {
        delete m_waters[i];
        m_waters.erase(m_waters.begin() + i);
    }
    cerr << "After cleanUp, m_waters.size() = " << m_waters.size() << endl;
    
    // Protester
//    for (int i = 0; i < m_protesters.size(); i++) {
//        delete m_protesters[i];
//        m_protesters.erase(m_protesters.begin() + i);
//    }
    for (vector<Protester*>::iterator it = m_protesters.begin(); it != m_protesters.end(); it++)
        delete (*it);
    m_protesters.clear();
    cerr << "After cleanUp, m_protesters.size() = " << m_protesters.size() << endl;
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
