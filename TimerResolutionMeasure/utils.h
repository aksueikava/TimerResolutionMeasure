#pragma once

#include "pch.h"

bool IsAdmin();

class AutoLibrary {
public:
    AutoLibrary(const wchar_t* libName);
    ~AutoLibrary();
    HMODULE get() const;

    operator HMODULE() const { return handle; }

private:
    HMODULE handle;
};

void calculateStatistics(const std::vector<double>& delays, double& average, double& stdev);