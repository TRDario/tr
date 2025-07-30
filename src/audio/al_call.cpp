#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/sysgfx/dialog.hpp"
#include <AL/al.h>

void tr::_validate_al_call(const char* file, int line, const char* function)
{
	switch (alGetError()) {
	case AL_INVALID_NAME:
		terminate("OpenAL validation error", std::format("Call to {} at {}:{} returned AL_INVALID_NAME.", function, file, line));
	case AL_INVALID_ENUM:
		terminate("OpenAL validation error", std::format("Call to {} at {}:{} returned AL_INVALID_ENUM.", function, file, line));
	case AL_INVALID_VALUE:
		terminate("OpenAL validation error", std::format("Call to {} at {}:{} returned AL_INVALID_VALUE.", function, file, line));
	case AL_INVALID_OPERATION:
		terminate("OpenAL validation error", std::format("Call to {} at {}:{} returned AL_INVALID_OPERATION.", function, file, line));
	case AL_OUT_OF_MEMORY:
		terminate("OpenAL validation error", std::format("Call to {} at {}:{} returned AL_OUT_OF_MEMORY.", function, file, line));
	default:
		break;
	}
}