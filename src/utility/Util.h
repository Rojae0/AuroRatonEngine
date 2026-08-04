#pragma once

#include <string>

class PathUtils {
public:
	static std::string MakeAbsPath(const std::string& path_from_solution_root)
	{
		return "D:/01_Develop/CPP/AuroRatonEngine/" + path_from_solution_root;
	}
};