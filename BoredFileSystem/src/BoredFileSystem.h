#pragma once
#include "BoredObject.h"
#include<string>
#include<memory>
#include<fstream>
#include<filesystem>
#include<iostream>
#include<nlohmann/json.hpp>

class BoredFileSystem
/*
	BoredFileSystem represent a file system that able to save data 
	from actors, components,... from the user's client code and write
	files from dir "SavedBoredData" from client code
*/
{
public:
	/*
		@arg: a BoredObject bject

		@return int 1 if success
	*/
	int SaveBject(std::shared_ptr<BoredObject> bject);
	
	/*
		@arg a SavedBoredData json file (.bored.json)

		@return data in Json format
	*/
	nlohmann::json ReadBoredFile(const std::string& filename); 

	/*
		@arg string jsonData as a json string format

		@return a BoredMap contain the json string's data from jsonData
	*/
	BoredMap ParseJson(nlohmann::json jsonData);

	// Helper method to perform parsing json type to BoredData*
	BoredData* ParseJsonHelper(nlohmann::json root);

	/*
		Print current directory for debugging
	*/
	void PrintCurrentDir();
};

