// This file contains all the code snippets available in the subject

/*
Exercise 0
*/

int main(void)
{
    Orange o;
    Strawberry s;
    const Almond a;
    IFruit& f = o;

    std::cout << o.getName() << ": " << o.getVitamins() << " vitamins" << std::endl;
    std::cout << s << std::endl;
    std::cout << a << std::endl;

    o.peel();
    std::cout << f << std::endl;

    return 0;
}

/*
Exercise 1
*/

int main(void)
{
    FruitBox box(3);
    const FruitBox& cref = box;

    box.pushFruit(new TestFruit("Cerise"));
    box.pushFruit(new TestFruit("Framboise"));
    box.pushFruit(new TestFruit("Anis"));
    std::cout << cref << std::endl;

    IFruit* tmp = new TestFruit("Serge");

    std::cout << box.pushFruit(tmp) << std::endl;
    delete tmp;

    tmp = box.popFruit();
    delete tmp;
    std::cout << cref << std::endl;

    return 0;
}

/*
Exercise 4
*/

int main(void)
{
    FruitFactory factory;

    factory.registerFruit(new Raspberry);
    factory.registerFruit(new BloodOrange);
    factory.registerFruit(new Almond);
    factory.registerFruit(new Coconut);
    factory.registerFruit(new Almond);

    factory.unregisterFruit("banana");
    factory.unregisterFruit("coconut");

    IFruit* fruit1 = factory.createFruit("almond");
    IFruit* fruit2 = factory.createFruit("coconut");
    IFruit* fruit3 = factory.createFruit("tomato");

    std::cout << *fruit1 << std::endl;
    std::cout << fruit2 << std::endl;
    std::cout << fruit3 << std::endl;

    delete fruit1;

    return 0;
}

/*
Exercise 5
*/

int main(void)
{
    FruitBox box(5);
    FruitMixer yourMixer;
    IFruitMixer& mixer(yourMixer);
    SteelBlade blade;
    IFruit* fruit;

    fruit = new Orange;
    fruit->peel();
    box.pushFruit(fruit);
    box.pushFruit(new Lemon);
    box.pushFruit(new Strawberry);
    box.pushFruit(new Almond);
    std::cout << box << std::endl;

    unsigned int vitamins = mixer.mixFruits(box);

    std::cout << "result: " << vitamins << std::endl;
    std::cout << box << std::endl;

    mixer.setBlade(&blade);
    vitamins = mixer.mixFruits(box);
    std::cout << "result: " << vitamins << std::endl;
    std::cout << box << std::endl;

    return 0;
}

/*
Exercise 6
*/

int main(void)
{
    IFruit* fruit = new Strawberry;
    Hack* hack = reinterpret_cast<Hack*>(fruit);

    std::cout << *fruit << std::endl;
    hack->hack(1138);
    std::cout << *fruit << std::endl;
    delete fruit;
    return 0;
}
