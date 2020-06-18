#include <iostream>
#include <string>
#include <nlohman/json.hpp>
#include <httplib/httplib.h>
#include <fstream>

using namespace httplib;
using namespace nlohmann;
using namespace std;

string Road = "/data/2.5/forecast?q=Feodosia&appid=309809d6c8c8d80cc221c21bc1a91790&units=metric&lang=en";

void gen_response(const Request& request, Response& response);

int main(){
	// Не запуская, создаём сервер 
	Server svr;      
	// Выполняем условие, если кто-то обратиться к корню "сайта" - вызвать функцию gen_response
	svr.Get("/", gen_response);  
	// Запускаем сервер 
	svr.listen("localhost", 3000); 	
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
		size_t l_ind = 0;

		string storg;
		for (short i = 0; i < j["cnt"].get<short>(); i += 8){
			storg = j["list"][i]["dt_txt"].get<string>();
			storg = storg.substr(0, storg.find(' '));
			l_ind = temp.find("{list.dt}", l_ind);
			temp.replace(l_ind, strlen("{list.dt}"), storg);

			storg = j["list"][i]["weather"][0]["icon"].get<string>();
			l_ind = temp.find("{list.weather.icon}", l_ind);
			temp.replace(l_ind, strlen("{list.weather.icon}"), storg);

			storg = to_string(j["list"][i]["main"]["temp"].get<double>());
			l_ind = temp.find("{list.main.temp}", l_ind);
			temp.replace(l_ind, strlen("{list.main.temp}"), 
			storg.substr(0, 4));
		  }
	        }
	response.set_content(temp, "text/html");
   }
