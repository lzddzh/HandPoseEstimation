#include <opencv2/opencv.hpp>
#define PI 3.14159265

class Block
{
public :
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

class Cal_Sim
{

public:
    static int cal_angle(float x,float y)
    {
        if(x<1&&x>0||x<0&&x>-1)
        {
            x=0;
        }
        if(y<1&&y>0||y<0&&y>-1)
        {
            y=0;
        }

        if(x==0&&y==0)
        {
            return 8;
        }
        if(x==0)
        {
            if(y>0)
            {
                return 2;
            } else{
                return 6;
            }
        }
        if(y==0)
        {
            if(x>0)
            {
                return 0;
            } else
            {
                return 4;
            }
        }

        float angle=atan(abs(y)/abs(x))*180/PI;
        if (angle!=angle)
        {
            return 8;
        } else
        {
            if(x<0 && y>0)
            {
                angle=180-angle;
            }
            if(x<0 && y<0)
            {
                angle=180+angle;
            }
            if(x>0 && y<0)
            {
                angle=360-angle;
            }
        }
        return int(angle/45);
    }

    static float cal_sim(float a[], float b[]) {
        using namespace cv;
        int scale = 1;
        int delta = 0;
        int ddepth = CV_32F;//CV_16S;
        Mat A, B, A_x, B_x,A_y,B_y;

        A = Mat(90, 60, CV_32F, a);
        B = Mat(90, 60, CV_32F, b);



        Sobel( A, A_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
        Sobel( B, B_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );

        Sobel( A, A_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
        Sobel( B, B_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );

        Block a_image [10][10],b_image[10][10];
        for(int i=0;i<90;i++)
        {
            for(int j=0;j<60;j++)
            {
                float a_x=A_x.at<float>(i,j);
                float a_y=A_y.at<float>(i,j);
                float b_x=B_x.at<float>(i,j);
                float b_y=B_y.at<float>(i,j);

                int a_angle=cal_angle(a_x,a_y);
                int b_angle=cal_angle(b_x,b_y);

                a_image[i/9][j/6].count[a_angle]++;
                b_image[i/9][j/6].count[b_angle]++;

            }
        }

        float similarity=0;
        for(int i=0;i<10;i++)
        {
            for(int j=0;j<10;j++)
            {
                if(a_image[i][j].find_block_direction()==b_image[i][j].find_block_direction())
                {
                    similarity++;
                }
            }
        }
        return similarity/100;
    }

};
