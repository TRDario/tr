#include "../../include/tr/audio/al_call.hpp"
#include <AL/al.h>

void tr::audio::validate_al_call(const char* file, int line, const char* function)
{
	const ALenum error{alGetError()};
	if (error != AL_NO_ERROR) {
		TR_LOG(log, tr::severity::FATAL, "OpenAL validation error.");
		switch (error) {
		case AL_INVALID_NAME:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned AL_INVALID_NAME.", function, file, line);
		case AL_INVALID_ENUM:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned AL_INVALID_ENUM.", function, file, line);
		case AL_INVALID_VALUE:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned AL_INVALID_VALUE.", function, file, line);
		case AL_INVALID_OPERATION:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned AL_INVALID_OPERATION.", function, file, line);
		case AL_OUT_OF_MEMORY:
			TR_LOG_CONTINUE(log, "Call to {} at {}:{} returned AL_OUT_OF_MEMORY.", function, file, line);
		}
		std::abort();
	}
}