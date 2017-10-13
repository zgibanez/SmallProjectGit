#pragma once
#include <opencv2/videoio/videoio.hpp>  // Video write
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <math.h>
#include <numeric>
#include <functional>
#include <opencv2/videoio/videoio_c.h>  // Video write
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>


#define SIZE_W_AUC Size(400,700)
#define SIZE_W_POS_ORI Size(400,250)

//Paths
//#define OUTPUT_FILES "D://SmallProject//VideosOutput"
#define OUTPUT_FILES "D://SmallProject//videos_ent_con"
#define TXTFILES     "D://SmallProject//txt_all"
#define ANNOTATIONS_FOLDER "D://SmallProject//VideosOutput//annotations//"
#define OUTPUT_FILES_TEST "D://SmallProject//videos_ent_con"
#define DETECTION_FOLDER "D://SmallProject//VideosOutput//detections"

//Math constants
#define PI 3.1416

//Keys
#define ESC 27
#define L_ARROW 2424832
#define R_ARROW 2555904
#define SHIFT 16

// General data
#define FRAME 0
#define TIMESTAMP 1
#define CONFIDENCE 2
#define SUCCESS 3

//Positions and orientations
#define GAZE_0_X 4
#define GAZE_0_Y 5
#define GAZE_0_Z 6
#define GAZE_1_X 7
#define GAZE_1_Y 8
#define GAZE_1_Z 9
#define POSE_TX 10
#define POSE_TY 11
#define POSE_TZ 12
#define POSE_RX 13
#define POSE_RY 14
#define POSE_RZ 15

//2D landmarks
#define x_0 16
#define y_0 84

//3D landmarks (add 62 and 124 to get coordinates Y and Z)
#define X_0 152
#define Y_0 220
#define Z_0 288

//AU Strength
#define AU01_r 356
#define AU02_r 357
#define AU04_r 358
#define AU05_r 359
#define AU06_r 360
#define AU07_r 361
#define AU09_r 362
#define AU10_r 363
#define AU12_r 364
#define AU14_r 365
#define AU15_r 366
#define AU17_r 367
#define AU20_r 368
#define AU23_r 369
#define AU25_r 370
#define AU26_r 371
#define AU45_r 372

// AU Presence
#define AU01_c 373
#define AU02_c 374
#define AU04_c 375
#define AU05_c 376
#define AU06_c 377
#define AU07_c 378
#define AU09_c 379
#define AU10_c 380
#define AU12_c 381
#define AU14_c 382
#define AU15_c 383
#define AU17_c 384
#define AU20_c 385
#define AU23_c 386
#define AU25_c 387
#define AU26_c 388
#define AU28_c 389
#define AU45_c 390

//Added features
#define AU01_AU02 391
#define EYEL_DIST 392
#define EYER_DIST 393
#define EYEL_ANGL 394
#define EYER_ANGL 395

//Display functions
#define MAX_SHAPE_STD 15.0 //Maximum std shape
#define MAX_CHIN_STD 5.0 //Maximum std shape
void writeAU(cv::Mat AUCFrame, int current_frame, cv::Mat data = cv::Mat());
void writePosOri(cv::Mat PosOriFrame, int current_frame, cv::Mat data = cv::Mat());
void writeDist(cv::Mat frame, int current_frame, cv::Mat data, int index, cv::Mat marks);
void writeEyeDist(cv::Mat frame, int current_frame, cv::Mat dataCSV, cv::Mat display);
void writeDetection(cv::Mat frame, int frame_number, cv::Mat annotationFile, cv::String message);
void writeDetection(cv::Mat frame, int frame_number, cv::Mat annotationData, std::vector<cv::String> folders);

//Read-Write CSV functions
cv::Mat readCSV(std::string txtfile, bool header = true);
std::vector<cv::String> sreadCSV(std::string txtfile);
void writeCSV(cv::Mat markData, cv::String name);
void writeCSV(std::vector<cv::String> names, cv::String name);

cv::Mat composeDetectionFile(std::vector<cv::String> folders, std::string file_name, int frame_count);

// Calculations
cv::Mat CalcDist(std::vector<cv::String> txtNames);
float CalcStdLandmark(int frame, cv::Mat data);
void CalcStdVideo(cv::Mat data);
cv::Mat get_transform_matrix(cv::Mat data, int frame);
cv::Mat get_dist_to_nose(cv::Mat data, int frame);
void get_video_distances(cv::Mat data, bool write = false, cv::String name = "s");

//Utils functions
float ang2rot(int frame_number, cv::Mat data);
float calc_translation(int frame_number, cv::Mat data);
cv::String get_file_name(cv::String path, bool getVideosOnly = false);
std::vector<cv::String> get_files(std::vector<cv::String> paths);
std::string rndup_str(float n, int decimals = 3);


//Classes
class Quaternion {

public:
	Quaternion();
	~Quaternion();
	float w;
	float x;
	float y;
	float z;

	void Quaternion::normalizeQ();
	float Quaternion::inner(Quaternion q1, Quaternion q2);

};

Quaternion rad2quaternion(float yaw, float roll, float pitch);