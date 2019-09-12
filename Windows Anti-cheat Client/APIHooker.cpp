#include "APIHooker.h"

const PHookAddressList APIHooker::getHookAddressList(void) {
	return &APIHooker::mHookAddressList;
}

bool APIHooker::InitizeHook(void) {
	if (APIHooker::IsHookInitized == true) {
		if (!APIHooker::ClearAPIHook()) {
			return false;
		}
	}

	if (APIHooker::InitizeJVMHook() && APIHooker::InitizeAPIHook()) {
		APIHooker::IsHookInitized = true;
		return true;
	} else {
		return false;
	}
}