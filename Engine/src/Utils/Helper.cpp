#include "../pch.h"
#include "Helper.h"

bool Helper::ReadOBJModel(const std::string& file, std::vector<glm::vec3>& positions, std::vector<glm::vec3>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned int>& indices)
{
	Assimp::Importer imp;

	const aiScene* scene = imp.ReadFile("./res/3d_models/" + file,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (nullptr == scene)
	{
		//std::cout << "Error: Cannot read model '" << file << "'" << std::endl;
		return false;
	}

	std::vector<glm::vec3> _vertices, _uvs, _normals;

	for (unsigned int i {0}; i < scene->mNumMeshes; i++)
	{
		auto& mesh = scene->mMeshes[i];

		for (unsigned int k{ 0 }; k < mesh->mNumVertices; k++)
		{
			auto& vert = mesh->mVertices[k];
			glm::vec3 v { vert.x, vert.y, vert.z };
			_vertices.push_back(v);

			if (mesh->HasTextureCoords(0))
			{
				auto& uv = mesh->mTextureCoords[0][k];
				glm::vec3 u { uv.x, uv.y, uv.z };
				_uvs.push_back(u);
			}

			if (mesh->HasNormals())
			{
				auto& normal = mesh->mNormals[k];
				glm::vec3 n { normal.x, normal.y, normal.z };
				_normals.push_back(n);
			}
		}

		for (unsigned int k{ 0 }; k < mesh->mNumFaces; k++)
		{
			auto& face = mesh->mFaces[k];
			for (unsigned int j{ 0 }; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
	}

	positions = _vertices;
	uvs = _uvs;
	normals = _normals;

	return true;
}


//std::vector<std::string> Helper::splitString(const std::string& s, const std::string& delim)
//{
//    int pos = s.find(delim);
//    int prevPos = 0;
//    std::vector<std::string> ans;
//    while (pos != std::string::npos)
//    {
//        std::string temp = s.substr(prevPos, pos - prevPos);
//        ans.push_back(temp);
//
//        prevPos = pos + delim.length();
//        pos = s.find(delim, prevPos);
//    }
//
//    ans.push_back(s.substr(prevPos, s.length() - prevPos));
//
//    return ans;
//}
//
//bool Helper::converibleToInt(const std::string& s)
//{
//    if (s[0] != '-' && s[0] != '+' && !std::isdigit(s[0])) return false;
//    if (s.empty()) return false;
//
//    bool hasDecimal{ false };
//
//    for (unsigned int i{ 1 }; i < s.length(); i++)
//    {
//        if (!std::isdigit(s[i]))
//        {
//            if ((s[i] == '.' || s[i] == ',') && !hasDecimal)
//                hasDecimal = true;
//            else
//                return false;
//        }
//    }
//
//    return true;
//}
//
//bool Helper::vsToVi(std::vector<std::string> v, std::vector<int>& ans)
//{
//    for (auto& s : v)
//    {
//        if (s.empty()) {
//            ans.push_back(0);
//            continue;
//        }
//
//        if (converibleToInt(s))
//            ans.push_back(std::stoi(s));
//        else
//            return false;
//    }
//
//    return true;
//}

//bool Helper::ReadOBJModel(const std::string& file, std::vector<glm::vec3>& positions, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& indices)
//{
//    std::ifstream f("./res/3d_models/" + file);
// 
//    if (!f)
//        return false;
//
//    std::vector<glm::vec3> temp_poses, temp_normals;
//    std::vector<glm::vec2> temp_uvs;
//
//    std::vector<Vertex> ans;
//
//    while (f.is_open())
//    {
//        char line[1024];
//        std::stringstream ss;
//        std::string s;
//
//        f.getline(line, 1024);
//        ss << line;
//
//        std::string command;
//        ss >> command;
//
//        switch (command[0])
//        {
//        case 'v':
//            switch (command[1])
//            {
//            case '\0': // Positions
//                glm::vec3 temp_pos;
//                ss >> temp_pos.x >> temp_pos.y >> temp_pos.z;
//
//                std::cout << "Position: " << temp_pos.x << " " << temp_pos.y << " " << temp_pos.z << std::endl;
//
//                temp_poses.push_back(temp_pos);
//                break;
//            case 't': // Texture coords
//                glm::vec2 temp_uv;
//                ss >> temp_uv.x >> temp_uv.y;
//
//                std::cout << "UV: " << temp_uv.x << " " << temp_uv.y << std::endl;
//
//                temp_uvs.push_back(temp_uv);
//                break;
//            case 'n': // Normals
//                glm::vec3 temp_norm;
//                ss >> temp_norm.x >> temp_norm.y >> temp_norm.z;
//
//                std::cout << "Normals: " << temp_norm.x << " " << temp_norm.y << " " << temp_norm.z << std::endl;
//
//                temp_normals.push_back(temp_norm);
//                break;
//            }
//
//            break;
//        case 'f':
//            while (ss >> s)
//            {
//                std::vector<int> indices;
//                vsToVi(splitString(s, "/"), indices);
//
//                // Not having 3 attributes
//                if (indices.size() < 3) return false;
//
//                // In case negative index
//                indices[0] = (indices[0] + temp_poses.size() + 1) % (temp_poses.size() + 1);
//                indices[1] = (indices[1] + temp_uvs.size() + 1) % (temp_uvs.size() + 1);
//                indices[2] = (indices[2] + temp_normals.size() + 1) % (temp_normals.size() + 1);
//
//                glm::vec3 pos = temp_poses[indices[0]];
//                glm::vec2 uv = indices[1] != 0 ? temp_uvs[indices[1]] : glm::vec2(-1.0f);
//                glm::vec3 norm = indices[2] != 0 ? temp_normals[indices[2]] : glm::vec3(0.0f);
//
//                Vertex v{ pos, uv, norm };
//                ans.push_back(v);
//            }
//
//            break;
//
//        default:
//            break;
//        }
//
//        if (f.eof())
//        {
//            f.close();
//            return true;
//        }
//
//        return false;
//    }
//
//    
//
//    return true;
//    //return false;
//}