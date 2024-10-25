/* A class to handle encoding file containing facial extraction
 * Created by Tong Tin Yi (2023)
 */

/**************************************/
/* all functions return int value for */
/* error handling. value 0 means okay */
/* if value is -1, error occurs       */
/**************************************/

#ifndef FACEEMBEDDING_H
#define FACEEMBEDDING_H

#include <fstream>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

#include "TMtCNN.h"
#include "TArcface.h"
#include "TRetina.h"

/**
 * @brief A struct type to hold person details: name, category and his/her facial embedding
 */
struct Person_Encodings {
    char name[50];  //49 characters
    char category[10];  //9 characters
    std::vector<cv::Mat> Encodings;
}__attribute__((packed));

typedef std::vector<Person_Encodings> Person_Encodings_vector;

/**
 * @brief A class to handle encoding file containing facial extraction
 */
class FaceEmbedding
{
    public:
        FaceEmbedding();
        virtual ~FaceEmbedding();
        /**
         * @brief Faces_path: Path, where all face images stored locally
         */
        std::vector<cv::String>Faces_path;
        /**
         * @brief FaceCnt: Number of face in database
         */
        int FaceCnt = 0;
        /**
         * @brief PersonCnt: Number of person in database
         */
        int PersonCnt = 0;

        //functions
        /**
         * @brief Get face embeddings of facial images in database
         * @param database_path: Location of facial images
         * @param vec: A vector containing person details and its facial features vector
         * @return -1, if error occurs
         */
        int getEmbeddings(std::string database_path, Person_Encodings_vector &vec);
        /**
         * @brief Save the embeddings of faces into dat file, encoded in binary form
         * @param vec: A vector containing person details and its facial features vector
         * @return -1, if error occurs
         */
        int saveEmbeddings(Person_Encodings_vector vec);
        /**
         * @brief Load the embeddings of faces from dat file
         * @param vec: A vector containing person details and its facial features vector
         * @return -1, if error occurs
         */
        int loadEmbeddings(Person_Encodings_vector &vec);

    protected:

    private:
        std::string encoding_file = "Encodings.dat";
        TArcFace ArcFace;
        cv::Mat face;
        Person_Encodings_vector person_enc_vec;
        Person_Encodings person_enc;
        std::vector<std::string> Category;
};

#endif // FACEEMBEDDING_H
