#ifndef ITEM_HPP
#define ITEM_HPP

struct Item
{
   Item(int con, int vow){
       consonants = con;
       vowels = vow;
       count = 1;
   }

   ~Item(){}

   int consonants; // number of consonants in key
   int vowels;     // number of vowels in key
   int count;      // number of identical keys stored at this location
};

#endif // ITEM_HPP
