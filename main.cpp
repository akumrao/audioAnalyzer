
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

int main(int argc, char* argv[]) {

   printf("plot_graph\n");
     
    plotwinlist plotwin_list = NULL;

    Psycho_anal psycho_anal(44100);
    
   
    plotwin_list = psycho_anal.plotwin_list;
    
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

        params->scale.x = 1;
        params->scale.y = 10;
        params->max.x = 10;
        params->max.y = 100;

            params->scale.x = 1;
            params->scale.y = 10;
            params->max.x = 10;
            params->max.y = 100;


        //  Plot_Window_params win_param;
        plotwin_list = push_back_plot_win(plotwin_list, params);

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
        coordinate_list = push_back_coord(coordinate_list, 0, 1, 5);
        coordinate_list = push_back_coord(coordinate_list, 0, 2, 12);
        coordinate_list = push_back_coord(coordinate_list, 0, 3, 18);
        coordinate_list = push_back_coord(coordinate_list, 0, 4, 3);

      

        //populate plot parameter object
        plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list);
        params->scale.x = 1;
        params->scale.y = 2;
        params->max.x = +5;
        params->max.y = 20;
        params->min.x = -5;
        params->min.y = -20;

        //  Plot_Window_params win_param;
        plotwin_list = push_back_plot_win(plotwin_list, params);

    }

   
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
            params->scale.x = .25;
            params->scale.y = .2;
         //  Plot_Window_params win_param;
         plotwin_list = push_back_plot_win(plotwin_list, params);

     }
     */

    Plot plot;


    int ret = plot.plot_graph(plotwin_list, "Plot");

    if (ret == EXIT_FAILURE) {
        printf("plot_graph return with status %d\n", ret);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}