/**
 Purpose: this is typical music library programe(adding music to list, deleting music, printing music etc.)
 Author: Eren Bezirganci
 Date: 30/07/2021
 
 */

#include <string>
#include <iostream>
#include "strutils.h"
#include <fstream>
#include <sstream>
using namespace std;
#define debug 0

struct Song{
    string name;
    string singer;
    string genre;
    int listener;
    int duration;
    Song *prev;
    Song *next;
    
    Song(){}
    Song(string n, string s, string g,  int l, int d, Song* prv, Song* nxt){
        name = n;
        singer = s;
        genre=g;
        listener= l;
        duration= d;
        prev=prv;
        next=nxt;
    }

};

/*
 this function is for updating the maximum value after changes
 ********
 
Input:
 Song max = tail of list
 
 **/
void updateMax(Song * &max){
    Song* ptr = max;
    

    while (ptr->next != max) {
        ptr= ptr->next;
        if (ptr->listener >= max->listener) {
            if (ptr->listener == max->listener) {
                if (ptr->name < max->name) {
                    max = ptr;
                }
            }else{
                max = ptr;
            }
            
        }
    }
}



/*
 this function adding song to doubly circular list. For keeping list sorted, it added higher listener count. if it is same, it checks name
 
 Input:
 max: tail of list
 name: name of song
 singer: singer name of the song
 genre: genre of the song
 listener: count of listener
 duration: duration of the song
 
 **/
void addToList(Song * &max, string name, string singer, string genre, int listener, int duration){
  
    if( max == nullptr){// in case of there is no element in list
        Song *song = new Song(name, singer, genre, listener, duration, nullptr, nullptr);
        song->next=song;
        song->prev=song;
        max=song;
    }else{
        #if debug == 1
            cout << "0" << endl;
        #endif
        Song* ptr = max;
    
            if (listener < ptr->prev->listener) {
                #if debug == 1
                    cout <<"5"<<endl;
                #endif
             
                Song *song = new Song(name, singer, genre, listener, duration, ptr->prev, ptr);
                ptr->prev->next = song;
                ptr->prev=song;
            }else{
                while (listener < ptr->listener) {
                        ptr=ptr->next;
                    }
                #if debug == 1
                    cout << ptr->name <<endl;
                #endif
                
                if (listener == ptr->listener) {
                    #if debug == 1
                        cout << "1" << endl;
                    #endif
                    
                    if (name < ptr->name) {
                        #if debug == 1
                            cout <<"2"<<endl;
                        #endif
                        Song *song = new Song(name, singer, genre, listener, duration, ptr->prev, ptr);
                        ptr->prev->next = song;
                        ptr->prev=song;
                      
                    }else{
                        #if debug == 1
                            cout <<"3"<<endl;
                        #endif
                        Song *song = new Song(name, singer, genre, listener, duration, ptr, ptr->next);
                        ptr->next->prev = song;
                        ptr->next=song;
                        
                    }
                }else{
                    #if debug == 1
                        cout <<"4"<<endl;
                    #endif

                    Song *song = new Song(name, singer, genre, listener, duration, ptr->prev, ptr);
                    ptr->prev->next = song;
                    ptr->prev=song;
                    
                    
                }
            }
        

                #if debug == 1
                    cout << max->name;
                #endif
    }
   
}

/* This function checks whether given song name is in the list or not.
 
 
 *****
 
 Input
 max= tail of the song
 name= name of the song which is wanted to be searched in list
 
 *****
 
 Return
 If song is in the list it returns this song, If it is not in the list, it returns nullptr
 
 **/
Song* isAdded(Song * &max, string name){
    Song *null = nullptr;
    if(max != nullptr){
        Song *ptr = max;
        
        
        while (ptr->next != max) {
            ptr=ptr->next;
            if(ptr->name ==  name){
                return ptr;
                
            }
        }
        
        if (max->name == name) {
            return max;
        }
        
        
    }
    return null;
    
}

/*
 This function prints single song with their all informations
 
 ********
 Input
 song: song which wanted to be printed
 */
void printSong(Song* song){
    cout<< "Song Name: " << song->name << endl;
    cout << "Singer Name: " << song->singer << endl;
    cout << "Genre: " << song->genre << endl;
    cout << "Duration: " << song->duration << endl;
    cout << "Listener Count: " << song->listener << endl  <<"---"<<endl;
}


/*
 This funciton prints all songs with their all information in the list.
 
 ********
 Input
 max: tail of the song
 type: print type("normal" or "reverse" or "debug")
 
 **/
void printList(Song * max, string type){
    Song * ptr = max;
    if (max==nullptr) {
        cout << "The song list is empty." <<endl<< "---" << endl;
    }else{
        if (type == "debug") {
            cout<< max->name << " /// " << max->listener  << endl;
            while(ptr->next != max){
                ptr=ptr->next;
                cout << ptr->name << " /// "<<ptr->listener << endl;
            }
        }else if(type== "normal"){
   
            printSong(max);
            while(ptr->next != max){
                ptr=ptr->next;
                printSong(ptr);
            }

        }else if (type == "reverse"){

            while(ptr->prev != max){
                ptr=ptr->prev;
                printSong(ptr);

            }
            printSong(max);

        }
    }
}
  /**
   This functions print songs with same genre or songs with same singer
   
   *****************
   Input
   max: tail of the list
   type: print type(according to "genre" or "singer")
   value:  genre name or signer name
   
   */
void printSpecial(Song * max, string type, string value){
    if (type=="genre") {
        cout << "List of All the Songs from Genre " << value << endl<<"---"<<endl;
        
        
        
        Song * ptr = max;
        if (max->genre == value) {
            cout << "- "<<max->name << " from " << max->singer <<endl;
            
        }

        while(ptr->next != max){
            ptr=ptr->next;
            if (ptr->genre == value) {
                cout <<"- " << ptr->name << " from " << ptr->singer <<endl;
                
            }


        }
        cout <<"---" << endl;
        
        
    }else if(type=="singer"){
        cout << "List of All the Songs from Singer " << value <<endl<<"---"<<endl;
        
        
        
        Song * ptr = max;
        if (max->singer == value) {
            cout << "- "<< max->name <<endl;
            
        }

        while(ptr->next != max){
            ptr=ptr->next;
            if (ptr->singer== value) {
                cout <<"- " <<ptr->name  <<endl;
                
            }


        }
        cout <<"---" << endl;
   
        
    }
    
    
}

/**
 This functions gets choice input from user and check the validation of input
 
 ***********
 validation: string which includes valid numbers ("1-2-3-4-5-6-7-8")
 choice: input which is user entered
 */
void getChoice(string validation, string &choice){
    cout << "Enter your choice: " ;
    getline(cin, choice);
    cout <<  "---" <<endl;
    
    while(validation.find(choice) == string::npos){
        
        cout <<  "Not a valid option."  << endl <<"---" <<endl;
        cout <<  "Enter your choice: " ;
        getline(cin, choice);
        cout << "---" << endl;
    }
    
}

/**
 This function uppercases the string inputs which is entered by user and it make only one space between entered inputs
 
 *********
 Input
 line: line which is came from getline()
 formatted: formatted version of each word
 txt: after process(adding each formatted to the txt), it is last version of line
 */
void formatInput(string &line, string &formatted, string &txt){
    ToUpper(line);
    istringstream ss(line);
    int count=0;
    while(ss >> txt){
 
        if (count!=0) {
            formatted = formatted + " " +txt ;
        }else{
            formatted = formatted + txt;
        }
        count ++;
    }
    
}

/**
 this functions deletes single song
 
 **********
 Input
 song: song which is wanted to be deleted
 max: tail of the list
 */
void Delete(Song *song, Song * &max){
    if(max==song && song->next != song ){
        max=max->next;
        song->prev->next = song->next;
        song->next->prev = song->prev;
        delete song;
        
    }else if (song->next != song ) {
        song->prev->next = song->next;
        song->next->prev = song->prev;
        delete song;
    }else{
        delete song;
        max=nullptr;
    }

}

/**
 This function deletes all songs in list
 
 **********
 max: tail of the list
 */
void deleteList(Song * &max){

    Song* temp,*temp2;
    temp2= max->next;
    while (temp2 != max) {
        temp = temp2->next;
        delete temp2;
        temp2 = temp;
    }
    delete max;
    max=nullptr;


}



int main() {
    cout << "This program helps to create a music list and suggest you songs that you might like!"   << endl << "---" << endl;
    cout << "MENU"<< endl;
    cout << "1. Add a song to the list" << endl;
    cout << "2. Delete a song from the list" << endl;
    cout << "3. Print the song list" << endl;
    cout << "4. Print the song list in reverse order" << endl;
    cout << "5. Show total duration of the list" << endl;
    cout << "6. Print songs from the same genre" << endl;
    cout << "7. Print songs from the same singer" << endl;
    cout << "8. Exit" <<endl;
    cout <<"---"<<endl;
    
    string choice; // number between 1-8, it is taken from user
    string numbers = "1-2-3-4-5-6-7-8";
    string txt;
    getChoice(numbers, choice);
    
    Song* max = nullptr;
    
    while(choice != "8"){
        if (choice=="1") {
            
            
            
            string songName;
            string finalName;//formatted song ame
            
            cout << "Enter the name of the song: ";
            getline(cin, songName);
            formatInput(songName, finalName , txt);
   
            
            if(isAdded(max, finalName) == nullptr){
              
                int listener;
                int duration;
                string slistener;
                string sduration;

                
                string singerLine;
                txt = "";
                string singer;//formatted singer name
                cout << "Enter singer name of the song: ";
                getline(cin, singerLine);
                formatInput(singerLine, singer, txt);
                

                string genreLine;
                txt = "";
                string genre;//formatted genre
                cout << "Enter genre of the song: ";
                getline(cin, genreLine);
                formatInput(genreLine, genre, txt);

                cout <<"Enter the listener count of the song: ";
                getline(cin,slistener);
                listener = atoi(slistener);//listener count
                
                cout << "Enter the duration of the song: ";
                getline(cin, sduration);
                duration = atoi(sduration);

                cout << "---"<< endl;
                addToList(max, finalName, singer, genre , listener , duration);
                cout << "The song " << finalName << " from "<< singer <<" is added to the song list."  << endl;
                cout <<  "---" << endl;
            }else{
                cout << "---"<< endl;
                cout << "The song " << finalName << " is already in the song list." << endl;
                cout <<  "---" << endl;
            }
            if(max!=nullptr){
                updateMax(max);
            }
            
#if debug == 1
            printList(max, "debug");
#endif


            
        }else if(choice == "2"){
            if(max==nullptr){
                cout << "The song list is empty." <<endl <<"---"<<endl;
            }else{
              
                
                string songLine;
                txt = "";
                string songName;//formatted song name
                cout << "Enter the name of the song: " ;
                getline(cin, songLine);
                formatInput(songLine, songName, txt);
                
                Song *song = isAdded(max, songName);
                if(song != nullptr){
                    Delete(song,max);
                    cout <<"---" <<endl;
                    cout << "The song " << songName << " is deleted from the list."<< endl;
                    cout << "---" << endl;
            
                }else{
                    cout << "The song " << song << " could not be found in the list."<<endl;
                }
           
                
                
            }
            
        }else if(choice == "3"){
            printList(max,"normal");
        }else if(choice == "4"){
            printList(max,"reverse");
            
        }else if(choice == "5"){
            if (max != nullptr) {
                Song *ptr = max;
                int count = ptr->duration;
                while (ptr->next != max) {
                    count += ptr->duration;
                    ptr=ptr->next;
                }
                
                cout << "Enjoy your next " << count << " minutes with the songs in the list." << endl;
                cout << "---" << endl;
                
            }else{
                cout << "The song list is empty."<<endl << "---" << endl;
            }

            
        }else if(choice == "6"){
            if(max != nullptr){
                string genreLine;
                txt = "";
                string genre;
                cout << "Please enter the genre of the songs you want to list: " ;
                getline(cin, genreLine);
                formatInput(genreLine, genre, txt);
                
                cout <<"---"<<endl;
                
                printSpecial(max, "genre", genre);
                
  
                
            }else{
                
                cout << "The song list is empty."<<endl<< "---" << endl;
            }
            
        }else if(choice == "7"){
            
            if(max != nullptr){
                string singerLine;
                txt = "";
                string singer;
                cout << "Please enter the singer name of the songs you want to list: " ;
                getline(cin, singerLine);
                formatInput(singerLine, singer, txt);
                cout <<"---"<<endl;
                printSpecial(max, "singer", singer);
                
                
                
            }else{
                
                cout << "The song list is empty."<<endl<< "---" << endl;
            }
            
        }
        
        getChoice(numbers, choice);
    }
    
    cout << "Deleting song list..."<<endl;
    if(max!=nullptr){
        deleteList(max);
    }

  
    cout << "Exiting the program..." << endl;
    
    return 0;
}
