#pragma once
#include <iostream>
#include <string>
#include<exception>
#include<vector>
using std::string;
using namespace std;

namespace family
{
    class Tree
    {
        public:
            Tree(string s);
            Tree& addFather(string child, string father);
            Tree& addMother(string s, string mother);
            string relation(string s);
            string find(string s);
            void display();
            void remove(string s);
            void Del(Tree * node);
            Tree* findTree(string name);

        private:
        string data;
        Tree * father;
        Tree * mother;

        
        int relativeInt(string name,int fOrM);
        Tree* removeHelp(string s, int mf,Tree * pre);

    };


}

