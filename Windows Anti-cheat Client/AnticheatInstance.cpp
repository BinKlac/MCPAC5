#include "AnticheatInstance.h"

AnticheatInstance* AnticheatInstance::mACInstance = nullptr;

AnticheatInstance* AnticheatInstance::getACInstance() {
	if (AnticheatInstance::mACInstance == nullptr) {
		AnticheatInstance::mACInstance = new AnticheatInstance();
	}
	return AnticheatInstance::mACInstance;
}
