
/**
        main.cpp
        test cases of plots

        @author Arvid Umrao <akumrao@yahoo.com> <arvind.umrao@harman.com>
        @version 0.1
 * 
 *     download wave test file from http://mauvecloud.net/sounds/index.html
 */

#define GRAPH  0

#include "stdio.h"
#include "plot/plot.h"


#include "audio/AudioFile.h"
using namespace std;
#include <fstream>

#include <string.h>


# include <limits.h>
typedef float       ieee754_float32_t;

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>


extern "C" {

int EMSCRIPTEN_KEEPALIVE fib(int n) {
  int i, t, a = 0, b = 1;
  for (i = 0; i < n; i++) {
    t = a + b;
    a = b;
    b = t;
  }
  return b;
}

int EMSCRIPTEN_KEEPALIVE setSrcImage(BYTE *jpegData,  unsigned long size)
{
    //EM_ASM({ console.log(size); });
    printf(" size = %d \n",  size);

    AudioFile<double> audioFile;
    std::vector<uint8_t> fileData(jpegData, jpegData+size);
    
    bool ret1 = audioFile.decodeWaveFile(fileData);


    //audioFile.printSummary();
    
    

   // Psycho_anal psycho_anal(44100);
    
   

    audioFile.printSummary();
    
     
     
     
    
    Plot plot;
    //plot.f_callback = std::bind( &AudioFile<double>::analyzeWave, audioFile );
    
    audioFile.play();

    int ret = plot.plot_graph("Plot");

    if (ret == EXIT_FAILURE) {
        printf("plot_graph return with status %d\n", ret);
        return EXIT_FAILURE;
    }



    printf(" ret = %d \n",  ret);

    EM_ASM({ console.log('setSrcImage done'); });
    return 99;
}

}
#else
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play
// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)

void my_audio_callback(void *userdata, Uint8 *stream, int len) {

    if (audio_len == 0)
        return;

    len = (len > audio_len ? audio_len : len);
    SDL_memcpy(stream, audio_pos, len); // simply copy from one buffer into the other
    //SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another

    //for (int x = 0; x < 100; ++x)
    //    printf("%d", stream[x]);
    //printf("\n");

    audio_pos += len;
    audio_len -= len;
}

void my_audio_callback1(void *userdata, Uint8 *stream, int len) {

    if (audio_len  <2)
        return;
    
    len = ( len > audio_len ? audio_len : len );

    for (int sampleIndex = 0; sampleIndex < len / 2; sampleIndex = sampleIndex + 2) {
        int16_t sampleAsInt = (audio_pos[sampleIndex + 1] << 8) | audio_pos[sampleIndex];
        ieee754_float32_t const u = sampleAsInt / 32768.0;
        SDL_memcpy(&stream[2 * sampleIndex], &u, sizeof (float));
    }

    audio_pos += len/2;
    audio_len -= len/2;
}


const char *SdlAudioFormatToString1(int sdlAudioType) {
  switch(sdlAudioType) {
  case AUDIO_U8: return "AUDIO_U8";
  case AUDIO_S8: return "AUDIO_S8";
  case AUDIO_U16LSB: return "AUDIO_U16LSB";
  case AUDIO_U16MSB: return "AUDIO_U16MSB";
  case AUDIO_S16LSB: return "AUDIO_S16LSB";
  case AUDIO_S16MSB: return "AUDIO_S16MSB";
  case AUDIO_F32: return "AUDIO_F32";
  case AUDIO_S32LSB: return "AUDIO_S32LSB";
  case AUDIO_S32MSB: return "AUDIO_S32MSB";
  
  //case AUDIO_F32LSB: return "AUDIO_F32LSB";
  case AUDIO_F32MSB: return "AUDIO_F32MSB";
  
  default: return "(unknown)";
  }
}


int main(int argc, char* argv[]) {

    printf("plot_graph\n");
   
   /*
   if (SDL_Init(SDL_INIT_AUDIO) < 0)
			return 1;

	// local variables
	static Uint32 wav_length; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music
	static SDL_AudioSpec des_wav_spec; // the specs of our piece of music
	
	
	// the specs, length and buffer of our wav are filled
	if( SDL_LoadWAV("/root/Desktop/delete/test.wav", &wav_spec, &wav_buffer, &wav_length) == NULL ){
	  return 1;
	}
	// set the callback function
	wav_spec.callback = my_audio_callback1;
	wav_spec.userdata = NULL;
        wav_spec.format= AUDIO_F32;
	// set our global static variables
	audio_pos = wav_buffer; // copy sound buffer
	audio_len = wav_length; // copy file length
	wav_spec.samples = 2048;

	if ( SDL_OpenAudio(&wav_spec, &des_wav_spec) < 0 ){
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  exit(-1);
	}
        
        
       printf("obtainedSpec   %d  freq with audio format %s, %d channels, and %d samples buffer.\n",
       (int)wav_spec.freq, SdlAudioFormatToString1(wav_spec.format), wav_spec.channels,  wav_spec.samples);
        
       printf("obtainedSpec   %d  freq with audio format %s, %d channels, and %d samples buffer.\n",
       (int)des_wav_spec.freq, SdlAudioFormatToString1(des_wav_spec.format), des_wav_spec.channels,  des_wav_spec.samples);

	

	SDL_PauseAudio(0);

	// wait until we're don't playing
	while ( audio_len > 2 ) {
		SDL_Delay(100); 
	}
	
	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);

        
        return 0;
   */
   
   AudioFile<double> audioFile;

    
   
    //audioFile.analyzeWave();
   
           

   // Psycho_anal psycho_anal(44100);
    
   
   // plotwin_list = psycho_anal.plotwin_list;

   
   // bool ret1 = audioFile.load("/root/Desktop/delete/test2.wav");
   
    if(argc == 2)
    bool ret1 = audioFile.load(argv[1]);
    else
    bool ret1 = audioFile.load("./outputs16.wav");

    audioFile.printSummary();
    
  /*

    {
        //populate caption list
        captionlist caption_list = NULL;

        caption_list = push_back_caption(caption_list, "Download", 0, 0x0000FF);
        caption_list = push_back_caption(caption_list, "Upload", 1, 0xFF0000);

        //print_list_caption(caption_list);

        //populate coordinate list
        coordlist coordinate_list = NULL;

        coordinate_list = push_back_coord(coordinate_list, 0, 0, 0);
        coordinate_list = push_back_coord(coordinate_list, 0, 1, 90);
        coordinate_list = push_back_coord(coordinate_list, 0, 2, 84);
        coordinate_list = push_back_coord(coordinate_list, 0, 3, 98);
        coordinate_list = push_back_coord(coordinate_list, 0, 4, 94);
        coordinate_list = push_back_coord(coordinate_list, 0, 5, 85);
        coordinate_list = push_back_coord(coordinate_list, 0, 6, 90);
        coordinate_list = push_back_coord(coordinate_list, 0, 7, 99);
        coordinate_list = push_back_coord(coordinate_list, 0, 8, 94);
        coordinate_list = push_back_coord(coordinate_list, 0, 9, 80);


        coordinate_list = push_back_coord(coordinate_list, 1, 0, 0);
        coordinate_list = push_back_coord(coordinate_list, 1, 1, 92);
        coordinate_list = push_back_coord(coordinate_list, 1, 2, 90);
        coordinate_list = push_back_coord(coordinate_list, 1, 3, 98);
        coordinate_list = push_back_coord(coordinate_list, 1, 4, 92);
        coordinate_list = push_back_coord(coordinate_list, 1, 5, 82);
        coordinate_list = push_back_coord(coordinate_list, 1, 6, 98);
        coordinate_list = push_back_coord(coordinate_list, 1, 7, 94);
        coordinate_list = push_back_coord(coordinate_list, 1, 8, 90);
        coordinate_list = push_back_coord(coordinate_list, 1, 9, 80);

  
        //print_list_coord(coordinate_list);

        //populate plot parameter object
        plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list);

        // params->screen_width = 500;
        // params->screen_heigth = 500;


        params->max.x = 10;
        params->max.y = 100;


        params->max.x = 10;
        params->max.y = 100;


        //  Plot_Window_params win_param;
        push_back_plot_win( params);

    }
    {
        //populate caption list
        captionlist caption_list = NULL;

        caption_list = push_back_caption(caption_list, "live", 0, 0x0000FF);



        //print_list_caption(caption_list);

        //populate coordinate list
        coordlist coordinate_list = NULL;

        coordinate_list = push_back_coord(coordinate_list, 0, -5, -2);
        coordinate_list = push_back_coord(coordinate_list, 0, -4, -9);
        coordinate_list = push_back_coord(coordinate_list, 0, -3, -9);
        coordinate_list = push_back_coord(coordinate_list, 0, -2, -3);
        coordinate_list = push_back_coord(coordinate_list, 0, -1, -8);
        coordinate_list = push_back_coord(coordinate_list, 0, 0, 0);
        coordinate_list = push_back_coord(coordinate_list, 0, 0, 9);
        coordinate_list = push_back_coord(coordinate_list, 0, 1, 5);audio/AudioFile.cpp:77:29: error: expected identifier before '{' token
        coordinate_list = push_back_coord(coordinate_list, 0, 2, 12);
        coordinate_list = push_back_coord(coordinate_list, 0, 3, 18);
        coordinate_list = push_back_coord(coordinate_list, 0, 4, 3);

      

        //populate plot parameter object
        plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list);

        params->max.x = +5;
        params->max.y = 20;
        params->min.x = -5;
        params->min.y = -20;

        //  Plot_Window_params win_param;
        push_back_plot_win(params);

    }
*/
   /*
      {
          //populate caption list
          captionlist caption_list = NULL;

          caption_list = push_back_caption(caption_list, "Sine", 0, 0x0000FF);

          coordlist coordinate_list = NULL;

          for (int i = 0; i < 18; ++i) {
              coordinate_list = push_back_coord(coordinate_list, 0, i, sin(i));
          }
        
            

          print_list_coord(coordinate_list);

          //populate plot parameter object
          plot_params *params = new plot_params("x", "Y", caption_list, coordinate_list);

          //  Plot_Window_params win_param;
          push_back_plot_win(params);

      }
     */


#if GRAPH ==1
    Plot plot; //std::bind( &Foo::print_add, foo, _1 );
    // using std::placeholders::_1;
    //  plot.f_callback = std::bind( &AudioFile<double>::analyzeWave, &audioFile );
    //    std::function<bool(void)> d1 =  std::bind( &AudioFile<double>::analyzeWave, &audioFile );

    plot.f_callback_stop = std::bind(&AudioFile<double>::stop, &audioFile);
#endif
   
    audioFile.printSummary();
    audioFile.play();

#if GRAPH ==1
    int ret = plot.plot_graph("Plot");

    if (ret == EXIT_FAILURE) {
        printf("plot_graph return with status %d\n", ret);
        return EXIT_FAILURE;
    }
#else
    
    cout << "Press a Key to exit main....." << endl <<endl;

    getchar();
    
#endif

    return EXIT_SUCCESS;
}


#endif