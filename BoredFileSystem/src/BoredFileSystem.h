#pragma once
#include "BoredObject.h"
#include<string>
#include<memory>
#include<fstream>
#include<filesystem>
#include<iostream>
#include<nlohmann/json.hpp>

class BoredFileSystem
{
public:
	/*
		@arg: a BoredObject bject

		@return int 1 if success
	*/
	int saveBject(std::shared_ptr<BoredObject> bject);
	
	/*
		@arg a SavedBoredData json file (.bored.json)

		@return data in Json format
	*/
	nlohmann::json readBoredFile(const std::string& filename); 

	/*
		@arg string jsonData as a json string format

		@return a BoredMap contain the json string's data from jsonData
	*/
	BoredMap parseJsonString(nlohmann::json jsonData);
	BoredData* helper(nlohmann::json root);

	/*
		Print current directory for debugging
	*/
	void printCurrentDir();
};

