#include "BoredFileSystem.h"
using json = nlohmann::json;

int BoredFileSystem::saveBject(std::shared_ptr<BoredObject> bject)
{
	const char* path = "./SavedBoredData/";
	std::string fileName = "BoredObject@" + bject->GetID().ToString();
	std::ofstream file(path + fileName + ".bored.json");
	std::cout << path + fileName + ".bored.json" << std::endl;
	if (file) {
		file << bject->ToString();
		file.close();
	}
	else {
		std::cout << "Error: Unable to write file " << fileName << std::endl;
	}
	return 1;
;}

json BoredFileSystem::readBoredFile(const std::string& filename)
{
	const char* path = "./SavedBoredData/";
	std::ifstream f(path + filename);
	if (f.fail()) {
		std::cout << "Can't not find file at " << path + filename << ". Please try again" << std::endl;
		json status = false;
		return status;
	}
	json data = json::parse(f);
	return data;
}

BoredMap BoredFileSystem::parseJsonString(json jsonData)
{
	BoredMap result;
	result["root"] = helper(jsonData);
	return result;
}

BoredData* BoredFileSystem::helper(nlohmann::json root)
{
	switch (root.type()) {
	case json::value_t::number_unsigned:
	case json::value_t::number_integer : {
		return new BoredInt(root);
	} 
	case json::value_t::number_float: {
		return new BoredFloat(root);
	}
	case json::value_t::boolean: {
		return new BoredBool(root);
	}
	case json::value_t::string: {
		return new BoredString(root.get<std::string>());
	} 
	case json::value_t::object: {
		BoredMap* result = new BoredMap();
		for (auto& ele : root.items()) {
			(* result)[ele.key()] = helper(ele.value());
		}
		return result;
	} 
	case json::value_t::array: {
		BoredArray* result = new BoredArray;
		for (json::iterator it = root.begin(); it != root.end(); ++it) {
			result->PushBack(helper(*it));
		}
		return result;
	}
	}
	return new BoredBool(false);
}

void BoredFileSystem::printCurrentDir()
{
	std::string path = "./SavedBoredData";
	for (const auto& entry : std::filesystem::directory_iterator(path))
		std::cout << entry.path() << std::endl;
}

