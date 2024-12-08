#include "pch.h"
#include "utils.h"

bool IsAdmin() {
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        std::cerr << "Failed to open process token. Error: " << GetLastError() << std::endl;
        return false;
    }

    std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(&CloseHandle)> tokenGuard(hToken, &CloseHandle);

    TOKEN_ELEVATION elevation{};
    DWORD size = 0;
    if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
        std::cerr << "Failed to get token information. Error: " << GetLastError() << std::endl;
        return false;
    }

    return elevation.TokenIsElevated != 0;
}

AutoLibrary::AutoLibrary(const wchar_t* libName) : handle(LoadLibraryW(libName)) {
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

AutoLibrary::AutoLibrary(AutoLibrary&& other) noexcept : handle(other.handle) {
    other.handle = nullptr;
}

AutoLibrary& AutoLibrary::operator=(AutoLibrary&& other) noexcept {
    if (this != &other) {
        if (handle) {
            FreeLibrary(handle);
        }
        handle = other.handle;
        other.handle = nullptr;
    }
    return *this;
}

HMODULE AutoLibrary::get() const {
    return handle;
}

AutoLibrary::operator HMODULE() const {
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