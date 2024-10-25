#include "Cloud_cURL.h"

Cloud_cURL::Cloud_cURL(std::string url)
{
    API_URL = url;
}

Cloud_cURL::~Cloud_cURL()
{
}

bool Cloud_cURL::CURL_init()
{
    bool open = false;
    _curl = curl_easy_init();
    if (!_curl)
        std::cerr << "Failed to initialize libcurl" << std::endl;
    else
        open =true;

    return open;
}

int Cloud_cURL::sendMsg(std::string Dict_key, std::string value)
{
    int status = 0;
    long response_code;
    long failure_code;
    std::string response_data;

    Json::Value json_data;
    json_data[Dict_key] = value;
    std::string json_string = json_data.toStyledString();
    std::cout << json_string << std::endl;

    //Set the options for the request
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(_curl, CURLOPT_POST, 1);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, json_string.length());
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);
    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }

    //Print the response data
    std::cout << "Response: " << response_data << std::endl;

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::sendMsg(std::string Dict_key, bool value)
{
    int status = 0;
    long response_code;
    long failure_code;
    std::string response_data;

    Json::Value json_data;
    json_data[Dict_key] = value;
    std::string json_string = json_data.toStyledString();
    std::cout << json_string << std::endl;

    //Set the options for the request
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(_curl, CURLOPT_POST, 1);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, json_string.length());
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);
    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }

    //Print the response data
    if (response_data.find("cancelled") != std::string::npos) status = 0;
    std::cout << "Response: " << response_data << std::endl;

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::sendMsg(std::string Dict_key, std::string value, std::string Dict_key2, std::string value2)
{
    int status = 0;
    long response_code;
    long failure_code;
    std::string response_data;

    Json::Value json_data;
    json_data[Dict_key2] = value2;
    json_data[Dict_key] = value;
    std::string json_string = json_data.toStyledString();
    std::cout << json_string << std::endl;

    //Set the options for the request
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(_curl, CURLOPT_POST, 1);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, json_string.length());
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);
    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }

    //Print the response data
    std::cout << "Response: " << response_data << std::endl;

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::sendMsg(std::string Dict_key, std::string value, std::string Dict_key2, std::string value2, std::string Dict_key3, std::string value3)
{
    int status = 0;
    long response_code;
    long failure_code;
    std::string response_data;

    Json::Value json_data;
    json_data[Dict_key3] = value3;
    json_data[Dict_key2] = value2;
    json_data[Dict_key] = value;
    std::string json_string = json_data.toStyledString();
    std::cout << json_string << std::endl;

    //Set the options for the request
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(_curl, CURLOPT_POST, 1);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, json_string.length());
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);
    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }

    //Print the response data
    std::cout << "Response: " << response_data << std::endl;

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::sendFaultyImage(std::vector<std::string> faultyimage)
{
    int status = 0;
    long response_code;
    long failure_code;
    std::string response_data;

    Json::Value arr;
    for (size_t i=0; i<faultyimage.size(); i++){
        arr.append(faultyimage[i]);
    }

    Json::Value json_data;
    json_data["faultyimage"] = arr;
    std::string json_string = json_data.toStyledString();
    std::cout << json_string << std::endl;

    //Set the options for the request
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(_curl, CURLOPT_POST, 1);
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, json_string.length());
    curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, json_string.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);
    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }

    //Print the response data
    std::cout << "Response: " << response_data << std::endl;

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::sendPhoto(std::string image_path)
{
    int status = 0;
    long response_code;
    long failure_code;

    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    static const char buf[] = "Expect:";

    /* Fill in the file upload field */
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "image",
               CURLFORM_FILE, image_path.c_str(),
               CURLFORM_CONTENTTYPE, "Image/jpeg",
               CURLFORM_END);

    /* Fill in the filename field */
    curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "face",
               CURLFORM_COPYCONTENTS, "face.jpg",
               CURLFORM_END);

    headerlist = curl_slist_append(headerlist, buf);

    //curl_global_init(CURL_GLOBAL_ALL);
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(_curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(_curl, CURLOPT_SERVER_RESPONSE_TIMEOUT, 10L);
    curl_easy_setopt(_curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);

    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }

    //Print the response data
    std::cout << "Response: " << response_code << std::endl;

    /* then cleanup the formpost chain */
    curl_formfree(formpost);
    /* free slist */
    curl_slist_free_all (headerlist);

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::downloadPhoto(std::string database_path, std::string &img_path)
{
    int status = -1;
    long response_code;
    long failure_code;

    std::string img_name = API_URL;
    std::string delimiter = "/";
    size_t pos = 0;
    std::string token;
    while((pos = img_name.find(delimiter)) != std::string::npos){
        token = img_name.substr(0,pos);
        img_name.erase(0, pos + delimiter.length());
    }

    img_name.erase(img_name.length()-4, img_name.length());
    pos = img_name.find("-");

    std::string folder = img_name.substr(pos+1,-1);
    std::string jpg_name = img_name.substr(0,pos);

    //create personal directory if not exist
    std::string person = jpg_name;
    person.erase(std::remove_if(person.begin(), person.end(), [](auto ch) { return std::isdigit(ch); }), person.end());

    std::string dir_cmd = "mkdir -p " + database_path + "/" + folder + "/" + person;
    const int dir_err = system(dir_cmd.c_str());
    if (-1 == dir_err){
        std::cout << "error creating directory" << std::endl;
        return status;
    }

    img_path = database_path + "/" + folder + "/" + person + "/" + jpg_name + ".jpg";
    FILE *fp = fopen(img_path.c_str(), "wb");
    if (!fp)
    {
        std::cout << "Failed to create jpg file on disk" << std::endl;
        return status;
    }

    curl_easy_setopt(_curl, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
	curl_easy_setopt(_curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");
	curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);

    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
	curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, nullptr);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);

    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
    }
    else
        status = 0;

    //Print the response data
    std::cout << "Response: " << response_code << std::endl;

    /* close stream */
    fclose(fp);

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::readRequestStatus(int requestCode, std::pair<int, std::string> &m_pair)
{
    int status = 0;
    long response_code;
    long failure_code;
    std::string response_data = "";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    //Set the options for the request
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);

    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);

    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }

    curl_slist_free_all (headers);

    // update requestCode to keep track of read request
    m_pair = std::make_pair(requestCode, response_data);

    curl_easy_cleanup(_curl);

    return status;
}

int Cloud_cURL::readMessage(int requestCode, int &answerCode, std::vector<std::string> &userToAdd, std::vector<std::string> &userToDelete, bool &liveStreamOn)
{
    int status = 0;
    long response_code;
    long failure_code;
    std::string response_data = "";
    Json::Value root;
    Json::Reader reader;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    //Set the options for the request
    curl_easy_setopt(_curl, CURLOPT_URL, API_URL.c_str());
    curl_easy_setopt(_curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(_curl, CURLOPT_TIMEOUT, 6L);
    curl_easy_setopt(_curl, CURLOPT_SERVER_RESPONSE_TIMEOUT, 5L);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(_curl, CURLOPT_FAILONERROR,&failure_code);
    //Perform the request
    CURLcode res = curl_easy_perform(_curl);

    //Check for errors
    curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE,&response_code);

    if (res != CURLE_OK || response_code != 200 || failure_code == 400 || failure_code == 500){
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        status = -1;
    }
    else{
        answerCode = requestCode;
        reader.parse(response_data, root);

        liveStreamOn = root["requestlivestreaming"].asBool();

        userToAdd.clear();
        const Json::Value &names_array = root["names"];
        for (const Json::Value &name : names_array){
            userToAdd.push_back(name.asString());
        }

        userToDelete.clear();
        const Json::Value &delete_array = root["delete"];
        for (const Json::Value &del: delete_array){
            userToDelete.push_back(del.asString());
        }
    }

    curl_slist_free_all (headers);

    curl_easy_cleanup(_curl);

    return status;
}
