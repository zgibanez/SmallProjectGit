
// OpenCV includes
#include "Tester.h"
#include "Annotator.h"

using namespace std;
using namespace cv;

Mat marks;
vector<String> folders{"smile", "brows_r"};
void main()
{

	//Retrieve list of files
	vector<String> fileNames, fileList;
	bool use_filtered_videos = false;

	//Use filtered videos with Tester
	if (use_filtered_videos)
	{
		fileList = sreadCSV(OUTPUT_FILES + string("//valid_videos.csv"));
	}
	// Or all the videos
	else
	{
		vector<String> allFiles;
		glob(OUTPUT_FILES + string("//vid//"), allFiles);
		fileList = get_files(allFiles);
	}

	// Retrieve list of videos marked as flawed
	marks = readCSV(OUTPUT_FILES + string("/markedVideos.csv"), false);

	//Tester functions
		//vector<String> files = get_files(txtNames);
		//Tester tester = Tester();
		//tester.all_files = files;
		//tester.initialize();
		//tester.marks = marks;
		////cout << tester.marks << endl;
		//if (tester.marks.empty())
		//	cout << "Marks are empty " << endl;
		//else
		//	cout << "markedVideos.csv loaded successfully" << endl;
		//tester.filter_confidence_level();
		//tester.test_shape_std();
		//tester.evaluate_results();
		//tester.write_valid_videos();

	// Display windows
	Mat frame;
	Mat frame_original_vid;
	Mat frame_processed_vid;
	Mat posAndOri = Mat::zeros(SIZE_W_POS_ORI,CV_8UC3);
	Mat AUC = Mat::zeros(SIZE_W_AUC, CV_8UC3);
	Mat DIST = Mat::zeros(SIZE_W_POS_ORI, CV_8UC3);

	//Flags for navigating
	bool quit = false;				//Are we quitting the program?
	bool previous = false;			//Are we moving to previous frame?
	bool paused = true;				//Is the video paused?

	//Variables for navigating
	int frameRate = 30;				//Initial frame rate when videos are not paused
	int frameStep = 5;
	bool file_loaded = false;		//Has a file been loaded?
	String txtPath;					//Path to txt file with features from OpenFace
	String aviPath;					//Path to the processed avi video for visualizing
	String vidPath;					//Path to the unprocessed video for visualizing wo landmarks
	String distPath;				//Path to the .csv file that contains processed feature data
	String detPath;					//Path to the detection files

	//Variable for annotating
	bool annotating = false;		// Are we annotating data?
	vector<string> opened_files;	// Files in which data has been annotated during the program execution
	string annotation_file;			// Name of the current annotating file
	Annotator annotator;
	int frame1 = 0, frame2 = 0;
	bool frame1_selected = false;

	int number_of_videos = fileList.size();

	for (int i = 0; i < number_of_videos; i++)
	{
		Mat dataCSV;
		Mat dataDistCSV;
		Mat dataDetcCSV;
		Mat dataDetc;
		VideoCapture vc;
		VideoCapture vs;

		//If file has been selected from the console load it
		if (!file_loaded)
		{
 			aviPath  = OUTPUT_FILES + string("//avi//") +		fileList[i] + ".avi";
			vidPath  = OUTPUT_FILES + string("//vid//") +		fileList[i] + ".mp4";
			//txtPath  = OUTPUT_FILES + string("//txt//") +		fileList[i] + ".txt";
			txtPath =  TXTFILES + string("//") + fileList[i] + ".csv";
			distPath = OUTPUT_FILES + string("//dist//") +		fileList[i] + ".csv";
			detPath  = DETECTION_FOLDER + string("//smiles//") +	fileList[i] + ".csv";
		}
		else file_loaded = false;

		//Open the processed video
		vc.open(aviPath);

		//Open the original video
		vs.open(vidPath);
		cout << "Current file: " << fileList[i] << endl;

		//Load .csv data
		dataCSV		= readCSV(txtPath);
		dataDistCSV = readCSV(distPath);
		dataDetcCSV = readCSV(detPath);
		dataDetc = composeDetectionFile(folders, fileList[i], vs.get(CAP_PROP_FRAME_COUNT));

		//CALCULATIONS VIDEO 
			//CalcStdVideo(dataCSV);
			//get_video_distances(dataCSV,true, aviPath);
			//continue;
			//cout << "NAME: " << get_file_name(aviNames[i]);

		int frame_count = 1;
		for (;;)
		{
			vc >> frame_processed_vid;
			vs >> frame_original_vid;

			if (frame_processed_vid.empty() || frame_original_vid.empty())
			{
				cout << "Error reading the video " << fileList[i] << endl;
				break;
			}
			else
			{ 
				hconcat(frame_original_vid, frame_processed_vid, frame);
			}

			///CALCULATIONS
			//CALC STD
				//CalcStdLandmark(frame_count, dataCSV);
			//CALC ROTATION
				//get_dist_to_nose(dataCSV, frame_count);
			///END CALCULATIONS 

			//Display AU, Position/Orientation and distance
			writeAU(AUC, frame_count, dataCSV);						imshow("AUC", AUC);
			writePosOri(posAndOri, frame_count, dataCSV);			imshow("posAndOri", posAndOri);
			//writeDist(DIST, frame_count, dataDistCSV, i, marks);	imshow("Std", DIST);
			writeEyeDist(DIST, frame_count, dataCSV, frame);				imshow("Eye distances", DIST);
			//writeDetection(frame, frame_count, dataDetcCSV, "SMILE");
			writeDetection(frame, frame_count, dataDetc, folders);


			//Display confidence interval and speed
			putText(frame, "Confidence: " + to_string(dataCSV.at<float>(frame_count, 2)), Point(10, 50), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
			if (!paused) putText(frame, "Speed: " + to_string(frameRate), Point(10, frame.rows - 10), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 0, 0));
			if (frame1_selected)
			{
				putText(frame, "Annotation length: " + to_string(abs(frame_count-frame1)),Point(10,70), FONT_HERSHEY_COMPLEX, 0.5f, Scalar(255, 255, 255));
			}
			//Display results
			imshow("video frame", frame);
			
			int key;

			if (paused)
			{
				key = waitKey(0);
			}
			else
			{
				key = waitKey(frameRate);
			}

			if (key == ESC || key == 'q')
			{
				quit = true;
				break;
			}
			else if (key == 'n')
			{
				break; //To next video
			}
			else if (key == 'b')
			{
				if (i == 0)
				{
					cout << "No previous videos exist" << endl;
				}
				else
				{
					// Substact 2 units to index to get previous video
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
			else if (key == 'o')
			{
				//Reset annotation
				frame1_selected = false;

				string fileName;
				cout << "Open video file: ";
				cin >> fileName;
				size_t found;
				for (int file = 0; file <= fileList.size(); file++)
				{
					found = fileList[file].find(fileName);
					if (found != string::npos)
					{
						cout << "Found at " << fileList[file] << endl;
						aviPath  = OUTPUT_FILES + string("//avi//") + fileList[file] + ".avi";
						txtPath  = OUTPUT_FILES + string("//txt//") + fileList[file] + ".txt";
						distPath = OUTPUT_FILES + string("//dist//") + fileList[file] + ".csv";
						vidPath  = OUTPUT_FILES + string("//vid//") + fileList[file] + ".mp4";
						if(file>0) i = file-1;
						file_loaded = true;
					}
				}
				if (file_loaded) break;
				else cout << "Could not find file with " << fileName << endl;
			}
			else if (key == 'a') //Take annotation (from this frame to its previous 20)
			{
				if (!annotating) //If annotation file is not selected, break
				{
					cout << " Mark cannot be placed because an annotating file has not been selected. Press 's' to select an annotation file. " << endl;
					continue;
				}
					frame2 = frame_count;
					annotator.writeSample(ANNOTATIONS_FOLDER + string(annotation_file) + string(".xml"), dataCSV, frame_count, frame_count-AVERAGE_LENGTH_SMILE, fileList[i]);
					frame1_selected = false;

			}
			else if (key == 'd')
			{
				if (frame1_selected)
				{
					frame1_selected = false;
					cout << "Annotation was reset. " << endl;
				}
			}
			else if (key == 's') //Select annotation file
			{
				annotating = true;
				cout << endl << "Commencing annotations. Write the name of the file where you want to write: " << endl;
				cin >> annotation_file;
				opened_files.push_back(annotation_file);
				cout << "Annotations will be stored in " << annotation_file + ".xml" << " and " << annotation_file + ".csv" << endl;
			}

			if (paused)
			{
				if (key == L_ARROW) //Rewind frame
				{
					if (frame_count > 1)
					{
						frame_count--;
						vc.set(CV_CAP_PROP_POS_FRAMES, frame_count);
						vs.set(CV_CAP_PROP_POS_FRAMES, frame_count);
					}
				}
				if (key == R_ARROW)
				{
					frame_count++;
				}
			}
			else
				frame_count++;
		}
		if (quit) break;
	}

	// Write the csv files of all the .xml that were opened
	if (!opened_files.empty())
	{
		for (int i = 0; i < opened_files.size(); i++)
		{
			annotator.writeCSVSampleFile(OUTPUT_FILES + string("//annotations//") + string(opened_files[i]));
			cout << " File " << opened_files[i] + ".csv" << " saved succesfully." << endl;
		}
		getchar();
	}
}

