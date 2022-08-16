/*****************************************************************************
* | File      	:   EPD_2IN13_V2_test.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper(V2) test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-13
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "EPD_2in13_V2.h"
#include <time.h>
#include <strings.h>
#include "sunrise.h"

typedef struct hour_min{
    double hour;
    double min;
};

int EPD_2in13_V2_test(void)
{
    printf("EPD_2IN13_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_SetMirroring(MIRROR_HORIZONTAL); //
    Paint_Clear(WHITE);

#if 0   // show bmp
    printf("show window BMP-----------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 10, 10);
    EPD_2IN13_V2_Display(BlackImage);
    DEV_Delay_ms(2000);

    printf("show bmp------------------------\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/2in13-v2.bmp", 0, 0);
    EPD_2IN13_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif    
    

#if 0   //show image for array    
    printf("Mne pic\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawBitMap(gImage_mono);

    EPD_2IN13_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 0   // Drawing on the image
    printf("Drawing\r\n");
    //1.Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);

    Paint_DrawLine(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 10, 20, 60, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(85, 10, 135, 60, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawLine(45, 15, 45, 55, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(25, 35, 70, 35, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawCircle(45, 35, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(110, 35, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    Paint_DrawString_EN(140, 15, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawNum(140, 40, 123456789, &Font16, BLACK, WHITE);

    Paint_DrawString_CN(140, 60, "���abc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(5, 65, "΢ѩ����", &Font24CN, WHITE, BLACK);

    EPD_2IN13_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
#endif

#if 1   //Partial refresh, example shows time    		
    printf("Partial refresh\r\n");
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_DisplayPartBaseImage(BlackImage);

    EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
    Paint_SelectImage(BlackImage);


    
    
     // variables to store the date and time components
    int hours, minutes, seconds, day, month, year;
 
    // `time_t` is an arithmetic time type
    time_t now;
    
    // Obtain current time
    // `time()` returns the current time of the system as a `time_t` value
    time(&now);
 
    // Convert to local time format and print to stdout
    printf("Today is %s", ctime(&now));
 
    // localtime converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);
 
    hours = local->tm_hour;         // get hours since midnight (0-23)
    minutes = local->tm_min;        // get minutes passed after the hour (0-59)
    seconds = local->tm_sec;        // get seconds passed after a minute (0-59)
 
    day = local->tm_mday;            // get day of month (1 to 31)
    month = local->tm_mon +1;      // get month of year (0 to 11)
    year = local->tm_year ;   // get year since 1900
    
    // print local time
    printf("Time is %02d:%02d:%02d\n", hours, minutes, seconds);

    // print the current date
    printf("Date is: %02d/%02d/%d\n", day, month, year);
    
    #define STRMAX 12
    char s_month[STRMAX];

    switch(month)
    {
        case 1:
           strncpy(s_month,"January",STRMAX);
            break;
        case 2:
            strncpy(s_month ,"February", STRMAX);
            break;
        case 3:
            strncpy(s_month ,"March", STRMAX);
            break;
        case 4:
            strncpy(s_month ,"April", STRMAX);
            break;
        case 5:
            strncpy(s_month ,"May", STRMAX);
            break;
        case 6:
           strncpy( s_month ,"June", STRMAX);
            break;
        case 7:
            strncpy(s_month ,"July", STRMAX);
            break;
        case 8:
            strncpy(s_month ,"August", STRMAX);
            break;
        case 9:
            strncpy(s_month ,"September", STRMAX);
            break;
        case 10:
            strncpy(s_month ,"October", STRMAX);
            break;
        case 11:
            strncpy(s_month ,"November", STRMAX);
            break;
        case 12:
            strncpy(s_month ,"December", STRMAX);
            break;
    }
    printf("\n HELLT %s",s_month);

#endif

#if 1
      //int year,month,day;
      double lon = -85.928618, lat = 38.291959;
      double daylen, civlen, nautlen, astrlen;
      double rise, set, civ_start, civ_end, naut_start, naut_end,
             astr_start, astr_end;
      int    rs, civ, naut, astr;
      struct hour_min set_st;
      struct hour_min rise_st;
      //time zone offset ut to est is -4 hours
      int timezone_offset = -4;
        daylen  = day_length(year,month,day,lon,lat);
        civlen  = day_civil_twilight_length(year,month,day,lon,lat);
        nautlen = day_nautical_twilight_length(year,month,day,lon,lat);
        astrlen = day_astronomical_twilight_length(year,month,day,
            lon,lat);

        printf( "Day length:                 %5.2f hours\n", daylen );
        printf( "With civil twilight         %5.2f hours\n", civlen );
        printf( "With nautical twilight      %5.2f hours\n", nautlen );
        printf( "With astronomical twilight  %5.2f hours\n", astrlen );
        printf( "Length of twilight: civil   %5.2f hours\n",
            (civlen-daylen)/2.0);
        printf( "                  nautical  %5.2f hours\n",
            (nautlen-daylen)/2.0);
        printf( "              astronomical  %5.2f hours\n",
            (astrlen-daylen)/2.0);

        rs   = sun_rise_set         ( year, month, day, lon, lat,
                                        &rise, &set );
        civ  = civil_twilight       ( year, month, day, lon, lat,
                                     &civ_start, &civ_end );
        naut = nautical_twilight    ( year, month, day, lon, lat,
                                        &naut_start, &naut_end );
        astr = astronomical_twilight( year, month, day, lon, lat,
                                        &astr_start, &astr_end );
        double intpart;
        double min_60 = modf(rise,&intpart );
        rise_st.hour =intpart + timezone_offset;
        rise_st.min=ceil(min_60 *60);
        min_60 = modf(set,&intpart );
        set_st.hour =intpart + timezone_offset;
        set_st.min=ceil(min_60 *60);
        printf( "Sun at south %5.2fh eastern\n", (rise+set)/2.0 );
        
        switch( rs )
        {
            case 0:
                printf( "Sun rises %2.0f:%2.0f eastern, sets %2.0f:%2.0f eastern\n",
                        rise_st.hour,rise_st.min, set_st.hour,set_st.min );
                break;
            case +1:
                printf( "Sun above horizon\n" );
                break;
            case -1:
                printf( "Sun below horizon\n" );
                break;
        }

        switch( civ )
        {
            case 0:
                printf( "Civil twilight starts %2.0f:%2.0f , "
                        "ends %5.2fh UT\n", ceil(modf(civ_start,&intpart)*60),intpart+timezone_offset, civ_end );
                break;
            case +1:
                printf( "Never darker than civil twilight\n" );
                break;
            case -1:
                printf( "Never as bright as civil twilight\n" );
                break;
        }

        switch( naut )
        {
            case 0:
                printf( "Nautical twilight starts %5.2fh, "
                        "ends %5.2fh UT\n", naut_start, naut_end );
                break;
            case +1:
                printf( "Never darker than nautical twilight\n" );
                break;
            case -1:
                printf( "Never as bright as nautical twilight\n" );
                break;
        }

        switch( astr )
        {
            case 0:
                printf( "Astronomical twilight starts %5.2fh, "
                        "ends %5.2fh UT\n", astr_start, astr_end );
                break;
            case +1:
                printf( "Never darker than astronomical twilight\n" );
                break;
            case -1:
                printf( "Never as bright as astronomical twilight\n" );
                break;
         }
#endif

#if 1   //show image for array    
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    //Paint_DrawNum(140, 40, 123456789, &Font16, BLACK, WHITE);
    //Paint_DrawBitMap(gImage_tank); //changed this
    //
    Paint_DrawString_EN(50, 0, s_month, &Font20, WHITE, BLACK);
    Paint_DrawLine(0,20,250,20, BLACK,DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    PAINT_TIME sPaint_times;
    sPaint_times.Hour = hours;
    sPaint_times.Min = minutes;
    sPaint_times.Sec = 00;
    Paint_DrawString_EN(50, 50, &sPaint_times, &Font20, WHITE, BLACK);
    Paint_DrawNum(50,80,month , &Font16, BLACK, WHITE);
    Paint_DrawNum(70,80,day , &Font16, BLACK, WHITE);
    Paint_DrawNum(90,80,year , &Font16, BLACK, WHITE);
    EPD_2IN13_V2_Display(BlackImage);
    DEV_Delay_ms(20000);
#endif

//test /home/mcg/Documents/waveshare/RaspberryPi&JetsonNano/c/bin/DEV_Config.o
#if 0  //Partial refresh, example shows time    		
    printf("Partial refresh\r\n");
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_DisplayPartBaseImage(BlackImage);

    EPD_2IN13_V2_Init(EPD_2IN13_V2_PART);
    Paint_SelectImage(BlackImage);
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    UBYTE num = 20;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        Paint_ClearWindows(140, 90, 140 + Font20.Width * 7, 90 + Font20.Height, WHITE);
        Paint_DrawTime(140, 90, &sPaint_time, &Font20, WHITE, BLACK);

        num = num - 1;
        if(num == 0) {
            break;
        }
        EPD_2IN13_V2_DisplayPart(BlackImage);
        DEV_Delay_ms(500);//Analog clock 1s

#endif
    printf("Clear...\r\n");

    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();
    DEV_Delay_ms(2000);//Analog clock 1s

    printf("Goto Sleep...\r\n");
    EPD_2IN13_V2_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(1000);//Analog clock 1s

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

