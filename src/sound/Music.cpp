#include "Sound.h"
#include <stdio.h>

#include <AL/al.h>
#include <AL/alc.h>

using namespace sound;

ALuint Music::source;
ALuint Music::buffer;
ALuint Music::streaming_source[1];

Music::Music (int argc, char** argv) {
  alutInit(&argc, argv);
  buffer = alutCreateBufferFromFile("snd/test.wav");
  alGenSources(1, &source);
  alSourcei(source, AL_LOOPING, AL_TRUE);
  alSourcei (source, AL_BUFFER, buffer);
}

Music::~Music () {
  alutExit();
}

void Music::play () {
  alSourcePlay(source);
  al_check_error("source playing");
}

ALenum Music::al_check_error(const char * message) {
  ALenum al_error;
  al_error = alGetError();

  if(AL_NO_ERROR != al_error) {
    printf("ERROR - %s  (%s)\n", alGetString(al_error), message);
  }
  return al_error;
}

