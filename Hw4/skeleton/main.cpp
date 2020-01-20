#include <iostream>
#include "ThreadSafeList.h"

class Student{
public:
    friend ostream &operator<<(ostream &os, const Student &student) {
        os << "id: " << student.id << " grades: " << student.grades<< std::endl;
        return os;
    }

    Student(const unsigned int id=0, const int grades=0):id(id),grades(grades){}
    unsigned int id;
    int grades;

    bool operator==(const Student &rhs) const {
        return grades == rhs.grades;
    }

    bool operator!=(const Student &rhs) const {
        return !(rhs == *this);
    }

    bool operator<(const Student &rhs) const {
        return grades < rhs.grades;
    }

    bool operator>(const Student &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Student &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Student &rhs) const {
        return !(*this < rhs);
    }
};
int main() {

    std::cout << "Hello, World!" << std::endl;
    List<Student> list;
    Student* temp = new Student(125,86);
    list.insert(*temp);
    cout<<"size: "<<list.getSize()<<std::endl;
    temp = new Student(125,89);
    list.insert(*temp);
    temp = new Student(12145,-86);
    list.insert(*temp);
    cout<<"size: "<<list.getSize()<<std::endl;
    temp = new Student(1215,86);
    list.insert(*temp);
    cout<<"size: "<<list.getSize()<<std::endl;
    list.print();
    temp = new Student(121435,856);
    list.insert(*temp);
    temp = new Student(122315,861);
    list.insert(*temp);
    temp = new Student(1223415,826);
    list.insert(*temp);
    temp = new Student(1213245,486);
    list.insert(*temp);
    list.print();
    cout<<"size: "<<list.getSize()<<std::endl;
    temp = new Student(1213245,486);
    list.remove(*temp);
    temp = new Student(1213245,861);
    list.remove(*temp);
    temp = new Student(1213245,86);
    list.remove(*temp);
    cout<<"size: "<<list.getSize()<<std::endl;
    temp = new Student(1213245,486);
    list.remove(*temp);
    cout<<"size: "<<list.getSize()<<std::endl;
    list.print();
    return 0;
}