#include <iostream>
#include <string>
#include<exception>
#include<vector>
#include "FamilyTree.hpp"
using family::Tree;
using std::string;
using std::vector;
using namespace std;

namespace family
{
    int fOrM = 0;


    void setFM(int x)
    {
        if (fOrM == 0)
            fOrM = x;
    }


    Tree::Tree(string s)
    {

        this->data = s;
        this->father = nullptr;
        this->mother = nullptr;
    }

    void Tree::Del(Tree * node)
    {
            if (node == nullptr) return;


            /* first delete both subtrees */
            Del(node->father);
            Del(node->mother);


            /* then delete the node */
            free(node);


    }

    Tree& Tree::addFather(string child, string father)
    {
        if(this == nullptr)
            throw std::out_of_range("There is no child that his name is "+child);

        if(this->data != child)
        {
            Tree * found = this->findTree(child);
            if(found == nullptr)
                throw std::out_of_range("There is no child that his name is "+child);

            if(found->father == nullptr)
            {
                found->father = new Tree(father);
                return *this;
            }
    
        }
        else
        {
            if(this->father == nullptr)
            {
                this->father = new Tree(father);
                return *this;
            }
                
            else
                throw std::logic_error("There is father to child " + child + " alredy");
        }
    }

    Tree& Tree::addMother(string child, string mother)
    {
        if(this == nullptr)
            throw std::out_of_range("There is no child that his name is "+child);

        if(this->data != child)
        {
            Tree * found = this->findTree(child);
            if(found == nullptr)
                throw std::invalid_argument("There is no child that his name is "+child);

            if(found->mother == nullptr)
            {
                found->mother = new Tree(mother);
                return *this;
            }
    
        }
        else
        {
            if(this->mother == nullptr)
            {
                this->mother = new Tree(mother);
                return *this;
            }
                
            else
                throw std::logic_error("There is father to child " + child + " alredy");
        }
    }

    string Tree::relation(string s)
    {
        if(this == nullptr)
            throw std::invalid_argument("null ptr exception");

        int relative = this->relativeInt(s,0);
        if(relative < 0)
            return "unrelated";


        switch (relative)
        {
            case 0:
                return "me";
            
            case 1:
            {
                if(fOrM == 2)
                    return "mother";
                return "father";
            }
            
            case 2:
            {
                if(fOrM == 2)
                    return "grandmother";
                return "grandfather";
            }
            
            default:
            {
                int grandNum = relative - 2;
                string value = "";

                for(int i =0; i<grandNum;i++)
                    value += "grand-";

                if(fOrM == 2)
                    value +="grandmother";
                else
                    value += "grandfather";
                return value;
            }
        }

        return "eror";

    }

    string Tree::find(string s)
    {
        if (s == "father")
        {
            if (this->father == nullptr)
                throw std::invalid_argument("father do not exist");
            else
            {
                return this->father->data;
            }
        }

        else if (s == "mother")
        {
            if (this->mother == nullptr)
                throw std::invalid_argument("mother do not exist");
            else
            {
                return this->mother->data;
            }
        }

        else if (s == "grandmother")
        {
            if (this->mother == nullptr && this->father == nullptr)
                throw std::invalid_argument("grandfather do not exist");
            if (this->mother != nullptr)
            {
                if (this->mother->mother != nullptr)
                    return this->mother->mother->data;
            }
            if (this->father != nullptr)
            {
                if (this->father->mother != nullptr)
                    return this->father->mother->data;
            }

            throw std::invalid_argument("grandfather do not exist");
        }

        else if (s == "grandfather")
        {
            if(this->mother == nullptr && this->father == nullptr)
                throw std::invalid_argument("grandfather do not exist");
            if (this->mother != nullptr)
            {
                if (this->mother->father != nullptr)
                    return this->mother->father->data;
            }
            if (this->father != nullptr)
            {
                if (this->father->father != nullptr)
                    return this->father->father->data;
            }

            throw std::invalid_argument("grandfather do not exist");
        }

        else if (s.substr(0, 5) == "great")
        {
            bool isFather = true;
            bool isMother = true;
            string father = ""; string mother = "";
            try {
                father = this->father->find(s.substr(6));
            }
            catch (const std::invalid_argument & e)
            {
                isFather = false;
            }
            if (father != "")
                return father;
            try
            {
                mother = this->mother->find(s.substr(6));
            }
            catch (const std::invalid_argument & e)
            {
                isMother = false;
            }

            if (isMother == isFather && isFather== false)
                throw invalid_argument(s + " do not recognized");
            else if (isFather)
            {
                return father;
            }
            return mother;
        }

        else
        {
            throw std::invalid_argument(s + " do not recognize");
        }
    }

    void Tree::display()
    {
        if(this->father == nullptr && this->mother == nullptr)
        {
            cout<< this->data + " = a leaf" <<endl;
        }
        else
        {
            cout<<this->data +" = ";

            if(this->father!=nullptr && this->mother != nullptr)
            {
                cout<<father->data + "(F) + (M)"+ this->mother->data <<endl;
                this->father->display();
                this->mother->display();
            }
            else if(this-> father == nullptr)
            {
                 cout<<this->mother->data + "(M)";
                 cout<<"\n";
                 this->mother->display();
            }
            else
            {
                cout<<this->father->data + "(F)";
                cout<<"\n";
                this->father->display();
            }
            
        }
        
    }

    void Tree::remove(string s)
    {
        Tree* root = removeHelp(s, 0, nullptr);
        if (root == nullptr)
            throw invalid_argument(s + " not exist");
        
        Del(root);

    }

    Tree* Tree::removeHelp(string s, int mf, Tree* pre)
    {
        Tree* root;

        if (this == nullptr)
            root = nullptr;
        else
        {

            if (this->data == s)
            {
                root = this;
                switch (mf)
                {
                case 0:
                    break;
                case 1:
                    pre->father = nullptr;
                    break;
                case 2:
                    pre->mother = nullptr;
                    break;
                }
                return this;


            }
            else
            {
                Tree* left = this->father->removeHelp(s, 1, this);
                Tree* right = this->mother->removeHelp(s, 2, this);

                if (left != nullptr)
                    return left;

                else if (right != nullptr)
                    return right;
                else
                {
                    return nullptr;
                }
            }
        }
        
        return nullptr;
    }
    Tree* Tree::findTree(string name)
    {
        if (this == nullptr)
            return nullptr;
        else
        {
            if (this->data == name)
                return this;
            else
            {
                Tree* left = this->father->findTree(name);
                Tree* right = this->mother->findTree(name);

                if (left != nullptr)
                    return left;
                else if (right != nullptr)
                    return right;
                else
                {
                    return nullptr;
                }

            }


        }

    }


   

    int Tree::relativeInt(string name, int fm)
    {
        if(this == nullptr)
            throw out_of_range("name not exist: "+name );
        
        else
        {
            if (this->data == name)
            {
                fOrM = fm;
                return 0;
            }
            else
            {
                if(this->mother == nullptr && this->father == nullptr)
                    return -999;
                else if(this->mother != nullptr && this->father != nullptr)
                {
                    int x =0;
                    x = this->father->relativeInt(name, 1);
                    int father = x == -999 ? -999 : 1 + x;
          
                    x = this->mother->relativeInt(name, 2);
                    int mother = x == -999 ? -999 : 1 + x;


                    if(father == -999 )
                    {
                        return mother;
                    }

                    return father;
                }
                else if(this->mother != nullptr)
                {
                     int x =0;
                    x = this->mother->relativeInt(name,2);
                    int mother = x == -999 ? -999 : 1 + x;

                     if(mother > 0 )
                    {
                        return mother;
                    }
                    else
                    {
                         return -999;
                    }
                    
                }
                else
                {
                      int x =0;
                    x = this->father->relativeInt(name, 1);
                    int father = x == -999 ? -999 : 1 +x;

                     if(father > 0 )
                    {
                        return father;
                    }
                    else
                    {
                        return -999;
                    }
                    
                }
                
            }
            
        }

      
        
    }



}
