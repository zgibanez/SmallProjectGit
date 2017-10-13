#pragma once
#include "OpenFaceOutput.h"


#define STD_FACE_RIGHT 3
#define STD_FACE_LEFT 4
#define STD_FACE_CHIN 5


#define MIN_CONF_LVL 0.8 //Minimum confidence level
#define PER_CONF_LVL 0.9 //Minimum percentage of valid frames

#define MAX_SHAPE_STD 15.0 //Maximum std shape
#define MAX_CHIN_STD 5.0 //Maximum std shape


#define OFFSET_EVALUATION 30

typedef struct {
	cv:: String name;
	bool passed_confidence;
	bool passed_shape;
} VideoScore;

class Tester
{
	public:
	Tester();
	virtual ~Tester();

	void Tester::initialize();

	std::vector<cv::String> all_files;
	std::vector<VideoScore> video_scores;
	std::vector<cv::String> valid_files;
	cv::Mat marks;

	void Tester::filter_confidence_level(bool verbose = false);
	bool Tester::test_file_conf_lvl(std::string file_name);

	void Tester::test_shape_std();
	bool Tester::test_file_shape_std(std::string file_name);

	void Tester::evaluate_results();

	void Tester::write_valid_videos();


};