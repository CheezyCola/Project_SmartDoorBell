#ifndef MYVIDEOCAPTURE_H
#define MYVIDEOCAPTURE_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <QtNetwork>

#include <iostream>
#include <algorithm>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>

#include <ctime>
#include <unistd.h>
#include <stdlib.h>
#include <filesystem>
#include <cstdlib>
#include <stdio.h>

#include "LibCam_Wrapper.h"
#include "TMtCNN.h"
#include "TArcface.h"
#include "TRetina.h"
#include "TWarp.h"
#include "TLive.h"
#include "TBlur.h"
#include "Cloud_cURL.h"
#include "ThreadPool.h"
#include "FaceEmbedding.h"
#include "Controls.h"
#include "LED_UNIX_Socket.h"
#include "LiveStream.h"

/**
 * @brief A class to capture, process the camera frame and perform other functions\n
 * modified from https://github.com/Qengineering/Qt5-OpenCV-Raspberry-Pi-Jetson-Nano/blob/main/myvideocapture.h
 */
class MyVideoCapture : public QThread
{
    Q_OBJECT
public:
    ~MyVideoCapture();
    MyVideoCapture(QObject *parent = nullptr);
    /**
     * @brief pixmap: Get the pixel map
     * @return
     */
    QPixmap pixmap() const
    {
        return mPixmap;
    }
    /**
     * @brief GetHint: Get the hint message
     * @return
     */
    QString GetHint() const
    {
        return Hint;
    }
    void cleanup();
    //----------------------------------------------------------------------------------------
    // Build defines
    // uncomment them to turn a function off
    //----------------------------------------------------------------------------------------
    // operating options
    #define CAM_AUTOFOCUS           //comment if you want to use NON Auto focus instead (auto focus not available for PiCam 2 or lower)
    #define RETINA                  //comment if you want to use MtCNN landmark detection instead
    #define RECOGNIZE_FACE          //comment if you want to turn off Recognition functions
    #define ANTI_SPOOFING           //comment if you want to disable anti-spoofing
    #define REPORT_STRANGER         //comment if you don't want to report stranger
    #define BLUR_FILTER             //comment if you don't want to filter blur image
    // display options
    //#define SHOW_LEGEND             //comment if you don't want to show legend
    //#define SHOW_LANDMARKS          //comment if you don't want to show landmarks on detected face
    //----------------------------------------------------------------------------------------
    // Adjustable Parameters
    //----------------------------------------------------------------------------------------
    const int   MinHeightFace        = 70;
    const float MinFaceThreshold     = 0.90;
    const float MinMatchTreshold     = 0.391;   //classification threshold
    const float MinTrueFaceThreshold = 0.85;
    const double MaxBlur             = -7.0;    //more positive = sharper image
    const double MaxAngle            = 7.5;
    const float sleep                = 10000.0; // system go into energy saving mode after 10s of inactive (not detecting any face)
    //----------------------------------------------------------------------------------------
    // Paths to source, database, etc
    //----------------------------------------------------------------------------------------
    const std::string user = getlogin();
    const std::string src = "/home/" + user + "/source";
    const std::string database = src + "/" + "database";
    const std::string temp_img = src + "/" + "temp_img";
    //----------------------------------------------------------------------------------------
    // Server
    //----------------------------------------------------------------------------------------
    const std::string cloud = "https://cloudserver2-1-b8466856.deta.app";
    const std::string visitor_info_url = cloud + "/micro/postvisitorhistory/unprotected";
    const std::string visitor_img_url = cloud + "/micro/postvisitorhistoryimage/unprotected";
    const std::string stranger_purpose_url = cloud + "/micro/postrequeststatus/unprotected";
    const std::string stranger_img_url = cloud + "/micro/postrequestimage/unprotected";
    const std::string query_permission_url = cloud + "/micro/getrequeststatus/unprotected";
    const std::string constant_check_url = cloud + "/micro/constantcheck/unprotected";
    const std::string get_new_image_url = cloud + "/micro/getnewuserimage/unprotected";
    const std::string door_lock_status_url = cloud + "/micro/postdoorlockstatus/unprotected";
    const std::string cancel_request_url = cloud + "/micro/postcancelrequest/unprotected";
    const std::string faulty_image_url = cloud + "/micro/postfaultyimage/unprotected";
    const std::string image_backup_url = cloud + "/micro/getnewuserimagebackup/unprotected";
    //----------------------------------------------------------------------------------------
    // Camera parameters for libcamerasrc (gst pipeline & lccv)
    //----------------------------------------------------------------------------------------
    const int capture_width = 640; //1280 ;
    const int capture_height = 480; //720 ;
    const int framerate = 30;
    const int display_width = 640; //1280 ;
    const int display_height = 480; //720 ;
    //----------------------------------------------------------------------------------------
    // Globals
    //----------------------------------------------------------------------------------------
    const int   RetinaWidth      = 320;
    const int   RetinaHeight     = 240;
    const std::string database_jpg = database + "/*.jpg"; //image type
    float ScaleX, ScaleY;
    bool Run = true;
    bool ring = false;
    bool doorLockStatus = false;
    bool doorIsOpen = false;
    bool cancelRequest = false;
    bool ToRequest = false;
    std::string Request = "";
    Person_Encodings_vector per_enc_vec;
    QString ErrorMsg;
    bool Online = false;
    bool BtnEnabled = false;
    bool stream_request = false;
    bool showDBFunc = false;

Q_SIGNALS:
    /**
     * @brief newPixmapCapture: Update new frame
     */
    void newPixmapCapture();
    /**
     * @brief displayHint: Update hint message to display on the UI
     */
    void displayHint();
    /**
     * @brief EXIT: Call for exit
     */
    void EXIT();
    /**
     * @brief NotInRequest: Change UI back to non request state
     */
    void NotInRequest();
    /**
     * @brief OnlineMode: Update online mode
     */
    void OnlineMode();
    /**
     * @brief enableButtons: Update button state
     */
    void enableButtons();
    /**
     * @brief Streaming: Update streaming state
     */
    void Streaming();
    /**
     * @brief showDoorBellFunctions: Update whether to hide or show menu panel
     */
    void showDoorBellFunctions();

protected:
    /**
     * @brief run: Main function of the system
     */
    void run() override;

private:
    QNetworkAccessManager nam;
    QNetworkRequest req;
    QNetworkReply* reply = nam.get(req);
    QEventLoop Loop;

    QPixmap mPixmap;
    QString Hint;

    //------------------------------ Variables -----------------------------------------------------------
    cv::Mat Frame;
    cv::Mat GreyScale;
    float LightIntensity;
    int DefaultLightMode = 0;
    //constant checking handling, check for database update (add or delete user) and request for streaming
    int get_number = 0; //check update count to prevent data race
    int ans_number = 0;
    int Check_Ret = 1;
    std::string Folder;
    std::string Jpg;
    std::vector<std::string> userToAdd;
    std::vector<std::string> userToDelete;
    std::vector<std::string> userToAddTemp;
    std::vector<std::string> userToDeleteTemp;
    std::vector<std::string> new_img_paths; //paths of downloaded imgs
    std::string new_img_path;
    int deleted_face_num = 0;
    bool downloading_img = false;
    bool deleting_user = false;
    std::vector<int> removed_from_cloud;
    std::vector<int> removed_from_local;
    bool encodingToUpdate = false;
    bool encodingIsUpdate = false;
    bool encodingUpdating = false;
    bool STREAM = false; //live streaming handling
    bool STREAMING = false;
    int LightModeCheck = 0;
    std::vector<std::string> FaultyImages;
    int completeB4End = 0;

    //for downloaded img, to not mix with camera frame
    cv::Mat result_cnn1;
    vector<FaceObject> detected_Faces;
    cv::Mat img1;
    cv::Mat aligned1;
    TWarp Warp1;
    TRetina Rtn1;  //no Vulkan support on a RPi

    //Object pointers
    FaceEmbedding *F_Embedding = new FaceEmbedding();
    Controls *controls = new Controls();
    LiveStream *Stream = new LiveStream();
    Cloud_cURL *door_lock_status_curl = new Cloud_cURL(door_lock_status_url);
    Cloud_cURL *visitor_info_curl = new Cloud_cURL(visitor_info_url);
    Cloud_cURL *visitor_img_curl = new Cloud_cURL(visitor_img_url);
    Cloud_cURL *stranger_purpose_curl = new Cloud_cURL(stranger_purpose_url);
    Cloud_cURL *stranger_img_curl = new Cloud_cURL(stranger_img_url);
    Cloud_cURL *cancel_request_curl = new Cloud_cURL(cancel_request_url);
    Cloud_cURL *query_permission_curl = new Cloud_cURL(query_permission_url);
    Cloud_cURL *constant_check_curl = new Cloud_cURL(constant_check_url);
    Cloud_cURL *faulty_image_curl = new Cloud_cURL(faulty_image_url);

    chrono::steady_clock::time_point TPressed, TReleased;
    float pressedTime;
    //------------------------------ Variables -----------------------------------------------------------

    //--------------------------------- Subthreads (outer) -------------------------------------
    ThreadPool CheckMsg_th; //for constantly check of msg only (seperate from curl_th due to heavy usage)

    ThreadPool Image_th; //for saving and deleting img in local memory

    ThreadPool FcEnc_th; //for reading, and writing of Encodings.dat file in local (system)

    ThreadPool LED_th; //for internal socket communication to control LEDs

    ThreadPool Door_th; //for sending door status to server
    //--------------------------------- Subthreads (outer) -------------------------------------

#ifdef MP4_TESTRUN
    cv::VideoCapture Cap;
#else
#ifdef CAM_AUTOFOCUS
    lccv::PiCamera *Cam = new lccv::PiCamera();
#else
    std::string pipeline;
    cv::VideoCapture Cap;
#endif // CAM_AUTOFOCUS
#endif // MP4_TESTRUN

    //Functions
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    /**
     * @brief NowToString: Get the current date time
     * @return The date time in string
     */
    std::string NowToString();
    /**
     * @brief gstreamer_pipeline: Get the pipeline of Gstreamer setting (for camera)
     * @param capture_width: width of frame
     * @param capture_height: height of frame
     * @param framerate: framerate [Frame/sec]
     * @param display_width: width of frame display
     * @param display_height: height of frame display
     * @return gstreamer piple setting command
     */
    std::string gstreamer_pipeline(int capture_width, int capture_height, int framerate, int display_width, int display_height);
    /**
     * @brief CosineSimilarity: Calculate cosine similarity between 2 vectors
     * @param v1: vector of facial embeddings
     * @param v2: vector of facial embeddigns
     * @return float value of range betwwen -1 to 1
     */
    inline float CosineSimilarity(const cv::Mat &v1, const cv::Mat &v2);
    /**
     * @brief DrawObjects: Draw the frame with reactangle box, landmarks etc.
     * @param frame: frame to be drawn
     * @param Faces: face to be drawn
     */
    void DrawObjects(cv::Mat &frame, std::vector<FaceObject> &Faces);
    /**
     * @brief GetFolderImage: Get the folder(path) and the jpg name
     * @param img_path: image path stored in cloud
     * @param folder: folder path derived from image path
     * @param jpg_name: jpg name derived from image path
     */
    void GetFolderImage(std::string img_path, std::string &folder, std::string &jpg_name);
    /**
     * @brief setLEDs: Set the LEDs mode
     * @param Mode: Mode of LEDs (0-6)
     * @param delay_ms: The waiting time of socket before closing (default=100ms)
     */
    void setLEDs(int Mode, int delay_ms=100);
    /**
     * @brief doConsoleCMD: Run command at terminal
     * @param command: terminal command
     * @return -1, if error occurs
     */
    std::string doConsoleCMD(char* command);
    /**
     * @brief checkInternet: Check connectivity of internet and update the internet status
     */
    void checkInternet();
};

#endif // MYVIDEOCAPTURE_H
