#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

std::string getAppDataPath() {
    char* appDataPath = nullptr;
    size_t len;
    if (_dupenv_s(&appDataPath, &len, "APPDATA") || appDataPath == nullptr) {
        return "";
    }
    std::string path(appDataPath);
    free(appDataPath);
    return path;
}

std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

bool writeFile(const std::string& filePath, const std::string& content) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return true;
}

int main() {
    std::string appDataPath = getAppDataPath();
    if (appDataPath.empty()) {
        return 1;
    }

    std::string filePath = appDataPath + "\\Vencord\\dist\\patcher.js";
    std::string fileContent = readFile(filePath);
    if (fileContent.empty()) {
        return 1;
    }
    std::string codeToRun = "alert(\"Hello World!\")";
    std::string searchString = "if(n.url.startsWith(\"https://open.spotify.com/embed/\")){";
    std::string replaceString = "(n.executeJavaScript(`" + codeToRun + "`));if(n.url.startsWith(\"https://open.spotify.com/embed/\")){";

    size_t pos = fileContent.find(searchString);
    if (pos != std::string::npos) {
        fileContent.replace(pos, searchString.length(), replaceString);
        if (writeFile(filePath, fileContent)) {
            std::cout << "Completed" << std::endl;
        }
    }

    return 0;
}