#include "OpenFaceOutput.h"


using namespace std;
using namespace cv;

//Quaternion class functions
Quaternion::Quaternion()
{}
Quaternion::~Quaternion() {}

void Quaternion::normalizeQ()
{
	float normQ = sqrt(w*w + x*x + y*y + z*z);
	w /= normQ;
	x /= normQ;
	y /= normQ;
	z /= normQ;
}

float Quaternion::inner(Quaternion q1, Quaternion q2)
{
	return q1.w*q2.w + q1.x*q2.x + q1.y*q2.y + q1.z*q2.z;
}

string rndup_str(float n, int decimals)
{
	stringstream ss;
	ss << fixed << setprecision(decimals) << n;
	return ss.str();
}


void writeAU(Mat AUCFrame, int current_frame, Mat data)
{

	//Reset matrix with zeros
	AUCFrame = Mat::zeros(SIZE_W_AUC, CV_8UC3);

	if (current_frame >= data.rows)
	{
		return;
	}

	//Declare padding
	int y_ini = 40;
	int y_step = 40;
	int y = y_ini;

	Scalar white = Scalar(255, 255, 255);

	// Put labels
	putText(AUCFrame, "AU01: Inner Brow raiser", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU02: Outer Brow raiser", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU04: Brow lowerer", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU05: Upper lid raiser", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU06: Cheek raiser", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU07: Lid tightener", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU09: Nose wrinkler", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU10: Upper lip raiser", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU12: Lip corner puller", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU14: Dimpler", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU15: Lip corner dep.", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU17: Chin raiser", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU20: Lip stretcher", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU23: Lip tightener", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU25: Lips part", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU26: Jaw drop", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;
	putText(AUCFrame, "AU45: Blink", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, white); y += y_step;

	//Draw filled rectangle according to intensity
	y = y_ini;
	for (int index = 0; index <= (AU45_r - AU01_r); index++)
	{

		//cout << "AU with index " + to_string(index) + " is " + to_string(data.at<float>(current_frame, AU01_r+index)) << endl;
		//Since AU28 has no intensity, this iteration needs to be treated specially
		if (AU01_c + index == AU28_c)
		{
			if (data.at<float>(current_frame, AU45_c) > 0.0f)
			{
				rectangle(AUCFrame, Rect(Point(250, y - 15), Size((int)(100 * data.at<float>(current_frame, AU45_r)*(0.2f)), 20)), Scalar(0, 255, 0), CV_FILLED);
				//cout << 100 * data.at<float>(current_frame, AU01_r)*(0.2f) << endl;
				//cout << data.at<float>(current_frame, AU01_r+index) << endl;
				//cout << "AU with index " + to_string(index) + " is " + to_string(data.at<float>(current_frame, AU45_r)) << endl;
				rectangle(AUCFrame, Rect(Point(250, y - 15), Size(100, 20)), Scalar(0, 255, 0), 2);
			}
			else rectangle(AUCFrame, Rect(Point(250, y - 15), Size(100, 20)), Scalar(0, 0, 255), 2);
			y += y_step;
			continue;
		}

		if (data.at<float>(current_frame, AU01_c + index) > 0.0f)
		{
			rectangle(AUCFrame, Rect(Point(250, y - 15), Size((int)(100 * data.at<float>(current_frame, AU01_r + index)*(0.2f)), 20)), Scalar(0, 255, 0), CV_FILLED);
			//cout << 100 * data.at<float>(current_frame, AU01_r)*(0.2f) << endl;
			//cout << data.at<float>(current_frame, AU01_r+index) << endl;
			rectangle(AUCFrame, Rect(Point(250, y - 15), Size(100, 20)), Scalar(0, 255, 0), 2);
		}
		else rectangle(AUCFrame, Rect(Point(250, y - 15), Size(100, 20)), Scalar(0, 0, 255), 2);
		y += y_step;
	}


}

void writePosOri(Mat PosOriFrame, int current_frame, Mat data)
{
	if (data.empty())
	{
		return;
	}

	//Reset matrix with zeros
	PosOriFrame = Mat::zeros(SIZE_W_POS_ORI, CV_8UC3);

	//Declare padding
	int y_ini = 40;
	int y_step = 40;
	int y = y_ini;
	int x_step = 90;

	// Gaze0 (Left)
	putText(PosOriFrame, "Gaze0 ", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "X: " + rndup_str(data.at<float>(current_frame, GAZE_0_X)), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Y: " + rndup_str(data.at<float>(current_frame, GAZE_0_Y)), Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Z: " + rndup_str(data.at<float>(current_frame, GAZE_0_Z)), Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "X: ", Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Y: ", Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Z: ", Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	y += y_step;

	// Gaze1 (Right)
	putText(PosOriFrame, "Gaze1 ", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "X: " + rndup_str(data.at<float>(current_frame, GAZE_1_X)), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Y: " + rndup_str(data.at<float>(current_frame, GAZE_1_Y)), Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Z: " + rndup_str(data.at<float>(current_frame, GAZE_1_Z)), Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "X: ", Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Y: ", Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Z: ", Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	y += y_step;

	// Head traslation
	putText(PosOriFrame, "HeadT ", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "X: " + rndup_str(data.at<float>(current_frame, POSE_TX), 1), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Y: " + rndup_str(data.at<float>(current_frame, POSE_TY), 1), Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Z: " + rndup_str(data.at<float>(current_frame, POSE_TZ), 1), Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "X: ", Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Y: ", Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Z: ", Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	y += y_step;

	// Head rotation
	putText(PosOriFrame, "HeadR ", Point(10, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "X: " + rndup_str(data.at<float>(current_frame, POSE_RX), 2), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Y: " + rndup_str(data.at<float>(current_frame, POSE_RY), 2), Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "Z: " + rndup_str(data.at<float>(current_frame, POSE_RZ), 2), Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
	putText(PosOriFrame, "X: ", Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Y: ", Point(90 + x_step, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	putText(PosOriFrame, "Z: ", Point(90 + x_step * 2, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	y += y_step;

	// Difference in orientation
	float angdiff = ang2rot(current_frame, data);
	Scalar color;
	if (angdiff >= 0.1)
		color = Scalar(0, 0, 255);
	else
		color = Scalar(0, 255, 0);
	putText(PosOriFrame, "Angle diff: " + rndup_str(angdiff), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;

	float dist = calc_translation(current_frame, data);
	putText(PosOriFrame, "Transl diff: " + rndup_str(dist, 2), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(0, 255, 0));
	y += y_step;
}

void writeDist(Mat frame, int current_frame, Mat data, int index, Mat marks)
{
	if (marks.empty() || data.empty())
		return;

	//Reset matrix with zeros
	frame = Mat::zeros(SIZE_W_POS_ORI, CV_8UC3);

	//Declare padding
	int y_ini = 40;
	int y_step = 40;
	int y = y_ini;
	int x_step = 90;

	Scalar color;

	if (data.at<float>(current_frame, 3) > MAX_SHAPE_STD) color = Scalar(0, 0, 255);
	else color = Scalar(0, 255, 0);
	putText(frame, "Righ dist: " + rndup_str(data.at<float>(current_frame, 3), 2), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;

	if (data.at<float>(current_frame, 4) > MAX_SHAPE_STD) color = Scalar(0, 0, 255);
	else color = Scalar(0, 255, 0);
	putText(frame, "Left dist: " + rndup_str(data.at<float>(current_frame, 4), 2), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;

	if (data.at<float>(current_frame, 5) > MAX_CHIN_STD) color = Scalar(0, 0, 255);
	else color = Scalar(0, 255, 0);
	putText(frame, "Chin dist: " + rndup_str(data.at<float>(current_frame, 5), 2), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;



}

void writeDetection(Mat frame, int frame_number, Mat annotationFile, String message)
{

	if (annotationFile.empty())
	{
		//cout << "Annotation file for this video is empty. " << endl;
		return;
	}

	//Write if the video was marked or not
	if (annotationFile.at<float>(frame_number, 0) > 0.0f)
	{
		Rect r = Rect(5, 70, 100, 30);
		rectangle(frame, r, Scalar(0, 0, 0), CV_FILLED);
		putText(frame, message, Point(10, 90), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
	}
}

void writeEyeDist(Mat frame, int current_frame, Mat dataCSV, Mat display)
{
	if (dataCSV.empty() || dataCSV.rows <= current_frame)
	{
		cout << "Cannot read eye distance from file. " << endl;
		return;
	}
		
	//Reset matrix with zeros
	frame = Mat::zeros(SIZE_W_POS_ORI, CV_8UC3);


	//Declare padding
	int y_ini = 40;
	int y_step = 40;
	int y = y_ini;
	int x_step = 90;

	Scalar color = Scalar(0,255,0);

	//putText(display, "37", Point(dataCSV.at<float>(current_frame, x_0 + 36), dataCSV.at<float>(current_frame, y_0 + 36)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "38", Point(dataCSV.at<float>(current_frame, x_0 + 37), dataCSV.at<float>(current_frame, y_0 + 37)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "39", Point(dataCSV.at<float>(current_frame, x_0 + 38), dataCSV.at<float>(current_frame, y_0 + 38)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "40" , Point(dataCSV.at<float>(current_frame,x_0 + 39), dataCSV.at<float>(current_frame, y_0 + 39)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "41", Point(dataCSV.at<float>(current_frame, x_0 + 40), dataCSV.at<float>(current_frame, y_0 + 40)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "42", Point(dataCSV.at<float>(current_frame, x_0 + 41), dataCSV.at<float>(current_frame, y_0 + 41)), FONT_HERSHEY_COMPLEX, 0.3f, color);

	//putText(display, "43", Point(dataCSV.at<float>(current_frame, x_0 + 42), dataCSV.at<float>(current_frame, y_0 + 42)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "44", Point(dataCSV.at<float>(current_frame, x_0 + 43), dataCSV.at<float>(current_frame, y_0 + 43)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "45", Point(dataCSV.at<float>(current_frame, x_0 + 44), dataCSV.at<float>(current_frame, y_0 + 44)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "46", Point(dataCSV.at<float>(current_frame, x_0 + 45), dataCSV.at<float>(current_frame, y_0 + 45)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "47", Point(dataCSV.at<float>(current_frame, x_0 + 46), dataCSV.at<float>(current_frame, y_0 + 46)), FONT_HERSHEY_COMPLEX, 0.3f, color);
	//putText(display, "48", Point(dataCSV.at<float>(current_frame, x_0 + 47), dataCSV.at<float>(current_frame, y_0 + 47)), FONT_HERSHEY_COMPLEX, 0.3f, color);

	putText(frame, "Left eye aperture : " + rndup_str(dataCSV.at<float>(current_frame, EYEL_DIST), 3), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;

	putText(frame, "Right eye aperture : " + rndup_str(dataCSV.at<float>(current_frame, EYER_DIST), 3), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;

	putText(frame, "Left eye angle : " + rndup_str(dataCSV.at<float>(current_frame, EYEL_ANGL), 3), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;

	putText(frame, "Right eye angle : " + rndup_str(dataCSV.at<float>(current_frame, EYER_ANGL), 3), Point(90, y), FONT_HERSHEY_COMPLEX, 0.5f, color);
	y += y_step;

}

void writeDetection(Mat frame, int frame_number, Mat annotationData, vector<String> folders)
{
	if (annotationData.empty())
		return;

	int y_ini = 70;
	int y = y_ini;
	int offset = 30;
	int count = 0;

	for (size_t i = 0; i < annotationData.cols; i++)
	{
		
		Rect r = Rect(5, y, 100, 30);
		if (annotationData.at<float>(frame_number, i) > 0.0f)
		{
			rectangle(frame, r, Scalar(0, 0, 0), CV_FILLED);
			putText(frame, folders[i], Point(10, 20+y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
			if(folders[i] == "smile" || folders[i] == "brows_r") count++;
		}
		y += offset;

	}

	if (count >= 2)
	{
		Rect r = Rect(5, y, 100, 30);
		rectangle(frame, r, Scalar(0, 0, 0), CV_FILLED);
		putText(frame, "ENTHUSIASM", Point(10, 20 + y), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
		count++;
	}

}



//Distance in angles between two Euler angles
float ang2rot(int frame_number, Mat data)
{
	//We cannot take this measure the first frame
	if (frame_number < 1) return 0;

	float x1, x2, y1, y2, z1, z2;

	// Read pose from the data and conver it into euler angles
	x1 = data.at<float>(frame_number-1, POSE_RX);	
	x2 = data.at<float>(frame_number, POSE_RX);		
	y1 = data.at<float>(frame_number-1, POSE_RY);	
	y2 = data.at<float>(frame_number, POSE_RY);		
	z1 = data.at<float>(frame_number - 1, POSE_RZ);	
	z2 = data.at<float>(frame_number, POSE_RZ);		

	Quaternion q1 = rad2quaternion(z1, x1, y1);
	q1.normalizeQ();
	Quaternion q2 = rad2quaternion(z2, x2, y2);
	q2.normalizeQ();

	float in = q1.inner(q1, q2);
	float r = 2 * in*in - 1;
	float ang;
	if (r < 0) r = 0;
	if (r > 1) r = 1;

	return  acos(r);
}

//Translation between frames
float calc_translation(int frame_number, Mat data)
{
	//We cannot take this measure the first frame
	if (frame_number < 1) return 0;

	float x1, x2, y1, y2, z1, z2;

	// Read pose from the data and conver it into euler angles
	x1 = data.at<float>(frame_number - 1, POSE_TX);
	x2 = data.at<float>(frame_number, POSE_TX);
	y1 = data.at<float>(frame_number - 1, POSE_TY);
	y2 = data.at<float>(frame_number, POSE_TY);
	z1 = data.at<float>(frame_number - 1, POSE_TZ);
	z2 = data.at<float>(frame_number, POSE_TZ);

	float dist = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1));
	return dist;
}

Quaternion rad2quaternion(float yaw, float roll, float pitch)
{
	Quaternion q;
	float cy = cos(yaw * 0.5);
	float sy = sin(yaw * 0.5);
	float cr = cos(roll * 0.5);
	float sr = sin(roll * 0.5);
	float cp = cos(pitch * 0.5);
	float sp = sin(pitch * 0.5);

	q.w = cy * cr * cp + sy * sr * sp;
	q.x = cy * sr * cp - sy * cr * sp;
	q.y = cy * cr * sp + sy * sr * cp;
	q.z = sy * cr * cp - cy * sr * sp;

	return q;
}

Mat readCSV(string txtfile, bool header)
{
	ifstream inputfile(txtfile);
	if (!inputfile)
		return Mat();
	string current_line;
	vector< vector<float> > all_data;

	//Skip headers if necessary
	if (header) getline(inputfile, current_line);

	// Start reading lines as long as there are lines in the file
	while (getline(inputfile, current_line)) {

		// Now inside each line we need to seperate the cols
		vector<float> values;
		stringstream temp(current_line);
		string single_value;
		while (getline(temp, single_value, ',')) {
			// convert the string element to a integer value
			values.push_back(atof(single_value.c_str()));
			//cout << atof(single_value.c_str()) << endl;
			//waitKey(0);
		}
		// add the row to the complete data vector
		all_data.push_back(values);
	}

	// Now add all the data into a Mat element
	Mat vect = Mat::zeros(all_data.size(), all_data[0].size(), CV_32F);
	// Loop over vectors and add the data
	for (int rows = 1; rows < (int)all_data.size(); rows++) {
		for (int cols = 0; cols< (int)all_data[0].size(); cols++) {
			vect.at<float>(rows, cols) = all_data[rows][cols];
		}
	}

	return vect;
	//cout << vect << endl;
}

//Read csv file of strings
vector<String> sreadCSV(string csvfile)
{
	vector<String> names;

	ifstream inputfile(csvfile);
	if (!inputfile)
		return names;
	string current_line;

	//Since the file is a single columns, there is no need to separate in commas
	while (getline(inputfile, current_line)) 
	{
		names.push_back(current_line);
	}

	return names;

}

void writeCSV(Mat markData, String name)
{
	fstream outputFile;
	outputFile.open(name, std::ios::out);
	//cout << " WRITING ROWS " << markData.rows << " COLS " << markData.cols << endl;

	for (int i = 0; i<markData.rows; i++)
	{
		//outputFile << aviNames[i] << ", ";
		for (int j = 0; j<markData.cols; j++)
		{
			//cout << j << endl;
			if (j < markData.cols - 1)
				outputFile << markData.at<float>(i, j) << ", ";
			else
				outputFile << markData.at<float>(i, j);
			//cout << "WRITING " << markData.at<float>(i, j) << " in row" + to_string(i) + " and col " + to_string(j) << endl;
		}
		outputFile << endl;

	}
	//cout << " FINAL WRITING ROWS " << markData.rows << " COLS " << markData.cols << endl;
	//waitKey(0);
	outputFile.close();
}

void writeCSV(vector<String> names, String name)
{
	fstream outputFile;
	outputFile.open(name, std::ios::out);

	for (int i = 0; i < names.size(); i++)
	{
		outputFile << names[i];
		outputFile << endl;
	}

	outputFile.close();

}

String get_file_name(String path, bool getVideosOnly)
{
	const char *p = path.c_str();
	char name[32];
	char ext[32];
	_splitpath(p, NULL, NULL, name, ext);
	String s(name);
	
	if (getVideosOnly && (ext != ".avi" || ext != ".mp4"))
		return nullptr;
	else
		return s;
}

vector<String> get_files(vector<String> paths)
{
	vector<String> file_names;
	for (int i = 0 ; i < paths.size(); i++)
	{
		String s = get_file_name(paths[i]);
		file_names.push_back(s);
	}

	return file_names;
}

Mat composeDetectionFile(vector<String> folders, string file_name, int frame_count)
{
	Mat detectionData = Mat::zeros(Size(folders.size(), frame_count),CV_32F);
	
	vector<Mat> files;
	for (size_t i = 0; i < folders.size(); i++)
	{
		string file = DETECTION_FOLDER + string("//") + folders[i] + "//" + file_name + ".csv";
		Mat data = readCSV(file, false);
		if (data.empty())
		{
			cout << "Detection file of variable " << folders[i] << " was not found. " << endl;
			continue;
		}
		if (data.rows != frame_count)
		{
			cout << "Number of rows in the detection file of variable " << folders[i] << " does not coincide. " << endl;
			continue;
		}
		else
		{
			data.col(0).copyTo(detectionData.col(i));
		}
	}

	//cout << detectionData << endl;
	return detectionData;
}

Mat CalcDist(vector<String> txtNames)
{
	namedWindow("W");
	for (int file = 0; file < txtNames.size(); file++)
	{
		cout << "Txt file: " << txtNames[file] << endl;

		Mat data = readCSV(txtNames[file]);
		Mat distance_matrix = Mat::zeros(Size(data.rows,16),CV_32F);
		for (int frame = 0; frame < data.rows; frame++)
		{
			float X = data.at<float>(frame, POSE_TX);
			float Y = data.at<float>(frame, POSE_TY);
			float Z = data.at<float>(frame, POSE_TZ);

			//cout << X << " " << Y << " " << Z << endl;

			for (int i = 0; i <= 16; i++)
			{
				float x = data.at<float>(frame, X_0 + i);
				float y = data.at<float>(frame, Y_0 + i);
				float z = data.at<float>(frame, Z_0 + i);

				float distance = sqrt((X-x)*(X-x) + (Y - y)*(Y - y) + (Z - z)*(Z - z));
				distance_matrix.at<float>(frame, i) = distance;
				cout << distance << endl;
				waitKey(0);
			}
		}
	}
	return Mat();
}

void CalcStdVideo(Mat data)
{
	Mat stdFrame = Mat::zeros(Size(1,data.rows),CV_32F);

	for (int i = 1; i < data.rows; i++)
	{
		stdFrame.at<float>(i,1) = CalcStdLandmark(i, data);
		//cout << stdFrame.at<float>(i, 1) << endl;
	}
	
	Mat stdVideo, stdVideoF, mean;
	meanStdDev(stdFrame, mean, stdVideo);
	stdVideo.convertTo(stdVideoF, CV_32F);
	cout << "Standard deviation " << stdVideoF << endl;
}

float CalcStdLandmark(int frame, Mat data)
{
	float X = data.at<float>(frame, POSE_TX);
	float Y = data.at<float>(frame, POSE_TY);
	float Z = data.at<float>(frame, POSE_TZ);

	float X_nose = data.at<float>(frame, X_0+32);
	float Y_nose = data.at<float>(frame, Y_0+32);
	float Z_nose = data.at<float>(frame, Z_0+32);

	Mat distances = Mat::zeros(Size(16, 1), CV_32F);
	Mat distances_nose = Mat::zeros(Size(16, 1), CV_32F);


	for (int i = 0; i <= 15; i++)
	{
		float x = data.at<float>(frame, X_0 + i);
		float y = data.at<float>(frame, Y_0 + i);
		float z = data.at<float>(frame, Z_0 + i);

		float dist = sqrt((X - x)*(X - x) + (Y - y)*(Y - y) + (Z - z)*(Z - z));
		distances.at<float>(0, i) = dist;

		float dist_nose = sqrt((X_nose - x)*(X_nose - x) + (Y_nose - y)*(Y_nose - y) + (Z_nose - z)*(Z_nose - z));
		distances_nose.at<float>(0, i) = dist_nose;
		//cout << "DIST IS " << dist << " and " << distances.at<float>(0, i) << endl;
		//cout << distances << endl;
	}
	Mat std;
	meanStdDev(distances, Mat(), std);
	//cout << std << endl;

	Mat std_nose, std_noseF;
	meanStdDev(distances_nose, Mat(), std_nose);
	//cout << "NOSE " << std_nose << endl;
	std_nose.convertTo(std_noseF, CV_32F);
	//cout << "NOSE " << std_noseF.at<float>(0,0) << endl;
	//cout << distances.at<float>(0, 2) << endl;
	return std_noseF.at<float>(0,0);
}

Mat get_transform_matrix(Mat data, int frame)
{
	float yaw = data.at<float>(frame, POSE_RZ);
	float pitch = data.at<float>(frame, POSE_RY);
	float roll = data.at<float>(frame, POSE_RX);

	float xc = data.at<float>(frame, POSE_TX);
	float yc = data.at<float>(frame, POSE_TY);
	float zc = data.at<float>(frame, POSE_TZ);

	//cout << "YAW  " << yaw << " " << "PITCH  " << pitch << " " << " ROLL  " << roll << endl;
	//cout << "X  " << X << " " << "Y  " << Y << " " << " Z  " << Z << endl;

	float Tr[4][4] = { {1,0,0,xc},
					  {0,1,0,yc},
					  {0,0,1,zc},
					  {0,0,0,1} };
	Mat TrM = (Mat_<float>(4, 4) << 1, 0, 0, xc, 0, 1, 0, yc, 0, 0, 1, zc, 0, 0, 0, 1);
	Mat RzM = (Mat_<float>(4, 4) << cos(yaw), -sin(yaw), 0, 0, sin(yaw), cos(yaw), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	Mat RyM = (Mat_<float>(4, 4) << cos(pitch), 0, sin(pitch), 0, 0, 1, 0, 0, -sin(pitch), 0, cos(pitch), 0, 0, 0, 0, 1);
	Mat RxM = (Mat_<float>(4, 4) << 1, 0, 0, 0, 0, cos(roll), -sin(roll), 0, 0, sin(roll), cos(roll), 0, 0, 0, 0, 1);

	Mat T = RzM*RyM*RxM*TrM;
	Mat T_inv = T.inv();

	return T_inv;
}

Mat get_dist_to_nose(Mat data, int frame)
{
	Mat R = get_transform_matrix(data, frame);

	//Head centre
	Mat centre = (Mat_<float>(4, 1) <<  data.at<float>(frame, POSE_TX),  data.at<float>(frame, POSE_TY), data.at<float>(frame, POSE_TZ), 1);

	//Right contour
	Mat p1 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 0), data.at<float>(frame, Y_0 + 0), data.at<float>(frame, Z_0 + 0), 1);
	Mat p2 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 1), data.at<float>(frame, Y_0 + 1), data.at<float>(frame, Z_0 + 1), 1);
	Mat p3 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0+2), data.at<float>(frame, Y_0+2), data.at<float>(frame, Z_0+2), 1);
	Mat p4 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 3), data.at<float>(frame, Y_0 + 3), data.at<float>(frame, Z_0 + 3), 1);
	Mat p5 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 4), data.at<float>(frame, Y_0 + 4), data.at<float>(frame, Z_0 + 4), 1);
	Mat p6 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 5), data.at<float>(frame, Y_0 + 5), data.at<float>(frame, Z_0 + 5), 1);

	//Left contour
	Mat p17 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 16), data.at<float>(frame, Y_0 + 16), data.at<float>(frame, Z_0 + 16), 1);
	Mat p16 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 15), data.at<float>(frame, Y_0 + 15), data.at<float>(frame, Z_0 + 15), 1);
	Mat p15 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 14), data.at<float>(frame, Y_0 + 14), data.at<float>(frame, Z_0 + 14), 1);
	Mat p14 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 13), data.at<float>(frame, Y_0 + 13), data.at<float>(frame, Z_0 + 13), 1);
	Mat p13 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 12), data.at<float>(frame, Y_0 + 12), data.at<float>(frame, Z_0 + 12), 1);
	Mat p12 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 11), data.at<float>(frame, Y_0 + 11), data.at<float>(frame, Z_0 + 11), 1);

	//Nose
	Mat p34 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 33), data.at<float>(frame, Y_0 + 33), data.at<float>(frame, Z_0 + 33), 1);
	Mat p31 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 30), data.at<float>(frame, Y_0 + 30), data.at<float>(frame, Z_0 + 30), 1);

	//Chin
	Mat p9 = (Mat_<float>(4, 1) << data.at<float>(frame, X_0 + 8), data.at<float>(frame, Y_0 + 8), data.at<float>(frame, Z_0 + 8), 1);

	//Translate their coordinates to moving frame
	Mat p1R = R*p1;
	Mat p2R = R*p2;
	Mat p3R = R*p3;
	Mat p4R = R*p4;
	Mat p5R = R*p5;
	Mat p6R = R*p6;

	Mat p17R = R*p17;
	Mat p16R = R*p16;
	Mat p15R = R*p15;
	Mat p14R = R*p14;
	Mat p13R = R*p13;
	Mat p12R = R*p12;

	Mat p34R = R*p34;
	Mat p31R = R * p31;
	Mat p9R = R * p9;

	//Calculate distances
	//float dist_xR = abs(p3R.at<float>(0, 0) - p34R.at<float>(0, 0))+ abs(p4R.at<float>(0, 0) - p34R.at<float>(0, 0)) + abs(p5R.at<float>(0, 0) - p34R.at<float>(0, 0));
	//float dist_xL = abs(p17R.at<float>(0, 0) - p34R.at<float>(0, 0)) + abs(p16R.at<float>(0, 0) - p34R.at<float>(0, 0)) + abs(p15R.at<float>(0, 0) - p34R.at<float>(0, 0));
	float dist_xR = abs(p3R.at<float>(0, 0) - p31R.at<float>(0, 0)) + abs(p4R.at<float>(0, 0) - p31R.at<float>(0, 0)) + abs(p5R.at<float>(0, 0) - p31R.at<float>(0, 0));
	float dist_xL = abs(p17R.at<float>(0, 0) - p31R.at<float>(0, 0)) + abs(p16R.at<float>(0, 0) - p31R.at<float>(0, 0)) + abs(p15R.at<float>(0, 0) - p31R.at<float>(0, 0));

	float dist_y = abs(p9R.at<float>(1, 0) - p34R.at<float>(1, 0));
	//cout << p3R.at<float>(0, 0) << " " << p34R.at<float>(0, 0) << " " << p15R.at<float>(0, 0) << endl;
	//cout << "DIST1 " << dist1 << " DIST2 " << dist2 << endl;
	//cout << "DIST1: " << dist_xR << " DIST2: " << dist_xL << endl;
	//cout << "Dist chin2nose: " << dist_y << endl;

	Mat distance = (Mat_<float>(1, 3) << dist_xR, dist_xL, dist_y);

	return distance;
}

// Calculates standard deviation upon a neighbourhood
Mat calc_std_distances(Mat distances, int neighbours = 5)
{

	int offset = neighbours*2;

	//Fill the first frames with zeros so that the initial frames are not considered
	Mat stds = Mat::zeros(Size(distances.cols, distances.rows), CV_32F);

	for (int j = 0; j < stds.cols; j++)
	{
		for (int i = offset; i < distances.rows - offset; i++)
		{
			Mat cropped = distances(Rect(j, i - neighbours, 1, neighbours * 2));
			Mat stdValue, stdValueF;
			meanStdDev(cropped, Mat(), stdValue);
			stdValue.convertTo(stdValueF, CV_32F);
			//cout << stdValueF.at<float>(0) << endl;

			stds.at<float>(i, j) = stdValueF.at<float>(0);
		}
	}
	//cout << stds << endl;
	//Concatenate distances + stds so that files can be read at once
	Mat cat;
	hconcat(distances, stds, cat);
	//cout << distances.rows << " " << distances.cols << " " << endl;
	//waitKey(0);
	return cat;
}

//Gets the videos distances to nose
void get_video_distances(Mat data, bool write, String file_name)
{
	//1: Calculate the distances to the nose
	Mat distances = Mat::zeros(Size(3,0),CV_32F);
	for (int i = 0; i < data.rows; i++)
	{
		distances.push_back(get_dist_to_nose(data,i).row(0));
	}

	//2: Calculate the neighbouring standard deviation
	Mat dist_std = calc_std_distances(distances, 5);

	//3: Write the file in a .csv
	if (write)
	{
		String path = OUTPUT_FILES;
		path += "/dist/" + get_file_name(file_name) + ".csv";
		//cout << path << endl;
		writeCSV(dist_std, path);
	}
}

