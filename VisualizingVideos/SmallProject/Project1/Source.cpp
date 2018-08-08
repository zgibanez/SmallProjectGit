#include <OpenFaceOutput.h>

using namespace std;
using namespace cv;

#define MARKED_VIDEOS "markedVideos.csv"
bool file_exist = false;
void writeMarkedVideos(Mat markData, vector<String> aviNames)
{
	fstream outputFile;
	outputFile.open("markedVideos.csv", std::ios::out);
	cout << " WRITING ROWS " << markData.rows << " COLS " << markData.cols << endl;

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
	cout << " FINAL WRITING ROWS " << markData.rows << " COLS " << markData.cols << endl;
	waitKey(0);
	outputFile.close();
}

void showMarks(Mat frame, int i, Mat markData)
{
	if (markData.at<float>(i, 0) == 1)
	{
		putText(frame, "VIDEO REJECTED", Point(10, 50), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(0, 255, 0));
	}

	if (markData.at<float>(i, 1) == 1)
	{
		rectangle(frame, Rect(50, 50, 150, 25),Scalar(0,0,0),CV_FILLED);
		putText(frame, "SHAPE CHANGE", Point(50, 70), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(0, 255, 0));
	}

	if (markData.at<float>(i, 2) == 1)
	{
		rectangle(frame, Rect(50, 50+20, 150, 25), Scalar(0, 0, 0), CV_FILLED);
		putText(frame, "CHIN SQUISH", Point(50, 90), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(0, 255, 0));
	}

	if (markData.at<float>(i, 3) == 1)
	{
		rectangle(frame, Rect(50, 50 + 40, 150, 25), Scalar(0, 0, 0), CV_FILLED);
		putText(frame, "EYEBROW FLIP", Point(50, 110), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(0, 255, 0));
	}

}

void main()
{

	//Collect videos and .csv files
	vector<String> txtNames, aviNames;
	glob(OUTPUT_FILES + string("//txt"), txtNames);
	sort(txtNames.begin(), txtNames.end());
	glob(OUTPUT_FILES + string("//avi"), aviNames);
	sort(aviNames.begin(), aviNames.end());

	// Display windows
	Mat frame;

	//Flags for navigating
	bool quit = false;
	bool previous = false;
	bool paused = true;

	//Variables for navigating
	int frameRate = 30;
	int frameStep = 5;
	String txtPath;
	String aviPath;
	bool file_loaded = false;

	//Variables for writing
	Mat markData = readCSV(MARKED_VIDEOS, false);
	if (markData.empty())
	{
		cout << MARKED_VIDEOS << " could not be found. A file will be created. " << endl;
		markData = Mat::zeros(Size(4, txtNames.size()), CV_32F);
	}
	else
	{
		cout << MARKED_VIDEOS << " loaded successfully." << endl;
	}

	for (int i = 0; i < aviNames.size(); i++)
	{
		Mat dataCSV;
		VideoCapture vc;
		//If file has been selected from the console load it
		if (!file_loaded)
		{
			aviPath = aviNames[i];
			txtPath = txtNames[i];
		}
		else file_loaded = false;

		dataCSV = readCSV(txtPath);
		vc.open(aviPath);

		cout << "Video file: " << aviPath << endl;
		cout << "Flags are " << markData.at<float>(i, 0) << " " << markData.at<float>(i, 1) << " " << markData.at<float>(i, 2) << " " << markData.at<float>(i, 3) << endl;
		//cout << "Txt file: " << txtPath << endl;
		
		int frame_count = 1;
		for (;;)
		{
			vc >> frame;

			if (frame.empty())
				break;

			//Display results
			showMarks(frame, i, markData);
			imshow("video frame", frame);

			//Write data on video
			//putText(frame, "Confidence: " + to_string(dataCSV.at<float>(frame_count, 2)), Point(10, 50), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
			if (!paused) putText(frame, "Speed: " + to_string(frameRate), Point(10, frame.rows - 10), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));

			int key;

			if (paused)
			{
				key = waitKey(0);
			}
			else
			{
				key = waitKey(frameRate);
			}

			if (key == ESC)
			{
				quit = true;
				break;
			}

			if (key == 'n')
			{
				break;
			}
			else if (key == 'b')
			{
				if (i == 0)
				{
					cout << "No previous videos exist" << endl;
				}
				else
				{
					// Substact 2 units to index to get previopus video
					i -= 2;
					break;
				}

			}
			else if (key == 'p')
			{
				paused = !paused;
			}
			else if (key == 'z')
			{
				if ((frameRate + frameStep) < 120)
					frameRate += frameStep;
				else
					frameRate = 120;
			}
			else if (key == 'x')
			{
				if ((frameRate - frameStep) > 1)
					frameRate -= frameStep;
				else
					frameRate = 1;
			}

			if (key == 'q')
			{
				//cout << markData.at<float>(i, 1) << endl;
				if (markData.at<float>(i, 1) == 0)
					markData.at<float>(i, 1) = 1;
				else
					markData.at<float>(i, 1) = 0;
			}
			else if (key == 'w')
			{
				if (markData.at<float>(i, 2) == 0)
					markData.at<float>(i, 2) = 1;
				else
					markData.at<float>(i, 2) = 0;
			}
			else if (key == 'e')
			{
				if (markData.at<float>(i, 3) == 0)
					markData.at<float>(i, 3) = 1;
				else
					markData.at<float>(i, 3) = 0;
			}
			else if (key == 'a')
			{
				string fileName;
				cout << "Open video file: ";
				cin >> fileName;
				size_t found;
				for (int file = 0; file <= txtNames.size(); file++)
				{
					found = txtNames[file].find(fileName);
					if (found != string::npos)
					{
						cout << "Found at " << txtNames[file] << endl;
						aviPath = aviNames[file];
						txtPath = txtNames[file];
						file_loaded = true;
						i = file - 1;
					}
				}
			}
			//cout << " ROWS " << markData.rows << " COLS " << markData.cols << endl;

			if (key == L_ARROW)
			{
				if (frame_count > 1)
				{
					frame_count--;
					vc.set(CV_CAP_PROP_POS_FRAMES, frame_count);
				}

			}
			else
			{
				frame_count++;
			}


		}
		
		if (quit)
		{
			cout << " ROWS " << markData.rows << " COLS " << markData.cols << endl;
			cout << aviNames.size() << " " << txtNames.size() << endl;
			break;
		}
	}

	writeMarkedVideos(markData, aviNames);

	//for (int i = 0; i < txtNames.size(); i++)
	//	cout << txtNames[i] << endl;
	//cout << txtNames[0] << endl;
}