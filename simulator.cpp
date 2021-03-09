# include <iostream>
# include <bits/stdc++.h>
# include <fstream>
# include <sstream> 
using namespace std;

int R[32];  // Global array of Registers


void PerformAdd (char a,char b, char c,int x,int y, int z)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if(c == 't')
        r = R[z];
    else if (c == 'r')
        r = R[10+z];

    p = q+r;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;

}

void PerformSub (char a,char b,char c, int x,int y,int z)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if(c == 't')
        r = R[z];
    else if (c == 'r')
        r = R[10+z];

    p = q-r;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;

}


bool PerformEqual (char a,char b, int x,int y)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    if( p == q)
        return true;
    else
        return false;
}


void PerformAddI (char a,char b, int x,int y, int z)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    p = q+z;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;
}

void PerformSubI (char a,char b, int x,int y, int z)
{
    int p,q,r;
    if(a == 't')
        p = R[x];
    else if (a == 'r')
        p = R[10+x];

    if(b == 't')
        q = R[y];
    else if (b == 'r')
        q = R[10+y];

    p = q-z;
    if(a == 't')
         R[x] = p;
    else if (a == 'r')
         R[10+x] = p;
}


bool add_sub_Check   ( string sentence, string word ) 
{
    int x,y,z;
    char a,b,c;

    // To break the sentence in words 
    stringstream s(sentence); 
  
    // To temporarily store each individual word 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            c = temp[p++];        // storing variable name of 3rd register
            z = temp[p++] - 48;   // storing value name of 3rd register

            if  (word == "add")
                PerformAdd(a,b,c,x,y,z);

            else if (word == "sub")
                PerformSub(a,b,c,x,y,z);

            return true; 
        }
           
            
    } 
    return false; 
}

bool addi_subi_Check ( string sentence, string word )
{
    int x,y,z;
    char a,b;

    // To break the sentence in words 
    stringstream s(sentence); 
  
    // To temporarily store each individual word 
    string temp; 
  
    while (s >> temp) 
    {   // Comparing the current word with the word to be searched 
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register


            s >> temp;  // temp[0] = '$'  
                      

            string str_temp = temp; 
            stringstream geek(str_temp); // object from the class stringstream 
  
            // The object has integer in string form and now, stream it to the integer int_temp 
            int int_temp = 0; 
            geek >> int_temp; 
  
            // Now the variable int_temp holds the integer value of the string        
              
            z = int_temp;   // storing integer value of 3rd register

            if  (word == "addi")
                PerformAddI(a,b,x,y,z);

            else if (word == "subi")
                PerformSubI(a,b,x,y,z);

            return true; 
        }
           
            
    } 
    return false; 
}


string jump_Check ( string sentence, string word )
{ 
    stringstream s(sentence); 
    string temp; 
    string example = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // This temp will store label i.e, where to jump
            //temp = temp+":";
            return temp;
        }
        else
            return example;
    } 
}

string bne_Check  ( string sentence, string word )
{
    int x,y;
    char a,b;

    stringstream s(sentence); 
    string temp; 
    string wrong = "fault";
  
    while (s >> temp) 
    {  
        if (temp.compare(word) == 0) 
        {  
            s >> temp;  // temp[0] = '$'
            int p=1;           
              
            a = temp[p++];        // storing variable name of 1st register
            x = temp[p++] - 48;   // storing value name of 1st register


            s >> temp;  // temp[0] = '$'  
            p=1;           
              
            b = temp[p++];        // storing variable name of 2nd register
            y = temp[p++] - 48;   // storing value name of 2nd register

            s >> temp; // This temp will be having our label 

            if( PerformEqual(a,b,x,y) == true )
                return wrong; // So that control just goes to next line
            else
                return temp;   
            
        }
        else
            return wrong;
    } 
}


string search_label(string str) 
{ 
    string word = ""; 
    for (auto x : str)  
    { 
        if (x == ':') 
        {   
            //cout << "*";
            return word;
            //cout << word << endl; 
            word = ""; 
        } 

        else if(x == ' ' || x == '\n') 
            word = ""; 

        else
             word = word + x; 
    } 
} 

void PrintAllRegisters(int R[] , int k)
{
    for(int i=0;i<k;i++)
        cout << " R[" << i << "] = " << R[i] << endl;
}


int main()
{
    int n = 10;      // file size
    string arr[n];   // Each one stores one line of file

    int i=0;
    for(int i=0;i<32;i++)
       R[i] = i;

    ////***********************************************////

    std :: string line;
    ifstream file("assembly.txt");

    if(!file.is_open())
        cout << "error" << endl;

    while (getline(file,line))
        arr[i++] = line;

    map <string,int> search;

    for(int i=0 ;i<n ; i++)
    {
        string temp  = arr[i];
        string label = search_label(temp);

        //cout << label;        
        //label = label + ":";  // Shoud not do this ,we need without colon only

        search.insert({label,i});
        //search.insert({"while",4});
    } 

    //   If input file is "I am a human:"
    //    search ["human"];   // Prints in which line it is there

    //****************************************************//


    for (int k=0;k<n;k++)
    {  
        add_sub_Check  ( arr[k], "add" ) ;
        add_sub_Check  ( arr[k], "sub" ) ;
            
        addi_subi_Check( arr[k], "addi" ) ;
        addi_subi_Check( arr[k], "subi" ) ;

       
        string jumpLabel = jump_Check(arr[k], "j");
        if( arr[k]!= ""  &&  jump_Check(arr[k], "j") != "fault" )
        {
            // jumpLabel = jumpLabel +":";     //should not do
            // cout << jumpLabel;
            // cout << search ["while"];       //Shows where while: is located
            
            // cout <<search [jumpLabel];
            k = search [jumpLabel];   //jump_Index;

            continue;
        }

        string  bne_Label =  bne_Check (arr[k], "bne");
        if( arr[k]!= ""  &&  bne_Check(arr[k], "bne") != "fault" )
        {
            // # Similar to above Jump case
            k = search [bne_Label];   //jump_Index;

            continue;
        }

    }

    PrintAllRegisters(R, 32);

    file.close();
    return 0;
}



/* Test file


add $t0, $t1, $t2 
add $t3, $t4, $t5 
j while 
loop: 
    addi $r3, $r3, 1 
while: 
   add $r0, $r1, $r2 
bne $r3, $r4, loop 
   addi $r9, $r9, 10 

*/