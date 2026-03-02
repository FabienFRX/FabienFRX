// This file contains all the code snippets available in the subject

/*
Exercise 0
*/

int main(void)
{
    int a = 42;
    int b = 21;

    ::swap(a, b);
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "min(a, b) = " << ::min(a, b) << std::endl;
    std::cout << "max(a, b) = " << ::max(a, b) << std::endl;
    std::cout << "clamp(0, a, b) = " << ::clamp(0, a, b) << std::endl;

    std::string c = "ghi";
    std::string d = "abc";

    ::swap(c, d);
    std::cout << "c = " << c << ", d = " << d << std::endl;
    std::cout << "min(c, d) = " << ::min(c, d) << std::endl;
    std::cout << "max(c, d) = " << ::max(c, d) << std::endl;
    std::cout << "clamp(\"def\", c, d) = " << ::clamp(std::string("def"), c, d) << std::endl;

    return 0;
}

/*
Exercise 1
*/

int main(void)
{
    Array<int, 3>   array;

    try {
        array[0] = 21;
        array[1] = 42;
        array[2] = 84;
        array[3] = 84;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << array << std::endl;
    array
        .convert<float>([](const int& v) { return static_cast<float>(v) / 10.f; })
        .forEach([](const float& v) { std::cout << v << std::endl; });
    return 0;
}

/*
Exercise 2
*/

int main(void)
{
    Stack stack;

    stack.push(42.21);
    stack.push(84.42);
    stack.push(21.12);
    stack.add();
    stack.div();

    try {
        stack.mul();
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    std::cout << stack.top() << std::endl;
    return 0;
}

/*
Exercise 3
*/

class Example
{
private:
    int _id;

public:
    Example(int id) : _id(id) { std::cout << "#" << _id << " construction" << std::endl; }
    ~Example() { std::cout << "#" << _id << " destruction" << std::endl; }

    void method() const { std::cout << "#" << _id << " method" << std::endl; }
};

int main(void)
{
    UniquePointer<Example>  ptr1(new Example(1));
    UniquePointer<Example>  ptr2(new Example(2));

    ptr1.reset();
    ptr2 = new Example(3);
    ptr2.get()->method();
    ptr2->method();

    return 0;
}

/*
Exercise 4
*/

int main(void)
{
    Command command;
    Stack stack;

    try {
        command.registerCommand("push", [&stack]() { stack.push(4.2); });
        command.registerCommand("display", [&stack]() { std::cout << stack.top() << std::endl; });
        command.registerCommand("add", [&stack]() { stack.add(); });
        command.registerCommand("sub", [&stack]() { stack.sub(); });
        command.registerCommand("mul", [&stack]() { stack.mul(); });
        command.registerCommand("div", [&stack]() { stack.div(); });
        command.registerCommand("display", []() {});
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        command.executeCommand("push");
        command.executeCommand("push");
        command.executeCommand("push");
        command.executeCommand("add");
        command.executeCommand("div");
        command.executeCommand("display");
        command.executeCommand("nau");
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

/*
Exercise 5
*/

class Example
{
private:
    int _id;

public:
    Example(int id) : _id(id) { std::cout << "#" << _id << " construction" << std::endl; }
    ~Example() { std::cout << "#" << _id << " destruction" << std::endl; }

    void method() const { std::cout << "#" << _id << " method" << std::endl; }
};

int main(void)
{
    SharedPointer<Example> ptr1(new Example(1));
    SharedPointer<Example> ptr2(ptr1);
    SharedPointer<Example> ptr3;

    ptr1.reset();
    ptr3 = ptr2;
    ptr2 = new Example(2);
    ptr2.get()->method();
    ptr3->method();

    return 0;
}

/*
Exercise 6
*/

int main(void)
{
    Shell shell(std::cin);
    Stack stack;
    Command command;

    command.registerCommand("push", [&shell, &stack]() { stack.push(shell.extract<double>()); });
    command.registerCommand("display", [&stack]() { std::cout << stack.top() << std::endl; });
    command.registerCommand("add", [&stack]() { stack.add(); });
    command.registerCommand("sub", [&stack]() { stack.sub(); });
    command.registerCommand("mul", [&stack]() { stack.mul(); });
    command.registerCommand("div", [&stack]() { stack.div(); });

    while (true) {
        try {
            std::cout << "> " << std::flush;
            shell.next();
            command.executeCommand(shell.extract<std::string>());
        }
        catch (const Shell::Error& e) {
            std::cout << e.what() << std::endl;
            break;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
    return 0;
}

/*
Exercise 7
*/

int main(void)
{
    Matrix<3, 1> point;
    Matrix<3, 3> translation;
    Matrix<3, 3> rotation;

    point(0, 0) = 3;
    point(1, 0) = 2;
    point(2, 0) = 1;
    std::cout << "P: " << point << std::endl;

    translation(0, 0) = translation(1, 1) = translation(2, 2) = 1.0;
    translation(0, 2) = 4;
    translation(1, 2) = 2;
    point = translation * point;
    std::cout << "T: " << translation << std::endl;
    std::cout << "P: " << point << std::endl;

    rotation(0, 0) = +std::cos(std::numbers::pi / 2);
    rotation(0, 1) = -std::sin(std::numbers::pi / 2);
    rotation(1, 0) = +std::sin(std::numbers::pi / 2);
    rotation(1, 1) = +std::cos(std::numbers::pi / 2);
    rotation(2, 2) = 1;
    point = rotation * point;
    std::cout << "R: " << rotation << std::endl;
    std::cout << "P: " << point << std::endl;

    return 0;
}
