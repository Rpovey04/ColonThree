#include <windows.h>
#include <string>
#include <vector>

static std::vector<std::string> getAvailableDirectories(const std::string fPath) {
	std::wstring stemp = std::wstring(fPath.begin(), fPath.end());
	LPCWSTR folderPath = stemp.c_str();

	WIN32_FIND_DATA findFileData;
	std::wstring tempCast;

	std::vector<std::string> names;

	HANDLE find = FindFirstFile(folderPath, &findFileData);
	if (find) {
		do {
			tempCast = findFileData.cFileName;
			if (tempCast[0] != '.') { names.push_back(std::string(tempCast.begin(), tempCast.end())); }
		} while (FindNextFile(find, &findFileData));
		FindClose(find);
	}
	return names;
}