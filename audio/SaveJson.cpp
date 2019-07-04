/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SaveJson.cpp
 * Author: root
 * 
 * Created on May 1, 2019, 10:52 PM
 */

#include "SaveJson.h"

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


SaveJson::SaveJson(std::string file):m_file(file) {
    
}

SaveJson::SaveJson(const SaveJson& orig) {
}

SaveJson::~SaveJson() {
}



void SaveJson::Add(std::string key, Json value) {
   // Json::object obj;
   // obj[key].push_back(4);
    //json[key] = json(value);
//    json[key].push_back(4);
    Json::object json_obj = json.object_items();
    json_obj[key]=Json(value);
    
    json = json_obj;
    
     std::string points_json = json.dump();
     
     int x = 9;
    
//Then the key keys3 contains a Json::array, that is nothing more than a std::vector<Json>.
//You can modify it as it follows:
//https://stackoverflow.com/questions/41245347/how-to-modify-a-json-object-using-json11-library
//json_obj["keys3"].push_back(4);
    
    
}

void SaveJson::Add(std::string key, std::vector<Json> value) {
    
   Json::object json_obj = json.object_items();
   
   json_obj[key]=Json(value);
     
   for( auto i :  json_obj)
   {
       std::cout <<  i.first  << " "  <<  i.second.dump() <<  std::endl;
   }
   
      
    json = json_obj;

}

bool SaveJson::SaveFile()
{
  ofstream myfile (m_file);
  if (myfile.is_open())
  {
    std::cout <<  json.dump()<<  std::endl;
    myfile<< std::setw(4) << json.dump();
    myfile.close();
    
    return true;
  }
  else std::cout << "Unable to open file";
  return false;
    
    
}