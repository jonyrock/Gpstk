/**************************************************************************/
/* This program reads in clock phase data provided by ordGen and ordClock */
/*  and outputs the data in the format required by oallandev and          */
/*  ohadamarddev                                                          */
/*                                                                        */
/* Written by Tim Craddock                                                */
/* Last Updated March 18, 2008                                            */
/**************************************************************************/

#include <iostream>

#include <stdio.h>
#include <string.h>

using namespace std;

int main()
{
	char line[512]; // stores input line
	
	char year[128]; // stores parsed data from line
	char day[128];
	char time[128];
	char type[128];
	char data[256];
	
	char * pch;  // used in tokenizing string
	int i, point; // used in itterating and calculating data point times
	
	char time1[128], time2[128];            // used in calculating tinterval, the time interval between data points
	char t1hr[128], t1min[128], t1sec[128];
	char t2hr[128], t2min[128], t2sec[128];
	double t1time, t2time;
	double tinterval = 1.0;
	bool time1Set, time2Set, tintSet;
	
	double adata; // stores phase data in numerical format
	
	point = 0; // ensure we start at data point zero
	time1Set = time2Set = tintSet = false; // and that we have no time information to calculate tinterval with yet
	while(!feof(stdin))
	{
		cin.getline(line, 512); // get a data line
		
		strcpy(type,"NA");
		
		if(line[0] != '#') // make sure its not a comment
		{
		
			// tokenize data line
			pch = strtok (line, " ");
			
			for(i = 0; (i < 5) && (pch != NULL); i++)
			{	
				switch(i)
				{
					case 0: strcpy(year, pch); break; // get year
					case 1: strcpy(day, pch); break;  // get day
					case 2: strcpy(time, pch); break; // get time
					case 3: strcpy(type, pch); break; // get type
					case 4: strcpy(data, pch); break; // get data
				}
				
				pch = strtok (NULL, " ");
			}
			
			// convert data from meters to seconds
			adata = atof(data);
			//divide by speed of light
			adata /= 299792458.0;
			
			// if it is a clock estimate (type 50)
			if(!strcmp(type, "50"))
			{
				if(!time1Set && !time2Set) // get time of 1st point, if it has not been found
				{
					strcpy(time1, time);
					time1Set = true;
				}else if(time1Set && !time2Set) // get time of 2nd point, if it has not been found
				{
					strcpy(time2, time);
					time2Set = true;
				}
				
				if(time2Set && !tintSet) // calculate tinterval, if it has not been found
				{
					// parse first time
					pch = strtok (time1, ":");
			
					for(i = 0; (i < 3) && (pch != NULL); i++)
					{	
						switch(i)
						{
							case 0: strcpy(t1hr, pch); break;
							case 1: strcpy(t1min, pch); break;
							case 2: strcpy(t1sec, pch); break;
						}
				
						pch = strtok (NULL, ":");
					}
					
					// parse second time
					pch = strtok (time2, ":");
			
					for(i = 0; (i < 3) && (pch != NULL); i++)
					{	
						switch(i)
						{
							case 0: strcpy(t2hr, pch); break;
							case 1: strcpy(t2min, pch); break;
							case 2: strcpy(t2sec, pch); break;
						}
				
						pch = strtok (NULL, ":");
					}
					
					// calculate time1 and time2 in seconds
					t1time = atof(t1hr)*3600.0 + atof(t1min)*60.0 + atof(t1sec);
					t2time = atof(t2hr)*3600.0 + atof(t2min)*60.0 + atof(t2sec);
					
					// calculate tinterval
					if(t1time > t2time) tinterval = t1time - t2time;
					else tinterval = t2time - t1time;
					tintSet = true;
				}
				
				// spit out data in proper format for use by Ghetto32
				fprintf(stdout, "%.1f %.25f\n", tinterval*(double)point, adata);
				point++;
			}
			
		}
		
	}
	
	return(0);
}
