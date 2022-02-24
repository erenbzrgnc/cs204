// eren bezirganci
#ifndef DYNQUEUE_H
#define DYNQUEUE_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

template <class Type>
struct QueueNode{
	Type value;
	QueueNode *next;
    QueueNode(){}
    QueueNode(Type num, QueueNode *ptr = NULL){
		value = num;
		next = ptr;
	}
};

template <class Type>
class DynQueue{
private:
	// These track the front and rear of the queue
	QueueNode<Type> *front;
	QueueNode<Type> *rear;
public:
    DynQueue(void);// Constructor
    ~DynQueue(void);// Destructor
    DynQueue(const DynQueue<Type> &); // deep copy constructor
	// Member functions
	void enqueue(Type);
	void dequeue(Type &);
	bool isEmpty(void) const;
	void clear(void);
    void createClone(QueueNode<Type> * &, QueueNode<Type> * &) const;
    const DynQueue<Type> & operator = (const DynQueue<Type> & rhs);
    void operator += (Type t);
    void operator *= (int t2);
    // friend functions
    template <class T>
    friend ostream & operator << (ostream & os, const DynQueue<T> & dq);
    template <class T2>
    friend DynQueue<T2> operator + (const DynQueue<T2> & lhs2, const DynQueue<T2> & rhs2);
    
   
};


//************************************************
// Constructor. Generates an empty queue         *
//************************************************

template <class Type>
DynQueue<Type>::DynQueue(void){
    front = NULL;
    rear = NULL;
    #ifdef _DEBUG
        cout << "An emmpty queue has been created\n";
    #endif
}

template <class Type>
//************************************************
// Copy constructor. Generates a deep copy         *
//************************************************
DynQueue<Type>::DynQueue(const DynQueue<Type> & copy){
    QueueNode<Type>  *f,  *r;
    copy.createClone(f, r);
    front=f;
    rear=r;

}

//************************************************
// Create a clone         *
//************************************************
template <class Type>
void DynQueue<Type>::createClone(QueueNode<Type> * &f, QueueNode<Type> * &r)const{

    
    if (!isEmpty()) {
        QueueNode<Type> * frontClone = new QueueNode<Type>(front->value, NULL);
        QueueNode<Type> * temp = frontClone;
        QueueNode<Type> * temp2 = front->next;
        f=frontClone;
        r=frontClone;
        
        while (temp2 != NULL) {
       
            temp->next = new QueueNode<Type>(temp2->value, NULL);
            r=temp->next;
            temp=temp->next;
            temp2=temp2->next;
            
        }
        
    }else{
        f=NULL;
        r=NULL;
    }

}

//************************************************
// Destructor       *
//************************************************
template <class Type>
DynQueue<Type>::~DynQueue(){
    if(front != NULL){
    QueueNode<Type> * ptr = front;
        while(ptr != NULL){
            ptr = ptr->next;
            delete front;
            front=ptr;
            
        }
    }
}

//************************************************
// Extraction operator overloading for ifstream objects    *
//************************************************
template <class Type>
ifstream & operator >> (ifstream & is, DynQueue<Type> & dq){
    string line;
    Type val;
    while(getline(is, line)){
        if(line!="\r" && line!=""){
            istringstream ss(line);
            ss >> val;
            dq.enqueue(val);
        }
    }
    return is;
};

//************************************************
// Insertion operator overloading        *
//************************************************
template <class T>
ostream & operator << (ostream & os, const DynQueue<T> & dq){
    QueueNode<T> * fprint= dq.front;
    while (fprint!=nullptr) {
        
        os << fprint->value << " ";
        fprint=fprint->next;
    }
    
    return os;
};


//************************************************
// "=" operator overloading        *
//************************************************
template <class Type>
const DynQueue<Type> & DynQueue<Type>::operator = (const DynQueue<Type> & rhs){
    if(this != &rhs){
        this->clear();
        QueueNode<Type>  *fr,  *re;
        rhs.createClone(fr, re);
        front=fr;
        rear=re;
        
    }

    return *this;
};


//************************************************
// "+=" operator overloading        *
//************************************************
template <class Type>
void DynQueue<Type>::operator += (Type t){
    this->enqueue(t);
    
};

//************************************************
// "*=" operator overloading        *
//************************************************
template <class Type>
void DynQueue<Type>::operator *= (int t2){
    QueueNode<Type> *ptr = front;

        while(ptr!=nullptr){
            for (int i=0; i<t2-1; i++) {
                ptr->value +=  ptr->value;
            }
           
            ptr=ptr->next;
        }
   

};

//************************************************
// "+" operator overloading        *
//************************************************
template <class T2>
DynQueue<T2> operator + (const DynQueue<T2> & lhs2, const DynQueue<T2> & rhs2){
    QueueNode<T2> *ptr = lhs2.front;
    QueueNode<T2> *ptr2 = rhs2.front;
    DynQueue<T2> result;
    while (ptr!=NULL) {
        T2 sum = ptr->value + ptr2->value;
        result.enqueue(sum);
        ptr=ptr->next;
        ptr2=ptr2->next;
    }
    return result;
    
    
};

//********************************************
// Function enqueue inserts the value in num *
// at the rear of the queue.                 *
//********************************************
template <class Type>
void DynQueue<Type>::enqueue(Type num){
    if (isEmpty()){   //if the queue is empty
        //make it the first element
        front = new QueueNode<Type>(num);
        rear = front;
    }
    else{  //if the queue is not empty
        //add it after rear
        rear->next = new QueueNode<Type>(num);
        rear = rear->next;
    }
    #ifdef _DEBUG
        cout << num << " enqueued\n";
    #endif
}

//**********************************************
// Function dequeue removes the value at the   *
// front of the queue, and copies it into num. *
//**********************************************
template <class Type>
void DynQueue<Type>::dequeue(Type &num){
    QueueNode<Type> *temp;
    if (isEmpty()){
        cout << "Attempting to dequeue on empty queue, exiting program...\n";
        exit(1);
    }
    else{ //if the queue is not empty
        //return front's value, advance front and delete old front
        num = front->value;
        temp = front;
        front = front->next;
        delete temp;
    }
}

//*********************************************
// Function isEmpty returns true if the queue *
// is empty, and false otherwise.             *
//*********************************************
template <class Type>
bool DynQueue<Type>::isEmpty(void) const{
    if (front == NULL)
        return true;
    else
        return false;
}

//********************************************
// Function clear dequeues all the elements  *
// in the queue.                             *
//********************************************
template <class Type>
void DynQueue<Type>::clear(void){
     Type value;   // Dummy variable for dequeue

    while(!isEmpty())
        dequeue(value); //delete all elements
    #ifdef _DEBUG
        cout << "queue cleared\n";
    #endif
}



#endif
