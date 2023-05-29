#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <optional>
#include <utility>
#include <thread>
#include <chrono>


using namespace std::literals;

typedef double f64;


class Project
{
    public:
        Project();
        Project(std::string arg1, std::string arg2);

        std::optional<std::string>        get_api_key();
        std::optional<std::pair<f64,f64>> get_stock_price();
        void                              update_ThingBoard();
        inline void                       sleep();
        inline void                       sleep(int seconds);
    private:
        std::string APIKEY;
        std::string TB_USERNAME    = "tenant@thingsboard.org";
        std::string TB_PASSWORD    = "tenant";
        std::string TB_ACCESSTOKEN;
        std::string TB_URL         = "http://thingsboard:9090/api/v1/";

        inline std::optional<std::pair<f64,f64>> parse_price(std::string input);
        static size_t                            WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
        bool                                     send_ThingsBoard(std::pair<f64,f64> price);
        std::optional<std::string>               get_TB_ACCESTOKEN();
};

Project::Project(std::string arg1, std::string arg2)
{
    this->APIKEY = arg1;
    this->TB_ACCESSTOKEN = arg2;
    this->TB_URL.append(this->TB_ACCESSTOKEN);
    this->TB_URL.append("/telemetry");

    auto test = this->get_stock_price();
    if (!test.has_value())
        std::cout << "Someting went wrong with the getting stock price\nCheck APIKEY";
    if (!this->send_ThingsBoard(test.value()))
    {
        std::cout << "Waiting for ThingBoard container\n";
    }
};

void Project::sleep()
{
    std::this_thread::sleep_for(60s);
}

void Project::sleep(int seconds)
{
    std::this_thread::sleep_for((std::chrono::seconds)seconds);
}

bool Project::send_ThingsBoard(std::pair<f64,f64> price)
{
    CURL *curl;
    CURLcode res;
    std::string data = "{\"lowestPrice\"=\"";
    data.append(std::to_string(price.first));
    data.append("\",\"highestPrice\"=\"");
    data.append(std::to_string(price.second));
    data.append("\"}");
    std::string readBuffer;
    curl = curl_easy_init();

    //check if curl works
    if(!curl)
        return false;

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string auth = "Authorization: Bearer " + this->TB_ACCESSTOKEN;
    headers = curl_slist_append(headers, auth.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, this->TB_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res)
        return false;
    return true;
}

void Project::update_ThingBoard()
{
    auto price = this->get_stock_price();
    if (!price.has_value())
        std::cout << "Could not fetch price (check APIKEY)\n";
    else if (!this->send_ThingsBoard(price.value()))
        std::cout << "Could not send to ThingsBoard\n";
    else
        std::cout << "Updated ThingsBoard!\n";
    this->sleep();
}

std::optional<std::pair<f64,f64>> Project::parse_price(std::string input)
{
    std::stringstream buffer(input);
    std::string high_s;
    std::string low_s;
    std::getline(buffer, high_s, 'h');
    std::getline(buffer, high_s, 'l');
    high_s.pop_back();
    high_s.pop_back();
    high_s.erase(0,2);
    /* std::cout << high_s << std::endl; */

    std::getline(buffer, low_s, 't');
    low_s.pop_back();
    low_s.pop_back();
    low_s.erase(0,2);
    /* std::cout << low_s << std::endl; */

    try
    {
        f64 high = std::stof(high_s);
        f64 low = std::stof(low_s);
        std::pair<f64,f64> price(low,high);
        return price;
    }
    catch (...)
    {
        return std::nullopt;
    }
}

std::optional<std::string> Project::get_TB_ACCESTOKEN()
{
    std::string result;
    try
    {
        result = (std::string) std::getenv("TB_ACCESSTOKEN");
    }
    catch (...)
    {
        return std::nullopt;
    }
    return result;
}

std::optional<std::string> Project::get_api_key()
{
    std::string result;
    try
    {
        result = (std::string) std::getenv("APIKEY");
    }
    catch (...)
    {
        return std::nullopt;
    }
    return result;
}

size_t Project::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::optional<std::pair<f64,f64>> Project::get_stock_price()
{
    std::string url = "https://api.polygon.io/v2/aggs/ticker/AAPL/prev?adjusted=false&apiKey=";
    url.append(this->APIKEY);
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();

    //check if curl works
    if(!curl)
        return std::nullopt;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res)
        return std::nullopt;

    auto price = this->parse_price(readBuffer);
    return price;
}

int main(int argc, char* argv[])
{
    Project temp = Project((std::string)argv[1], (std::string)argv[2]);
    for(;;)
        temp.update_ThingBoard();
}
