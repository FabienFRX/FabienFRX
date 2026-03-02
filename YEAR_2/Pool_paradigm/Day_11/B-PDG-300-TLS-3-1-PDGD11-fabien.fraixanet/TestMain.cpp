/*
** EPITECH PROJECT, 2025
** B-PDG-300-TLS-3-1-PDGD11-fabien.fraixanet
** File description:
** TestMain
*/

#include "IDirectoryLister.hpp"
#include "DirectoryLister.hpp"
#include "SafeDirectoryLister.hpp"
#include "UniquePointer.hpp"
#include "IObject.hpp"
#include "TestObject.hpp"

int main(void)
{
    UniquePointer ptr1;
    UniquePointer ptr2(new TestObject("Eccleston"));
    //UniquePointer ptr3(ptr2); <- Does not compile!

    ptr1 = new TestObject("Tennant");
    ptr2 = new TestObject("Smith");
    ptr1->touch();
    (*ptr2).touch();
    {
        UniquePointer ptr4(new TestObject("Whittaker"));
    }
    ptr1.reset(new TestObject("Capaldi"));
    ptr1.swap(ptr2);
    //ptr1 = ptr2; <- Does not compile!
    ptr2.reset();
    return 0;
}