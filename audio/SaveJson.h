/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SaveJson.h
 * Author: root
 *
 * Created on May 1, 2019, 10:52 PM
 */

#ifndef SAVEJSON_H
#define SAVEJSON_H
#include <vector> 
#include <iostream> 
#include <algorithm>
#include "json11.h"

using namespace json11;
class SaveJson {
public:
    SaveJson(std::string file);
    void Add(std::string key, Json value);
    void Add(std::string key, std::vector<Json> value);

    SaveJson(const SaveJson& orig);
    bool SaveFile();
    virtual ~SaveJson();
    
    Json json;
private:
    std::string m_file;

};

#endif /* SAVEJSON_H */

