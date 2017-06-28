#include "debugger.h"


class FDetect
{
public:
	FDetect();
    FDetect(string path, string pic_path);

	void detect_save();
    vector<Rect> detectFaces(Mat img_gray);
    void saveFaces(Mat img,vector<Rect> rec, int idx);


	vector<string> fname_vec;
    vector<string> face_name_vec;
};
