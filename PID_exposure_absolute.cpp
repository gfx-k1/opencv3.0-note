           if((cicleCalculator > 8))   //--------------- GRPH OF R G B CHANNEL ----------------------/
           {


               if(!bgr_image.empty() )
               {
                       /// Separate the image in 3 places ( B, G and R )
                       vector<Mat> bgr_planes;
                       split( bgr_image, bgr_planes );

                       /// Establish the number of bins
                       int histSize = 256;

                       /// Set the ranges ( for B,G,R) )
                       float range[] = { 0, 256 } ;
                       const float* histRange = { range };

                       bool uniform = true; bool accumulate = false;

                       Mat b_hist, g_hist, r_hist, w_hist;

                       /// Compute the histograms:
                       calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
                       calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
                       calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );


                       // Draw the histograms for B, G and R
                       int hist_w = 512; int hist_h = 400;
                       int bin_w = cvRound( (double) hist_w/histSize );
                       yGreenExposure.clear();
                       yBlueExposure.clear();
                       BlueLevel = 0;
                       GreenRange = 0;


                       Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

                       /// Normalize the result to [ 0, histImage.rows ]
                       normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
                       normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
                       normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
                      // normalize(w_hist, w_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

                       /// Draw for each channel
                       for( int i = 1; i < histSize; i++ )
                       {
                           line( histImage, Point( bin_w*(i-1),hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                                            Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                                            Scalar( 255, 0, 0), 2, 8, 0  );
                           line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                                            Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                                            Scalar( 0, 255, 0), 2, 8, 0  );
                           line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                                            Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                                            Scalar( 0, 0, 255), 2, 8, 0  );
                           line( histImage, Point( 450, 400 ) ,Point( 450, hist_h - cvRound(g_hist.at<float>(i)) ), Scalar( 112, 255, 0), 2, 8, 0  );
                           line( histImage, Point( 0, 400-240 ) ,Point( 500, 400-240 ), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 0, 400-220 ) ,Point( 500, 400-220 ), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 0, 400-200 ) ,Point( 500, 400-200 ), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 0, 400-180 ) ,Point( 500, 400-180 ), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 0, 400-160 ) ,Point( 500, 400-160 ), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 0, 400-140 ) ,Point( 500, 400-140 ), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 0, 400-100 ) ,Point( 500, 400-100 ), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 20, 0 ) ,Point( 20, 400), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 6, 0 ) ,Point( 6, 400), Scalar( 112, 255, 0), 1, 8, 0  );
                           line( histImage, Point( 80, 0 ) ,Point( 80, 400 ), Scalar( 112, 255, 0), 1, 8, 0  );

                           //----- line is grph line for better see the righ channel value
                           //----- bin_w*(i) in these example is +/- 2 time histsize so I have set if  ((3 < i) && (i < 40)) 
                           //----- hist_h - cvRound(g_hist.at<float>(i)) = yGreenExposure
                           //----- hist_h - cvRound(b_hist.at<float>(i)) = yBluexposure
                           //----- xExposure<70 & yGreenExposure > 205 & yGreenExposure > 215 => perfect ESPOSURE_ABSOLUTE value
                           //----- for find these perfect ESPOSURE_ABSOLUTE value
                           //----- if yBluexposure > 300 => down ESPOSURE_ABSOLUTE (2 unit at time)
                           //----- if yBluexposure < 300 => push Up ESPOSURE_ABSOLUTE (2 unit at time)
                           if  ((3 < i) && (i < 40)) 
                           {
                               yGreenExposure.push_back(cvRound(g_hist.at<float>(i)));
                               yBlueExposure.push_back(cvRound(b_hist.at<float>(i)));
                               greenCounter = cvRound(g_hist.at<float>(i));
                               blueCounter = cvRound(b_hist.at<float>(i));
                               //qDebug() << "pos. graph Green value: " << i;
                              // qDebug() << "Green value: " << greenCounter;
                              // qDebug() << "Blue Value: " << blueCounter;
                              // qDebug() << "-----------------------------------  " << i;
                           }

                       }

                       BlueLevel = *std::max_element(yBlueExposure.begin(), yBlueExposure.end());
                       GreenRange = *std::max_element(yGreenExposure.begin(), yGreenExposure.end());
                       
                       /// Display
                        if(grphRGB_sel) /*if a gui flag is selected*/
                        {
                            namedWindow("Diagramma R G B", CV_WINDOW_AUTOSIZE );
                            imshow("Diagramma R G B", histImage );
                        }




               //************ PID control of exposure_abs (only Proportional part) baser on G channel of hist **********//
               //************ limit min value to 10 and disable value during calculation (if thexposure != exposure_abs not make calculation\n
               //************ make calculation in next loop when thexposure == exposure_abs
               if((!(ex_exposure_abs < 10)) && (thExposure == ex_exposure_abs))
               {
                   ex_exposure_abs_try = ex_exposure_abs;


                   RgbPidKp = 0.6;      //******* Kp
                   RgbPidKi = 0.01;     //******* Ki for furter implementation of Integral
                   RgbPidKgreen = 160;  //******* Targhet Value
                   pidTemp = 0;         //******* temp variable

                   RgbPidP = RgbPidKp*(GreenRange-RgbPidKgreen);
                   if(RgbPidP < 0){RgbPidP = RgbPidP*-1;}
                   pidTemp = round(RgbPidP);
                   if(GreenRange < 160) {ex_exposure_abs_try = ex_exposure_abs_try - pidTemp;}
                   else{ex_exposure_abs_try = ex_exposure_abs_try + pidTemp;}

                   if (ex_exposure_abs_try < 0) {ex_exposure_abs_try = -1*ex_exposure_abs_try;}
                   if(ex_exposure_abs_try < 10) {thExposure = ex_exposure_abs;}
                   else{ex_exposure_abs = ex_exposure_abs_try;}
               }
               else{thExposure = ex_exposure_abs;}

               //qDebug() << "***************************************  ";
               //qDebug() << "RgbPidP:  " << RgbPidP;
               //qDebug() << "pidTemp:  " << pidTemp;
               //qDebug() << "ex_exposure_abs:  " << ex_exposure_abs;
               //qDebug() << "Blue Max Level:  " << BlueLevel;
               //qDebug() << "Green Max Level:  " << GreenRange;
               //qDebug() << "***************************************  ";

            }



                 cicleCalculator = 0;
           }
