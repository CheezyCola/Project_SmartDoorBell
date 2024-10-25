#ifndef LCCV_HPP
#define LCCV_HPP

#include <mutex>
#include <atomic>
#include <pthread.h>
#include <opencv2/opencv.hpp>

#include "libcamera_app.hpp"
#include "libcamera_app_options.hpp"

/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2020-2021, Raspberry Pi (Trading) Ltd.
 * https://github.com/kbarni/LCCV/blob/main/include/lccv.hpp
 * Wrapper class
 *
 * Modified by Tong Tin Yi (2023)
 * - added autofocus from libcamera
 *
 */

namespace lccv {

/**
* @brief C++ wrapper for camera app\n
* modified from https://github.com/kbarni/LCCV/blob/main/include/lccv.hpp
*/
class PiCamera {
public:
    PiCamera();
    ~PiCamera();

    Options *options;

    //Photo mode
    bool startPhoto();
    bool capturePhoto(cv::Mat &frame);
    bool stopPhoto();

    //Video mode
    bool startVideo();
    bool getVideoFrame(cv::Mat &frame, unsigned int timeout);
    void stopVideo();

protected:
    void run();
protected:
    LibcameraApp *app;
    void getImage(cv::Mat &frame, CompletedRequestPtr &payload);
    static void *videoThreadFunc(void *p);
    pthread_t videothread;
    unsigned int still_flags;
    unsigned int vw,vh,vstr;
    std::atomic<bool> running,frameready;
    uint8_t *framebuffer;
    std::mutex mtx;
    bool camerastarted;
};

}
#endif
