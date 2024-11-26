#include "pch.h"
#include "utils.h"

bool IsAdmin() {
    bool admin = false;
    HANDLE hToken = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation{};
        DWORD size;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
            admin = elevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }

    return admin;
}

AutoLibrary::AutoLibrary(const wchar_t* libName) {
    handle = LoadLibraryW(libName);
    if (!handle) {
        DWORD error = GetLastError();
        std::wcerr << L"Failed to load library " << libName << L". Error code: " << error << std::endl;
        throw std::runtime_error("Failed to load library");
    }
}

AutoLibrary::~AutoLibrary() {
    if (handle) {
        FreeLibrary(handle);
    }
}

HMODULE AutoLibrary::get() const {
    return handle;
}

void calculateStatistics(const std::vector<double>& delays, double& average, double& stdev) {
    size_t size = delays.size();

    double sum = std::accumulate(delays.begin(), delays.end(), 0.0);
    double sq_sum = std::accumulate(delays.begin(), delays.end(), 0.0, [](double acc, double val) {
        return acc + val * val;
        });

    average = sum / size;
    stdev = std::sqrt(sq_sum / (size - 1) - average * average);
}