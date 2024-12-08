#include "pch.h"
#include "utils.h"
#include "conio.h"

typedef NTSTATUS(NTAPI* NtQueryTimerResolution_t)(PULONG MinimumResolution, PULONG MaximumResolution, PULONG CurrentResolution);

int main() {
#ifndef _DEBUG
    if (!IsAdmin()) {
        std::cerr << "Administrator privileges are required. The program will not function correctly and will now exit.\n";
        std::cout << "Press Enter to continue...\n";
        std::cin.get();
        return 0;
    }
#else
    std::cout << "[DEBUG] Administrator privilege check is skipped in debug mode.\n"
        << "         Note: Some functionality might not work correctly without elevated permissions.\n\n";
#endif

    int sleep_duration = 1;

    AutoLibrary ntdll(L"ntdll.dll");
    NtQueryTimerResolution_t NtQueryTimerResolution = (NtQueryTimerResolution_t)GetProcAddress(ntdll.get(), "NtQueryTimerResolution");
    if (!NtQueryTimerResolution) {
        std::cerr << "Failed to get address of NtQueryTimerResolution\n";
        return 1;
    }

    ULONG minimum_resolution, maximum_resolution, current_resolution;
    LARGE_INTEGER start, end, freq;
    std::vector<double> sleep_delays;

    if (!QueryPerformanceFrequency(&freq)) {
        std::cerr << "QueryPerformanceFrequency failed\n";
        return 1;
    }

    if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
        std::cerr << "SetPriorityClass failed\n";
        return 1;
    }

    std::cout << "Press Enter to stop the cycle and calculate results...\n";

    while (!_kbhit()) {
        NTSTATUS status = NtQueryTimerResolution(&minimum_resolution, &maximum_resolution, &current_resolution);
        if (!NT_SUCCESS(status)) {
            std::cerr << "NtQueryTimerResolution failed\n";
            return 1;
        }

        QueryPerformanceCounter(&start);
        Sleep(sleep_duration);
        QueryPerformanceCounter(&end);

        double delta_s = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
        double delta_ms = delta_s * 1000;
        double delta_from_sleep = delta_ms - sleep_duration;

        std::cout << std::fixed << std::setprecision(4)
            << "Resolution: " << (current_resolution / 10000.0) << "ms, "
            << "Sleep(n=" << sleep_duration << ") slept " << delta_ms << "ms "
            << "(delta: " << delta_from_sleep << ")\n";

        sleep_delays.push_back(delta_from_sleep);
        Sleep(1000);
    }

    std::cin.get();
    sleep_delays.erase(sleep_delays.begin());

    std::nth_element(sleep_delays.begin(), sleep_delays.begin() + sleep_delays.size() / 2, sleep_delays.end());
    std::sort(sleep_delays.begin(), sleep_delays.end());

    size_t size = sleep_delays.size();

    double sum = 0.0;
    for (double delay : sleep_delays) {
        sum += delay;
    }

    double average = sum / size;

    double standard_deviation = 0.0;
    for (double delay : sleep_delays) {
        standard_deviation += pow(delay - average, 2);
    }

    double stdev = sqrt(standard_deviation / (size - 1));

    std::cout << "Results from " << size << " samples (excluding the first sample, which is often inaccurate)\n";
    std::cout << "Max: " << std::setw(10) << std::fixed << std::setprecision(4) << sleep_delays.back() << " ms\n";
    std::cout << "Avg: " << std::setw(10) << std::fixed << std::setprecision(4) << average << " ms\n";
    std::cout << "Min: " << std::setw(10) << std::fixed << std::setprecision(4) << sleep_delays.front() << " ms\n";
    std::cout << "STDEV: " << std::setw(10) << std::fixed << std::setprecision(4) << stdev << " ms\n";

    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0;
}