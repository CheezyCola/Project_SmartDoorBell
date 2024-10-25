#include "FaceEmbedding.h"

FaceEmbedding::FaceEmbedding()
{
    //ctor
}

FaceEmbedding::~FaceEmbedding()
{
    //dtor
}

int FaceEmbedding::getEmbeddings(std::string database_path, Person_Encodings_vector & vec)
{
    int status = 0;
    PersonCnt = 0;

    std::cout << "Loading faces in databases" << std::endl;
    try{
        cv::glob(database_path, Faces_path, true);
    }
    catch(const std::exception &e){
        std::cout << "Failed to extract encodings from database" << std::endl;
        std::cout << "Error: " << e.what() << std::endl;
        return status = -1;
    }

    FaceCnt=Faces_path.size();
	if(FaceCnt==0) {
		std::cout << "No image files[jpg] in database" << std::endl;
	}
	else{

        std::vector<cv::Mat> encodings;

        std::string temp_name = "";
        std::string store_name = "";
        std::string store_category = "";

        for(int i=0; i<FaceCnt; i++){
            //convert to landmark vector
            face = cv::imread(Faces_path[i]);
            cv::Mat faceEncoding = ArcFace.GetFeature(face);

            std::string str = Faces_path[i];
            str = str.erase(str.length()-4, str.length()-1); //remove .jpg
            std::string img_name = str.substr(str.find_last_of("/\\")+1, -1);

            std::string face_loc = Faces_path[i];
            std::string _name = face_loc.substr(0, face_loc.find_last_of("/\\"));
            std::string name = _name.substr(_name.find_last_of("/\\")+1, -1);
            name.erase(std::remove_if(name.begin(), name.end(), [](auto ch) { return std::isdigit(ch); }), name.end());

            std::string category = _name.substr(0, _name.find_last_of("/\\"));
            category = category.substr(category.find_last_of("/\\")+1, -1);

//            std::cout << "image name " << img_name << std::endl;
//            std::cout << "name " << name << std::endl;
//            std::cout << "category " << category << std::endl;
//            std::cout << "size" << faceEncoding.size() << std::endl;

            if (i == 0){
                // initialize store name to name
                // initialize store category to category
                store_name = name;
                store_category = category;
            }

            if (i == FaceCnt-1){
                if (temp_name != name){
                    PersonCnt ++;
                    strcpy(person_enc.name, store_name.c_str());
                    strcpy(person_enc.category, store_category.c_str());
                    person_enc.Encodings = encodings;
                    std::cout << "store " << "\t" << person_enc.name << "\t" << person_enc.category << "\t" << person_enc.Encodings.size() << std::endl;
                    person_enc_vec.push_back(person_enc);
                    encodings.clear();
                }
                store_name = name;
                store_category = category;
                name = "";  //to trigger storing into map
                encodings.push_back(faceEncoding);
            }

            //store encodings in map form
            if(name != store_name){
                PersonCnt ++;
                strcpy(person_enc.name, store_name.c_str());
                strcpy(person_enc.category, store_category.c_str());
                person_enc.Encodings = encodings;
                std::cout << "store " << "\t" << person_enc.name << "\t" << person_enc.category << "\t" << encodings.size() << std::endl;
                person_enc_vec.push_back(person_enc);
                store_name = name;
                store_category = category;
                encodings.clear();
            }
            else
                temp_name = name;

            encodings.push_back(faceEncoding);

            if(FaceCnt>1) std::cout << "Loading: " << (i*100.0)/(FaceCnt-1) << "\t%" << std::setprecision(2) << std::fixed << std::endl;
        }

        std::cout << "" << std::endl;
        std::cout << "Loaded " << FaceCnt << " faces in total" << std::endl;
        std::cout << "Found " << PersonCnt << " person(s) in database." << std::endl;
        std::cout << "" << std::endl;
	}

    vec = person_enc_vec;
    person_enc_vec.clear();

	return status;
}

int FaceEmbedding::saveEmbeddings(Person_Encodings_vector vec)
{
    int status = 0;

    std::ofstream ofs(encoding_file.c_str(), std::ios::binary | std::ios::out);
    if (ofs.is_open()){
        std::cout << "Saving encoding to file" << std::endl;
        size_t iter = 0;
        while (iter != vec.size()){
            ofs.write(reinterpret_cast<char const*>(&vec[iter].name), sizeof(vec[iter].name));
            ofs.write(reinterpret_cast<char const*>(&vec[iter].category), sizeof(vec[iter].category));

            size_t index_count(vec[iter].Encodings.size());
            ofs.write(reinterpret_cast<char const*>(&index_count), sizeof(index_count));
            for (size_t i=0; i < index_count; i++){
                int cols(vec[iter].Encodings[i].cols), rows(vec[iter].Encodings[i].rows), type(vec[iter].Encodings[i].type());
                bool continuous(vec[iter].Encodings[i].isContinuous());

                ofs.write(reinterpret_cast<char const*>(&cols), sizeof(cols));
                ofs.write(reinterpret_cast<char const*>(&rows), sizeof(rows));
                ofs.write(reinterpret_cast<char const*>(&type), sizeof(type));
                ofs.write(reinterpret_cast<char const*>(&continuous), sizeof(continuous));

                if (continuous) {
                    size_t const data_size(rows * cols * vec[iter].Encodings[i].elemSize());
                    ofs.write(reinterpret_cast<char const*>(vec[iter].Encodings[i].ptr()), data_size);
                } else {
                    size_t const row_size(cols * vec[iter].Encodings[i].elemSize());
                    for (int i(0); i < rows; ++i) {
                        ofs.write(reinterpret_cast<char const*>(vec[iter].Encodings[i].ptr(i)), row_size);
                    }
                }
            }
            iter ++;
        }
        ofs.close();
    }
    else{
        std::cout << "Failed to save encoding to file" << std::endl;
        status = -1;
    }
    std::cout << "Successfully save encoding to file" << std::endl;

    return status;
}

int FaceEmbedding::loadEmbeddings(Person_Encodings_vector &vec)
{
    int status = 0;

    std::ifstream infile(encoding_file.c_str(), std::ios::binary | std::ios::in);
    if (infile.is_open()){
        std::cout << "Loading saved encoding from file" << std::endl;
        while (!infile.eof()){
            size_t index_count(0);
            infile.read(reinterpret_cast<char*>(&person_enc.name), sizeof(person_enc.name));
            infile.read(reinterpret_cast<char*>(&person_enc.category), sizeof(person_enc.category));

            infile.read(reinterpret_cast<char*>(&index_count), sizeof(index_count));

            std::vector<cv::Mat> mat_vector;
            for (size_t i=0; i < index_count; i++){
                int cols, rows, type;
                bool continuous;

                infile.read(reinterpret_cast<char*>(&cols), sizeof(cols));
                infile.read(reinterpret_cast<char*>(&rows), sizeof(rows));
                infile.read(reinterpret_cast<char*>(&type), sizeof(type));
                infile.read(reinterpret_cast<char*>(&continuous), sizeof(continuous));

                cv::Mat mat;
                mat.create(rows, cols, type);

                if (continuous) {
                    size_t const data_size(rows * cols * mat.elemSize());
                    infile.read(reinterpret_cast<char*>(mat.ptr()), data_size);
                } else {
                    size_t const row_size(cols * mat.elemSize());
                    for (int i(0); i < rows; ++i) {
                        infile.read(reinterpret_cast<char*>(mat.ptr(i)), row_size);
                    }
                }
                mat_vector.push_back(mat);
            }

            person_enc.Encodings = mat_vector;
            vec.push_back(person_enc);
        }
        infile.close();
        status = true;
    }
    else {
        std::cout << "Failed to open encoding file" << std::endl;
        status = -1;
    }
    std::cout << "" << std::endl;

    return status;
}
