#include <iostream>
#include <vector>
#include <map>

int main() {
    // vector 
//     std::vector<int> vec ;

//     vec.push_back(1);
//     vec.push_back(2);
//     vec.push_back(3);
//     vec.push_back(4);
//     vec.push_back(5);


//     // Iterator to the beginning of the vector
//     std::vector<int>::iterator it = vec.begin ();

//     // Traverse the vector and print its elements
//  while (it != vec.end())
//  {
//     std::cout << *it << std::endl;
//     it++;
//  }
//     return 0;

// map
   std::map<std::string, int> myMap ; //= {{"apple", 2}, {"banana", 3}, {"orange", 4}};

    // Using iterator to print map elements
    myMap.insert (std::make_pair ("apple",1));
    myMap.insert (std::make_pair ("bannana",2));
    myMap.insert (std::make_pair ("strawberry",3));
    myMap.insert (std::make_pair ("watermellon",4));
    myMap.insert (std::make_pair ("orange",5));
    std::map<std::string, int>::iterator it;
    for (it = myMap.begin(); it != myMap.end(); it++)
    {
        std::cout << it->first << " : " << it->second << std::endl;
    }
    
    return 0;

}