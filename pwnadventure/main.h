#pragma once

struct Vector3 {
    float x;
    float y;
    float z;
};

/*
struct Item {
    char name[0x30];
    DWORD count;
};
*/

const char* DEBUG_PRINT = "[*] MSG: %s\n";
const char* TP = "tp \x00";
const char* TPX = "tpx \x00";
const char* TPY = "tpy \x00";
const char* TPZ = "tpz \x00";
// const char* GIVE = "give \x00";

const char* FORMAT = "%f\x00";
const char* FORMAT_XYZ = "%f %f %f\x00";
// const char* FORMAT_ITEM = "%s %u\x00";
