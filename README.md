# Timer Resolution Measure

> [!IMPORTANT]  
> This program must be run with **administrator privileges** to function correctly. It requires elevated permissions to access system-level functions.

This program measures the sleep latency of a process by querying the system's timer resolution and calculating the delay between `Sleep()` calls. The program runs in an infinite loop until the user presses the Enter key, after which it calculates and outputs statistics based on collected samples.

## Features

- Retrieves the current system timer resolution using `NtQueryTimerResolution`.
- Measures the actual time spent during a `Sleep(n)` call in comparison to the expected time.
- Outputs statistics such as:
  - Max delay
  - Min delay
  - Average delay
  - Standard deviation
- Supports real-time priority mode for the process to reduce external interference.

## License
This program is licensed under the GPL-3.0 License - see the LICENSE file for details.