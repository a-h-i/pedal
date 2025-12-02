#pragma once

#define FATAL(x) Serial.print("FATAL: "); Serial.println(x); if (CrashReport) Serial.print(CrashReport)
#define DEBUG(x) Serial.print("DEBUG: "); Serial.println(x)