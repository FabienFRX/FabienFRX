// This file contains all the code snippets available in the subject

/*
Exercise 0
*/

int main()
{
    Skat s("Junior", 5);

    std::cout << "Soldier " << s.name() << std::endl;
    s.status();
    s.useStimPaks();

    return 0;
}



/*
Exercise 1
*/

int main()
{
    KoalaBot kb;

    std::cout << std::boolalpha << kb.status() << std::endl;
    kb.informations();
    return 0;
}

/*
Exercise 2
*/

int main()
{
    KreogCom k(42, 42, 101010);

    k.addCom(56, 25, 65);
    k.addCom(73, 34, 51);

    k.locateSquad();

    k.removeCom();
    k.removeCom();

    return 0;
}


/*
Exercise 3
*/

int main()
{
    Phaser p(5, Phaser::ROCKET);

    p.fire();
    p.reload();

    std::cout << "Firing all ammunition" << std::endl;
    while (p.getCurrentAmmos() > 0)
        p.fire();

    return 0;
}

/*
Exercise 4
*/

int main()
{
    Squad s(0, 0, Phaser::REGULAR);

    s.fire();

    return 0;
}
