#include "myvideocapture.h"
#include <QDebug>

// streaming url
std::string LiveStream::server_url = "wss://tops-project.onrender.com/ws/video_stream";

/**
 * @brief Contructor to set up the environment, file location, server etc
 * @param parent is the QObject parent to this class, which is QThread
 */
MyVideoCapture::MyVideoCapture(QObject *parent)
    :QThread { parent },
      req(QUrl("http://www.google.com")),
      Rtn1(RetinaWidth, RetinaHeight, false)
{
    connect(reply, SIGNAL(finished()), &Loop, SLOT(quit()));

    Cloud_cURL::CURL_globalInit();

    Run = true;
    Door_th.Start();
    CheckMsg_th.Start();
    Image_th.Start();
    FcEnc_th.Start();
    LED_th.Start();
    Loop.exec();

    if (0 != system("xrandr --output DSI-1 --off"))
    {
        ErrorMsg =  "Failed to turn off display";
        Run = false;
    }

    // create if not exist
    std::string dir_cmd = "mkdir -p " + database;
    int dir_err = system(dir_cmd.c_str());
    if (-1 == dir_err){
        ErrorMsg = "Error creating directory - database";
        Run = false;
    }

    dir_cmd = "mkdir -p " + temp_img;
    dir_err = system(dir_cmd.c_str());
    if (-1 == dir_err){
        ErrorMsg = "Error creating directory - temp_img";
        Run = false;
    }

    if(controls->Initialize() != 0){
        ErrorMsg = "Error initializing GPIO pins";
        Run = false;
    }

    std::string internet = doConsoleCMD((char*)"cat /sys/class/net/wlan0/operstate");
    if (internet.find("up") != 0){
        system("sudo ifconfig wlan0 down");
        usleep(5000);
        system("sudo rfkill unblock wifi");
        usleep(5000);
        system("sudo ifconfig wlan0 up");
        usleep(5000);
        system("sudo systemctl restart wpa_supplicant.service");
        usleep(5000);
        system("sudo systemctl restart dhcpcd.service");
        usleep(5000);
        //ErrorMsg =  "Wifi service was down and restarted. Program is terminated";
        //Run = false;
    }

    try{
#ifdef CAM_AUTOFOCUS
        Cam->options->video_width=capture_width;
        Cam->options->video_height=capture_height;
        Cam->options->framerate=framerate;
#else
        pipeline = gstreamer_pipeline(capture_width, capture_height, framerate, display_width, display_height);
#endif // CAM_AUTOFOCUS
    }catch (std::exception &exp){
        ErrorMsg = QString("Error setting up camera - ") + exp.what();
        Run = false;
    }

    //clear leftover open socket by LED_server from last session if any
    setLEDs(-1, 500);

    if (0 != system("sudo ../LED_UNIX_Server/LED_UNIX_Server&"))
    {
        ErrorMsg = "Failed to open LED Server";
        Run = false;
    }

    usleep(100000); // wait LED server to open
}

/**
 * @brief Destructor of class deletes pointers to prevent memory leaks
 */
MyVideoCapture::~MyVideoCapture()
{
    // cleaning
    delete F_Embedding;
    F_Embedding = nullptr;

    delete controls;
    controls = nullptr;

    delete Stream;
    Stream = nullptr;

    delete Cam;
    Cam = nullptr;

    delete door_lock_status_curl;
    door_lock_status_curl = nullptr;
    delete visitor_info_curl;
    visitor_info_curl = nullptr;

    delete visitor_img_curl;
    visitor_img_curl = nullptr;

    delete stranger_purpose_curl;
    stranger_purpose_curl = nullptr;

    delete cancel_request_curl;
    cancel_request_curl = nullptr;

    delete query_permission_curl;
    query_permission_curl = nullptr;

    delete constant_check_curl;
    constant_check_curl = nullptr;

    delete faulty_image_curl;
    faulty_image_curl = nullptr;
}

/**
 * @brief Stop all threads and clear libcurl resources
 */
void MyVideoCapture::cleanup()
{
    Door_th.Stop();
    LED_th.Stop();
    FcEnc_th.Stop();
    Image_th.Stop();
    CheckMsg_th.Stop();
    Loop.quit();

    LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
    LED_socket->Stop();
    usleep(100000);
    delete LED_socket;
    LED_socket = nullptr;

    std::cout << "Killing sub threads..." << std::endl;

    int wait = 0;
    while(FcEnc_th.busy() || Image_th.busy()){
        if (wait == 0)
            std::cout << "Waiting for sub thread progress to end";
        std::cout << "..." << std::endl;
        wait ++;
        usleep(250000);
    }

    std::cout << "All sub threads killed." << std::endl;

    Cloud_cURL::CURL_globalClear();

    // remove temp dir if dir exist
    std::string dir_cmd = "rm -rf " + temp_img;
    const int dir_err = system(dir_cmd.c_str());
    if (-1 == dir_err){
        std::cerr << "failed to delete temp_img folder" << std::endl;
    }
}

/**
 * @brief Run the program main loop
 */
void MyVideoCapture::run()
{
    //seperate thread - check msg from cloud to update/delete user or perform live stream
    CheckMsg_th.QueueJob([&]{
        while(Run){
//            if (Online){
                try{
                    if (get_number == ans_number){
                        get_number++;
                        if (constant_check_curl->CURL_init()){
                            std::this_thread::sleep_for(std::chrono::milliseconds(150));
                            if(constant_check_curl->readMessage(get_number, ans_number, userToAdd, userToDelete, STREAM) != -1){
                                std::this_thread::sleep_for(std::chrono::milliseconds(150));
                                Check_Ret = 0;
                            }else{
                                std::cout << "[App constant check] Retry..." << std::endl;
                                delete constant_check_curl;
                                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                                constant_check_curl = new Cloud_cURL(constant_check_url);
                                std::this_thread::sleep_for(std::chrono::milliseconds(250));
                                ans_number = get_number;
                            }
                        }

                        if(ans_number == 2000000000){
                            get_number = 0;
                            ans_number = 0;
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    }
                }catch (std::exception &exp){
                    continue;
                }
//            }
        }
    });

    /*************************************************** PROGRAM MAIN LOOP ************************************************************/
    // continuously check for door bell, streaming and user update
    // if door bell is pressed, reacts according to its pressed time
    // short pressed - turn on recognition system with LCD
    // long pressed (>5 sec) - terminate program
    while (Run){
run_cycle:
        checkInternet();

        controls->CheckIsDoorOpen(doorIsOpen);
        if(Online && (doorIsOpen != !doorLockStatus)){
            Door_th.QueueJob([&]{
                doorLockStatus = !doorIsOpen;
                for(int i = 1; i <= 5; i++){
                    if (door_lock_status_curl->CURL_init()){
                        if(door_lock_status_curl->sendMsg("doorlockstatus", doorLockStatus) != -1){
                            break;
                        }
                    }
                    std::cout << "[door lock status] Retry..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            });
        }

        if(doorIsOpen){
            for(;;){
                controls->BeepSound();
                controls->CheckIsDoorOpen(doorIsOpen);
                if(Online && (doorIsOpen != !doorLockStatus)){
                    Door_th.QueueJob([&]{
                        doorLockStatus = !doorIsOpen;
                        for(int i = 1; i <= 5; i++){
                            if (door_lock_status_curl->CURL_init()){
                                if(door_lock_status_curl->sendMsg("doorlockstatus", doorLockStatus) != -1){
                                    break;
                                }
                            }
                            std::cout << "[door lock status] Retry..." << std::endl;
                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        }
                    });
                }
                if (!doorIsOpen){
                    controls->closeDoor();
                    controls->closeDoor();
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        controls->CheckIsDoorBellRing(ring);
        if (ring){
            TPressed = chrono::steady_clock::now();
            while(1){
                controls->CheckIsDoorBellRing(ring);
                if (!ring){
                    TReleased = chrono::steady_clock::now();
                    break;
                }
                usleep(150000); // 0.15s
            }
            pressedTime = chrono::duration_cast <chrono::milliseconds> (TReleased - TPressed).count();

            if (pressedTime >= 5000.0){
                /************************************ LONG PRESSED ************************************/
                std::cout << "Long pressed, terminating program" << std::endl;
                if (0 != system("xrandr --output DSI-1 --preferred"))
                {
                    ErrorMsg = "Failed to turn on display";
                    break; //to terminate the program
                }
                goto end; //to terminate the program
            }
            else if (pressedTime < 5000.0){
                /************************************ SHORT PRESSED ************************************/
                /************************************** VARIABLES **************************************/
                size_t i;
                float FPS[16];
                for(i=0;i<16;i++) FPS[i]=0.0;

                //----------------------------- variables for camera frame --------------------------------
                cv::Mat frame;
                cv::Mat greyScale;
                float lightIntensity;
                cv::Mat result_cnn;
                cv::Mat faces;
                cv::String NewItemName;
                cv::String KnownItemName;
                cv::String KnownCategory;
                cv::String StrangerJPG;
                std::vector<FaceObject> Faces; //detected faces
                std::vector<cv::Mat> fc1; //face encoding in database
                std::vector<cv::Mat> strfc1; //face encoding from camera for strangers

                //door request handling
                bool doorInRequest = false;
                bool requestIsSent = false;
                int request_number = 0; //initial request count to prevent data race
                std::pair<int, string> response = std::make_pair(request_number, " "); //query result of permission

                int reg_result = -99; //Steady frame result handling
                int reg_result_repeat_cnt = 0; //only action after steady frame result is achieved
                int steady_frame_cnt = 10;  //number of same frame recognition result to be considered as steady
                bool steady_outcome = false;  //eg: only when 10 frames produce 'stranger' result continuosly, system will take action
                //----------------------------- variables for camera frame --------------------------------

                //--------------------------- Obj variables of the AI networks ----------------------------
                TLive Live;
                Live.LoadModel();
                TWarp Warp;
                TMtCNN MtCNN;
                TArcFace ArcFace;
                TRetina Rtn(RetinaWidth, RetinaHeight, false);  //no Vulkan support on a RPi
                TBlur Blur;
                //--------------------------- Obj variables of the AI networks ----------------------------

                // Time
                chrono::steady_clock::time_point Tbegin, Tend, Tnofacebegin;
                bool inactive = false;
                float inactiveTime = 0.0; // no face
                //------------------------------- Subthreads (inner) ---------------------------------------
                ThreadPool curl_th; //for cloud communication - general
                curl_th.Start();

                ThreadPool visit_th; //for cloud communication - visitor info
                visit_th.Start();

                ThreadPool permi_th; //for cloud communication - permission request
                permi_th.Start();

                ThreadPool Ctrl_th; //for controlling reading and writing GPIO pins
                Ctrl_th.Start();
                //------------------------------- Subthreads (inner) ---------------------------------------

                /************************************** PRINTING SYSTEM INFO **************************************/
                //OpenCV Version
                std::cout << "OpenCV Version: " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << "."
                    << CV_SUBMINOR_VERSION << std::endl;
                std::cout << " " << std::endl;

#ifdef RECOGNIZE_FACE
                std::cout << "Recognize Face " << std::endl;
                std::cout << " " << std::endl;
#ifdef RETINA
                std::cout << "Using Retina" << std::endl;
                std::cout << " " << std::endl;
#else
                std::cout << "Using MtCNN" << std::endl;
                std::cout << " " << std::endl;
#endif // RETINA
#ifdef BLUR_FILTER
                std::cout << "Blur filter is ON" << std::endl;
                std::cout << " " << std::endl;
#endif // BLUR_FILTER
#ifdef ANTI_SPOOFING
                std::cout << "Anti-Spoofing is ON" << std::endl;
                std::cout << " " << std::endl;
#endif // ANTI_SPOOFING
#ifdef REPORT_STRANGER
                std::cout << "System will report stranger" << std::endl;
                std::cout << " " << std::endl;
#endif // REPORT_STRANGER
#endif // RECOGNIZE_FACE

#ifdef CAM_AUTOFOCUS
                std::cout << "Auto Focus is ON" << std::endl;
                std::cout << " " << std::endl;
#else
                std::cout << "Auto Focus is OFF" << std::endl;
                std::cout << " " << std::endl;
#endif // CAM_AUTOFOCUS

                /****************************************** SYSTEM ON ********************************************/
                Q_EMIT enableButtons();
                usleep(10000);
                Q_EMIT showDoorBellFunctions();

                Ctrl_th.QueueJob([&]{
                    controls->OnMusic();
                });

                setLEDs(4, 600);
                setLEDs(5, 600);
                setLEDs(6, 600);

                setLEDs(DefaultLightMode);

                if (0 != system("xrandr --output DSI-1 --preferred"))
                {
                    ErrorMsg = "Failed to turn on display";
                    break;
                }

                int wait = 0;
                while (encodingUpdating){
                    if (LED_UNIX_Socket::lightMode != 5){
                        LED_th.QueueJob([&]{
                            setLEDs(5);
                        });
                    }
                    usleep(250000);
                    if (wait == 0){
                        Hint = "Waiting for database update to complete...";
                        Q_EMIT displayHint();
                    }
                    wait ++;
                }

                while (encodingToUpdate){
                    if (LED_UNIX_Socket::lightMode != 5){
                        LED_th.QueueJob([&]{
                            setLEDs(5);
                        });
                    }
                    usleep(250000);
                    if (wait == 0){
                        Hint = "Waiting for database update to complete...";
                        Q_EMIT displayHint();
                    }
                    wait ++;
                }

                if (Online){
                    while (downloading_img || deleting_user){
                        if (LED_UNIX_Socket::lightMode != 5){
                            LED_th.QueueJob([&]{
                                setLEDs(5);
                            });
                        }
                        usleep(250000);
                        if (wait == 0){
                            Hint = "Waiting for database update to complete...";
                            Q_EMIT displayHint();
                        }
                        wait ++;
                    }
                }

                if (!encodingToUpdate && !encodingUpdating){
                    Hint = "Loading saved Encodings";
                    Q_EMIT displayHint();
                    if (F_Embedding->loadEmbeddings(per_enc_vec) == -1){
                        // Failed to load encodings from file
                        Hint = "Extracting Encodings from database";
                        Q_EMIT displayHint();
                        if (F_Embedding->getEmbeddings(database_jpg, per_enc_vec) != -1){
                            // extracted encodings from database
                            Hint = "Saving encodings";
                            Q_EMIT displayHint();
                            if (F_Embedding->saveEmbeddings(per_enc_vec) != -1){
                                // save encodings
                                Hint = "Encodings saved";
                                Q_EMIT displayHint();
                            }
                        }
                    }
                }

                if (!STREAMING){ //if not in streaming mode, turn on cam
                    try{
#ifdef CAM_AUTOFOCUS
                        Hint = "Turning on Camera...";
                        Q_EMIT displayHint();
                        if(!Cam->startVideo()){
                            std::cerr << "ERROR: Unable to open the camera" << std::endl;
                            break;
                        }
#else
                        Cap(pipeline, cv::CAP_GSTREAMER); // (only for libcamerasrc, not raspicam)
                        if (!Cap.isOpened()){
                            std::cerr << "ERROR: Unable to open the camera" << std::endl;
                            break;
                        }
#endif // CAM_AUTOFOCUS
                        STREAMING = true; //indicate camera on
                    }catch (std::exception &exp){
                        ErrorMsg = QString("Error to open camera or load video - ") + exp.what();
                        break; // to terminate program
                    }
                }

                usleep(10000);

#ifdef CAM_AUTOFOCUS
                if(!Cam->getVideoFrame(frame,1000)){
                    std::cerr << "Timeout error" << std::endl;
                    break; // to terminate program
                }
                // get light exposure and control light source
                cv::cvtColor(frame, greyScale, cv::COLOR_BGR2GRAY);
                lightIntensity = cv::mean(greyScale)[0];
                if (lightIntensity<5 && DefaultLightMode != 1){
                    //lightMode = 1
                    setLEDs(1);
                }
                else if (lightIntensity<50 && DefaultLightMode != 2){
                    //lightMode = 2
                    setLEDs(2);
                }else if (lightIntensity<105 && DefaultLightMode != 3){
                    //lightMode = 3
                    setLEDs(3);
                }
                else if (DefaultLightMode != 0){
                    setLEDs(0);
                }

#else
                Cap >> frame; //shift right 4 bits?
                if (frame.empty()) {
                    cerr << "End of video" << endl;
                    break; // to terminate program
                }
#endif // CAM_AUTOFOCUS

                Hint = "Hello!";
                Q_EMIT displayHint();

                std::cout << "Start grabbing frames" << std::endl;
                bool skip_frame = false;
                for(;;){
                    if (skip_frame){
                        skip_frame = false;
                        continue;
                    }

                    checkInternet();

                    controls->CheckIsDoorOpen(doorIsOpen); //dockLockStatus is true if door is open
                    if (doorIsOpen){
                        break; // break to while(Run) loop
                    }

                    if (ring){
                        Ctrl_th.QueueJob([&]{
                            controls->DoorBell();
                            QString temp = Hint;
                            Hint = "Door Bell is rang.";
                            Q_EMIT displayHint();
                            usleep(500000);
                            Hint = temp;
                            Q_EMIT displayHint();
                        });
                        ring = false;
                    }

                    try{
#ifdef CAM_AUTOFOCUS
                        if(!Cam->getVideoFrame(frame,1000)){
                            std::cerr << "Timeout error" << std::endl;
                            frame = cv::Mat(capture_height, capture_width, CV_8UC3, cv::Scalar(0,0,0));
                        }
#else
                        Cap >> frame; //shift right 4 bits?
                        if (frame.empty()) {
                            cerr << "End of video" << endl;
                            frame = cv::Mat(capture_height, capture_width, CV_8UC3, cv::Scalar(0,0,0));
                        }
#endif // CAM_AUTOFOCUS

                        cv::rotate(frame, frame, cv::ROTATE_180);   // rotate the frame 180 degree due to camera upside down positioning
                        ScaleX = ((float) frame.cols) / RetinaWidth;
                        ScaleY = ((float) frame.rows) / RetinaHeight;
                        cv::resize(frame, result_cnn, cv::Size(RetinaWidth,RetinaHeight)); // copy/resize image to result_cnn as input tensor
                        Tbegin = chrono::steady_clock::now();

#ifdef RETINA
                        Rtn.detect_retinaface(result_cnn,Faces);
#else
                        MtCNN.detect(result_cnn,Faces);
#endif // RETINA

#ifdef RECOGNIZE_FACE
                        //reset indicators (too many faces)
                        for(i=0;i<Faces.size();i++){
                            Faces[i].NameIndex = -4;
                            Faces[i].Color     = 4;
                            Faces[i].NameProb  = 0.0;
                            Faces[i].LiveProb  = 0.0;
                            Faces[i].Angle  = 0.0;
                        }

                        //run recognition when only one person capture and when database encoding not updating
                        //an input image of 324x240 means too tiny
                        if(Faces.size()==1 && !encodingToUpdate && !encodingUpdating && !doorInRequest){
                            if (inactive) inactive = false;
                            //looks stupid, running through a loop of size 1
                            //however, for your convenience using [i]
                            for(i=0;i<Faces.size();i++){
                                if(Faces[i].FaceProb < MinFaceThreshold){
                                    Faces[i].NameIndex = -3; //fake
                                    Faces[i].Color     =  3;
                                    break; // no face
                                }
                                if(Faces[i].rect.height > MinHeightFace){
                                    //a good size face is seen
                                    cv::Mat aligned = Warp.Process(result_cnn,Faces[i]); //get centre aligned image
                                    Faces[i].Angle  = Warp.Angle;
                                    //extract features of camera face
                                    cv::Mat fc2 = ArcFace.GetFeature(aligned);

#ifdef BLUR_FILTER
                                    // reject low quality blur or askew face
                                    double blur = 0.0;
                                    blur=Blur.Execute(aligned);
                                    if(Faces[i].Angle>=MaxAngle && blur<=MaxBlur){
                                        Faces[i].NameIndex = -5;
                                        Faces[i].Color     =  5;
                                        break;
                                    }
#endif // BLUR_FILTER
                                    //calculate the similarity score
                                    if(per_enc_vec.size()>0){
                                        std::vector<double> score; //average score of images
                                        for(size_t c=0; c<per_enc_vec.size(); c++){
                                            //cout << per_enc_vec[c].name << endl;
                                            //cout << per_enc_vec[c].category << endl;
                                            //cout << per_enc_vec[c].Encodings.size() << endl;
                                            double _score = 0.0;
                                            for (size_t f=0; f<per_enc_vec[c].Encodings.size(); f++){
                                                _score += CosineSimilarity(per_enc_vec[c].Encodings[f], fc2);

                                            }
                                            _score /= (double)per_enc_vec[c].Encodings.size();
                                            //                            cout << _score << endl;
                                            score.push_back(_score);
                                        }

                                        int Pmax = max_element(score.begin(),score.end()) - score.begin();
                                        Faces[i].NameIndex = Pmax;
                                        Faces[i].NameProb  = score[Pmax];
                                        score.clear();
                                        if(Faces[i].NameProb >= MinMatchTreshold){
                                            //recognize a face
                                            Faces[i].Color = 0; //found face in database
                                        }
                                        else{
                                            //stranger
                                            Faces[i].NameIndex = -1;    //a stranger
                                            Faces[i].Color     =  1;
                                        }
                                    }
                                    else{
                                        Faces[i].NameIndex = -1;    //all stranger since no person in db
                                        Faces[i].Color     =  1;
                                    }

#ifdef ANTI_SPOOFING
                                    if (Faces[i].Color <2){
                                        //test true or fake face for only stranger or known face
                                        float x1 = Faces[i].rect.x;
                                        float y1 = Faces[i].rect.y;
                                        float x2 = Faces[i].rect.width+x1;
                                        float y2 = Faces[i].rect.height+y1;
                                        struct LiveFaceBox LiveBox={x1,y1,x2,y2};
                                        Faces[i].LiveProb=Live.Detect(result_cnn,LiveBox);
                                        if(Faces[i].LiveProb<=MinTrueFaceThreshold){
                                            Faces[i].Color     =  3; //fake
                                            Faces[i].NameIndex = -3;
                                            std::cout << "Fake Face detected" << std::endl;
                                        }
                                    }
#endif // ANTI_SPOOFING
                                    // Action on a steady known face
                                    if (Faces[i].Color == 0 && steady_outcome){
                                        KnownItemName = per_enc_vec[Faces[i].NameIndex].name;
                                        KnownCategory = per_enc_vec[Faces[i].NameIndex].category;
                                        imwrite(temp_img + "/" + KnownItemName + ".jpg", frame);
                                        if(Online){
                                            visit_th.QueueJob([&]{
                                                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // wait for img to save
                                                if (visitor_info_curl->CURL_init()){
                                                    visitor_info_curl->sendMsg("datetime", NowToString(), "name", KnownItemName, "category", KnownCategory);
                                                }
                                                for(int i=0; i<3; i++){
                                                    if (visitor_img_curl->CURL_init()){
                                                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                                                        if(visitor_img_curl->sendPhoto(temp_img + "/" + KnownItemName + ".jpg") != -1){
                                                            Hint = "Visitor Image recorded!";
                                                            Q_EMIT displayHint();
                                                            break;
                                                        }
                                                    }
                                                    if(i==2){
                                                        Hint = "Failed to send known person image!";
                                                        Q_EMIT displayHint();
                                                        delete visitor_img_curl;
                                                        visitor_img_curl = new Cloud_cURL(visitor_img_url);
                                                    }
                                                    std::cout << "[Known visitor img] Retry..." << std::endl;
                                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                }
                                            });
                                        }
                                        Hint = "Unlocking Door...";
                                        Q_EMIT displayHint();
                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));

                                        Ctrl_th.QueueJob([&]{
                                            setLEDs(4, 500);
                                            controls->openDoor();
                                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                            controls->openDoor();
                                        });
                                    }

                                    if (Faces[i].NameIndex == -1 && steady_outcome){
                                        //check if the stranger in temporary stranger database
                                        //avoid to report the same face multiple times
                                        if (strfc1.size()>0){
                                            vector<double> score2_;
                                            for(size_t f=0;f<strfc1.size();f++) score2_.push_back(CosineSimilarity(strfc1[f], fc2));
                                            int Pmax2 = max_element(score2_.begin(),score2_.end()) - score2_.begin();

                                            if(score2_[Pmax2] >= MinMatchTreshold){
                                                //found face in temporary stranger database
                                                std::cout << "Stranger face was seen" << std::endl;
                                                Faces[i].NameIndex = -6;
                                                Faces[i].Color = 6;
                                            }
                                        }
                                        strfc1.push_back(fc2);

#ifdef REPORT_STRANGER
                                        if (Faces[i].NameIndex == -1){
                                            StrangerJPG = "Stranger_" + NowToString() + ".jpg";
                                            imwrite(temp_img + "/" + StrangerJPG, frame);
                                            if(Online){
                                                visit_th.QueueJob([&]{
                                                    std::this_thread::sleep_for(std::chrono::milliseconds(500)); //wait for image to save
                                                    if (visitor_info_curl->CURL_init()){
                                                        visitor_info_curl->sendMsg("datetime", NowToString(), "name", StrangerJPG , "category", "Stranger");
                                                    }
                                                    for(int i=0; i<3; i++){
                                                        if (visitor_img_curl->CURL_init()){
                                                            std::this_thread::sleep_for(std::chrono::milliseconds(250));
                                                            if(visitor_img_curl->sendPhoto(temp_img + "/" + StrangerJPG) != -1){
                                                                Hint = "Stranger Image recorded!";
                                                                Q_EMIT displayHint();
                                                                break;
                                                            }
                                                        }
                                                        if(i==2){
                                                            Hint = "Failed to send stranger image!";
                                                            Q_EMIT displayHint();
                                                            delete visitor_img_curl;
                                                            visitor_img_curl = new Cloud_cURL(visitor_img_url);
                                                            strfc1.pop_back();
                                                        }
                                                        std::cout << "[Stranger visitor img] Retry..." << std::endl;
                                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                    }
                                                });
                                            }
                                            Ctrl_th.QueueJob([&]{
                                                controls->AlertSound();
                                            });

                                            if(showDBFunc != true){
                                                showDBFunc = true;
                                                Q_EMIT showDoorBellFunctions();
                                            }

                                            if(BtnEnabled != true){
                                                BtnEnabled = true;
                                                Q_EMIT enableButtons();
                                            }
                                        }
#endif // REPORT_STRANGER
                                    }

                                    if(Online){
                                        if (ToRequest && !requestIsSent && !doorInRequest){
                                            Hint = "Show your face clearly to capture request image";
                                            Q_EMIT displayHint();
                                            if ((Faces[i].NameIndex == -1 || Faces[i].NameIndex == -6) && steady_outcome){
                                                doorInRequest = true;
                                                response.second = "";

                                                Hint = "Face is captured! Sending request...";
                                                Q_EMIT displayHint();

                                                imwrite(temp_img +"/request.jpg", frame);

                                                LED_th.QueueJob([&]{
                                                    setLEDs(6);
                                                });

                                                permi_th.QueueJob([&]{
                                                    while(visit_th.busy()){
                                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                    }
                                                    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // wait for img to save
                                                    if (stranger_purpose_curl->CURL_init()){
                                                        stranger_purpose_curl->sendMsg("datetime", NowToString(), "purpose", Request, "status", " ");
                                                    }
                                                    for(int i=0; i<=2; i++){
                                                        if (stranger_img_curl->CURL_init()){
                                                            std::this_thread::sleep_for(std::chrono::milliseconds(250));
                                                            if(stranger_img_curl->sendPhoto(temp_img+"/request.jpg") != -1){
                                                                requestIsSent = true;
                                                                break;
                                                            }
                                                        }
                                                        if(i==2){
                                                            Hint = "Failed to send request image!";
                                                            Q_EMIT displayHint();
//                                                            Q_EMIT NotInRequest();
//                                                            cancelRequest = false;
//                                                            doorInRequest = false;
                                                            requestIsSent = true;
                                                            delete stranger_img_curl;
                                                            stranger_img_curl = new Cloud_cURL(stranger_img_url);
                                                        }
                                                        else{
                                                            std::cout << "[Permission req] Retry..." << std::endl;
                                                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                        }
                                                    }
                                                });
                                            }
                                        }// if ToRequest && !requestIsSent && !doorInRequest
                                    }// if Online
                                }else{
                                    Faces[i].Color = 2; //too tiny
                                    Faces[i].NameIndex = -2;
                                }

                                if (reg_result == (int)Faces[i].NameIndex)
                                    reg_result_repeat_cnt += 1;
                                else
                                    reg_result_repeat_cnt = 0;  // set back to 0 when difference reg_result is seen

                                if (reg_result_repeat_cnt == steady_frame_cnt){
                                    steady_outcome = true;
                                    reg_result_repeat_cnt = 0;
                                }else
                                    steady_outcome = false;

                                reg_result = Faces[i].NameIndex;
                            }
                        }
                        else{
                            reg_result = -99;
                            if (Faces.size()==0){
                                reg_result_repeat_cnt = 0;
                                if(!inactive){
                                    Tnofacebegin = chrono::steady_clock::now();
                                    inactive = true;
                                }else if (!doorInRequest && !encodingToUpdate && !encodingUpdating){
                                    inactiveTime = chrono::duration_cast <chrono::milliseconds> (chrono::steady_clock::now() - Tnofacebegin).count();
                                }else{
                                    inactive = false;
                                    inactiveTime = 0;
                                }
                            }
                        }
                    }catch (std::exception &exp){
                        ErrorMsg = QString("Frame processing error - ") + exp.what();
                        goto end; //break nested loop to terminate program
                    }

#endif // RECOGNIZE_FACE

                    Tend = chrono::steady_clock::now();

                    if (!encodingToUpdate && !encodingUpdating && !doorInRequest){
                        DrawObjects(frame, Faces); //Do not show recognition result since no recognition when updating database
                        if (inactiveTime > sleep){
                            Hint = "System is going to sleep...";
                            Q_EMIT displayHint();
                            usleep(1000000);
                            break;
                        }
                    }

                    //calculate frame rate
                    //                    f = chrono::duration_cast <chrono::milliseconds> (Tend - Tbegin).count();
                    //                    if(f>0.0) FPS[((Fcnt++)&0x0F)]=1000.0/f;
                    //                    for(f=0.0, i=0;i<16;i++){ f+=FPS[i]; }
                    //                    cv::putText(frame, cv::format("FPS %0.2f", f/16),cv::Point(10,20),cv::FONT_HERSHEY_SIMPLEX,0.6, cv::Scalar(180, 180, 0));

                    mPixmap = cvMatToQPixmap(frame);
                    Q_EMIT newPixmapCapture();

                    //streaming
                    if (STREAM){
                        Stream->stream(frame);
                    }

                    skip_frame = true; //skip one frame after each processed frame to save time

                    LightModeCheck ++; //increase with each processed frame

                    if (LightModeCheck >= 20 && !doorInRequest && !encodingUpdating && !encodingToUpdate){
                        LightModeCheck = 0;
                        LED_th.QueueJob([&]{
                            cv::cvtColor(frame, greyScale, cv::COLOR_BGR2GRAY);
                            float temp = cv::mean(greyScale)[0];
                            if (!(lightIntensity < temp + 60 && lightIntensity > temp - 30)){
                                lightIntensity = temp;
                                if (lightIntensity<5 && DefaultLightMode != 1){
                                    //lightMode = 1
                                    setLEDs(1);
                                }
                                else if (lightIntensity<50 && DefaultLightMode != 2){
                                    //lightMode = 2
                                    setLEDs(2);
                                }else if (lightIntensity<105 && DefaultLightMode != 3){
                                    //lightMode = 3
                                    setLEDs(3);
                                }
                                else if (DefaultLightMode != 0){
                                    setLEDs(0);
                                }
                            }
                        });
                    }

                    if(Online){
                        try{
                            // entry permission request
                            if (requestIsSent){
                                if (LED_UNIX_Socket::lightMode != 6){ //if not white cycle, run it
                                    LED_th.QueueJob([&]{
                                        setLEDs(6);
                                    });
                                }

                                if (cancelRequest){
                                    cancelRequest = false;
                                    request_number = 0;
                                    curl_th.QueueJob([&]{
                                        for(;;){
                                            if (cancel_request_curl->CURL_init()){
                                                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                                if(cancel_request_curl->sendMsg("cancelrequest", false) != -1){ //send false to cancel request
                                                    ToRequest = false;
                                                    doorInRequest = false;
                                                    requestIsSent = false;
                                                    response.first = 0;
                                                    Q_EMIT NotInRequest();
                                                    Hint = "Hello!";
                                                    Q_EMIT displayHint();
                                                    LED_th.QueueJob([&]{
                                                        setLEDs(DefaultLightMode);
                                                    });
                                                    break;
                                                }
                                            }
                                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                            std::cout << "[Cancel request] Retry..." << std::endl;
                                        }
                                    });
                                }else if (response.second == "\"n/a\"" || response.second == "\"allow\"" || response.second == "\"deny\"" || response.second == "\"connection lost\""){
                                    request_number = 0;
                                    ToRequest = false;
                                    cancelRequest = false;
                                    requestIsSent = false;
                                    doorInRequest = false;

                                    Q_EMIT NotInRequest();

                                    if (response.second == "\"n/a\""){
                                        Hint = "No response from Home Owner";
                                        Q_EMIT displayHint();
                                        response.first = 0;
                                        LED_th.QueueJob([&]{
                                            setLEDs(DefaultLightMode);
                                        });
                                    }else if (response.second == "\"allow\""){
                                        Hint = "Access allowed";

                                        Q_EMIT displayHint();
                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));

                                        Ctrl_th.QueueJob([&]{
                                            setLEDs(4, 500);
                                            controls->openDoor();
                                            std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                            controls->openDoor();
                                        });
                                    }else if (response.second == "\"deny\""){
                                        Hint = "Access denied";
                                        Q_EMIT displayHint();
                                        response.first = 0;
                                        LED_th.QueueJob([&]{
                                            setLEDs(DefaultLightMode);
                                        });
                                    }else if (response.second == "\"connection lost\""){
                                        Hint = "Connection lost";
                                        Q_EMIT displayHint();
                                        response.first = 0;
                                        get_number = 0;
                                        ans_number = 0;
                                        LED_th.QueueJob([&]{
                                            setLEDs(DefaultLightMode);
                                        });
                                    }
                                }else if (request_number == response.first){
                                    if (Hint != "Request sent. Waiting for reply..."){
                                        Hint = "Request sent. Waiting for reply...";
                                        Q_EMIT displayHint();
                                    }
                                    request_number++;
                                    permi_th.QueueJob([&]{
                                        for(int i=0; i<=3; i++){
                                            if (query_permission_curl->CURL_init()){
                                                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                                if (query_permission_curl->readRequestStatus(request_number, response) != -1){
                                                    std::this_thread::sleep_for(std::chrono::milliseconds(350));
                                                    break;
                                                }
                                            }
                                            std::this_thread::sleep_for(std::chrono::milliseconds(350));
                                            std::cout << "[Permission response] Retry..." << std::endl;
                                        }
                                    });
                                    std::cout << "Number of query to permission status: " << response.first << std::endl;
                                }
                            }
                        }catch (std::exception &exp){
                            ErrorMsg = QString("Cloud request handling error - ") + exp.what();
                            goto end; //break nested loop to terminate program
                        }

                        try{
                            /*********************************************** CHECK MSG FROM CLOUD (USER) AND HANDLE *********************************************************/
                            if (stream_request != STREAM){
                                stream_request = STREAM;
                                Q_EMIT Streaming();
                            }

                            if(Check_Ret == 0 && !downloading_img && !deleting_user && !encodingToUpdate && !encodingUpdating){
                                Check_Ret = 1;
                                userToAddTemp = userToAdd;
                                userToDeleteTemp = userToDelete;
                            }

                            if (!userToAddTemp.empty() && !Image_th.busy() && !downloading_img && !deleting_user && !encodingToUpdate && !encodingUpdating){
                                downloading_img = true;
                                std::cout << "Downloading image(s)..." << std::endl;
                                std::cout << "=========== Recognition paused for database update ==========" << std::endl;
                                Hint = "Recognition paused for database update...";
                                Q_EMIT displayHint();
                                BtnEnabled = false;
                                Q_EMIT enableButtons();
                                if(LED_UNIX_Socket::lightMode != 5){
                                    setLEDs(5);
                                }

                                removed_from_cloud.clear();
                                new_img_paths.clear();
                                Image_th.QueueJob([&]{
                                    for (size_t u=0; u<userToAddTemp.size(); u++){
                                        Cloud_cURL *new_image_curl = new Cloud_cURL(get_new_image_url + "/" + userToAddTemp[u]);
                                        if (new_image_curl->CURL_init()){
                                            std::this_thread::sleep_for(std::chrono::milliseconds(250));
                                            if (new_image_curl->downloadPhoto(database, new_img_path) == -1){
                                                delete new_image_curl;
                                                new_image_curl = nullptr;
                                                for(int i = 1; i <= 5; i++){
                                                    Cloud_cURL *image_backup_curl = new Cloud_cURL(image_backup_url + "/" + userToAddTemp[u]);
                                                    if (image_backup_curl->CURL_init()){
                                                        if (image_backup_curl->downloadPhoto(database, new_img_path) != 1){
                                                            delete image_backup_curl;
                                                            image_backup_curl = nullptr;
                                                            removed_from_cloud.push_back(u);
                                                            new_img_paths.push_back(new_img_path);
                                                            break;
                                                        }
                                                        delete image_backup_curl;
                                                        image_backup_curl = nullptr;
                                                    }
                                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                    std::cout << "[App backup img] Retry..." << std::endl;
                                                }
                                            }else{
                                                delete new_image_curl;
                                                new_image_curl = nullptr;
                                                removed_from_cloud.push_back(u);
                                                new_img_paths.push_back(new_img_path);
                                            }
                                        }
                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                    }

                                    for (size_t r=0; r<removed_from_cloud.size(); r++){
                                        userToAddTemp.erase(userToAddTemp.begin() + removed_from_cloud[r]);
                                    }

                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                    downloading_img = false;
                                    encodingToUpdate = true;
                                    std::cout<< "end of downloading" << std::endl;
                                });
                            }

                            if (!userToDeleteTemp.empty() && !Image_th.busy() && !downloading_img && !deleting_user && !encodingToUpdate && !encodingUpdating){
                                deleting_user = true;
                                std::cout << "Deleting user(s)..." << std::endl;
                                std::cout << "=========== Recognition paused for database update ==========" << std::endl;
                                Hint = "Recognition paused for database update...";
                                Q_EMIT displayHint();
                                BtnEnabled = false;
                                Q_EMIT enableButtons();
                                if(LED_UNIX_Socket::lightMode != 5){
                                    setLEDs(5);
                                }

                                removed_from_local.clear();
                                Image_th.QueueJob([&]{
                                    for (size_t u=0; u<userToDeleteTemp.size(); u++){
                                        try{
                                            std::filesystem::remove_all((database + "/" + userToDeleteTemp[u]).c_str());
                                        }
                                        catch(const std::exception &e){
                                            std::cout << "e.what" << std::endl;
                                            continue;
                                        }
                                        removed_from_local.push_back(u);
                                        deleted_face_num ++;
                                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                    }

                                    for (size_t r=0; r<removed_from_local.size(); r++){
                                        userToDeleteTemp.erase(userToDeleteTemp.begin() + removed_from_local[r]);
                                    }

                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                    encodingToUpdate = true;
                                    deleting_user = false;
                                    std::cout<< "end of deleting" << std::endl;
                                });
                            }

                            if (!new_img_paths.empty() && !encodingUpdating && encodingToUpdate && !FcEnc_th.busy()){
                                encodingUpdating = true;
                                FaultyImages.clear();
                                std::cout << "Checking images..." << std::endl;

                                int goodface_num = 0; //to store number of accepted img

                                FcEnc_th.QueueJob([&]{
                                    for (size_t u=0; u<new_img_paths.size(); u++){
                                        std::cout << new_img_paths[u] << std::endl;
                                        img1 = cv::imread(new_img_paths[u], 1);
                                        if (img1.empty()){
                                            std::cout << "Could not open or read the input image" << std::endl;
                                        }
                                        else{
                                            //extract
                                            ScaleX = ((float) img1.cols) / RetinaWidth;
                                            ScaleY = ((float) img1.rows) / RetinaHeight;
                                            // copy/resize image to result_cnn as input tensor
                                            cv::resize(img1, result_cnn1, cv::Size(RetinaWidth,RetinaHeight),cv::INTER_LINEAR);
                                            //get the face
                                            Rtn1.detect_retinaface(result_cnn1,detected_Faces);
                                        }
                                        //only one face per picture
                                        if(detected_Faces.size()==1){
                                            if(detected_Faces[0].FaceProb>MinFaceThreshold && detected_Faces[0].rect.height > MinHeightFace){
                                                //get centre aligned image
                                                aligned1 = Warp1.Process(result_cnn1,detected_Faces[0]);
                                                std::remove(new_img_paths[u].c_str());
                                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                cv::imwrite(new_img_paths[u], aligned1);  //replace
                                                goodface_num ++;
                                                std::cout << "Added " << new_img_paths[u] << std::endl;
                                            }
                                            else{
                                                std::cout << "No human face found or face is too small" << std::endl;
                                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                std::remove(new_img_paths[u].c_str());
                                                GetFolderImage(new_img_paths[u], Folder, Jpg);
                                                FaultyImages.push_back(Jpg+"-"+Folder+".jpg");
                                                std::cout << "Deleted" << std::endl;
                                            }
                                        }
                                        else{
                                            if (detected_Faces.size()==0)
                                                std::cout << "No face is found" << std::endl;
                                            if (detected_Faces.size()>1)
                                                std::cout << "More than 1 Face are seen" << std::endl;
                                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                            std::remove(new_img_paths[u].c_str());
                                            GetFolderImage(new_img_paths[u], Folder, Jpg);
                                            FaultyImages.push_back(Jpg+"-"+Folder+".jpg");
                                            std::cout << "Deleted" << std::endl;
                                        }

                                        if (std::filesystem::is_empty(new_img_paths[u].substr(0,new_img_paths[u].find_last_of('/'))))
                                            std::filesystem::remove(new_img_paths[u].substr(0,new_img_paths[u].find_last_of('/')));
                                    }

                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                                    if(!FaultyImages.empty()){
                                        for(int i=0; i<10; i++){
                                            if (faulty_image_curl->CURL_init()){
                                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                                if(faulty_image_curl->sendFaultyImage(FaultyImages) != -1){
                                                    break;
                                                }
                                            }
                                            std::cout << "[App Faulty img] Retry..." << std::endl;
                                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                        }
                                    }

                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));

                                    if(goodface_num > 0){
                                        std::cout << "Updating encoding of images added .........." << std::endl;
                                        // Update Encoding
                                        Hint = "Extracting Encodings from database";
                                        Q_EMIT displayHint();
                                        if (F_Embedding->getEmbeddings(database_jpg, per_enc_vec) != -1){
                                            // extracted encodings from database
                                            Hint = "Saving encodings";
                                            Q_EMIT displayHint();
                                            if (F_Embedding->saveEmbeddings(per_enc_vec) != -1){
                                                // save encodings
                                                Hint = "Encodings saved";
                                                Q_EMIT displayHint();
                                            }
                                        }
                                    }
                                    new_img_paths.clear();
                                    encodingToUpdate = false;
                                    encodingUpdating = false;
                                    encodingIsUpdate = true;
                                });
                            }

                            if (deleted_face_num > 0 && !encodingUpdating && encodingToUpdate && !FcEnc_th.busy()){
                                encodingUpdating = true;
                                deleted_face_num = 0;
                                std::cout << "Updating encoding of after deleting user .........." << std::endl;

                                deleting_user = false;
                                encodingToUpdate = true;
                                FcEnc_th.QueueJob([&]{
                                    if (F_Embedding->getEmbeddings(database_jpg, per_enc_vec) != -1){
                                        std::cout << "Face Encoding in database updated" << std::endl;
                                        if (F_Embedding->saveEmbeddings(per_enc_vec) == -1){
                                            // failed to save encodings
                                            std::cout << "Failed to save encodings" << std::endl;
                                        }
                                    }
                                    encodingToUpdate = false;
                                    encodingUpdating = false;
                                    encodingIsUpdate = true;
                                });
                            }

                            if (!encodingToUpdate && !encodingUpdating && encodingIsUpdate){
                                Hint = "Hello!";
                                Q_EMIT displayHint();
                                BtnEnabled = true;
                                Q_EMIT enableButtons();
                                LED_th.QueueJob([&]{
                                    setLEDs(DefaultLightMode);
                                });
                                encodingIsUpdate = false;
                            }
                            /*********************************************** CHECK MSG FROM CLOUD (USER) AND HANDLE *********************************************************/
                        }catch (std::exception &exp){
                            ErrorMsg = QString("Cloud request handling error - ") + exp.what();
                            goto end; //break nested loop to terminate program
                        }
                    }
                } //while loop of camera frame grabbing

                if (!STREAM && STREAMING){ //when it is not required to stream (!STREAM) but cam is on (STREAMING)
#ifdef CAM_AUTOFOCUS
                    Cam->stopVideo();
#else
                    Cap.release();
#endif // CAM_AUTOFOCUS
                    STREAMING = false;
                    setLEDs(0);
                }

                Online = false;
                BtnEnabled = false;
                stream_request = false;
                showDBFunc = false;

                ToRequest = false;
                cancelRequest = false;
                Q_EMIT NotInRequest();

                curl_th.Stop();
                visit_th.Stop();
                permi_th.Stop();
                Ctrl_th.Stop();

                int t = 0;
                while(Ctrl_th.busy()){
                    usleep(100000);
                    Hint = "Waiting for sub thread progress to end...";
                    Q_EMIT displayHint();
                    t ++;
                    if (t == 20)
                        break;
                }

                Hint = "Process killed.";
                Q_EMIT displayHint();

                if (0 != system("xrandr --output DSI-1 --off"))
                {
                    std::cout << "Failed to turn off display" << std::endl;
                    break; //to terminate program
                }
            } // if (PressedTime < 5.0s)
        } // if Pressed
        else{
            if (Online){
                try{
                    //Constantly check message from Cloud (USER) without qt application window
                    /*********************************************** CHECK MSG FROM CLOUD (USER) AND HANDLE *********************************************************/
                    if (stream_request != STREAM){
                        stream_request = STREAM;
                        Q_EMIT Streaming();
                    }

                    if(Check_Ret == 0 && !downloading_img && !deleting_user && !encodingToUpdate && !encodingUpdating){
                        Check_Ret = 1;
                        userToAddTemp = userToAdd;
                        userToDeleteTemp = userToDelete;
                    }

                    if (!userToAddTemp.empty() && !Image_th.busy() && !downloading_img && !deleting_user && !encodingToUpdate && !encodingUpdating){
                        downloading_img = true;
                        std::cout << "Downloading image(s)..." << std::endl;

                        removed_from_cloud.clear();
                        new_img_paths.clear();
                        Image_th.QueueJob([&]{
                            for (size_t u=0; u<userToAddTemp.size(); u++){
                                Cloud_cURL *new_image_curl = new Cloud_cURL(get_new_image_url + "/" + userToAddTemp[u]);
                                if (new_image_curl->CURL_init()){
                                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                                    if (new_image_curl->downloadPhoto(database, new_img_path) == -1){
                                        delete new_image_curl;
                                        new_image_curl = nullptr;
                                        for(int i = 1; i <= 5; i++){
                                            Cloud_cURL *image_backup_curl = new Cloud_cURL(image_backup_url + "/" + userToAddTemp[u]);
                                            if (image_backup_curl->CURL_init()){
                                                if (image_backup_curl->downloadPhoto(database, new_img_path) != 1){
                                                    delete image_backup_curl;
                                                    image_backup_curl = nullptr;
                                                    removed_from_cloud.push_back(u);
                                                    new_img_paths.push_back(new_img_path);
                                                    break;
                                                }
                                                delete image_backup_curl;
                                                image_backup_curl = nullptr;
                                            }
                                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                            std::cout << "[App backup img] Retry..." << std::endl;
                                        }
                                    }else{
                                        delete new_image_curl;
                                        new_image_curl = nullptr;
                                        removed_from_cloud.push_back(u);
                                        new_img_paths.push_back(new_img_path);
                                    }
                                }
                                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            }

                            for (size_t r=0; r<removed_from_cloud.size(); r++){
                                userToAddTemp.erase(userToAddTemp.begin() + removed_from_cloud[r]);
                            }

                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            downloading_img = false;
                            encodingToUpdate = true;
                            std::cout<< "end of downloading" << std::endl;
                        });
                    }

                    if (!userToDeleteTemp.empty() && !Image_th.busy() && !downloading_img && !deleting_user && !encodingToUpdate && !encodingUpdating){
                        deleting_user = true;
                        std::cout << "Deleting user(s)..." << std::endl;

                        removed_from_local.clear();
                        Image_th.QueueJob([&]{
                            for (size_t u=0; u<userToDeleteTemp.size(); u++){
                                try{
                                    std::filesystem::remove_all((database + "/" + userToDeleteTemp[u]).c_str());
                                }
                                catch(const std::exception &e){
                                    std::cout << "e.what" << std::endl;
                                    continue;
                                }
                                removed_from_local.push_back(u);
                                deleted_face_num ++;
                                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                            }

                            for (size_t r=0; r<removed_from_local.size(); r++){
                                userToDeleteTemp.erase(userToDeleteTemp.begin() + removed_from_local[r]);
                            }

                            std::this_thread::sleep_for(std::chrono::milliseconds(500));
                            encodingToUpdate = true;
                            deleting_user = false;
                            std::cout<< "end of deleting" << std::endl;
                        });
                    }

                    if (!new_img_paths.empty() && !encodingUpdating && encodingToUpdate && !FcEnc_th.busy()){
                        encodingUpdating = true;
                        FaultyImages.clear();
                        std::cout << "Checking images..." << std::endl;

                        int goodface_num = 0; //to store number of accepted img

                        FcEnc_th.QueueJob([&]{
                            for (size_t u=0; u<new_img_paths.size(); u++){
                                std::cout << new_img_paths[u] << std::endl;
                                img1 = cv::imread(new_img_paths[u], 1);
                                if (img1.empty()){
                                    std::cout << "Could not open or read the input image" << std::endl;
                                }
                                else{
                                    //extract
                                    ScaleX = ((float) img1.cols) / RetinaWidth;
                                    ScaleY = ((float) img1.rows) / RetinaHeight;
                                    // copy/resize image to result_cnn as input tensor
                                    cv::resize(img1, result_cnn1, cv::Size(RetinaWidth,RetinaHeight),cv::INTER_LINEAR);
                                    //get the face
                                    Rtn1.detect_retinaface(result_cnn1,detected_Faces);
                                }
                                //only one face per picture
                                if(detected_Faces.size()==1){
                                    if(detected_Faces[0].FaceProb>MinFaceThreshold && detected_Faces[0].rect.height > MinHeightFace){
                                        //get centre aligned image
                                        aligned1 = Warp1.Process(result_cnn1,detected_Faces[0]);
                                        std::remove(new_img_paths[u].c_str());
                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                        cv::imwrite(new_img_paths[u], aligned1);  //replace
                                        goodface_num ++;
                                        std::cout << "Added " << new_img_paths[u] << std::endl;
                                    }
                                    else{
                                        std::cout << "No human face found or face is too small" << std::endl;
                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                        std::remove(new_img_paths[u].c_str());
                                        GetFolderImage(new_img_paths[u], Folder, Jpg);
                                        FaultyImages.push_back(Jpg+"-"+Folder+".jpg");
                                        std::cout << "Deleted" << std::endl;
                                    }
                                }
                                else{
                                    if (detected_Faces.size()==0)
                                        std::cout << "No face is found" << std::endl;
                                    if (detected_Faces.size()>1)
                                        std::cout << "More than 1 Face are seen" << std::endl;
                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                    std::remove(new_img_paths[u].c_str());
                                    GetFolderImage(new_img_paths[u], Folder, Jpg);
                                    FaultyImages.push_back(Jpg+"-"+Folder+".jpg");
                                    std::cout << "Deleted" << std::endl;
                                }

                                if (std::filesystem::is_empty(new_img_paths[u].substr(0,new_img_paths[u].find_last_of('/'))))
                                    std::filesystem::remove(new_img_paths[u].substr(0,new_img_paths[u].find_last_of('/')));
                            }

                            std::this_thread::sleep_for(std::chrono::milliseconds(500));

                            if(!FaultyImages.empty()){
                                for(int i=0; i<10; i++){
                                    if (faulty_image_curl->CURL_init()){
                                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                        if(faulty_image_curl->sendFaultyImage(FaultyImages) != -1){
                                            break;
                                        }
                                    }
                                    std::cout << "[App Faulty img] Retry..." << std::endl;
                                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                                }
                            }

                            std::this_thread::sleep_for(std::chrono::milliseconds(500));

                            if(goodface_num > 0){
                                std::cout << "Updating encoding of images added .........." << std::endl;
                                // Update Encoding
                                if (F_Embedding->getEmbeddings(database_jpg, per_enc_vec) != -1){
                                    // extracted encodings from database
                                    if (F_Embedding->saveEmbeddings(per_enc_vec) != -1){
                                        // save encodings
                                    }
                                }
                            }
                            new_img_paths.clear();
                            encodingToUpdate = false;
                            encodingUpdating = false;
                            encodingIsUpdate = true;
                        });
                    }

                    if (deleted_face_num > 0 && !encodingUpdating && encodingToUpdate && !FcEnc_th.busy()){
                        encodingUpdating = true;
                        deleted_face_num = 0;
                        std::cout << "Updating encoding of after deleting user .........." << std::endl;

                        deleting_user = false;
                        encodingToUpdate = true;
                        FcEnc_th.QueueJob([&]{
                            if (F_Embedding->getEmbeddings(database_jpg, per_enc_vec) != -1){
                                std::cout << "Face Encoding in database updated" << std::endl;
                                if (F_Embedding->saveEmbeddings(per_enc_vec) == -1){
                                    // failed to save encodings
                                    std::cout << "Failed to save encodings" << std::endl;
                                }
                            }
                            encodingToUpdate = false;
                            encodingUpdating = false;
                            encodingIsUpdate = true;
                        });
                    }

                    if (!encodingToUpdate && !encodingUpdating && encodingIsUpdate){
                        encodingIsUpdate = false;
                    }
                    /*********************************************** CHECK MSG FROM CLOUD (USER) AND HANDLE *********************************************************/
                }catch (std::exception &exp){
                    ErrorMsg = QString("Cloud request handling error - ") + exp.what();
                    break;
                }
                try{
                    if (STREAM){
                        if (!STREAMING){ //if camera not on
#ifdef CAM_AUTOFOCUS
                            Cam->startVideo();
                            usleep(100000);
                            if(!Cam->getVideoFrame(Frame,1000)){
                                std::cerr << "Timeout error" << std::endl;
                                break; // to terminate program
                            }

                            // get light exposure and control light source
                            cv::cvtColor(Frame, GreyScale, cv::COLOR_BGR2GRAY);
                            if (LightIntensity < cv::mean(GreyScale)[0] + 20 && LightIntensity > cv::mean(GreyScale)[0] - 20){
                                LightIntensity = cv::mean(GreyScale)[0];
                                if (LightIntensity<5 && DefaultLightMode != 1){
                                    //lightMode = 1
                                    setLEDs(1);
                                }
                                else if (LightIntensity<50 && DefaultLightMode != 2){
                                    //lightMode = 2
                                    setLEDs(2);
                                }else if (LightIntensity<105 && DefaultLightMode != 3){
                                    //lightMode = 3
                                    setLEDs(3);
                                }
                                else if (DefaultLightMode != 0){
                                    setLEDs(0);
                                }
                            }

#else
                            Cap(pipeline, cv::CAP_GSTREAMER); // (only for libcamerasrc, not raspicam)
                            if (!Cap.isOpened()) {
                                std::cerr << "ERROR: Unable to open the camera" << std::endl;
                                break; // to terminate program
                            }
#endif // CAM_AUTOFOCUS
                            STREAMING = true;
                        }
                        else{ //if camera is on

#ifdef CAM_AUTOFOCUS
                            if(!Cam->getVideoFrame(Frame,1000)){
                                std::cerr << "Failed to get frame" << std::endl;
                                Frame = cv::Mat(capture_height, capture_width, CV_8UC3, cv::Scalar(0,0,0));
                            }
#else
                            Cap >> Frame; //shift right 4 bits?
                            if (Frame.empty()) {
                                std::cerr << "Failed to get frame" << std::endl;
                                Frame = cv::Mat(capture_height, capture_width, CV_8UC3, cv::Scalar(0,0,0));
                            }
#endif // CAM_AUTOFOCUS
                            cv::rotate(Frame, Frame, cv::ROTATE_180);   // rotate the frame 180 degree due to camera upside down positioning
                            Stream->stream(Frame);

                            LightModeCheck ++; //increase with each processed frame
                            if (LightModeCheck == 10){
                                LightModeCheck = 0;
                                LED_th.QueueJob([&]{
                                    cv::cvtColor(Frame, GreyScale, cv::COLOR_BGR2GRAY);
                                    float temp = cv::mean(GreyScale)[0];
                                    if (!(LightIntensity < temp + 60 && LightIntensity > temp - 30)){
                                        LightIntensity = temp;
                                        if (LightIntensity<5 && DefaultLightMode != 1){
                                            //lightMode = 1
                                            setLEDs(1);
                                        }
                                        else if (LightIntensity<50 && DefaultLightMode != 2){
                                            //lightMode = 2
                                            setLEDs(2);
                                        }else if (LightIntensity<105 && DefaultLightMode != 3){
                                            //lightMode = 3
                                            setLEDs(3);
                                        }
                                        else if (DefaultLightMode != 0){
                                            setLEDs(0);
                                        }
                                    }
                                });
                            }
                        }// if !STREAMING
                    }// if STREAM

                    if(STREAMING && !STREAM){

#ifdef CAM_AUTOFOCUS
                        Cam->stopVideo();
#else
                        Cap.release();
#endif // CAM_AUTOFOCUS
                        STREAMING = false;
                        setLEDs(0); // set LED back to OFF
                    }
                }catch (std::exception &exp){
                    ErrorMsg = QString("Streaming error - ") + exp.what();
                    break; // break to terminate program
                }
            }// if Online
        }// else - no button pushed
        usleep(130000); //0.13s
    }// end of while(Run)

    /**************************************************************/
    /**************************** EXIT ****************************/
    /**************************************************************/
end:
    Run = false;
    system("xrandr --output DSI-1 --preferred"); // turn on display

    if (FcEnc_th.busy() || encodingUpdating || encodingToUpdate || downloading_img || deleting_user){
        if (completeB4End == 0)
            std::cout << "Waiting for database update to complete";
        std::cout << "..." << std::endl;
        completeB4End ++;
        goto run_cycle;
    }

    cleanup();
    if (ErrorMsg.isEmpty()){
        ErrorMsg = "Program terminated";
        Q_EMIT EXIT();
    }
    else{
        Q_EMIT EXIT();
        exit(EXIT_FAILURE);
    }
}

QImage MyVideoCapture::cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );
#endif

        return image;
    }

    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}
QPixmap MyVideoCapture::cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ));
}
//----------------------------------------------------------------------------------------
// To pull current time
//----------------------------------------------------------------------------------------
std::string MyVideoCapture::NowToString()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string str(buffer);
    return str;
}
//----------------------------------------------------------------------------------------
// camera pipeline for bullseye OS using gstreamer (non-autofocus)
//----------------------------------------------------------------------------------------
std::string MyVideoCapture::gstreamer_pipeline(int capture_width, int capture_height, int framerate, int display_width, int display_height)
{
    return
            " libcamerasrc ! video/x-raw, "
            " width=(int)" + std::to_string(capture_width) + ","
            " height=(int)" + std::to_string(capture_height) + ","
            " framerate=(fraction)" + std::to_string(framerate) +"/1 !"
            " videoconvert ! videoscale !"
            " video/x-raw,"
            " width=(int)" + std::to_string(display_width) + ","
            " height=(int)" + std::to_string(display_height) + " ! appsink";
}
//----------------------------------------------------------------------------------------
//  Computing the cosine distance between input feature and ground truth feature
//----------------------------------------------------------------------------------------
inline float MyVideoCapture::CosineSimilarity(const cv::Mat &v1, const cv::Mat &v2)
{
    return v1.dot(v2) / (cv::norm(v1) * cv::norm(v2));
}
//----------------------------------------------------------------------------------------
// painting
//----------------------------------------------------------------------------------------
void MyVideoCapture::DrawObjects(cv::Mat &frame, std::vector<FaceObject> &Faces)
{
    cv::Scalar RectangleColor = cv::Scalar(0, 255, 0); //default rectangle color = yellow

    for(size_t i=0; i < Faces.size(); i++){
        FaceObject& obj = Faces[i];

        //----- rectangle around the face -------
        obj.rect.x *= ScaleX;
        obj.rect.y *= ScaleY;
        obj.rect.width *= ScaleX;
        obj.rect.height*= ScaleY;
        //---------------------------------------

        //----- diagnostic ----------------------
#ifdef SHOW_LANDMARKS
        for(int u=0;u<5;u++){
            obj.landmark[u].x*=ScaleX;
            obj.landmark[u].y*=ScaleY;
        }
        cv::circle(frame, obj.landmark[0], 2, cv::Scalar(0, 255, 255), -1);
        cv::circle(frame, obj.landmark[1], 2, cv::Scalar(0, 255, 255), -1);
        cv::circle(frame, obj.landmark[2], 2, cv::Scalar(0, 255, 255), -1);
        cv::circle(frame, obj.landmark[3], 2, cv::Scalar(0, 255, 255), -1);
        cv::circle(frame, obj.landmark[4], 2, cv::Scalar(0, 255, 255), -1);
#endif // SHOW_LANDMARKS
        //---------------------------------------

#ifdef SHOW_LEGEND
        cv::putText(frame, cv::format("Angle : %0.1f", obj.Angle),cv::Point(10,40),cv::FONT_HERSHEY_SIMPLEX,0.6, cv::Scalar(180, 180, 0));
        cv::putText(frame, cv::format("Face prob : %0.4f", obj.FaceProb),cv::Point(10,60),cv::FONT_HERSHEY_SIMPLEX,0.6, cv::Scalar(180, 180, 0));
        cv::putText(frame, cv::format("Match prob : %0.4f", obj.NameProb),cv::Point(10,80),cv::FONT_HERSHEY_SIMPLEX,0.6, cv::Scalar(180, 180, 0));
        cv::putText(frame, cv::format("True prob : %0.4f", obj.LiveProb),cv::Point(10,100),cv::FONT_HERSHEY_SIMPLEX,0.6, cv::Scalar(180, 180, 0));
#endif // SHOW_LEGEND

        //----- labels ----------------------------
#ifdef RECOGNIZE_FACE
        cv::String Str;
        cv::Scalar color;
        int  baseLine = 0;

        switch(obj.Color){
        case 0 : color = cv::Scalar(165, 206, 94, 255); break;  // green -> known face
        case 1 : color = cv::Scalar(80, 255, 255, 255); break;  // yellow -> stranger
        case 2 : color = cv::Scalar(255, 237, 178, 255); break;  // blue -> too tiny
        case 3 : color = cv::Scalar(127, 127, 255, 255); break;  // red -> fake
        case 4 : color = cv::Scalar(255, 255, 255, 255); break;  // white -> too many faces
        case 5 : color = cv::Scalar(299, 204, 255, 255); break;  // pink -> Blur or Askew
        case 6 : color = cv::Scalar(0, 129, 255, 255); break;  // orange ->stranger in temp database
        default: color = cv::Scalar(255, 255, 255, 255);
        }

        switch(obj.NameIndex){
        case -1: Str="Stranger"; break;
        case -2: Str="Too Tiny"; break;
        case -3: Str="Fake !";   break;
        case -4: Str="1 Face Only"; break;
        case -5: Str="Blur or Askew"; break;
        case -6: Str="Stranger"; break; // for stranger in temp database
        default: Str=per_enc_vec[obj.NameIndex].name;
        }

        cv::Size label_size = cv::getTextSize(Str, cv::FONT_HERSHEY_SIMPLEX, 0.6, 1, &baseLine);
        int x = obj.rect.x;
        int y = obj.rect.y - label_size.height - baseLine;
        if(y<0) y = 0;
        if(x+label_size.width > frame.cols) x=frame.cols-label_size.width;

        cv::rectangle(frame, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),color,-1);
        cv::putText(frame, Str, cv::Point(x, y+label_size.height+2),cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0));
        RectangleColor = color;
#endif // RECOGNIZE_FACE
        cv::rectangle(frame, obj.rect, RectangleColor);
    }
}
//----------------------------------------------------------------------------------------
// Get folder and img name
//----------------------------------------------------------------------------------------
void MyVideoCapture::GetFolderImage(std::string img_path, std::string &folder, std::string &jpg_name)
{
    img_path.erase(img_path.length()-4, img_path.length());
    img_path += "/";

    std::vector<std::string> vec;
    std::string delimiter = "/";
    size_t pos = 0;
    while((pos = img_path.find(delimiter)) != std::string::npos){
        vec.push_back(img_path.substr(0,pos));
        img_path.erase(0, pos + delimiter.length());
    }

    if (vec.size() >= 2){
        folder = vec[vec.size()-3];
        jpg_name = vec.back();
    }
}
//----------------------------------------------------------------------------------------
// Get LEDs Mode
//----------------------------------------------------------------------------------------
void MyVideoCapture::setLEDs(int Mode, int delay_ms)
{
    if (Mode == 0){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->Pause();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
        DefaultLightMode = LED_UNIX_Socket::lightMode;
    }else if (Mode == 1){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->FullWhite();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
        DefaultLightMode = LED_UNIX_Socket::lightMode;
    }
    else if (Mode == 2){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->MediumWhite();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
        DefaultLightMode = LED_UNIX_Socket::lightMode;
    }
    else if (Mode == 3){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->SoftWhite();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
        DefaultLightMode = LED_UNIX_Socket::lightMode;
    }
    else if (Mode == 4){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->GreenCycle();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
    }
    else if (Mode == 5){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->RainbowCycle();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
    }
    else if (Mode == 6){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->WhiteCycle();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
    }else if (Mode == -1){
        LED_UNIX_Socket *LED_socket = new LED_UNIX_Socket();
        LED_socket->Stop();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        delete LED_socket;
        LED_socket = nullptr;
    }
}
//----------------------------------------------------------------------------------------
// Do Console command and get return
//----------------------------------------------------------------------------------------
std::string MyVideoCapture::doConsoleCMD(char* command)
{
    FILE *pipe = popen(command, "r");
    if(!pipe)
        return "ERROR";

    char buffer[128];
    std::string result = "";
    while(!feof(pipe))
    {
        if(fgets(buffer, 128, pipe) != NULL)
            result +=buffer;
    }
    pclose(pipe);
    return(result);
}
//----------------------------------------------------------------------------------------
// Check internet conectivity
//----------------------------------------------------------------------------------------
void MyVideoCapture::checkInternet()
{
    if(!reply->bytesAvailable()){
        Online = false;
        Q_EMIT OnlineMode();
    }
    else{
        //        std::string reply = doConsoleCMD((char*)"ping -c 1 www.google.com | head -5 | tail -1 | cut -d \",\" -f 3");
        //        if (reply.find(" 0%") != 0){
        //            Q_EMIT OfflineMode();
        //            return false;
        //        }else{
        Online = true;
        Q_EMIT OnlineMode();
    }
    //    }
}
