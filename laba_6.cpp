#include <iostream>
#include <string>
#include <nlohman/json.hpp>
#include <httplib/httplib.h>
#include <fstream>

using namespace httplib;
using namespace nlohmann;
using namespace std;

string Road = "/data/2.5/forecast?q=Feodosia&appid=c6fac05b9afef023bd1347495d1c2052&units=metric&lang=en";

void gen_response(const Request& request, Response& response);

int main(){
	// Не запуская, создаём сервер 
	Server svr;      
	// Выполняем условие, если кто-то обратиться к корню "сайта" - вызвать функцию gen_response
	svr.Get("/", gen_response);  
	// Запускаем сервер 
	svr.listen("localhost", 2020); 	
}

void gen_response(const Request& request, Response& response){
	ifstream fin("t_informer.html");
	string temp;
	getline(fin, temp, '\0');

	Client man("api.openweathermap.org", 80);

	auto client_response = man.Get(Road.c_str());
	if (client_response && client_response->status == 200){
		json j = json::parse(client_response->body);
		string C_name = "{city.name}";
		temp.replace(temp.find("{city.name}"), C_name.size(), j["city"]["name"].get<string>());
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
	response.set_content(temp, "text/html");
   }
