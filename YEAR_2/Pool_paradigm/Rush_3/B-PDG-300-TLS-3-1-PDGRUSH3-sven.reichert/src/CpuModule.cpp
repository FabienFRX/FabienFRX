#include "../include/CpuModule.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

Krell::CPUModule::CPUModule()
{
    cpuModel = "";
    cpuFrequency = "";
    cpuUsage = "";
}

void Krell::CPUModule::update()
{
    std::ifstream file("/proc/cpuinfo");
    std::string line;

    cpuModel.clear();
    cpuFrequency.clear();

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (line.find("model name") != std::string::npos && cpuModel.empty()) {
                std::istringstream iss(line);
                std::string temp;
                iss >> temp >> temp >> temp;
                while (iss >> temp) {
                    cpuModel += temp + " ";
                }
            } else if (line.find("cpu MHz") != std::string::npos && cpuFrequency.empty()) {
                std::istringstream iss(line);
                std::string temp;
                iss >> temp >> temp >> temp;
                iss >> temp;
                cpuFrequency = temp;
            }
        }
        file.close();
    }

    std::ifstream statFile("/proc/stat");
    if (statFile.is_open()) {
        std::getline(statFile, line);
        std::istringstream iss(line);
        std::string cpu;
        long user, nice, system, idle, iowait, irq, softirq, steal;
        iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        long idleTime = idle + iowait;
        long nonIdleTime = user + nice + system + irq + softirq + steal;
        long totalTime = idleTime + nonIdleTime;
        static long prevTotalTime = 0;
        static long prevIdleTime = 0;
        long deltaTotal = totalTime - prevTotalTime;
        long deltaIdle = idleTime - prevIdleTime;
        if (deltaTotal != 0) {
            cpuUsage = std::to_string((100 * (deltaTotal - deltaIdle)) / deltaTotal);
        } else {
            cpuUsage = "0";
        }
        prevTotalTime = totalTime;
        prevIdleTime = idleTime;
        statFile.close();
    }
}

void Krell::CPUModule::render(IDisplay &display)
{
    this->update();
    display.draw("CPU Model: " + cpuModel);
    display.draw("CPU Frequency: " + cpuFrequency + " MHz");
    display.draw("CPU Usage: " + cpuUsage + "%");
}

std::string Krell::CPUModule::getData() const
{
    return cpuModel + " " + cpuFrequency + " MHz " + cpuUsage + "%";
}