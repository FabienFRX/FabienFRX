// This file contains all the code snippets available in the subject

/*
Exercise 0
*/

int main(void)
{
    Peasant peasant("Gildas", 42);

    peasant.damage(50);
    peasant.damage(100);
    peasant.damage(200);
    peasant.rest();
}

/*
Exercise 1
*/

int main(void)
{
    Knight knight("Arthur", 20);

    knight.attack();
    knight.special();
    knight.rest();
    knight.special();
    knight.damage(50);
}

/*
Exercise 2
*/

int main(void)
{
    Enchanter enchanter("Merlin", 20);

    enchanter.attack();
    enchanter.special();
    enchanter.rest();
    enchanter.special();
    enchanter.damage(50);
}

/*
Exercise 3
*/

int main(void)
{
    Priest priest("Trichelieu", 20);

    priest.attack();
    priest.special();
    priest.rest();
    priest.damage(50);
}

/*
Exercise 4
*/

int main(void)
{
    Paladin paladin("Uther", 99);

    paladin.attack();
    paladin.special();
    paladin.rest();
    paladin.damage(50);
}

/*
Exercise 5
*/

int main(void)
{
    ICharacter *peasant = new Peasant("Gildas", 42);
    ICharacter *knight = new Knight("Arthur", 20);
    ICharacter *enchanter = new Enchanter("Merlin", 20);
    ICharacter *priest = new Priest("Trichelieu", 20);
    ICharacter *paladin = new Paladin("Uther", 99);

    peasant->attack();
    knight->special();
    enchanter->rest();
    priest->damage(21);
    std::cout << paladin->getName() << ": "
              << paladin->getHp() << " health points, "
              << paladin->getPower() << " power points."
              << std::endl;

    delete peasant;
    delete knight;
    delete enchanter;
    delete priest;
    delete paladin;
}

/*
Exercise 6
*/

int main(void)
{
    ICharacter *peasant = new Peasant("Gildas", 42);
    PoisonPotion poison_potion;
    HealthPotion health_potion;
    IPotion& potion = health_potion;

    std::cout << peasant->getName() << ": " << peasant->getHp() << "HP, "
              << peasant->getPower() << " PP." << std::endl;
    peasant->drink(poison_potion);
    std::cout << peasant->getName() << ": " << peasant->getHp() << "HP, "
              << peasant->getPower() << " PP." << std::endl;
    peasant->drink(potion);
    std::cout << peasant->getName() << ": " << peasant->getHp() << "HP, "
              << peasant->getPower() << " PP." << std::endl;

    delete peasant;
}