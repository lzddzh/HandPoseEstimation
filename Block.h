#include "common.h"

class Block
{
public:
    int count[9];
    Block()
    {
        for(int i=0;i<9;i++)
        {
            count[i]=0;
        }
    }

    int find_block_direction()
    {
        int result=0;
        int max_count=count[0];
        for(int i=1;i<9;i++)
        {
            if(count[i]>max_count)
            {
                result=i;
                max_count=count[i];
            }
        }
        return result;
    }
};


