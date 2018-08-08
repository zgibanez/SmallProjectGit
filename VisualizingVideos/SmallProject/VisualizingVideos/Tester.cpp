#include "Tester.h"

using namespace std;
using namespace cv;

Tester::Tester()
{
}

Tester::~Tester() {}

void Tester::initialize()
{
	cout << "Initializing Tester ..." << endl;
	for (int i = 0; i < all_files.size(); i++)
	{
		VideoScore vs;
		vs.name = all_files[i];
		video_scores.push_back(vs);
	}
}

///<summary>
/// Returns list of files that have passed the confidence level test
///</summary>
void Tester::filter_confidence_level(bool verbose)
{

	for (int i = 0; i < video_scores.size(); i++)
	{

		if (test_file_conf_lvl(video_scores[i].name))
		{
			if(verbose) cout << "File " + video_scores[i].name + " PASSED the confidence level test." << endl;
			video_scores[i].passed_confidence = true;
		}
		else
		{
			if(verbose) cout << "File " + video_scores[i].name + " DID NOT pass the confidence level test." << endl;
			video_scores[i].passed_confidence = false;
		}
	}
}

///<summary>
/// Returns true or false depending on if the file suffices the minimum percentage of frames with the confidence level above the minimum value.
///</summary>
bool Tester::test_file_conf_lvl(string file_name)
{
	Mat data = readCSV(OUTPUT_FILES + string("//txt/") + file_name + string(".txt"));

	//Take out the columns with confidence levels
	Mat conf_lvl = data.col(CONFIDENCE);

	//Apply threshold upon confidence level
	Mat thresh;
	threshold(conf_lvl, thresh, MIN_CONF_LVL, 1, THRESH_BINARY);
	//namedWindow("Q");
	//cout << file_name << thresh << endl;
	//waitKey(0);

	//Sum the columns
	int sum = 0;
	for (int i = 0; i < thresh.rows; i++)
	{
		if (thresh.at<float>(i, 0) >= 1)
		{
			sum++;
		}

	}

	//Check if the frames surpasses the confidence level
	if ((float)sum / (float)thresh.rows > PER_CONF_LVL)
	{
		return true;
	}
	else
		return false;
}

void Tester::test_shape_std()
{
	cout << endl << endl << "Starting shape evaluation..." << endl;

	for (int i = 0; i < video_scores.size(); i++)
	{
		//If the video was filtered by confidence level, skip it
		if (!video_scores[i].passed_confidence)
			continue;

		//Else, evaluate its standar deviation
		video_scores[i].passed_shape = test_file_shape_std(video_scores[i].name);

	}
}

//Returns "true" if the file passed the shape test
bool Tester::test_file_shape_std(string file_name)
{
	//Load distances to the nose
	Mat data = readCSV(OUTPUT_FILES + string("//dist/") + file_name + string(".csv"));
	
	//Load facial action units
	Mat dataAU = readCSV(OUTPUT_FILES + string("//txt/") + file_name + string(".txt"));
	if (data.empty())
	{
		cout << "ERROR: File " << file_name + string(".csv") << " not found" << endl;
		return false;
	}

	//Test the contour std
	for (int i = OFFSET_EVALUATION; i < data.rows - OFFSET_EVALUATION; i++)
	{
		if (data.at<float>(i, STD_FACE_LEFT) > MAX_SHAPE_STD || data.at<float>(i, STD_FACE_RIGHT) > MAX_SHAPE_STD)
		{
			cout << "File " << file_name << " DID NOT PASS the shape test" << endl;
			return false;
		}
	}

	int neighbours = 5;
	float jaw_drop_thresh = 0.1;
	float lips_part_thresh = 0.1;
	//Test the chin std
	for (int i = OFFSET_EVALUATION; i < data.rows - OFFSET_EVALUATION; i++)
	{
		if (data.at<float>(i, STD_FACE_CHIN) > MAX_CHIN_STD)
		{
			//Check for the neighbouring 
			for (int j = -neighbours; j <= neighbours; j++)
			{
				//If the std of the chin changes but the jaw was dropping or lips were parting, ignore it
				if (dataAU.at<float>(i + j, AU25_r) > jaw_drop_thresh || dataAU.at<float>(i + j, AU26_r) > lips_part_thresh)
				break;

				if (j == neighbours)
				{
					cout << "File " << file_name << " DID NOT PASS the shape test" << endl;
					return false;
				}
			}
		}
	}

	cout << "File " << file_name << " PASSED the shape test" << endl;
	return true;
}

void Tester::evaluate_results()
{
	//namedWindow("W");
	int valid_videos = 0;

	int positives = 0;
	int negatives = 0;

	int true_positive = 0;
	int true_negative = 0;
	int false_positive = 0;
	int false_negative = 0;

	for (int i = 0; i < video_scores.size(); i++)
	{
		if (!video_scores[i].passed_confidence)
		{
			//cout << " --> " << marks.at<float>(i, 1) << endl;
			continue;

		}

		valid_videos++;
		bool passed_test = video_scores[i].passed_shape;
		bool marked_as_good;

		//cout << "Reading video " << video_scores[i].name << " Passes std test? " << !not_passed << " Marked as flawed at row " + to_string(i) + "?" << marks.at<float>(i, 1) << endl;
		//cout << marks.at<float>(i, 1) << " " << video_scores[i].passed_shape << endl;
		if (marks.at<float>(i, 1) >= 1)
		{
			marked_as_good = false;
			positives++;
		}
		else
		{
			marked_as_good = true;
			negatives++;
		}

		if (marked_as_good && passed_test) true_negative++;
		if (marked_as_good && !passed_test) {
			cout << " Video " << video_scores[i].name << " is a false positive. " << endl;
			false_positive++;
		}
		if (!marked_as_good && !passed_test) true_positive++;
		if (!marked_as_good && passed_test) 
		{
			cout << " Video " << video_scores[i].name << " is a false negative. " << endl;
			false_negative++;
		}
	}

	float precision = (float)true_positive / float(true_positive + false_positive);
	float recall = (float)true_positive / float(true_positive + false_negative);

	cout << " There are " << video_scores.size() << " of which " << valid_videos << " are valid " << endl;
	cout << " Precision is " << precision << endl;
	cout << " Recall is " << recall << endl;
	cout << " Positives: " << positives << " Negatives: " << negatives << endl;
	cout << " TP: " << true_positive << " TN: " << true_negative << endl;
	cout << " FP: " << false_positive << " FN: " << false_negative << endl;

}

void Tester::write_valid_videos()
{
	vector<String> valid_video_names;
	for (int i = 0; i<video_scores.size(); i++)
	{
		if (video_scores[i].passed_confidence && video_scores[i].passed_shape)
		{
			valid_video_names.push_back(video_scores[i].name);
		}
	}

	writeCSV(valid_video_names, "valid_videos.csv");
}