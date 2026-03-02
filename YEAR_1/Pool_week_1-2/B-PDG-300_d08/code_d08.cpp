// This file contains all the code snippets available in the subject

/*
Exercise 0
*/

int main()
{
    Droid d;
    Droid d1("Avenger");
    size_t Durasel = 200;

    std::cout << d << std::endl;
    std::cout << d1 << std::endl;
    d = d1;
    d.setStatus(new std::string("Kill Kill Kill!"));
    d << Durasel;
    std::cout << d << "--" << Durasel << std::endl;
    Droid d2 = d;
    d.setId("Rex");
    std::cout << (d2 != d) << std::endl;

    return 0;
}

/*
Exercise 1
*/

int main()
{ 
    DroidMemory mem1;
    mem1 += 42;

    DroidMemory mem2 = mem1;
    std::cout << mem1 << std::endl;

    DroidMemory mem3;
    mem3 << mem1;
    mem3 >> mem1;
    mem3 << mem1;

    std::cout << mem3 << std::endl;
    std::cout << mem1 << std::endl;
}

/*
Exercise 2
*/

static void testMemory()
{
    DroidMemory mem1;
    mem1 += 42;
    std::cout << mem1 << std::endl;

    DroidMemory mem2;
    mem2 << mem1;
    mem2 >> mem1;
    mem2 << mem1;

    std::cout << mem2 << std::endl;
    std::cout << mem1 << std::endl;

    DroidMemory mem3 = mem1;
    DroidMemory mem4;
    mem4 = mem1 + mem3;
}

static void testDroids()
{
    Droid d("rudolf");
    Droid d2("gaston");
    size_t DuraSell = 40;

    d << DuraSell;
    d.setStatus(new std::string("having some reset"));
    d2.setStatus(new std::string("having some reset"));

    if (d2 != d && !(d == d2))
    std::cout << "a droid is a droid, all its matter is what it's doing" << std::endl;

    d(new std::string("take a coffee"), 20);
    std::cout << d << std::endl;
    while (d(new std::string("Patrol around"), 20)) {
        if (!d(new std::string("Shoot some ennemies"), 50))
            d(new std::string("Run Away"), 20);
        std::cout << d << std::endl;
    }
    std::cout << d << std::endl;
}

int main()
{
    testMemory();
    testDroids();
    return 0;
}


/*
Exercise 3
*/

int main()
{
    Carrier c("HellExpress");
    Droid *d1= new Droid("Commander");
    Droid *d2 = new Droid("Sergent");
    Droid *d3 = new Droid("Troufiont");
    Droid *d4 = new Droid("Groupie");
    Droid *d5 = new Droid("BeerHolder");

    c << d1 << d2 << d3 << d4 << d5;
    std::cout << c.getSpeed() << d1 << std::endl;
    c >> d1 >> d2 >> d3;
    std::cout << c.getSpeed() << std::endl;
    c[0] = d1;
    std::cout << (~c).getSpeed() << std::endl;
    c(4, 2);
    std::cout << c << std::endl;
    c(-15, 4);
    std::cout << c << std::endl;
    c[3] = 0;
    c[4] = 0;
    (~c)(-15, 4);
    std::cout << c << std::endl;
    return 0;
}

/*
Exercise 4
*/

int main()
{
    Droid **w = new Droid*[10];
    char c = '0';

    for (int i = 0; i < 3; ++i)
        w[i] = new Droid(std::string("wreck: ") + (char)(c + i));
    Supply s1(Supply::Silicon, 42);
    Supply s2(Supply::Iron, 70);
    Supply s3(Supply::Wreck, 3, w);
    std::cout << s3 << std::endl;

    size_t s = s2;
    std::cout << s << std::endl;
    std::cout << *(*(--s3)) << std::endl;
    std::cout << *(++s3)->getStatus() << std::endl;
    ++s3;
    *s3 = 0;
    std::cout << *s3 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << !s3 << std::endl;

    return 0;
}

/*
Exercise 5
*/
int main()
{
    DroidFactory factory(3);
    Droid **w = new Droid*[10];
    Droid *newbie;
    char c = '0';

    for (int i = 0; i < 3; ++i) {
        w[i] = new Droid(std::string("wreck: ") + (char)(c + i));
        *(w[i]->getBattleData()) += (i * 100);
    }

    Supply s1(Supply::Silicon, 42);
    Supply s2(Supply::Iron, 70);
    Supply s3(Supply::Wreck, 3, w);

    factory >> newbie;
    std::cout << newbie << std::endl;

    factory << s1 << s2;
    std::cout << factory << std::endl;
    s3 >> factory >> newbie;
    std::cout << factory << std::endl;
    factory++ >> newbie;
    std::cout << *newbie->getBattleData() << std::endl;
    --factory >> newbie;
    std::cout << *newbie->getBattleData() << std::endl;

    return 0;
}