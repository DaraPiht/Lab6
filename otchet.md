МИНИСТЕРСТВО НАУКИ  И ВЫСШЕГО ОБРАЗОВАНИЯ РОССИЙСКОЙ ФЕДЕРАЦИИ  
Федеральное государственное автономное образовательное учреждение высшего образования  
"КРЫМСКИЙ ФЕДЕРАЛЬНЫЙ УНИВЕРСИТЕТ им. В. И. ВЕРНАДСКОГО"  
ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ  
Кафедра компьютерной инженерии и моделирования
<br/><br/>

### Отчёт по лабораторной работе №6<br/> по дисциплине "Программирование"
<br/>

студентки 1 курса группы ПИ-б-о-191(1)  
Пихтиенко Дарьи Владиславовны
направления подготовки 09.03.04 "Программная инженерия"  
<br/>

<table>
<tr><td>Научный руководитель<br/> старший преподаватель кафедры<br/> компьютерной инженерии и моделирования</td>
<td>(оценка)</td>
<td>Чабанов В.В.</td>
</tr>
</table>
<br/><br/>

Симферополь, 2020

### Лабораторная работа №6
Погодный информер

### Цель: 
1. Закрепить навыки разработки многофайловыx приложений;
2. Изучить способы работы с API web-сервиса;
3. Изучить процесс сериализации/десериализации данных.

### Ход работы:

**1\.** Скачиваем указанную в методичке библиотеку для работы с сетью с GitHub, подключаем необходимые файлы. Затем пишем тест-сервер, который будет выводить "Hellow word!" в браузере на веб-странице.<br/>
![Рисунок 1 Тест-сервер в браузере](https://github.com/DaraPiht/Lab6/blob/master/MlcS08SuSII.jpg)
*Рис.1 Тест-сервер в браузере*
**2\.** Регистрируемся на предоставленнном сайте, нам на почту приходит **API key - 309809d6c8c8d80cc221c21bc1a91790**, с помощью будем получать актуальный прогноз погоды.<br/>
**3\.** Пишем код для работы с **.html** файлом. Итоговый код программы выглядит так:
```c++
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
```
**4\.** Написанный кода в браузере.
![Рис.2 Ответ клиенту](https://github.com/DaraPiht/Lab6/blob/master/T_qBHb32dmA.jpg)
*Рис.2 Ответ клиенту*


### Вывод:
В ходе выполнения данной лабораторной работы я закрепила навыки работы с GitHub и многофайловыми файлами, научилась работать с сервером и клиентом, изучила процессы сериализации/десериализации данных.

