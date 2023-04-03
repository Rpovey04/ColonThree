#include <windows.h>
#include <string>
#include <vector>

class DatasetHandle {
private:
	// generic directory code
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
	
	static std::string root;

public:
	// methods for paths
	static void setPathToArchive(std::string p) {
		root = p;
	}
	static std::vector<std::string> getJpgPaths() {
		std::vector<std::string> folders = getAvailableDirectories(root + "/*");
		std::vector<std::string> potentialImages;
		std::vector<std::string> jpgPaths;
		for (int i = 0; i < folders.size(); i++) {
			potentialImages = getAvailableDirectories(root + "/" + folders[i] + "/*");
			for (int j = 0; j < potentialImages.size(); j++) {
				if (potentialImages[j][potentialImages[j].length() - 1] == 'g' && potentialImages[j][potentialImages[j].length() - 2] == 'p' && potentialImages[j][potentialImages[j].length() - 3] == 'j') {
					jpgPaths.push_back(root + "/" + folders[i] + "/" + potentialImages[j]);
				}
			}
		}
		return jpgPaths;
	}
};

std::string DatasetHandle::root = "";
