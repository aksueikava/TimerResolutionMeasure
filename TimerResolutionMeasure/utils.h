#pragma once

#include "pch.h"

bool IsAdmin();

class AutoLibrary {
public:
    explicit AutoLibrary(const wchar_t* libName);
    ~AutoLibrary();

    AutoLibrary(AutoLibrary&& other) noexcept;
    AutoLibrary& operator=(AutoLibrary&& other) noexcept;

    HMODULE get() const;
    operator HMODULE() const;

private:
    HMODULE handle;

    AutoLibrary(const AutoLibrary&) = delete;
    AutoLibrary& operator=(const AutoLibrary&) = delete;
};

void calculateStatistics(const std::vector<double>& delays, double& average, double& stdev);