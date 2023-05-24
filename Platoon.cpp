#include "Platoon.h"
#include <iostream>
using namespace std;

Platoon::Platoon(string init)
{
    // the string 'init' looks like:
    // a,b ; c,d ; ... where, a and c are the vehicle id, b and d are positions.
    stringstream ss(init);
	string segment;
	while(getline(ss, segment, ';'))
	{
		size_t found = segment.find(",");
		if (found!=std::string::npos){
			string id = segment.substr(0, found);
			string pos = segment.substr(found+1, segment.size()-1);

			// conversion to integer
			stringstream s1(id);
			int id_value;
			s1 >> id_value;
			stringstream s2(pos);
			int pos_value;
			s2 >> pos_value;
			Car* car = new Car(id_value, pos_value);

			// add car to list
			insert(car);
		}
	}
}

Car* Platoon::get_tail()
{
	return tail;
}

Car* Platoon::get_head()
{
	return head;
}

/*
 * A function to remove a node from a list
 */
void Platoon::remove(Car* c)
{
	if (head == NULL || tail == NULL || c == NULL)	//if linked list is empty
	{return;}
	
	if (c == head)									//if head needs to be removed
	{head = c->get_next();}

	if (c == tail)									//if tail needs to be removed
	{tail = c->get_prev();}
	
	if (c->get_next() != NULL)						//change next only if node is not the last node
	{c->get_next()->set_prev(c->get_prev());}
	
	if (c->get_prev() != NULL)						//change prev only if node is not the first node
	{c->get_prev()->set_next(c->get_next());}
}

/*
 * A function to add a node at the end of a list
 */
void Platoon::append(Car* c)
{
	c->set_next(NULL);						//set next of node to null since its the last in the list

	if (head == NULL && tail == NULL)		//set new node as head and tail if the list in null
	{
		head = c;
		tail = c;
	}

	tail->set_next(c);						//if list is not null
	c->set_prev(tail);
	tail = c;
}

/*
 * A function to add a node to the front of a list
 */
void Platoon::prepend(Car* c)
{
	c->set_next(head);			//make next of c as head and previous as NULL
	c->set_prev(NULL);

	if (head != NULL)			//change prev of head node to c
	{head->set_prev(c);}

	head = c; 					//move the head to point to c
}

/*
 * A function to insert a node after another specified node
 */
void Platoon::insertAfter(Car* c)
{
	Car* prev_node = search(c);				//get the prev node

	if (prev_node == NULL)					//check that prev node is not NULL
	{return;}

	c->set_next(prev_node->get_next());		//set next of c as next of prev node
	prev_node->set_next(c);					//set c to next of prev node
	c->set_prev(prev_node);					//set prev of c as prev node

	if (c->get_next() != NULL)				//set prev of c's next to c
	{c->get_next()->set_prev(c);}
}

/*
 * A function that determines the correct insert to use depending on where the node is being inserted
 */
bool Platoon::insert(Car* c)
{
	if (head == NULL || tail == NULL) //if lane is empty
	{
		head = c;
		tail = c;
		c->set_prev(NULL);
		c->set_next(NULL);
	}
	else if (c->get_position() < head->get_position()) //if inserting before head
	{
		prepend(c);
		return true;
	}
	else if (c->get_position() > tail->get_position()) //if inserting after tail
	{
		append(c);
		return true;
	}
	else //insert after a node in the list
	{
		insertAfter(c);
		return true;
	}
	return false;
}

/*
 * A function to search for the position after a given node
 */
Car* Platoon::search(Car* c)
{
	Car* current = tail;

	while (current != NULL)
	{
		if (c->get_position() > current->get_position())
		{
			return current;}
		current = current->get_prev();
	}
	return NULL;
}

/*
 * A function to move a car foward
 */
void Platoon::moveFoward(Car* c)
{
	if (c == tail) //check if tail
	{c->set_position(c->get_position() + 1);}
	else if ((c->get_position() + 1) != c->get_next()->get_position()) //check if car in position + 1
	{c->set_position(c->get_position() + 1);}
}

/*
 * A function to check if a position is free in the linked list for another node
 */
bool Platoon::free(Car* c)
{
	Car* current = tail;

	while (current != NULL)
	{
		if (c->get_position() == current->get_position())
		return false;
		current = current->get_prev();
	}
	return true;
}