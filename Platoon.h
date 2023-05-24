#ifndef _Platoon_H_
#define _Platoon_H_

#include <vector>
#include <string>
#include <sstream>
#include "Car.h"

using namespace std;

class Platoon
{
    private:
        Car* head = NULL;
        Car* tail = NULL;

        /*private helper functions*/
        void append(Car* c); 
        void prepend(Car* c); 
        void insertAfter(Car* c);
        Car* search(Car* c);

    public:
        Platoon(string init);
        Car* get_tail();
        Car* get_head();

        /*new public functions*/
        void remove(Car* c); 
        bool insert(Car* c);
        void moveFoward(Car* c);
        bool free(Car* c);
};
#endif
