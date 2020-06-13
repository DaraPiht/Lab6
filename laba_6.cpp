#include <string>
#include <iostream>
#include <httplib/httplib.h>
#include <nlohman/json.hpp>
#include <fstream>

using namespace httplib;
using namespace nlohmann;
using namespace std;

string PATH = "/data/2.5/forecast?q=Simferopol&appid=c6fac05b9afef023bd1347495d1c2052&units=metric&lang=en";

void gen_response(const Request& req, Response& res){
	ifstream fin("informer_template.html");
	string temp;
	getline(fin, temp, '\0');

	Client cli("api.openweathermap.org", 80);

	auto client_res = cli.Get(PATH.c_str());
	if (client_res && client_res->status == 200){
		json j = json::parse(client_res->body);
		std::string forr = "{city.name}";
		temp.replace(temp.find("{city.name}"), forr.size(), j["city"]["name"].get<string>());
		size_t last_index = 0;

		string storage;
		for (short i = 0; i < j["cnt"].get<short>(); i += 8){
			storage = j["list"][i]["dt_txt"].get<string>();
			storage = storage.substr(0, storage.find(' '));
			last_index = temp.find("{list.dt}", last_index);
			temp.replace(last_index, strlen("{list.dt}"), storage);

			storage = j["list"][i]["weather"][0]["icon"].get<string>();
			last_index = temp.find("{list.weather.icon}", last_index);
			temp.replace(last_index, strlen("{list.weather.icon}"), storage);

			storage = to_string(j["list"][i]["main"]["temp"].get<double>());
			last_index = temp.find("{list.main.temp}", last_index);
			temp.replace(last_index, strlen("{list.main.temp}"), storage.substr(0, 4));
		  }
	        }
	res.set_content(temp, "text/html");
   }
int main(){
	// Не запуская, создаём сервер 
	Server svr;      
	// Выполняем условие, если кто-то обратиться к корню "сайта" - вызвать функцию gen_response
	svr.Get("/", gen_response);  
	// Запускаем сервер 
	svr.listen("localhost", 2020); 	
}
