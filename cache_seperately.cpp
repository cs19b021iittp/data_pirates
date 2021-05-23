# include <iostream>
# include <bits/stdc++.h>
# include <fstream>
# include <sstream> 
using namespace std;

int CacheSize_L1 , BlockSize_L1 , Associativity_L1 ;
int CacheSize_L2 , BlockSize_L2 , Associativity_L2 ;

int Latency_L1 , Latency_L2 , Latency_MM ;

// int Num_Tag_Bits_L1    = 5, Num_Tag_Bits_L2    = 5; 
// we can decide their length but we dont need them

int Num_Set_Bits_L1    , Num_Set_Bits_L2 ;
int Num_Offset_Bits_L1 , Num_Offset_Bits_L2 ;

int Num_BlocksInSet_L1 , Num_BlocksInSet_L2 ;
int Num_SetsInCache_L1 , Num_SetsInCache_L2 ;

int access_L1 = 0, miss_L1 = 0;
int access_L2 = 0, miss_L2 = 0;


int extract_TagValue(int address, int temp)
{
    if(temp == 0)
    {
        for(int i=0 ;  i < Num_Set_Bits_L1 + Num_Offset_Bits_L1 ; i++)
        {
            address = address >> 1;
        }
        return address;
    }    

    if(temp == 1)
    {
        for(int i=0 ;  i < Num_Set_Bits_L2 + Num_Offset_Bits_L2 ; i++)
        {
            address = address >> 1;
        }
        return address;
    }    

}     

int extract_SetValue(int address, int temp)
{
    int actual = address;
    if(temp == 0)
    {
        for(int i=0; i < Num_Set_Bits_L1 + Num_Offset_Bits_L1 ; i++)
            address = address >> 1;

        for(int i=0; i < Num_Set_Bits_L1 + Num_Offset_Bits_L1 ; i++)
            address = address << 1;

        actual = actual - address; // Now we wnt be having any tag

        for(int i=0; i < Num_Offset_Bits_L1 ; i++)
            actual = actual >> 1;  

        return actual;   // We wont be having any offset too
    }    

    if(temp == 1)
    {
        for(int i=0; i < Num_Set_Bits_L2 + Num_Offset_Bits_L2 ; i++)
            address = address >> 1;

        for(int i=0; i < Num_Set_Bits_L2 + Num_Offset_Bits_L2 ; i++)
            address = address << 1;

        actual = actual - address; // Now we wnt be having any tag

        for(int i=0; i < Num_Offset_Bits_L2 ; i++)
            actual = actual >> 1;  

        return actual;   // We wont be having any offset too
    }    

}       


int extract_OffsetValue(int address, int temp)
{
    int actual = address;
    if(temp == 0)
    {
        for(int i=0;i<Num_Offset_Bits_L1;i++)
            address = address >> 1;

        for(int i=0;i<Num_Offset_Bits_L1;i++)
            address = address << 1;

        actual = actual - address;

        return actual;
    }    

    if(temp == 1)
    {
        for(int i=0;i<Num_Offset_Bits_L2;i++)
            address = address >> 1;

        for(int i=0;i<Num_Offset_Bits_L2;i++)
            address = address << 1;

        actual = actual - address;

        return actual;
    }    
}       



// Cache -> Sets -> Blocks

class Block
{
    public:
        int tag;
        //int data[4];
        //bool dirty;
        int lru;

    Block()
    {
        tag = -1;
        //BlockSize = 4;
        //data[4];
        lru = 0;
        // dirty(false),lru(0); 
    }

};

class Set
{
    public:
        Block* blocks;
        //int Num_BlocksInSet_L1 = 1;

    // no need to store set index in hardware   
    void UpdateLRU (int blk_lru, int temp)
    {
        int k;
        if(temp == 0)
          k = Num_BlocksInSet_L1;

        else
          k = Num_BlocksInSet_L2;

        for(int i=0; i< k; i++) 
        {
            if(blocks[i].lru < blk_lru)
            if(blocks[i].tag != -1)
            blocks[i].lru++;
        }
    }

    void Increase_All_LRU (int temp)
    {
        int k;
        if(temp == 0)
          k = Num_BlocksInSet_L1;

        else
          k = Num_BlocksInSet_L2;

        for(int i=0; i< k; i++) 
        {
            if(blocks[i].tag != -1)
                blocks[i].lru++;
        }
    }


    bool find_Block(int address, int temp)
    {
        int TagValue = extract_TagValue(address, temp);
        int k;

        if(temp == 0)
          k = Num_BlocksInSet_L1;

        else
          k = Num_BlocksInSet_L2;

        for(int i=0 ; i< k ; i++)
        {
            if( blocks[i].tag != -1)
            {
                if( TagValue == blocks[i].tag )
                {   
                    UpdateLRU(blocks[i].lru, temp);
                    blocks[i].lru  = 0;
                    return true;
                }
            }
            else
                return false;
        }
        return false;

    }

    void InsertBlock(int address, int temp)
    {
        int k;
        if(temp == 0)
          k = Num_BlocksInSet_L1;

        else
          k = Num_BlocksInSet_L2;
          
        int TagValue = extract_TagValue(address, temp);

        // If set is not totally filled
        for(int i=0 ; i< k ; i++)
        {
            if( blocks[i].tag == -1)
            {
                blocks[i].tag = TagValue;

                // increase all lru
                Increase_All_LRU(temp);

                blocks[i].lru  = 0;

                return;
            }
        }

        // If set is completely filled
        Increase_All_LRU(temp);  // inc all lru
        for(int i=0 ; i< k ; i++)
        {
            /*// Here we dont need to do this because
              // If a block is getting kicked out of L1,it must be there in L2 too at that time
            * if(temp == 0)
              {
                // check if that block to be removed is in L2
                // If yes dont do anything
                // else Insert it in L2

                int SetValue = extract_SetValue(address,1);
                bool present = L2_Cache.sets[SetValue].find_Block(address,1);

                if(present == 0)
                {
                    L2_Cache.sets[SetValue_L2].InsertBlock(address,1);
                }
              }
            */

            if( blocks[i].lru == k )
            {
                // replace the block
                blocks[i].tag = TagValue;
                blocks[i].lru  = 0;
                return;
            }
        }

        //Now search this removed block in L2  if there then leave,else add it there 
    }
};

class Cache
{
    public:
        Set* sets;
        // int Num_SetsInCache;

    // Access the cache
    int access ( int address , int temp )
    {
        int SetValue = extract_SetValue(address,temp);
        bool present = sets[SetValue].find_Block(address,temp);

        // cout << "Hi" << endl;
        
        if(present == true)
            return true;

        else
            return false;
    }    
};

Cache L1_Cache ;
Cache L2_Cache ;


int FOUND_IN_CACHE (int address)
{
    int SetValue_L1 = extract_SetValue(address,0);
    int SetValue_L2 = extract_SetValue(address,1);

    if(L1_Cache.access(address,0) == 1)
    {
        access_L1 ++;
        return 1;
    }
    else
    {
        miss_L1 ++;
        access_L2 ++;

        if(L2_Cache.access(address,1) == 1)
        {
            // insert in L1;
            L1_Cache.sets[SetValue_L1].InsertBlock(address,0);

            return 2;
        }
           
        else
        {
            miss_L2 ++;

            // insert in L1,L2
            L1_Cache.sets[SetValue_L1].InsertBlock(address,0);
            L2_Cache.sets[SetValue_L2].InsertBlock(address,1);
            return 3;
        }   
    }    
}


int main()
{
    // take inputs
    cout << " Enter CacheSize_L1 ,BlockSize_L1 ,Associativity_L1 :" ;
    cin >> CacheSize_L1 >> BlockSize_L1 >> Associativity_L1 ;

    cout << " Enter CacheSize_L2 ,BlockSize_L2 ,Associativity_L2 :" ;
    cin >> CacheSize_L2 >> BlockSize_L2 >> Associativity_L2 ;

    cout << " Latency_L1, Latency_L2, Latency_MM ";
    cin >> Latency_L1 >> Latency_L2 >> Latency_MM ;

    // Calculating all required info
    Num_SetsInCache_L1 = (CacheSize_L1)/(BlockSize_L1 * Associativity_L1);
    Num_SetsInCache_L2 = (CacheSize_L2)/(BlockSize_L2 * Associativity_L2);

    Num_BlocksInSet_L1 = Associativity_L1;
    Num_BlocksInSet_L2 = Associativity_L2;


    Num_Offset_Bits_L1 = log2(BlockSize_L1) ;
    Num_Offset_Bits_L2 = log2(BlockSize_L2) ;

    Num_Set_Bits_L1 = log2((CacheSize_L1)/(BlockSize_L1 * Associativity_L1));
    Num_Set_Bits_L2 = log2((CacheSize_L2)/(BlockSize_L2 * Associativity_L2));

    // create sets for each cache
    L1_Cache.sets = new Set[Num_SetsInCache_L1];
    L2_Cache.sets = new Set[Num_SetsInCache_L2];

    // for each sets create blocks 

    for ( int i=0; i < Num_SetsInCache_L1 ; i++ )
        L1_Cache.sets[i].blocks = new Block[Num_BlocksInSet_L1 ];

    for ( int i=0; i < Num_SetsInCache_L2 ; i++ )
        L2_Cache.sets[i].blocks = new Block[ Num_BlocksInSet_L2 ];

    
    // cout << "*";
    // cout << extract_TagValue(76935, 0) << "*" << extract_OffsetValue(76935, 0) << "*";
    // cout << extract_SetValue(76935, 0) << endl;

    cout << FOUND_IN_CACHE(76935) ;
    cout << FOUND_IN_CACHE(88937) ;
    cout << FOUND_IN_CACHE(63731) ;
    cout << FOUND_IN_CACHE(63731) ;

    //cout << L1_Cache.access(82918,0) ;
    
    // cout << "**";




    // Miss rates
    cout << "Cache Miss Rate Of L1: " << (miss_L1)/(access_L1) << endl ; 
    cout << "Cache Miss Rate Of L2: " << (miss_L2)/(access_L2) << endl ;

    return 0;
}
