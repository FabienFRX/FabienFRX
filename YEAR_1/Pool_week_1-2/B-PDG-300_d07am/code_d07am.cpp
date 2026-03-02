// This file contains all the code snippets available in the subject

/*
Exercise 0
*/

int main()
{
    Federation::Starfleet::Ship UssJanissary(289, 132, "Janissary", 6);
    Federation::Ship Independent(150, 230, "Nomad");
    WarpSystem::QuantumReactor QR;
    WarpSystem::QuantumReactor QR2;
    WarpSystem::Core core(&QR);
    WarpSystem::Core core2(&QR2);

    UssJanissary.setupCore(&core);
    UssJanissary.checkCore();
    Independent.setupCore(&core2);
    Independent.checkCore();

    QR.setStability(false);
    QR2.setStability(false);
    UssJanissary.checkCore();
    Independent.checkCore();

    return 0;
}

/*
Exercise 1
*/

int main()
{
    Federation::Starfleet::Ship UssJanissary(289, 132, "Janissary", 6);
    Federation::Starfleet::Captain James("James T. Kirk");
    Federation::Starfleet::Ensign Ensign("Pavel Chekov");
    WarpSystem::QuantumReactor QR;
    WarpSystem::QuantumReactor QR2;
    WarpSystem::Core core(&QR);
    WarpSystem::Core core2(&QR2);

    UssJanissary.setupCore(&core);
    UssJanissary.checkCore();
    UssJanissary.promote(&James);

    Borg::Ship Cube;
    Cube.setupCore(&core2);
    Cube.checkCore();

    return 0;
}

/*
Exercise 4
*/

int main()
{
    Federation::Starfleet::Ship UssJanissary(289, 132, "Janissary", 6, 20);
    Federation::Starfleet::Admiral Pierce("Pierce");
    Borg::Ship Cube(25);

    Pierce.fire(&UssJanissary, &Cube);
    Pierce.fire(&UssJanissary, &Cube);

    return 0;
}

/*
Exercise 5
*/

int main()
{
    Exam e = Exam(&Exam::cheat);

    e.kobayashiMaru = &Exam::start;
    (e.*e.kobayashiMaru)(3);
    Exam::cheat = true;
    if (e.isCheating())
        (e.*e.kobayashiMaru)(4);
}
