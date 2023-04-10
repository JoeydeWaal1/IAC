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
        std::optional<std::string> get_api_key();
        std::optional<std::pair<f64,f64>> get_stock_price();
        void update_ThingBoard();
    private:
        std::string APIKEY;

        inline
        std::optional<std::pair<f64,f64>> parse_price(std::string input);
        static
        size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
        bool send_ThingsBoard(std::pair<f64,f64> price);
};

Project::Project()
{
    std::optional<std::string> apikey = this->get_api_key();
    if (!apikey.has_value())
    {
        std::cout << "Someting went wrong with the apikey\n";
        exit(1);
    }
    this->APIKEY = apikey.value();
    auto test = this->get_stock_price();
    if (!test.has_value())
    {
        std::cout << "Someting went wrong with the getting stock price\n";
        exit(1);
    }
    /* std::cout << test->first << std::endl; */
    /* std::cout << test->second << std::endl; */
};

bool Project::send_ThingsBoard(std::pair<f64,f64> price)
{
    //todo: implement this
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

std::optional<std::string> Project::get_api_key()
{
    std::string result;
    std::ifstream envfile(".env");
    if (!envfile.is_open())
        return std::nullopt;
    std::string apikey;
    try
    {
        std::getline(envfile, apikey, '=');
        std::getline(envfile, apikey, '\n');
    }
    catch (...)
    {
        return std::nullopt;
    }
    return apikey;
}

size_t Project::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
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
