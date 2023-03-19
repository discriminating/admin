#include <iostream>
#include <Windows.h>

bool isExecutableInPath(const char* exeName) {
	// Get the PATH environment variable
	char pathVar[MAX_PATH];
	if (!GetEnvironmentVariableA("PATH", pathVar, MAX_PATH)) {
		std::cout << "Failed to get PATH environment variable" << std::endl;
		return false;
	}

	// Split the PATH into individual directories
	char* context;
	char* dir = strtok_s(pathVar, ";", &context);
	while (dir) {
		// Check if the executable file exists in the directory
		std::string fullPath = std::string(dir) + "\\" + exeName;
		if (GetFileAttributesA(fullPath.c_str()) != INVALID_FILE_ATTRIBUTES) return true;

		// Move on to the next directory in the PATH
		dir = strtok_s(NULL, ";", &context);
	} return false;
}

int main(int argc, char* argv[]) {
	// Check if the executable is in the PATH
	const char* exeName = strrchr(argv[0], '\\') ? strrchr(argv[0], '\\') + 1 : argv[0];
	if (!isExecutableInPath(exeName)) {
		// Prompt the user to add the executable to the PATH
		std::cout << exeName << " is not in your PATH environment variable. This can cause errors and annoyance upon using." << std::endl;
	}

	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <exe_path> [args...]" << std::endl;
		return 1;
	}

	// Get the command-line arguments
	const char* exePath = argv[1];
	std::string args;
	for (int i = 2; i < argc; i++) { args += std::string(argv[i]) + " "; }

	// Start the process with admin privileges
	SHELLEXECUTEINFOA shellInfo = { sizeof(shellInfo) };
	shellInfo.lpVerb = "runas"; // Request admin privileges
	shellInfo.lpFile = exePath;
	shellInfo.lpParameters = args.c_str(); // Pass additional arguments
	shellInfo.nShow = SW_SHOW;
	if (!ShellExecuteExA(&shellInfo)) {
		std::cout << "Failed to run process with admin privileges" << std::endl;
		return 2;
	} return 0;
}
