#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>
#include <string>
#include <glob.h>
#include <sys/stat.h>
#include <sys/types.h>

// Header:
void SearchForFaces(cv::Mat picture, int i, cv::CascadeClassifier face_cascade, bool showAtEnd);

int main(){
	cv::CascadeClassifier face_cascade;
	
	std::string foldname;
	std::vector<cv::String> list;
	
	// Ask for file or folder name
	std::cout << "Please type in the file (requires .jpg) or folder name: " << std::endl;
	std::cin >> foldname;
	
	std::vector<cv::Mat> folder; // Create a vector that will be filled with all the pictures the detection is run on.
	
	if(foldname.find(".jpg") != std::string::npos){ // If the input is just a single picture
		if(!cv::imread(foldname).empty()){
			folder.push_back(cv::imread(foldname));
		}
	}
	else{ // If the input is a folder
		glob(foldname + "/*.jpg", list, false); // Read in all the pictures in the folder
		for (int i=0; i<list.size(); ++i){
    			folder.push_back(cv::imread(list[i]));
    		}
	}
	
    	// Does the file or folder exist?
	while (folder.empty()){
		std::cout << "File (or folder) not found or empty. Please try again: " << std::endl;
		std::cin >> foldname;
		if(foldname.find(".jpg") != std::string::npos){ // picture
			if(!cv::imread(foldname).empty()){
				folder.push_back(cv::imread(foldname));
			}
		}
		else{ // folder
			glob(foldname + "/*.jpg", list, false);
			for (int i=0; i<list.size(); ++i){
	    			folder.push_back(cv::imread(list[i]));
	    		}	
		}	
	}
	
	// Choose a cascade:
	std::cout << "What do you want to detect? Select from: (If you type something else the default is the first)" << std::endl;
	std::vector<cv::String> trainerlist;
	glob("trainer/*.xml", trainerlist, false); //Read in all available cascades
		
	int trainer_counter = trainerlist.size();
	for(int i=0;i<trainer_counter;++i){
		trainerlist[i] = trainerlist[i].substr(trainerlist[i].find("/")+1,trainerlist[i].find(".xml")-8); // remove trainers/ at the begining and .xml at the end
		std::cout << i+1 << ". " << trainerlist[i] << std::endl;
	}
	int selected_trainer = 1;
	std::cin >> selected_trainer;
	if(selected_trainer < 1 || selected_trainer > trainer_counter){ //We set the first trainer as default one in case of invalid input.
		selected_trainer = 1;
	}
	
	std::string face_cascade_name = "trainer/" + trainerlist[selected_trainer-1] + ".xml";
	
	if (!face_cascade.load(face_cascade_name)){ // Couldnt load, so do nothing
		std::cout << "Trainer not found" << std::endl;
		return -1;
	}


	// Run main function
	if(folder.size() == 1) { // In case of single picture detection, the final result will be displayed
		SearchForFaces(folder[0], 0, face_cascade, true);
		// Wait for closing of detected picture window
		cv::waitKey(0);
	}
	else{ // For multiple picture detections, we do not display the results.
		for(int i=0; i<folder.size(); ++i) {
			SearchForFaces(folder[i], i, face_cascade, false);
		}
		std::cout << "Detection completed" << std::endl;
	}
}
	
// Face detection process:
void SearchForFaces(cv::Mat picture, int i, cv::CascadeClassifier face_cascade, bool showAtEnd) {
	std::vector<cv::Rect> faces;
	cv::Mat Greypicture;

	cvtColor(picture, Greypicture, cv::COLOR_BGR2GRAY); // Function given by OpenCV to change color profile
	equalizeHist(Greypicture, Greypicture); // Equalizes the histogram of a grayscale image.

	// Use OpenCV to find all faces:
	face_cascade.detectMultiScale(Greypicture, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	for (int j = 0; j < faces.size(); ++j){ // We go through all the rectangles and paint them
		cv::Point corner1(faces[j].x, faces[j].y); // Add rectangles to picture
		cv::Point corner2((faces[j].x + faces[j].height), (faces[j].y + faces[j].width));
		cv::rectangle(picture, corner1, corner2, cv::Scalar(255, 105, 180), 2, 8, 0); // Draw the rectangles in pink color, with thickness 2 and type line 8 (continuous one). 
	}
	
	// Save (and show) picture with all detected faces
	mkdir("output",0700); // Create a folder, does nothing if folder already exists
	cv::imwrite("output/" + std::to_string(i) + "_Detected.jpg", picture);
	if(showAtEnd){
		cv::imshow("Detected",picture);
	}
}
