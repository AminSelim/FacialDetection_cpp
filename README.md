---------------------------------------------------
Training and Facial Detection Software: README file
---------------------------------------------------


0. OpenCV:
----------
This software requires the OpenCV library V 2.4.11. "OpenCV is released under a BSD license and hence it’s free for both academic and commercial use. It has C++, C, Python and Java interfaces and supports Windows, Linux, Mac OS, iOS and Android." - www.opencv.org
Input the following commands in the terminal. 
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
cd ~/opencv
mkdir release
cd release
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make //This step might take some time. Make sure you have enough space on your computer. 
sudo make install

1. Training:
-------------
The positive and negative images were downloaded from the University of Illinois's computer science department's website. We used pictures of cars from the sideview to obtain our trainer. There were 550 poitive pictures and 500 negative pictures. https://cogcomp.cs.illinois.edu/Data/Car/
(a) We created a txt file named "cars.info" which included the path to each positive picture, the amount of objects (cars) in each picture, and the coordinates of the object. 
(b) We created a txt file named "bg.txt" which included the path to each negative picture. 
(c) opencv_createsamples -info cars.info -num 550 -w 48 -h 24 -vec cars.vec //opencv_createsamples is an OpenCV utility used to prepare a dataset of positive pictures. 
	(i)   num represents the number of positive pictures we have. 
	(ii)  w and h represent the width and height respectively. In the cars.info file we have specified them to be 100 and 40 respectively, however here we only take the relevant part of the picture. 
	(iii) cars.vec is the output file. 
(d) opencv_createsamples -vec cars.vec -w 48 -h 24 //This openCV utility is used to show the pictures used to create the .vec file. 
(e) Create an empty folder named "EmptyFolder"
(f) opencv_traincascade -data EmptyFolder -vec cars.vec -bg bg.txt -numPos 50 -numNeg 50 -numStages 10 -w 48 -h 24 -featureType LBP
This utility does the actual training. We tell the machine to do 10 trainings, each with 50 different positive and negative pictures using Local Binary Patterns (LBP). We couldn't use Viola-Jones (Haar) because it is exclusively for faces.  

**This particular training should be dealt with carefully since the dataset of pictures is really small and the number of stages is relatively low. A more accurate training would require thousands of pictures and between 12 - 13 stages. There are more details to prevent overtraining and other parameters that are not discussed here. 

2. Detection:
-------------
- Add Trainers:
	(i)   create a folder called "trainer"
	(ii)  go to opencv/data/haarscascades
	(iii) copy the 3 files named "Frontalface.xml", "Noses.xml" and "Profileface.xml" to the folder "trainer". (They are attached in the email.)
- To compile: g++ facedetec.cpp -o compiled3.exe -I /usr/local/include/ -L /usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_imgcodecs --std=c++14

