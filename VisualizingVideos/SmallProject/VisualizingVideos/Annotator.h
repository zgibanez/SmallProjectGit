#include "OpenFaceOutput.h"

#define AVERAGE_LENGTH_SMILE 20
class Annotator
{
	public:
		Annotator();
		virtual ~Annotator();
		std::vector<cv::String> videos;

		void Annotator::begin();
		void Annotator::writeSample(std::string file_name, cv::Mat data, int frame1, int frame2, std::string video_name);
		void Annotator::writeCSVSampleFile(std::string file_name);
};
