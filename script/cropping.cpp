#include <stdio.h>
#include "tool.hpp"
#include <map>
#include <climits>
struct vec3f
{
    float val[3];
};
 void cropping ()
{
    using namespace std;
    const char *YourMethodName = "YourMethodName";
    string path = "/Users/mac/Documents/courses/Uncertainty Modelling in AI/Evaluation_Dataset/Test_Dataset/";
    const char *fileinfo = "/Users/mac/Documents/courses/Uncertainty Modelling in AI/Evaluation_Dataset/subject_pose_mappings.csv";
    const int width = 320, height = 240;
    int box_height=90;
    int box_width=60;
    const int img_size = width * height;
    int low_bound=170;          //170 for test   150 for train




    cv::Mat depth(height, width, CV_16SC1), confi(height, width, CV_16SC1);//CV_16SC1
    // go through each depth image
    vector<vector<string> > mat;
    vector<vector<float> > result;
    CSV::read_mat_str(fileinfo, mat);
    for (int i = 0; i < mat.size(); i++)
    {
        cv::Mat D, C;

        // read depth file
        string filedepth = path + mat[i][0] + "/" + mat[i][3];
        cout<<mat[i][0]<<endl;
        ifstream filedepth_(filedepth.c_str());
        Utility_Check(filedepth_);
        filedepth_.read((char*) depth.data, img_size * sizeof(short));
        short value=0;

        depth.convertTo(D, CV_8UC1, 0.25, 0);
        cv::imshow("depth", D);
        cv::waitKey(1);


        for(int i=0;i<height;i++)
        {

            for(int j=0;j<width;j++)
            {
                if (i>low_bound||i<50)
                {
                    depth.at<short>(i,j)=32001;
                    continue;
                }

                value=depth.at<short>(i,j);
                if(value>830)
                {
                    depth.at<short>(i,j)=32001;
                }

            }
        }



        depth.convertTo(D, CV_8UC1, 0.25, 0);
        cv::imshow("reversed", D);
        cv::waitKey(1);



        int validate_pixel=0;
        int best=INT_MAX;
        int best_i=box_height/2;
        int best_j=box_width/2;

        for(int i=box_height/2;i<height-box_height/2;i++)
        {
            for(int j=box_width/2;j<width-box_width/2;j++)
            {
                validate_pixel=0;
                for(int cur_i=i-box_height/2;cur_i<i+box_height/2;cur_i++)
                {
                    for (int cur_j=j-box_width/2;cur_j<j+box_width/2;cur_j++)
                    {
                        validate_pixel+=depth.at<short>(cur_i,cur_j);
                    }
                }
                if (validate_pixel<best)
                {
                    best=validate_pixel;
                    best_i=i;
                    best_j=j;
                }
            }
        }

        cv::Mat box(box_height,box_width,CV_16SC1);

        CvRect mat_rect=cvRect(best_j-box_width/2,best_i-box_height/2,box_width,box_height);

        box=depth(mat_rect);


        box.convertTo(D, CV_8UC1, 0.25, 0);
        cv::imshow("crop", D);
        cv::waitKey(1000);

        ofstream file;
        file.open(filedepth+".cropping");
        for(int i=0;i<box_height;i++)
        {
            for (int j=0;j<box_width;j++)
            {
                file<<box.at<short>(i,j)<<" ";
            }
            file<<endl;

        }
        file.close();

    }

}