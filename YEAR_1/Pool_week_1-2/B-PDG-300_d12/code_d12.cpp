// This file contains all the code snippets available in the subject

/*
Exercise 0
*/

int main()
{
    Toy toto;
    Toy ET(Toy::ALIEN, "green", "./alien.txt");

    toto.setName("TOTO !");

    if (toto.getType() == Toy::BASIC_TOY)
        std::cout << "basic toy: " << toto.getName() << std::endl
                  << toto.getAscii() << std::endl;
    if (ET.getType() == Toy::ALIEN)
        std::cout << "this alien is: " << ET.getName() << std::endl
                  << ET.getAscii() << std::endl;
    return 0;
}

/*
Exercise 3
*/

int main()
{
    std::unique_ptr<Toy> b(new Buzz("buzziiiii"));
    std::unique_ptr<Toy> w(new Woody("wood"));
    std::unique_ptr<Toy> t(new Toy(Toy::ALIEN, "ET", "alien.txt"));

    b->speak("To the code, and beyond !!!!!!!!");
    w->speak("There's a snake in my boot.");
    t->speak("the claaaaaaw");
}

/*
Exercise 4
*/

int main()
{
    Toy a(Toy::BASIC_TOY, "REX", "example.txt");

    std::cout << a;
    a << "rawr xD";
    std::cout << a;
}

/*
Exercise 5
*/

int main()
{
    Woody w("wood");

    if (w.setAscii("file_who_does_not_exist.txt") == false) {
        auto e = w.getLastError();
        if (e.type == Toy::Error::PICTURE) {
                std::cout << "Error in " << e.where() << ": " << e.what() << std::endl;
        }
    }

    if (w.speak_es("Woody does not have spanish mode") == false) {
        auto e = w.getLastError();
        if (e.type == Toy::Error::SPEAK) {
            std::cout << "Error in " << e.where() << ": " << e.what() << std::endl;
        }
    }

    if (w.speak_es("Woody does not have spanish mode") == false) {
        auto e = w.getLastError();
        if (e.type == Toy::Error::SPEAK) {
            std::cout << "Error in " << e.where() << ": " << e.what() << std::endl;
        }
    }
}

/*
Exercise 6
*/

int main()
{
    Buzz b("buzzi");
    Woody w("wood");

    ToyStory::tellMeAStory("superStory.txt", b, &Toy::speak_es, w, &Toy::speak);
}
