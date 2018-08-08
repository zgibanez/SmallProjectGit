#include "Annotator.h"


using namespace std;
using namespace cv;

Annotator::Annotator()
{
	videos = sreadCSV(OUTPUT_FILES + string("//valid_videos.csv"));
}

Annotator::~Annotator() {}

void Annotator::begin()
{

	VideoCapture vc;
	Mat frame;
	Mat dataCSV, dataCSVdist;
	for (int i = 0; i < videos.size(); i++)
	{
		vc.open(OUTPUT_FILES + string("//") + videos[i]);

		for (;;)
		{
			vc >> frame;
			if (frame.empty())
				break;
		}
	}

}

//Write the data between two specified frames. Columns is used to specify which columns will be written.
void Annotator::writeSample(string file_name, Mat data, int frame1, int frame2, string video_name)
{
	int f1, f2;
	if (frame1 > frame2) 
	{
		f1 = frame2;
		f2 = frame1;
	}
	else
	{
		f1 = frame1;
		f2 = frame2;
	}

	if (frame2 < 0)
	{
		cout << "Cannot take a sample of " + to_string(AVERAGE_LENGTH_SMILE) + " frames." << endl;
		return;
	}

	/*Mat data_sample = data(Rect(Point(0,f1),Point(data.cols-1,f2)));
	Mat new_sample = Mat::zeros(Size(columns.size(), f2 - f1), CV_32F);
	for (int i = 0; i < columns.size(); i++)
	{
		data_sample.col(columns[i]).copyTo(new_sample.col(i));
	}*/

	FileStorage fo(file_name, FileStorage::READ);
	int samples = 0;
	/*vector<Mat> mat_samples;*/
	vector<string> video_names;
	vector<int> start_frames;

	//If file exist, open and take its contents
	if (fo.isOpened())
	{
		fo["samples"] >> samples;
		for (int i = 0; i < samples; i++)
		{
			Mat sampleMat;
			string sample_video_name;
			int sample_start_frame;
			/*fo["sample" + to_string(i)] >> sampleMat;*/
			fo["file_name" + to_string(i)] >> sample_video_name;
			fo["start_frame" + to_string(i)] >> sample_start_frame;
		/*	mat_samples.push_back(sampleMat);*/
			video_names.push_back(sample_video_name);
			start_frames.push_back(sample_start_frame);
		}
		fo.release();
	}
	else
	{
		cout << "File " + file_name + " does not exist. Do you want to create this file? Y/N " << endl;
		int yn;
		
		do 
		{
			yn = waitKey(0);
			if (yn == 'n')
			{
				cout << "File " + file_name + " will NOT be created. " << endl;
				return;
			}
			else if (yn == 'y')
			{
				cout << "File " + file_name + " will be created. " << endl;
			}
			else
			{
				cout << "Press Y or N to choose." << endl;
			}
		} while (yn != 'n' && yn != 'y');

	}

	//Then rewrite it again + new sample.
	//If this file does not exist, it will create a new one
	samples++;
	FileStorage fs(file_name,FileStorage::WRITE);
	fs << "samples" << samples;
	for (int i = 0; i < video_names.size(); i++)
	{
		//fs << "sample" + to_string(i) << mat_samples[i];
		fs << "file_name" + to_string(i) << video_names[i];
		fs << "start_frame" + to_string(i) << start_frames[i];
	}
	//Mat new_sample1D = new_sample.reshape(0, 1);
	//fs << "sample" + to_string(samples-1) << new_sample1D;
	fs << "file_name" + to_string(samples - 1) << video_name;
	fs << "start_frame" + to_string(samples - 1) << f2;

	cout << " Sample " << samples-1 << " written in " << string(file_name) + string(".xml") << endl;
	fs.release();
}

void Annotator::writeCSVSampleFile(string file_name)
{
	string fileName = get_file_name(file_name);

	// Read from the .XML
	FileStorage fo(ANNOTATIONS_FOLDER + fileName + ".xml", FileStorage::READ);
	int samples = 0;
	vector<Mat> mat_samples;
	vector<string> video_names;
	vector<int> start_frames;
	vector<String> fileNames;
	//If file exist, open and take its contents
	if (fo.isOpened())
	{
		fo["samples"] >> samples;
		for (int i = 0; i < samples; i++)
		{
			Mat sampleMat;
			string sample_video_name;
			int sample_start_frame;
			fo["sample" + to_string(i)] >> sampleMat;
			fo["file_name" + to_string(i)] >> sample_video_name;
			fo["start_frame" + to_string(i)] >> sample_start_frame;
			mat_samples.push_back(sampleMat);
			video_names.push_back(sample_video_name);
			start_frames.push_back(sample_start_frame);
		}
		fo.release();
	}

	// Write on the .CSV
	fstream outputFile;
	outputFile.open(ANNOTATIONS_FOLDER + fileName + string(".csv"), std::ios::out);

	//Write headers
	outputFile << "sample, " << " file_name, " << " start_frame" << endl;
	for (int i = 0; i<mat_samples.size(); i++)
	{

		//First columns are sample number, file name and frame where the sample was captured
		outputFile << i << ", " << video_names[i] << ", " << start_frames[i] << ", ";

		for (int j = 0; j<mat_samples[i].cols; j++)
		{
			//cout << j << endl;
			if (j < mat_samples[i].cols - 1)
				outputFile << mat_samples[i].at<float>(0, j) << ", ";
			else
				outputFile << mat_samples[i].at<float>(0, j);
			//cout << "WRITING " << markData.at<float>(i, j) << " in row" + to_string(i) + " and col " + to_string(j) << endl;
		}
		outputFile << endl;
	}
	//cout << " FINAL WRITING ROWS " << markData.rows << " COLS " << markData.cols << endl;
	//waitKey(0);
	outputFile.close();
}


