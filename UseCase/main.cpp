#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <optional>
#include <utility>
#include <thread>

using namespace std::literals;

typedef double f64;


class Project
{
    public:
        Project();
        std::optional<std::string>        get_api_key();
        std::optional<std::pair<f64,f64>> get_stock_price();
        void update_ThingBoard();
    private:
        std::string APIKEY;
        std::string TB_USERNAME    = "tenant@thingsboard.org";
        std::string TB_PASSWORD    = "tenant";
        std::string TB_ACCESSTOKEN;
        std::string TB_URL         = "http://localhost:8080/api/v1/";

        inline std::optional<std::pair<f64,f64>> parse_price(std::string input);
        static size_t                            WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
        bool                                     send_ThingsBoard(std::pair<f64,f64> price);
        std::optional<std::string>               get_TB_ACCESTOKEN();
};

Project::Project()
{
    std::optional<std::string> apikey = this->get_api_key();
    if (!apikey.has_value())
    {
        std::cout << "Someting went wrong with the stockprice apikey\n";
        exit(1);
    }
    this->APIKEY = apikey.value();

    std::optional<std::string> tb_apikey = this->get_TB_ACCESTOKEN();
    if (!tb_apikey.has_value())
    {
        std::cout << "Someting went wrong with the ThingsBoard apikey\n";
        exit(1);
    }
    this->TB_URL.append(tb_apikey.value());
    this->TB_URL.append("/telemetry");
    this->TB_ACCESSTOKEN = tb_apikey.value();

    /* return; */
    auto test = this->get_stock_price();
    if (!test.has_value())
    {
        std::cout << "Someting went wrong with the getting stock price\n";
        exit(1);
    }
    std::cout << test->first << std::endl;
    std::cout << test->second << std::endl;
    this->send_ThingsBoard(test.value());
    /* std::cout << test->first << std::endl; */
    /* std::cout << test->second << std::endl; */
};

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

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, this->TB_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res)
        return false;

    /* auto price = this->parse_price(readBuffer); */
    //todo: implement this
    std::cout << readBuffer << std::endl;
    return true;
}

void Project::update_ThingBoard()
{
    auto price = this->get_stock_price();
    if (!price.has_value())
        return;
    if (!this->send_ThingsBoard(price.value()))
    {
        std::cout << "Could not send to ThingsBoard\n";
        return;
    }
    std::cout << price->first << std::endl;
    std::cout << price->second << std::endl;

    //sleep()
    std::this_thread::sleep_for(60s);
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
    /* std::ifstream envfile(".env"); */
    /* if (!envfile.is_open()) */
    /*     return std::nullopt; */
    /* std::string apikey; */
    try
    {
        result = getenv("TB_ACCESSTOKEN");
        /* std::getline(envfile, apikey, '='); */
        /* std::getline(envfile, apikey, '\n'); */
        /* std::getline(envfile, apikey, '='); */
        /* std::getline(envfile, apikey, '\n'); */
        /* std::cout << apikey << std::endl; */
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
    /* std::ifstream envfile(".env"); */
    /* if (!envfile.is_open()) */
    /*     return std::nullopt; */
    /* std::string apikey; */
    try
    {
        result = getenv("APIKEY");
        /* std::getline(envfile, apikey, '='); */
        /* std::getline(envfile, apikey, '\n'); */
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

int main(void)
{
    Project temp = Project();
    for(;;)
        temp.update_ThingBoard();
}
