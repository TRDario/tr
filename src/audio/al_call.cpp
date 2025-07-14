#include "../../include/tr/audio/al_call.hpp"
#include "../../include/tr/utility.hpp"
#include <AL/al.h>

void tr::_validate_al_call(const char* file, int line, const char* function) noexcept
{
	switch (alGetError()) {
	case AL_INVALID_NAME:
		TR_LOG(log, severity::FATAL, "Call to OpenAL function {} at {}:{} returned AL_INVALID_NAME.", function, file, line);
		std::abort();
	case AL_INVALID_ENUM:
		TR_LOG(log, severity::FATAL, "Call to OpenAL function {} at {}:{} returned AL_INVALID_ENUM.", function, file, line);
		std::abort();
	case AL_INVALID_VALUE:
		TR_LOG(log, severity::FATAL, "Call to OpenAL function {} at {}:{} returned AL_INVALID_VALUE.", function, file, line);
		std::abort();
	case AL_INVALID_OPERATION:
		TR_LOG(log, severity::FATAL, "Call to OpenAL function {} at {}:{} returned AL_INVALID_OPERATION.", function, file, line);
		std::abort();
	case AL_OUT_OF_MEMORY:
		TR_LOG(log, severity::FATAL, "Call to OpenAL function {} at {}:{} returned AL_OUT_OF_MEMORY.", function, file, line);
		std::abort();
	default:
		break;
	}
}