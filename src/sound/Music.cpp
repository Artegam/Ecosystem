#include "Sound.h"
#include <stdio.h>

#include <AL/al.h>
#include <AL/alc.h>

using namespace sound;

ALuint Music::source;
ALuint Music::streaming_source[1];

//thread Music::m(Music::play);

Music::Music () {

  const char * defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

  this->openal_output_device  = alcOpenDevice(defname);
  this->openal_output_context = alcCreateContext(this->openal_output_device, NULL);
  alcMakeContextCurrent(this->openal_output_context);

  // setup buffer and source
  ALuint internal_buffer;

  alGenBuffers(1, &internal_buffer);
  al_check_error("failed call to alGenBuffers");

//====================
// Playing the music

  // ======
  // Ajouts ici pour utiliser un fichier wave au lieu de sinus
  // ======
	ALenum format;
	ALvoid *data;
	ALsizei size, freq;
	//ALboolean loop = AL_FALSE;
	ALboolean loop = AL_TRUE;

	alGenSources((ALuint)1, &source);
	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, loop);

	alutLoadWAVFile((ALbyte*)"snd/test.wav", &format, &data, &size, &freq, &loop);
  al_check_error("loading wav file");

	fprintf(stdout, "size: %d\n", size);
	fprintf(stdout, "freq: %d\n", freq);
	fprintf(stdout, "format: %d\n", format);

	alBufferData(internal_buffer, format, data, size, freq);
  al_check_error("buffer copy");

  alSourcei(source, AL_BUFFER, internal_buffer);
  al_check_error("buffer binding");
}

Music::~Music () {
  // Stop the sources
  alSourceStopv(1, & streaming_source[0]);        //      streaming_source
  int ii;
  for (ii = 0; ii < 1; ++ii) {
    alSourcei(streaming_source[ii], AL_BUFFER, 0);
  }
  // Clean-up
  alDeleteSources(1, &streaming_source[0]);
  alDeleteBuffers(16, &streaming_source[0]);
  alcMakeContextCurrent(NULL);
	alcDestroyContext(openal_output_context);
	alcCloseDevice(openal_output_device);

  //m.detach();
}

void Music::play () {
	alSourcePlay(source);
  al_check_error("source playing");

  ALenum source_state;
	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
  al_check_error("source state get");

	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    al_check_error("source state get (playing)");
	}
}

ALenum Music::al_check_error(const char * message) {
  ALenum al_error;
  al_error = alGetError();

  if(AL_NO_ERROR != al_error) {
    printf("ERROR - %s  (%s)\n", alGetString(al_error), message);
  }
  return al_error;
}

