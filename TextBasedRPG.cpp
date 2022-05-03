#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <sstream>

using namespace std;

class Character;
void botDamage(Character *, Character *, vector<string> &);
void setConsoleColor(int x)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, x);
}

class IOInterface
{

public:
    virtual istream &input(istream &in) = 0;
    virtual istream &userInput(istream &in) = 0;
    virtual ostream &print(ostream &out) const = 0;
    virtual ostream &printStats(ostream &out) const = 0;
};

class Character : public IOInterface
{
protected:
    // ability struct
    struct Ability
    {
    public:
        string name, description;
        // int cooldown;
    };

    // user defined properties
    string name, kingdom, personality, bodyType;

    // base stats for each class
    const unsigned int baseHealthPoints, baseDamage, baseStrength, baseIntelligence, baseDexterity;
    const float criticalStrikeDamage;

    // modifiable stats in-game
    float healthPoints, damage;
    unsigned int lvl, exp, expNeededForLvl, strength, intelligence, dexterity;
    float criticalChance;

    Ability sleep, passive, damageAbility1, damageAbility2;

public:
    Character(string name, string kingdom, string personality, string bodytype, const unsigned int hp, const unsigned int baseDMG,
              const float critDMG, float criticalChance, unsigned int lvl, unsigned int exp, unsigned int expNextLvl, const unsigned int strength, const unsigned int intelligence, const unsigned int dexterity);

    Character(const unsigned int baseHP, const unsigned int baseDMG, const float critDMG, const unsigned int baseStrength, const unsigned int baseDexterity, const unsigned int baseIntelligence, float critChance);
    Character(const Character &);
    Character &operator=(const Character &);
    friend istream &operator>>(istream &is, Character &);
    friend ostream &operator<<(ostream &os, const Character &);
    void setSleep();
    void checkXP();
    void takeaNap(int);
    int getHP();
    int getXP();
    void setXP(int);

    string getName();
    string getSleepName();
    string getPassiveName();
    string getAbility1Name();
    string getAbility2Name();
    int getDamage();
    float getCriticalStrikeChance();

    virtual void takeDMG(int, bool);
    virtual istream &input(istream &);
    virtual istream &userInput(istream &in);
    virtual ostream &print(ostream &) const;
    virtual ostream &printStats(ostream &out) const;
    virtual void printInGameStats() const;
    virtual void playerScreen(vector<string> &, Character *);
    virtual void levelUpScreen();
    virtual void levelUP() = 0;
    virtual void bonusStatsCalculator(unsigned int, unsigned int, unsigned int) = 0;
    virtual void setPassive() = 0;
    virtual void setDamageAbility1() = 0;
    virtual void setDamageAbility2() = 0;
    virtual void Passive() = 0;
    virtual int ability1() = 0;
    virtual int ability2(bool) = 0;
};

Character ::Character(string name, string kingdom, string personality, string bodytype, const unsigned int baseHP, const unsigned int baseDMG, const float critDMG, float criticalChance, const unsigned int baseStrength, const unsigned int baseIntelligence, const unsigned int baseDexterity,
                      unsigned int lvl, unsigned int exp, unsigned int expNextLvl) : baseHealthPoints(baseHP), criticalStrikeDamage(critDMG), baseDamage(baseDMG), baseStrength(baseStrength), baseDexterity(baseDexterity), baseIntelligence(baseIntelligence)
{
    this->name = name;
    this->kingdom = kingdom;
    this->personality = personality;
    this->bodyType = bodyType;
    this->lvl = lvl;
    this->exp = exp;
    this->strength = strength;
    this->intelligence = intelligence;
    this->dexterity = dexterity;
    this->criticalChance = criticalChance;
    this->healthPoints = baseHP;
    this->damage = baseDMG;
    this->expNeededForLvl = expNextLvl;
    setSleep();
}

Character ::Character(const unsigned int baseHP = 1000, const unsigned int baseDMG = 100, const float critDMG = 2, const unsigned int baseStrength = 10, const unsigned int baseDexterity = 10, const unsigned int baseIntelligence = 10, float critChance = 0.2)
    : name("CharacterX"), kingdom("KingdomX"), personality("charismatic"), bodyType("Muscular"), baseHealthPoints(baseHP), baseDamage(baseDMG), criticalStrikeDamage(critDMG),
      criticalChance(critChance), lvl(1), exp(0), expNeededForLvl(1000), baseStrength(baseStrength), baseDexterity(baseDexterity), baseIntelligence(baseIntelligence), strength(baseStrength),
      intelligence(baseIntelligence), dexterity(baseDexterity), healthPoints(baseHP), damage(baseDMG)
{
    setSleep();
};

Character ::Character(const Character &character) : baseHealthPoints(character.baseHealthPoints), baseDamage(character.baseDamage), criticalStrikeDamage(character.criticalStrikeDamage),
                                                    baseStrength(character.baseStrength), baseDexterity(character.baseDexterity), baseIntelligence(character.baseIntelligence)
{
    this->name = character.name;
    this->kingdom = character.kingdom;
    this->personality = character.personality;
    this->bodyType = character.bodyType;
    this->lvl = character.lvl;
    this->exp = character.exp;
    this->strength = character.strength;
    this->intelligence = character.intelligence;
    this->dexterity = character.dexterity;
    this->criticalChance = character.criticalChance;
    this->healthPoints = character.healthPoints;
    this->damage = character.damage;
    this->expNeededForLvl = character.expNeededForLvl;
}

Character &Character ::operator=(const Character &character)
{
    if (this != &character)
    {
        this->name = character.name;
        this->kingdom = character.kingdom;
        this->personality = character.personality;
        this->bodyType = character.bodyType;
        this->lvl = character.lvl;
        this->exp = character.exp;
        this->strength = character.strength;
        this->intelligence = character.intelligence;
        this->dexterity = character.dexterity;
        this->criticalChance = character.criticalChance;
        this->healthPoints = character.healthPoints;
        this->damage = character.damage;
        this->expNeededForLvl = character.expNeededForLvl;
    }
    return *this;
}

ostream &Character ::print(ostream &print = cout) const
{
    setConsoleColor(15);
    print << "----------------------"
          << this->name
          << "----------------------"
          << "\nKingdom : " << this->kingdom
          << "\nPersonality : " << this->personality
          << "\nBody type : " << this->bodyType;
    setConsoleColor(6);
    print << "\n----------------------ABILITIES----------------------"
          << "\n1." << this->sleep.name << "\n  " << this->sleep.description
          << "\n2." << this->passive.name << "\n  " << this->passive.description
          << "\n3." << this->damageAbility1.name << "\n  " << this->damageAbility1.description
          << "\n4." << this->damageAbility2.name << "\n  " << this->damageAbility2.description;
    setConsoleColor(9);
    print << "\n----------------------"
          << "STATS"
             "------------------------"
          << "\nHealth : " << this->healthPoints
          << "\nDamage : " << this->damage
          << "\nLevel : " << this->lvl
          << "\nExperience : " << this->exp
          << "\nExperience needed to get to the next level : " << this->expNeededForLvl
          << "\nStrength : " << this->strength
          << "\nIntelligence : " << this->intelligence
          << "\nDexterity : " << this->dexterity
          << "\nCritical strike chance : " << this->criticalChance * 100 << "%"
          << "\nCritical strike damage : " << this->damage * this->criticalStrikeDamage;

    setConsoleColor(15);
    return print;
}

istream &Character ::input(istream &in)
{

    cout << "Name : ";
    in >> this->name;
    cout << "Kingdom : ";
    in >> this->kingdom;
    cout << "Personality : ";
    in >> this->personality;
    cout << "Body type : ";
    in >> this->bodyType;
    cout << "Health points : ";
    in >> this->healthPoints;
    cout << "Damage : ";
    in >> this->damage;
    cout << "Level : ";
    in >> this->lvl;
    cout << "Experience : ";
    in >> this->exp;
    cout << "Experience needed to get to the next level : ";
    in >> this->expNeededForLvl;
    cout << "Strength : ";
    in >> this->strength;
    cout << "Intelligence : ";
    in >> this->intelligence;
    cout << "Dexterity : ";
    in >> this->dexterity;
    cout << "Critical chance : ";
    in >> this->criticalChance;

    return in;
}

istream &Character ::userInput(istream &in = cin)
{

    cout << "Name : ";
    in >> this->name;
    cout << "Kingdom : ";
    in >> this->kingdom;
    cout << "Personality : ";
    in >> this->personality;
    cout << "Body type : ";
    in >> this->bodyType;

    return in;
}

ostream &Character ::printStats(ostream &print = cout) const
{

    setConsoleColor(14);

    print << "----------------------"
          << name
          << "----------------------"
          << "\nHealth : " << this->healthPoints
          << "\nDamage : " << this->damage
          << "\nLevel : " << this->lvl
          << "\nExperience : " << this->exp
          << "\nExperience needed to get to the next level : " << this->expNeededForLvl
          << "\nStrength : " << this->strength
          << "\nIntelligence : " << this->intelligence
          << "\nDexterity : " << this->dexterity
          << "\nCritical strike chance : " << this->criticalChance * 100 << "%"
          << "\nCritical strike damage : " << this->damage * this->criticalStrikeDamage;
    setConsoleColor(15);

    return print;
}

istream &operator>>(istream &in, Character &character)
{
    return character.input(in);
}

ostream &operator<<(ostream &print, const Character &character)
{
    return character.print(print);
}

void Character ::levelUpScreen()
{
    // user has the option to upgrade one of the 3 skills with skill points available after each level
    char key;
    int skillPoints = 2;
    ++this->lvl;
    while (skillPoints > -1)
    {
        setConsoleColor(13);
        cout << "-----------------CONGRATULATIONS, YOU'VE REACHED LEVEL " << this->lvl << "!-----------------";
        setConsoleColor(15);
        cout << "\nSkill points available : " << skillPoints;
        cout << "\nUpgrade one of the following skills :"
             << "\n1.Strength(" << this->strength << ")"
             << "\n2.Intelligence(" << this->intelligence << ")"
             << "\n3.Dexterity(" << this->dexterity << ")";

        // key and enter afterwards to select the option
        if (skillPoints)
        {
            key = _getch();
            switch (key)
            {
            case 49:
                this->strength++;
                break;
            case 50:
                this->intelligence++;
                break;
            case 51:
                this->dexterity++;
                break;
            case 27:
                exit(1);
            default:
                system("cls");
                continue;
            }
            skillPoints--;
            cin.get();
            system("cls");
        }
        else
        {
            cout << "\nPress anything to continue ... ";
            cin.get();
            system("cls");
            break;
        }
    }
}

void Character ::setSleep()
{
    this->sleep.name = "Sleep";
    this->sleep.description = "You take a rest for the current round and regenerate a bit of your hp(and mana if you're a wizard Harry)";
}

void Character ::takeaNap(int initialHP)
{
    this->healthPoints += 0.08 * this->healthPoints;
    if (this->healthPoints > initialHP)
        this->healthPoints = initialHP;
}

void Character ::checkXP()
{
    if (this->exp >= this->expNeededForLvl)
    {
        int extraXP = this->exp - this->expNeededForLvl;
        this->exp = extraXP;
        levelUP();
    }
}

void Character ::takeDMG(int damage, bool x = false)
{
    this->healthPoints -= damage;
}

void Character ::playerScreen(vector<string> &battleLog, Character *enemy)
{
    string message;

    int expArr[] = {500, 1000, 1500};
    int expVoter = rand() % 3;
    char playerInput;
    int initialPlayerHP = this->healthPoints;
    int initialBotHP = enemy->healthPoints;
    bool battleOutcome = true;
    Character *playerCPY = this;
    while (this->healthPoints > 0 && enemy->getHP() > 0)
    {
        bool badInput = true;
        while (badInput)
        {
            setConsoleColor(4);
            cout << this->name;
            printInGameStats();
            setConsoleColor(15);
            cout << "\n\nYour turn to strike!\n";
            setConsoleColor(9);
            cout << "\n1." << this->sleep.name
                 << "\n2." << this->passive.name
                 << "\n3." << this->damageAbility1.name
                 << "\n4." << this->damageAbility2.name;
            playerInput = _getch();

            if (playerInput == 49)
            {
                takeaNap(initialPlayerHP);
                ostringstream buffer;
                buffer << this->name << " uses " << this->sleep.name << " to recover his strength";
                message = buffer.str();
                battleLog.push_back(message);
                badInput = false;
            }
            else if (playerInput == 50)
            {
                this->Passive();
                ostringstream buffer;
                buffer << this->name << " uses " << this->passive.name << " and his stats go wild";
                message = buffer.str();
                battleLog.push_back(message);
                badInput = false;
            }
            else if (playerInput == 51)
            {
                ostringstream buffer;
                int damageDealt = this->ability1();
                int x = 100 - this->criticalChance;
                int y = rand() % 101;
                if (y > x)
                {
                    damageDealt += this->damage * this->criticalStrikeDamage;
                }
                enemy->takeDMG(damageDealt);
                buffer << this->name << " uses " << this->damageAbility1.name << " and deals " << damageDealt << " damage";
                message = buffer.str();
                battleLog.push_back(message);
                badInput = false;
            }
            else if (playerInput == 52)
            {
                ostringstream buffer;
                int damageDealt = this->ability2(false);
                int x = 100 - this->criticalChance;
                int y = rand() % 101;
                if (y > x)
                {
                    damageDealt += damage * this->criticalStrikeDamage;
                }
                enemy->takeDMG(damageDealt);
                buffer << this->name << " uses " << this->damageAbility2.name << " and deals " << damageDealt << " damage";
                message = buffer.str();
                battleLog.push_back(message);
                badInput = false;
            }
            else if (playerInput == 27)
                exit(1);
            else
            {
                system("cls");
                continue;
            }
            setConsoleColor(6);
            cout << "\n\nBattle log :\n";
            for (auto x : battleLog)
                cout << '\n'
                     << x;
            cin.get();
            system("cls");
        }
        botDamage(this, enemy, battleLog);
    }
    if (this->healthPoints < 0)
    {
        cout << "---------------------------------DEFEAT!---------------------------------";
        battleOutcome = false;
        setConsoleColor(15);
        cout << "\n\nPress anything to continue...";
        cin.get();
    }
    if (battleOutcome)
    {
        cout << "---------------------------------YOU WIN!---------------------------------\n";
        this->exp += expArr[expVoter];
        setConsoleColor(15);
        cout << "\nPress anything to continue...";
        cin.get();
    }
}

void Character ::printInGameStats() const
{
    cout << "\nHP : " << this->healthPoints
         << "\nDamage : " << this->damage
         << "\nCritical strike chance : " << this->criticalChance * 100 << '%';
}

int Character ::getHP()
{
    return this->healthPoints;
}

string Character ::getName()
{
    return this->name;
}
string Character ::getSleepName()
{
    return this->sleep.name;
}

string Character ::getPassiveName()
{
    return this->passive.name;
}
string Character ::getAbility1Name()
{
    return this->damageAbility1.name;
}
string Character ::getAbility2Name()
{
    return this->damageAbility2.name;
}
int Character ::getDamage()
{
    return this->damage;
}
float Character ::getCriticalStrikeChance()
{
    return this->criticalChance;
}

void Character ::setXP(int xp)
{
    this->exp = xp;
}

int Character ::getXP()
{
    return this->exp;
}

class Warrior : virtual public Character
{

protected:
    // base stats
    const unsigned int baseArmor, baseMR;
    // warrior stats, increasing by leveling up
    unsigned int armor, magicResistance;

public:
    Warrior(string name, string kingdom, string personality, string bodytype, float criticalChance,
            unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity, unsigned int armor, unsigned int MR);
    Warrior();
    Warrior(const Warrior &warrior);
    Warrior &operator=(const Warrior &warrior);
    istream &input(istream &in);
    ostream &print(ostream &out) const;
    ostream &printStats(ostream &print) const;
    void printInGameStats() const;
    void levelUP();
    void bonusStatsCalculator(unsigned int, unsigned int, unsigned int);
    void setPassive();
    void setDamageAbility1();
    void setDamageAbility2();
    void takeDMG(int, bool);
    void Passive();
    int ability1();
    int ability2(bool);
};

Warrior ::Warrior(string name, string kingdom, string personality, string bodytype, float criticalChance,
                  unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity, unsigned int armor, unsigned int MR)
    : Character(name, kingdom, personality, bodytype, 1500, 120, 1.5, criticalChance, lvl, exp, expNextLvl, strength, intelligence, dexterity), baseArmor(10), baseMR(8)
{
    this->armor = armor;
    this->magicResistance = MR;
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
}
Warrior ::Warrior() : Character(1200, 1000, 2, 15, 7, 3, 0.2), baseArmor(10), baseMR(8), armor(baseArmor), magicResistance(baseMR)
{
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
};

Warrior ::Warrior(const Warrior &warrior) : Character(warrior), baseArmor(10), baseMR(8)
{
    this->armor = warrior.armor;
    this->magicResistance = warrior.magicResistance;
}
Warrior &Warrior ::operator=(const Warrior &warrior)
{
    if (this != &warrior)
    {
        // explicit call for = defined for Warrior
        Character ::operator=(warrior);
        this->armor = warrior.armor;
        this->magicResistance = warrior.magicResistance;
    }
    return *this;
}

istream &Warrior ::input(istream &in)
{
    Character ::input(in);
    cout << "Armor : ";
    in >> this->armor;
    cout << "Magic Resistance : ";
    in >> this->magicResistance;
    return in;
}

ostream &Warrior ::print(ostream &print = cout) const
{
    Character ::print(print);
    setConsoleColor(9);
    print << "\nArmor : " << this->armor
          << "\nMagic resitance : " << this->magicResistance;
    setConsoleColor(15);

    return print;
}

ostream &Warrior ::printStats(ostream &print = cout) const
{
    Character ::printStats(print);
    setConsoleColor(14);
    print << "\nArmor : " << this->armor
          << "\nMagic resitance : " << this->magicResistance;
    setConsoleColor(15);
}

void Warrior ::bonusStatsCalculator(unsigned int str, unsigned int dex, unsigned int intelligence)
{
    this->damage += this->strength - str;
    this->healthPoints += (this->strength - str) * 20;
    this->armor += (this->dexterity - dex) * 5;
    this->magicResistance += (this->dexterity - dex) * 5;
    this->magicResistance += (this->intelligence - intelligence) * 7;
}

void Warrior ::levelUP()
{
    int cpyStr = this->strength;
    int cpyDex = this->dexterity;
    int cpyInt = this->intelligence;
    int cpyHP = this->healthPoints;
    int cpyDMG = this->damage;
    int cpyArm = this->armor;
    int cpyMR = this->magicResistance;

    levelUpScreen();
    bonusStatsCalculator(cpyStr, cpyDex, cpyInt);

    cout << "Some Stats have changed :)";
    setConsoleColor(9);
    cout << "\nHealth : " << cpyHP << " -> " << (int)(this->healthPoints += 0.1 * this->healthPoints);
    cout << "\nDamage : " << cpyDMG << " -> " << (int)(this->damage += 0.05 * this->damage);
    cout << "\nArmor : " << cpyArm << " -> " << ++this->armor;
    cout << "\nMagic Resist : " << cpyMR << " -> " << ++this->magicResistance;
    cout << "\nExperience needed to get to the next level : " << (this->expNeededForLvl += 1000);
    setConsoleColor(14);
    cout << "\nPress anything to continue...";
    cin.get();
    system("cls");
}

void Warrior ::setPassive()
{
    this->passive.name = "Defensive Stance";
    this->passive.description = "For the remainder of the fight your armor and magic resistance get a boost";
    // this -> passive.cooldown = 5;
}

void Warrior ::setDamageAbility1()
{
    this->damageAbility1.name = "Berserk";
    this->damageAbility1.description = "You give a vicious blow to your opponent. You have a slight chance that your hit will deal double damage.";
    // this->damageAbility1.cooldown = 0;
}

void Warrior ::setDamageAbility2()
{
    this->damageAbility2.name = "Havoc";
    this->damageAbility2.description = "You spin with your axes and slash everything around you. Bonus damage vs mages";
    // this->damageAbility2.cooldown = 0;
}

void Warrior ::Passive()
{
    this->armor += this->armor * 0.25;
    this->magicResistance += this->magicResistance * 0.2;
}

int Warrior ::ability1()
{
    int number;
    // srand(time(0));
    number = rand() % 101;
    if (number > 80)
        return this->damage * 2;
    else
        return this->damage;
}

int Warrior ::ability2(bool mageOpponent)
{
    if (mageOpponent)
        return this->damage * 1.5;
    else
        return this->damage * 1.2;
}

void Warrior ::takeDMG(int damage, bool wizard)
{
    if (wizard)
        damage -= 1.3 * this->magicResistance;
    else
        damage -= 1.5 * this->armor;
    this->healthPoints -= damage;
}

void Warrior ::printInGameStats() const
{
    Character ::printInGameStats();
    cout << "\nArmor : " << this->armor;
    cout << "\nMagic resistance : " << this->magicResistance;
}

class Mage : virtual public Character
{

protected:
    // base stats
    const unsigned int baseMana, baseMagicDamage;
    const float baseManaRegen;
    // mage stats, increasing by leveling up
    unsigned int mana, magicDamage;
    float manaRegen;

public:
    Mage(string name, string kingdom, string personality, string bodytype, float criticalChance,
         unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity, unsigned int mana, float manaRegen, unsigned int magicDamage);
    Mage();
    Mage(const Mage &mage);
    Mage &operator=(const Mage &mage);
    ostream &print(ostream &out) const;
    istream &input(istream &in);
    ostream &printStats(ostream &out) const;
    void printInGameStats() const;
    void takeaNap(int, int);
    void levelUP();
    void bonusStatsCalculator(unsigned int, unsigned int, unsigned int);
    void setPassive();
    void setDamageAbility1();
    void setDamageAbility2();
    void Passive();
    int ability1();
    int ability2(bool);
};

Mage::Mage(string name, string kingdom, string personality, string bodytype, float criticalChance,
           unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity, unsigned int mana, float manaRegen, unsigned int magicDamage)
    : Character(name, kingdom, personality, bodytype, 1200, 0, 0, 0, lvl, exp, expNextLvl, strength, intelligence, dexterity), baseMana(100), baseMagicDamage(50), baseManaRegen(0.2)
{
    this->mana = mana;
    this->manaRegen = manaRegen;
    this->magicDamage = magicDamage;
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
}
Mage::Mage() : Character(600, 0, 0, 3, 7, 15, 0), baseMana(100), baseMagicDamage(50), baseManaRegen(0.2), mana(baseMana), magicDamage(baseMagicDamage), manaRegen(baseManaRegen)
{
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
};

Mage::Mage(const Mage &mage) : Character(mage), baseMana(100), baseMagicDamage(50), baseManaRegen(0.2)
{
    this->mana = mage.mana;
    this->manaRegen = mage.manaRegen;
    this->magicDamage = mage.magicDamage;
}
Mage &Mage ::operator=(const Mage &mage)
{
    if (this != &mage)
    {
        // explicit call for = defined for Mage
        Character ::operator=(mage);
        this->mana = mage.mana;
        this->manaRegen = mage.manaRegen;
        this->magicDamage = mage.magicDamage;
    }
    return *this;
}

istream &Mage ::input(istream &in)
{
    cout << "Name : ";
    in >> this->name;
    cout << "Kingdom : ";
    in >> this->kingdom;
    cout << "Personality : ";
    in >> this->personality;
    cout << "Body type : ";
    in >> this->bodyType;
    cout << "Health points : ";
    in >> this->healthPoints;
    cout << "Level : ";
    in >> this->lvl;
    cout << "Experience : ";
    in >> this->exp;
    cout << "Experience needed to get to the next level : ";
    in >> this->expNeededForLvl;
    cout << "Strength : ";
    in >> this->strength;
    cout << "Intelligence : ";
    in >> this->intelligence;
    cout << "Dexterity : ";
    in >> this->dexterity;
    cout << "Mana : ";
    in >> this->mana;
    cout << "Mana regen : ";
    in >> this->manaRegen;
    cout << "Magic damage : ";
    in >> this->magicDamage;
    return in;
}

ostream &Mage ::print(ostream &print = cout) const
{
    setConsoleColor(15);
    print << "----------------------" << name << "----------------------"
          << "\nKingdom : " << this->kingdom
          << "\nPersonality : " << this->personality
          << "\nBody type : " << this->bodyType;
    setConsoleColor(6);
    print << "\n----------------------ABILITIES----------------------"
          << "\n1." << this->sleep.name << "\n  " << this->sleep.description
          << "\n2." << this->passive.name << "\n  " << this->passive.description
          << "\n3." << this->damageAbility1.name << "\n  " << this->damageAbility1.description
          << "\n4." << this->damageAbility2.name << "\n  " << this->damageAbility2.description;

    setConsoleColor(9);

    print << "\n----------------------"
          << "STATS"
             "------------------------"
          << "\nHealth : " << this->healthPoints
          << "\nLevel : " << this->lvl
          << "\nExperience : " << this->exp
          << "\nExperience needed to get to the next level : " << this->expNeededForLvl
          << "\nStrength : " << this->strength
          << "\nIntelligence : " << this->intelligence
          << "\nDexterity : " << this->dexterity
          << "\nMana : " << this->mana
          << "\nMana regen : " << this->manaRegen
          << "\nMagic damage : " << this->magicDamage;

    setConsoleColor(15);

    return print;
}

ostream &Mage ::printStats(ostream &print = cout) const
{
    setConsoleColor(14);
    print << "----------------------" << name << "----------------------"
          << "\nHealth : " << this->healthPoints
          << "\nLevel : " << this->lvl
          << "\nExperience : " << this->exp
          << "\nExperience needed to get to the next level : " << this->expNeededForLvl
          << "\nStrength : " << this->strength
          << "\nIntelligence : " << this->intelligence
          << "\nDexterity : " << this->dexterity
          << "\nMana : " << this->mana
          << "\nMana regen : " << this->manaRegen
          << "\nMagic damage : " << this->magicDamage;
    setConsoleColor(15);

    return print;
}
void Mage ::bonusStatsCalculator(unsigned int str, unsigned int dex, unsigned int intelligence)
{
    this->healthPoints += (this->strength - str) * 10;
    this->magicDamage += (this->intelligence - intelligence) * 10;
    this->mana += (this->intelligence - intelligence) * 40;
    this->manaRegen += (this->intelligence - intelligence) * 0.01;
}

void Mage ::levelUP()
{

    int cpyStr = this->strength;
    int cpyDex = this->dexterity;
    int cpyInt = this->intelligence;
    int cpyHP = this->healthPoints;
    int cpyDMG = this->damage;
    int cpyMD = this->magicDamage;
    int cpyMana = this->mana;
    float cpyManaRegen = this->manaRegen;

    levelUpScreen();
    bonusStatsCalculator(cpyStr, cpyDex, cpyInt);

    cout << "Some Stats have changed :)";
    setConsoleColor(9);
    cout << "\nHealth : " << cpyHP << " -> " << (int)(this->healthPoints += 0.1 * this->healthPoints);
    cout << "\nMana : " << cpyMana << " -> " << (int)(this->mana += 0.2 * this->mana);
    cout << "\nMana regen : " << cpyManaRegen * this->mana << " -> " << ((this->manaRegen += 0.01 * this->manaRegen) * this->mana);
    cout << "\nMagic damage : " << cpyMD << " -> " << (int)(this->magicDamage += 0.15 * this->magicDamage);
    cout << "\nExperience needed to get to the next level : " << (this->expNeededForLvl += 1000);

    setConsoleColor(14);
    cout << "\nPress anything to continue...";
    cin.get();
    system("cls");
}

void Mage ::setPassive()
{
    this->passive.name = "Ancient Aura";
    this->passive.description = "Increases your magic damage for the remainder of the combat";
}
void Mage ::setDamageAbility1()
{
    this->damageAbility1.name = "Magic Overload";
    this->damageAbility1.description = "Using a mysterious force you annihilate everything around you. Bonus damage chance";
}

void Mage ::setDamageAbility2()
{
    this->damageAbility2.name = "Mythical charge";
    this->damageAbility2.description = "Unleash hell among enemies. Bonus damage to marksmen";
}

void Mage ::Passive()
{
    this->magicDamage += 10;
}

int Mage ::ability1()
{
    // the more mana the mages have the more dmg they deal
    int number;
    int manaCPY = this->mana;
    // mana cost
    this->mana -= 0.4 * this->mana;
    // srand(time(0));
    number = rand() % 101;
    if (number > 70)
        return this->magicDamage * 1.5 + manaCPY * 0.2;
    else
        return this->magicDamage * 1.2 + this->mana * 0.1;
}

int Mage ::ability2(bool marksmanOpponent)
{
    int manaCPY = this->mana;
    // mana cost
    this->mana -= 0.2 * this->mana;
    if (marksmanOpponent)
        return manaCPY * 0.7;
    else
        return manaCPY * 1.5;
}

void Mage ::takeaNap(int initialHP, int initialMana)
{

    this->healthPoints += 0.08 * this->healthPoints;
    if (this->healthPoints > initialHP)
        this->healthPoints = initialHP;

    this->mana += 50;
    if (this->mana > initialMana)
        this->mana = initialMana;
}

void Mage ::printInGameStats() const
{
    cout << "\n\nHP : " << this->healthPoints
         << "\nMagic Damage : " << this->magicDamage
         << "\nMana : " << this->mana
         << "\nMana regen : " << this->manaRegen * this->mana;
}
class Marksman : virtual public Character
{

protected:
    // base stats
    const float basePoisonDMG, baseDoubleStrikeChance;
    // marksman stats, increasing by leveling up
    float poisonDMG, doubleStrikeChance;

public:
    Marksman(string name, string kingdom, string personality, string bodytype, float criticalChance, float poisonDMG, float doubleStrikeChance, unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity);
    Marksman();
    Marksman(const Marksman &marksman);
    Marksman &operator=(const Marksman &marksman);
    istream &input(istream &in);
    ostream &print(ostream &out) const;
    ostream &printStats(ostream &print) const;
    void levelUP();
    void printInGameStats() const;
    void bonusStatsCalculator(unsigned int, unsigned int, unsigned int);
    void setPassive();
    void setDamageAbility1();
    void setDamageAbility2();
    void Passive();
    int ability1();
    int ability2(bool);
    void getPassive();
};

Marksman::Marksman(string name, string kingdom, string personality, string bodytype, float criticalChance, float poisonDMG, float doubleStrikeChance, unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity)
    : Character(name, kingdom, personality, bodytype, 900, 70, 1.5, 0.5, lvl, exp, expNextLvl, strength, intelligence, dexterity), basePoisonDMG(0.05), baseDoubleStrikeChance(0.3), poisonDMG(basePoisonDMG), doubleStrikeChance(baseDoubleStrikeChance)
{
    this->poisonDMG = poisonDMG;
    this->doubleStrikeChance = doubleStrikeChance;
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
}
Marksman::Marksman() : Character(800, 50, 1.5, 8, 12, 5, 0.35), basePoisonDMG(0.05), baseDoubleStrikeChance(0.3), poisonDMG(basePoisonDMG), doubleStrikeChance(baseDoubleStrikeChance)
{
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
};

Marksman ::Marksman(const Marksman &marksman) : Character(marksman), basePoisonDMG(0.05), baseDoubleStrikeChance(0.3)
{
    this->poisonDMG = marksman.poisonDMG;
    this->doubleStrikeChance = marksman.doubleStrikeChance;
}

Marksman &Marksman::operator=(const Marksman &marksman)
{
    if (this != &marksman)
    {
        // explicit call for = defined for Marksman
        Character ::operator=(marksman);
        this->poisonDMG = marksman.poisonDMG;
        this->doubleStrikeChance = marksman.doubleStrikeChance;
    }
    return *this;
}

istream &Marksman ::input(istream &in)
{
    Character ::input(in);
    cout << "Poison dmg : ";
    in >> this->poisonDMG;
    cout << "Double strike chance : ";
    in >> this->doubleStrikeChance;
    return in;
}

ostream &Marksman ::print(ostream &print = cout) const
{
    Character ::print(print);
    setConsoleColor(9);
    print << "\nPoison damage : " << (int)(this->poisonDMG * this->damage)
          << "\nDouble strike chance  : " << this->doubleStrikeChance * 100 << "%";
    setConsoleColor(15);

    return print;
}

ostream &Marksman ::printStats(ostream &print = cout) const
{
    Character ::printStats(print);
    setConsoleColor(14);
    print << "\nPoison damage : " << (int)this->poisonDMG * this->damage
          << "\nDouble strike chance : " << this->doubleStrikeChance * 100 << "%";
    setConsoleColor(15);
}
void Marksman ::bonusStatsCalculator(unsigned int str, unsigned int dex, unsigned int intelligence)
{
    this->damage += this->strength - str;
    this->healthPoints += (this->strength - str) * 20;
    this->criticalChance += (this->dexterity - dex) * 0.015;
    this->doubleStrikeChance += (this->intelligence - intelligence) * 0.015;
    this->poisonDMG += (this->dexterity - dex) * 0.005;
}

void Marksman ::levelUP()
{
    int cpyStr = this->strength;
    int cpyDex = this->dexterity;
    int cpyInt = this->intelligence;
    int cpyHP = this->healthPoints;
    int cpyDMG = this->damage;
    float cpyDoubleStrikeChance = this->baseDoubleStrikeChance;
    float cpyPoisonDMG = this->poisonDMG;
    float cpyCriticalChance = this->criticalChance;
    levelUpScreen();
    bonusStatsCalculator(cpyStr, cpyDex, cpyInt);

    cout << "Some Stats have changed :)";
    setConsoleColor(9);
    cout << "\nHealth : " << cpyHP << " -> " << (int)(this->healthPoints += 0.09 * this->healthPoints);
    cout << "\nDamage : " << cpyDMG << " -> " << (int)(this->damage += 0.025 * this->damage);
    cout << "\nPoison damage: " << cpyPoisonDMG * this->damage << " -> " << (this->poisonDMG += 0.5 * this->poisonDMG) * this->damage;
    cout << "\nCritical strike chance: " << cpyCriticalChance * 100 << "%"
         << " -> " << (this->criticalChance += 0.1 * this->criticalChance) * 100 << "%";
    cout << "\nDouble strike chance : " << cpyDoubleStrikeChance * 100 << "%"
         << " -> " << (this->doubleStrikeChance += 0.05 * this->doubleStrikeChance) * 100 << "%";
    cout << "\nExperience needed to get to the next level : " << (this->expNeededForLvl += 1000);
    setConsoleColor(14);
    cout << "\nPress anything to continue...";
    cin.get();
    system("cls");
}

void Marksman ::setPassive()
{
    this->passive.name = "Forbidden Tacitcs";
    this->passive.description = "Increase your critical strike chance by a bit for the rest of the combat";
}
void Marksman ::setDamageAbility1()
{
    this->damageAbility1.name = "Sharp Arrows";
    this->damageAbility1.description = "Your arrows pierce through your opponents flesh and bones. Bonus damage to warriors";
}
void Marksman ::setDamageAbility2()
{
    this->damageAbility2.name = "Death Shower";
    this->damageAbility2.description = "You shoot a bunch of arrows into the sky. With a little bit of chance you can surprise your enemy. Bonus damage chance";
}

void Marksman ::Passive()
{
    this->criticalChance += 0.02;
}
int Marksman ::ability1()
{
    int number;
    // srand(time(0));
    number = rand() % 101;
    if (number > 70)
        return this->damage * 2.2;
    else
        return this->damage * 1.2;
}

int Marksman ::ability2(bool warriorOpponent)
{
    if (warriorOpponent)
        return this->damage * 2;
    else
        return this->damage * 1.5;
}

void Marksman ::getPassive()
{
    cout << this->passive.name << '\n'
         << this->passive.description << '\n';
}

void Marksman ::printInGameStats() const
{
    Character ::printInGameStats();
    cout << "\nPoison damage : " << this->poisonDMG;
    cout << "\nDouble strike chane : " << this->doubleStrikeChance * 100 << '%';
}

class WarGeneral : public Warrior, public Marksman
{

public:
    WarGeneral(string name, string kingdom, string personality, string bodytype, float criticalChance,
               unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity, unsigned int armor, unsigned int MR, float doubleStrikeChance, float poisonDMG);
    WarGeneral();
    WarGeneral(const WarGeneral &warGeneral);
    WarGeneral &operator=(const WarGeneral &warGeneral);
    istream &input(istream &in);
    ostream &print(ostream &out) const;
    ostream &printStats(ostream &print) const;
    void printInGameStats() const;
    void levelUP();
    void bonusStatsCalculator(unsigned int, unsigned int, unsigned int);
    void setPassive();
    void setDamageAbility1();
    void setDamageAbility2();
    void takeDMG(int, bool);
    void Passive();
    int ability1();
    int ability2(bool);
};

WarGeneral ::WarGeneral(string name, string kingdom, string personality, string bodytype, float criticalChance, unsigned int lvl, unsigned int exp, unsigned int expNextLvl, unsigned int strength, unsigned int intelligence, unsigned int dexterity, unsigned int armor, unsigned int MR, float doubleStrikeChance, float poisonDMG)
    : Warrior(name, kingdom, personality, bodytype, criticalChance, lvl, exp, expNextLvl, strength, intelligence, dexterity, armor, MR),
      Marksman(name, kingdom, personality, bodytype, criticalChance, poisonDMG, doubleStrikeChance, lvl, exp, expNextLvl, strength, intelligence, dexterity),
      Character(name, kingdom, personality, bodytype, 1500, 120, 1.5, criticalChance, lvl, exp, expNextLvl, strength, intelligence, dexterity)
{
    this->armor = armor;
    this->magicResistance = MR;
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
}
WarGeneral::WarGeneral() : Warrior(), Marksman(), Character(1500, 70, 1.6, 12, 10, 8, 0.1)
{
    setPassive();
    setDamageAbility1();
    setDamageAbility2();
};

WarGeneral::WarGeneral(const WarGeneral &warGeneral) : Warrior(warGeneral), Marksman(warGeneral){};
WarGeneral &WarGeneral ::operator=(const WarGeneral &warGeneral)
{
    if (this != &warGeneral)
    {
        // explicit call for = defined for WarGeneral
        Character ::operator=(warGeneral);

        this->armor = warGeneral.armor;
        this->magicResistance = warGeneral.magicResistance;
        this->poisonDMG = warGeneral.poisonDMG;
        this->baseDoubleStrikeChance;
    }
    return *this;
}

istream &WarGeneral ::input(istream &in)
{
    Character ::input(in);
    cout << "Armor : ";
    in >> this->armor;
    cout << "Magic Resistance : ";
    in >> this->magicResistance;
    cout << "Poison damage : ";
    in >> this->poisonDMG;
    cout << "Double strike chance : ";
    in >> this->doubleStrikeChance;

    return in;
}

ostream &WarGeneral ::print(ostream &print = cout) const
{
    Character ::print(print);
    setConsoleColor(9);
    print << "\nArmor : " << this->armor
          << "\nMagic resitance : " << this->magicResistance
          << "\nPoison damage : " << this->poisonDMG
          << "\nDouble strike chance : " << this->doubleStrikeChance;
    setConsoleColor(15);

    return print;
}

ostream &WarGeneral ::printStats(ostream &print = cout) const
{
    Character ::printStats(print);
    setConsoleColor(14);
    print << "\nArmor : " << this->armor
          << "\nMagic resitance : " << this->magicResistance
          << "\nPoison damage : " << this->poisonDMG
          << "\nDouble strike chance : " << this->doubleStrikeChance;

    setConsoleColor(15);
}

void WarGeneral ::bonusStatsCalculator(unsigned int str, unsigned int dex, unsigned int intelligence)
{
    this->damage += this->strength - str;
    this->healthPoints += (this->strength - str) * 20;
    this->armor += (this->dexterity - dex) * 5;
    this->magicResistance += (this->dexterity - dex) * 2;
    this->magicResistance += (this->intelligence - intelligence) * 5;
    this->doubleStrikeChance += (this->intelligence - intelligence) * 0.005;
    this->poisonDMG += (this->dexterity - dex) * 0.001;
}

void WarGeneral ::levelUP()
{
    int cpyStr = this->strength;
    int cpyDex = this->dexterity;
    int cpyInt = this->intelligence;
    int cpyHP = this->healthPoints;
    int cpyDMG = this->damage;
    int cpyArm = this->armor;
    int cpyMR = this->magicResistance;
    int cpyPoisonDMG = this->poisonDMG;
    int cpyDoubleStrikeChance = this->doubleStrikeChance;
    int cpyCriticalChance = this->criticalChance;

    levelUpScreen();
    bonusStatsCalculator(cpyStr, cpyDex, cpyInt);

    cout << "Some Stats have changed :)";
    setConsoleColor(9);
    cout << "\nHealth : " << cpyHP << " -> " << (int)(this->healthPoints += 0.1 * this->healthPoints);
    cout << "\nDamage : " << cpyDMG << " -> " << (int)(this->damage += 0.05 * this->damage);
    cout << "\nArmor : " << cpyArm << " -> " << ++this->armor;
    cout << "\nMagic Resist : " << cpyMR << " -> " << ++this->magicResistance;
    cout << "\nPoison damage: " << cpyPoisonDMG * this->damage << " -> " << (this->poisonDMG += 0.5 * this->poisonDMG) * this->damage;
    cout << "\nCritical strike chance: " << cpyCriticalChance * 100 << "%"
         << " -> " << (this->criticalChance += 0.1 * this->criticalChance) * 100 << "%";
    cout << "\nDouble strike chance : " << cpyDoubleStrikeChance * 100 << "%"
         << " -> " << (this->doubleStrikeChance += 0.05 * this->doubleStrikeChance) * 100 << "%";
    cout << "\nExperience needed to get to the next level : " << (this->expNeededForLvl += 1000);
    setConsoleColor(14);
    cout << "\nPress anything to continue...";
    cin.get();
    system("cls");
}

void WarGeneral ::setPassive()
{
    this->passive.name = "Combat experience";
    this->passive.description = "For the remainder of the fight your armor and magic resistance get a boost";
}

void WarGeneral ::setDamageAbility1()
{
    this->damageAbility1.name = "Hawkeye";
    this->damageAbility1.description = "You fire a deadly bullet with your sniper rifle. You have a slight chance that your hit will deal double damage.";
}

void WarGeneral ::setDamageAbility2()
{
    this->damageAbility2.name = "Old man strikes hard";
    this->damageAbility2.description = "You use your combat skills to destroy your enemies. Bonus damage vs mages";
}

void WarGeneral ::Passive()
{
    this->armor += this->armor * 0.05;
    this->magicResistance += this->magicResistance * 0.04;
    this->poisonDMG += 0.02;
}

int WarGeneral ::ability1()
{
    int number;
    number = rand() % 101;
    if (number > 80)
        return this->damage * 2;
    else
        return this->damage;
}

int WarGeneral ::ability2(bool x = false)
{
    return this->damage * 1.2;
}

void WarGeneral ::takeDMG(int damage, bool wizard)
{
    if (wizard)
        damage -= 1.3 * this->magicResistance;
    else
        damage -= 1.5 * this->armor;
    this->healthPoints -= damage;
}

void WarGeneral ::printInGameStats() const
{
    Character ::printInGameStats();
    cout << "\nArmor : " << this->armor;
    cout << "\nMagic resistance : " << this->magicResistance;
    cout << "\nPoison damage : " << this->poisonDMG;
    cout << "\nDouble strike chance" << this->doubleStrikeChance;
}

void createBots(vector<Character *> &botList)
{
    Character *bot1 = new Warrior("Alexander the betrayer", "Valley of the Wolf", "Evil", "Brute", 0.2, 1, 500, 500, 18, 2, 8, 15, 20);
    Character *bot2 = new Mage("Faraday the priest", "Mountain of Guisan", "Wise", "Light", 0, 1, 0, 1000, 3, 25, 6, 500, 0.2, 150);
    Character *bot3 = new Marksman("Harry Quickeye", "Sherwood Forests", "Sly", "Fit", 0.35, 0.02, 0.35, 1, 0, 1000, 7, 5, 20);
    botList.push_back(bot1);
    botList.push_back(bot2);
    botList.push_back(bot3);
}

void botDamage(Character *player, Character *bot, vector<string> &battleLog)
{
    system("cls");
    int abilityVoter = rand() % 4 + 1;
    int initialHP = bot->getHP();
    string message;
    if (abilityVoter == 1)
    {
        bot->takeaNap(initialHP);
        ostringstream buffer;
        buffer << bot->getName() << " uses " << bot->getSleepName() << " to recover his strength";
        message = buffer.str();
        battleLog.push_back(message);
    }
    else if (abilityVoter == 2)
    {
        bot->Passive();
        ostringstream buffer;
        buffer << bot->getName() << " uses " << bot->getPassiveName() << " and his stats go wild";
        message = buffer.str();
        battleLog.push_back(message);
    }
    else if (abilityVoter == 3)
    {
        ostringstream buffer;
        int damageDealt = bot->ability1();
        int x = 100 - bot->getCriticalStrikeChance();
        int y = rand() % 101;
        if (y > x)
        {
            damageDealt += bot->getDamage() * bot->getCriticalStrikeChance();
        }
        player->takeDMG(damageDealt);
        buffer << bot->getName() << " uses " << bot->getAbility1Name() << " and deals " << damageDealt << " damage";
        message = buffer.str();
        battleLog.push_back(message);
    }
    else
    {

        ostringstream buffer;
        int damageDealt = bot->ability2(false);
        int x = 100 - bot->getCriticalStrikeChance();
        int y = rand() % 101;
        if (y > x)
        {
            damageDealt += bot->getDamage() * bot->getCriticalStrikeChance();
        }
        player->takeDMG(damageDealt);
        buffer << bot->getName() << " uses " << bot->getAbility2Name() << " and deals " << damageDealt << " damage";
        message = buffer.str();
        battleLog.push_back(message);
    }
    setConsoleColor(4);
    cout << bot->getName() << '\n';
    bot->printInGameStats();
    setConsoleColor(6);
    cout << "\n\nBattle log :\n";
    for (auto x : battleLog)
        cout << '\n'
             << x;
    cin.get();
    system("cls");
}

int main()
{
    vector<string> battleLog;
    vector<Character *> bots = vector<Character *>();
    createBots(bots);
    srand(time(0));
    // Menu
    while (true)
    {
        system("cls");
        char userInput;
        setConsoleColor(89);
        cout << "-----------------------------------Welcome mighty gamer-----------------------------------";
        setConsoleColor(15);
        cout << "\n1.Start Game"
             << "\n2.View Characters"
             << "\n3.Developer Options";
        cout << "\n\nNote : to select the options press the option number then ENTER"
             << "\nTo exit the console press ESC";

        userInput = _getch();

        switch (userInput)
        {
        case 49:
            while (true)
            {
                system("cls");
                char userInput;
                cout << "It's time to select a class"
                     << "\n1.Warrior"
                     << "\n2.Mage"
                     << "\n3.Marksman"
                     << "\n4.War General";
                userInput = _getch();
                system("cls");
                cout << "Enter some of your character info :\n";
                Character *player;
                Character *playerCPY;

                if (userInput == 49)
                {
                    player = new Warrior();
                    playerCPY = new Warrior();
                }
                else if (userInput == 50)
                {
                    player = new Mage();
                    playerCPY = new Mage();
                }
                else if (userInput == 51)
                {
                    player = new Marksman();
                    playerCPY = new Marksman();
                }
                else if (userInput == 52)
                {
                    player = new WarGeneral();
                    playerCPY = new WarGeneral();
                }
                else if (userInput == 27)
                    exit(1);
                else
                {
                    system("cls");
                    continue;
                }
                player->userInput();
                *playerCPY = *player;
                while (true)
                {
                    system("cls");
                    cout << "1.View character"
                         << "\n2.Fight";
                    userInput = _getch();
                    switch (userInput)
                    {
                    case 49:
                        system("cls");
                        char userInput;
                        cout << *player;
                        cout << "\nPress BACKSPACE to go back";
                        userInput = _getch();
                        while (userInput != 8)
                            userInput = _getch();
                        continue;
                        break;
                    case 50:
                    {
                        system("cls");
                        int botVoter = rand() % bots.size();
                        int initialPlayerHP = player->getHP();
                        int initalBotHP = bots[botVoter]->getHP();
                        playerCPY->playerScreen(battleLog, bots[botVoter]);
                        player->setXP(playerCPY->getXP());
                        player->checkXP();
                        *playerCPY = *player;
                        battleLog.clear();
                        break;
                    }
                    case 27:
                        exit(1);

                    default:
                        system("cls");
                        continue;
                    }
                }
            }
        case 50:
            while (true)
            {
                system("cls");
                cout << "Choose the class you'd like to see more info"
                     << "\n1.Warrior"
                     << "\n2.Mage"
                     << "\n3.Marksman"
                     << "\n4.War General"
                     << "\nPress BACKSPACE to go back...";
                char userInput;
                userInput = _getch();
                Character *infoClass;
                if (userInput == 8)
                    break;
                switch (userInput)
                {
                case 49:
                    while (true)
                    {
                        infoClass = new Warrior();
                        char userInput;
                        system("cls");
                        cout << *infoClass;
                        cout << "\nPress BACKSPACE to go back...";
                        userInput = _getch();
                        if (userInput == 8)
                            break;
                    }
                    break;
                case 50:
                    while (true)
                    {
                        infoClass = new Mage();
                        char userInput;
                        system("cls");
                        cout << *infoClass;
                        cout << "\nPress BACKSPACE to go back...";
                        userInput = _getch();
                        if (userInput == 8)
                            break;
                    }
                    break;
                case 51:
                    while (true)
                    {
                        infoClass = new Marksman();
                        char userInput;
                        system("cls");
                        cout << *infoClass;
                        cout << "\nPress BACKSPACE to go back...";
                        userInput = _getch();
                        if (userInput == 8)
                            break;
                    }
                    break;
                case 52:
                    while (true)
                    {
                        infoClass = new WarGeneral();
                        char userInput;
                        system("cls");
                        cout << *infoClass;
                        cout << "\nPress BACKSPACE to go back...";
                        userInput = _getch();
                        if (userInput == 8)
                            break;
                    }
                    break;
                case 27:
                    exit(1);
                default:
                    system("cls");
                    continue;
                }
            }
            break;
        case 51:
            while (true)
            {
                system("cls");
                cout << "1.View bot list"
                     << "\n2.Create bot"
                     << "\nPress BACKSPACE to go back...";
                char userInput;
                userInput = _getch();
                if (userInput == 8)
                    break;
                switch (userInput)
                {
                case 49:
                {
                    system("cls");
                    for (auto bot : bots)
                        cout << *bot << "\n\n";
                    cout << "Press BACKSPACE to go back...";
                    char userInput;
                    userInput = _getch();
                    while (userInput != 8)
                        userInput = _getch();
                }
                break;
                case 50:
                    while (true)
                    {
                        system("cls");
                        cout << "Choose bot class"
                             << "\n1.Warrior"
                             << "\n2.Mage"
                             << "\n3.Marksman"
                             << "\n4.War General"
                             << "\nPress BACKSPACE to go back...";
                        char userInput;
                        Character *bot;
                        userInput = _getch();
                        if (userInput == 8)
                            break;
                        switch (userInput)
                        {
                        case 49:

                            bot = new Warrior();
                            system("cls");
                            cin >> *bot;
                            bots.push_back(bot);
                            cout << "\nPress BACKSPACE to go back...";
                            userInput = _getch();
                            while (userInput != 8)
                                userInput = _getch();
                            break;

                            break;
                        case 50:

                            bot = new Mage();
                            system("cls");
                            cin >> *bot;
                            bots.push_back(bot);
                            cout << "\nPress BACKSPACE to go back...";
                            userInput = _getch();
                            while (userInput != 8)
                                userInput = _getch();
                            break;

                        case 51:

                            bot = new Marksman();
                            system("cls");
                            cin >> *bot;
                            bots.push_back(bot);
                            cout << "\nPress BACKSPACE to go back...";
                            userInput = _getch();
                            while (userInput != 8)
                                userInput = _getch();
                            break;

                            break;
                        case 52:

                            bot = new WarGeneral();
                            char userInput;
                            system("cls");
                            cin >> *bot;
                            bots.push_back(bot);
                            cout << "\nPress BACKSPACE to go back...";
                            userInput = _getch();
                            while (userInput != 8)
                                userInput = _getch();
                            break;

                        case 27:
                            exit(1);
                        default:
                            system("cls");
                            continue;
                        }
                    }
                }
            }
            break;
        case 27:
            exit(1);
        case 8:
            break;
        default:
            system("cls");
            continue;
        }
    }
    return 0;
}