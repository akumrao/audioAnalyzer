
/**
        main.cpp
        test cases of plots

        @author Arvid Umrao <akumrao@yahoo.com> <arvind.umrao@harman.com>
        @version 0.1
 */
#include "stdio.h"
#include "plot/plot.h"


#include "audio/AudioFile.h"
using namespace std;
#include <fstream>
#include "audio/Psycho_anal.h"
#include <string.h>

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
    
    
    plotwinlist plotwin_list = NULL;

   // Psycho_anal psycho_anal(44100);
    
   
  // plotwin_list = psycho_anal.plotwin_list;
    
     plotwin_list = audioFile.plotwin_list;


   // audioFile.printSummary();
    
     
     
     
    
    Plot plot;
    plot.f_callback = std::bind( &AudioFile<double>::analyzeWave, audioFile );

    int ret = plot.plot_graph(plotwin_list, "Plot");

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

int main(int argc, char* argv[]) {

   printf("plot_graph\n");
   
   
   AudioFile<double> audioFile;

    
   
    //audioFile.analyzeWave();
   
           

    Psycho_anal psycho_anal(44100);
    
   
   // plotwin_list = psycho_anal.plotwin_list;

    
    bool ret1 = audioFile.load("/root/Desktop/delete/test.wav");

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
   

    Plot plot;  //std::bind( &Foo::print_add, foo, _1 );
    // using std::placeholders::_1;
  //  plot.f_callback = std::bind( &AudioFile<double>::analyzeWave, audioFile );

    int ret = plot.plot_graph("Plot");

    if (ret == EXIT_FAILURE) {
        printf("plot_graph return with status %d\n", ret);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


#endif