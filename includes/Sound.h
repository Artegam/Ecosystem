#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>
//#include <thread>

#include <AL/al.h>
#include <AL/alc.h>

#ifdef LIBAUDIO
#include <audio/wave.h>
#define BACKEND	"libaudio"
#else
#include <AL/alut.h>
#define BACKEND "alut"
#endif

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return;		\
	}

using namespace std;

namespace sound {

  class Music {
    private:
      //static thread m;
      static ALuint streaming_source[1];
      ALCcontext * openal_output_context;
      ALCdevice * openal_output_device;
      static ALuint source;

    public:
      Music ();
      ~Music ();

      static void play ();
      static ALenum al_check_error (const char * message);
  };
}

#endif
